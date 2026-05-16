/**
*********************************************************************************************************
*               Copyright(c) 2016, Realtek Semiconductor Corporation. All rights reserved.
*********************************************************************************************************
* @file      gap_aox_connless_transmitter.h
* @brief    Head file for GAP AoA/AoD connless transmitter
* @details
* @author
* @date      2020-10-18
* @version   v0.4
* *********************************************************************************************************
*/

/*============================================================================*
 *               Define to prevent recursive inclusion
 *============================================================================*/
#ifndef GAP_AOX_CONNLESS_TRANSMITTER_H
#define GAP_AOX_CONNLESS_TRANSMITTER_H

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

#if F_BT_LE_5_1_AOX_CONNLESS_TRANSMITTER_SUPPORT

/** @addtogroup GAP GAP Module
  * @{
  */

/** @addtogroup GAP_LE GAP LE Module
  * @{
  */

/** @addtogroup GAP_LE_AOX_CONNLESS_TRANSMITTER GAP LE AoA/AoD Module
  * @{
  */


/*============================================================================*
 *                         Macros
 *============================================================================*/
/** @defgroup GAP_LE_AOX_CONNLESS_TRANSMITTER_Exported_Macros GAP LE AoX Exported Macros
  * @{
  */

/** @defgroup AOX_CONNLESS_TRANSMITTER_CTE_ENABLE Periodic Advertising Enable flag
  * @brief Use the macro definitions to describe the enable parameters.
  * @{
  */
#define AOX_CONNLESS_TRANSMITTER_CTE_ENABLE_ADV_WITH_CTE_DISABLED      0x00   /**< Advertising with Constant Tone Extension is disabled (default). */
#define AOX_CONNLESS_TRANSMITTER_CTE_ENABLE_ADV_WITH_CTE_ENABLED       0x01   /**< Advertising with Constant Tone Extension is enabled. */
/** End of AOX_CONNLESS_TRANSMITTER_CTE_ENABLE
  * @}
  */

/** End of GAP_LE_AOX_CONNLESS_TRANSMITTER_Exported_Macros
  * @}
  */

/*============================================================================*
 *                         Types
 *============================================================================*/
/** @defgroup GAP_LE_AOX_CONNLESS_TRANSMITTER_Exported_Types GAP LE AoA/AoD Exported Types
  * @brief
  * @{
  */
typedef enum
{
    GAP_AOX_CONNLESS_TRANSMITTER_CTE_TYPES_AOA = 0x00,      /**< AoA Constant Tone Extension. */
    GAP_AOX_CONNLESS_TRANSMITTER_CTE_TYPES_AOD_1US = 0x01,  /**< AoD Constant Tone Extension with 1 μs slots. */
    GAP_AOX_CONNLESS_TRANSMITTER_CTE_TYPES_AOD_2US = 0x02,  /**< AoD Constant Tone Extension with 2 μs slots. */
} T_GAP_AOX_CONNLESS_TRANSMITTER_CTE_TYPES;

typedef enum
{
    AOX_CONNLESS_TRANSMITTER_STATE_IDLE = 0x00,
    AOX_CONNLESS_TRANSMITTER_STATE_ENABLING_EXT_ADV_STATE_PA_ADV_STATE_IDLE = 0x01,
    AOX_CONNLESS_TRANSMITTER_STATE_WAIT_EXT_ADV_STATE_PA_ADV_STATE_ADVERTISING = 0x02,
    AOX_CONNLESS_TRANSMITTER_STATE_ENABLING_PA_ADV_STATE_IDLE = 0x03,
    AOX_CONNLESS_TRANSMITTER_STATE_WAIT_PA_ADV_STATE_ADVERTISING = 0x04,
    AOX_CONNLESS_TRANSMITTER_STATE_ENABLING_EXT_ADV_STATE_IDLE = 0x05,
    AOX_CONNLESS_TRANSMITTER_STATE_WAIT_EXT_ADV_STATE_ADVERTISING = 0x06,
    AOX_CONNLESS_TRANSMITTER_STATE_ENABLING = 0x07,
    AOX_CONNLESS_TRANSMITTER_STATE_TRANSMITTING = 0x08,  /**< Transmitting Constant Tone Extensions. */
    AOX_CONNLESS_TRANSMITTER_STATE_DISABLING = 0x09,
} T_GAP_AOX_CONNLESS_TRANSMITTER_STATE;

/** End of GAP_LE_AOX_CONNLESS_TRANSMITTER_Exported_Types
  * @}
  */

/*============================================================================*
 *                         Functions
 *============================================================================*/

/** @defgroup GAP_LE_AOA_AOD_Exported_Functions GAP LE AoA/AoD Exported Functions
  * @brief
  * @{
  */

T_GAP_CAUSE le_aox_connless_transmitter_init(uint8_t adv_set_num);

T_GAP_CAUSE le_aox_connless_transmitter_set_cte_transmit_params(uint8_t adv_handle,
                                                                uint8_t cte_length, T_GAP_AOX_CONNLESS_TRANSMITTER_CTE_TYPES cte_type, uint8_t cte_count,
                                                                uint8_t switching_pattern_length, uint8_t *p_antenna_ids);

/*
    cte_enable: @ref AOX_CONNLESS_TRANSMITTER_CTE_ENABLE
*/
T_GAP_CAUSE le_aox_connless_transmitter_set_cte_transmit_enable(uint8_t adv_handle,
                                                                uint8_t cte_enable);

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

#endif /* GAP_AOX_CONNLESS_TRANSMITTER_H */
