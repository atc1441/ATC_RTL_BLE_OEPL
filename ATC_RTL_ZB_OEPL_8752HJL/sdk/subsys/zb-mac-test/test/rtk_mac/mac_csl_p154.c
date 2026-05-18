/**
*****************************************************************************************
*     Copyright(c) 2017, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
   * @file      mac_csl_p154.c
   * @brief     example of CSL demo
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
#include <stdio.h>
#include "shell.h"
#include "dbg_printf.h"
#include "mac_driver_interface.h"
#include "mac_802154_frame_parser.h"
#include "mac_test_common.h"
#include "mac_csl.h"
#if defined(CONFIG_SOC_SERIES_RTL87X2G) || defined(CONFIG_SOC_SERIES_RTL87X2H)
#include "power_manager_unit_zbmac.h"
#endif
#if GPIO_DEBUG
#include "rtl876x_gpio.h"
#endif

static mac_timer_handle_t g_csl_ctimer_pool;
static uint8_t g_csl_tx_seq = 0;

static void p154_csma_arq_enable(void)
{
    dbg_printf("enable csma and arq\r\n");
    mac_cca_mode_set(MAC_CCA_CS);
    mac_txn_csma_set(true);
    mac_txn_retry_set(3);
}

static void p154_csma_arq_disable(void)
{
    dbg_printf("disable csma and arq\r\n");
    g_csl_tx_seq = 1;
    clear_tx_stat();
    mac_cca_mode_set(MAC_CCA_NONE);
    mac_txn_csma_set(false);
    mac_txn_retry_set(0);
    if (g_csl_role == CSL_ROLE_COORD)
    {
        g_csl_tx_check_offest = ((g_csl_listen_window - 400) >> 1) - 760;
    }
}

static void p154_config_init(void)
{
    if (g_csl_role == CSL_ROLE_ENDPOINT)
    {
        g_csl_ppm = -3;
        g_csl_prepare_to_rx = 400;
        g_csl_ack_require_time = 900;
    }
    else
    {
        //g_csl_tx_check_offest = -740;
    }
}

static int p154_send(uint8_t *data, uint8_t data_len, uint32_t *tx_timestamp)
{
    fc_t fc = {0};
    uint16_t panid = mac_panid_get();
    uint16_t saddr = mac_short_addr_get();

    fc.type = FRAME_TYPE_COMMAND;
    fc.sec_en = 0;
    fc.pending = 0;
    fc.ack_req = 1;
    fc.panid_compress = 1;
    fc.dst_addr_mode = ADDR_MODE_SHORT;
    fc.ver = FRAME_VER_2006;
    fc.src_addr_mode = ADDR_MODE_SHORT;
    g_tx_buf.len = generate_ieee_frame(FRAME_TYPE_COMMAND, g_tx_buf.buf, 125, fc, g_csl_tx_seq,
                                       panid, (uint8_t *)&saddr, panid, (uint8_t *)&g_csl_peer_addr,
                                       NV_FIELD, NV_FIELD, MAC_CMD_CSL, NV_FIELD);
    CPY_MV_PTR(&g_tx_buf.buf[g_tx_buf.len], data, data_len, g_tx_buf.len);
    mac_txn_payload_set(0, g_tx_buf.len, g_tx_buf.buf);
    RESET_TXDOWN();
#if GPIO_DEBUG
    GPIO_WriteBit(GPIO_PIN_OUTPUT_1, (BitAction)(1));
#endif
    mac_txn_trig(fc.ack_req, fc.sec_en);
    trigger_gpio_at_anchor(0);
    WAIT_FOR_TXDOWN();
#if GPIO_DEBUG
    GPIO_WriteBit(GPIO_PIN_OUTPUT_1, (BitAction)(0));
#endif
    dbg_printf("send ");
    print_tx_result(g_csl_tx_seq, 0xff);
    g_csl_tx_seq++;
    if (g_tx_done == TX_SUCCESS)
    {
        // get tx time as csl anchor point
        *tx_timestamp = mac_txn_timestamp_get();
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

static int p154_input(uint8_t *rx_data)
{
    // depend on rx_data format
    uint8_t *buf = MAC_RX_PKT(rx_data);
    uint8_t buf_len = MAC_RX_PKT_LEN(rx_data);
    uint32_t rx_timestamp = MAC_RX_PKT_TIMESTAMP(rx_data);
    uint8_t hdr_len = mac_802154_frame_parser_addressing_end_offset_get(rx_data) + 1;
    bool is_src_addr_extended = false;
    uint16_t saddr = *((uint16_t *)(mac_802154_frame_parser_src_addr_get(rx_data,
                                                                         &is_src_addr_extended)));

    uint8_t *optptr = rx_data + hdr_len;
    uint8_t *end = buf + buf_len;
    csl_input(optptr, end - optptr, rx_timestamp, saddr);
    //dbg_mem_dump(optptr, end - optptr);
    return MAC_CMD_PROCESSED;
}

MAC_CMD_HANDLER(csl_handler, MAC_CMD_CSL, p154_input);

static void *p154_timer_init(void)
{
    mac_sw_timer_init(&g_csl_ctimer_pool, 1);
    return (void *)mac_sw_timer_alloc();
}

static void p154_timer_start(void *timer, uint32_t timeout, void *callback, void *arg)
{
    mac_sw_timer_start((pmac_timer_handle_t)timer, timeout, callback, arg);
}

static void p154_timer_stop(void *timer)
{
    mac_sw_timer_stop((pmac_timer_handle_t)timer);
}

static void p154_pm_init(void *pm_exit_cb)
{
    zbmac_power_manager_init((zbpm_callback_t)pm_exit_cb);
}

static int p154_pm_set(uint32_t time)
{
    return zbmac_power_manager_set(time, 0);
}

void p154_init(void)
{
    mac_panid_set(0x89);
    mac_channel_set(14);
    //mac_cca_mode_set(MAC_CCA_CS);
    mac_cca_ed_threshold_set(60);
    // register receive packet handler
    mac_cmd_register_handler(&csl_handler);
    // register protocol function table
    PROTOCOL.id = PROTOCOL_154;
    PROTOCOL.get_curr_timestamp = mac_btus_get;
    PROTOCOL.timer_init = p154_timer_init;
    PROTOCOL.timer_stop = p154_timer_stop;
    PROTOCOL.timer_start = p154_timer_start;
    PROTOCOL.send = p154_send;
    PROTOCOL.start_periodic_tx_sched_cb = p154_csma_arq_disable;
    PROTOCOL.stop_periodic_tx_sched_cb = p154_csma_arq_enable;
    PROTOCOL.pm_init = p154_pm_init;
    PROTOCOL.pm_set = p154_pm_set;
    PROTOCOL.config_init = p154_config_init;
    dbg_printf("Protocol IEEE 802.15.4 initialization done\r\n");
    dbg_printf("Use panid 0x%x, channel %u, address 0x%x\r\n", mac_panid_get(),
               mac_channel_get(), mac_short_addr_get());
}
