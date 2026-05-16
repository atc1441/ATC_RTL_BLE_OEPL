/**
*****************************************************************************************
*     Copyright(c) 2016, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
  * @file    gap_privacy.h
  * @brief   This file contains all the functions prototypes for the GAP bond and pairing
  *          related functions.
  * @details
  * @author  ranhui
  * @date    2016-02-18
  * @version v1.0
  * *************************************************************************************
  */

/*============================================================================*
 *               Define to prevent recursive inclusion
 *============================================================================*/
#ifndef GAP_PRIVACY_H
#define GAP_PRIVACY_H

#ifdef __cplusplus
extern "C"
{
#endif


/*============================================================================*
 *                        Header Files
 *============================================================================*/
#include "upperstack_config.h"
#include "gap_le.h"
#include "gap_conn_le.h"
#include "gap_storage_le.h"

#if F_BT_LE_PRIVACY_SUPPORT
/** @defgroup GAP_PRIVACY_MODULE GAP LE Privacy
  * @brief GAP LE privacy
  * @{
  */

/*============================================================================*
 *                         Types
 *============================================================================*/
/** @defgroup Gap_Privacy_Exported_Types GAP Privacy Exported Types
  * @{
  */

/** @brief Define the privacy callback type */
#define GAP_MSG_LE_PRIVACY_RESOLUTION_STATUS_INFO       0x00 //!<Notification msg type for resolution status
#define GAP_MSG_LE_PRIVACY_SET_RESOLV_PRIV_ADDR_TIMEOUT 0x01 //!<response msg type for le_privacy_set_resolv_priv_addr_timeout
#define GAP_MSG_LE_PRIVACY_MODIFY_RESOLV_LIST           0x02 //!<response msg type for le_privacy_modify_resolv_list
#define GAP_MSG_LE_PRIVACY_READ_PEER_RESOLV_ADDR        0x03 //!<response msg type for le_privacy_read_peer_resolv_addr
#define GAP_MSG_LE_PRIVACY_READ_LOCAL_RESOLV_ADDR       0x04 //!<response msg type for le_privacy_read_local_resolv_addr
#define GAP_MSG_LE_PRIVACY_SET_MODE                     0x05 //!<response msg type for le_privacy_set_mode


/** @brief Define the subtype of Message IO_MSG_TYPE_BT_STATUS */
typedef enum
{
    GAP_PARAM_PRIVACY_TIMEOUT           = 0x300, //!< Timeout value for privacy
    GAP_PARAM_PRIVACY_RESOLUTION_STATE  = 0x301, //!< Resolution state
    GAP_PARAM_PRIVACY_RESOLV_LIST_SIZE  = 0x302, //!< Resolvable list size
} T_LE_PRIVACY_PARAM_TYPE;

/** @brief Define the privacy state */
typedef enum
{
    LE_PRIVACY_RESOLUTION_DISABLED,
    LE_PRIVACY_RESOLUTION_DISABLING,
    LE_PRIVACY_RESOLUTION_ENABLING,
    LE_PRIVACY_RESOLUTION_ENABLED
} T_LE_PRIVACY_STATE;

/** @brief LE resolution status info */
typedef struct
{
    T_LE_PRIVACY_STATE   status;
    uint16_t             cause;
} T_LE_PRIVACY_RESOLUTION_STATUS_INFO;

/** @brief  Response of le set resolvable private address timeout request.*/
typedef struct
{
    uint16_t             cause;
} T_LE_PRIVACY_SET_RESOLV_PRIV_ADDR_TIMEOUT_RSP;

/** @brief  Response of le modify resolvable list request.*/
typedef struct
{
    T_GAP_RESOLV_LIST_OP operation;
    uint16_t             cause;
} T_LE_PRIVACY_MODIFY_RESOLV_LIST_RSP;

/** @brief  Response of le read peer resolvable address request.*/
typedef struct
{
    uint16_t             cause;
    uint8_t              peer_resolv_addr[6];
} T_LE_PRIVACY_READ_PEER_RESOLV_ADDR_RSP;

/** @brief  Response of le read local resolvable address request.*/
typedef struct
{
    uint16_t             cause;
    uint8_t              local_resolv_addr[6];
} T_LE_PRIVACY_READ_LOCAL_RESOLV_ADDR_RSP;

/** @brief  Response of set privacy mode.*/
typedef struct
{
    uint16_t             cause;
} T_LE_PRIVACY_SET_MODE_RSP;

/** @brief LE privacy callback data  */
typedef union
{
    T_LE_PRIVACY_RESOLUTION_STATUS_INFO            le_privacy_resolution_status_info;
    T_LE_PRIVACY_SET_RESOLV_PRIV_ADDR_TIMEOUT_RSP *p_le_privacy_set_resolv_priv_addr_timeout_rsp;
    T_LE_PRIVACY_MODIFY_RESOLV_LIST_RSP           *p_le_privacy_modify_resolv_list_rsp;
    T_LE_PRIVACY_READ_PEER_RESOLV_ADDR_RSP        *p_le_privacy_read_peer_resolv_addr_rsp;
    T_LE_PRIVACY_READ_LOCAL_RESOLV_ADDR_RSP       *p_le_privacy_read_local_resolv_addr_rsp;
    T_LE_PRIVACY_SET_MODE_RSP                     *p_le_privacy_set_mode_rsp;
} T_LE_PRIVACY_CB_DATA;

/**
  * @brief Callback for gap to notify app
  * @param[in] msg_type Callback msy type
  * @param[in] msg_data Message data @ref T_LE_PRIVACY_CB_DATA.
  * @return result
  * @retval results @ref T_APP_RESULT
  */

typedef T_APP_RESULT(*P_FUN_PRIVACY_CB)(uint8_t msg_type, T_LE_PRIVACY_CB_DATA msg_data);

/** End of Gap_Privacy_Exported_Types
* @}
*/

/*============================================================================*
 *                         Functions
 *============================================================================*/
/** @defgroup Gap_Privacy_Exported_Functions GAP Privacy Exported Functions
  * @{
  */

/**
  * @brief  Set a LE privacy parameter.
  *
  *         NOTE: You can call this function with a LE Privacy Parameter type and it will set the
  *         LE Privacy Parameter.  LE Privacy Parameters are defined in @ref T_LE_PRIVACY_PARAM_TYPE.
  * @param[in]  param LE Privacy parameter type: @ref T_LE_PRIVACY_PARAM_TYPE.
  * @param[in]  len Length of data to write.
  * @param[in]  p_value Pointer to data to write.
  * @return Operation result.
  * @retval GAP_CAUSE_SUCCESS Operation success.
  * @retval Others Operation failure.
  */
T_GAP_CAUSE le_privacy_set_param(T_LE_PRIVACY_PARAM_TYPE param, uint8_t len, void *p_value);

/**
  * @brief  Get a LE privacy parameter.
  *
  *         NOTE: You can call this function with a LE Privacy Parameter type and it will get a
  *         LE Privacy Parameter.  LE Privacy Parameters are defined in @ref T_LE_PRIVACY_PARAM_TYPE.
  *
  * @param[in]  param GAP parameter type: @ref T_LE_PRIVACY_PARAM_TYPE
  * @param[in,out]  p_value Pointer to location to get the parameter value.  This is dependent on
  *                  the parameter ID and WILL be cast to the appropriate
  *                  data type (For example: if data type of param is uint16_t, p_value will be cast to
  *                      pointer of uint16_t).
  * @return Operation result.
  * @retval GAP_CAUSE_SUCCESS Operation success.
  * @retval Others Operation failure.
  */
T_GAP_CAUSE le_privacy_get_param(T_LE_PRIVACY_PARAM_TYPE param, void *p_value);

/**
  * @brief  Register privacy callback to stack.
  * @param[in] p_privacy_cb Callback function provided by the APP to handle le privacy messages sent from the BT Stack.
  *              @arg NULL -> Use gap defualt callback function.
  *              @arg Other -> Use application defined callback function.
  * @return none
  */
void        le_privacy_register_cb(P_FUN_PRIVACY_CB p_privacy_cb);

/**
  * @brief  Enable/disable le privacy address resolution mode.
  * @param[in] enable Enable or disable address resolution.
  * @return Operation result
  * @retval GAP_CAUSE_SUCCESS Operation success.
  * @retval Others Operation failure.
  */
T_GAP_CAUSE le_privacy_set_addr_resolution(bool enable);

/**
  * @brief  Read peer resolvable random address.
  * @param[in] peer_identity_address_type Peer identity address type.
  * @param[in] peer_identity_address Peer identity address.
  * @return Operation result
  * @retval GAP_CAUSE_SUCCESS Operation success.
  * @retval Others Operation failure.
  */
T_GAP_CAUSE le_privacy_read_peer_resolv_addr(T_GAP_IDENT_ADDR_TYPE peer_identity_address_type,
                                             uint8_t *peer_identity_address);

/**
  * @brief  Read local resolvable random address.
  * @param[in] peer_identity_address_type Peer identity address type.
  * @param[in] peer_identity_address Peer identity address.
  * @return Operation result
  * @retval GAP_CAUSE_SUCCESS Operation success.
  * @retval Others Operation failure.
  */
T_GAP_CAUSE le_privacy_read_local_resolv_addr(T_GAP_IDENT_ADDR_TYPE peer_identity_address_type,
                                              uint8_t *peer_identity_address);

/**
  * @brief  Set resolvable private address timeout.
  * @return Operation result
  * @retval GAP_CAUSE_SUCCESS Operation success.
  * @retval Others Operation failure.
  */
T_GAP_CAUSE le_privacy_set_resolv_priv_addr_timeout(void);


/**
  * @brief  Modicy local resolvable device list.
  *         NOTE: You can call this function to add, remove or clear the resolvable list
  *
  *               When the default value of @ref gap_config_local_addr_storage is true or
  *               @ref gap_config_local_addr_storage (true) has been configured and operation
  *               equals @ref GAP_RESOLV_LIST_OP_ADD, this function cannot be used if local
  *               device uses different local addresses to bond with the same remote device.
  *               In this situation, @ref le_privacy_add_resolv_list shall be called.
  *
  * @param[in] operation type, @ref T_GAP_RESOLV_LIST_OP.
  * @param[in] peer_identity_address_type Peer identity address type.
  * @param[in] peer_identity_address Peer identity address.
  * @return Operation result
  * @retval GAP_CAUSE_SUCCESS Operation success.
  * @retval Others Operation failure.
  */
T_GAP_CAUSE le_privacy_modify_resolv_list(T_GAP_RESOLV_LIST_OP operation,
                                          T_GAP_IDENT_ADDR_TYPE peer_identity_address_type,
                                          uint8_t *peer_identity_address);

/**
  * @brief  Add device to local resolving list.
  *
  * @param[in] peer_identity_address_type Peer identity address type.
  * @param[in] peer_identity_address Peer identity address.
  * @param[in] peer_irk              Pointer to peer IRK (16 octets).
                                     Note:
                                          NULL: no peer IRK, use all-zero IRK.
  * @param[in] use_local_irk         Indicates whether local IRK has been distributed to peer device.
                                     False: local IRK has not been distributed to peer device
                                     True: local IRK has been distributed to peer device.
  * @return Operation result.
  * @retval GAP_CAUSE_SUCCESS Operation success.
  * @retval Others Operation failure.
  */
T_GAP_CAUSE le_privacy_add_resolv_list(T_GAP_IDENT_ADDR_TYPE peer_identity_address_type,
                                       uint8_t *peer_identity_address, uint8_t *peer_irk, bool use_local_irk);

/**
  * @brief  Set privacy mode.
  * @param[in] peer_identity_address_type Peer identity address type.
  * @param[in] peer_identity_address Peer identity address.
  * @param[in] privacy_mode Privacy mode @ref T_GAP_PRIVACY_MODE.
  * @return Operation result
  * @retval GAP_CAUSE_SUCCESS Operation success.
  * @retval GAP_CAUSE_SEND_REQ_FAILED Operation failure.
  */
T_GAP_CAUSE le_privacy_set_mode(T_GAP_IDENT_ADDR_TYPE peer_identity_address_type,
                                uint8_t *peer_identity_address,
                                T_GAP_PRIVACY_MODE privacy_mode);

/**
  * @brief  Convert remote bluetooth device address type to identity address type.
  * @param[in] type Bluetooth device address type @ref T_GAP_REMOTE_ADDR_TYPE.
  * @return Identity address type @ref T_GAP_IDENT_ADDR_TYPE
  * @retval GAP_IDENT_ADDR_PUBLIC  Public identity address.
  * @retval GAP_IDENT_ADDR_RAND    Random identity address.
  */
T_GAP_IDENT_ADDR_TYPE le_privacy_convert_addr_type(T_GAP_REMOTE_ADDR_TYPE type);
#endif

/**
 * @brief Check if a given Resolvable Private Address (RPA) can be resolved using a specified Identity Resolving Key (IRK).
 *
 * This function determines if a provided Resolvable Private Address (RPA) associated with Bluetooth Low Energy (BLE) privacy
 * mechanisms can be resolved using the specified Identity Resolving Key (IRK). It achieves this by utilizing encryption and
 * comparison methods as defined in the Bluetooth specification for privacy address resolution.
 *
 * @param[in] rpa Pointer to a 6-byte Resolvable Private Address that needs to be resolved. This address is typically a random
 *                address generated for privacy protection during BLE operations.
 * @param[in] irk Pointer to a 16-byte Identity Resolving Key used for resolving the Resolvable Private Address.
 *
 * @return Check result.
 * @retval true The Resolvable Private Address matches the IRK and successfully resolves. This indicates that the provided RPA
 *              is associated with the given IRK and should be accepted as a legitimate identity.
 * @retval false The Resolvable Private Address does not match the IRK and fails to resolve. This means that the RPA is either
 *               not associated with the given IRK or could not be resolved, indicating it is not recognized as a valid identity
 *               with the provided key.
 */
bool le_privacy_check_resolvable_private_address(uint8_t *rpa, uint8_t *irk);

/** @} */ /* End of group Gap_Privacy_Exported_Functions */
/** @} */ /* End of group GAP_PRIVACY_MODULE */

#ifdef __cplusplus
}
#endif

#endif /* GAP_PRIVACY_H */
