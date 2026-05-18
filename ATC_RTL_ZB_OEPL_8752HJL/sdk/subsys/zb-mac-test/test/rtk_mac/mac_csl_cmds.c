/**
*****************************************************************************************
*     Copyright(c) 2017, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
   * @file      mac_csl_cmds.c
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
#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <os_msg.h>
#include <os_task.h>
#include "shell.h"
#include "dbg_printf.h"
#include "strproc.h"
#include "mac_test_common.h"
#include "mac_csl.h"
#include "crc16btx.h"

#define OPTION_CSL_NONE         0x0
#define OPTION_CSL_SYN          0x1
#define OPTION_CSL_WAKEUP_REQ   0x2
#define OPTION_CSL_WAKEUP_REP   0x3
#define OPTION_CSL_SLEEP_REQ    0x4
#define OPTION_CSL_SLEEP_REP    0x5 // not used, send OPTION_CSL_SYN instead
#define OPTION_CSL_DATA         0x6
#define OPTION_CSL_FRAG         0x7

#define CSL_EV_NONE             0x0
#define CSL_EV_LISTEN_END       0x1
#define CSL_EV_PM_EXIT          0x2
#define CSL_EV_INDIRECT_SEND    0x3
#define CSL_EV_DIRECT_SEND      0x4
#define CSL_EV_WAKEUP_REQ       0x5
#define CSL_EV_SLEEP_REQ        0x6
#define CSL_EV_DATA_SEND        0x7

#define MAX_QUEUE_BUF_LEN 16
#define MAX_QUEUE_PACKETS 5

#define MAX_RETRY 3
#define PPM 1000000
#define TASK_NOTIFY_DELAY 34 // us

#define BLOCK_SIZE 64

enum CSL_SCHE_STATE
{
    CSL_SCHE_STOP        = 0,
    CSL_SCHE_ENTR_PM     = 1,
    CSL_SCHE_EXIT_PM     = 2,
    CSL_SCHE_EV_PM       = 3,
    CSL_SCHE_ENTR_TM     = 4,
    CSL_SCHE_EXIT_TM     = 5,
    CSL_SCHE_EV_TM       = 6,
};

struct option_head
{
    uint8_t option_type;
    uint8_t option_len;
} __attribute__((packed));

struct option_csl_sync
{
    struct option_head head;
    uint32_t csl_period;
    uint32_t csl_listen_window;
} __attribute__((packed));

struct option_csl_frag
{
    struct option_head head;
    uint16_t frag_crc;
    uint16_t total_len;
    uint16_t m_flag : 1;
    uint16_t block_id : 15;
} __attribute__((packed));

struct queuebuf
{
    uint8_t buf[MAX_QUEUE_BUF_LEN];
    uint8_t len;
};

struct packet_queue
{
    struct packet_queue *next;
    struct queuebuf buf;
    void *ptr;
    uint8_t len;
};

struct protocol_fn PROTOCOL = {0};
uint16_t g_csl_peer_addr;
static void *g_csl_ctimer = NULL;
static buf_t g_csl_buf;
static uint32_t g_csl_anchor_timestamp = 0;
static volatile uint32_t g_csl_new_anchor_timestamp = 0;
static volatile bool g_csl_schedule_stop = TRUE;
static void *g_csl_ev_queue_handle;
static void *g_csl_task_handle;
MEMB(packet_memb, struct packet_queue, MAX_QUEUE_PACKETS);
LIST(g_csl_queue_list);
static uint32_t g_csl_period; // us
static uint32_t g_csl_recent_sync_timestamp = 0;
static uint8_t g_csl_sche_state = CSL_SCHE_STOP;
static uint32_t g_csl_dbg_counter = 0;
// config
int g_csl_ppm = 0;
uint8_t g_csl_role = CSL_ROLE_INVALID;
int g_csl_tx_check_offest = 0;
uint32_t g_csl_ack_require_time = 0;
uint32_t g_csl_prepare_to_rx = 0;
uint32_t g_csl_listen_window; // us
static uint32_t g_csl_autosync_period = 0;
static uint32_t g_csl_resync_req = 0;
static uint32_t g_csl_conf_period; // us
static bool g_csl_loop_test = TRUE;
static bool g_csl_debug_timing = FALSE;
static bool g_csl_debug = FALSE;
#define MAX_TEST_DATA (MAX_QUEUE_BUF_LEN - 2)
static uint8_t g_csl_test_data[MAX_TEST_DATA];

// add frag
static uint8_t g_image[8000];
static uint16_t g_frag_crc = 0;
static bitmap_t *g_frag_bitmap = NULL;

#if GPIO_DEBUG
#include "rtl876x_gpio.h"
void trigger_gpio_at_anchor(bool reverse)
{
    if (PROTOCOL.get_curr_timestamp() > g_csl_anchor_timestamp)
    {
        return;
    }

    while (g_csl_anchor_timestamp > PROTOCOL.get_curr_timestamp());
    if (reverse)
    {
        GPIO_WriteBit(GPIO_PIN_OUTPUT_0, (BitAction)(0));
        GPIO_WriteBit(GPIO_PIN_OUTPUT_0, (BitAction)(1));
    }
    else
    {
        GPIO_WriteBit(GPIO_PIN_OUTPUT_0, (BitAction)(1));
        GPIO_WriteBit(GPIO_PIN_OUTPUT_0, (BitAction)(0));
    }
}
#endif

static uint8_t *create_csl_msg(uint8_t *msgptr)
{
    return msgptr;
}

static uint8_t *add_option_csl_sync(uint8_t *optptr, uint32_t period, uint32_t listen_window)
{
    struct option_csl_sync *option_csl_sync = (struct option_csl_sync *)optptr;
    option_csl_sync->head.option_type = OPTION_CSL_SYN;
    option_csl_sync->head.option_len = 8;
    option_csl_sync->csl_period = period;
    option_csl_sync->csl_listen_window = listen_window;
    return optptr + sizeof(struct option_csl_sync);
}

static uint8_t *add_option_csl_data(uint8_t *optptr, uint8_t *data, uint8_t len)
{
    struct option_head *option_head = (struct option_head *)optptr;
    option_head->option_type = OPTION_CSL_DATA;
    option_head->option_len = len;
    memcpy(optptr + sizeof(struct option_head), data, len);
    return optptr + sizeof(struct option_head) + len;
}

static uint8_t *add_option_csl_frag(uint8_t *optptr, uint8_t *data, uint16_t frag_crc,
                                    uint16_t total_len,
                                    uint8_t m_flag, uint16_t block_id, uint8_t data_len)
{
    struct option_csl_frag *option_csl_frag = (struct option_csl_frag *)optptr;
    option_csl_frag->head.option_type = OPTION_CSL_FRAG;
    option_csl_frag->head.option_len = sizeof(struct option_csl_frag) - sizeof(
                                           struct option_head) + data_len;
    option_csl_frag->frag_crc = frag_crc;
    option_csl_frag->total_len = total_len;
    option_csl_frag->m_flag = m_flag;
    option_csl_frag->block_id = block_id;
    memcpy(optptr + sizeof(struct option_csl_frag), data, data_len);
    return optptr + sizeof(struct option_csl_frag) + data_len;
}

static uint8_t *add_option_csl_no_data_command(uint8_t *optptr, uint8_t type)
{
    struct option_head *option_head = (struct option_head *)optptr;
    option_head->option_type = type;
    option_head->option_len = 0;
    return optptr + sizeof(struct option_head);
}

static void csl_timer_cb(void *arg)
{
    g_csl_sche_state = CSL_SCHE_EXIT_TM;
    uint8_t event = (g_csl_role == CSL_ROLE_ENDPOINT) ? CSL_EV_LISTEN_END : CSL_EV_INDIRECT_SEND;
    os_msg_send(g_csl_ev_queue_handle, &event, 0);
    //if (g_csl_debug == TRUE)
    //    dbg_printf("csl_timer_cb ev %u\r\n", event);
}

static void csl_schedule_periodic_rx(void)
{
    if (g_csl_schedule_stop == TRUE)
    {
        dbg_printf("Wake up on coordinator request\r\n");
        return;
    }

    if (g_csl_new_anchor_timestamp)
    {
        dbg_printf("Coordinator request re-sync.\r\n");
        g_csl_anchor_timestamp = g_csl_new_anchor_timestamp;
        g_csl_new_anchor_timestamp = 0;
    }

    uint32_t now = PROTOCOL.get_curr_timestamp();
    if (g_csl_debug_timing)
    {
        dbg_printf("sleep %u diff %u recent %u\r\n", now, now - g_csl_anchor_timestamp,
                   now - g_recent_rx_timestamp);
    }

    bool close_to_32_max = (now + g_csl_period) < now ? 1 : 0;
    while (g_csl_anchor_timestamp < now)
    {
        uint32_t new_anchor = g_csl_anchor_timestamp + g_csl_period;
        if (close_to_32_max == 1)
        {
            if (new_anchor < g_csl_anchor_timestamp)
            {
                g_csl_anchor_timestamp = new_anchor;
                break;
            }
        }
        g_csl_anchor_timestamp = new_anchor;
    }

    if (g_csl_anchor_timestamp < now + g_csl_prepare_to_rx + 6000) // add more 6 ms guard time
    {
        g_csl_anchor_timestamp += g_csl_period;
    }

    if (PROTOCOL.timer_stop)
    {
        PROTOCOL.timer_stop(g_csl_ctimer);
    }
#if GPIO_DEBUG
    GPIO_WriteBit(GPIO_PIN_OUTPUT_0, (BitAction)(0));
#endif
    if (g_csl_debug == TRUE)
    {
        dbg_printf("entr_pm %u\r\n", g_csl_anchor_timestamp - PROTOCOL.get_curr_timestamp() -
                   g_csl_prepare_to_rx);
    }

    g_csl_sche_state = CSL_SCHE_ENTR_PM;
    g_csl_dbg_counter = g_csl_anchor_timestamp - PROTOCOL.get_curr_timestamp() - g_csl_prepare_to_rx;
    if (PROTOCOL.pm_set(g_csl_dbg_counter) == FALSE)
    {
        dbg_printf("pm_set fail\r\n");
    }
}

static void csl_schedule_periodic_tx(void)
{
    if (g_csl_schedule_stop == TRUE)
    {
        dbg_printf("The endpoint has woken up and terminated the tx scheduler\r\n");
        return;
    }

    uint32_t now = PROTOCOL.get_curr_timestamp();
    bool close_to_32_max = (now + g_csl_period) < now ? 1 : 0;
    while (g_csl_anchor_timestamp < now)
    {
        uint32_t new_anchor = g_csl_anchor_timestamp + g_csl_period;
        if (close_to_32_max == 1)
        {
            if (new_anchor < g_csl_anchor_timestamp)
            {
                g_csl_anchor_timestamp = new_anchor;
                break;
            }
        }
        g_csl_anchor_timestamp = new_anchor;
    }

    if (g_csl_anchor_timestamp + g_csl_tx_check_offest < now + 20)
    {
        g_csl_anchor_timestamp += g_csl_period;
    }

    if (PROTOCOL.timer_stop)
    {
        PROTOCOL.timer_stop(g_csl_ctimer);
    }
    if (PROTOCOL.timer_start)
    {
        g_csl_sche_state = CSL_SCHE_ENTR_TM;
        g_csl_dbg_counter = g_csl_anchor_timestamp + g_csl_tx_check_offest - now;
        PROTOCOL.timer_start(g_csl_ctimer, g_csl_anchor_timestamp + g_csl_tx_check_offest,
                             csl_timer_cb, 0);
    }
    //if (g_csl_debug_timing == TRUE)
    //    dbg_printf("schedule tx after %u us\r\n", g_csl_anchor_timestamp + g_csl_tx_check_offest - now);
}

static void csl_pm_exit(void)
{
    g_csl_sche_state = CSL_SCHE_EXIT_PM;
#if GPIO_DEBUG
    GPIO_WriteBit(GPIO_PIN_OUTPUT_0, (BitAction)(0));
#endif
    if (PROTOCOL.pm_exit_cb)
    {
        PROTOCOL.pm_exit_cb();
    }
    uint8_t event = CSL_EV_PM_EXIT;
    os_msg_send(g_csl_ev_queue_handle, &event, 0);
}

void csl_tx_queue_add(uint8_t option, uint8_t *data, uint8_t data_len)
{
    uint8_t *end;
    if (data)   // length check
    {
        if (data_len + sizeof(struct option_head) > MAX_QUEUE_BUF_LEN)
        {
            dbg_printf("packet too long\r\n");
            return;
        }
    }

    // buffer alloc
    struct packet_queue *packet = memb_alloc(&packet_memb);
    if (packet == NULL)
    {
        dbg_printf("no buffer\r\n");
        return;
    }
    memset(packet, 0, sizeof(struct packet_queue));
    uint8_t *buf = packet->buf.buf;

    end = create_csl_msg(buf);
    if (data)
    {
        end = add_option_csl_data(end, data, data_len);
    }
    else
    {
        end = add_option_csl_no_data_command(end, option);
    }
    packet->buf.len = end - packet->buf.buf;
    // append to queue list
    list_add(g_csl_queue_list, packet);
    if (g_csl_schedule_stop == TRUE)
    {
        uint8_t event = CSL_EV_DIRECT_SEND;
        os_msg_send(g_csl_ev_queue_handle, &event, 0);
    }
}

static void csl_sync_process()
{
    uint8_t *end;
    uint32_t timestamp;

    end = create_csl_msg(g_csl_buf.buf);
    end = add_option_csl_sync(end, g_csl_conf_period, g_csl_listen_window);

    for (int i = 0; i < 3; i++)
    {
        if (PROTOCOL.send(g_csl_buf.buf, end - g_csl_buf.buf, &timestamp) == TRUE)
        {
            g_csl_anchor_timestamp = timestamp;
            g_csl_new_anchor_timestamp = 0;
            g_csl_schedule_stop = FALSE;
            csl_schedule_periodic_rx();
            break;
        }
    }
}

static void csl_wakeup_reply_send()
{
    uint8_t *end;
    uint32_t timestamp;
    end = create_csl_msg(g_csl_buf.buf);
    end = add_option_csl_no_data_command(end, OPTION_CSL_WAKEUP_REP);
    PROTOCOL.send(g_csl_buf.buf, end - g_csl_buf.buf, &timestamp);
}

void csl_input(uint8_t *data, uint8_t len, uint32_t rx_timestamp, uint16_t saddr)
{
    uint8_t *optptr = data;
    uint8_t *end = optptr + len;
    //dbg_mem_dump(optptr, len);
    struct option_head *option_head;
    for (; optptr < end; optptr += (sizeof(struct option_head) + option_head->option_len))
    {
        option_head = (struct option_head *)optptr;
        //dbg_printf("type %u len %u\r\n", option_head->option_type, option_head->option_len);
        //dbg_mem_dump(optptr + sizeof(struct option_head), option_head->option_len);
        switch (option_head->option_type)
        {
        case OPTION_CSL_SYN:
            {
                if (g_csl_role == CSL_ROLE_COORD)
                {
                    struct option_csl_sync *option_csl_sync = (struct option_csl_sync *)optptr;
                    g_csl_recent_sync_timestamp = rx_timestamp;
                    g_csl_anchor_timestamp = rx_timestamp; // get rx time as csl anchor point
                    g_csl_period = option_csl_sync->csl_period;
                    g_csl_listen_window = option_csl_sync->csl_listen_window;
                    g_csl_peer_addr = saddr;
                    g_csl_schedule_stop = FALSE;
                    if (PROTOCOL.start_periodic_tx_sched_cb)
                    {
                        PROTOCOL.start_periodic_tx_sched_cb();
                    }
                    csl_schedule_periodic_tx();
                    dbg_printf("record endpoint 0x%04x period %u listen window %u\r\n", g_csl_peer_addr, g_csl_period,
                               g_csl_listen_window);
                }
                else if (g_csl_role == CSL_ROLE_ENDPOINT)
                {
                    g_csl_new_anchor_timestamp = rx_timestamp; // get rx time as new csl anchor point
                }
                break;
            }
        case OPTION_CSL_WAKEUP_REQ:
            {
                g_csl_schedule_stop = TRUE;
                csl_wakeup_reply_send();
                break;
            }
        case OPTION_CSL_WAKEUP_REP:
            {
                g_csl_schedule_stop = TRUE;
                if (PROTOCOL.stop_periodic_tx_sched_cb)
                {
                    PROTOCOL.stop_periodic_tx_sched_cb();
                }
                // send buffered packet
                uint8_t event = CSL_EV_DIRECT_SEND;
                os_msg_send(g_csl_ev_queue_handle, &event, 0);
                break;
            }
        case OPTION_CSL_SLEEP_REQ:
            {
                csl_sync_process();
                break;
            }
        case OPTION_CSL_DATA:
            {
                uint32_t anchor = g_csl_anchor_timestamp;
                if ((rx_timestamp + (g_csl_period >> 1)) < g_csl_anchor_timestamp)
                {
                    anchor = g_csl_anchor_timestamp - g_csl_period;
                }
                uint32_t diff = ((rx_timestamp > anchor) ? (rx_timestamp - anchor)
                                 : (anchor - rx_timestamp));
                dbg_printf("diff:%c%u msg:%s\r\n", (rx_timestamp > anchor) ? '+' : '-', diff,
                           optptr + sizeof(struct option_head));
                break;
            }
        case OPTION_CSL_FRAG:
            {
                struct option_csl_frag *option_csl_frag = (struct option_csl_frag *)optptr;
                uint8_t data_length = option_csl_frag->head.option_len + sizeof(struct option_head) - sizeof(
                                          struct option_csl_frag);
                if (!g_frag_bitmap)
                {
                    g_frag_crc = option_csl_frag->frag_crc;
                }

                if (g_frag_bitmap && option_csl_frag->frag_crc != g_frag_crc)
                {
                    free(g_frag_bitmap);
                    g_frag_bitmap = NULL;
                }
                /*
                * case 1: there is only one block, block_size = data_length.
                * case 2: it is an intermediate block, block_size = data_length.
                * case 3: it is the last block, calculate the block size.
                */
                uint8_t block_size = data_length;
                if (option_csl_frag->m_flag == 0 && option_csl_frag->block_id != 0) // case 3
                {
                    block_size = (option_csl_frag->total_len - data_length) / option_csl_frag->block_id;
                }

                if (g_frag_bitmap == NULL && block_size)
                {
                    dbg_printf("start receiving fragment data. total length = %u block size = %u now = %u\r\n",
                               option_csl_frag->total_len, block_size, PROTOCOL.get_curr_timestamp());
                    g_frag_bitmap = bitmap_alloc((option_csl_frag->total_len + block_size - 1) / block_size);
                }

                if (g_frag_bitmap)
                {
                    int ret = bitmap_get(g_frag_bitmap, option_csl_frag->block_id);
                    if (ret == 0)
                    {
                        bitmap_set(g_frag_bitmap, option_csl_frag->block_id);
                        memcpy(g_image + option_csl_frag->block_id * block_size, optptr + sizeof(struct option_csl_frag),
                               data_length);

                        // All data received?
                        if (g_frag_bitmap->received_blocks == g_frag_bitmap->total_blocks)
                        {
                            uint16_t re_crc = 0x0000;
                            re_crc = btxfcs(0x0000, g_image, option_csl_frag->total_len);

                            if (re_crc == g_frag_crc)
                            {
                                dbg_printf("All data received successfully. now = %u\r\n", PROTOCOL.get_curr_timestamp());
                                if (g_csl_debug == TRUE)
                                {
                                    dbg_mem_dump(g_image, option_csl_frag->total_len);
                                }
                            }
                            else
                            {
                                dbg_printf("The received data is incorrect.\r\n");
                                if (g_csl_debug == TRUE)
                                {
                                    dbg_mem_dump(g_image, option_csl_frag->total_len);
                                }
                            }
                            free(g_frag_bitmap->bits);
                            free(g_frag_bitmap);
                            g_frag_bitmap = NULL;
                        }
                    }
                    else if (ret == 1)
                    {
                        dbg_printf("Ignore duplicated block\r\n");
                    }
                    else
                    {
                        dbg_printf("Ignore wrong block_id\r\n");
                    }
                }
                else
                {
                    dbg_printf("error: no bitmap\r\n");
                }
                break;
            }
        default:
            dbg_printf("unknown option\r\n");
            break;
        }
    }
}

static void csl_main_task(void *p_param)
{
    static uint8_t retry = 0;
    uint8_t event;

    if (PROTOCOL.config_init)
    {
        PROTOCOL.config_init();
    }

    os_msg_queue_create(&g_csl_ev_queue_handle, 3, sizeof(uint8_t));
    // create timer to wait packet in listening window
    if (PROTOCOL.timer_init)
    {
        g_csl_ctimer = PROTOCOL.timer_init();
    }

    if (g_csl_role == CSL_ROLE_ENDPOINT)
    {
        dbg_printf("csl daemon start: endpoint\r\n");
        g_csl_period = g_csl_conf_period + ((int)g_csl_conf_period * g_csl_ppm / PPM);
        // register DLPS exit callback
        PROTOCOL.pm_init(csl_pm_exit);
        csl_sync_process();
    }
    else
    {
        dbg_printf("csl daemon start: coordinator\r\n");
        memb_init(&packet_memb);
    }

    while (true)
    {
        if (os_msg_recv(g_csl_ev_queue_handle, &event, 0xFFFFFFFF) == true)
        {
            //dbg_printf("ev %u\r\n", event);
            switch (event)
            {
            case CSL_EV_LISTEN_END:
                {
                    g_csl_sche_state = CSL_SCHE_EV_TM;
                    if (g_csl_debug == TRUE)
                    {
                        dbg_printf("exit_tm\r\n");
                    }
                    // Check whether the listening window needs to be extended
                    uint32_t now = PROTOCOL.get_curr_timestamp();
                    uint32_t require_time = g_recent_rx_timestamp + g_csl_ack_require_time;
                    if (now + 30 < require_time && PROTOCOL.timer_start)
                    {
                        g_csl_sche_state = CSL_SCHE_ENTR_TM;
                        g_csl_dbg_counter = require_time - now;
                        PROTOCOL.timer_start(g_csl_ctimer, require_time, csl_timer_cb, 0);
                        if (g_csl_debug == TRUE)
                        {
                            dbg_printf("entr_tm: extend %u\r\n", require_time - now);
                        }
                    }
                    else
                    {
#if GPIO_DEBUG
                        GPIO_WriteBit(GPIO_PIN_OUTPUT_0, (BitAction)(0));
#endif
                        csl_schedule_periodic_rx();
                    }
                    break;
                }
            case CSL_EV_PM_EXIT:
                {
                    g_csl_sche_state = CSL_SCHE_EV_PM;
                    trigger_gpio_at_anchor(0);
                    uint32_t now = PROTOCOL.get_curr_timestamp();
                    uint32_t diff = ((now > g_csl_anchor_timestamp) ? (now - g_csl_anchor_timestamp) :
                                     (g_csl_anchor_timestamp - now));
                    if (g_csl_debug_timing)
                    {
                        dbg_printf("wakeup %u diff %s%u\r\n", now, (now > g_csl_anchor_timestamp) ? "+" : "-", diff);
                    }
#if GPIO_DEBUG
                    GPIO_WriteBit(GPIO_PIN_OUTPUT_0, (BitAction)(1));
#endif
                    if (g_csl_anchor_timestamp + g_csl_listen_window - TASK_NOTIFY_DELAY < now)
                    {
                        // The listening window time has expired, give up listening
                        //if (g_csl_debug == TRUE)
                        dbg_printf("exit_pm: listening window expired. diff %s%u\r\n",
                                   (now > g_csl_anchor_timestamp) ? "+" : "-", diff);
                        csl_schedule_periodic_rx();
                    }
                    else
                    {
                        // start listening window
                        if (g_csl_debug == TRUE)
                        {
                            dbg_printf("exit_pm\r\n");
                        }
                        if (PROTOCOL.timer_start)
                        {
                            g_csl_sche_state = CSL_SCHE_ENTR_TM;
                            g_csl_dbg_counter = g_csl_anchor_timestamp + g_csl_listen_window - TASK_NOTIFY_DELAY - now;
                            PROTOCOL.timer_start(g_csl_ctimer,
                                                 g_csl_anchor_timestamp + g_csl_listen_window - TASK_NOTIFY_DELAY,
                                                 csl_timer_cb, 0);
                            if (g_csl_debug == TRUE)
                            {
                                dbg_printf("entr_tm %u\r\n", g_csl_anchor_timestamp + g_csl_listen_window - TASK_NOTIFY_DELAY -
                                           now);
                            }
                        }
                    }
                    break;
                }
            case CSL_EV_INDIRECT_SEND:
                {
                    g_csl_sche_state = CSL_SCHE_EV_TM;
                    uint32_t now = PROTOCOL.get_curr_timestamp();
                    uint32_t diff = ((now > g_csl_anchor_timestamp) ? (now - g_csl_anchor_timestamp) :
                                     (g_csl_anchor_timestamp - now));

                    // check queue and send only one packet
                    struct packet_queue *packet = list_head(g_csl_queue_list);
                    if (packet)
                    {
                        uint32_t timestamp;
                        uint8_t *end;
                        int ret;
                        /*
                         * re-sync condition:
                         *     1. the number of re-sync request is configured a value
                         *  or 2. auto sync period is not 0
                        */
                        if (g_csl_resync_req || (g_csl_autosync_period &&
                                                 (now > g_csl_recent_sync_timestamp + g_csl_autosync_period)))
                        {
                            end = create_csl_msg(g_csl_buf.buf);
                            end = add_option_csl_sync(end, NV_FIELD,
                                                      NV_FIELD); // period field is not used for Coord to Endpoint sync
                            memcpy(end, packet->buf.buf, packet->buf.len);
                            end += packet->buf.len;
                            ret = PROTOCOL.send(g_csl_buf.buf, end - g_csl_buf.buf, &timestamp);
                            if (ret == TRUE)
                            {
                                if (g_csl_resync_req)
                                {
                                    g_csl_resync_req--;
                                }
                                g_csl_anchor_timestamp = timestamp;
                                g_csl_recent_sync_timestamp = timestamp;
                            }
                        }
                        else
                        {
                            ret = PROTOCOL.send(packet->buf.buf, packet->buf.len, &timestamp);
                        }

                        if (g_csl_loop_test == FALSE)
                        {
                            if (ret == FALSE && retry < MAX_RETRY)
                            {
                                retry++;
                            }
                            else
                            {
                                list_pop(g_csl_queue_list);
                                memb_free(&packet_memb, packet);
                                retry = 0;
                            }
                        }
                    }

                    if (g_csl_debug_timing)
                    {
                        dbg_printf("checkpoint %u diff %s%u\r\n", now, (now > g_csl_anchor_timestamp) ? "+" : "-", diff);
                    }
                    trigger_gpio_at_anchor(0);
                    csl_schedule_periodic_tx();
                    break;
                }
            case CSL_EV_DIRECT_SEND:
                {
                    // send all packet
                    struct packet_queue *packet;
                    uint32_t timestamp;
                    packet = list_pop(g_csl_queue_list);
                    while (packet)
                    {
                        PROTOCOL.send(packet->buf.buf, packet->buf.len, &timestamp);
                        memb_free(&packet_memb, packet);
                        packet = list_pop(g_csl_queue_list);
                    }
                    break;
                }
            case CSL_EV_WAKEUP_REQ:
                csl_tx_queue_add(OPTION_CSL_WAKEUP_REQ, NULL, 0);
                break;
            case CSL_EV_SLEEP_REQ:
                csl_tx_queue_add(OPTION_CSL_SLEEP_REQ, NULL, 0);
                break;
            case CSL_EV_DATA_SEND:
                csl_tx_queue_add(OPTION_CSL_DATA, g_csl_test_data, MAX_TEST_DATA);
                break;
            default:
                break;
            }
        }
    }
}

// add fragmentation function
static uint8_t *get_tx_frag_frm(uint8_t *image, uint16_t image_size, uint16_t block_id,
                                uint8_t block_size, uint8_t *data_length)
{
    uint16_t last_block_id = ((image_size + block_size - 1) / block_size) - 1;
    if (block_id > last_block_id)
    {
        return NULL;
    }
    uint8_t last_block_size  = image_size % block_size;
    if (block_id == last_block_id && last_block_size != 0)
    {
        *data_length = last_block_size;
    }
    else
    {
        *data_length = block_size;
    }
    return image + block_id * block_size;
}

static int cmd_csl(int argc, char *argv[])
{
    uint16_t addr = 0x2;
    uint32_t value = 3000000; // 3s
    uint32_t listen_window = 4000; // 4ms
    uint8_t role;
    switch (argc)
    {
    case 4:
        listen_window = _strtoul((const char *)(argv[3]), (char **)NULL, 10);
    case 3:
        addr = _strtoul((const char *)(argv[2]), (char **)NULL, 16);
    case 2:
        value = _strtoul((const char *)(argv[1]), (char **)NULL, 10);
    case 1:
        if (strcmp(argv[0], "data") == 0)
        {
            uint8_t event = CSL_EV_DATA_SEND;
            strncpy((char *)g_csl_test_data, argv[1], MAX_TEST_DATA);
            g_csl_test_data[MAX_TEST_DATA - 1] = 0;
            os_msg_send(g_csl_ev_queue_handle, &event, 0);
            goto done;
        }
        else if (strcmp(argv[0], "sync") == 0)
        {
            g_csl_resync_req = value;
            goto done;
        }
        else if (strcmp(argv[0], "wakeup") == 0)
        {
            g_csl_loop_test = FALSE;
            uint8_t event = CSL_EV_WAKEUP_REQ;
            os_msg_send(g_csl_ev_queue_handle, &event, 0);
            goto done;
        }
        else if (strcmp(argv[0], "sleep") == 0)
        {
            uint8_t event = CSL_EV_SLEEP_REQ;
            os_msg_send(g_csl_ev_queue_handle, &event, 0);
            goto done;
        }
        else if (strcmp(argv[0], "154") == 0)
        {
            extern void p154_init(void);
            p154_init();
            goto done;
        }
        else if (strcmp(argv[0], "fragment") == 0)
        {
            // add frag
            uint8_t *data;
            uint8_t *end;
            uint8_t m_flag = 1;
            uint8_t data_length = 0;
            uint16_t block_id = 0;
            uint32_t timestamp;
            uint16_t image_crc = 0x0000;
            if (value > sizeof(g_image) || value < 1)
            {
                dbg_printf("Error: InvalidArgs: max. image size %u\r\n", sizeof(g_image));
                goto done;
            }

            uint16_t total_block = (value + BLOCK_SIZE - 1) / BLOCK_SIZE;
            // fill data to image
            for (int i = 0; i < value; i++)
            {
                g_image[i] = i % 256;
            }

            image_crc = btxfcs(image_crc, g_image, value);
            dbg_printf("image_crc = %x\r\n", image_crc);

            dbg_printf("start image transfer: total blocks %u now %u\r\n", total_block,
                       PROTOCOL.get_curr_timestamp());

            for (; block_id < total_block; block_id++)
            {
                if (block_id == (total_block - 1))
                {
                    m_flag = 0;
                }
                data = get_tx_frag_frm(g_image, value, block_id, BLOCK_SIZE, &data_length);
                end = create_csl_msg(g_csl_buf.buf);
                end = add_option_csl_frag(end, data, image_crc, value, m_flag, block_id, data_length);
                if (g_csl_debug == TRUE)
                {
                    dbg_mem_dump(g_csl_buf.buf, end - g_csl_buf.buf);
                }
                for (int i = 0; i < 3; i++)
                {
                    if (g_csl_debug == TRUE)
                    {
                        dbg_printf("send block %u\r\n", block_id);
                    }
                    if (PROTOCOL.send(g_csl_buf.buf, end - g_csl_buf.buf, &timestamp) == TRUE)
                    {
                        //dbg_printf("send successful!\r\n");
                        break;
                    }
                }
            }
            dbg_printf("transfer completed. now %u\r\n", PROTOCOL.get_curr_timestamp());
            goto done;
        }
        else
        {
            role = _strtoul((const char *)(argv[0]), (char **)NULL, 10);
            if (role >= CSL_ROLE_INVALID)
            {
                dbg_printf("Error: InvalidArgs\r\n");
                return FALSE;
            }
        }
        break;
    default:
        dbg_printf("Error: InvalidArgs\r\n");
        return FALSE;
    }
    if (PROTOCOL.id == PROTOCOL_NONE)
    {
        dbg_printf("Error: Select protocol first\r\n");
        return FALSE;
    }
    static bool task_is_running = 0;
    if (task_is_running == 1)
    {
        dbg_printf("Error: Task is running\r\n");
        return FALSE;
    }
    task_is_running = 1;
    g_csl_conf_period = value;
    g_csl_listen_window = listen_window;
    g_csl_role = role;
    g_csl_peer_addr = addr;
    // create csl application task
    os_task_create(&g_csl_task_handle, "csl_task", csl_main_task, NULL,
                   1024 * 4, 4);
done:
    dbg_printf("Done\r\n");
    return TRUE;
}

static int cmd_csl_config(int argc, char *argv[])
{
    int value;
    if (argc == 2)
    {
        value = _strtol((const char *)(argv[1]), (char **)NULL, 10);
        if (strcmp(argv[0], "ppm") == 0)
        {
            g_csl_ppm = value;
            g_csl_period = g_csl_conf_period + ((int)g_csl_conf_period * g_csl_ppm / PPM);
        }
        else if (strcmp(argv[0], "p2r") == 0)
        {
            g_csl_prepare_to_rx = value;
        }
        else if (strcmp(argv[0], "txc") == 0)
        {
            g_csl_tx_check_offest = value;
        }
        else if (strcmp(argv[0], "debug_t") == 0)
        {
            g_csl_debug_timing = value;
        }
        else if (strcmp(argv[0], "debug") == 0)
        {
            g_csl_debug = value;
        }
        else if (strcmp(argv[0], "ack") == 0)
        {
            g_csl_ack_require_time = value;
        }
        else if (strcmp(argv[0], "autosync") == 0)
        {
            g_csl_autosync_period = value;
        }
        else if (strcmp(argv[0], "loop") == 0)
        {
            g_csl_loop_test = value;
        }
    }
    dbg_printf("sche_state %u\r\n", g_csl_sche_state);
    dbg_printf("sche_dbg_counter %u\r\n", g_csl_dbg_counter);
    dbg_printf("schedule_stop %u\r\n", g_csl_schedule_stop);
    dbg_printf("loop %u\r\n", g_csl_loop_test);
    dbg_printf("autosync %u\r\n", g_csl_autosync_period);
    dbg_printf("ppm %d\r\n", g_csl_ppm);
    dbg_printf("p2r %u\r\n", g_csl_prepare_to_rx);
    dbg_printf("txc %d\r\n", g_csl_tx_check_offest);
    dbg_printf("ack %u\r\n", g_csl_ack_require_time);
    dbg_printf("debug_t %u\r\n", g_csl_debug_timing);
    dbg_printf("debug %u\r\n", g_csl_debug);
    dbg_printf("g_csl_period %u\r\n", g_csl_period);
    dbg_printf("Done\r\n");
    return TRUE;
}

void shell_register_user_cmd(void)
{
    shell_register((shell_program_t)cmd_csl, "csl",
                   BRIEF("proprietary csl implementation")
                   SYNOPSIS("csl data <string>")
                   DESCRIPTION("    string: no longer than 14 character")
                   SYNOPSIS("csl fragment <length>")
                   DESCRIPTION("    length: bulk data length")
                   SYNOPSIS("csl <role> [<csl_period>] [<shortaddr>] [<listen window>]")
                   DESCRIPTION("role: 0/1 - Coordinator/Endpoint")
                   DESCRIPTION("shortaddr: 0x0000 ~ 0xffff")
                   DESCRIPTION("csl_period: 1 ~ 4294967295 (us) - default 3000000")
                   DESCRIPTION("listen window: 1 ~ 4294967295 (us) - default 4000")
                   EXAMPLE("csl 154 - use IEEE-802.15.4 as transport protocol")
                   EXAMPLE("csl 24g - use proprietary 2.4G as transport protocol")
                   EXAMPLE("csl 0 - start as coordinator")
                   EXAMPLE("csl 1 3000000 0x2 4000 - start as endpoint")
                   EXAMPLE("csl data - send test data to endpoint")
                   EXAMPLE("csl fragment 2000 - send bulk data (2k) to endpoint")
                   EXAMPLE("csl sync - send sync command to endpoint"));

    shell_register((shell_program_t)cmd_csl_config, "csl_config",
                   BRIEF("csl configuration")
                   SYNOPSIS("csl_config [<loop/autosync/ppm/p2r/txc/ack/debug_t/debug> <value>]")
                   DESCRIPTION("loop: 1/0 - enable/disable send test data in loop")
                   DESCRIPTION("autosync: 10000000 (us) - the period of auto sync to endpoint")
                   DESCRIPTION("ppm: -50 ~ 50 - adjust endpoint clock")
                   DESCRIPTION("p2r: 0 ~ 2000 (us) - wake up early and prepare for RX (only for endpoint)")
                   DESCRIPTION("txc: -2000 ~ 2000 (us) - adjust tx check point (only for coordinator)")
                   DESCRIPTION("ack: 0 ~ 1000 (us) - extend listen window to ensure ack reply (only for endpoint)")
                   DESCRIPTION("debug_t: 1/0 - enable/disable timing debug")
                   DESCRIPTION("debug: 1/0 - enable/disable debug"));
}
