/**
*****************************************************************************************
*     Copyright(c) 2017, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
   * @file      main_test_common.c
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
#include "shell.h"
#include "dbg_printf.h"
#include "trace.h"

#ifdef CONFIG_SOC_SERIES_RTL87X2G
//#include "rtl_gpio.h"
#include "rtl_nvic.h"
#include "rtl_tim.h"
#include "wdt.h"
#else
#include "rtl876x_nvic.h"
#include "rtl876x_tim.h"
#include "rtl876x_wdg.h"
#if GPIO_DEBUG
#include "rtl876x_pinmux.h"
#include "rtl876x_gpio.h"
#include "rtl876x_rcc.h"
#endif
#endif

#if defined(CONFIG_SOC_SERIES_RTL87X2G) || defined(CONFIG_SOC_SERIES_RTL87X2H)
#include "power_manager_interface.h"
#include "power_manager_slave.h"
#endif
#include "mac_driver_interface.h"
#include <os_sched.h>
#include <os_sync.h>
#include <os_task.h>
#include <limits.h>
#include "zb_tst_cfg.h"
#include "strproc.h"
#include "patch.h"
#include "mac_802154_frame_parser.h"
#include "mac_test_common.h"
#include "vector_table.h"
#include "mac_csl.h"

#define FORCE_DUMP_TX 0
extern void Zigbee_Handler_Patch(void);
extern void modem_set_zb_cca_combination_rom(uint8_t comb);
extern void set_zigbee_priority(uint16_t priority, uint16_t priority_min);
extern uint32_t (*lowerstack_SystemCall)(uint32_t opcode, uint32_t param, uint32_t param1,
                                         uint32_t param2);
mac_attribute_t g_mac_attribute;
mac_driver_t g_mac_driver;
pan_mac_comm_t g_pan_mac_comm;
volatile uint32_t g_recent_rx_timestamp = 0;
ifrxstat_t g_ifrxstat = {0};
iftxstat_t g_iftxstat = {0};
static uint8_t g_zbpm_inited = FALSE;
#if defined(CONFIG_SOC_SERIES_RTL87X2G) || defined(CONFIG_SOC_SERIES_RTL87X2H)
static volatile zbpm_adapter_t g_zbpm_adap;
#endif

void clear_tx_stat(void)
{
    memset(&g_iftxstat, 0, sizeof(iftxstat_t));
}

void clear_rx_stat(void)
{
    memset(&g_ifrxstat, 0, sizeof(ifrxstat_t));
}

void dbg_mem_dump(const uint8_t *addr, uint32_t len)
{
    for (int i = 0; i < len; i++)
    {
        dbg_printf("%02x ", addr[i]);
        if ((i + 1) % 16 == 0)
        {
            dbg_printf("\r\n");
        }
    }
    dbg_printf("\r\n");
}

int32_t edscan_lv2dbm(int32_t level)
{
    return (level << 1) - 90;
}

uint32_t get_curr_us(void)
{
    mac_bt_clk_t curr_clk;
    mac_btclk_get(&curr_clk);
    return mac_btclk_to_us(curr_clk);
}

uint8_t parse_digit(char c)
{
    if (('A' <= c) && (c <= 'F'))
    {
        return (c - 'A' + 10);
    }
    if (('a' <= c) && (c <= 'f'))
    {
        return (c - 'a' + 10);
    }
    if (('0' <= c) && (c <= '9'))
    {
        return (c - '0');
    }
    return 0xff;
}

void print_tx_result(uint8_t val, uint8_t prt_tx_mask)
{
    if (prt_tx_mask)
    {
        switch (g_tx_done)
        {
        case TX_SUCCESS:
            if (prt_tx_mask & TX_SUCCESS_MASK)
            {
                dbg_printf("%u success\r\n", val);
            }
            break;
        case TX_BUSY:
            if (prt_tx_mask & TX_BUSY_MASK)
            {
                dbg_printf("%u busy\r\n", val);
            }
            break;
        case TX_NOACK:
            if (prt_tx_mask & TX_NOACK_MASK)
            {
                dbg_printf("%u noack\r\n", val);
            }
            break;
        case TX_AT_FAIL:
            if (prt_tx_mask & TX_AT_FAIL_MASK)
            {
                dbg_printf("%u txat fail\r\n", val);
            }
            break;
        default:
            dbg_printf("%u undefined error\r\n", val);
            break;
        }
    }
}

void print_rx_packet(uint8_t *rx_data)
{
    uint8_t *buf = MAC_RX_PKT(rx_data);
    uint8_t buf_len = MAC_RX_PKT_LEN(rx_data);
    uint16_t crc = MAC_RX_PKT_CRC(rx_data);
    uint8_t lqi = MAC_RX_PKT_LQI(rx_data);
    int8_t rssi = MAC_RX_PKT_RSSI(rx_data);
    uint64_t rx_timestamp = MAC_RX_PKT_TIMESTAMP(rx_data);

    dbg_printf("crc:0x%04x lqi:%u rssi:%d timestamp:%llu\r\n", crc, lqi, rssi, rx_timestamp);
    dbg_mem_dump(buf, buf_len);
}

void memb_init(struct memb *m)
{
    memset(m->used, 0, m->num);
    memset(m->mem, 0, m->size * m->num);
}

void *memb_alloc(struct memb *m)
{
    int i;
    for (i = 0; i < m->num; ++i)
    {
        if (m->used[i] == FALSE)
        {
            m->used[i] = TRUE;
            return (void *)((char *)m->mem + (i * m->size));
        }
    }
    return NULL;
}

int memb_free(struct memb *m, void *ptr)
{
    int i;
    char *ptr2;

    ptr2 = (char *)m->mem;
    for (i = 0; i < m->num; ++i)
    {
        if (ptr2 == (char *)ptr)
        {
            if (m->used[i] == false)
            {
                return -1;
            }
            m->used[i] = false;
            return 0;
        }
        ptr2 += m->size;
    }
    return -1;
}

struct list
{
    struct list *next;
};

void *list_tail(const_list_t list)
{
    struct list *l;
    if (*list == NULL)
    {
        return NULL;
    }
    for (l = *list; l->next != NULL; l = l->next);
    return l;
}

void list_add(list_t list, void *item)
{
    struct list *l;

    /* Make sure not to add the same element twice */
    list_remove(list, item);

    ((struct list *)item)->next = NULL;

    l = list_tail(list);

    if (l == NULL)
    {
        *list = item;
    }
    else
    {
        l->next = item;
    }
}

void *list_pop(list_t list)
{
    struct list *l;
    l = *list;
    if (*list != NULL)
    {
        *list = ((struct list *)*list)->next;
    }

    return l;
}

void list_remove(list_t list, const void *item)
{
    struct list *l, *r;

    if (*list == NULL)
    {
        return;
    }

    r = NULL;
    for (l = *list; l != NULL; l = l->next)
    {
        if (l == item)
        {
            if (r == NULL)
            {
                /* First on list */
                *list = l->next;
            }
            else
            {
                /* Not first on list */
                r->next = l->next;
            }
            l->next = NULL;
            return;
        }
        r = l;
    }
}

LIST(g_mac_cmd_handler_list);

static mac_cmd_handler_t *mac_cmd_handler_lookup(uint8_t type)
{
    mac_cmd_handler_t *handler = NULL;
    for (handler = list_head(g_mac_cmd_handler_list);
         handler != NULL;
         handler = list_item_next(handler))
    {
        if (handler->type == type)
        {
            return handler;
        }
    }
    return NULL;
}

void mac_cmd_register_handler(mac_cmd_handler_t *handler)
{
    list_add(g_mac_cmd_handler_list, handler);
}

uint8_t txl_check(uint32_t count)
{
    //dbg_printf("check %u\r\n", count);
    RESET_TXDOWN();
    return g_tx_loop_state;
}

// return some useful information to loop_report
uint32_t txl_exec(uint32_t count, uint32_t ctrl_info)
{
    uint32_t base_us = 0;
    txl_ctrl_info_t *info = (txl_ctrl_info_t *)ctrl_info;
    fc_t *fc = (fc_t *)g_tx_buf.buf;
    uint8_t *seq = &g_tx_buf.buf[sizeof(fc_t)];

    //dbg_printf("exec %u\r\n", count);
    if (!info->fix_seq && count)
    {
        (*seq)++;
    }
    mac_txn_payload_set(info->hdr_len, g_tx_buf.len, g_tx_buf.buf);
#if !FORCE_DUMP_TX
    if (info->dump_pkt)
#endif
        dbg_mem_dump((const uint8_t *)g_tx_buf.buf, g_tx_buf.len);
    if (fc->sec_en)
    {
        RESET_TXDOWN();
        mac_upper_enc_trig();
        if (info->dump_enc_pkt)
        {
            dbg_mem_dump((const uint8_t *)MAC_TXN_BASE_ADDR, 125);
        }
    }
    RESET_TXDOWN();
    if (mac_TrigTxNDelay(fc->ack_req, fc->sec_en, fc->ver, info->delay_us,
                         &base_us) == MAC_STS_SUCCESS)
    {
        WAIT_FOR_TXDOWN_UNTIL(g_tx_loop_state == TX_LOOP_STOP);
        if (info->prt_tx_mask)
        {
            print_tx_result(*seq, info->prt_tx_mask);
        }
        //RESET_TXDOWN();
    }
    else
    {
        dbg_printf("mac_TrigTxNDelay fail %u\r\n", *seq);
    }
    return base_us;
}

#if Auto_test
void txl_report(uint32_t count, uint32_t exec_info)
{
    unsigned char bytes[] = {0x04, 0x0e, 0x06, 0x02, 0x00, 0xfc, 0x01, 0x01, 0x03};
    UART_SendData(UART2, bytes, sizeof(bytes));
}
#endif

#if Auto_test
void cmd_data_report(uint32_t count, uint32_t exec_info)
{
    dbg_printf("report %u exec_info %u\r\n", count, exec_info);
    uint32_t trig_time = mac_txn_timestamp_get();
    unsigned char bytes[] = {0x04, 0x0e, 0x0c, 0x08, 0x00, 0xfc, 0x01,
                             exec_info >> 24, exec_info >> 16, exec_info >> 8, exec_info,
                             trig_time >> 24, trig_time >> 16, trig_time >> 8, trig_time
                            };
    UART_SendData(UART2, bytes, sizeof(bytes));
}
#endif

/**
*
* @fn void loop_ctrl(uint32_t count, loop_check_cb check_cb,
*                   loop_exec_cb exec_cb, loop_report_cb report_cb,
*                   uint8_t report_mode, uint32_t interval, uint32_t ctrl_info)
*
* @brief Provide an event loop control process.
*
* @param count loop count. 0 is infinite loop
*
* @param check_cb user defined callback will be executed at the end of each loop
*
* @param exec_cb user defined callback will be executed at the beginning of each loop
*
* @param report_cb user defined callback will be executed after exec_cb
*
* @param report_mode could be LOOP_REPORT_NONE, LOOP_REPORT_FIRST, LOOP_REPORT_FINAL, LOOP_REPORT_ALL
*
* @param interval interval between each loop
*
* @param ctrl_info additional information used in loop_ctrl
*
* @return None
*
*/
void loop_ctrl(uint32_t count, loop_check_cb check_cb,
               loop_exec_cb exec_cb, loop_report_cb report_cb,
               uint8_t report_mode, uint32_t interval, uint32_t ctrl_info)
{
    uint32_t i, exec_info = 0;

    g_tx_loop_state = TX_LOOP_CONT;
    for (i = 0;; i++)
    {
        if (exec_cb)
        {
            exec_info = exec_cb(i, ctrl_info);
        }

        if (report_cb)
        {
            if (report_mode & LOOP_REPORT_FIRST)
            {
                report_mode &= ~LOOP_REPORT_FIRST;
                if (count == 1)
                {
                    report_mode &= ~LOOP_REPORT_FINAL;
                }
                report_cb(i, exec_info);
            }
            else if (report_mode & 0x2)
            {
                report_cb(i, exec_info);
            }
        }
        if (interval)
        {
#if BUSY_WAIT
            uint64_t target_us = mac_timestamp_get() + interval * 1000;
            while (mac_timestamp_get() < target_us);
#else
            mac_btus_intr_set(MAC_BT_TIMER0, mac_btus_get() + interval * 1000);
            os_sem_take(zb_sem, 0xffffffff);
#endif
        }
        // terminate condition
        if ((count && ((i + 1) == count)) || check_cb(i) == TX_LOOP_STOP)
        {
            break;
        }
    }
    if (report_cb && (report_mode & LOOP_REPORT_FINAL) && (report_mode & 0x2) == 0)
    {
        report_cb(i, exec_info);
    }
    RESET_TXDOWN();
    g_tx_loop_state = TX_LOOP_STOP;
}

uint8_t mac_TrigTxNDelay(uint8_t ackreq, uint8_t secreq, uint8_t frm_ver, uint32_t delay,
                         uint32_t *base_us)
{
    if (delay > 0)
    {
        *base_us = get_curr_us(); // must return base time to caller
        uint32_t target_us = *base_us + delay;
        mac_bt_clk_t target_clk;
        mac_USToBTClk(target_us, &target_clk);
        mac_btus_intr_set(MAC_BT_TIMER0, target_us);
        return mac_txn_trig_at_time(ackreq, secreq, true, target_clk);
    }
    else
    {
        return mac_txn_trig(ackreq, secreq);
    }
}

uint8_t generate_ieee_frame(uint8_t frm_type, uint8_t *tx_buf, uint16_t tx_buf_len, fc_t fc,
                            uint8_t seq, uint16_t spid,
                            uint8_t *saddr, uint16_t dpid, uint8_t *daddr, uint8_t *aux, uint16_t aux_len, uint8_t cmd_id,
                            ss_t *ss)
{
    uint16_t len = 0;
    // frame control header rule
    if (fc.ver == FRAME_VER_2015)
    {
        if (fc.dst_addr_mode == 0 && fc.src_addr_mode == 0 && dpid == 0 && spid == 0) { fc.panid_compress = 0; }
        else if (fc.dst_addr_mode == 0 && fc.src_addr_mode == 0 && dpid > 0 && spid == 0) { fc.panid_compress = 1; }
        else if (fc.dst_addr_mode > 0 && fc.src_addr_mode == 0 && dpid > 0 && spid == 0) { fc.panid_compress = 0; }
        else if (fc.dst_addr_mode > 0 && fc.src_addr_mode == 0 && dpid == 0 && spid == 0) { fc.panid_compress = 1; }
        else if (fc.dst_addr_mode == 0 && fc.src_addr_mode > 0 && dpid == 0 && spid > 0) { fc.panid_compress = 0; }
        else if (fc.dst_addr_mode == 0 && fc.src_addr_mode > 0 && dpid == 0 && spid == 0) { fc.panid_compress = 1; }
        else if (fc.dst_addr_mode == 3 && fc.src_addr_mode == 3 && dpid > 0 && spid == 0) { fc.panid_compress = 0; }
        else if (fc.dst_addr_mode == 3 && fc.src_addr_mode == 3 && dpid == 0 && spid == 0) { fc.panid_compress = 1; }
        else if (fc.dst_addr_mode == 2 && fc.src_addr_mode == 2 && dpid > 0 && spid > 0) { fc.panid_compress = 0; }
        else if (fc.dst_addr_mode == 2 && fc.src_addr_mode == 3 && dpid > 0 && spid > 0) { fc.panid_compress = 0; }
        else if (fc.dst_addr_mode == 3 && fc.src_addr_mode == 2 && dpid > 0 && spid > 0) { fc.panid_compress = 0; }
        else if (fc.dst_addr_mode == 2 && fc.src_addr_mode == 3 && dpid > 0 && spid == 0) { fc.panid_compress = 1; }
        else if (fc.dst_addr_mode == 3 && fc.src_addr_mode == 2 && dpid > 0 && spid == 0) { fc.panid_compress = 1; }
        else if (fc.dst_addr_mode == 2 && fc.src_addr_mode == 2 && dpid > 0 && spid == 0) { fc.panid_compress = 1; }
        else { fc.panid_compress = 0; }
    }
    else
    {
        if (fc.dst_addr_mode > 0 && fc.src_addr_mode > 0 && dpid == spid) { fc.panid_compress = 1; }
        else { fc.panid_compress = 0; } // only fc.dst_addr_mode > 0 || only fc.src_addr_mode > 0 || ImmAck
    }

    // fill frame control header
    CPY_MV_PTR((void *)(tx_buf + len), (void *)&fc, sizeof(fc), len);

    // fill seq number
    if (fc.seq_num_suppress == 0)
    {
        tx_buf[len++] = seq;
    }

    if (fc.ver == FRAME_VER_2015)
    {
        // fill dest pan id & address
        if (dpid > 0)
        {
            CPY_MV_PTR(&tx_buf[len], &dpid, 2, len);
        }
        CPY_MV_PTR(&tx_buf[len], daddr, ADDR_MODE2LEN[fc.dst_addr_mode], len);

        // fill src pan id & address
        if (spid > 0)
        {
            CPY_MV_PTR(&tx_buf[len], &spid, 2, len);
        }
        CPY_MV_PTR(&tx_buf[len], saddr, ADDR_MODE2LEN[fc.src_addr_mode], len);
    }
    else
    {
        if (fc.dst_addr_mode > 0 && fc.src_addr_mode > 0)
        {
            // fill dest pan id & address
            CPY_MV_PTR(&tx_buf[len], &dpid, 2, len);
            CPY_MV_PTR(&tx_buf[len], daddr, ADDR_MODE2LEN[fc.dst_addr_mode], len);

            // fill src pan id & address
            if (!fc.panid_compress)
            {
                CPY_MV_PTR(&tx_buf[len], &spid, 2, len);
            }
            CPY_MV_PTR(&tx_buf[len], saddr, ADDR_MODE2LEN[fc.src_addr_mode], len);
        }
        else
        {
            if (fc.dst_addr_mode > 0)
            {
                // fill dest pan id & address
                if (dpid > 0)
                {
                    CPY_MV_PTR(&tx_buf[len], &dpid, 2, len);
                }
                CPY_MV_PTR(&tx_buf[len], daddr, ADDR_MODE2LEN[fc.dst_addr_mode], len);
            }

            if (fc.src_addr_mode > 0)
            {
                // fill src pan id & address
                if (spid > 0)
                {
                    CPY_MV_PTR(&tx_buf[len], &spid, 2, len);
                }
                CPY_MV_PTR(&tx_buf[len], saddr, ADDR_MODE2LEN[fc.src_addr_mode], len);
            }
        }
    }

    // fill security header
    if (fc.sec_en && aux)
    {
        CPY_MV_PTR((void *)(tx_buf + len), (void *)aux, aux_len, len);
    }

    if (frm_type == FRAME_TYPE_COMMAND)
    {
        // fill command id
        tx_buf[len++] = cmd_id;
    }
    else if (frm_type == FRAME_TYPE_BEACON)
    {
        // fill Superframe Specification field
        if (ss)
        {
            CPY_MV_PTR(&tx_buf[len], ss, sizeof(ss_t), len);
        }
        // No GTS Info field
        tx_buf[len++] = 0;
        // No Pending Address field
        tx_buf[len++] = 0;
    }
    return len;
}

#define RX_BUF_SIZE     16
typedef struct
{
    uint8_t raw[160];
} rx_item_t;
static uint8_t rx_tail = 0;
static uint8_t rx_head = 0;
static rx_item_t rx_buf[RX_BUF_SIZE];
void radio_rx(void)
{
    fc_t *p_fc;
    uint32_t lock;

    while (rx_head != rx_tail)
    {
        g_ifrxstat.packets++;

        p_fc = (fc_t *)&rx_buf[rx_head].raw[1];
        if (p_fc->type == FRAME_TYPE_COMMAND)
        {
            uint8_t offset = mac_802154_frame_parser_addressing_end_offset_get(rx_buf[rx_head].raw);
            mac_cmd_handler_t *handler = mac_cmd_handler_lookup(rx_buf[rx_head].raw[offset]);
            if (handler && handler->handler)
            {
                if (handler->handler(rx_buf[rx_head].raw) == MAC_CMD_PROCESSED)
                {
                    goto next;
                }
            }
        }

        uint8_t *buf = MAC_RX_PKT(rx_buf[rx_head].raw);
        uint8_t buf_len = MAC_RX_PKT_LEN(rx_buf[rx_head].raw);
        uint8_t lqi = MAC_RX_PKT_LQI(rx_buf[rx_head].raw);
        int8_t rssi = MAC_RX_PKT_RSSI(rx_buf[rx_head].raw);

        for (uint32_t i = 0; i < buf_len; i++)
        {
            dbg_printf("%02x ", buf[i]);
        }

        dbg_printf("LQI %u ", lqi);
        dbg_printf("RSSI %d\r\n", rssi);
#if Auto_test
        mac_rxfifo_tail_t *info = MAC_RX_PKT_INFO(rx_buf[rx_head].raw);
        unsigned char bytes[] = {0x04, 0xff, 0x06, 0x01, lqi, info->mac_time >> 24, info->mac_time >> 16, info->mac_time >> 8, info->mac_time};
        UART_SendData(UART2, bytes, sizeof(bytes));
#endif

next:
        lock = os_lock();
        rx_head = (rx_head + 1) % RX_BUF_SIZE;
        os_unlock(lock);
    }
}

static tx_buf_t g_enh_ack_buf;
static uint32_t sMacFrameCounter = 0;

//TXNTERRIF

// TXNIF
void txn_handler(uint8_t pan_idx, uint32_t arg)
{
    uint8_t tx_status = mac_txn_status_get();
    if (mac_GetTxAtStatus())
    {
        g_tx_done = TX_AT_FAIL;
        g_iftxstat.errors++;
    }
    else if (tx_status & 0x1)
    {
        if (tx_status & 0x20)
        {
            g_tx_done = TX_BUSY;
            g_iftxstat.collisions++;
        }
        else
        {
            g_tx_done = TX_NOACK;
            g_iftxstat.noack++;
        }
    }
    else
    {
        g_tx_done = TX_SUCCESS;
        g_iftxstat.packets++;
    }
}

// TXG1IF

// TXG2IF

// EXELYIF
void rxely_handler(uint8_t pan_idx, uint32_t arg)
{
    uint16_t panid = 0;
    uint16_t saddr = 0;
    uint64_t laddr = 0;
    uint8_t *daddr = NULL;
    fc_t fc_ack = {0};
    aux_t aux = {0};

#if GPIO_DEBUG
    GPIO_WriteBit(GPIO_PIN_OUTPUT_1, (BitAction)(1));
    GPIO_WriteBit(GPIO_PIN_OUTPUT_1, (BitAction)(0));
#endif
    if (mac_rx_frm_version_get() == FRAME_VER_2015 && mac_rx_frm_ack_req_get() == 1)
    {
        fc_ack.type = FRAME_TYPE_ACK;
        fc_ack.ver = FRAME_VER_2015;
        fc_ack.sec_en = mac_rx_frm_sec_en_get();
        fc_ack.seq_num_suppress = mac_rx_frm_seq_compress_get();
        //fc_ack.src_addr_mode = 0;
        fc_ack.dst_addr_mode = mac_rx_frm_src_addr_mode_get();

        if (fc_ack.sec_en)
        {
            aux.sec_ctl.sec_level = mac_rx_frm_sec_level_get();
            aux.sec_ctl.key_id_mode = mac_rx_frm_sec_keyid_mode_get();
            aux.frame_counter = sMacFrameCounter;
            aux.key_id = mac_rx_frm_sec_keyid_get();
        }

        if (!mac_rx_frm_panid_compress_get())
        {
            panid = mac_panid_get();
        }

        if (fc_ack.dst_addr_mode == ADDR_MODE_SHORT)
        {
            saddr = mac_rx_frm_short_addr_get();
            daddr = (uint8_t *)&saddr;
        }
        else if (fc_ack.dst_addr_mode == ADDR_MODE_EXTEND)
        {
            laddr = mac_rx_frm_long_addr_get();
            daddr = (uint8_t *)&laddr;
        }
        g_enh_ack_buf.len = generate_ieee_frame(FRAME_TYPE_ACK, g_enh_ack_buf.buf, 125, fc_ack,
                                                mac_rx_frm_seq_get(),
                                                NV_FIELD, NV_FIELD, panid, daddr,
                                                (uint8_t *)&aux, sizeof(aux), NV_FIELD, NV_FIELD);

        if (fc_ack.sec_en)
        {
            nonce_t nonce = {0};
            mac_memcpy(&nonce.src_ext_addr, mac_long_addr_get(), sizeof(nonce.src_ext_addr));
            nonce.sec_level = aux.sec_ctl.sec_level;
            nonce.frame_counter = aux.frame_counter;

            sMacFrameCounter++;
            // set nonce
            mac_nonce_set((uint8_t *)&nonce);
            // set key
            mac_tx_enh_ack_key_set(g_mac_key);
            // set security level
            mac_tx_enh_ack_cipher_set(mac_rx_frm_sec_level_get());
        }

        mac_tx_enh_ack_payload_set(fc_ack.sec_en ? g_enh_ack_buf.len : 0, g_enh_ack_buf.len,
                                   g_enh_ack_buf.buf);

        if (g_enh_ack_early)
        {
            mac_tx_enh_ack_trig(TRUE, fc_ack.sec_en);
        }
        else
        {
            mac_tx_enh_ack_set_pending(TRUE);  // set enh-ack tx trigger is pending
        }
    }
    g_recent_rx_timestamp = mac_btus_get();
}

// SECIF

// RXIF
void rxdone_handler(uint8_t pan_idx, uint32_t arg)
{
    uint8_t enh_ack_tx_sts = MAC_STS_SUCCESS;

    mac_rx(rx_buf[rx_tail].raw);
    rx_tail = (rx_tail + 1) % RX_BUF_SIZE;

    if (mac_tx_enh_ack_get_pending())
    {
        if (g_test_enh_ack_late)
        {
            while (mac_tx_enh_ack_state_get());
            enh_ack_tx_sts = mac_tx_enh_ack_trig(FALSE, mac_rx_frm_sec_en_get());
        }
        else
        {
            enh_ack_tx_sts = mac_tx_enh_ack_trig(FALSE, mac_rx_frm_sec_en_get());
        }

        if (enh_ack_tx_sts == MAC_STS_TIMEOUT)
        {
#if Auto_test
            unsigned char bytes[] = {0x04, 0xff, 0x02, 0x01, 0x01};
            UART_SendData(UART2, bytes, sizeof(bytes));
#endif
        }
    }
    os_task_notify_give(rx_task_handle);
}

// BTCMP0IF
void btcmp0_handler(uint8_t pan_idx, uint32_t arg)
{
    os_sem_give(zb_sem);
}

void btcmp1_handler(uint8_t pan_idx, uint32_t arg)
{
}

void edscan_handler(uint8_t pan_idx, uint32_t arg)
{
}

#if CONFIG_SOC_SERIES_RTL87X2H
extern void modem_set_zb_cca_combination_rom(uint8_t comb);
#define set_zb_cca_combination modem_set_zb_cca_combination_rom
#else
extern void modem_set_zb_cca_combination(uint8_t comb);
#define set_zb_cca_combination modem_set_zb_cca_combination
#endif

void zb_mac_interrupt_enable(void)
{
    //NVIC_InitTypeDef NVIC_InitStruct;
    // TODO: enable MAC interrupt
    /* share the same IRQ number with BT_MAC on FPGA temporary, so the interrupt
       shall be initialed in BT lower stack initialization */
    NVIC_SetPriority(Zigbee_IRQn, 2);
    NVIC_EnableIRQ(Zigbee_IRQn);
#if defined(CONFIG_SOC_SERIES_RTL87X2G) || defined(CONFIG_SOC_SERIES_RTL87X2H)
    RamVectorTableUpdate(Zigbee_VECTORn, Zigbee_Handler_Patch);
    DBG_DIRECT("RamVectorTableUpdate");
#else
    uint32_t zigbee_vector_no = IRQn_TO_VECTORn(Zigbee_IRQn);
    RamVectorTableUpdate(zigbee_vector_no, Zigbee_Handler_Patch);
#endif
}

void zb_mac_drv_init(void)
{
    mac_attribute_init(&g_mac_attribute);
    g_mac_attribute.mac_cfg.rf_early_term = 0;
    //g_mac_attribute.mac_cfg.frm06_rx_early = 0;
#ifdef BOARD_RTL8771HTV
    g_mac_attribute.phy_arbitration_en = 0;
#else
    lowerstack_SystemCall(10, 1, 512, -1);
#endif
    mac_enable();
    mac_init(&g_mac_driver, &g_mac_attribute);
    mac_init_ext();
#if defined(CONFIG_SOC_SERIES_RTL87X2G) || defined(CONFIG_SOC_SERIES_RTL87X2H)
    // reinit pm state
    PMUnitStatus state = power_manager_interface_get_unit_status(PM_SLAVE_ZIGBEE, PM_UNIT_ZIGBEE);
    if (state != PM_UNIT_ACTIVE)
    {
        PowerManagerSlaveUnit *zbmac = power_manager_slave_get_unit(PM_UNIT_ZIGBEE);
        if (zbmac)
        {
            dbg_printf("re-register 15.4 power unit\r\n");
            power_manager_slave_register_unit(PM_UNIT_ZIGBEE, zbmac);
        }
    }
    // wake up BT
    state = power_manager_interface_get_unit_status(PM_SLAVE_BTMAC, PM_UNIT_BTMAC);
    if (state != PM_UNIT_ACTIVE)
    {
        dbg_printf("wakeup BT\r\n");
        power_manager_interface_check_unit_active(PM_SLAVE_BTMAC, PM_UNIT_BTMAC);
        mac_radio_off();
        mac_radio_on();
    }
#endif
    mac_cca_mode_set(MAC_CCA_ED);
    mac_txn_csma_set(true);
    mac_txn_retry_set(3);
    mac_panid_set(0x5);
    mac_channel_set(12);
    mac_short_addr_set(0x1);
    debug_gpio_init();
}

bitmap_t *bitmap_alloc(uint16_t total_blocks)
{
    bitmap_t *bitmap = (bitmap_t *)malloc(sizeof(bitmap_t));
    if (!bitmap)
    {
        return NULL;
    }
    bitmap->total_blocks = total_blocks;
    bitmap->received_blocks = 0;
    // calculate bytes
    uint16_t bytes_needed = (total_blocks + 7) / 8;
    bitmap->bits = (uint8_t *)malloc(bytes_needed);
    if (bitmap->bits)
    {
        memset(bitmap->bits, 0, bytes_needed); // init
    }
    else
    {
        free(bitmap);
        return NULL;
    }
    return bitmap;
}

int bitmap_get(const bitmap_t *bitmap, uint16_t id)
{
    if (!bitmap || id >= bitmap->total_blocks)
    {
        return -1;
    }
    uint16_t byte_index = id / 8;
    uint16_t bit_index = id % 8;

    return (bitmap->bits[byte_index] & (1 << bit_index)) ? 1 : 0;
}

void bitmap_set(bitmap_t *bitmap, uint16_t id)
{
    if (!bitmap || id >= bitmap->total_blocks)
    {
        return;
    }
    uint16_t byte_index = id / 8;
    uint16_t bit_index = id % 8;
    if ((bitmap->bits[byte_index] & (1 << bit_index)) == 0)
    {
        bitmap->bits[byte_index] |= (1 << bit_index);
        bitmap->received_blocks++;
    }
}

static zbpm_callback_t g_app_zbpm_exit = NULL;
int g_zbpm_wakeup_diff_min = INT_MAX;
int g_zbpm_wakeup_diff_max = INT_MIN;
int g_zbpm_wakeup_diff_avg = 0;
#if defined(CONFIG_SOC_SERIES_RTL87X2G) || defined(CONFIG_SOC_SERIES_RTL87X2H)
#if GPIO_DEBUG
static void default_zbpm_exit(void)
{
    //dbg_printf("exit_callback\r\n");
    GPIO_WriteBit(GPIO_PIN_OUTPUT_0, (BitAction)(1));
    //toggle_gpio(GPIO_PIN_OUTPUT_1);
}
#endif

void zbmac_power_manager_init(zbpm_callback_t exit_callback)
{
    volatile zbpm_adapter_t *padapter = &g_zbpm_adap;
    g_app_zbpm_exit = exit_callback;
    if (TRUE == g_zbpm_inited)
    {
        return;
    }
    g_zbpm_inited = TRUE;
    padapter->power_mode = ZBMAC_ACTIVE;
    padapter->wakeup_reason = ZBMAC_PM_WAKEUP_UNKNOWN;
    padapter->error_code = ZBMAC_PM_ERROR_UNKNOWN;

    padapter->stage_time[ZBMAC_PM_CHECK] = 20;
    padapter->stage_time[ZBMAC_PM_STORE] = 15;
    padapter->stage_time[ZBMAC_PM_ENTER] = 5;
    padapter->stage_time[ZBMAC_PM_EXIT] = 5;
    padapter->stage_time[ZBMAC_PM_RESTORE] = 20;
    padapter->minimum_sleep_time = 20;
    padapter->learning_guard_time = 7; // 3 (learning guard time) + 4 (two 16k po_intr drift)

    padapter->cfg.wake_interval_en = 0;
    padapter->cfg.stage_time_learned = 0;
    padapter->wakeup_time_us = mac_btus_get();
    padapter->wakeup_interval_us = 0;
    //padapter->enter_callback = zbpm_enter;
#if GPIO_DEBUG
    padapter->exit_callback = default_zbpm_exit;
#endif

    dbg_printf("zbmac_pm_init\r\n");
    zbmac_pm_init((zbpm_adapter_t *)padapter);
}
#endif

int zbmac_power_manager_set(uint32_t next, uint32_t period)
{
    if (FALSE == g_zbpm_inited)
    {
        dbg_printf("fail: not initiated\r\n");
        return FALSE;
    }
#if defined(CONFIG_SOC_SERIES_RTL87X2G) || defined(CONFIG_SOC_SERIES_RTL87X2H)
    volatile zbpm_adapter_t *padapter = &g_zbpm_adap;

    if (padapter->power_mode == ZBMAC_DEEP_SLEEP)
    {
        dbg_printf("fail: still in sleep state\r\n");
        return FALSE;
    }

    padapter->cfg.wake_interval_en = period ? 1 : 0;
    padapter->wakeup_time_us = mac_btus_get() + next;
    padapter->wakeup_interval_us = period;
    padapter->wakeup_reason = ZBMAC_PM_WAKEUP_UNKNOWN;
    padapter->error_code = ZBMAC_PM_ERROR_UNKNOWN;
    padapter->power_mode = ZBMAC_DEEP_SLEEP;
    /*
     * DLPS procedure: store->enter->exit->restore
    */
    next = (next + 999) / 1000; // ms
    os_delay(next);
    if (padapter->power_mode == ZBMAC_DEEP_SLEEP)
    {
        //dbg_printf("warn: still in sleep state. error_code = %u wakeup_reason = %u\r\n", padapter->error_code, padapter->wakeup_reason);
        if (padapter->error_code != ZBMAC_PM_ERROR_UNKNOWN)
        {
            dbg_printf("ZbPmSet: not sleep. force change state to active\r\n");
            padapter->power_mode = ZBMAC_ACTIVE;
        }
        uint64_t now = os_sys_time_get();
        while (padapter->power_mode == ZBMAC_DEEP_SLEEP)
        {
            if (now + 2 < os_sys_time_get()) // Avoid unknown errors and getting stuck in loops
            {
                dbg_printf("ZbPmSet: wait wake_up timeout. mac_enable = %u error_code = %u wakeup_reason = %u\r\n",
                           mac_enabled_check(), padapter->error_code, padapter->wakeup_reason);
                padapter->power_mode = ZBMAC_ACTIVE;
                break;
            }
        }
        //dbg_printf("exit sleep state\r\n");
    }

    int new_delay = ((int)(mac_btus_get() - padapter->wakeup_time_us)) << 7;
    if (new_delay < g_zbpm_wakeup_diff_min)
    {
        g_zbpm_wakeup_diff_min = new_delay;
    }
    if (new_delay > g_zbpm_wakeup_diff_max)
    {
        g_zbpm_wakeup_diff_max = new_delay;
    }
    if (g_zbpm_wakeup_diff_avg == 0)
    {
        g_zbpm_wakeup_diff_avg = new_delay;
    }
    else
    {
        g_zbpm_wakeup_diff_avg = (g_zbpm_wakeup_diff_avg * 7 + new_delay) >> 3;
    }

    //dbg_printf("new: %d avg: %d\r\n", new_delay, g_zbpm_wakeup_diff_avg);
    if (g_app_zbpm_exit)
    {
        g_app_zbpm_exit();
    }
    return TRUE;
#else
    dbg_printf("not support\r\n");
    return FALSE;
#endif
}

typedef void (*bt_hci_reset_handler_t)(void);
extern void mac_RegisterBtHciResetHanlder(bt_hci_reset_handler_t handler);
void zb_mac_drv_enable(void)
{
    mpan_CommonInit(&g_pan_mac_comm);
    zb_mac_drv_init();
#if F_BT_DLPS_EN
    zbmac_power_manager_init(NULL);
#endif
    mpan_RegisterISR(0, txn_handler, rxely_handler, rxdone_handler, edscan_handler);
    mpan_RegisterTimer(0, MAC_BT_TIMER0, btcmp0_handler, 0);
    mpan_RegisterTimer(0, MAC_BT_TIMER1, btcmp1_handler, 0);
    mpan_EnableCtl(0, 1);
    mac_RegisterBtHciResetHanlder(zb_mac_drv_init);
    mac_callback_register(NULL, edscan_lv2dbm, set_zigbee_priority,
                          set_zb_cca_combination);
}

#if GPIO_DEBUG
void debug_gpio_init(void)
{
    Pad_Config(GPIO_OUTPUT_PIN_0, PAD_PINMUX_MODE, PAD_IS_PWRON, PAD_PULL_NONE, PAD_OUT_ENABLE,
               PAD_OUT_HIGH);
    Pinmux_Config(GPIO_OUTPUT_PIN_0, DWGPIO);
    RCC_PeriphClockCmd(APBPeriph_GPIO, APBPeriph_GPIO_CLOCK, ENABLE);
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_StructInit(&GPIO_InitStruct);
    GPIO_InitStruct.GPIO_Pin    = GPIO_PIN_OUTPUT_0;
    GPIO_InitStruct.GPIO_Mode   = GPIO_Mode_OUT;
    GPIO_InitStruct.GPIO_ITCmd  = DISABLE;
    GPIO_Init(&GPIO_InitStruct);
    GPIO_WriteBit(GPIO_PIN_OUTPUT_0, (BitAction)(0));

    Pad_Config(GPIO_OUTPUT_PIN_1, PAD_PINMUX_MODE, PAD_IS_PWRON, PAD_PULL_NONE, PAD_OUT_ENABLE,
               PAD_OUT_HIGH);
    Pinmux_Config(GPIO_OUTPUT_PIN_1, DWGPIO);
    GPIO_StructInit(&GPIO_InitStruct);
    GPIO_InitStruct.GPIO_Pin    = GPIO_PIN_OUTPUT_1;
    GPIO_InitStruct.GPIO_Mode   = GPIO_Mode_OUT;
    GPIO_InitStruct.GPIO_ITCmd  = DISABLE;
    GPIO_Init(&GPIO_InitStruct);
    GPIO_WriteBit(GPIO_PIN_OUTPUT_1, (BitAction)(0));
}

void toggle_gpio(uint32_t GPIO_Pin)
{
    uint8_t output_bit = GPIO_ReadOutputDataBit(GPIO_Pin);

    if (output_bit)
    {
        GPIO_WriteBit(GPIO_Pin, (BitAction)(0));
    }
    else
    {
        GPIO_WriteBit(GPIO_Pin, (BitAction)(1));
    }
}
#endif
