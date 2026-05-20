#include "zigbee.h"
#include "mac_driver_interface.h"
#include "mac_driver_mpan.h"
#include "mac_driver.h"
#include "vector_table.h"
#include "syncedproto.h"
#include "power_manager_unit_zbmac.h"
#include "boot_screen.h"
#include <os_sync.h>
#include <os_sched.h>
#include <os_mem.h>
#include <rtl876x_wdg.h>
#include <string.h>
#include <stdio.h>

extern volatile uint32_t g_dlps_enter_count;

/*============================================================================*
 * Constants
 *============================================================================*/
#define ZIGBEE_IRQn 9
#define PROTO_PAN_ID 0x4447u
#define TX_TIMEOUT_US 50000u
#define PHY_GRANT_TIMEOUT_US 1000000u

#define RX_BUF_SLOTS 4u
#define RX_BUF_MASK (RX_BUF_SLOTS - 1u)
#define RX_RAW_SIZE 136u

typedef struct
{
    uint8_t raw[RX_RAW_SIZE];
} rx_entry_t;

/*============================================================================*
 * Public data
 *============================================================================*/
uint8_t channelList[6] = {11, 15, 20, 25, 26, 27};

/*============================================================================*
 * Global state
 *============================================================================*/
static rx_entry_t s_rx_buf[RX_BUF_SLOTS];
static volatile uint8_t s_rx_wr = 0;
static volatile uint8_t s_rx_rd = 0;

static mac_attribute_t s_mac_attr;
static mac_driver_t s_mac_drv;
static pan_mac_comm_t s_pan_comm;

static volatile bool s_tx_done = true;
static volatile bool s_rx_on = false;
static bool s_mac_init = false;
static uint8_t s_channel = 11;

static zbpm_adapter_t s_zbpm_adap;
static uint8_t *s_mac_retention_buf = NULL;

static zbpm_callback_t s_orig_exit_cb = NULL;

static void zbpm_exit_wrapper(void)
{
    mac_enable();
    if (s_orig_exit_cb)
        s_orig_exit_cb();
}

/*============================================================================*
 * External symbols
 *============================================================================*/
extern void Zigbee_Handler_Patch(void);
extern uint32_t (*lowerstack_SystemCall)(uint32_t, uint32_t, uint32_t, uint32_t);
extern uint8_t mSelfMac[8];

/*============================================================================*
 * MAC ISR callbacks
 *============================================================================*/
static void txn_isr(uint8_t pan, uint32_t arg)
{
    (void)pan;
    (void)arg;
    s_tx_done = true;
}
static void rxdone_isr(uint8_t pan, uint32_t arg)
{
    (void)pan;
    (void)arg;
    uint8_t next = (s_rx_wr + 1u) & RX_BUF_MASK;
    if (next == s_rx_rd)
    {
        uint8_t tmp[RX_RAW_SIZE];
        mac_rx(tmp);
        return;
    }
    if (mac_rx(s_rx_buf[s_rx_wr].raw) == 0)
        s_rx_wr = next;
}

/*============================================================================*
 * Radio HAL Internal Helpers
 *============================================================================*/
static void show_phy_error_and_reset(const char *reason)
{
    /*uint32_t wakeup_cnt, last_wakeup, last_sleep;
    platform_pm_get_statistics(&wakeup_cnt, &last_wakeup, &last_sleep);
    char l0[32], l1[40], l2[40];
    snprintf(l0, sizeof(l0), "%s", reason);
    snprintf(l1, sizeof(l1), "dlps_cb=%u tot=%u",
             (unsigned)g_dlps_enter_count, (unsigned)wakeup_cnt);
    snprintf(l2, sizeof(l2), "pm_err=%u zb_err=%u",
             (unsigned)platform_pm_get_error_code(),
             (unsigned)radioGetZbpmError());
    const char *lines[] = {l0, l1, l2};
    boot_screen_show_error(lines, 3);*/
    uint8_t chIdx = 0x0F;
    for (uint8_t i = 0; i < sizeof(channelList); i++)
    {
        if (channelList[i] == currentChannel)
        {
            chIdx = i;
            break;
        }
    }
    WDG_SystemReset(RESET_ALL, 0xE0 | chIdx);
}

static void radio_hw_apply_config(void)
{
    uint32_t t = mac_btus_get();
    uint32_t prev_ts = t;
    uint32_t stuck_loops = 0;

    while (!mac_GrantPHYStatus())
    {
        uint32_t now = mac_btus_get();

        if (now == prev_ts)
        {
            if (++stuck_loops >= 1000u)
            {
                printf("PHY timer stuck!\r\n");
                show_phy_error_and_reset("PHY timer stuck!");
            }
        }
        else
        {
            stuck_loops = 0;
            prev_ts = now;
        }

        if ((uint32_t)(now - t) >= PHY_GRANT_TIMEOUT_US)
        {
            printf("PHY grant timeout!\r\n");
            show_phy_error_and_reset("PHY grant timeout!");
        }
    }
    mac_channel_set(s_channel);
}
/*============================================================================*
 * Public Radio HAL
 *============================================================================*/
bool radioInit(void)
{
    if (s_mac_init)
        return true;

    memset(s_rx_buf, 0, sizeof(s_rx_buf));
    memset(&s_mac_attr, 0, sizeof(s_mac_attr));
    memset(&s_mac_drv, 0, sizeof(s_mac_drv));
    memset(&s_pan_comm, 0, sizeof(s_pan_comm));

    mac_enable();
    mac_attribute_init(&s_mac_attr);
    mac_init(&s_mac_drv, &s_mac_attr);
    mac_init_ext();

    lowerstack_SystemCall(10, 1, 512, -1);
    mac_panid_set(PROTO_PAN_ID);
    mac_short_addr_set(0xFFFF);
    mac_long_addr_set(mSelfMac);
    mac_promiscuous_set(1);
    radio_hw_apply_config();
    mpan_CommonInit(&s_pan_comm);
    mpan_RegisterISR(0, txn_isr, NULL, rxdone_isr, NULL);
    mpan_EnableCtl(0, 1);
    RamVectorTableUpdate(ZIGBEE_VECTORn, Zigbee_Handler_Patch);
    NVIC_SetPriority((IRQn_Type)ZIGBEE_IRQn, 2);
    NVIC_EnableIRQ((IRQn_Type)ZIGBEE_IRQn);
    mac_radio_on();

    /* Prepare Register Retention to survive DLPS without driver re-init */
    uint32_t ret_size = mac_adapter.get_retention_reg_size();
    if (ret_size > 0)
    {
        s_mac_retention_buf = os_mem_alloc(RAM_TYPE_DATA_ON, ret_size);
    }

    memset(&s_zbpm_adap, 0, sizeof(s_zbpm_adap));
    s_zbpm_adap.power_mode = ZBMAC_ACTIVE;
    s_zbpm_adap.stage_time[ZBMAC_PM_CHECK] = 20;
    s_zbpm_adap.stage_time[ZBMAC_PM_STORE] = 15;
    s_zbpm_adap.stage_time[ZBMAC_PM_ENTER] = 5;
    s_zbpm_adap.stage_time[ZBMAC_PM_EXIT] = 5;
    s_zbpm_adap.stage_time[ZBMAC_PM_RESTORE] = 20;
    s_zbpm_adap.minimum_sleep_time = 20;
    s_zbpm_adap.learning_guard_time = 7;
    s_zbpm_adap.wakeup_time_us = mac_btus_get();
    s_zbpm_adap.pretain_buf = s_mac_retention_buf;
    // s_zbpm_adap.exit_callback_app = zbpm_exit_wrapper;

    zbmac_pm_init(&s_zbpm_adap);
    s_orig_exit_cb = s_zbpm_adap.exit_callback;
    s_zbpm_adap.exit_callback = zbpm_exit_wrapper;

    s_mac_init = true;
    s_rx_on = true;
    printf("radioInit done)\r\n");
    return true;
}

bool radioSetChannel(uint_fast8_t channel)
{
    if ((uint8_t)channel == s_channel && mac_radio_state_get() == MAC_RADIO_STATE_RX)
        return true;
    s_channel = (uint8_t)channel;
    mac_channel_set(s_channel);
    return true;
}

bool radioRxEnable(bool on)
{
    radioRxFlush();
    s_rx_on = on;
    return true;
}
void radioRxFlush(void) { s_rx_rd = s_rx_wr; }

int32_t radioRxDequeuePkt(uint8_t *dstBuf, uint32_t maxLen, int8_t *rssiP, uint8_t *lqiP)
{
    if (s_rx_rd == s_rx_wr)
        return -1;
    rx_entry_t *e = &s_rx_buf[s_rx_rd];
    uint8_t flen = e->raw[0];
    if (flen == 0 || flen > 126u || (uint32_t)flen > maxLen)
    {
        s_rx_rd = (s_rx_rd + 1u) & RX_BUF_MASK;
        return -1;
    }
    if (lqiP)
        *lqiP = e->raw[1u + flen];
    if (rssiP)
    {
        uint16_t rssi_raw = (uint16_t)e->raw[2u + flen] | ((uint16_t)e->raw[3u + flen] << 8);
        *rssiP = mac_GetRSSIFromRaw(rssi_raw, s_channel);
    }
    memcpy(dstBuf, &e->raw[1], flen);
    s_rx_rd = (s_rx_rd + 1u) & RX_BUF_MASK;
    return (int32_t)flen;
}

bool radioTxLL(uint8_t *pkt)
{
    if (pkt[0] < 3u)
        return false;
    uint8_t frame_len = pkt[0] - 2u;
    s_tx_done = false;
    mac_txn_payload_set(0, frame_len, &pkt[1]);
    mac_txn_trig(0, 0);
    uint32_t t = mac_btus_get();
    while (!s_tx_done && (uint32_t)(mac_btus_get() - t) < TX_TIMEOUT_US)
    {
    }
    return s_tx_done;
}

void radioSleep(uint32_t sleep_ms)
{
    s_zbpm_adap.cfg.wake_interval_en = 0;
    s_zbpm_adap.wakeup_interval_us = 0;
    s_zbpm_adap.wakeup_reason = ZBMAC_PM_WAKEUP_UNKNOWN;
    s_zbpm_adap.error_code = ZBMAC_PM_ERROR_UNKNOWN;
    uint32_t wakeup_us = (uint32_t)((uint64_t)sleep_ms * 1000u);
    s_zbpm_adap.wakeup_time_us = mac_btus_get() + wakeup_us;
    s_zbpm_adap.power_mode = ZBMAC_DEEP_SLEEP;
    s_rx_on = false;
}

uint8_t radioGetZbpmError(void)
{
    return (uint8_t)s_zbpm_adap.error_code;
}

void radioWake(void)
{
    mac_enable();
    s_zbpm_adap.power_mode = ZBMAC_ACTIVE;
    radio_hw_apply_config();
    mac_radio_on();
    s_rx_on = true;
}

uint32_t clock_time(void) { return mac_btus_get(); }
bool clock_time_exceed(uint32_t ref, uint32_t us) { return (uint32_t)(mac_btus_get() - ref) >= us; }
void WaitUs(uint32_t us)
{
    uint32_t t = clock_time();
    while (!clock_time_exceed(t, us))
        ;
}

void WaitMs(uint32_t ms)
{
    if (ms == 0)
        return;
    os_delay(ms);
}
