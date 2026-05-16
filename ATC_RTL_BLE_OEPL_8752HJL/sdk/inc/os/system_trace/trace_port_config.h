/****************************************************************************************************//**
 * @file     trace_port_config.h
 *
 * @brief
 *
 * @version  v0.1
 * @date     2023-04-23
 *
 * @note
 *******************************************************************************************************/
#ifndef _TRACE_PORT_CONFIG_H_
#define _TRACE_PORT_CONFIG_H_

#include "mem_config.h"

/******************* CPU CONFIGURATION  *******************/
#define PLATFORM_SUPPORT_CM4_INSTRUCTIONS         0


/******************* OS CONFIGURATION  *******************/
#define MAX_TASK_COUNT_FOR_DEBUG                ( 12 )
#define configMAX_TASK_NAME_LEN                 ( 10 )

/******************* HEAP CONFIGURATION *******************/

/* Bee2 has 2 types: data and buffer */
#define TRACE_HEAP_TYPE_NUM                     ( 2 )
#define TRACE_HEAP_MAX_NUMBER_IN_BLOCK_LIST     ( 16 )

/******************* STACK CONFIGURATION *******************/

#define MAIN_STACK_START_ADDRESS                ( DATA_RAM_START_ADDR + DATA_RAM_ROM_GLOBAL_SIZE ) //the bottom of stack

/******************* TIMER CONFIGURATION *******************/




/******************* QUEUE CONFIGURATION *******************/

#define MAX_QUEUE_OBJECTS_NUM                   ( 8 )

/******************* DLPS CONFIGURATION *******************/




/******************* CONFIGURATION END*******************/
#ifdef __cplusplus
extern "C" {
#endif


#ifdef __cplusplus
}
#endif

#endif /*_TRACE_PORT_CONFIG_H_*/

/******************* (C) COPYRIGHT 2015 Realtek Semiconductor Corporation *****END OF FILE****/
