/**
*********************************************************************************************************
*               Copyright (c) 2024, Realtek Semiconductor Corporation. All rights reserved.
*********************************************************************************************************
* \file     rtl876x_nvic.h
* \brief    Header file for NVIC driver.
* \details  This file provides all NVIC firmware functions.
* \author   Elliot Chen
* \date     2024-01-22
* \version  v1.0
*********************************************************************************************************
*/

#ifndef _RTL876X_NVIC_H_
#define _RTL876X_NVIC_H_

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \addtogroup  IO          Peripheral Drivers
 * \defgroup    NVIC        NVIC
 * \brief       Manage the NVIC peripheral functions.
 * \ingroup     IO
 */

/*============================================================================*
 *                         Includes
 *============================================================================*/
#include "rtl876x.h"

/*============================================================================*
 *                         Types
 *============================================================================*/
/**
 * \defgroup    NVIC_Exported_Types NVIC Exported Types
 *
 * \ingroup     NVIC
 * \{
 */

/**
 * \brief       NVIC initialization structure definition
 *
 * \ingroup     NVIC_Exported_Types
 */
typedef struct
{
    IRQn_Type NVIC_IRQChannel;              /*!< Specify the IRQ channel to be enabled or disabled.
                                                 This parameter can be a value of \ref IRQn_Type defined in rtl876x.h. */
    uint32_t NVIC_IRQChannelPriority;       /*!< Specify the priority for the IRQ channel.
                                                 This parameter can be a value between 0 and the maximum priority value. */
    FunctionalState
    NVIC_IRQChannelCmd;     /*!< Specify whether to enable or disable the IRQ channel. */
} NVIC_InitTypeDef;

/** End of NVIC_Exported_Types
  * \}
  */

/*============================================================================*
 *                         Functions
 *============================================================================*/
/**
 * \defgroup    NVIC_Exported_Functions NVIC Exported Functions
 * \{
 * \ingroup     NVIC
 */

/**
 * \brief     Initialize the NVIC peripheral according to the specified parameters
 *            in the NVIC_InitStruct.
 * \param[in] NVIC_InitStruct: Pointer to a NVIC_InitTypeDef structure that contains
 *            the configuration information for the NVIC peripheral.
 * \return  None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void nvic_config(void)
 * {
 *     NVIC_InitTypeDef NVIC_InitStruct;
 *     NVIC_InitStruct.NVIC_IRQChannel = RTC_IRQn;
 *     NVIC_InitStruct.NVIC_IRQChannelPriority = 3;
 *     NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
 *     NVIC_Init(&NVIC_InitStruct);
 * }
 * \endcode
 */
void NVIC_Init(NVIC_InitTypeDef *NVIC_InitStruct);

/**
 * \brief     Clear a pending interrupt. This function is applicable to both primary and secondary interrupts.
 * \param[in] IRQn - External interrupt number. This value cannot be negative.
 * \return    None.
 *
 * <b>Example usage</b>
 *
 * To clear the pending bit of a secondary interrupt (e.g., Qdecode, KeyScan, WatchDog, etc.),
 * use NVIC_ClearPendingIRQ_Generic instead of the CMSIS original API NVIC_ClearPendingIRQ.
 *
 * To clear the pending bit of a primary interrupt (e.g., System, UART0, GDMA0_Channel0, etc.),
 * use the CMSIS original API NVIC_ClearPendingIRQ.
 *
 * \note The IRQ number for a secondary interrupt is not less than Peripheral_First_IRQn.
 *       Refer to rtl876x.h for more details about primary and secondary interrupts.
 *
 * \code{.c}
 *
 * void nvic_clear_pending_demo(void)
 * {
 *     NVIC_ClearPendingIRQ_Generic(WDT_IRQn);  // WDT_IRQn is a secondary interrupt.
 * }
 * \endcode
 */
void NVIC_ClearPendingIRQ_Generic(IRQn_Type IRQn);

/** End of NVIC_Exported_Functions
  * \}
  */

/** End of NVIC
  * \}
  */

#ifdef __cplusplus
}
#endif

#endif /* _RTL876X_NVIC_H_ */

/******************* (C) COPYRIGHT 2024 Realtek Semiconductor Corporation *****END OF FILE****/
