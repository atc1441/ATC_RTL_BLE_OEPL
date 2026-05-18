/**
*****************************************************************************************
*     Copyright(c) 2017, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
  * @file     mac_test_common.h
  * @brief    Demonstration of how to implement a self-definition service.
  * @details  Demonstration of different kinds of service interfaces.
  * @author
  * @date
  * @version
  * *************************************************************************************
  */
#ifndef _MAC_TEST_COMMON_H_
#define _MAC_TEST_COMMON_H_
#if defined(CONFIG_SOC_SERIES_RTL87X2G) || defined(CONFIG_SOC_SERIES_RTL87X2H)
#include "power_manager_unit_zbmac.h"
#endif

//++++++++++++++++++++++++++++++++++++++++++++++++
// type and macro define
//------------------------------------------------
#ifdef AUTO_TEST
#define Auto_test AUTO_TEST
#else
#define Auto_test 0 // Default enable auto test
#endif

#if 0 // defined in mac_802154_frame_parser.h
#define FRAME_TYPE_BEACON   0
#define FRAME_TYPE_DATA     1
#define FRAME_TYPE_ACK      2
#define FRAME_TYPE_COMMAND  3
#endif

#define FRAME_VER_2003 0
#define FRAME_VER_2006 1
#define FRAME_VER_2015 2

#define TX_NONE         0
#define TX_SUCCESS      1
#define TX_BUSY         2
#define TX_NOACK        3
#define TX_AT_FAIL      4
#define TX_SUCCESS_MASK (1 << 0)
#define TX_BUSY_MASK    (1 << 1)
#define TX_NOACK_MASK   (1 << 2)
#define TX_AT_FAIL_MASK (1 << 3)

#define ADDR_MODE_NOT_PRESENT   0
#define ADDR_MODE_RSV           1
#define ADDR_MODE_SHORT         2
#define ADDR_MODE_EXTEND        3
#define ADDR_MODE_MAX           4

#define SEC_NONE        0
#define SEC_MIC_32      1
#define SEC_MIC_64      2
#define SEC_MIC_128     3
#define SEC_ENC         4
#define SEC_ENC_MIC_32  5
#define SEC_ENC_MIC_64  6
#define SEC_ENC_MIC_128 7

#define KEY_ID_MODE_0_NOOFFSET  0
#define KEY_ID_MODE_1_NOOFFSET  1
#define KEY_ID_MODE_2_NOOFFSET  2
#define KEY_ID_MODE_3_NOOFFSET  3

#define TX_LOOP_STOP 0
#define TX_LOOP_CONT 1

#define LOOP_REPORT_NONE 0x0    // no report
#define LOOP_REPORT_FIRST 0x1   // first report
#define LOOP_REPORT_FINAL 0x4   // final report
#define LOOP_REPORT_ALL 0x7     // report every time
#define TX_BUF_MAX_LEN 127
#define NV_FIELD 0 // Not Available Field

// [CMD_VENDOR_SPEC:1B][VENDOR_OUI:3B][VENDOR_INFO:1B][PING_SEQ:1B]
#define CMD_VENDOR_SPEC         0x24
#define VENDOR_INFO_PING_REQ    0x00
#define VENDOR_INFO_PING_REPLY  0x01

#define BRIEF(name) "\t"name"\r\n"
#define SYNOPSIS(synopsis) "\t"synopsis"\r\n"
#define DESCRIPTION(description) "\t"description"\r\n"
#define EXAMPLE(example) "\te.g. "example"\r\n"

#define CPY_MV_PTR(dst, src, size, ptr) \
    { \
        if ((size) != 0) { \
            void *__ptr = (void *)(src); \
            if (__ptr) { \
                memcpy(dst, __ptr, size); \
            } else { \
                memset(dst, 0, size); \
            } \
            ptr += (size); \
        } \
    }

#define GEN_SEQ_DATA_MV_PTR(buf, start, size, ptr) \
    { \
        for (uint16_t __i = start; __i < (start + size); __i++) \
            buf[ptr++] = __i; \
    }

#define WAIT_FOR_TXDOWN() {while (g_tx_done == TX_NONE);}

#define WAIT_FOR_TXDOWN_UNTIL(cond) \
    { \
        while (g_tx_done == TX_NONE) { \
            if (cond) { \
                dbg_printf("break WAIT_FOR_TXDOWN\r\n"); \
                break; \
            } \
        } \
    }

#define RESET_TXDOWN() {g_tx_done = TX_NONE;}

#define BUF_RESET(_buf) \
    { \
        memset(_buf.buf, 0, TX_BUF_MAX_LEN); \
        _buf.len = 0; \
    }

typedef struct fc_s
{
    uint16_t type: 3;
    uint16_t sec_en: 1;
    uint16_t pending: 1;
    uint16_t ack_req: 1;
    uint16_t panid_compress: 1;
    uint16_t rsv: 1;
    uint16_t seq_num_suppress: 1;
    uint16_t ie_present: 1;
    uint16_t dst_addr_mode: 2;
    uint16_t ver: 2;
    uint16_t src_addr_mode: 2;
} __attribute__((packed)) fc_t;

typedef struct ss_s
{
    uint16_t bo: 4;
    uint16_t so: 4;
    uint16_t final_cap_slot: 4;
    uint16_t ble: 1;
    uint16_t rsv: 1;
    uint16_t pan_coord: 1;
    uint16_t assoc_permit: 1;
} __attribute__((packed)) ss_t;

typedef struct gts_spec_s
{
    uint8_t gts_desc_cnt: 3;
    uint8_t rsv: 4;
    uint8_t gts_permit: 1;
} gts_spec_t;

typedef struct pendaddr_spec_s
{
    uint8_t saddr_pend_num: 3;
    uint8_t rsv0: 1;
    uint8_t extaddr_pend_num: 3;
    uint8_t rsv1: 1;
} pendaddr_spec_t;

typedef struct aux_sec_ctl_s
{
    uint8_t sec_level: 3;
    uint8_t key_id_mode: 2;
    uint8_t rsv: 3;
} aux_sec_ctl_t;

typedef struct aux_s
{
    aux_sec_ctl_t sec_ctl;
    uint32_t frame_counter;
    uint8_t key_id;
} __attribute__((packed)) aux_t;

typedef struct nonce_s
{
    uint8_t sec_level;
    uint32_t frame_counter;
    uint64_t src_ext_addr;
} __attribute__((packed)) nonce_t;

typedef struct tx_buf_s
{
    uint8_t buf[TX_BUF_MAX_LEN];
    uint8_t len;
} tx_buf_t, buf_t;

typedef struct txl_ctrl_info_s
{
    uint8_t hdr_len;
    uint8_t prt_tx_mask; // print tx result mask
    uint8_t fix_seq : 1;
    uint8_t dump_pkt : 1;
    uint8_t dump_enc_pkt : 1;
    uint8_t rsv : 5;
    uint8_t rsv2;
    uint32_t delay_us;
} txl_ctrl_info_t;

typedef struct ifrxstat_s
{
    uint32_t packets;
    uint32_t errors;
    uint32_t dropped;
} ifrxstat_t;

typedef struct iftxstat_s
{
    uint32_t packets;
    uint32_t errors;
    uint32_t dropped;
    uint32_t noack;
    uint32_t collisions;
} iftxstat_t;

// add bitmap
typedef struct
{
    uint8_t *bits;
    uint16_t total_blocks;
    uint16_t received_blocks;
} bitmap_t;

uint8_t mac_TrigTxNDelay(uint8_t ackreq, uint8_t secreq, uint8_t frm_ver,
                         uint32_t delay, uint32_t *base_us);

typedef uint8_t (*loop_check_cb)(uint32_t count);
typedef uint32_t (*loop_exec_cb)(uint32_t count, uint32_t ctrl_info);
typedef void (*loop_report_cb)(uint32_t count, uint32_t exec_info);

//++++++++++++++++++++++++++++++++++++++++++++++++
// global variable
//------------------------------------------------
extern volatile uint32_t g_tx_done;
extern volatile bool g_tx_loop_state;
extern void *g_ping_reply_sequence_mbx;

extern const cmd_shell_func_stubs_t cmd_shell_stubs;
extern void *zb_sem;
extern void *zb_task_handle;
extern void *rx_task_handle;

extern tx_buf_t g_tx_buf;
extern uint32_t g_tx_interval_ms;
extern bool g_enh_ack_early;
extern int8_t g_peak_ed_level;
extern int8_t g_avrg_ed_level;
extern bool g_test_enh_ack_late;
extern ifrxstat_t g_ifrxstat;
extern iftxstat_t g_iftxstat;
extern volatile uint32_t g_recent_rx_timestamp;
extern int g_zbpm_wakeup_diff_min;
extern int g_zbpm_wakeup_diff_max;
extern int g_zbpm_wakeup_diff_avg;

extern uint8_t ADDR_MODE2LEN[ADDR_MODE_MAX];
extern uint8_t g_mac_key[16];

#define MAC_RX_PKT_LEN(buf) (buf[0] - 2)
#define MAC_RX_PKT(buf) ((uint8_t *)&buf[1])
#define MAC_RX_PKT_CRC(buf) ((buf[buf[0] - 1] << 8) | (buf[buf[0]]))
#define MAC_RX_PKT_INFO(buf) ((mac_rxfifo_tail_t *)&buf[buf[0] + 1])
#define MAC_RX_PKT_LQI(buf) (MAC_RX_PKT_INFO(buf)->lqi)
#define MAC_RX_PKT_RSSI(buf) (mac_rssi_get(MAC_RX_PKT_INFO(buf)->rssi, mac_channel_get()))
extern uint64_t bt_clk_offset;
#define MAC_RX_PKT_TIMESTAMP(buf) (bt_clk_offset + mac_btclk_to_us(MAC_RX_PKT_INFO(buf)->bt_time))

//++++++++++++++++++++++++++++++++++++++++++++++++
// public function
//------------------------------------------------
void clear_tx_stat(void);
void clear_rx_stat(void);
void zb_mac_interrupt_enable(void);
void zb_mac_drv_enable(void);
void zb_mac_drv_init(void);
void radio_rx(void);
void dbg_mem_dump(const uint8_t *addr, uint32_t len);
uint32_t get_curr_us(void);
int32_t edscan_lv2dbm(int32_t level);
uint8_t parse_digit(char c);
void print_tx_result(uint8_t val, uint8_t prt_tx_mask);
void print_rx_packet(uint8_t *rx_data);
uint8_t txl_check(uint32_t count);
uint32_t txl_exec(uint32_t count, uint32_t ctrl_info);
#if Auto_test
void txl_report(uint32_t count, uint32_t exec_info);
#else
#define txl_report NULL
#endif

#if Auto_test
void cmd_data_report(uint32_t count, uint32_t exec_info);
#else
#define cmd_data_report NULL
#endif
void loop_ctrl(uint32_t count, loop_check_cb check_cb, loop_exec_cb exec_cb,
               loop_report_cb report_cb, uint8_t report_mode, uint32_t interval,
               uint32_t ctrl_info);
uint8_t mac_TrigTxNDelay(uint8_t ackreq, uint8_t secreq, uint8_t frm_ver, uint32_t delay,
                         uint32_t *base_us);
uint8_t generate_ieee_frame(uint8_t frm_type, uint8_t *tx_buf, uint16_t tx_buf_len, fc_t fc,
                            uint8_t seq,
                            uint16_t spid, uint8_t *saddr, uint16_t dpid, uint8_t *daddr,
                            uint8_t *aux, uint16_t aux_len, uint8_t cmd_id, ss_t *ss);
#if defined(CONFIG_SOC_SERIES_RTL87X2G) || defined(CONFIG_SOC_SERIES_RTL87X2H)
int zbmac_power_manager_set(uint32_t next, uint32_t period);
void zbmac_power_manager_init(zbpm_callback_t exit_callback);
#endif
#endif /* _MAC_TEST_COMMON_H_ */

#define LIST_CONCAT2(s1, s2) s1##s2
#define LIST_CONCAT(s1, s2) LIST_CONCAT2(s1, s2)
#define LIST(name) \
    static void *LIST_CONCAT(name,_list) = NULL; \
    static list_t name = (list_t)&LIST_CONCAT(name,_list)
typedef void **list_t;
typedef void *const *const_list_t;
static inline void list_init(list_t list)
{
    *list = NULL;
}

static inline void *list_head(const_list_t list)
{
    return *list;
}

void list_add(list_t list, void *item);
void *list_pop(list_t list);
void list_remove(list_t list, const void *item);
void *list_tail(const_list_t list);
static inline void *list_item_next(const void *item)
{
    struct list
    {
        struct list *next;
    };
    return item == NULL ? NULL : ((struct list *)item)->next;
}
#define CC_CONCAT2(s1, s2) s1##s2
#define CC_CONCAT(s1, s2) CC_CONCAT2(s1, s2)

#define MEMB(name, structure, num) \
    static bool CC_CONCAT(name,_memb_used)[num]; \
    static structure CC_CONCAT(name,_memb_mem)[num]; \
    static struct memb name = {sizeof(structure), num, \
        CC_CONCAT(name,_memb_used), \
        (void *)CC_CONCAT(name,_memb_mem)}
struct memb
{
    unsigned short size;
    unsigned short num;
    bool *used;
    void *mem;
};
void  memb_init(struct memb *m);
void *memb_alloc(struct memb *m);
int  memb_free(struct memb *m, void *ptr);

typedef struct mac_cmd_handler_s
{
    struct mac_cmd_handler_s *next;
    int (*handler)(uint8_t *rx_data);
    uint8_t type;
} mac_cmd_handler_t;

#define MAC_CMD_HANDLER(name, type, func) \
    static mac_cmd_handler_t name = {NULL, func, type}

void mac_cmd_register_handler(mac_cmd_handler_t *handler);

#define MAC_CMD_VENDOR  0x24
#define MAC_CMD_PING    0x25
#define MAC_CMD_CSL     0x26 // proprietary CSL application

#define MAC_CMD_PROCESSED 1
#define MAC_CMD_UNPROCESSED 0

// bitmap fuction
bitmap_t *bitmap_alloc(uint16_t total_blocks);
int bitmap_get(const bitmap_t *bitmap, uint16_t id);
void bitmap_set(bitmap_t *bitmap, uint16_t id);

#if GPIO_DEBUG
#define GPIO_OUTPUT_PIN_0   P0_1 // LED 0
#define GPIO_OUTPUT_PIN_1   P0_2 // LED 1
#define GPIO_PIN_OUTPUT_0   GPIO_GetPin(GPIO_OUTPUT_PIN_0)
#define GPIO_PIN_OUTPUT_1   GPIO_GetPin(GPIO_OUTPUT_PIN_1)
void debug_gpio_init(void);
void toggle_gpio(uint32_t GPIO_Pin);
#else
#define toggle_gpio(GPIO_Pin)
#define debug_gpio_init()
#endif
