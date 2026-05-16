/**
*********************************************************************************************************
*               Copyright(c) 2016, Realtek Semiconductor Corporation. All rights reserved.
**********************************************************************************************************
* @file     pingpong_buffer.h
* @brief    This file provides APIs of PingPong Buffer.
* @details
* @author   Lory Xu
* @date     2015-12-20
* @version  v0.1
*********************************************************************************************************
*/

#ifndef PINGPONG_BUFFER_H
#define PINGPONG_BUFFER_H

#include <stdint.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif


/** @brief LogMissedCounter records the number of log missed.
    If the value is greater then 0, please reduce logs or speed up log uart/DMA.
    */
typedef struct
{
    uint8_t *pInputBuffer;
    uint8_t *pOutputBuffer;
    uint16_t InputBufferSize;
    uint16_t OutputBufferSize;
    uint32_t LogMissedCounter;
    uint32_t MaxBufferSize;
} PingpongBuffer;

static inline uint32_t PPB_GetLogMissedCounter(PingpongBuffer *pPPB)
{
    return pPPB->LogMissedCounter;
}

static inline void PPB_ClearLogMissedCounter(PingpongBuffer *pPPB)
{
    pPPB->LogMissedCounter = 0;
}

static inline bool PPB_IsInit(PingpongBuffer *pPPB)
{
    return ((pPPB->pInputBuffer != NULL) && (pPPB->pOutputBuffer != NULL));
}

static inline bool PPB_IsOutputBufEmpty(PingpongBuffer *pPPB)
{
    return (pPPB->OutputBufferSize == 0);
}

static inline bool PPB_IsInputBufEmpty(PingpongBuffer *pPPB)
{
    return (pPPB->InputBufferSize == 0);
}

static inline uint16_t PPB_GetOutputBufSize(PingpongBuffer *pPPB)
{
    return pPPB->OutputBufferSize;
}

static inline uint16_t PPB_GetInputBufSize(PingpongBuffer *pPPB)
{
    return pPPB->InputBufferSize;
}

void PPB_Init(PingpongBuffer *pPPB);
void PPB_Uninit(PingpongBuffer *pPPB);
void PPB_Init_DLPS_Restore(PingpongBuffer *pPPB);
void PPB_Write(PingpongBuffer *pPPB, const uint8_t *source, uint16_t size);
void PPB_ClearOutputBuffer(PingpongBuffer *pPPB);
void PPB_BufSwitch(PingpongBuffer *pPPB);


#ifdef __cplusplus
}
#endif

#endif
