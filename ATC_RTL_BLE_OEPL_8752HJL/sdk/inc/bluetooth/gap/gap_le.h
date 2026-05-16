/**
*****************************************************************************************
*     Copyright(c) 2016, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
  * @file    gap_le.h
  * @brief   This file contains all the constants and function prototypes for the GAP protocol.
  * @details
  * @author  jane
  * @date    2016-02-18
  * @version v1.0
  * *************************************************************************************
  */

/*============================================================================*
 *               Define to prevent recursive inclusion
 *============================================================================*/
#ifndef GAP_LE_H
#define GAP_LE_H

#ifdef __cplusplus
extern "C"
{
#endif

/*============================================================================*
 *                        Header Files
 *============================================================================*/
#include <gap.h>
#include <gap_callback_le.h>


/** @addtogroup GAP_LE_COMMON GAP LE Common Module
  * @brief GAP LE common module
  * @{
  */

/*============================================================================*
 *                         Types
 *============================================================================*/
/** @defgroup GAP_LE_Exported_Types GAP LE Exported Types
  * @brief
  * @{
  */
/** @brief  GAP LE Parameter Types.*/
typedef enum
{
    GAP_PARAM_DEV_STATE                = 0x220, //!< Device's current GAP device state. Read only. Size is sizeof(@ref T_GAP_DEV_STATE).
    GAP_PARAM_APPEARANCE               = 0x221, //!< Local Device's Appearance. Read/Write. Size is uint16. Appearance value please refer to GAP Appearance Values.(@ref GAP_LE_APPEARANCE_VALUES).
    GAP_PARAM_DEVICE_NAME              = 0x222, //!< Local Device's Name. Write Only. Name string length is @ref GAP_DEVICE_NAME_LEN.
    GAP_PARAM_SLAVE_INIT_GATT_MTU_REQ  = 0x223, //!< Slave initiates the GATT exchange MTU procedure. Write Only. Size is 1 octet.
    GAP_PARAM_RANDOM_ADDR              = 0x224, //!< Random address. Write Only. Size is @ref GAP_BD_ADDR_LEN.
    GAP_PARAM_LATEST_CONN_BD_ADDR      = 0x226, //!< Latest connected Bluetooth device address. Read Only. Size is @ref GAP_BD_ADDR_LEN.
    GAP_PARAM_LATEST_CONN_BD_ADDR_TYPE = 0x227, //!< Latest connected Bluetooth device address type. Read Only. Size is sizeof(@ref T_GAP_REMOTE_ADDR_TYPE).
    GAP_PARAM_HANDLE_CREATE_CONN_IND   = 0x228, //!< APP handles the create connection indication message. Write Only. Size is 1 octet.
    GAP_PARAM_DEFAULT_PHYS_PREFER      = 0x229, //!< Preferred values for the transmitter PHY and receiver PHY to be used for all subsequent connections over the LE transport. Write Only. Size is 1 octet.
    GAP_PARAM_DEFAULT_TX_PHYS_PREFER   = 0x22a, //!< The transmitter PHYs that the Host prefers the Controller to use. Write Only. Size is 1 octet.
    GAP_PARAM_DEFAULT_RX_PHYS_PREFER   = 0x22b, //!< The receiver PHYs that the Host prefers the Controller to use. Write Only. Size is 1 octet.
    GAP_PARAM_USE_EXTENDED_ADV         = 0x22c, //!< Use LE Advertising Extensions. Write Only. Size is 1 octet.
    GAP_PARAM_SET_REM_MIN_SCA          = 0x22d, //!< Set Minimum Remote SCA. Write Only. Size is 1 octet.
    GAP_PARAM_DEFAULT_DATA_LEN_MAX_TX_OCTETS = 0x22e, //!< The Host's suggested value for the Controller's maximum transmitted number of payload octets in LL Data PDUs to be used for new connections. Write Only. Size is uint16. Value range: 0x001B to 0x00FB.
    GAP_PARAM_DEFAULT_DATA_LEN_MAX_TX_TIME   = 0x22f, //!< The Host's suggested value for the Controller's maximum packet transmission time in microseconds for packets containing LL Data PDUs to be used for new connections. Write Only. Size is uint16. Value range: 0x0148 to 0x4290.

    GAP_PARAM_LOCAL_FEATURES           = 0x230, //!< Local supported features. Read Only. Size is @ref GAP_LE_SUPPORTED_FEATURES_LEN.
    GAP_PARAM_DS_POOL_ID               = 0x231, //!< Downstream PoolID. Read only. Size is uint16.
    GAP_PARAM_DS_DATA_OFFSET           = 0x232, //!< Downstream pool buffer data offset. Read only. Size is uint16.
    GAP_PARAM_LE_REMAIN_CREDITS        = 0x233, //!< Remaining credits available for TX. Read only. Size is uint16.
    GAP_PARAM_MAX_WL_SIZE              = 0x234, //!< Max Filter Accept List size. Read only. Size is uint16.
    GAP_PARAM_MIN_TX_POWER             = 0x235, //!< The minimum transmit powers supported by the Controller. Read Only. Size is 1 octet.
    GAP_PARAM_MAX_TX_POWER             = 0x236, //!< The maximum transmit powers supported by the Controller. Read Only. Size is 1 octet.
    GAP_PARAM_ISOCH_HOST_SUPPORT       = 0x237, //!< Use CIS of LE Isochronous Channels. Write Only. Size is 1 octet.
} T_GAP_LE_PARAM_TYPE;

/** End of GAP_LE_Exported_Types
  * @}
  */


/*============================================================================*
 *                         Functions
 *============================================================================*/
/**
 * @defgroup GAP_LE_COMMON_Exported_Functions GAP LE Common Exported Functions
 *
 * @{
 */

/**
  * @brief Callback for GAP LE to notify APP.
  * @param[in] cb_type Callback msg type @ref GAP_LE_MSG_Types.
  * @param[in] p_cb_data Point to callback data @ref T_LE_CB_DATA.
  * @return    Result.
  * @retval    result @ref T_APP_RESULT.
  */
typedef T_APP_RESULT(*P_FUN_LE_APP_CB)(uint8_t cb_type, void *p_cb_data);

/**
  * @brief  Initialize parameters of GAP.
  * @param[in] link_num Initialize link number.
  * @retval true Success.
  * @retval false Failed because of invalid parameter.
 *
 * <b>Example usage</b>
 * \code{.c}
    int main(void)
    {
        board_init();
        driver_init();
        le_gap_init(1);
        app_le_gap_init();
        app_le_profile_init();
        pwr_mgr_init();
        task_init();
        os_sched_start();

        return 0;
    }
 * \endcode
 */
bool        le_gap_init(uint8_t link_num);

/**
  * @brief  Set GAP message inform way.
  *
  * Default value is true.
  * 1. When use_msg is true, GAP will send the GAP message to io_queue registered by gap_start_bt_stack.
  *    Message type is @ref GAP_MSG_TYPE.
  * 2. When use_msg is false, GAP will send the GAP message using callback function registered by le_register_app_cb.
  *    Message type is @ref GAP_MSG_LE_GAP_STATE_MSG.
  *
  * @param[in] use_msg Whether to use message.
  * @return void.
 *
 * <b>Example usage</b>
 * \code{.c}
    int test(void)
    {
        le_gap_msg_info_way(false);
    }
    T_APP_RESULT app_gap_callback(uint8_t cb_type, void *p_cb_data)
    {
        T_APP_RESULT result = APP_RESULT_SUCCESS;
        T_LE_CB_DATA cb_data;
        memcpy(&cb_data, p_cb_data, sizeof(T_LE_CB_DATA));
        APP_PRINT_TRACE1("app_gap_callback: msgType %d", cb_type);
        switch (cb_type)
        {
        ...
        case GAP_MSG_LE_GAP_STATE_MSG:
            APP_PRINT_INFO0("GAP_MSG_LE_GAP_STATE_MSG");
            app_handle_gap_msg((T_IO_MSG *)cb_data->p_gap_state_msg);
            break;
        ...
        }
    }

 * \endcode
 */
void        le_gap_msg_info_way(bool use_msg);

/**
  * @brief  Get max supported LE link count.
  * @retval max_link_num Max supported LE link count.
 *
 * <b>Example usage</b>
 * \code{.c}
    void test(void)
    {
        link_num = le_get_max_link_num();
        le_gap_init(link_num);
    }
 * \endcode
 */
uint8_t     le_get_max_link_num(void);

/**
  * @brief  Register APP callback from GAP LE.
  * @param[in]  app_callback Callback function defined by APP.
  * @return void.
 *
 * <b>Example usage</b>
 * \code{.c}
    void app_le_profile_init(void)
    {
        le_register_app_cb(app_gap_callback);
        client_init(client_num);
        simple_ble_client_id = simp_ble_add_client(app_client_callback);
        client_register_general_client_cb(app_client_callback);
    }
 * \endcode
 */
void        le_register_app_cb(P_FUN_LE_APP_CB app_callback);

/**
  * @brief  Set a GAP LE parameter.
  *
  * This function can be called with a GAP LE Parameter ID and it will set the
  *         GAP LE Parameter. GAP LE Parameter IDs are defined in @ref T_GAP_LE_PARAM_TYPE.
  *         If the "len" field sets to the size of a "uint16_t", the
  *         "p_value" field must point to a data with type of "uint16_t".
  *
  * @param[in]  param Profile parameter ID: @ref T_GAP_LE_PARAM_TYPE.
  * @param[in]  len Length of data to write.
  * @param[in]  p_value Pointer to data to write.  This is dependent on
  *                  the parameter ID and will be cast to the appropriate
  *                  data type (For example: if data type param is uint16, p_value will be cast to
  *                  pointer of uint16_t).
  * @return Set result.
  * @retval GAP_CAUSE_SUCCESS Set parameter success.
  * @retval other Set parameter failed.
  *
  * <b>Example usage</b>
  * \code{.c}
    void app_le_gap_init(void)
    {
        ...
        //device name and device appearance
        uint8_t  device_name[GAP_DEVICE_NAME_LEN] = "BLE_PERIPHERAL";
        uint16_t appearance = GAP_GATT_APPEARANCE_UNKNOWN;

        //Set device name and device appearance
        le_set_gap_param(GAP_PARAM_DEVICE_NAME, GAP_DEVICE_NAME_LEN, device_name);
        le_set_gap_param(GAP_PARAM_APPEARANCE, sizeof(appearance), &appearance);
        ...
    }
  * \endcode
  */
T_GAP_CAUSE le_set_gap_param(T_GAP_LE_PARAM_TYPE param, uint8_t len, void *p_value);

/**
  * @brief  Get a GAP LE parameter.
  *
  * This function can be called with a GAP LE Parameter ID and it will get a
  *         GAP LE Parameter. GAP LE Parameter IDs are defined in @ref T_GAP_LE_PARAM_TYPE.
  *         The "p_value" field must point to an appropriate data type matches the Parameter ID.
  *
  * @param[in]  param Profile parameter ID: @ref T_GAP_LE_PARAM_TYPE.
  * @param[in,out] p_value Pointer to location to get the parameter value.  This is dependent on
  *                  the parameter ID and will be cast to the appropriate
  *                  data type (For example: if data type of param is uint16_t, p_value will be cast to
 *                   pointer of uint16_t).
  *
  * @return Get result.
  * @retval GAP_CAUSE_SUCCESS Get parameter success.
  * @retval other Get parameter failed.
  *
  * <b>Example usage</b>
  * \code{.c}
    void test(void)
    {
        uint8_t gap_link_credits;

        T_GAP_CAUSE cause = le_get_gap_param(GAP_PARAM_LE_REMAIN_CREDITS, &gap_link_credits);
    }
  * \endcode
  */
T_GAP_CAUSE le_get_gap_param(T_GAP_LE_PARAM_TYPE param, void *p_value);

/**
  * @brief  Modify local Filter Accept List.
  *
  * @note This function can be called to add, remove, or clear the Filter Accept List.
  *
  * Applications can only call this API after the Bluetooth Host is ready.
  *
  * Explanation: If the Bluetooth Host is ready, the application will be notified by message @ref GAP_MSG_LE_DEV_STATE_CHANGE
  * with new_state about gap_init_state, which is configured as @ref GAP_INIT_STATE_STACK_READY.
  *
  * @param[in] operation Add, remove, or clear the Filter Accept List, @ref T_GAP_WHITE_LIST_OP.
  * @param[in] bd_addr Bluetooth Device Address.
  * @param[in] bd_type Bluetooth Device Address type.
  * @retval GAP_CAUSE_SUCCESS Operation success.
  * @retval GAP_CAUSE_SEND_REQ_FAILED Operation failure.
  *
  * <b>Example usage</b>
  * \code{.c}
    void test(void)
    {
        T_GAP_WHITE_LIST_OP operation = GAP_WHITE_LIST_OP_ADD;
        uint8_t bd_addr[BD_ADDR_SIZE] = {0};
        T_GAP_REMOTE_ADDR_TYPE bd_type = GAP_REMOTE_ADDR_LE_PUBLIC;
        le_modify_white_list(operation, bd_addr, bd_type);
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
        case GAP_MSG_LE_MODIFY_WHITE_LIST:
           APP_PRINT_INFO2("GAP_MSG_LE_MODIFY_WHITE_LIST: operation  0x%x, cause 0x%x",
                           cb_data.p_le_modify_white_list_rsp->operation,
                           cb_data.p_le_modify_white_list_rsp->cause);
           break;
        }
        ...
    }
  * \endcode
  */
T_GAP_CAUSE le_modify_white_list(T_GAP_WHITE_LIST_OP operation, uint8_t *bd_addr,
                                 T_GAP_REMOTE_ADDR_TYPE bd_type);

/**
  * @brief    Generate a local random address.
  * @param[in] rand_addr_type Bluetooth Device Random Address type, @ref T_GAP_RAND_ADDR_TYPE.
  * @param[in,out] random_bd Pointer to Bluetooth Device Address.
  * @retval GAP_CAUSE_SUCCESS Operation success.
  * @retval GAP_CAUSE_SEND_REQ_FAILED Operation failure.
  *
  * <b>Example usage</b>
  * \code{.c}
    void test(void)
    {
        T_GAP_RAND_ADDR_TYPE rand_addr_type = GAP_RAND_ADDR_RESOLVABLE;
        uint8_t random_bd[BD_ADDR_SIZE] = {0};
        le_gen_rand_addr(rand_addr_type, random_bd);
    }
  *
  * \endcode
  */
T_GAP_CAUSE le_gen_rand_addr(T_GAP_RAND_ADDR_TYPE rand_addr_type, uint8_t *random_bd);

/**
  * @brief    Set local random address.
  *
  * Applications can only call this API after the Bluetooth Host is ready.
  *
  * Explanation: If the Bluetooth Host is ready, the application will be notified by message @ref GAP_MSG_LE_DEV_STATE_CHANGE
  * with new_state about gap_init_state, which is configured as @ref GAP_INIT_STATE_STACK_READY.
  *
  * @param[in] random_bd Bluetooth Device Address.
  * @retval GAP_CAUSE_SUCCESS Operation success.
  * @retval GAP_CAUSE_SEND_REQ_FAILED Operation failure.
  *
  * <b>Example usage</b>
  * \code{.c}
    void test(void)
    {
        uint8_t random_bd[BD_ADDR_SIZE] = {0};
        le_set_rand_addr(random_bd);
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
        case GAP_MSG_LE_SET_RAND_ADDR:
            APP_PRINT_INFO1("GAP_MSG_LE_SET_RAND_ADDR: cause 0x%x",
                       cb_data.p_le_set_rand_addr_rsp->cause);
        break;
        }
        ...
    }
  * \endcode
  */
T_GAP_CAUSE le_set_rand_addr(uint8_t *random_bd);

/**
  * @brief    Configure local identity address.
  *
  * This function can be called before @ref gap_start_bt_stack is invoked.
  *
  * @param[in] addr Bluetooth Device Address.
  * @param[in] type Bluetooth Device Address type.
  * @retval GAP_CAUSE_SUCCESS Operation success.
  * @retval GAP_CAUSE_SEND_REQ_FAILED Operation failure.
  *
  * <b>Example usage</b>
  * \code{.c}
    void test(void)
    {
        T_GAP_IDENT_ADDR_TYPE type = GAP_IDENT_ADDR_PUBLIC;
        uint8_t addr[BD_ADDR_SIZE] = {0};
        le_cfg_local_identity_address(addr, type);
    }
  * \endcode
  */
T_GAP_CAUSE le_cfg_local_identity_address(uint8_t *addr, T_GAP_IDENT_ADDR_TYPE type);

/**
  * @brief   Set Host channel classification.
  *
  * If the request to send the operation is successful, the setting result will be returned by the callback function
  *          registered by @ref le_register_app_cb with msg type @ref GAP_MSG_LE_SET_HOST_CHANN_CLASSIF.
  *
  * Applications can only call this API after the Bluetooth Host is ready.
  *
  * Explanation: If the Bluetooth Host is ready, the application will be notified by message @ref GAP_MSG_LE_DEV_STATE_CHANGE
  * with new_state about gap_init_state, which is configured as @ref GAP_INIT_STATE_STACK_READY.
  *
  * @param[in]   p_channel_map Size: 5 octets (37 bits meaningful).
  *                            This parameter contains 37 1-bit fields.
  *                            The nth such field (in the range 0 to 36) contains the value for the
  *                            Link Layer channel index n.
  *                            Channel n is bad = 0.
  *                            Channel n is unknown = 1.
  *                            The most significant bits are reserved for future use.
  *                            At least one channel shall be marked as unknown.
  *
  * @return The result of sending request.
  * @retval GAP_CAUSE_SUCCESS Sending request operation is successful.
  * @retval Others Sending request operation is failed.
  *
  * <b>Example usage</b>
  * \code{.c}
    void test(void)
    {
        T_GAP_CAUSE cause = le_set_host_chann_classif(p_channel_map);
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
        case GAP_MSG_LE_SET_HOST_CHANN_CLASSIF:
           APP_PRINT_INFO1("GAP_MSG_LE_SET_HOST_CHANN_CLASSIF: cause 0x%x",
                           cb_data.p_le_set_host_chann_classif_rsp->cause);
           break;
        }
        ...
    }
  * \endcode
  */
T_GAP_CAUSE le_set_host_chann_classif(uint8_t *p_channel_map);

/**
  * @brief   Specify suggested values for the maximum transmission number of payload
             octets and maximum packet transmission time for new connections.
  *
  * Applications can only call this API after the Bluetooth Host is ready.
  *
  * Explanation: If the Bluetooth Host is ready, the application will be notified by message @ref GAP_MSG_LE_DEV_STATE_CHANGE
  * with new_state about gap_init_state, which is configured as @ref GAP_INIT_STATE_STACK_READY.
  *
  * @param[in]   tx_octets Maximum transmission number of payload octets, range: 0x001B-0x00FB.
  * @param[in]   tx_time   Maximum packet transmission time, range: 0x0148-0x4290.
  *
  * @retval GAP_CAUSE_SUCCESS: Operation success.
  * @retval GAP_CAUSE_SEND_REQ_FAILED: Operation failure.
  *
  * <b>Example usage</b>
  * \code{.c}
    void test(void)
    {
        uint16_t tx_octets = 0x00FB;
        uint16_t tx_time = 0x0848;
        le_write_default_data_len(tx_octets, tx_time);
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
        case GAP_MSG_LE_WRITE_DEFAULT_DATA_LEN:
           APP_PRINT_INFO1("GAP_MSG_LE_WRITE_DEFAULT_DATA_LEN: cause 0x%x",
                           cb_data->le_cause.cause);
           break;
        }
        ...
    }
  * \endcode
  */
T_GAP_CAUSE le_write_default_data_len(uint16_t tx_octets, uint16_t tx_time);

/**
  * @brief   Set Minimum Remote SCA.
  *
  * Applications can only call this API after Bluetooth Host is ready.
  *
  * Explanation: If Bluetooth Host is ready, the application will be notified by message @ref GAP_MSG_LE_DEV_STATE_CHANGE
  * with new_state about gap_init_state which is configured as @ref GAP_INIT_STATE_STACK_READY.
  *
  * Set Min_sca to compare with M_sca sent by master (carried in CONNECT_REQ pdu).
  *
  * The finally used master sca is set to MIN(Min_sca, M_sca).
  *
  * The default Min_sca is 7.
  *
  * The setting is used by all links in slave role, can set when boot up or link exist, and will reset to default value when HCI_RESET.
  *
  * @param[in] min_sca Minimum Remote SCA Index.
  *
  * @retval GAP_CAUSE_SUCCESS: Operation success.
  * @retval GAP_CAUSE_SEND_REQ_FAILED: Operation fail.
  *
  * <b>Example usage</b>
  * \code{.c}
    void test(void)
    {
        T_GAP_CAUSE cause;
        T_GAP_SCA_FIELD_ENCODING index = GAP_SCA_76_TO_100_PPM;
        cause = le_vendor_set_rem_min_sca(index);
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
        case GAP_MSG_LE_VENDOR_SET_MIN_REM_SCA:
            APP_PRINT_INFO1("GAP_MSG_LE_VENDOR_SET_MIN_REM_SCA: cause 0x%x",
                            p_data->le_cause.cause);
            break;
        }
        ...
    }
  * \endcode
  */
T_GAP_CAUSE le_vendor_set_rem_min_sca(T_GAP_SCA_FIELD_ENCODING min_sca);

/** @} */ /* End of group GAP_LE_COMMON_Exported_Functions */
/** @} */ /* End of group GAP_LE_COMMON */


/*-------------------------------------------------------------------
-------------------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif /* GAP_LE_H */




