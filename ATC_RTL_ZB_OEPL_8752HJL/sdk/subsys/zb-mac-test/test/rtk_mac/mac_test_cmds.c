/**
*****************************************************************************************
*     Copyright(c) 2017, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
   * @file      main_test_cmds.c
   * @brief     Source file for IEEE 802.15.4 MAC test command
   * @author    jane
   * @date      2017-06-12
   * @version   v1.0
   **************************************************************************************
   * @attention
   * <h2><center>&copy; COPYRIGHT 2017 Realtek Semiconductor Corporation</center></h2>
   **************************************************************************************
  */

/*============================================================================*
*                              Header Files
*============================================================================*/
#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <os_sync.h>
#include "shell.h"
#include "dbg_printf.h"
#include "trace.h"

#ifdef CONFIG_SOC_SERIES_RTL87X2G
//#include "rtl_gpio.h"
#include "rtl_nvic.h"
#include "rtl_tim.h"
#include "wdt.h"
#else
//#include "rtl876x_gpio.h"
#include "rtl876x_nvic.h"
#include "rtl876x_tim.h"
#include "rtl876x_wdg.h"
#endif

#include "mac_driver_interface.h"
#if defined(CONFIG_SOC_SERIES_RTL87X2G) || defined(CONFIG_SOC_SERIES_RTL87X2H)
#include "power_manager_unit_zbmac.h"
#include "power_manager_interface.h"
#endif
#include "zb_tst_cfg.h"
#include "strproc.h"
#include "patch.h"
#include "mac_802154_frame_parser.h"
#include "mac_test_common.h"

//++++++++++++++++++++++++++++++++++++++++++++++++
// type and macro define
//------------------------------------------------

//++++++++++++++++++++++++++++++++++++++++++++++++
// global variable
//------------------------------------------------
volatile uint32_t g_tx_done = TX_NONE;
volatile bool g_ed_scan_done = TRUE;
volatile bool g_tx_loop_state = TX_LOOP_STOP;
void *g_ping_reply_sequence_mbx = NULL;

tx_buf_t g_tx_buf;
uint32_t g_tx_interval_ms = 1;
bool g_enh_ack_early = TRUE;
int8_t g_peak_ed_level;
int8_t g_avrg_ed_level;
bool g_test_enh_ack_late = FALSE;

uint8_t ADDR_MODE2LEN[ADDR_MODE_MAX] = {0, 0, 2, 8};

uint8_t g_mac_key[16] =
{
    0xc0, 0xc1, 0xc2, 0xc3, 0xc4, 0xc5, 0xc6, 0xc7,
    0xc8, 0xc9, 0xca, 0xcb, 0xcc, 0xcd, 0xce, 0xcf
};
//++++++++++++++++++++++++++++++++++++++++++++++++
// private function
//------------------------------------------------

//++++++++++++++++++++++++++++++++++++++++++++++++
// test command
//------------------------------------------------
static int cmd_mac_init(int argc, char *argv[])
{
    zb_mac_drv_init();
#if Auto_test
    unsigned char bytes[] = {0x04, 0x0e, 0x04, 0x02, 0x00, 0xfc, 0x00};
    UART_SendData(UART2, bytes, sizeof(bytes));
#endif
    dbg_printf("MAC Init Done. Now %u\r\n", mac_btus_get());
    return TRUE;
}

static int cmd_mac_enable(int argc, char *argv[])
{
    if (argc == 1)
    {
        if (argv[0][0] == '1')
        {
            mac_enable();
        }
        else if (argv[0][0] == '0')
        {
            mac_disable();
        }
        else
        {
            goto prf;
        }
        dbg_printf("Done\r\n");
        return TRUE;
    }
prf:
    dbg_printf("mac_IsEnabled %u\r\n", mac_enabled_check());
    return TRUE;
}

static int cmd_mac_disable(int argc, char *argv[])
{
    mac_RadioOff();
    mac_disable();
    dbg_printf("Done\r\n");
    return TRUE;
}

static int cmd_reset(int argc, char *argv[])
{
#if defined(CONFIG_SOC_SERIES_RTL87X2G) || defined(CONFIG_SOC_SERIES_RTL87X2H)
    WDG_SystemReset(RESET_ALL, SW_RESET_APP_END);
#else
    WDG_SystemReset(RESET_ALL);
#endif
    return TRUE;
}

#if defined(CONFIG_SOC_SERIES_RTL87X2G) || defined(CONFIG_SOC_SERIES_RTL87X2H)
static int cmd_radio(int argc, char *argv[])
{
    if (argc == 2)
    {
        bool on = _strtoul((const char *)(argv[1]), (char **)NULL, 10);
        if (strcmp(argv[0], "bt") == 0)
        {
            if (on)
            {
                PMUnitStatus state = power_manager_interface_get_unit_status(PM_SLAVE_BTMAC, PM_UNIT_BTMAC);
                dbg_printf("BT_PM state %u\r\n", state);
                power_manager_interface_check_unit_active(PM_SLAVE_BTMAC, PM_UNIT_BTMAC);
            }
        }
        else if (strcmp(argv[0], "rf") == 0)
        {
            if (on)
            {
                mac_radio_on();
            }
            else
            {
                mac_radio_off();
            }
        }
    }
    PMUnitStatus state = power_manager_interface_get_unit_status(PM_SLAVE_BTMAC, PM_UNIT_BTMAC);
    dbg_printf("BT_PM state %u (rd only)\r\n", state);
    dbg_printf("radio state %u (rd only)\r\n", mac_radio_state_get());
    return TRUE;
}
#else
static int cmd_radio(int argc, char *argv[])
{
    return TRUE;
}
#endif

void zbpm_exit(void)
{
    dbg_printf("exit_pm = %u pm_wakeup_diff(min/avg/max) = %d/%d/%d us\r\n", mac_btus_get(),
               g_zbpm_wakeup_diff_min >> 7, g_zbpm_wakeup_diff_avg >> 7, g_zbpm_wakeup_diff_max >> 7);
}

#if defined(CONFIG_SOC_SERIES_RTL87X2G) || defined(CONFIG_SOC_SERIES_RTL87X2H)
static int cmd_pm(int argc, char *argv[])
{
    uint32_t next = 1000000;
    uint32_t period = 0;

    switch (argc)
    {
    case 0:
        zbmac_power_manager_init(zbpm_exit);
        dbg_printf("Done\r\n");
        return TRUE;
    case 2:
        period = _strtoul((const char *)(argv[1]), (char **)NULL, 10);
    case 1:
        next = _strtoul((const char *)(argv[0]), (char **)NULL, 10);
        break;
    default:
        dbg_printf("Error: InvalidArgs\r\n");
        return FALSE;
    }

    do
    {
        dbg_printf("entr_pm = %u\r\n", mac_btus_get());
        if (zbmac_power_manager_set(next, 0) == FALSE)
        {
            dbg_printf("entr pm fail\r\n");
        }
    }
    while (period);
    return TRUE;
}
#else
static int cmd_pm(int argc, char *argv[])
{
    return TRUE;
}
#endif

static mac_timer_handle_t g_swtimer_pool;
static mac_timer_handle_t *g_swtimer;
static void swtimer_cb(void *arg)
{
    uint32_t now = mac_btus_get();
    dbg_printf("fire %u\r\n", now);
}

static int cmd_swtimer(int argc, char *argv[])
{
    uint32_t timeout;

    if (argc)
    {
        timeout = _strtoul((const char *)(argv[0]), (char **)NULL, 10);
        uint32_t now = mac_btus_get();
        mac_sw_timer_start(g_swtimer, now + timeout, swtimer_cb, (void *)timeout);
        dbg_printf("swtimer start: now %u timeout %u us\r\n", now, timeout);
    }
    else
    {
        dbg_printf("swtimer init\r\n");
        mac_sw_timer_init(&g_swtimer_pool, 1);
        g_swtimer = mac_sw_timer_alloc();
        //mac_sw_timer_stop(g_swtimer);
    }
    dbg_printf("Done\r\n");
    return TRUE;
}

static int cmd_bttimer(int argc, char *argv[])
{
    uint32_t timeout;

    if (argc == 1)
    {
        timeout = _strtoul((const char *)(argv[0]), (char **)NULL, 10);
        uint32_t now = mac_btus_get();
        dbg_printf("bttimer start: now %u timeout %u us\r\n", now, timeout);
        mac_btus_intr_set(MAC_BT_TIMER0, mac_btus_get() + timeout);
        os_sem_take(zb_sem, 0xffffffff);
        dbg_printf("fire %u us\r\n", mac_btus_get());
        dbg_printf("Done\r\n");
    }
    else
    {
        dbg_printf("Error: InvalidArgs\r\n");
    }
    return TRUE;
}

#if ENABLE_BLE
#include "gap_msg.h"
#include "gap_adv.h"
extern void ble_peripheral_init(void);
static int cmd_ble_adv(uint32_t argc, uint8_t  *argv[])
{
    static bool g_ble_peripheral_init = FALSE;
    T_GAP_DEV_STATE state;
    bool enable;
    switch (argc)
    {
    case 1:
        enable = _strtoul((const char *)(argv[0]), (char **)NULL, 10);
        if (enable)
        {
            if (g_ble_peripheral_init == FALSE)
            {
                ble_peripheral_init();
                g_ble_peripheral_init = TRUE;
            }
            else
            {
                le_adv_start();
            }
        }
        else
        {
            le_adv_stop();
        }
    case 0:
        le_get_gap_param(GAP_PARAM_DEV_STATE, &state);
        dbg_printf("ble_adv %u\r\n", (state.gap_adv_state == GAP_ADV_STATE_START ||
                                      state.gap_adv_state == GAP_ADV_STATE_ADVERTISING) ? 1 : 0);
        /*
        #define GAP_ADV_STATE_IDLE           0   //!< Idle, no advertising
        #define GAP_ADV_STATE_START          1   //!< Start Advertising. A temporary state, haven't received the result.
        #define GAP_ADV_STATE_ADVERTISING    2   //!< Advertising
        #define GAP_ADV_STATE_STOP           3   //!< Stop Advertising. A temporary state, haven't received the result.
        */
        break;
    default:
        dbg_printf("Error: InvalidArgs\r\n");
        return FALSE;
    }
    return TRUE;
}
#endif /* ENABLE_BLE */
static int cmd_upper_sec_test(int argc, char *argv[])
{
    nonce_t nonce = {0};
    uint8_t data_len = MAC_MAX_TX_FRM_LEN;
    uint8_t sec_level = SEC_ENC;
    uint8_t unnecessary_enc_len = 0;
    pmac_txfifo_t pTxNFIFO = (pmac_txfifo_t)MAC_TXN_BASE_ADDR;
    //tx_buf_t ciphertext_buf;

    if (argc > 0)
    {
        data_len = _strtoul((const char *)(argv[0]), (char **)NULL, 10);
    }

    //if (argc > 1)
    //    sec_level = _strtoul((const char *)(argv[1]), (char **)NULL, 10);

    if ((data_len > MAC_MAX_TX_FRM_LEN || data_len < MAC_MIN_TX_FRM_LEN) ||
        (sec_level > SEC_ENC_MIC_128))
    {
        dbg_printf("Error: InvalidArgs\r\n");
        return FALSE;
    }

    dbg_printf("Start enc/dec test. data_len %u sec_level %u\r\n", data_len, sec_level);
    dbg_printf("Generate sequential test data\r\n");
    dbg_printf("plaintext:\r\n");
    BUF_RESET(g_tx_buf);
    GEN_SEQ_DATA_MV_PTR(g_tx_buf.buf, 1, data_len, g_tx_buf.len);
    dbg_mem_dump((const uint8_t *)g_tx_buf.buf, g_tx_buf.len);
    // load plaintext data
    mac_txn_payload_set(unnecessary_enc_len, g_tx_buf.len, g_tx_buf.buf);
    // load nonce
    nonce.sec_level = sec_level;
    nonce.frame_counter = 5;
    memcpy(&nonce.src_ext_addr, mac_long_addr_get(), sizeof(nonce.src_ext_addr));
    mac_nonce_set((uint8_t *)&nonce);
    // load key
    mac_txn_key_set(g_mac_key);
    // set cipher mode
    mac_txn_cipher_set(nonce.sec_level);
    // do encrypt
    mac_upper_enc_trig();

    // store encrypted data with MIC
    uint8_t mic_len = (nonce.sec_level % 4 == 0) ? 0 : (4 << ((nonce.sec_level % 4) - 1));
    //BUF_RESET(ciphertext_buf);
    //CPY_MV_PTR(ciphertext_buf.buf, pTxNFIFO->payload, pTxNFIFO->frm_len, ciphertext_buf.len);
    dbg_printf("\r\ncipher text: %s", mic_len == 0 ? "\r\n" : "MIC-");
    dbg_mem_dump(pTxNFIFO->payload + pTxNFIFO->frm_len - mic_len, mic_len);
    dbg_mem_dump(pTxNFIFO->payload, pTxNFIFO->frm_len - mic_len);

    // load ciphertext data
    // mac_txn_payload_set(unnecessary_enc_len, ciphertext_buf.len, ciphertext_buf.buf);
    // do decrypt
    mac_upper_dec_trig();
    //uint8_t ret = mac_UpperDecipher(nonce.sec_level, g_mac_key, (uint8_t *)&nonce);
    //dbg_printf("mac_UpperDecipher %u\r\n", ret);

    dbg_printf("\r\nplain text: %s", mic_len == 0 ? "\r\n" : "MIC-");
    dbg_mem_dump(pTxNFIFO->payload + pTxNFIFO->frm_len - mic_len, mic_len);
    dbg_mem_dump(pTxNFIFO->payload, pTxNFIFO->frm_len - mic_len);

    if (memcmp(g_tx_buf.buf, pTxNFIFO->payload, g_tx_buf.len))
    {
        dbg_printf("Fail\r\n");
        return FALSE;
    }
#if Auto_test
    txl_report(NV_FIELD, NV_FIELD);
#endif
    dbg_printf("Done\r\n");
    return TRUE;
}

static int cmd_panid(int argc, char *argv[])
{
    uint16_t panid = 0;
    switch (argc)
    {
    case 1:
        panid = _strtoul((const char *)(argv[0]), (char **)NULL, 16);
        mac_panid_set(panid);
#if Auto_test
        txl_report(NV_FIELD, NV_FIELD);
#endif
    case 0:
        dbg_printf("panid 0x%04x\r\n", mac_panid_get());
        break;
    default:
        dbg_printf("Error: InvalidArgs\r\n");
        return FALSE;
    }
    //dbg_printf("Done\r\n");
    return TRUE;
}

static int cmd_channel(int argc, char *argv[])
{
    uint8_t ch = 0;
    switch (argc)
    {
    case 1:
        ch = _strtoul((const char *)(argv[0]), (char **)NULL, 10);
        mac_channel_set(ch);
#if Auto_test
        txl_report(NV_FIELD, NV_FIELD);
#endif
    case 0:
        dbg_printf("channel %u\r\n", mac_channel_get());
        break;
    default:
        dbg_printf("Error: InvalidArgs\r\n");
        return FALSE;
    }
    //dbg_printf("Done\r\n");
    return TRUE;
}

static int cmd_freq(int argc, char *argv[])
{
    uint16_t freq;
    switch (argc)
    {
    case 1:
        freq = _strtoul((const char *)(argv[0]), (char **)NULL, 10);
        mac_freq_set(freq);
    case 0:
        dbg_printf("freq = %u MHz\r\n", mac_freq_get());
        break;
    default:
        dbg_printf("Error: InvalidArgs\r\n");
        return FALSE;
    }
    //dbg_printf("Done\r\n");
    return TRUE;
}

static int cmd_shortaddr(int argc, char *argv[])
{
    uint16_t saddr = 0;
    switch (argc)
    {
    case 1:
        saddr = _strtoul((const char *)(argv[0]), (char **)NULL, 16);
        mac_short_addr_set(saddr);
#if Auto_test
        txl_report(NV_FIELD, NV_FIELD);
#endif
    case 0:
        dbg_printf("shortaddr 0x%04x\r\n", mac_short_addr_get());
        break;
    default:
        dbg_printf("Error: InvalidArgs\r\n");
        return FALSE;
    }
    //dbg_printf("Done\r\n");
    return TRUE;
}

static int cmd_extaddr(int argc, char *argv[])
{
    uint64_t laddr = 0;
    switch (argc)
    {
    case 1:
        laddr = _strtoull((const char *)(argv[0]), (char **)NULL, 16);
        mac_SetLongAddress((uint8_t *)&laddr);
#if Auto_test
        txl_report(NV_FIELD, NV_FIELD);
#endif
    case 0:
        memcpy(&laddr, mac_long_addr_get(), sizeof(laddr));
        dbg_printf("extaddr 0x%016llx\r\n", laddr);
        break;
    default:
        dbg_printf("Error: InvalidArgs\r\n");
        return FALSE;
    }
    //dbg_printf("Done\r\n");
    return TRUE;
}

static int cmd_mackey(int argc, char *argv[])
{
    int i;
    char *tmp;

    switch (argc)
    {
    case 1:
        if (strlen(argv[0]) != (sizeof(g_mac_key) << 1))
        {
            dbg_printf("Error: InvalidArgs\r\n");
            return FALSE;
        }

        tmp = argv[0];
        for (i = 0; i < sizeof(g_mac_key); i++)
        {
            g_mac_key[i] = (parse_digit(tmp[i << 1]) << 4) + parse_digit(tmp[(i << 1) + 1]);
        }
#if Auto_test
        txl_report(NV_FIELD, NV_FIELD);
#endif
    case 0:
        dbg_printf("mackey ");
        for (i = 0; i < 16; i++)
        {
            dbg_printf("%02x", g_mac_key[i]);
        }
        dbg_printf("\r\n");
        break;
    default:
        dbg_printf("Error: InvalidArgs\r\n");
        return FALSE;
    }

    //dbg_printf("Done\r\n");
    return TRUE;
}

static int cmd_ccamode(int argc, char *argv[])
{
    uint8_t mode;
    uint8_t th = 0;

    switch (argc)
    {
    case 2:
        th = _strtoul((const char *)(argv[1]), (char **)NULL, 10);
    case 1:
        mode = _strtoul((const char *)(argv[0]), (char **)NULL, 10);
        if (mode > MAC_CCA_CS_ED_AND)
        {
            dbg_printf("Error: InvalidArgs\r\n");
            return FALSE;
        }
        mac_cca_mode_set(mode);
        if (argc == 2 && (mode == MAC_CCA_ED || mode == MAC_CCA_CS_ED || mode == MAC_CCA_CS_ED_AND))
        {
            mac_cca_ed_threshold_set(th);
        }
    case 0:
        mode = mac_cca_mode_get();
        dbg_printf("ccamode %u\r\n", mode);
        if (mode == MAC_CCA_ED || mode == MAC_CCA_CS_ED || mode == MAC_CCA_CS_ED_AND)
        {
            dbg_printf("ED threshold %d\r\n", mac_cca_ed_threshold_get());
        }
        break;
    default:
        dbg_printf("Error: InvalidArgs\r\n");
        return FALSE;
    }
    //dbg_printf("Done\r\n");
    return TRUE;
}

static int cmd_csma(int argc, char *argv[])
{
    int8_t key = -1, expr;
    int enable = -1, minbe = -1, maxbe = -1, maxbo = -1;
    int value = 0;

    for (int i = 0; i < argc; i++)
    {
        if (key == 'e' || key == 'i' || key == 'a' || key == 'b')
        {
            value = _strtol((const char *)(argv[i]), (char **)NULL, 10);
            expr = key;
        }
        else
        {
            expr = argv[i][0];
            key = -1;
        }
        switch (expr)
        {
        case 'e':
            if (key == 'e')
            {
                enable = (value == 1) ? 1 : 0;
                key = -1;
            }
            else
            {
                key = 'e';
            }
            break;
        case 'i':
            if (key == 'i')
            {
                minbe = value;
                key = -1;
            }
            else
            {
                key = 'i';
            }
            break;
        case 'a':
            if (key == 'a')
            {
                maxbe = value;
                key = -1;
            }
            else
            {
                key = 'a';
            }
            break;
        case 'b':
            if (key == 'b')
            {
                maxbo = value;
                key = -1;
            }
            else
            {
                key = 'b';
            }
            break;
        default:
            i = argc;
            key = '_';
            break;
        }
    }

    if (key != -1)
    {
        dbg_printf("Error: InvalidArgs\r\n");
        return FALSE;
    }

    if (enable != -1)
    {
        mac_txn_csma_set(enable);
    }
    if (minbe != -1)
    {
        mac_csma_minbe_set(minbe);
    }
    if (maxbe != -1)
    {
        mac_csma_maxbe_set(maxbe);
    }
    if (maxbo != -1)
    {
        mac_csma_max_backoffs_set(maxbo);
    }

    dbg_printf("csma.enable %u\r\n", mac_txn_csma_get());
    dbg_printf("csma.minBe %u\r\n", mac_csma_minbe_get());
    dbg_printf("csma.maxBe %u\r\n", mac_csma_maxbe_get());
    dbg_printf("csma.maxBackoff %u\r\n", mac_csma_max_backoffs_get());
    return TRUE;
}

static int cmd_txgain(int argc, char *argv[])
{
    uint8_t tx_gain = 0;
    switch (argc)
    {
    case 1:
        tx_gain = _strtoul((const char *)(argv[0]), (char **)NULL, 10);
        mac_tx_gain_set(tx_gain);
    case 0:
        dbg_printf("tx_gain %u\r\n", mac_tx_gain_get());
        break;
    default:
        dbg_printf("Error: InvalidArgs\r\n");
        return FALSE;
    }
#if Auto_test
    txl_report(NV_FIELD, NV_FIELD);
#endif
    //dbg_printf("Done\r\n");
    return TRUE;
}

static int cmd_notxcrc(int argc, char *argv[])
{
    uint8_t enable = 0;
    switch (argc)
    {
    case 1:
        enable = _strtoul((const char *)(argv[0]), (char **)NULL, 10);
        mac_txn_nocrc_set(enable ? 1 : 0);
    case 0:
        dbg_printf("notxcrc %u\r\n", mac_txn_nocrc_get());
        break;
    default:
        dbg_printf("Error: InvalidArgs\r\n");
        return FALSE;
    }
#if Auto_test
    txl_report(NV_FIELD, NV_FIELD);
#endif
    //dbg_printf("Done\r\n");
    return TRUE;
}

static int cmd_enh_ack_early(int argc, char *argv[])
{
    switch (argc)
    {
    case 1:
        g_enh_ack_early = _strtoul((const char *)(argv[0]), (char **)NULL, 10);
    case 0:
        dbg_printf("enh_ack_early %u\r\n", g_enh_ack_early ? 1 : 0);
        break;
    default:
        dbg_printf("Error: InvalidArgs\r\n");
        return FALSE;
    }
#if Auto_test
    unsigned char bytes[] = {0x04, 0x0e, 0x05, 0x02, 0x00, 0xfc, 0x01, g_enh_ack_early};
    UART_SendData(UART2, bytes, sizeof(bytes));
#endif
    //dbg_printf("Done\r\n");
    return TRUE;
}

static int cmd_tx_interval(int argc, char *argv[])
{
    switch (argc)
    {
    case 1:
        g_tx_interval_ms = _strtoul((const char *)(argv[0]), (char **)NULL, 10);
    case 0:
        dbg_printf("tx_interval %u ms\r\n", g_tx_interval_ms);
        break;
    default:
        dbg_printf("Error: InvalidArgs\r\n");
        return FALSE;
    }
    //dbg_printf("Done\r\n");
    return TRUE;
}

static int cmd_promiscuous(int argc, char *argv[])
{
    uint8_t enable = 0;
    switch (argc)
    {
    case 1:
        enable = _strtoul((const char *)(argv[0]), (char **)NULL, 10);
        mac_promiscuous_set(enable ? 1 : 0);
    case 0:
        dbg_printf("promiscuous %u\r\n", mac_promiscuous_get());
        break;
    default:
        dbg_printf("Error: InvalidArgs\r\n");
        return FALSE;
    }
#if Auto_test
    txl_report(NV_FIELD, NV_FIELD);
#endif
    //dbg_printf("Done\r\n");
    return TRUE;
}

static int cmd_recverrpkt(int argc, char *argv[])
{
    uint8_t enable = 0;
    switch (argc)
    {
    case 1:
        enable = _strtoul((const char *)(argv[0]), (char **)NULL, 10);
        mac_rx_err_pkt_set(enable ? 1 : 0);
    case 0:
        dbg_printf("recverrpkt %u\r\n", mac_rx_err_pkt_get());
        break;
    default:
        dbg_printf("Error: InvalidArgs\r\n");
        return FALSE;
    }
#if Auto_test
    txl_report(NV_FIELD, NV_FIELD);
#endif
    //dbg_printf("Done\r\n");
    return TRUE;
}

static int cmd_scanmode(uint32_t argc, uint8_t  *argv[])
{
    uint32_t mode;
    switch (argc)
    {
    case 1:
        mode = _strtoul((const char *)(argv[0]), (char **)NULL, 10);
        mac_scan_mode_set(mode);
    case 0:
        dbg_printf("scanmode %u\r\n", mac_scan_mode_get());
        break;
    default:
        dbg_printf("Error: InvalidArgs\r\n");
        return FALSE;
    }
    //dbg_printf("Done\r\n");
    return TRUE;
}

static int cmd_rxframetype(int argc, char *argv[])
{
    uint8_t rxftype = 0x0b;
    switch (argc)
    {
    case 1:
        if (argv[0][0] == 'b')
        {
            rxftype = 0x0b;
        }
        else if (argv[0][0] == 'f')
        {
            rxftype = 0x0f;
        }
        else
        {
            dbg_printf("Error: InvalidArgs\r\n");
            return FALSE;
        }
        mac_rx_frm_filter_set(FRAME_VER_2006, rxftype);
    case 0:
        dbg_printf("rxframetype %s\r\n",
                   (mac_rx_frm_filter_get(FRAME_VER_2006) & 0x4) ? "Include ACK" : "Default");
        break;
    default:
        dbg_printf("Error: InvalidArgs\r\n");
        return FALSE;
    }
#if Auto_test
    txl_report(NV_FIELD, NV_FIELD);
#endif
    //dbg_printf("Done\r\n");
    return TRUE;
}

static int cmd_srcmatchmode(int argc, char *argv[])
{
    uint8_t mode = 0;
    switch (argc)
    {
    case 1:
        mode = _strtoul((const char *)(argv[0]), (char **)NULL, 10);
        mac_addr_match_mode_set(mode);
    case 0:
        dbg_printf("srcmatchmode %u\r\n", mac_addr_match_mode_get());
        break;
    default:
        dbg_printf("Error: InvalidArgs\r\n");
        return FALSE;
    }
#if Auto_test
    txl_report(NV_FIELD, NV_FIELD);
#endif
    //dbg_printf("Done\r\n");
    return TRUE;
}

static int cmd_srcmatchfilter(int argc, char *argv[])
{
    uint16_t panid, saddr;
    uint64_t laddr;

    switch (argc)
    {
    case 0:
        {
            for (uint8_t i = 0; i < EXT_SRC_ADDR_MATCH_ENTRY_NUM; i++)
            {
                dbg_printf("SRC_ADDR_MATCH_ENTRY[%02u] ", i);
                dbg_printf("%08x ", mac_addr_match_entry_get(i, FALSE));
                dbg_printf("%08x ", mac_addr_match_entry_get(i, TRUE));
                dbg_printf("\r\n");
            }
        }
        break;
    case 1:
        laddr = _strtoull((const char *)(argv[0]), (char **)NULL, 16);
        mac_addr_match_long_add((uint8_t *)&laddr);
        dbg_printf("Done\r\n");
        break;
    case 2:
        saddr = _strtoul((const char *)(argv[0]), (char **)NULL, 16);
        panid = _strtoul((const char *)(argv[1]), (char **)NULL, 16);
        mac_addr_match_short_add(saddr, panid);
        dbg_printf("Done\r\n");
        break;
    default:
        dbg_printf("Error: InvalidArgs\r\n");
        return FALSE;
    }
#if Auto_test
    txl_report(NV_FIELD, NV_FIELD);
#endif
    return TRUE;
}

static int cmd_ifconfig(int argc, char *argv[])
{
    char c = 'a';

    if (argc > 1)
    {
        c = 'x';
    }
    if (argc == 1)
    {
        c = argv[0][0];
    }
    switch (c)
    {
    case 'c':
        memset(&g_ifrxstat, 0, sizeof(g_ifrxstat));
        memset(&g_iftxstat, 0, sizeof(g_iftxstat));
        break;
    case 'a':
    case 'r':
        dbg_printf("RX packets %u errors %u dropped %u\r\n",
                   g_ifrxstat.packets, g_ifrxstat.errors, g_ifrxstat.dropped);
#if Auto_test
        unsigned char bytes[] = {0x04, 0x0e, 0x05, 0x02, 0x00, 0xfc, 0x01, g_ifrxstat.packets};
        UART_SendData(UART2, bytes, sizeof(bytes));
#endif
        if (c == 'r')
        {
            break;
        }
    case 't':
        dbg_printf("TX packets %u errors %u dropped %u\r\n",
                   g_iftxstat.packets, g_iftxstat.errors, g_iftxstat.dropped);
        dbg_printf("TX noack %u collisions %u\r\n", g_iftxstat.noack, g_iftxstat.collisions);
        break;
    default:
        dbg_printf("Error: InvalidArgs\r\n");
        return FALSE;
    }
    return TRUE;
}

static int cmd_config_dump(int argc, char *argv[])
{
    dbg_printf("[Basic PAN Config]\r\n");
    cmd_panid(0, NULL);
    cmd_channel(0, NULL);
    cmd_freq(0, NULL);
    cmd_shortaddr(0, NULL);
    cmd_extaddr(0, NULL);
    cmd_mackey(0, NULL);
#if ENABLE_BLE
    cmd_ble_adv(0, NULL);
#endif /* ENABLE_BLE */
    cmd_radio(0, NULL);
#if F_BT_DLPS_EN
    dbg_printf("PLATFORM_DLPS 1 (rd only)\r\n");
#else
    dbg_printf("PLATFORM_DLPS 0 (rd only)\r\n");
#endif

    dbg_printf("\r\n[TX Config]\r\n");
    cmd_ccamode(0, NULL);
    cmd_csma(0, NULL);
    cmd_txgain(0, NULL);
    cmd_notxcrc(0, NULL);
    cmd_enh_ack_early(0, NULL);
    cmd_tx_interval(0, NULL);

    dbg_printf("\r\n[RX Filter]\r\n");
    cmd_promiscuous(0, NULL);
    cmd_recverrpkt(0, NULL);
    cmd_scanmode(0, NULL);
    cmd_rxframetype(0, NULL);
    cmd_srcmatchmode(0, NULL);
    cmd_srcmatchfilter(0, NULL);
    dbg_printf("Done\r\n");
    return TRUE;
}

static int cmd_data(int argc, char *argv[])
{
    uint16_t panid = mac_panid_get();
    uint16_t saddr = mac_short_addr_get();
    uint16_t daddr = 0;
    uint16_t data_len = 0;
    uint32_t delay_us = 0;
    uint32_t loop_cnt = 1;
    fc_t fc = {0};

    if (argc < 3)
    {
        dbg_printf("Error: InvalidArgs\r\n");
        return FALSE;
    }

    daddr = _strtoul((const char *)(argv[0]), (char **)NULL, 16);       // param 1: dest address
    data_len = _strtoul((const char *)(argv[1]), (char **)NULL, 10);    // param 2: data payload len
    loop_cnt = _strtoul((const char *)(argv[2]), (char **)NULL, 10);    // param 3: loop count

    if (argc > 3)
    {
        delay_us = _strtoul((const char *)(argv[3]), (char **)NULL, 10);    // param 4: TX at given time
    }

    if (!daddr || !data_len)
    {
        dbg_printf("Error: InvalidArgs\r\n");
        return FALSE;
    }

    fc.type = FRAME_TYPE_DATA;
    fc.sec_en = 0;
    fc.pending = 0;
    fc.ack_req = ((daddr == 0xffff) ? 0 : 1);
    fc.panid_compress = 1;
    fc.dst_addr_mode = ADDR_MODE_SHORT;
    fc.ver = FRAME_VER_2006;
    fc.src_addr_mode = ADDR_MODE_SHORT;
    g_tx_buf.len = generate_ieee_frame(FRAME_TYPE_DATA, g_tx_buf.buf, 125, fc, 0,
                                       panid, (uint8_t *)&saddr, panid, (uint8_t *)&daddr,
                                       NV_FIELD, NV_FIELD, NV_FIELD, NV_FIELD);
    GEN_SEQ_DATA_MV_PTR(g_tx_buf.buf, 0, data_len, g_tx_buf.len);
    txl_ctrl_info_t info = {0};
    info.prt_tx_mask = 0xff;
    info.delay_us = delay_us;
    loop_ctrl(loop_cnt, txl_check, txl_exec, cmd_data_report, LOOP_REPORT_FIRST, g_tx_interval_ms,
              (uint32_t)&info);
    dbg_printf("Done\r\n");
    return TRUE;
}

static int cmd_data2015(int argc, char *argv[])
{
    uint16_t panid = mac_panid_get();
    uint8_t *saddr = mac_long_addr_get();
    uint64_t daddr = 0;
    uint16_t data_len = 0;
    uint32_t delay_us = 0;
    uint32_t loop_cnt = 1;
    fc_t fc = {0};

    if (argc < 3)
    {
        dbg_printf("Error: InvalidArgs\r\n");
        return FALSE;
    }

    daddr = _strtoull((const char *)(argv[0]), (char **)NULL, 16);      // param 1: dest address
    data_len = _strtoul((const char *)(argv[1]), (char **)NULL, 10);    // param 2: data payload len
    loop_cnt = _strtoul((const char *)(argv[2]), (char **)NULL, 10);    // param 3: loop count

    if (argc > 3)
    {
        delay_us = _strtoul((const char *)(argv[3]), (char **)NULL, 10);    // param 4: TX at given time
    }

    if (!daddr || !data_len)
    {
        dbg_printf("Error: InvalidArgs\r\n");
        return FALSE;
    }

    fc.type = FRAME_TYPE_DATA;
    fc.sec_en = 0;
    fc.pending = 0;
    fc.ack_req = 1;
    fc.panid_compress = 1;
    fc.dst_addr_mode = ADDR_MODE_EXTEND;
    fc.ver = FRAME_VER_2015;
    fc.src_addr_mode = ADDR_MODE_EXTEND;
    g_tx_buf.len = generate_ieee_frame(FRAME_TYPE_DATA, g_tx_buf.buf, 125, fc, 0,
                                       0, saddr, panid, (uint8_t *)&daddr,
                                       NV_FIELD, NV_FIELD, NV_FIELD, NV_FIELD);
    GEN_SEQ_DATA_MV_PTR(g_tx_buf.buf, 0, data_len, g_tx_buf.len);
    txl_ctrl_info_t info = {0};
    info.prt_tx_mask = 0xff;
    info.delay_us = delay_us;
    loop_ctrl(loop_cnt, txl_check, txl_exec, txl_report, LOOP_REPORT_FIRST, g_tx_interval_ms,
              (uint32_t)&info);
    dbg_printf("Done\r\n");
    return TRUE;
}

static int cmd_raw(int argc, char *argv[])
{
    if (argc < 2)
    {
        dbg_printf("Error: InvalidArgs\r\n");
        return FALSE;
    }

    BUF_RESET(g_tx_buf);
    for (int i = 0; i < argc; i++)
    {
        g_tx_buf.buf[i] = _strtoul((const char *)(argv[i]), (char **)NULL, 16);
    }
    g_tx_buf.len = argc;
    txl_ctrl_info_t info = {0};
    info.prt_tx_mask = 0xff;
    loop_ctrl(1, NULL, txl_exec, txl_report, LOOP_REPORT_FIRST, 0, (uint32_t)&info);
    dbg_printf("Done\r\n");
    return TRUE;
}

static int cmd_encdata(int argc, char *argv[])
{
    uint64_t daddr = 0;
    uint16_t data_len = 0;
    uint16_t panid = mac_panid_get();
    uint8_t *saddr = mac_long_addr_get();
    uint32_t delay_us = 0;
    uint32_t loop_cnt = 1;
    uint8_t key_id_mode = KEY_ID_MODE_0_NOOFFSET;
    uint8_t sec_level = SEC_NONE;

    fc_t fc = {0};
    aux_t aux = {0};
    nonce_t nonce = {0};

    if (argc < 5)
    {
        dbg_printf("Error: InvalidArgs\r\n");
        return FALSE;
    }

    daddr = _strtoull((const char *)(argv[0]), (char **)NULL, 16);
    data_len = _strtoul((const char *)(argv[1]), (char **)NULL, 10);
    sec_level = _strtoul((const char *)(argv[2]), (char **)NULL, 10);
    key_id_mode = _strtoul((const char *)(argv[3]), (char **)NULL, 10);
    loop_cnt = _strtol((const char *)(argv[4]), (char **)NULL, 10);

    if (argc > 5)
    {
        delay_us = _strtoul((const char *)(argv[5]), (char **)NULL, 10);    // TX at given time
    }

    if (!daddr || !data_len || sec_level > SEC_ENC_MIC_128 || key_id_mode > KEY_ID_MODE_1_NOOFFSET)
    {
        dbg_printf("Error: InvalidArgs\r\n");
        return FALSE;
    }

    fc.type = FRAME_TYPE_DATA;
    fc.sec_en = 1;
    fc.pending = 0;
    fc.ack_req = 1;
    fc.panid_compress = 1;
    fc.dst_addr_mode = ADDR_MODE_EXTEND;
    fc.ver = FRAME_VER_2006;
    fc.src_addr_mode = ADDR_MODE_EXTEND;

    aux.sec_ctl.sec_level = sec_level;
    aux.sec_ctl.key_id_mode = key_id_mode;
    aux.sec_ctl.rsv = 0;
    aux.frame_counter = 5;
    if (aux.sec_ctl.key_id_mode == KEY_ID_MODE_1_NOOFFSET)
    {
        aux.key_id = 1;
    }
    mac_memcpy(&nonce.src_ext_addr, mac_long_addr_get(), sizeof(nonce.src_ext_addr));
    nonce.sec_level = aux.sec_ctl.sec_level;
    nonce.frame_counter = aux.frame_counter;

    uint8_t aux_len = sizeof(aux);
    if (aux.sec_ctl.key_id_mode == KEY_ID_MODE_0_NOOFFSET)
    {
        aux_len--;
    }

    g_tx_buf.len = generate_ieee_frame(FRAME_TYPE_DATA, g_tx_buf.buf, 125, fc, 0x84,
                                       panid, saddr, panid, (uint8_t *)&daddr,
                                       (uint8_t *)&aux, aux_len, NV_FIELD, NV_FIELD);
    txl_ctrl_info_t info = {0};
    info.hdr_len = g_tx_buf.len;
    info.fix_seq = 1;
    info.prt_tx_mask = 0xff;
    info.delay_us = delay_us;

    GEN_SEQ_DATA_MV_PTR(g_tx_buf.buf, 0x61, data_len, g_tx_buf.len);

    if (aux.sec_ctl.sec_level < SEC_ENC)
    {
        info.hdr_len = g_tx_buf.len;
    }

    // set nonce
    mac_nonce_set((uint8_t *)&nonce);
    // set key
    mac_txn_key_set(g_mac_key);
    // set security level
    mac_txn_cipher_set(aux.sec_ctl.sec_level);
    loop_ctrl(loop_cnt, txl_check, txl_exec, txl_report, LOOP_REPORT_FINAL, g_tx_interval_ms,
              (uint32_t)&info);
    dbg_printf("Done\r\n");
    return TRUE;
}

static void ed_scan_done_callback(int8_t peak_ed_level, int8_t avrg_ed_level, uint8_t status)
{
    g_peak_ed_level = peak_ed_level;
    g_avrg_ed_level = avrg_ed_level;
    g_ed_scan_done = TRUE;
    //dbg_printf("ED Scan done\r\n");
}

static int cmd_ed_scan(uint32_t argc, uint8_t  *argv[])
{
    uint8_t ret = TRUE;
    uint32_t wait_cnt = 0, duration = 100;

    if (argc < 2)
    {
        dbg_printf("Error: InvalidArgs\r\n");
        return FALSE;
    }

    uint16_t freq = mac_freq_get(); // backup orig freq
    duration = _strtoul((const char *)(argv[0]), (char **)NULL, 10);
    dbg_printf("Start ED Scan. Duration %u us\r\n", duration);
    for (uint32_t i = 0; i < (argc - 1); i++)
    {
        uint8_t scan_channel = _strtoul((const char *)(argv[1 + i]), (char **)NULL, 10);
        if (mac_channel_set(scan_channel) != MAC_STS_SUCCESS)
        {
            dbg_printf("Set Channel %u failed\r\n", scan_channel);
            continue;
        }
#if 0
        // use busy wait ED scan
        ret = mac_EDScan(duration, &ed_level);
        dbg_printf("ED Scan sts=%d ED_Level=%d\r\n", ret, ed_level);
        ret = mac_ed_scan_poll(duration, &ed_level, &ed_level_avg);
        dbg_printf("ED Scan sts=%u ED_Level=%d Level_Avg=%d\r\n", ret, ed_level, ed_level_avg);
#else
        // use ED scan scheduling
        g_ed_scan_done = FALSE;
        ret = mac_ed_scan_schedule(duration, ed_scan_done_callback);
        while (g_ed_scan_done == FALSE)
        {
            wait_cnt++;
#if 0
            if (wait_cnt == 10000)
            {
                // for ED scan cancel test
                mac_ed_scan_cancel();
            }
#endif
        }
        dbg_printf("ED Scan channel %u peak_Level %d avg_level %d\r\n", scan_channel, g_peak_ed_level,
                   g_avrg_ed_level);
#endif
    }
    mac_freq_set(freq); // restore orig freq
    dbg_printf("Done\r\n");
    return ret;
}

static int cmd_pendack(int argc, char *argv[])
{
    uint8_t pendack = 0;
#if Auto_test
    unsigned char bytes[] = {0x04, 0x0e, 0x05, 0x02, 0x00, 0xfc, 0x01, 0x01};
#endif
    pendack = mac_imm_ack_fp_check();
    dbg_printf("ACK pending %u (read only)\r\n", pendack);
#if Auto_test
    if (pendack == 0)
    {
        bytes[sizeof(bytes) - 1] = 0;
    }
    UART_SendData(UART2, bytes, sizeof(bytes));
#endif
    //dbg_printf("Done\r\n");
    return TRUE;
}

static int cmd_txretry(int argc, char *argv[])
{
    uint8_t retry_num = mac_txn_retry_get();
    dbg_printf("tx retry count %u (read only)\r\n", retry_num);
#if Auto_test
    unsigned char bytes[] = {0x04, 0x0e, 0x06, 0x02, 0x00, 0xfc, 0x01, 0x01, retry_num};
    UART_SendData(UART2, bytes, sizeof(bytes));
#endif
    //dbg_printf("Done\r\n");
    return TRUE;
}

#if defined(CONFIG_SOC_SERIES_RTL87X2G) || defined(CONFIG_SOC_SERIES_RTL87X2H)
#include <os_task.h>
#include <os_sync.h>
typedef void (*ctimer_task_cb_t)(void);
void *ctimer_task_handle;
ctimer_task_cb_t g_ctimer_task_cb = NULL;

#define USE_MACSWTIMER 1
//#define USE_OSTIMER 1
#if (USE_MACSWTIMER == 1)
#define MAX_CTIMER 3
mac_timer_handle_t g_ctimer_pool[MAX_CTIMER];
pmac_timer_handle_t g_ctimer;
static void ctimer_cb(void *arg)
{
    uint32_t now = mac_btus_get();
    uint32_t target = (uint32_t)arg + 1000000;
    mac_sw_timer_start(g_ctimer, target, ctimer_cb, (void *)target);
    dbg_printf("fire %u\r\n", now);
    os_task_notify_give(ctimer_task_handle);
}
#define ctimer_init() \
    do { \
        mac_sw_timer_init(g_ctimer_pool, MAX_CTIMER); \
        g_ctimer = mac_sw_timer_alloc(); \
    } while (0)

#define ctimer_stop mac_sw_timer_stop
#define ctimer_start mac_sw_timer_start
#elif (USE_OSTIMER == 1)
#include <os_timer.h>
void *g_ctimer;
static void ctimer_cb(void *arg)
{
    uint32_t now = mac_btus_get();
    dbg_printf("fire %u\r\n", now);
    os_task_notify_give(ctimer_task_handle);
}
#define ctimer_init() \
    do { \
        os_timer_create(&g_ctimer, "ctimer", 1, 1000, true, ctimer_cb); \
    } while (0)

#define ctimer_stop(timer) os_timer_stop(&timer)
#define ctimer_start(timer,b,c,d) os_timer_start(&timer)
#else /* Not use timer */
#define g_ctimer
#define ctimer_init()
#define ctimer_stop(timer)
#define ctimer_start(timer,b,c,d) os_task_notify_give(ctimer_task_handle)
#endif

static void ctimer_task(void *p_param)
{
    uint32_t notify;
    dbg_printf("ctimer_task start\r\n");
    while (1)
    {
        if (os_task_notify_take(1, 0xffffffff, &notify))
        {
            //dbg_printf("task_notified %u\r\n", mac_btus_get());
            if (g_ctimer_task_cb)
            {
                g_ctimer_task_cb();
            }
        }
    }
}

static int cmd_demo_timer(uint32_t argc, uint8_t  *argv[])
{
    os_task_create(&ctimer_task_handle, "ctimer", ctimer_task, NULL,
                   ZB_TASK_STACK_SIZE, ZB_TASK_PRIORITY);
    ctimer_init();
    ctimer_stop(g_ctimer);
    uint32_t target = mac_btus_get() + 1000000;
    dbg_printf("start ctimer in %u us\r\n", target);
    ctimer_start(g_ctimer, target, ctimer_cb, (void *)target);
    return TRUE;
}

static uint32_t g_demo_sleep_duration;
static void demo_cb(void)
{
    static uint32_t seq = 0;
    fc_t fc = {0};
    uint32_t start = mac_btus_get();
    uint16_t panid = mac_panid_get();
    uint16_t saddr = mac_short_addr_get();
    uint16_t daddr = saddr;

    fc.type = FRAME_TYPE_DATA;
    fc.sec_en = 0;
    fc.pending = 0;
    fc.ack_req = 1;
    fc.panid_compress = 1;
    fc.dst_addr_mode = ADDR_MODE_SHORT;
    fc.ver = FRAME_VER_2006;
    fc.src_addr_mode = ADDR_MODE_SHORT;
    g_tx_buf.len = generate_ieee_frame(FRAME_TYPE_DATA, g_tx_buf.buf, 125, fc, 0,
                                       panid, (uint8_t *)&saddr, panid, (uint8_t *)&daddr,
                                       NV_FIELD, NV_FIELD, NV_FIELD, NV_FIELD);
    GEN_SEQ_DATA_MV_PTR(g_tx_buf.buf, 0, 10, g_tx_buf.len);
    mac_txn_payload_set(0, g_tx_buf.len, g_tx_buf.buf);
    RESET_TXDOWN();
    dbg_printf("send ");
    mac_txn_trig(fc.ack_req, fc.sec_en);
    WAIT_FOR_TXDOWN_UNTIL(mac_btus_get() > (start + 1000000));
    print_tx_result(seq++, 0xff);
    uint32_t end = mac_btus_get();
    dbg_printf("\t proc_t %u\r\n", end - start);
    zbmac_power_manager_set(g_demo_sleep_duration, 0);
}

void demo_pm_exit(void)
{
    os_task_notify_give(ctimer_task_handle);
    dbg_printf("exit_pm = %u pm_wakeup_diff(min/avg/max) = %d/%d/%d us\r\n", mac_btus_get(),
               g_zbpm_wakeup_diff_min >> 7, g_zbpm_wakeup_diff_avg >> 7, g_zbpm_wakeup_diff_max >> 7);
}

static int cmd_demo(uint32_t argc, uint8_t  *argv[])
{
    if (argc != 1)
    {
        dbg_printf("Error: InvalidArgs\r\n");
        return FALSE;
    }
    g_demo_sleep_duration = _strtoul((const char *)(argv[0]), (char **)NULL, 10);

    g_ctimer_task_cb = demo_cb;
    os_task_create(&ctimer_task_handle, "ctimer", ctimer_task, NULL,
                   ZB_TASK_STACK_SIZE, ZB_TASK_PRIORITY);
    zbmac_power_manager_init(demo_pm_exit);
    demo_cb();
    return TRUE;
}
#endif

void shell_register_test_cmd(void)
{
    /* brief, synopsis, description, example */
#if defined(CONFIG_SOC_SERIES_RTL87X2G) || defined(CONFIG_SOC_SERIES_RTL87X2H)
    shell_register((shell_program_t)cmd_demo_timer, "demo_timer",
                   BRIEF("demo_timer test command"));

    shell_register((shell_program_t)cmd_demo, "demo",
                   BRIEF("demo test command")
                   SYNOPSIS("demo <time>")
                   DESCRIPTION("time: sleep duration (us)")
                   DESCRIPTION("Periodically do <sleep - wake up - send> in this demo"));
#endif
    /* System Command */
    shell_register((shell_program_t)cmd_mac_init, "mac_init",
                   BRIEF("Initial mac")
                   DESCRIPTION("Must be called once at startup before any test commands"));
    shell_register((shell_program_t)cmd_mac_disable, "mac_disable",
                   BRIEF("Disable mac")
                   DESCRIPTION("After disabling, you need to do mac_init command again to continue executing other test commands"));
    shell_register((shell_program_t)cmd_mac_enable, "mac_enable",
                   BRIEF("get mac state or enable it")
                   SYNOPSIS("mac_enable [<enable/disable>]")
                   DESCRIPTION("enable/disable: 1/0"));
    shell_register((shell_program_t)cmd_radio, "radio",
                   BRIEF("get/set radio state")
                   SYNOPSIS("radio <bt/rf> [<on/off>]")
                   DESCRIPTION("on/off: 1/0"));
    shell_register((shell_program_t)cmd_reset, "reset",
                   BRIEF("Reset system"));
    shell_register((shell_program_t)cmd_pm, "pm",
                   BRIEF("Power management test")
                   SYNOPSIS("pm [<time>]")
                   DESCRIPTION("time: next wakeup time (us)")
                   EXAMPLE("pm - Register power management entry into system")
                   EXAMPLE("pm 5000000 - Go to sleep immediately and wake up after 5 seconds"));
    shell_register((shell_program_t)cmd_swtimer, "swtimer",
                   BRIEF("MAC sw timer test")
                   SYNOPSIS("swtimer [<timeout>]")
                   DESCRIPTION("timeout: 1 ~ 4294967295 (us)")
                   EXAMPLE("swtimer - Init swtimer")
                   EXAMPLE("swtimer 2000000 - timeout after 2s"));
    shell_register((shell_program_t)cmd_bttimer, "bttimer",
                   BRIEF("BT timer test")
                   SYNOPSIS("bttimer <timeout>")
                   DESCRIPTION("timeout: 1 ~ 4294967295 (us)")
                   EXAMPLE("bttimer 2000000 - timeout after 2s"));

#if ENABLE_BLE
    /* BLE */
    shell_register((shell_program_t)cmd_ble_adv, "ble_adv",
                   BRIEF("get/set BLE Advertising")
                   SYNOPSIS("ble_adv [<enable/disable>]")
                   DESCRIPTION("enable/disable: 1/0"));
#endif /* ENABLE_BLE */
    /* Security */
    shell_register((shell_program_t)cmd_upper_sec_test, "upper_sec_test",
                   BRIEF("Security test with security level 4 (SEC_ENC)")
                   SYNOPSIS("upper_sec_test [<data_len>]")
                   DESCRIPTION("data_len: plaintext data length"));

    /* Config command */
    shell_register((shell_program_t)cmd_config_dump, "config",
                   BRIEF("Show configuration parameters"));
    shell_register((shell_program_t)cmd_ifconfig, "ifconfig",
                   BRIEF("show/clear interface statistic")
                   SYNOPSIS("ifconfig [<value>]")
                   DESCRIPTION("value: c/r/t/a")
                   DESCRIPTION("       c - clear statistic")
                   DESCRIPTION("       r - show RX statistic")
                   DESCRIPTION("       t - show TX statistic")
                   DESCRIPTION("       a - show All statistic")
                   EXAMPLE("ifconfig - show ALL statistic"));
    shell_register((shell_program_t)cmd_panid, "panid",
                   BRIEF("get/set panid")
                   SYNOPSIS("panid [<value>]")
                   DESCRIPTION("value: 0x0000 ~ 0xffff"));
    shell_register((shell_program_t)cmd_channel, "channel",
                   BRIEF("get/set channel")
                   SYNOPSIS("channel [<value>]")
                   DESCRIPTION("value: 11 ~ 26"));
    shell_register((shell_program_t)cmd_freq, "freq",
                   BRIEF("get/set rf frequency")
                   SYNOPSIS("freq [<value>]")
                   DESCRIPTION("value: 2402 ~ 2480 frequency (MHz)")
                   EXAMPLE("freq 2402"));
    shell_register((shell_program_t)cmd_shortaddr, "shortaddr",
                   BRIEF("get/set short addr")
                   SYNOPSIS("shortaddr [<value>]")
                   DESCRIPTION("value: 0x0000 ~ 0xffff"));
    shell_register((shell_program_t)cmd_extaddr, "extaddr",
                   BRIEF("get/set extended addr")
                   SYNOPSIS("extaddr [<value>]")
                   DESCRIPTION("value: 0x0000000000000000 ~ 0xffffffffffffffff"));
    shell_register((shell_program_t)cmd_mackey, "mackey",
                   BRIEF("get/set mac test key")
                   SYNOPSIS("mackey [<value>]")
                   DESCRIPTION("value: 16 bytes HEX data")
                   EXAMPLE("mackey 11223344556677889900aabbccddeeff"));

    /* TX Setting */
    shell_register((shell_program_t)cmd_ccamode, "ccamode",
                   BRIEF("get/set CCA mode")
                   SYNOPSIS("ccamode [<mode> [<threshold>]]")
                   DESCRIPTION("mode: 0 ~ 4")
                   DESCRIPTION("      0 disable CCA")
                   DESCRIPTION("      1 Energy Detection mode")
                   DESCRIPTION("      2 Carrier Sense mode")
                   DESCRIPTION("      3 CS or ED combination mode")
                   DESCRIPTION("      4 CS and ED combination mode")
                   DESCRIPTION("threshold: 0 ~ 255")
                   DESCRIPTION("           Only applicable to modes 1, 3, and 4"));
    shell_register((shell_program_t)cmd_csma, "csma",
                   BRIEF("get/set CSMA parameters")
                   SYNOPSIS("csma [e <enable/disable>] [i <minBe>] [a <maxBe>] [b <maxBackoff>]")
                   DESCRIPTION("enable/disable: 1/0")
                   DESCRIPTION("minBe: 0 ~ 15")
                   DESCRIPTION("maxBe: 0 ~ 15")
                   DESCRIPTION("maxBackoff: 0 ~ 15")
                   EXAMPLE("csma e 1 i 3 a 5 b 4"));
    shell_register((shell_program_t)cmd_txgain, "txgain",
                   BRIEF("get/set TX gain")
                   SYNOPSIS("txgain [<value>]")
                   DESCRIPTION("value: 0 ~ 127"));
    shell_register((shell_program_t)cmd_notxcrc, "notxcrc",
                   BRIEF("Send frame without FCS")
                   SYNOPSIS("notxcrc [<enable/disable>]")
                   DESCRIPTION("enable/disable: 1/0"));
    shell_register((shell_program_t)cmd_enh_ack_early, "enhackearly",
                   BRIEF("Send enhanced ACK at RX early interrupt")
                   SYNOPSIS("enhackearly [<enable/disable>]")
                   DESCRIPTION("enable/disable: 1/0"));
    shell_register((shell_program_t)cmd_tx_interval, "txinterval",
                   BRIEF("The interval between continuous send test")
                   SYNOPSIS("txinterval [<value>]")
                   DESCRIPTION("value: 0 ~  4294967295 ms")
                   DESCRIPTION("       0 - no additional delay"));

    /* RX Filter Setting */
    shell_register((shell_program_t)cmd_promiscuous, "promiscuous",
                   BRIEF("Accept all packets with CRC OK")
                   SYNOPSIS("promiscuous [<enable/disable>]")
                   DESCRIPTION("enable/disable: 1/0"));
    shell_register((shell_program_t)cmd_recverrpkt, "recverrpkt",
                   BRIEF("Accept all kinds of pkt(including CRC error)")
                   SYNOPSIS("recverrpkt [<enable/disable>]")
                   DESCRIPTION("enable/disable: 1/0"));
    shell_register((shell_program_t)cmd_scanmode, "scanmode",
                   BRIEF("enable RX Filter scan mode")
                   SYNOPSIS("scanmode [<enable/disable>]")
                   DESCRIPTION("enable/disable: 1/0")
                   DESCRIPTION("When scan mode is enabled, only Beacon frame with FCS correct will be accepted by RX filter"));
    shell_register((shell_program_t)cmd_rxframetype, "rxframetype",
                   BRIEF("RX Frame Type Filter")
                   SYNOPSIS("rxframetype [<type>]")
                   DESCRIPTION("type: b/f")
                   DESCRIPTION("      b - default without receiving ACK")
                   DESCRIPTION("      f - allow receiving ACK"));
    shell_register((shell_program_t)cmd_srcmatchmode, "srcmatchmode",
                   BRIEF("enable the enhanced frame pending mechanism of auto frame pending bit in Imm-Ack")
                   SYNOPSIS("srcmatchmode [<enable/disable>]")
                   DESCRIPTION("enable/disable: 1/0")
                   DESCRIPTION("                1 - response to Data frames or Command frames")
                   DESCRIPTION("                0 - only response to a Data Request command frame"));
    shell_register((shell_program_t)cmd_srcmatchfilter, "srcmatchfilter",
                   BRIEF("get/set source address match filter")
                   SYNOPSIS("srcmatchfilter [(<extaddr>) / (<shortaddr> <panid>)]")
                   DESCRIPTION("extaddr: 0x0000000000000000 ~ 0xffffffffffffffff")
                   DESCRIPTION("shortaddr: 0x0000 ~ 0xffff")
                   DESCRIPTION("panid: 0x0000 ~ 0xffff")
                   EXAMPLE("srcmatchfilter 0xacde480000000001")
                   EXAMPLE("srcmatchfilter 0x1122 0x1aaa"));

    /* TX Commands */
    shell_register((shell_program_t)cmd_data, "data",
                   BRIEF("send IEEE 802.15.4-2006 data frame")
                   SYNOPSIS("data <shortaddr> <len> <loop_cnt> [<delay>]")
                   DESCRIPTION("shortaddr: 0x0000 ~ 0xffff")
                   DESCRIPTION("len: 1 ~ 125")
                   DESCRIPTION("loop_cnt: 0 ~ 4294967295")
                   DESCRIPTION("          0 - infinite loop")
                   DESCRIPTION("delay: 0 ~ 4294967295 us")
                   EXAMPLE("data 0x1122 100 5"));
    shell_register((shell_program_t)cmd_data2015, "data2015",
                   BRIEF("send IEEE 802.15.4-2015 data frame")
                   SYNOPSIS("data2015 <extaddr> <len> <loop_cnt> [<delay>]")
                   DESCRIPTION("extaddr: 0x0000000000000000 ~ 0xffffffffffffffff")
                   DESCRIPTION("len: 1 ~ 125")
                   DESCRIPTION("loop_cnt: 0 ~ 4294967295")
                   DESCRIPTION("          0 - infinite loop")
                   DESCRIPTION("delay: 0 ~ 4294967295 us")
                   EXAMPLE("data2015 0xacde480000000001 100 5"));
    shell_register((shell_program_t)cmd_raw, "raw",
                   BRIEF("send user defined raw data")
                   SYNOPSIS("raw <byte 0> [<byte 1> <byte 2> ...<byte N>]")
                   EXAMPLE("raw 00 22 00 86 11 34 12 33 cf 00 0f 04 0d ae 09 35 0c 80 3f 01 02"));
    shell_register((shell_program_t)cmd_encdata, "encdata",
                   BRIEF("send IEEE 802.15.4-2006 encrypted data frame")
                   SYNOPSIS("encdata <extaddr> <len> <sec_level> <key_id_mode> <loop_cnt> [<delay>]")
                   DESCRIPTION("extaddr: 0x0000000000000000 ~ 0xffffffffffffffff")
                   DESCRIPTION("len: 1 ~ 125")
                   DESCRIPTION("sec_level: 1 ~ 7")
                   DESCRIPTION("           1 - SEC_MIC_32")
                   DESCRIPTION("           2 - SEC_MIC_64")
                   DESCRIPTION("           3 - SEC_MIC_128")
                   DESCRIPTION("           4 - SEC_ENC")
                   DESCRIPTION("           5 - SEC_ENC_MIC_32")
                   DESCRIPTION("           6 - SEC_ENC_MIC_64")
                   DESCRIPTION("           7 - SEC_ENC_MIC_128")
                   DESCRIPTION("key_id_mode: 0 ~ 3")
                   DESCRIPTION("loop_cnt: 0 ~ 4294967295")
                   DESCRIPTION("          0 - infinite loop")
                   DESCRIPTION("delay: 0 ~ 4294967295 us")
                   EXAMPLE("encdata 0xacde480000000001 100 4 1 5"));

    /* RX Command */
    shell_register((shell_program_t)cmd_ed_scan, "ed_scan",
                   BRIEF("start a scheduled ED scan procedure")
                   SYNOPSIS("ed_scan <duration> <ch0> [<ch1> ... <chN>]")
                   DESCRIPTION("duration: 1 ~ 4,294,967,295 us")
                   DESCRIPTION("ch: 11 ~ 26")
                   EXAMPLE("ed_scan 500 11 12 13"));
    shell_register((shell_program_t)cmd_txretry, "txretry",
                   BRIEF("Get tx retry counter"));
    shell_register((shell_program_t)cmd_pendack, "pendack",
                   BRIEF("Get RX Imm-Ack pending bit"));
}
