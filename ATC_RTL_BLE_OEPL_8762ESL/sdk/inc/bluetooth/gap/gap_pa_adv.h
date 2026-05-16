/**
*********************************************************************************************************
*               Copyright(c) 2016, Realtek Semiconductor Corporation. All rights reserved.
*********************************************************************************************************
* @file      gap_pa_adv.h
* @brief     Header file for Gap pa adv
* @details   This file defines periodic advertising related API.
* @author    ranhui
* @date      2020-10-18
* @version   v0.7
* *********************************************************************************************************
*/

/*============================================================================*
 *               Define to prevent recursive inclusion
 *============================================================================*/
#ifndef GAP_PA_ADV_H
#define GAP_PA_ADV_H

#ifdef __cplusplus
extern "C"
{
#endif

/*============================================================================*
 *                        Header Files
 *============================================================================*/
#include "upperstack_config.h"
#include "gap_le.h"

#if F_BT_LE_5_0_PA_ADV_SUPPORT

/** @addtogroup GAP GAP Module
  * @{
  */

/** @addtogroup GAP_LE GAP LE Module
  * @{
  */

/** @addtogroup GAP_LE_PA_ADV GAP LE PA Adv Module
  * @{
  */

/*============================================================================*
 *                         Macros
 *============================================================================*/
/** @defgroup GAP_LE_PERIODIC_ADV_Exported_Macros GAP LE Periodic Adv Exported Macros
  * @{
  */

/** @defgroup PA_ADV_PARAM Periodic Advertising Parameter flag
  * @brief Use the combination of macro definitions to set periodic advertising related parameters
           for a specified advertising set by calling @ref le_pa_adv_start_setting.
  * @{
  */
#define PA_ADV_SET_PERIODIC_ADV_PARAS      0x01   /**< Set advertising parameters supplied by @ref le_pa_adv_set_periodic_adv_param. */
#define PA_ADV_SET_PERIODIC_ADV_DATA       0x02   /**< Set advertising data supplied by @ref le_pa_adv_set_periodic_adv_data. */
/** End of PA_ADV_PARAM
  * @}
  */

/** @defgroup PERIODIC_ADV_PROP Periodic Advertising Properties flag
  * @brief Use the combination of macro definitions to indicate which fields should be included in the advertising packet.
  * @{
  */
#define PA_ADV_PROP_INCLUDE_TX_POWER      0x0040   /**< Include TxPower in the periodic header of the advertising PDU. */
/** End of PERIODIC_ADV_PROP
  * @}
  */

/** @defgroup PA_ADV_ENABLE Periodic Advertising Enable flag
  * @brief Use the combination of macro definitions to describe the enable parameters.
  * @{
  */
#define PA_ADV_ENABLE_ENABLE_PERIODIC_ADVERTISING      0x01   /**< Enable periodic advertising. */
#define PA_ADV_ENABLE_INCLUDE_ADI                      0x02   /**< Include the ADI field in AUX_SYNC_IND PDUs. */
/** End of PA_ADV_ENABLE
  * @}
  */
/** End of GAP_LE_PA_Exported_Macros
  * @}
  */

/*============================================================================*
 *                         Types
 *============================================================================*/
/** @defgroup GAP_LE_PA_ADV_Exported_Types GAP LE PA Adv Exported Types
  * @{
  */

/** @brief GAP periodic advertising adv state. */
typedef enum
{
    PA_ADV_STATE_IDLE,         /**< Idle, no advertising. */
    PA_ADV_STATE_START_EXT_ADV_STATE_IDLE, /**< Start Advertising when periodic advertising of specific advertising set is disabled. A temporary state, haven't received the result. */
    PA_ADV_STATE_WAIT_EXT_ADV_STATE_ADVERTISING,  /**< Periodic Advertising is not started until the periodic advertising of specific advertising set is enabled. */
    PA_ADV_STATE_START,        /**< Start Advertising when periodic advertising of specific advertising set is enabled. A temporary state, haven't received the result. */
    PA_ADV_STATE_ADVERTISING,  /**< Periodic Advertising. */
    PA_ADV_STATE_STOP,         /**< Stop Advertising. A temporary state, haven't received the result. */
} T_GAP_PA_ADV_STATE;

/** End of GAP_LE_PA_ADV_Exported_Types
  * @}
  */

/*============================================================================*
 *                         Functions
 *============================================================================*/
/** @defgroup GAP_LE_PA_ADV_Exported_Functions GAP LE PA Adv Exported Functions
  * @brief
  * @{
  */
T_GAP_CAUSE le_pa_adv_init(uint8_t adv_set_num);

/*
    pa_update_flags: @ref PA_ADV_PARAM
*/
T_GAP_CAUSE le_pa_adv_start_setting(uint8_t adv_handle, uint8_t pa_update_flags);

/*
    periodic_adv_prop: @ref PERIODIC_ADV_PROP
*/
T_GAP_CAUSE le_pa_adv_set_periodic_adv_param(uint8_t adv_handle, uint16_t periodic_adv_interval_min,
                                             uint16_t periodic_adv_interval_max, uint16_t periodic_adv_prop);

T_GAP_CAUSE le_pa_adv_set_periodic_adv_data(uint8_t adv_handle, uint16_t periodic_adv_data_len,
                                            uint8_t *p_periodic_adv_data, bool pa_unchanged_data_flag);

/*
    enable: @ref PA_ADV_ENABLE
*/
T_GAP_CAUSE le_pa_adv_set_periodic_adv_enable(uint8_t adv_handle, uint8_t enable);

/** End of GAP_LE_PA_ADV_Exported_Functions
  * @}
  */

/** End of GAP_LE_PA_ADV
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

#endif /* GAP_PA_ADV_H */
