/**
*********************************************************************************************************
*               Copyright(c) 2016, Realtek Semiconductor Corporation. All rights reserved.
**********************************************************************************************************
* @file     log_uart_dma.h
* @brief    This file provides APIs of log uart with DMA channel.
* @details
* @author   Lory Xu
* @date     2016-01-11
* @version  v0.1
*********************************************************************************************************
*/

#ifndef LOG_UART_DMA_H
#define LOG_UART_DMA_H

#include "pingpong_buffer.h"
#include "rtl876x.h"
#include "trace.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
    LOG_DMA_STATE_IDLE = 0,
    LOG_DMA_STATE_PROCESS_MCU,
    LOG_DMA_STATE_PENDING_MCU,
    LOG_DMA_STATE_MAX
} T_LOG_DMA_STATE;

typedef enum
{
    LOG_DMA_SIGNAL_MCU_REQUEST = 0,
    LOG_DMA_SIGNAL_MCU_DONE,
    LOG_DMA_SIGNAL_MAX
} T_LOG_DMA_SIGNAL;

typedef struct
{
    uint16_t MCU_LogMissed;
    uint16_t PendedMCU_BufferSize;
    uint8_t *PendedMCU_Buffer;      /* MCU uses ping pong buffer */
    volatile T_LOG_DMA_STATE State;
    T_LOG_DMA_STATE LastState;
} T_LOG_DMA_SM;

typedef struct
{
    uint32_t ReadIndex;     /* DSP uses ring buffer */
    uint32_t WriteIndex;
    uint8_t *Buffer;
    uint16_t BufferSize;    /* MCU uses ping pong buffer */
    T_LOG_DMA_SIGNAL Signal;
} T_LOG_DMA_EVT;

#define LogDMA_StateTran(State_) (me->LastState = me->State, me->State = (State_))

/* Config Log DMA Channel */
#define LogDMAChannelNum      3
#define LogDMAChannel         GDMA_Channel3
#define LogDMAIRQn            GDMA0_Channel3_IRQn
#define LogUartDMAIntrHandler GDMA0_Channel3_Handler

extern PingpongBuffer *pMCU_PPB;
extern T_LOG_DMA_SM   *pLogDMA_SM;

void LogUartDMAInit(void);
void LogUartDMAStart(uint8_t *p_outputbuf, uint16_t output_size);
void LogUartDMAIntrHandler(void);
void LogUartDMAIdleHook(void);
bool log_pm_check(void);

void LogDMA_SM_Dispatch(T_LOG_DMA_SM *me, T_LOG_DMA_EVT const *e);

__STATIC_INLINE void NEVER_HAPPEN(uint32_t line_no, T_LOG_DMA_SM *me, T_LOG_DMA_EVT const *e)
{
    OS_PRINT_WARN3("Line %d: Log SM Error: state: %d, signal: %d", line_no, me->State, e->Signal);
}

#ifdef __cplusplus
}
#endif

#endif
