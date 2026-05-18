/**
*****************************************************************************************
*     Copyright(c) 2017, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
  * @file    gap_vendor.h
  * @brief
  * @details
  * @author  ranhui_xia
  * @date    2017-08-02
  * @version v1.0
  ******************************************************************************
  * @attention
  * <h2><center>&copy; COPYRIGHT 2017 Realtek Semiconductor Corporation</center></h2>
  ******************************************************************************
  */
#ifndef GAP_VNR_H
#define GAP_VNR_H

#include <gap_le.h>

/** @addtogroup GAP_LE_VENDOR  GAP LE Vendor
  * @brief GAP LE vendor command API provides extended function for controller.
  * @{
  */

/*============================================================================*
 *                         Macros
 *============================================================================*/
/** @defgroup Gap_LE_Vendor_Exported_Macros GAP LE Vendor Exported Macros
  * @{
  */

/** @defgroup GAP_LE_Vendor_MSG_Types GAP LE Vendor Msg Types
  * @{
  */
#define GAP_MSG_LE_VENDOR_ADV_3_DATA_ENABLE         0xA0 //!<Response msg type for le_vendor_adv_3_data_enable
#define GAP_MSG_LE_VENDOR_ADV_3_DATA_SET            0xA1 //!<Response msg type for le_vendor_adv_3_data_set
#define GAP_MSG_LE_VENDOR_DROP_ACL_DATA             0xA4 //!<Response msg type for le_vendor_drop_acl_data
#define GAP_MSG_GAP_SW_RESET                        0xA5 //!<Response msg type for gap_sw_reset_req
#define GAP_MSG_GAP_SET_LPS_BOOTUP_ACTIVE_TIME      0xA6 //!<Response msg type for gap_set_lps_bootup_active_time
#define GAP_MSG_LE_AE_CODING_SCHEME                 0xA7 //!<Response msg type for le_ae_coding_scheme
#define GAP_MSG_LE_VENDOR_SET_PRIORITY              0xA8 //!<Response msg type for le_vendor_set_priority  
#define GAP_MSG_LE_VENDOR_AE_SCHEME                 0xA9 //!<Response msg type for le_vendor_ae_scheme 
#define GAP_MSG_LE_VENDOR_MEASURE_MASTER_CLK_FREQ   0xAA //!<Response msg type for le_vendor_measure_master_clk_freq_mode 
#define GAP_MSG_LE_VENDOR_MORE_PRECISE_32K          0xAB //!<Response msg type for le_vendor_more_precise_32k_option 

/**
  * @}
  */

/** @defgroup GAP_VENDOR_MSG_TYPE GAP Vendor Msg Types
  * @{
  */
#define GAP_MSG_GAP_VENDOR_SET_ANT_CTRL              0xB0

/**
  * @}
  */

/** End of GAP_LE_Vendor_Exported_Macros
  * @}
  */

/*============================================================================*
 *                         Types
 *============================================================================*/
/** @defgroup GAP_LE_Vendor_Exported_Types GAP LE Vendor Exported Types
  * @{
  */

typedef enum
{
    GAP_SW_RESET_IDLE_OR_ADV = 1,    //!< Software reset when advertising
} T_GAP_SW_RESET_MODE;

/** @brief Coding scheme of LE Coded PHY when device uses LE Advertising Extensions.*/
typedef enum
{
    GAP_AE_CODING_SCHEME_S8 = 2,  //!< Coding scheme S8
    GAP_AE_CODING_SCHEME_S2 = 3,  //!< Coding scheme S2
} T_GAP_AE_CODING_SCHEME;

/** @brief Mode of setting priority. */
typedef enum
{
    GAP_VENDOR_UPDATE_PRIORITY, //!< Set priority without operation of resetting priority
    GAP_VENDOR_SET_PRIORITY,    //!< Set priority after operation of resetting priority
    GAP_VENDOR_RESET_PRIORITY,  //!< Reset priority
} T_GAP_VENDOR_SET_PRIORITY_MODE;

/** @brief Definition of priority level. */
typedef enum
{
    GAP_VENDOR_PRIORITY_LEVEL_0,  //!< Level 0
    GAP_VENDOR_PRIORITY_LEVEL_1,  //!< Level 1
    GAP_VENDOR_PRIORITY_LEVEL_2,  //!< Level 2
    GAP_VENDOR_PRIORITY_LEVEL_3,  //!< Level 3
    GAP_VENDOR_PRIORITY_LEVEL_4,  //!< Level 4
    GAP_VENDOR_PRIORITY_LEVEL_5,  //!< Level 5
    GAP_VENDOR_PRIORITY_LEVEL_6,  //!< Level 6
    GAP_VENDOR_PRIORITY_LEVEL_7,  //!< Level 7
    GAP_VENDOR_PRIORITY_LEVEL_8,  //!< Level 8
    GAP_VENDOR_PRIORITY_LEVEL_9,  //!< Level 9
    GAP_VENDOR_PRIORITY_LEVEL_10, //!< Highest priority level 10
    GAP_VENDOR_RESERVED_PRIORITY, //!< Reserved
} T_GAP_VENDOR_PRIORITY_LEVEL;

/** @brief Mode of setting link priority. */
typedef enum
{
    GAP_VENDOR_NOT_SET_LINK_PRIORITY,       //!< Not set priority of link
    GAP_VENDOR_SET_SPECIFIC_LINK_PRIORITY,  //!< Set priority of specific links
    GAP_VENDOR_SET_ALL_LINK_PRIORITY,       //!< Set priority of all links
} T_GAP_VENDOR_SET_LINK_PRIORITY_MODE;

/** @brief Definition of common priority. */
typedef struct
{
    bool set_priority_flag;
    T_GAP_VENDOR_PRIORITY_LEVEL priority_level; /**< Priority is valid if set_priority_flag is true. */
} T_GAP_VENDOR_COMMON_PRIORITY;

/** @brief Definition of connection priority. */
typedef struct
{
    uint8_t conn_id;
    T_GAP_VENDOR_PRIORITY_LEVEL conn_priority_level;/**< Priority of specific connection. */
} T_GAP_VENDOR_CONN_PRIORITY;

/** @brief  Parameters of setting priority.*/
typedef struct
{
    T_GAP_VENDOR_SET_PRIORITY_MODE set_priority_mode;/**< Mode of setting priority.
                                                            (@ref T_GAP_VENDOR_SET_PRIORITY_MODE). */
    T_GAP_VENDOR_COMMON_PRIORITY adv_priority;/**< Priority of advertising.
                                                     (@ref T_GAP_VENDOR_COMMON_PRIORITY). */
    T_GAP_VENDOR_COMMON_PRIORITY scan_priority;/**< Priority of scan.
                                                      (@ref T_GAP_VENDOR_COMMON_PRIORITY). */
    T_GAP_VENDOR_COMMON_PRIORITY initiate_priority;/**< Priority of initiating.
                                                          (@ref T_GAP_VENDOR_COMMON_PRIORITY). */
    T_GAP_VENDOR_SET_LINK_PRIORITY_MODE link_priority_mode;/**< Mode of setting link priority.
                                                                  (@ref T_GAP_VENDOR_SET_LINK_PRIORITY_MODE). */
    T_GAP_VENDOR_PRIORITY_LEVEL link_priority_level;/**< Priority of all links is valid
                                                           if link_priority_mode is GAP_VENDOR_SET_ALL_LINK_PRIORITY. */
    uint8_t num_conn_ids;/**< Number of specific links is valid if link_priority_mode is GAP_VENDOR_SET_SPECIFIC_LINK_PRIORITY. */
    T_GAP_VENDOR_CONN_PRIORITY p_conn_id_list[1];/**< List of connection priority is valid
                                                        if link_priority_mode is GAP_VENDOR_SET_SPECIFIC_LINK_PRIORITY.
                                                        (@ref T_GAP_VENDOR_CONN_PRIORITY). */
} T_GAP_VENDOR_PRIORITY_PARAM;

typedef struct
{
    uint16_t cause;
} T_GAP_VENDOR_CAUSE;

typedef union
{
    T_GAP_VENDOR_CAUSE gap_vendor_cause;
} T_GAP_VENDOR_CB_DATA;

/** End of GAP_LE_Vendor_Exported_Types
  * @}
  */

/*============================================================================*
 *                         Functions
 *============================================================================*/

/** @defgroup GAP_LE_VENDOR_Exported_Functions GAP LE Vendor Exported Functions
  * @brief GAP LE vendor command Exported Functions
  * @{
  */

/**
 * @brief  Register callback to vendor command, when messages in @ref GAP_VENDOR_MSG_TYPE happens, it will callback to APP.
 * @param[in] vendor_callback Callback function provided by the APP to handle @ref GAP_VENDOR_MSG_TYPE messages sent from the GAP.
 *              @arg NULL -> Not send @ref GAP_VENDOR_MSG_TYPE messages to APP.
 *              @arg Other -> Use application defined callback function.
 * @return void.
 *
 * <b>Example usage</b>
 * \code{.c}
   void app_le_gap_init(void)
    {
        ......
        gap_register_vendor_cb(app_gap_vendor_callback);
    }
    void app_gap_vendor_callback(uint8_t cb_type, void *p_cb_data)
    {
        T_GAP_VENDOR_CB_DATA cb_data;
        memcpy(&cb_data, p_cb_data, sizeof(T_GAP_VENDOR_CB_DATA));
        APP_PRINT_INFO1("app_gap_vendor_callback: cb_type is %d", cb_type);
        switch (cb_type)
        {
        case GAP_MSG_GAP_VENDOR_SET_ANT_CTRL:
            APP_PRINT_INFO1("GAP_MSG_GAP_VENDOR_SET_ANT_CTRL: cause 0x%x",
                            cb_data.gap_vendor_cause.cause);
            break;
        default:
            break;
        }
        return;
    }
   \endcode
 */
void gap_register_vendor_cb(P_FUN_GAP_APP_CB vendor_callback);

/**
  * @brief  Enable 3 advertising channel advertising data.
  *         Set 3 ADV data please refer to @ref le_vendor_adv_3_data_enable.
  *
  * @param[in] enable
  * @arg true:  Enable each ADV channel with different data.
  * @arg false: Disable.
  *
  * @return Operation result.
  * @retval GAP_CAUSE_SUCCESS: Operation success.
  * @retval GAP_CAUSE_SEND_REQ_FAILED: Operation fail.
  *
  * <b>Example usage</b>
  * \code{.c}
    void gap_vendor_test(bool enable)
    {
        ...
        cause = le_vendor_adv_3_data_enable(enable);
        return cause;
    }
    T_APP_RESULT app_gap_callback(uint8_t cb_type, void *p_cb_data)
    {
        T_APP_RESULT result = APP_RESULT_SUCCESS;
        T_LE_CB_DATA cb_data;
        memcpy(&cb_data, p_cb_data, sizeof(T_LE_CB_DATA));
        APP_PRINT_TRACE1("app_gap_callback: cb_type %d", cb_type);
        switch (cb_type)
        {
        ...
        case GAP_MSG_LE_VENDOR_ADV_3_DATA_ENABLE:
            APP_PRINT_INFO1("GAP_MSG_LE_VENDOR_ADV_3_DATA_ENABLE: cause 0x%x",
                            p_data->le_cause.cause);
            break;
        }
        ...
    }
  * \endcode
  */
T_GAP_CAUSE le_vendor_adv_3_data_enable(bool enable);

/**
  * @brief  Set different advertising date or scan response data in 3 advertising channels.
  *         This command is used to set 38 / 39 channel data, set 37 channel data please use normal HCI command.
  *         It is necessary to enable 3 ADV data with @ref le_vendor_adv_3_data_enable.
  *
  * @param[in] type   LE vendor advertising data type @ref T_GAP_ADV_VENDOR_DATA_TYPE.
  * @param[in] len    The number of significant octets in the advertising data.
  * @param[in] p_data Pointer to data to write.
  *
  * @return Operation result.
  * @retval GAP_CAUSE_SUCCESS: Operation success.
  * @retval GAP_CAUSE_SEND_REQ_FAILED: Operation fail.
  * @retval GAP_CAUSE_INVALID_PARAM: Invalid parameter.
  *
  * <b>Example usage</b>
  * \code{.c}
    T_GAP_CAUSE gap_vendor_test(T_USER_CMD_PARSED_VALUE *p_parse_value)
    {
        T_GAP_CAUSE cause;
        T_GAP_ADV_VENDOR_DATA_TYPE type = (T_GAP_ADV_VENDOR_DATA_TYPE)p_parse_value->dw_param[0];
        uint8_t len = p_parse_value->dw_param[1];
        uint8_t value = p_parse_value->dw_param[2];
        uint8_t adv_data[31];
        if (len > GAP_MAX_ADV_LEN)
        {
            return (RESULT_ERR);
        }
        memset(adv_data, value, len);

        cause = le_vendor_adv_3_data_set((T_GAP_ADV_VENDOR_DATA_TYPE)type, len, adv_data);
        return (T_GAP_CAUSE)cause;
    }
    T_APP_RESULT app_gap_callback(uint8_t cb_type, void *p_cb_data)
    {
        T_APP_RESULT result = APP_RESULT_SUCCESS;
        T_LE_CB_DATA cb_data;
        memcpy(&cb_data, p_cb_data, sizeof(T_LE_CB_DATA));
        APP_PRINT_TRACE1("app_gap_callback: cb_type %d", cb_type);
        switch (cb_type)
        {
        ...
        case GAP_MSG_LE_VENDOR_ADV_3_DATA_SET:
            APP_PRINT_INFO2("GAP_MSG_LE_VENDOR_ADV_3_DATA_SET: type %d, cause 0x%x",
                            p_data->p_le_vendor_adv_3_data_set_rsp->type,
                            p_data->p_le_vendor_adv_3_data_set_rsp->cause);
            break;
        ...
    }
  * \endcode
  */
T_GAP_CAUSE le_vendor_adv_3_data_set(T_GAP_ADV_VENDOR_DATA_TYPE type,
                                     uint8_t len, uint8_t *p_data);

/**
  * @brief  LE drop ACL data.
  *
  * Drop pending LE acl packet that assigned by user except the acl packet which is currently waiting for ack.
  * The packet-dropping rule is cleared if link is disconnected or HCI_RESET.
  *
  * @param[in] conn_id  Connection ID for this link.
  * @param[in] mask     Assign the mask to compare with data.
  * @param[in] pattern  Drop data that match pattern.
  * @param[in] offset   The offset in octets started from data.
  * @return Operation result.
  * @retval GAP_CAUSE_SUCCESS: Operation success.
  * @retval GAP_CAUSE_SEND_REQ_FAILED: Operation fail.
  *
  * <b>Example usage</b>
  * \code{.c}
    static T_USER_CMD_PARSE_RESULT cmd_vdropdata(T_USER_CMD_PARSED_VALUE *p_parse_value)
    {
        T_GAP_CAUSE cause;
        uint8_t conn_id = p_parse_value->dw_param[0];

        cause = le_vendor_drop_acl_data(conn_id, 0xffff, 0x0015, 5);
        return (T_USER_CMD_PARSE_RESULT)cause;
    }
    T_APP_RESULT app_gap_callback(uint8_t cb_type, void *p_cb_data)
    {
        T_APP_RESULT result = APP_RESULT_SUCCESS;
        T_LE_CB_DATA cb_data;
        memcpy(&cb_data, p_cb_data, sizeof(T_LE_CB_DATA));
        APP_PRINT_TRACE1("app_gap_callback: cb_type %d", cb_type);
        switch (cb_type)
        {
        ...
        case GAP_MSG_LE_VENDOR_DROP_ACL_DATA:
            APP_PRINT_INFO1("GAP_MSG_LE_VENDOR_DROP_ACL_DATA: cause 0x%x",
                            p_data->le_cause.cause);
            break;
        ...
    }
  * \endcode
  */
T_GAP_CAUSE le_vendor_drop_acl_data(uint8_t conn_id, uint16_t mask, uint16_t pattern,
                                    uint8_t offset);

/**
  * @brief  Modify Bluetooth Host LE FW policy.
  *
  * @param[in] mask     Assign the mask to compare with data.
  * @param[in] value    Value to set with the mask.
  * @return Operation result.
  * @retval GAP_CAUSE_SUCCESS: Operation success.
  * @retval GAP_CAUSE_SEND_REQ_FAILED: Operation fail.
  */
T_GAP_CAUSE le_vendor_modify_bt_le_fw_policy(uint32_t mask, uint32_t value);

/**
  * @brief  Reset Bluetooth Host controller.
  *
  * @param[in] reset_mode  GAP software reset mode @ref T_GAP_SW_RESET_MODE.
  * @return Operation result.
  * @retval GAP_CAUSE_SUCCESS: Operation success.
  * @retval GAP_CAUSE_SEND_REQ_FAILED: Operation fail.
  *
  * <b>Example usage</b>
  * \code{.c}
    static T_USER_CMD_PARSE_RESULT cmd_reset(T_USER_CMD_PARSED_VALUE *p_parse_value)
    {
        T_GAP_CAUSE cause;
        T_GAP_SW_RESET_MODE reset_mode = (T_GAP_SW_RESET_MODE)p_parse_value->dw_param[0];
        cause = gap_sw_reset_req(reset_mode);
        return (T_USER_CMD_PARSE_RESULT)cause;
    }
    T_APP_RESULT app_gap_callback(uint8_t cb_type, void *p_cb_data)
    {
        T_APP_RESULT result = APP_RESULT_SUCCESS;
        T_LE_CB_DATA cb_data;
        memcpy(&cb_data, p_cb_data, sizeof(T_LE_CB_DATA));
        APP_PRINT_TRACE1("app_gap_callback: cb_type %d", cb_type);
        switch (cb_type)
        {
        ...
        case GAP_MSG_GAP_SW_RESET:
            APP_PRINT_INFO1("GAP_MSG_GAP_SW_RESET: cause 0x%x",
                            p_data->le_cause.cause);
            break;
        ...
    }
  * \endcode
  */
T_GAP_CAUSE gap_sw_reset_req(T_GAP_SW_RESET_MODE reset_mode);

/**
  * @brief  Set platform bootup active time.
  *
  * Platform is allowed to enter low power mode after this timeout value.
  * Can use this command at any time even if low power mode is set.
  *
  * @param[in] active_time  Unit: 50msec, Range:0x03-0x800. Default value is 5 sec.
  * @return Operation result.
  * @retval true Operation success.
  * @retval false Operation fail.
  *
  * <b>Example usage</b>
  * \code{.c}
    static T_USER_CMD_PARSE_RESULT cmd_lpstime(T_USER_CMD_PARSED_VALUE *p_parse_value)
    {
        uint16_t active_time = p_parse_value->dw_param[0];

        if(gap_set_lps_bootup_active_time(active_time))
        {
            return RESULT_SUCESS;
        }
        else
        {
            return RESULT_GAP_CAUSE_INVALID_PARAM;
        }
    }
    T_APP_RESULT app_gap_callback(uint8_t cb_type, void *p_cb_data)
    {
        T_APP_RESULT result = APP_RESULT_SUCCESS;
        T_LE_CB_DATA cb_data;
        memcpy(&cb_data, p_cb_data, sizeof(T_LE_CB_DATA));
        APP_PRINT_TRACE1("app_gap_callback: cb_type %d", cb_type);
        switch (cb_type)
        {
        ...
        case GAP_MSG_GAP_SET_LPS_BOOTUP_ACTIVE_TIME:
            APP_PRINT_INFO1("GAP_MSG_GAP_SET_LPS_BOOTUP_ACTIVE_TIME: cause 0x%x",
                            p_data->le_cause.cause);
            break;
        ...
    }
  * \endcode
  */
bool gap_set_lps_bootup_active_time(uint16_t active_time);

/**
  * @brief  Configure coding scheme of LE Coded PHY when device uses LE Advertising Extensions.
  *
  * @note Advertiser should delete advertising set before changing coding scheme.
  *
  * @param[in] coding_scheme  Coding scheme of LE Coded PHY when device uses LE Advertising Extensions, @ref T_GAP_AE_CODING_SCHEME.
  * @return Operation result.
  * @retval GAP_CAUSE_SUCCESS Send request success.
  * @retval other             Send request failed.
  *
  * <b>Example usage</b>
  * \code{.c}
    void test(void)
    {
        le_ae_coding_scheme(GAP_AE_CODING_SCHEME_S8);
    }
    T_APP_RESULT app_gap_callback(uint8_t cb_type, void *p_cb_data)
    {
        T_APP_RESULT result = APP_RESULT_SUCCESS;
        T_LE_CB_DATA cb_data;
        memcpy(&cb_data, p_cb_data, sizeof(T_LE_CB_DATA));
        APP_PRINT_TRACE1("app_gap_callback: cb_type %d", cb_type);
        switch (cb_type)
        {
        ...
        case GAP_MSG_LE_AE_CODING_SCHEME:
            APP_PRINT_INFO1("GAP_MSG_LE_AE_CODING_SCHEME: cause 0x%x",
                            cb_data->le_cause.cause);
        break;
        ...
    }
  * \endcode
  */
T_GAP_CAUSE le_ae_coding_scheme(T_GAP_AE_CODING_SCHEME coding_scheme);

/** End of GAP_LE_VENDOR_Exported_Functions
  * @}
  */

/** End of GAP_LE_VENDOR
  * @}
  */
#endif /* GAP_VNR_H */
