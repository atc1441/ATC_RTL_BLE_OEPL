/**
*********************************************************************************************************
*               Copyright(c) 2016, Realtek Semiconductor Corporation. All rights reserved.
*********************************************************************************************************
* @file      gap_aox_connless_receiver.h
* @brief    Head file for GAP AoA/AoD
* @details
* @author
* @date      2020-06-18
* @version   v0.8
* *********************************************************************************************************
*/

/*============================================================================*
 *               Define to prevent recursive inclusion
 *============================================================================*/
#ifndef GAP_AOX_CONNLESS_RECEIVER_H
#define GAP_AOX_CONNLESS_RECEIVER_H

#ifdef __cplusplus
extern "C"
{
#endif

/*============================================================================*
 *                        Header Files
 *============================================================================*/
#include "upperstack_config.h"
#include "gap_le.h"
#include "gap_le_types.h"

#if F_BT_LE_5_1_AOX_CONNLESS_RECEIVER_SUPPORT

/** @addtogroup GAP GAP Module
  * @{
  */

/** @addtogroup GAP_LE GAP LE Module
  * @{
  */

/** @addtogroup GAP_LE_AOX GAP LE AoA/AoD Module
  * @{
  */


/*============================================================================*
 *                         Macros
 *============================================================================*/
/** @defgroup GAP_LE_AOX_Exported_Macros GAP LE AoX Exported Macros
  * @{
  */
/** @defgroup AOX_CONNLESS_RECEIVER_SAMPLING_ENABLE Periodic Advertising Enable flag
  * @brief Use the macro definitions to describe the enable parameters.
  * @{
  */
#define AOX_CONNLESS_RECEIVER_SAMPLING_ENABLE_IQ_SAMPLING_DISABLED      0x00   /**< Connectionless IQ sampling is disabled (default). */
#define AOX_CONNLESS_RECEIVER_SAMPLING_ENABLE_IQ_SAMPLING_ENABLED       0x01   /**< Connectionless IQ sampling is enabled. */
/** End of AOX_CONNLESS_RECEIVER_SAMPLING_ENABLE
  * @}
  */

/** End of GAP_LE_AOX_Exported_Macros
  * @}
  */

/*============================================================================*
 *                         Types
 *============================================================================*/

/*============================================================================*
 *                         Functions
 *============================================================================*/

/** @defgroup GAP_LE_AOA_AOD_Exported_Functions GAP LE AoA/AoD Exported Functions
  * @brief
  * @{
  */
/*
    sampling_enable: @ref AOX_CONNLESS_RECEIVER_SAMPLING_ENABLE
*/
T_GAP_CAUSE le_aox_connless_receiver_set_iq_sampling_enable(uint8_t sync_id,
                                                            uint8_t sampling_enable, T_GAP_SLOT_DUATIONS_TYPE slot_durations, uint8_t max_sampled_ctes,
                                                            uint8_t switching_pattern_length, uint8_t *p_antenna_ids);

/** End of GAP_LE_AOA_AOD_Exported_Functions
  * @}
  */

/** End of GAP_LE_AOA_AOD
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

#endif /* GAP_AOX_CONNLESS_RECEIVER_H */
