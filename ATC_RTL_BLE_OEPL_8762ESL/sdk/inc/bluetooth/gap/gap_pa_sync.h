/**
*********************************************************************************************************
*               Copyright(c) 2016, Realtek Semiconductor Corporation. All rights reserved.
*********************************************************************************************************
* @file      gap_pa_sync.h
* @brief     Header file for Gap pa sync
* @details
* @author
* @date      2020-10-18
* @version   v0.6
* *********************************************************************************************************
*/

/* Define to prevent recursive inclusion **/
#ifndef GAP_PA_SYNC_H
#define GAP_PA_SYNC_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "upperstack_config.h"
#include "gap_le.h"

#if F_BT_LE_5_0_PA_SYNC_SUPPORT
#define GAP_INVALID_CONN_ID      0xFF
#define GAP_INVALID_SYNC_ID      0xFF
#define GAP_INVALID_SYNC_HANDLE  0xFFFF

/** @defgroup PA_SYNC_CREATE_SYNC_OPTIONS Synchronization State of Periodic Advertising Create Sync Options
  * @brief Use the combination of macro definitions to determine whether the Periodic Advertiser List is used,
          whether periodic advertising report for this periodic advertising train are initially enabled or disabled.
  * @{
  */
#define PA_SYNC_CREATE_SYNC_OPTIONS_USE_PERIODIC_ADV_LIST      0x01   /**< Use the Periodic Advertiser List to determine which advertiser to listen to.
                                                                        Otherwise, Use the Advertising_SID, AdvertisierAdvertiser_Address_Type, and
                                                                        Advertiser_Address parameters to determine which advertiser to listen to.*/
#define PA_SYNC_CREATE_SYNC_OPTIONS_REPORT_INITIALLY_DISABLED  0x02   /**< Reporting initially disabled.
                                                                        Otherwise, Reporting initially enabled.*/
#define PA_SYNC_CREATE_SYNC_OPTIONS_DUPLICATE_FILTER_INITIALLY_ENABLED  0x04   /**< Duplicate filtering initially enabled.
                                                                        Otherwise, Duplicate filtering initially disabled.*/
/** End of PA_SYNC_CREATE_SYNC_OPTIONS
  * @}
  */

/** @defgroup PA_SYNC_CREATE_SYNC_SYNC_CTE_TYPE Synchronization State of Periodic Advertising Create Sync Sync CTE Type
  * @brief Use the combination of macro definitions to
  * @{
  */
#define PA_SYNC_CREATE_SYNC_CTE_TYPE_CTE_IRRELEVANT             0x00   /**< A value of 0 (i.e. all bits clear) indicates that the presence or absence of
                                                                                              a Constant Tone Extension is irrelevant. */
#define PA_SYNC_CREATE_SYNC_CTE_TYPE_NOT_SYNC_WITH_AOA_CTE      0x01   /**< Do not sync to packets with an AoA Constant Tone Extension */
#define PA_SYNC_CREATE_SYNC_CTE_TYPE_NOT_SYNC_WITH_AOD_CTE_1US  0x02   /**< Do not sync to packets with an AoD Constant Tone Extension with 1 μs slots*/
#define PA_SYNC_CREATE_SYNC_CTE_TYPE_NOT_SYNC_WITH_AOD_CTE_2US  0x04   /**< Do not sync to packets with an AoD Constant Tone Extension with 2 μs slots*/
#define PA_SYNC_CREATE_SYNC_CTE_TYPE_NOT_SYNC_WITH_TYPE_3_CTE   0x08   /**< Do not sync to packets with a type 3 Constant Tone Extension (currently
                                                                            reserved for future use)*/
#define PA_SYNC_CREATE_SYNC_CTE_TYPE_NOT_SYNC_WITHOUT_CTE       0x10   /**< Do not sync to packets without a Constant Tone Extension*/
/** End of PA_SYNC_CREATE_SYNC_SYNC_CTE_TYPE
  * @}
  */

#if ((F_BT_LE_5_0_PA_SYNC_SUPPORT && F_BT_LE_5_1_SUPPORT) || F_BT_LE_5_1_PAST_RECIPIENT_SUPPORT)
/** @defgroup PA_SYNC_PA_RECEIVE_ENABLE_PARAM Enable parameter of Periodic Advertising Receive Enable
  * @brief Use the combination of macro definitions to determine whether reporting and duplicate filtering
          are enabled or disabled.
  * @{
  */
#define PA_SYNC_PA_RECEIVE_ENABLE_PARAM_REPORT_ENABLED      0x01   /**< Reporting enabled */
#define PA_SYNC_PA_RECEIVE_ENABLE_PARAM_DUPLICATE_FILTER_ENABLED  0x02   /**< Duplicate filtering enabled*/
/** End of PA_SYNC_PA_RECEIVE_ENABLE_PARAM
  * @}
  */
#endif

/** @defgroup GAP_PA_TERMINATE_SYNC_DEV_STATE GAP periodic advertising synchronization and scan device State
  * @{
  */
#define GAP_PA_TERMINATE_SYNC_DEV_STATE_IDLE           0   //!< Idle
#define GAP_PA_TERMINATE_SYNC_DEV_STATE_TERMINATING    1   //!< Terminating
/** @} End GAP_PA_SYNC_SCAN_DEV_STATE */

#if F_BT_LE_5_0_PA_SYNC_SCAN_SUPPORT
/** @defgroup GAP_PA_CREATE_SYNC_DEV_STATE GAP periodic advertising synchronization and scan device State
  * @{
  */
#define GAP_PA_CREATE_SYNC_DEV_STATE_IDLE              0   //!< Idle
#define GAP_PA_CREATE_SYNC_DEV_STATE_SYNCHRONIZING     1   //!< Synchronizing
/** @} End GAP_PA_SYNC_SCAN_DEV_STATE */
#endif

#if ((F_BT_LE_5_0_PA_SYNC_SUPPORT && F_BT_LE_5_1_SUPPORT) || F_BT_LE_5_1_PAST_RECIPIENT_SUPPORT)
/** @defgroup GAP_PA_RECEIVE_ENABLE_DEV_STATE GAP periodic advertising synchronization and scan device State
  * @{
  */
#define GAP_PA_RECEIVE_ENABLE_DEV_STATE_IDLE              0   //!< Idle
#define GAP_PA_RECEIVE_ENABLE_DEV_STATE_ENABLING          1   //!< Enabling
/** @} End GAP_PA_RECEIVE_ENABLE_DEV_STATE */
#endif

/** @brief GAP periodic advertising synchronization states*/
typedef enum
{
    GAP_PA_SYNC_STATE_TERMINATED = 0x00, //!< Terminated.
#if F_BT_LE_5_0_PA_SYNC_SCAN_SUPPORT
    GAP_PA_SYNC_STATE_SYNCHRONIZING_SCAN_IDLE = 0x01,  //!< Start synchronizing when extended scanning is disabled. A temporary state, haven't received the result.
    GAP_PA_SYNC_STATE_SYNCHRONIZING_WAIT_SCANNING = 0x02, //!< No attempt to synchronize will take place when extended scanning is disabled.
#endif
    GAP_PA_SYNC_STATE_SYNCHRONIZING = 0x03,   //!< Start synchronizing when extended scanning is enabled.
    GAP_PA_SYNC_STATE_SYNCHRONIZED = 0x04,    //!< Synchronized
    GAP_PA_SYNC_STATE_TERMINATING = 0x05,//!< Terminate synchronization.
} T_GAP_PA_SYNC_STATE;

/** @brief LE periodic advertising sync parameter types */
typedef enum
{
    PA_SYNC_PARAM_PERIODIC_ADV_LIST_SIZE = 0x2A0, //!< Periodic advertiser list size. Read only.
    PA_SYNC_PARAM_SYNCHRONIZED_PARAM     = 0x2A1, //!< PA synchronized parameters. Read only. Identifier is sync_id.
} T_GAP_PA_SYNC_PARAM_TYPE;

typedef struct
{
    uint16_t         sync_handle;
    uint8_t          adv_sid;
    uint8_t          adv_addr_type;
    uint8_t          adv_addr[GAP_BD_ADDR_LEN];  /* current remote BD */
    uint16_t         skip;   /* Range: 0x0000 to 0x01F3 */
    uint16_t         sync_timeout;  /* Range: 0x000A to 0x4000*/
    uint8_t          sync_cte_type;
    T_GAP_PHYS_TYPE  adv_phy;
    uint8_t          adv_clock_accuracy;
    uint16_t         periodic_adv_interval;
    bool             sync_transfer_received_flag;
} T_GAP_PA_SYNC_COMMON_SYNC_PARAM;

#if F_BT_LE_5_1_PAST_RECIPIENT_SUPPORT
typedef struct
{
    uint8_t         conn_id;
    uint16_t        service_data;
} T_GAP_PAST_SYNC_TRANSFER_RECEIVED_PARAM;
#endif

#if F_BT_LE_5_0_PA_SYNC_SCAN_SUPPORT
typedef struct
{
    uint8_t options; /* @ref PA_SYNC_CREATE_SYNC_OPTIONS */
    uint8_t sync_cte_type; /* @ref PA_SYNC_CREATE_SYNC_SYNC_CTE_TYPE */
    uint8_t adv_sid;
    T_GAP_PA_SYNC_ADV_ADDR_TYPE
    adv_addr_type; /* @ref Only PA_SYNC_ADV_ADDR_PUBLIC and PA_SYNC_ADV_ADDR_RANDOM can be used. */
    uint8_t adv_addr[GAP_BD_ADDR_LEN];
    uint16_t skip;
    uint16_t sync_timeout;
} T_GAP_PA_SYNC_CREATE_SYNC_PARAM;
#endif

T_GAP_CAUSE le_pa_sync_init(uint8_t sync_handle_num);
T_GAP_CAUSE le_pa_sync_get_param(T_GAP_PA_SYNC_PARAM_TYPE param, void *p_value, uint8_t sync_id);
T_GAP_CAUSE le_pa_sync_terminate_sync(uint8_t sync_id);
T_GAP_CAUSE le_pa_sync_modify_periodic_adv_list(T_GAP_PA_SYNC_PERIODIC_ADV_LIST_OP operation,
                                                uint8_t *adv_addr,
                                                T_GAP_PA_SYNC_ADV_ADDR_TYPE adv_addr_type, uint8_t adv_sid);

#if F_BT_LE_5_0_PA_SYNC_SCAN_SUPPORT
T_GAP_CAUSE le_pa_sync_create_sync(T_GAP_PA_SYNC_CREATE_SYNC_PARAM *p_pa_sync_create_sync_param,
                                   uint8_t *p_sync_id);
#endif

#if ((F_BT_LE_5_0_PA_SYNC_SUPPORT && F_BT_LE_5_1_SUPPORT) || F_BT_LE_5_1_PAST_RECIPIENT_SUPPORT)
/*
    enable: @ref PA_SYNC_PA_RECEIVE_ENABLE_PARAM
*/
T_GAP_CAUSE le_pa_sync_set_periodic_adv_receive_enable(uint8_t sync_id, uint8_t enable);
#endif
#endif
#ifdef __cplusplus
}
#endif

#endif /* GAP_PA_SYNC_H */
