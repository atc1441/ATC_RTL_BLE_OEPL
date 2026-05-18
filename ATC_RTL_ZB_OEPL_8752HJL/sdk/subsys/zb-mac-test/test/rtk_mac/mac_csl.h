/**
*****************************************************************************************
*     Copyright(c) 2017, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
  * @file     mac_csl.h
  * @brief    Demonstration of how to implement a self-definition service.
  * @details  Demonstration of different kinds of service interfaces.
  * @author
  * @date
  * @version
  * *************************************************************************************
  */
#ifndef _MAC_CSL_H_
#define _MAC_CSL_H_

#define PROTOCOL_NONE 0
#define PROTOCOL_154  1
#define PROTOCOL_24g  2

#define CSL_ROLE_COORD      0x0
#define CSL_ROLE_ENDPOINT   0x1
#define CSL_ROLE_INVALID    0x2

#if GPIO_DEBUG
void trigger_gpio_at_anchor(bool reverse);
#else
#define trigger_gpio_at_anchor(reverse)
#endif

struct protocol_fn
{
    /* Mandatory */
    uint8_t id; // PROTOCOL_154 or PROTOCOL_24g
    uint32_t (*get_curr_timestamp)(void);
    int (*send)(uint8_t *data, uint8_t data_len, uint32_t *tx_timestamp);
    void (*pm_init)(void *exit_callback);
    int (*pm_set)(uint32_t time);

    /* Mandatory for coordinator, Optional for endpoint */
    void *(*timer_init)(void);
    void (*timer_stop)(void *timer);
    void (*timer_start)(void *timer, uint32_t timeout, void *callback, void *arg);

    /* Optional */
    void (*config_init)(void);
    void (*start_periodic_tx_sched_cb)(void);
    void (*stop_periodic_tx_sched_cb)(void);
    void (*pm_exit_cb)(void);
};

extern struct protocol_fn PROTOCOL;
extern uint16_t g_csl_peer_addr;
extern int g_csl_ppm;
extern uint32_t g_csl_prepare_to_rx;
extern uint8_t g_csl_role;
extern int g_csl_tx_check_offest;
extern uint32_t g_csl_ack_require_time;
extern uint32_t g_csl_listen_window;
void csl_input(uint8_t *data, uint8_t len, uint32_t rx_timestamp, uint16_t saddr);

#endif /*_MAC_CSL_H_*/
