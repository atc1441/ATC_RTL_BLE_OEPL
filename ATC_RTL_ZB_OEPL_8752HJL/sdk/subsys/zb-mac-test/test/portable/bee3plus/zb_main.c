/**
*****************************************************************************************
*     Copyright(c) 2017, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
   * @file      main.c
   * @brief     Source file for BLE peripheral project, mainly used for initialize modules
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
#include <os_sched.h>
#include <string.h>
#include <stdlib.h>
#include <trace.h>
#include <os_task.h>
#include <os_sync.h>
#include <os_msg.h>
#include "rtl876x.h"
#include "rtl876x_pinmux.h"
#include "rtl876x_rcc.h"
//#include "rtl876x_gpio.h"
#include "rtl876x_uart.h"
#include "rtl876x_tim.h"
#include "rtl876x_nvic.h"
#include "vector_table.h"

#include "zb_tst_cfg.h"
#include "dbg_printf.h"
#include "stdio_port.h"
#include "shell.h"
#include "mac_test_common.h"

/** @defgroup  PERIPH_DEMO_MAIN Peripheral Main
    * @brief Main file to initialize hardware and BT stack and start task scheduling
    * @{
    */

/*============================================================================*
 *                              Constants
 *============================================================================*/

/*============================================================================*
 *                              Variables
 *============================================================================*/
void *zb_sem;
void *zb_task_handle;   //!< ZB MAC Task handle
void *rx_task_handle;
extern void shell_cmd_init(void);

#define RX_BUF_SIZE     128
static void *rx_queue = NULL;

/*============================================================================*
 *                              Functions
 *============================================================================*/
/**
 * @brief    Contains the initialization of pinmux settings and pad settings
 * @note     All the pinmux settings and pad settings shall be initiated in this function,
 *           but if legacy driver is used, the initialization of pinmux setting and pad setting
 *           should be peformed with the IO initializing.
 * @return   void
 */
void zb_pin_mux_init(void)
{
    // UART for CLI/DBG
    Pad_Config(ZB_DBG_UART_TX_PIN, PAD_PINMUX_MODE, PAD_IS_PWRON, PAD_PULL_UP, PAD_OUT_DISABLE,
               PAD_OUT_HIGH);
    Pad_Config(ZB_DBG_UART_RX_PIN, PAD_PINMUX_MODE, PAD_IS_PWRON, PAD_PULL_UP, PAD_OUT_DISABLE,
               PAD_OUT_HIGH);
    Pinmux_Config(ZB_DBG_UART_TX_PIN, ZB_DBG_UART_TX);
    Pinmux_Config(ZB_DBG_UART_RX_PIN, ZB_DBG_UART_RX);
}

void ZB_DBG_UART_Handler(void)
{
    uint32_t int_status;
    uint16_t rx_len;
    uint8_t tmp;
    int_status = UART_GetIID(ZB_DBG_UART);
    switch (int_status & 0x0E)
    {
    case UART_INT_ID_RX_LEVEL_REACH:
    case UART_INT_ID_RX_TMEOUT:
        rx_len = UART_GetRxFIFOLen(ZB_DBG_UART);
        while (rx_len--)
        {
            tmp = (uint8_t)ZB_DBG_UART->RB_THR;
            if (tmp == 'Q')
            {
                g_tx_loop_state = TX_LOOP_STOP;
            }
            else
            {
                os_msg_send(rx_queue, &tmp, 0);
            }
        }
        break;

    default:
        break;
    }
}

/**
 * @brief    Contains the initialization of peripherals
 * @note     Both new architecture driver and legacy driver initialization method can be used
 * @return   void
 */
void zb_periheral_drv_init(void)
{
    os_msg_queue_create(&rx_queue, RX_BUF_SIZE, sizeof(uint8_t));
    RCC_PeriphClockCmd(APBPeriph_UART2, APBPeriph_UART2_CLOCK, ENABLE);
    UART_InitTypeDef UART_InitStruct;
    NVIC_InitTypeDef NVIC_InitStruct;
    UART_StructInit(&UART_InitStruct);
    // Baud rate = 2000000
    UART_InitStruct.div         = 2;
    UART_InitStruct.ovsr        = 5;
    UART_InitStruct.ovsr_adj     = 0;
    UART_InitStruct.rxTriggerLevel  = 1;
    UART_InitStruct.idle_time       = UART_RX_IDLE_2BYTE;      //idle interrupt wait time
    UART_InitStruct.dmaEn          = UART_DMA_ENABLE;
    UART_InitStruct.TxWaterlevel   = 15;     //Better to equal TX_FIFO_SIZE(16)- GDMA_MSize
    UART_InitStruct.RxWaterlevel   = 1;      //Better to equal GDMA_MSize
    UART_InitStruct.TxDmaEn   = ENABLE;
    UART_Init(ZB_DBG_UART, &UART_InitStruct);
    UART_INTConfig(ZB_DBG_UART, UART_INT_RD_AVA, ENABLE);
    NVIC_InitStruct.NVIC_IRQChannel = ZB_DBG_UART_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelCmd = (FunctionalState)ENABLE;
    NVIC_InitStruct.NVIC_IRQChannelPriority = 3;
    NVIC_Init(&NVIC_InitStruct);
    RamVectorTableUpdate(ZB_DBG_UART_VECTORn, ZB_DBG_UART_Handler);
    DBG_DIRECT("RamVectorTableUpdate");
}

void stdio_putc_wrap(void *adapter, const char data)
{
    while (UART_GetFlagState((UART_TypeDef *)adapter, UART_FLAG_THR_EMPTY) == 0);
    UART_SendByte(adapter, data);
}

int stdio_getc_wrap(void *adapter, char *data)
{
    return os_msg_recv(rx_queue, data, 0xffffffff);
}

extern void shell_register_test_cmd(void);
__WEAK void shell_register_priv_cmd(void)
{
}
__WEAK void shell_register_user_cmd(void)
{
}

void zb_test_task(void *p_param)
{
    DBG_DIRECT("ZB Test Task==>");
    dbg_init();
    _stdio_port_init(ZB_DBG_UART, (stdio_putc_t)&stdio_putc_wrap, (stdio_getc_t)&stdio_getc_wrap);
    shell_cmd_init();
    shell_register_test_cmd();
    shell_register_priv_cmd();
    shell_register_user_cmd();
    dbg_printf("start\r\n");
    DBG_DIRECT("while 1");
    while (1)
    {
        shell_task();
    }
}

void rx_test_task(void *p_param)
{
    uint32_t notify;
    while (1)
    {
        if (os_task_notify_take(1, 0xffffffff, &notify))
        {
            radio_rx();
        }
    }
}
/**
 * @brief    Entry of APP code
 * @return   int (To avoid compile warning)
 */
extern void mac_Initialize_Patch(void);
void zb_task_init(void)
{
    mac_Initialize_Patch();

    DBG_DIRECT("zb_task_init");
    zb_pin_mux_init();
    DBG_DIRECT("zb_pin_mux_init");
    zb_periheral_drv_init();
    DBG_DIRECT("zb_periheral_drv_init");
    zb_mac_interrupt_enable();
    DBG_DIRECT("zb_mac_interrupt_enable");
    zb_mac_drv_enable();
    DBG_DIRECT("zb_mac_drv_enable");
    bool zb_sem_create = false;
    zb_sem_create = os_sem_create(&zb_sem, "zb_sem", 0, 16);
    if (zb_sem_create)
    {
        DBG_DIRECT("os_sem_create true");
    }
    else
    {
        DBG_DIRECT("os_sem_create false");
    }
    os_task_create(&zb_task_handle, "zb_test", zb_test_task, NULL, ZB_TASK_STACK_SIZE,
                   ZB_TASK_PRIORITY);
    os_task_create(&rx_task_handle, "rx_test", rx_test_task, NULL, ZB_TASK_STACK_SIZE,
                   ZB_TASK_PRIORITY);
    DBG_DIRECT("os_task_create");
}
/** @} */ /* End of group PERIPH_DEMO_MAIN */
