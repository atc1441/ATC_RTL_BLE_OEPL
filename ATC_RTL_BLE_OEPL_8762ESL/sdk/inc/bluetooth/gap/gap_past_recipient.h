/**
*********************************************************************************************************
*               Copyright(c) 2016, Realtek Semiconductor Corporation. All rights reserved.
*********************************************************************************************************
* @file      gap_past_recipient.h
* @brief     Header file for Gap past recipient
* @details
* @author
* @date      2020-10-18
* @version   v0.4
* *********************************************************************************************************
*/

/* Define to prevent recursive inclusion **/
#ifndef GAP_PAST_RECIPIENT_H
#define GAP_PAST_RECIPIENT_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "upperstack_config.h"
#include "gap_le.h"

#if F_BT_LE_5_1_PAST_RECIPIENT_SUPPORT
/*============================================================================*
 *                         Macros
 *============================================================================*/
/** @defgroup GAP_LE_PAST_RECIPIENT_Exported_Macros GAP LE PAST Recipient Exported Macros
  * @{
  */

/** End of GAP_LE_PAST_RECIPIENT_Exported_Macros
  * @}
  */

/*============================================================================*
 *                         Types
 *============================================================================*/
/** @defgroup GAP_LE_PAST_RECIPIENT_Exported_Types GAP LE PAST Recipient Exported Types
  * @{
  */

/** @brief GAP PAST recipient periodic adv sync transfer mode. */
typedef enum
{
    PERIODIC_ADV_SYNC_TRANSFER_MODE_NO_ATTEMPT_TO_SYNCHRONIZE = 0x00,   /**< No attempt is made to synchronize to the periodic advertising and
                                                                      no HCI_LE_Periodic_Advertising_Sync_Transfer_Received event is sent to the Host (default). */
    PERIODIC_ADV_SYNC_TRANSFER_MODE_PERIODIC_ADV_REPORT_DISABLED = 0x01, /**< An HCI_LE_Periodic_Advertising_Sync_Transfer_Received event is
sent to the Host. HCI_LE_Periodic_Advertising_Report events will be disabled. */
    PERIODIC_ADV_SYNC_TRANSFER_MODE_PERIODIC_ADV_REPORT_ENABLED_WITH_DUPLICATE_FILTER_DISABLED = 0x02,  /**< An HCI_LE_Periodic_Advertising_Sync_Transfer_Received event is
sent to the Host. HCI_LE_Periodic_Advertising_Report events will be enabled with duplicate filtering disabled. */
    PERIODIC_ADV_SYNC_TRANSFER_MODE_PERIODIC_ADV_REPORT_ENABLED_WITH_DUPLICATE_FILTER_ENABLED = 0x03,  /**< An HCI_LE_Periodic_Advertising_Sync_Transfer_Received event is
sent to the Host. HCI_LE_Periodic_Advertising_Report events will be enabled with duplicate filtering enabled. */
} T_GAP_PAST_RECIPIENT_PERIODIC_ADV_SYNC_TRANSFER_MODE;

/** End of GAP_LE_PAST_RECIPIENT_Exported_Types
  * @}
  */

/** @defgroup PAST_RECIPIENT_CREATE_SYNC_SYNC_CTE_TYPE Synchronization State of Periodic Advertising Create Sync Sync CTE Type
  * @brief Use the combination of macro definitions to specifies whether to only synchronize to periodic
           advertising with certain types of Constant Tone Extension.
  * @{
  */
#define PAST_RECIPIENT_PERIODIC_ADV_SYNC_TRANSFER_CTE_TYPE_CTE_IRRELEVANT             0x00   /**< A value of 0 (i.e. all bits clear) indicates that the presence or absence of
                                                                                              a Constant Tone Extension is irrelevant. */
#define PAST_RECIPIENT_PERIODIC_ADV_SYNC_TRANSFER_CTE_TYPE_NOT_SYNC_WITH_AOA_CTE      0x01   /**< Do not sync to packets with an AoA Constant Tone Extension */
#define PAST_RECIPIENT_PERIODIC_ADV_SYNC_TRANSFER_CTE_TYPE_NOT_SYNC_WITH_AOD_CTE_1US  0x02   /**< Do not sync to packets with an AoD Constant Tone Extension with 1 μs slots*/
#define PAST_RECIPIENT_PERIODIC_ADV_SYNC_TRANSFER_CTE_TYPE_NOT_SYNC_WITH_AOD_CTE_2US  0x04   /**< Do not sync to packets with an AoD Constant Tone Extension with 2 μs slots*/
#define PAST_RECIPIENT_PERIODIC_ADV_SYNC_TRANSFER_CTE_TYPE_NOT_SYNC_WITHOUT_CTE       0x10   /**< Do not sync to packets without a Constant Tone Extension*/
/** End of PAST_RECIPIENT_CREATE_SYNC_SYNC_CTE_TYPE
  * @}
  */
typedef struct
{
    T_GAP_PAST_RECIPIENT_PERIODIC_ADV_SYNC_TRANSFER_MODE mode;
    uint8_t cte_type; /* @ref PAST_RECIPIENT_CREATE_SYNC_SYNC_CTE_TYPE */
    uint16_t skip;
    uint16_t sync_timeout;
} T_GAP_PAST_RECIPIENT_PERIODIC_ADV_SYNC_TRANSFER_PARAM;

/*============================================================================*
 *                         Functions
 *============================================================================*/
/** @defgroup GAP_LE_PAST_SENDER_Exported_Functions GAP LE PAST Recipient Exported Functions
  * @brief
  * @{
  */
T_GAP_CAUSE le_past_recipient_set_default_periodic_adv_sync_transfer_params(
    T_GAP_PAST_RECIPIENT_PERIODIC_ADV_SYNC_TRANSFER_PARAM *p_periodic_adv_sync_transfer_param);

T_GAP_CAUSE le_past_recipient_set_periodic_adv_sync_transfer_params(uint8_t conn_id,
                                                                    T_GAP_PAST_RECIPIENT_PERIODIC_ADV_SYNC_TRANSFER_PARAM *p_periodic_adv_sync_transfer_param);
#endif
#ifdef __cplusplus
}
#endif

#endif /* GAP_PAST_RECIPIENT_H */
