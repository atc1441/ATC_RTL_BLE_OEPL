/**
*********************************************************************************************************
*               Copyright(c) 2016, Realtek Semiconductor Corporation. All rights reserved.
*********************************************************************************************************
* @file      gap_pa_adv.h
* @brief     Header file for Gap past sender
* @details   This file defines sender of PAST related API.
* @author
* @date      2020-10-18
* @version   v0.4
* *********************************************************************************************************
*/

/*============================================================================*
 *               Define to prevent recursive inclusion
 *============================================================================*/
#ifndef GAP_PAST_SENDER_H
#define GAP_PAST_SENDER_H

#ifdef __cplusplus
extern "C"
{
#endif

/*============================================================================*
 *                        Header Files
 *============================================================================*/
#include "upperstack_config.h"
#include "gap_le.h"

#if F_BT_LE_5_1_PAST_SENDER_SUPPORT

/** @addtogroup GAP GAP Module
  * @{
  */

/** @addtogroup GAP_LE GAP LE Module
  * @{
  */

/** @addtogroup GAP_LE_PAST_SENDER GAP LE PAST Sender Module
  * @{
  */

/*============================================================================*
 *                         Macros
 *============================================================================*/
/** @defgroup GAP_LE_PAST_SENDER_Exported_Macros GAP LE PAST Sender Exported Macros
  * @{
  */

/** End of GAP_LE_PAST_SENDER_Exported_Macros
  * @}
  */

/*============================================================================*
 *                         Types
 *============================================================================*/
/** @defgroup GAP_LE_PAST_SENDER_Exported_Types GAP LE PAST Sender Exported Types
  * @{
  */

/** End of GAP_LE_PAST_SENDER_Exported_Types
  * @}
  */

/*============================================================================*
 *                         Functions
 *============================================================================*/
/** @defgroup GAP_LE_PAST_SENDER_Exported_Functions GAP LE PAST Sender Exported Functions
  * @brief
  * @{
  */
#if F_BT_LE_5_1_PAST_SENDER_ADV_SUPPORT
T_GAP_CAUSE le_past_sender_periodic_adv_set_info_transfer(uint8_t conn_id, uint16_t service_data,
                                                          uint8_t adv_handle);
#endif

#if F_BT_LE_5_1_PAST_SENDER_SYNC_SUPPORT
T_GAP_CAUSE le_past_sender_periodic_adv_sync_transfer(uint8_t conn_id, uint16_t service_data,
                                                      uint8_t sync_id);
#endif
/** End of GAP_LE_PAST_SENDER_Exported_Functions
  * @}
  */

/** End of GAP_LE_PAST_SENDER
  * @}
  */

/** End of GAP_LE
  * @}
  */

/** End of GAP
  * @}
  */


#endif
#ifdef __cplusplus
}
#endif

#endif /* GAP_PAST_SENDER_H */
