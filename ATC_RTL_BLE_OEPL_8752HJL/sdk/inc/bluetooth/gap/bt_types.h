
#ifndef _BT_TYPES_H_
#define _BT_TYPES_H_

#ifdef __cplusplus
extern "C" {
#endif

/** @defgroup    BT_TYPES    Bluetooth Types
  * @brief   Defines Bluetooth related macros for the Bluetooth Host.
  *
  * @{
  */


/** @defgroup    BT_LE_LOCAL_SUPPORTED_FEATURES  Bluetooth LE Local Supported Features
  * @brief       Bluetooth LE Local Supported Features definitions.
  * @{
  */
#define LE_SUPPORT_FEATURES_MASK_ARRAY_INDEX0                           0

#define LE_SUPPORT_FEATURES_ENCRYPTION_MASK_BIT                         ((uint8_t)0x01)
#define LE_SUPPORT_FEATURES_CONNPARA_REQ_PROC_MASK_BIT                  ((uint8_t)0x02)
#define LE_SUPPORT_FEATURES_EXTENDED_REJECTION_IND_MASK_BIT             ((uint8_t)0x04)
#define LE_SUPPORT_FEATURES_SLAVE_INITIATED_FEATURE_EXCHANGE_MASK_BIT   ((uint8_t)0x08)
#define LE_SUPPORT_FEATURES_LE_PING_MASK_BIT                            ((uint8_t)0x10)
#define LE_SUPPORT_FEATURES_LE_DATA_LENGTH_EXTENSION_MASK_BIT           ((uint8_t)0x20)
#define LE_SUPPORT_FEATURES_LL_PRIVACY_MASK_BIT                         ((uint8_t)0x40)
#define LE_SUPPORT_FEATURES_EXTENDED_SCANNER_FILTER_POLICY_MASK_BIT     ((uint8_t)0x80)

#define LE_SUPPORT_FEATURES_MASK_ARRAY_INDEX1                           1

#define LE_SUPPORT_FEATURES_LE_2M_MASK_BIT                              ((uint8_t)0x01)
#define LE_SUPPORT_FEATURES_STABLE_MODULATION_INDEX_TX_MASK_BIT         ((uint8_t)0x02)
#define LE_SUPPORT_FEATURES_STABLE_MODULATION_INDEX_RX_MASK_BIT         ((uint8_t)0x04)
#define LE_SUPPORT_FEATURES_LE_CODED_PHY_MASK_BIT                       ((uint8_t)0x08)
#define LE_SUPPORT_FEATURES_LE_EXTENDED_ADV_BIT                         ((uint8_t)0x10)
#define LE_SUPPORT_FEATURES_LE_PERIODIC_ADV_MASK_BIT                    ((uint8_t)0x20)
#define LE_SUPPORT_FEATURES_CSA2_BIT                                    ((uint8_t)0x40)
#define LE_SUPPORT_FEATURES_LE_POWER_CLASS1_MASK_BIT                    ((uint8_t)0x80)

#define LE_SUPPORT_FEATURES_MASK_ARRAY_INDEX2                           2

#define LE_SUPPORT_FEATURES_MIN_NUM_USED_CHANNEL_MASK_BIT               ((uint8_t)0x01)
#define LE_SUPPORT_FEATURES_CONNECTION_CTE_REQUEST_MASK_BIT             ((uint8_t)0x02)
#define LE_SUPPORT_FEATURES_CONNECTION_CTE_RESPONSE_MASK_BIT            ((uint8_t)0x04)
#define LE_SUPPORT_FEATURES_CONNECTIONLESS_CTE_TRANSMITTER_MASK_BIT     ((uint8_t)0x08)
#define LE_SUPPORT_FEATURES_CONNECTIONLESS_CTE_RECEIVER_MASK_BIT        ((uint8_t)0x10)
#define LE_SUPPORT_FEATURES_AOD_MASK_BIT                                ((uint8_t)0x20)
#define LE_SUPPORT_FEATURES_AOA_MASK_BIT                                ((uint8_t)0x40)
#define LE_SUPPORT_FEATURES_RECEIVING_CTE_MASK_BIT                      ((uint8_t)0x80)

#define LE_SUPPORT_FEATURES_MASK_ARRAY_INDEX3                           3

#define LE_SUPPORT_FEATURES_PAST_SENDER_MASK_BIT                        ((uint8_t)0x01)
#define LE_SUPPORT_FEATURES_PAST_RECIPIENT_MASK_BIT                     ((uint8_t)0x02)
#define LE_SUPPORT_FEATURES_SLEEP_CLOCK_ACCURACY_UPDATES_MASK_BIT       ((uint8_t)0x04)
#define LE_SUPPORT_FEATURES_REMOTE_PUBLIC_KEY_VALIDATION_MASK_BIT       ((uint8_t)0x08)
#define LE_SUPPORT_FEATURES_CIS_MASTER_MASK_BIT                         ((uint8_t)0x10)
#define LE_SUPPORT_FEATURES_CIS_SLAVE_MASK_BIT                          ((uint8_t)0x20)
#define LE_SUPPORT_FEATURES_ISOCHRONOUS_BROADCASTER_MASK_BIT            ((uint8_t)0x40)
#define LE_SUPPORT_FEATURES_SYNCHRONIZED_RECEIVER_MASK_BIT              ((uint8_t)0x80)

#define LE_SUPPORT_FEATURES_MASK_ARRAY_INDEX4                           4

#define LE_SUPPORT_FEATURES_ISOCH_HOST_SUPPORT_MASK_BIT                 ((uint8_t)0x01)
#define LE_SUPPORT_FEATURES_LE_POWER_CONTROL_REQUEST_MASK_BIT           ((uint8_t)0x02)
#define LE_SUPPORT_FEATURES_LE_POWER_CHANGE_INDICATION_MASK_BIT         ((uint8_t)0x04)
#define LE_SUPPORT_FEATURES_LE_PATH_LOSS_MONITORING_MASK_BIT            ((uint8_t)0x08)
/** End of BT_LE_LOCAL_SUPPORTED_FEATURES
 * @}
 */

/** @defgroup    BT_LE_SET_HOST_FEATURE      Bluetooth LE Set Host Feature
  * @brief       Bluetooth LE Set Host Feature definitions.
  * @{
  */
#define LE_SET_HOST_FEATURE_BIT_VALUE_DISABLE                            ((uint8_t)0x00)
#define LE_SET_HOST_FEATURE_BIT_VALUE_ENABLE                             ((uint8_t)0x01)
#define LE_SET_HOST_FEATURE_BIT_VALUE_RFU                                ((uint8_t)0xFF)

#define LE_SET_HOST_FEATURE_BIT_NUMBER_ISOCH_HOST_SUPPORT                ((uint8_t)0x20)
#define LE_SET_HOST_FEATURE_BIT_NUMBER_RFU                               ((uint8_t)0xFF)
/** End of BT_LE_SET_HOST_FEATURE
 * @}
 */

/** @defgroup    BT_HOST_MODULE_ERROR   Bluetooth Host Module Error Code
  * @brief       Bluetooth Host module error code mask.
  * @{
  */
#define GAP_ERR                 0x0000
#define HCI_ERR                 0x0100
#define L2C_ERR                 0x0200
#define SDP_ERR                 0x0300
#define ATT_ERR                 0x0400
#define SM_ERR                  0x0500

#define GATT_ERR                0x0C00
#define APP_ERR                 0x0D00
/** End of BT_HOST_MODULE_ERROR
 * @}
 */

/** @defgroup    BT_HCI_ERROR    Bluetooth HCI Error Code
  * @brief       Bluetooth HCI error code definitions.
  * @{
  */
#define HCI_SUCCESS                             0x00        /**< Success. */
#define HCI_ERR_UNKNOWN_CMD                     0x01        /**< The Controller does not understand the HCI Command packet opcode that the Host sent. */
#define HCI_ERR_UNKNOWN_CONN_ID                 0x02        /**< Command sent from the Host should identify a connection, but that connection does not exist or does not identify the correct type of connection. */
#define HCI_ERR_HARDWARE_FAIL                   0x03        /**< Something in the Controller has failed in a manner that cannot be described with any other error code. */
#define HCI_ERR_PAGE_TIMEOUT                    0x04        /**< A page timed out because of the Page Timeout configuration parameter. This error code may occur only with the HCI_Remote_Name_Request and HCI_Create_Connection commands. */
#define HCI_ERR_AUTHEN_FAIL                     0x05        /**< Pairing or authentication failed due to incorrect results in the pairing or authentication procedure. This could be due to an incorrect PIN or Link Key. */
#define HCI_ERR_KEY_MISSING                     0x06        /**< Pairing failed because of a missing PIN, or authentication failed because of a missing Key. */
#define HCI_ERR_MEMORY_FULL                     0x07        /**< The Controller has run out of memory to store new parameters. */
#define HCI_ERR_CONN_TIMEOUT                    0x08        /**< Either the link supervision timeout has expired for a given connection or that the synchronization timeout has expired for a given broadcast. */
#define HCI_ERR_MAX_NUM_CONN                    0x09        /**< An attempt to create another connection failed because the Controller is already at its limit of the number of connections it can support. */
#define HCI_ERR_MAX_NUM_SCO_CONN                0x0A        /**< The Controller has reached the limit to the number of synchronous connections that can be achieved to a device. */
#define HCI_ERR_ACL_CONN_EXIST                  0x0B        /**< An attempt was made to create a new Connection to a device when there is already a connection to this device and multiple connections to the same device are not permitted. */
#define HCI_ERR_CMD_DISALLOWED                  0x0C        /**< The command requested cannot be executed because the Controller is in a state where it cannot process this command at this time. This error shall not be used for command opcodes where the error code Unknown HCI Command is valid. */
#define HCI_ERR_REJECT_LIMITED_RESOURCE         0x0D        /**< A connection was rejected due to limited resources. */
#define HCI_ERR_REJECT_SECURITY_REASON          0x0E        /**< A connection was rejected due to security requirements not being fulfilled, like authentication or pairing. */
#define HCI_ERR_REJECT_UNACCEPTABLE_ADDR        0x0F        /**< A connection was rejected because this device does not accept the BD_ADDR. This may be because the device will only accept connections from specific BD_ADDRs. */
#define HCI_ERR_HOST_TIMEOUT                    0x10        /**< The Connection Accept Timeout has been exceeded for this connection attempt. */
#define HCI_ERR_UNSUPPORTED_PARAM               0x11        /**< A feature or parameter value in the HCI command is not supported. This error code shall not be used in an LMP PDU. */
#define HCI_ERR_INVALID_PARAM                   0x12        /**< At least one of the HCI command parameters is invalid. This shall be used when:
* @arg the parameter total length is invalid.
* @arg a command parameter is an invalid type.
* @arg a connection identifier does not match the corresponding event.
* @arg a parameter is odd when it is required to be even.
* @arg a parameter is outside of the specified range.
* @arg two or more parameter values have inconsistent values. */
#define HCI_ERR_REMOTE_USER_TERMINATE           0x13        /**< The user on the remote device either terminated the connection or stopped broadcasting packets. */
#define HCI_ERR_REMOTE_LOW_RESOURCE             0x14        /**< The remote device terminated the connection because of low resources. */
#define HCI_ERR_REMOTE_POWER_OFF                0x15        /**< The remote device terminated the connection because the device is about to power off. */
#define HCI_ERR_LOCAL_HOST_TERMINATE            0x16        /**< Either the local device terminated the connection, terminated synchronization with a broadcaster, or terminated broadcasting packets. */
#define HCI_ERR_REPEATED_ATTEMPTS               0x17        /**< The Controller is disallowing an authentication or pairing procedure because too little time has elapsed since the last authentication or pairing attempt failed. */
#define HCI_ERR_PARING_NOT_ALLOWED              0x18        /**< The device does not allow pairing. */
#define HCI_ERR_UNKNOWN_LMP_PDU                 0x19        /**< The Controller has received an unknown LMP opcode. */
#define HCI_ERR_UNSUPPORTED_REMOTE_FEAT         0x1A        /**< The remote device does not support the feature associated with the issued command, LMP PDU, or Link Layer Control PDU. */
#define HCI_ERR_SCO_OFFSET_REJECTED             0x1B        /**< The offset requested in the
* LMP_SCO_LINK_REQ PDU has been rejected. */
#define HCI_ERR_SCO_INTERVAL_REJECTED           0x1C        /**< The interval requested in the LMP_SCO_LINK_REQ PDU has been rejected. */
#define HCI_ERR_SCO_AIR_MODE_REJECTED           0x1D        /**< The air mode requested in the LMP_SCO_LINK_REQ PDU has been rejected. */
#define HCI_ERR_INVALID_LMP_PARAM               0x1E        /**< Some LMP PDU or LL Control PDU parameters were invalid. This shall be used when:
* @arg the PDU length is invalid.
* @arg a parameter is odd when it is required to be even.
* @arg a parameter is outside of the specified range.
* @arg two or more parameters have inconsistent values. */
#define HCI_ERR_UNSPECIFIED_ERROR               0x1F        /**< No other error code specified is appropriate to use. */
#define HCI_ERR_UNSUPPORTED_LMP_PARAM           0x20        /**< An LMP PDU or an LL Control PDU contains at least one parameter value that is not supported by the Controller at this time. This is normally used after a long negotiation procedure, for example during an
* LMP_HOLD_REQ, LMP_SNIFF_REQ and LMP_ENCRYPTION_KEY_SIZE_REQ PDU exchanges. This may be used by the Link Layer, for example during the Connection Parameters Request Link Layer Control procedure. */
#define HCI_ERR_ROLE_CHANGE_NOT_ALLOWED         0x21        /**< A Controller will not allow a role change at this time. */
#define HCI_ERR_LMP_RESPONSE_TIMEOUT            0x22        /**< An LMP transaction failed to respond within the LMP response timeout or an LL transaction failed to respond within the LL response timeout. */
#define HCI_ERR_LMP_ERROR_TRANS_COLLISION       0x23        /**< An LMP transaction or LL procedure has collided with the same transaction or procedure that is already in progress. */
#define HCI_ERR_LMP_PDU_NOT_ALLOWED             0x24        /**< A Controller sent an LMP PDU with an opcode that was not allowed. */
#define HCI_ERR_ENCRYPT_MODE_NOT_ACCEPTABLE     0x25        /**< The requested encryption mode is not acceptable at this time. */
#define HCI_ERR_UNIT_KEY_USED                   0x26        /**< A link key cannot be changed because a fixed unit key is being used. */
#define HCI_ERR_QOS_NOT_SUPPORTED               0x27        /**< The requested Quality of Service is not supported. */
#define HCI_ERR_INSTANT_PASSED                  0x28        /**< An LMP PDU or LL PDU that includes an instant cannot be performed because the instant when this would have occurred has passed. */
#define HCI_ERR_PAIR_UNIT_KEY_NOT_SUPPORT       0x29        /**< It was not possible to pair as a unit key was requested and it is not supported. */
#define HCI_ERR_DIFF_TRANS_COLLISION            0x2A        /**< An LMP transaction or LL Procedure was started that collides with an ongoing transaction. */
#define HCI_ERR_QOS_UNACCEPTABLE_PARAM          0x2C        /**< The specified quality of service parameters could not be accepted at this time, but other parameters may be acceptable. */
#define HCI_ERR_QOS_REJECT                      0x2D        /**< The specified quality of service parameters cannot be accepted and QoS negotiation should be terminated. */
#define HCI_ERR_CHANN_ASSESS_NOT_SUPPORT        0x2E        /**< The Controller cannot perform channel assessment because it is not supported. */
#define HCI_ERR_INSUFFICIENT_SECURITY           0x2F        /**< The HCI command or LMP PDU sent is only possible on an encrypted link. */
#define HCI_ERR_PARAM_OUT_OF_RANGE              0x30        /**< A parameter value requested is outside the mandatory range of parameters for the given HCI command or LMP PDU and the recipient does not accept that value. */
#define HCI_ERR_ROLE_SWITCH_PENDING             0x32        /**< A Role Switch is pending. This can be used when an HCI command or LMP PDU cannot be accepted because of a pending role switch. This can also be used to notify a peer device about a pending role switch. */
#define HCI_ERR_RESERVED_SLOT_VIOLATION         0x34        /**< The current Synchronous negotiation was terminated with the negotiation state set to Reserved Slot Violation. */
#define HCI_ERR_ROLE_SWITCH_FAILED              0x35        /**< A role switch was attempted but it failed and the original piconet structure is restored. The switch may have failed because the TDD switch or piconet switch failed. */
#define HCI_ERR_EXT_INQUIRY_RSP_TOO_LARGE       0x36        /**< The extended inquiry response, with the requested requirements for FEC, is too large to fit in any of the packet types supported by the Controller. */
#define HCI_ERR_SSP_NOT_SUPPORTED_BY_HOST       0x37        /**< The IO capabilities request or response was rejected because the sending Host does not support Secure Simple Pairing even though the receiving Link Manager does. */
#define HCI_ERR_HOST_BUSY_PAIRING               0x38        /**< The Host is busy with another pairing operation and unable to support the requested pairing. The receiving device should retry pairing again later. */
#define HCI_ERR_REJECT_NO_SUITABLE_CHANN        0x39        /**< The Controller could not calculate an appropriate value for the Channel selection operation. */
#define HCI_ERR_CONTROLLER_BUSY                 0x3A        /**< The operation was rejected because the Controller was busy and unable to process the request. */
#define HCI_ERR_UNACCEPTABLE_CONN_INTERVAL      0x3B        /**< The remote device either terminated the connection or rejected a request because of one or more unacceptable connection parameters. */
#define HCI_ERR_DIRECTED_ADV_TIMEOUT            0x3C        /**< Advertising for a fixed duration completed or, for directed advertising, that advertising completed without a connection being created. */
#define HCI_ERR_MIC_FAILURE                     0x3D        /**< Either the connection or the synchronization was terminated because the Message Integrity Check (MIC) failed on a received packet. */
#define HCI_ERR_FAIL_TO_ESTABLISH_CONN          0x3E        /**< The LL initiated a connection or initiated synchronization to periodic advertising but the connection has failed to be established or the Link Layer failed to synchronize with the periodic advertising within 6 periodic advertising events of the first attempt. */
#define HCI_ERR_COARSE_CLOCK_ADJUST_REJECTED    0x40        /**< The Central, at this time, is unable to make a coarse adjustment to the piconet clock, using the supplied parameters. Instead the Central will attempt to move the clock using clock dragging. */
#define HCI_ERR_TYPE0_SUBMAP_NOT_DEFINED        0x41        /**< The LMP PDU is rejected because the Type 0 submap is not currently defined. */
#define HCI_ERR_UNKNOWN_ADVERTISING_IDENTIFIER  0x42        /**< A command was sent from the Host that should identify an Advertising or Sync handle, but the Advertising or Sync handle does not exist. */
#define HCI_ERR_LIMIT_REACHED                   0x43        /**< Number of operations requested has been reached and has indicated the completion of the activity (e.g., advertising or scanning). */
#define HCI_ERR_OPERATION_CANCELLED_BY_HOST     0x44        /**< A request to the Controller issued by the Host and still pending was successfully canceled. */
#define HCI_ERR_OPERATION_PACKET_TOO_LONG       0x45        /**< An attempt was made to send or receive a packet that exceeds the maximum allowed packet length. */

/* HCI local error code */
#define HCI_ERR_TIMEOUT                         0x65        /**< HCI if error timeout. */
#define HCI_ERR_OUT_OF_SYNC                     0x66        /**< HCI if error out of sync. */
#define HCI_ERR_NO_DESCRIPTOR                   0x67        /**< HCI allocates no descriptor of link when handle HCI_LE_Connection_Complete event. */
#define HCI_ERR_NO_MEMORY                       0x68        /**< HCI has run out of memory. */
/** End of BT_HCI_ERROR
 * @}
 */

/** @defgroup    BT_L2CAP_ERROR      Bluetooth L2CAP Error Code
  * @brief       Bluetooth L2CAP error code definitions.
  * @{
  */
#define L2C_SUCCESS                             0x00

#define L2C_CONN_ACCEPT                         0x0000
#define L2C_CONN_RSP_PENDING                    0x0001
#define L2C_CONN_RSP_INVALID_PSM                0x0002
#define L2C_CONN_RSP_SECURITY_BLOCK             0x0003
#define L2C_CONN_RSP_NO_RESOURCE                0x0004
#define L2C_CONN_RSP_INVALID_PARAM              0x0005
#define L2C_CONN_RSP_INVALID_SOURCE_CID         0x0006
#define L2C_CONN_RSP_SOURCE_CID_ALLOCATED       0x0007

/* request cmd reject reasons */
#define L2C_CMD_REJ_NOT_UNDERSTOOD              0x0000
#define L2C_CMD_REJ_SIGNAL_MTU_EXCEEDED         0x0001
#define L2C_CMD_REJ_INVALID_CID_IN_REQ          0x0002

/* config response result codes */
#define L2C_CFG_RSP_SUCCESS                     0x0000
#define L2C_CFG_RSP_UNACCEPTABLE_PARAMS         0x0001
#define L2C_CFG_RSP_REJECTED                    0x0002
#define L2C_CFG_RSP_UNKNOWN_OPTIONS             0x0003
#define L2C_CFG_RSP_PENDING                     0x0004
#define L2C_CFG_RSP_FLOW_SPEC_REJECTED          0x0005

/* LE credit based connection response result codes */
#define L2C_LE_CREDIT_CONN_SUCCESS                      0x0000
#define L2C_LE_CREDIT_RSP_PSM_NOT_SUPPORTED             0x0002
#define L2C_LE_CREDIT_RSP_NO_RESOURCES                  0x0004
#define L2C_LE_CREDIT_RSP_INSUFFICIENT_AUTHEN           0x0005
#define L2C_LE_CREDIT_RSP_INSUFFICIENT_AUTHOR           0x0006
#define L2C_LE_CREDIT_RSP_INSUFFICIENT_ENCRYP_KEY_SIZE  0x0007
#define L2C_LE_CREDIT_RSP_INSUFFICIENT_ENCRYP           0x0008
#define L2C_LE_CREDIT_RSP_INVALID_SOURCE_CID            0x0009
#define L2C_LE_CREDIT_RSP_SOURCE_CID_ALREADY_ALLOC      0x000A

/* status for connection update response */
#define L2C_LE_CONN_UPDATE_ACCEPT               0x0000
#define L2C_LE_CONN_UPDATE_RSP_REJECT           0x0001
#define L2C_LE_CONN_UPDATE_RSP_TOUT             0x0002

/* L2CAP error code */
#define L2C_ERR_REJ_BASE                        0x0010
#define L2C_ERR_CFG_BASE                        0x0020
#define L2C_ERR_LE_CREDIT_BASE                  0x0030
#define L2C_ERR_LE_CONN_UPDATE_BASE             0x0040
#define L2C_ERR_VND_BASE                        0x00F0

#define L2C_ERR_PENDING                         L2C_CONN_RSP_PENDING
#define L2C_ERR_INVALID_PSM                     L2C_CONN_RSP_INVALID_PSM
#define L2C_ERR_SECURITY_BLOCK                  L2C_CONN_RSP_SECURITY_BLOCK
#define L2C_ERR_NO_RESOURCE                     L2C_CONN_RSP_NO_RESOURCE
#define L2C_ERR_INVALID_PARAM                   L2C_CONN_RSP_INVALID_PARAM
#define L2C_ERR_INVALID_SOURCE_CID              L2C_CONN_RSP_INVALID_SOURCE_CID
#define L2C_ERR_SOURCE_CID_ALLOCATED            L2C_CONN_RSP_SOURCE_CID_ALLOCATED

#define L2C_ERR_CMD_NOT_UNDERSTOOD              (L2C_ERR_REJ_BASE + L2C_CMD_REJ_NOT_UNDERSTOOD)
#define L2C_ERR_SIGNAL_MTU_EXCEEDED             (L2C_ERR_REJ_BASE + L2C_CMD_REJ_SIGNAL_MTU_EXCEEDED)
#define L2C_ERR_INVALID_CID_IN_REQ              (L2C_ERR_REJ_BASE + L2C_CMD_REJ_INVALID_CID_IN_REQ)

#define L2C_ERR_CFG_UNACCEPTABLE_PARAM          (L2C_ERR_CFG_BASE + L2C_CFG_RSP_UNACCEPTABLE_PARAMS)
#define L2C_ERR_CFG_REJECTED                    (L2C_ERR_CFG_BASE + L2C_CFG_RSP_REJECTED)
#define L2C_ERR_CFG_UNKNOWN_OPTIONS             (L2C_ERR_CFG_BASE + L2C_CFG_RSP_UNKNOWN_OPTIONS)
#define L2C_ERR_CFG_PENDING                     (L2C_ERR_CFG_BASE + L2C_CFG_RSP_PENDING)
#define L2C_ERR_CFG_FLOW_SPEC_REJECTED          (L2C_ERR_CFG_BASE + L2C_CFG_RSP_FLOW_SPEC_REJECTED)

#define L2C_ERR_LE_CREDIT_PSM_NOT_SUPPORTED             (L2C_ERR_LE_CREDIT_BASE + L2C_LE_CREDIT_RSP_PSM_NOT_SUPPORTED)
#define L2C_ERR_LE_CREDIT_NO_RESOURCES                  (L2C_ERR_LE_CREDIT_BASE + L2C_LE_CREDIT_RSP_NO_RESOURCES)
#define L2C_ERR_LE_CREDIT_INSUFFICIENT_AUTHEN           (L2C_ERR_LE_CREDIT_BASE + L2C_LE_CREDIT_RSP_INSUFFICIENT_AUTHEN)
#define L2C_ERR_LE_CREDIT_INSUFFICIENT_AUTHOR           (L2C_ERR_LE_CREDIT_BASE + L2C_LE_CREDIT_RSP_INSUFFICIENT_AUTHOR)
#define L2C_ERR_LE_CREDIT_INSUFFICIENT_ENCRYP_KEY_SIZE  (L2C_ERR_LE_CREDIT_BASE + L2C_LE_CREDIT_RSP_INSUFFICIENT_ENCRYP_KEY_SIZE)
#define L2C_ERR_LE_CREDIT_INSUFFICIENT_ENCRYP           (L2C_ERR_LE_CREDIT_BASE + L2C_LE_CREDIT_RSP_INSUFFICIENT_ENCRYP)
#define L2C_ERR_LE_CREDIT_INVALID_SOURCE_CID            (L2C_ERR_LE_CREDIT_BASE + L2C_LE_CREDIT_RSP_INVALID_SOURCE_CID)
#define L2C_ERR_LE_CREDIT_SOURCE_CID_ALREADY_ALLOC      (L2C_ERR_LE_CREDIT_BASE + L2C_LE_CREDIT_RSP_SOURCE_CID_ALREADY_ALLOC)

#define L2C_ERR_LE_CONN_PARAM_UPDATE_REJECT     (L2C_ERR_LE_CONN_UPDATE_BASE + L2C_LE_CONN_UPDATE_RSP_REJECT)
#define L2C_ERR_LE_CONN_PARAM_UPDATE_TOUT       (L2C_ERR_LE_CONN_UPDATE_BASE + L2C_LE_CONN_UPDATE_RSP_TOUT)

#define L2C_ERR_VND_CREDITS_EXCEED              (L2C_ERR_VND_BASE + 0x00)
#define L2C_ERR_VND_INVAILD_PDU                 (L2C_ERR_VND_BASE + 0x01)
#define L2C_ERR_VND_CREDITS_LACK                (L2C_ERR_VND_BASE + 0x02)
#define L2C_ERR_VND_NO_MEMORY                   (L2C_ERR_VND_BASE + 0x03)
#define L2C_ERR_VND_INVALID_STATE               (L2C_ERR_VND_BASE + 0x04)
#define L2C_ERR_VND_INVALID_RX_SEQ              (L2C_ERR_VND_BASE + 0x05)
#define L2C_ERR_VND_TIMEOUT                     (L2C_ERR_VND_BASE + 0x06)
#define L2C_ERR_VND_INVALID_MODE                (L2C_ERR_VND_BASE + 0x07)
#define L2C_ERR_VND_REMOTE_DISCONNECT           (L2C_ERR_VND_BASE + 0x08)
/** End of BT_L2CAP_ERROR
 * @}
 */


/** @defgroup    BT_ATT_ERROR    Bluetooth ATT Error Code
  * @brief       Bluetooth ATT error code definitions.
  * @{
  */
#define ATT_SUCCESS                         0    /**< Success. */
#define ATT_ERR_INVALID_HANDLE              0x01 /**< Attribute handle value given was not valid on this attribute server. */
#define ATT_ERR_READ_NOT_PERMITTED          0x02 /**< Attribute cannot be read. */
#define ATT_ERR_WRITE_NOT_PERMITTED         0x03 /**< Attribute cannot be written. */
#define ATT_ERR_INVALID_PDU                 0x04 /**< The attribute PDU was invalid. */
#define ATT_ERR_INSUFFICIENT_AUTHEN         0x05 /**< The attribute requires authentication before it can be read or written. */
#define ATT_ERR_UNSUPPORTED_REQ             0x06 /**< Attribute server doesn't support the request received from the attribute client. */
#define ATT_ERR_INVALID_OFFSET              0x07 /**< Offset specified was past the end of the attribute. */
#define ATT_ERR_INSUFFICIENT_AUTHOR         0x08 /**< The attribute requires an authorization before it can be read or written. */
#define ATT_ERR_PREP_QUEUE_FULL             0x09 /**< Too many prepare writes have been queued. */
#define ATT_ERR_ATTR_NOT_FOUND              0x0A /**< No attribute found within the given attribute handle range. */
#define ATT_ERR_ATTR_NOT_LONG               0x0B /**< Attribute cannot be read or written using the Read Blob Request or Prepare Write Request. */
#define ATT_ERR_INSUFFICIENT_KEY_SIZE       0x0C /**< The Encryption Key Size used for encrypting this link is insufficient. */
#define ATT_ERR_INVALID_VALUE_SIZE          0x0D /**< The attribute value length is invalid for the operation. */
#define ATT_ERR_UNLIKELY                    0x0E /**< The attribute request that was requested has encountered an error that was very unlikely, and therefore could not be completed as requested. */
#define ATT_ERR_INSUFFICIENT_ENCRYPT        0x0F /**< The attribute requires encryption before it can be read or written. */
#define ATT_ERR_UNSUPPORTED_GROUP_TYPE      0x10 /**< The attribute type is not a supported grouping attribute as defined by a higher layer specification. */
#define ATT_ERR_INSUFFICIENT_RESOURCES      0x11 /**< Insufficient Resources to complete the request. */
#define ATT_ERR_DATABASE_OUT_OF_SYNC        0x12 /**< The server requests the client to rediscover the database. */
#define ATT_ERR_VALUE_NOT_ALLOWED           0x13 /**< The attribute parameter value was not allowed. */

/* Application Error 0x80 - 0x9F */
#define ATT_ERR_INVALID_VALUE               0x80 /**< The attribute value is invalid for the operation. */

/* Manufacturer specific error codes that are "missing" in GATT spec. >= 0xC0:   */
#define ATT_ERR_INVALID_CCC_BITS            0xC0 /**< Invalid client characteristic config bits. */
#define ATT_ERR_INVALID_SIGNED_COUNT        0xC1 /**< Invalid sign count. */
#define ATT_ERR_INVALID_SIGNED_MAC_FAILED   0xC2 /**< Invalid sign MAC value. */
#define ATT_ERR_MIN_APPLIC_CODE             0xC3 /**< ATT error occurs due to application code. */

/* Common profile and service error codes >= 0xE0 (CSS)*/
#define ATT_ERR_WRITE_REQUEST_REJECTED      0xFC /**< Write Request Rejected. */
#define ATT_ERR_CCCD_IMPROPERLY_CONFIGURED  0xFD /**< CCCD improperly configured. */
#define ATT_ERR_PROC_ALREADY_IN_PROGRESS    0xFE /**< Procedure Already in Progress. */
#define ATT_ERR_OUT_OF_RANGE                0xFF /**< An attribute value is out of range as defined by a profile or service specification. */
/** End of BT_ATT_ERROR
 * @}
 */


/** @defgroup    BT_GATT_ERROR   Bluetooth GATT Error Code
  * @brief       Bluetooth GATT error code definitions.
  * @{
  */
#define GATT_SUCCESS                        0x00
#define GATT_ERR_OUT_OF_RESOURCE            0x01
#define GATT_ERR_UNSUPPORTED                0x02
#define GATT_ERR_INVALID_ROLE               0x03
#define GATT_ERR_INVALID_STATE              0x04
#define GATT_ERR_INVALID_CODING             0x05
#define GATT_ERR_INVALID_HANDLE             0x06
#define GATT_ERR_INVALID_PARAM              0x07
#define GATT_ERR_INTERNAL                   0x08
#define GATT_ERR_NOT_ALLOWED                0x09
#define GATT_ERR_NOTIF_IND_NOT_CFG          0x0A
#define GATT_ERR_NOTIF_IND_CFG              0x0B
#define GATT_ERR_NOTIF_IND_CONF_PD          0x0C
#define GATT_ERR_TIMEOUT                    0x0D
#define GATT_ERR_LINK_DEACTIVATED           0x0E
#define GATT_ERR_NOT_AUTHENTICATED          0x0F
#define GATT_ERR_NOT_ENCRYPTED              0x10
#define GATT_ERR_PROC_FAIL                  0x11
/** End of BT_GATT_ERROR
 * @}
 */

/** @defgroup    BT_SM_ERROR     Bluetooth SM Error Code
  * @brief       Bluetooth SM error code definitions.
  * @{
  */
/* SM error code */
#define SM_SUCCESS                          0x00        /**< Success. */
/* SMP_OP_PAIRING_FAILED causes */
#define SMP_ERR_SUCCESS                         0x00    /**< Success. */
#define SMP_ERR_PASSKEY_ENTRY_FAIL              0x01    /**< The user input of passkey failed, for example, the user cancelled the operation. */
#define SMP_ERR_OOB_NOT_AVAIABLE                0x02    /**< The OOB data is not available. */
#define SMP_ERR_AUTHEN_REQUIREMENTS             0x03    /**< The pairing procedure cannot be performed as authentication requirements cannot be met due to IO capabilities of one or both devices. */
#define SMP_ERR_CFM_VALUE_FAILED                0x04    /**< The confirm value does not match the calculated compare value. */
#define SMP_ERR_PAIRING_NOT_SUPPORTED           0x05    /**< Pairing is not supported by the device. */
#define SMP_ERR_ENCRYPTION_KEY_SIZE             0x06    /**< The resultant encryption key size is not long enough for the security requirements of this device. */
#define SMP_ERR_CMD_NOT_SUPPORTED               0x07    /**< The SMP command received is not supported on this device. */
#define SMP_ERR_UNSPECIFIED_REASON              0x08    /**< Pairing failed due to an unspecified reason. */
#define SMP_ERR_REPEATED_ATTEMPTS               0x09    /**< Pairing or authentication procedure is disallowed because too little time has elapsed since last pairing request or security request. */
#define SMP_ERR_INVALID_PARAM                   0x0A    /**< The Invalid Parameters error code indicates that the command length is invalid or that a parameter is outside of the specified range. */
#define SMP_ERR_DHKEY_CHECK_FAILED              0x0B    /**< Indicates to the remote device that the DHKey Check value received doesn't match the one calculated by the local device. */
#define SMP_ERR_NUMERIC_COMPARISION_FAIL        0x0C    /**< Indicates that the confirm values in the numeric comparison protocol do not match. */
#define SMP_ERR_BREDR_PAIRING_IN_PROGRESS       0x0D    /**< Indicates that the pairing over the LE transport failed due to a Pairing Request sent over the BR/EDR transport in progress. */
#define SMP_ERR_CRS_TRANS_KEY_GEN_NOT_ALLOW     0x0E    /**< Indicates that the BR/EDR Link Key generated on the BR/EDR transport cannot be used to derive and distribute keys for the LE transport or the LE LTK generated on the LE transport cannot be used to derive a key for the BR/EDR transport. */

#define SM_ERR_NO_ENTRY                         0xE1    /**< Host find no corresponding entry in the security database. */
#define SM_ERR_DB_FULL                          0xE2    /**< The security database is full. */
#define SM_ERR_INVALID_PARAM                    0xE3    /**< At least one of the SM parameters is invalid. This shall be used when:
* @arg LE Secure Simple Pairing parameter to set is invalid.
* @arg found key entry is invalid.
* @arg a connection identifier does not match the corresponding event. */
#define SM_ERR_INVALID_STATE                    0xE6    /**< Invalid link state. */
#define SM_ERR_NO_RESOURCE                      0xE7    /**< The security database has run out of resources. */
#define SM_ERR_LINK_KEY_MISSING                 0xE8    /**< Host find no corresponding Link Key in the security database. */
#define SM_ERR_DISCONNECT                       0xE9    /**< Pairing failed due to disconnection during the procedure. */
#define SM_ERR_PARING_NOT_ALLOWED               0xEA    /**< Pairing is not allowed when device is the Central. */
#define SM_ERR_KEY_SAVE_FAILED                  0xEB    /**< Failed to save key to the security database. */
#define SM_ERR_TIMEOUT                          0xEC    /**< If the Security Manager Timer reaches 30 seconds, the procedure shall be considered to have failed. */
#define SM_ERR_UNKNOWN                          0xED    /**< No other error code is appropriate to use. */
#define SM_ERR_LINK_DEACTIVATED                 0xEE    /**< Pairing failed because the link is deactivated. */
#define SM_ERR_NO_PAIRABLE_MODE                 0xEF    /**< Pairing is not allowed when device is the Peripheral. */
/** End of BT_SM_ERROR
 * @}
 */

/** @defgroup    BT_BTIF_ERROR   Bluetooth BTIF Error Code
  * @brief       Bluetooth BTIF error code definitions.
  * @{
  */
#define BTIF_ERR                0x0000
#define BTIF_SUCCESS                        0x00
#define BTIF_ACCEPT                         0x01
#define BTIF_ERR_REJECT                     0x02
#define BTIF_ERR_NO_RESOURCE                0x03
#define BTIF_ERR_INVALID_PARAM              0x04
#define BTIF_ERR_INVALID_STATE              0x05
#define BTIF_ERR_CONN_DISCONNECT            0x06
#define BTIF_ERR_CONN_LOST                  0x07
#define BTIF_ERR_AUTHEN_FAIL                0x08
#define BTIF_ERR_INIT_TOUT                  0x09
#define BTIF_ERR_INIT_OUT_OF_SYNC           0x0A
#define BTIF_ERR_INIT_HARDWARE_ERROR        0x0B
#define BTIF_ERR_LOWER_LAYER_ERROR          0x0C
#define BTIF_ERR_ADDR_NOT_RESOLVED          0x0D
#define BTIF_ERR_TOUT                       0x0E

#define BTIF_ERR_UNSPECIFIED                0xFD
#define BTIF_ERR_NOT_SUPPORTED              0xFE
/** End of BT_BTIF_ERROR
 * @}
 */

/** @defgroup    BT_GAP_ERROR    Bluetooth GAP Error Code
  * @brief       Bluetooth GAP error code definitions.
  * @{
  */
#define GAP_SUCCESS                         0x00
#define GAP_ACCEPT                          0x01
#define GAP_ERR_REJECT                      0x02
#define GAP_ERR_NO_RESOURCE                 0x03
#define GAP_ERR_INVALID_PARAM               0x04
#define GAP_ERR_INVALID_STATE               0x05
#define GAP_ERR_CONN_DISCONNECT             0x06
#define GAP_ERR_CONN_LOST                   0x07
#define GAP_ERR_AUTHEN_FAIL                 0x08
#define GAP_ERR_INIT_TOUT                   0x09
#define GAP_ERR_INIT_OUT_OF_SYNC            0x0A
#define GAP_ERR_INIT_HARDWARE_ERROR         0x0B
#define GAP_ERR_LOWER_LAYER_ERROR           0x0C
#define GAP_ERR_ADDR_NOT_RESOLVED           0x0D
#define GAP_ERR_TOUT                        0x0E

#define GAP_ERR_REQ_FAILED                  0xFB
#define GAP_ERR_SW_RESET                    0xFC
#define GAP_ERR_UNSPECIFIED                 0xFD
#define GAP_ERR_NOT_SUPPORTED               0xFE
/** End of BT_GAP_ERROR
 * @}
 */

/** @defgroup    BT_APP_ERROR    Bluetooth APP Error Code
  * @brief       Bluetooth APP error code definitions.
  * @{
  */
#define APP_SUCCESS                             0x00
#define APP_ERR_PENDING                         0x01
#define APP_ERR_ACCEPT                          0x03
#define APP_ERR_REJECT                          0x04
#define APP_ERR_NOT_RELEASE                     0x05
/** End of BT_APP_ERROR
 * @}
 */

/** @defgroup    BT_BYTE_ORDER       Bluetooth Byte-order
  * @brief       Bluetooth buffer/array byte-order utility macros.
  * @{
  */
/** Calculate integer bit count of b'1. */
#define INT_BIT_COUNT(integer, count)   {               \
        count = 0;                                      \
        while (integer)                                 \
        {                                               \
            count++;                                    \
            integer &= integer - 1;                     \
        }                                               \
    }

/** Stream skip len. */
#define STREAM_SKIP_LEN(s, len)     {                   \
        s += len;                                       \
    }

/** Stream to array. */
#define STREAM_TO_ARRAY(a, s, len)  {                   \
        uint32_t ii;                                    \
        for (ii = 0; ii < len; ii++)                    \
        {                                               \
            *((uint8_t *)(a) + ii) = *s++;              \
        }                                               \
    }

/** Array to stream. */
#define ARRAY_TO_STREAM(s, a, len)  {                   \
        uint32_t ii;                                    \
        for (ii = 0; ii < len; ii++)                    \
        {                                               \
            *s++ = *((uint8_t *)(a) + ii);              \
        }                                               \
    }

/** Little Endian stream to uint8. */
#define LE_STREAM_TO_UINT8(u8, s)   {                   \
        u8  = (uint8_t)(*s);                            \
        s  += 1;                                        \
    }

/** Little Endian stream to uint16. */
#define LE_STREAM_TO_UINT16(u16, s) {                   \
        u16  = ((uint16_t)(*(s + 0)) << 0) +            \
               ((uint16_t)(*(s + 1)) << 8);             \
        s   += 2;                                       \
    }

/** Little Endian stream to uint24. */
#define LE_STREAM_TO_UINT24(u24, s) {                   \
        u24  = ((uint32_t)(*(s + 0)) <<  0) +           \
               ((uint32_t)(*(s + 1)) <<  8) +           \
               ((uint32_t)(*(s + 2)) << 16);            \
        s   += 3;                                       \
    }

/** Little Endian stream to uint32. */
#define LE_STREAM_TO_UINT32(u32, s) {                   \
        u32  = ((uint32_t)(*(s + 0)) <<  0) +           \
               ((uint32_t)(*(s + 1)) <<  8) +           \
               ((uint32_t)(*(s + 2)) << 16) +           \
               ((uint32_t)(*(s + 3)) << 24);            \
        s   += 4;                                       \
    }

/** Little Endian uint8 to stream. */
#define LE_UINT8_TO_STREAM(s, u8)   {                   \
        *s++ = (uint8_t)(u8);                           \
    }

/** Little Endian uint16 to stream. */
#define LE_UINT16_TO_STREAM(s, u16) {                   \
        *s++ = (uint8_t)((u16) >> 0);                   \
        *s++ = (uint8_t)((u16) >> 8);                   \
    }

/** Little Endian uint24 to stream. */
#define LE_UINT24_TO_STREAM(s, u24) {                   \
        *s++ = (uint8_t)((u24) >>  0);                  \
        *s++ = (uint8_t)((u24) >>  8);                  \
        *s++ = (uint8_t)((u24) >> 16);                  \
    }

/** Little Endian uint32 to stream. */
#define LE_UINT32_TO_STREAM(s, u32) {                   \
        *s++ = (uint8_t)((u32) >>  0);                  \
        *s++ = (uint8_t)((u32) >>  8);                  \
        *s++ = (uint8_t)((u32) >> 16);                  \
        *s++ = (uint8_t)((u32) >> 24);                  \
    }

/** Little Endian array to uint8. */
#define LE_ARRAY_TO_UINT8(u8, a)    {                   \
        u8  = (uint8_t)(*(a + 0));                      \
    }

/** Little Endian array to uint16. */
#define LE_ARRAY_TO_UINT16(u16, a)  {                   \
        u16 = ((uint16_t)(*(a + 0)) << 0) +             \
              ((uint16_t)(*(a + 1)) << 8);              \
    }

/** Little Endian array to uint24. */
#define LE_ARRAY_TO_UINT24(u24, a)  {                   \
        u24 = ((uint32_t)(*(a + 0)) <<  0) +            \
              ((uint32_t)(*(a + 1)) <<  8) +            \
              ((uint32_t)(*(a + 2)) << 16);             \
    }

/** Little Endian array to uint32. */
#define LE_ARRAY_TO_UINT32(u32, a) {                    \
        u32 = ((uint32_t)(*(a + 0)) <<  0) +            \
              ((uint32_t)(*(a + 1)) <<  8) +            \
              ((uint32_t)(*(a + 2)) << 16) +            \
              ((uint32_t)(*(a + 3)) << 24);             \
    }

/** Little Endian uint8 to array. */
#define LE_UINT8_TO_ARRAY(a, u8)    {                   \
        *((uint8_t *)(a) + 0) = (uint8_t)(u8);          \
    }

/** Little Endian uint16 to array. */
#define LE_UINT16_TO_ARRAY(a, u16)  {                   \
        *((uint8_t *)(a) + 0) = (uint8_t)((u16) >> 0);  \
        *((uint8_t *)(a) + 1) = (uint8_t)((u16) >> 8);  \
    }

/** Little Endian uint24 to array. */
#define LE_UINT24_TO_ARRAY(a, u24) {                    \
        *((uint8_t *)(a) + 0) = (uint8_t)((u24) >>  0); \
        *((uint8_t *)(a) + 1) = (uint8_t)((u24) >>  8); \
        *((uint8_t *)(a) + 2) = (uint8_t)((u24) >> 16); \
    }

/** Little Endian uint32 to array. */
#define LE_UINT32_TO_ARRAY(a, u32) {                    \
        *((uint8_t *)(a) + 0) = (uint8_t)((u32) >>  0); \
        *((uint8_t *)(a) + 1) = (uint8_t)((u32) >>  8); \
        *((uint8_t *)(a) + 2) = (uint8_t)((u32) >> 16); \
        *((uint8_t *)(a) + 3) = (uint8_t)((u32) >> 24); \
    }

/** Big Endian stream to uint8. */
#define BE_STREAM_TO_UINT8(u8, s)   {                   \
        u8   = (uint8_t)(*(s + 0));                     \
        s   += 1;                                       \
    }

/** Big Endian stream to uint16. */
#define BE_STREAM_TO_UINT16(u16, s) {                   \
        u16  = ((uint16_t)(*(s + 0)) << 8) +            \
               ((uint16_t)(*(s + 1)) << 0);             \
        s   += 2;                                       \
    }

/** Big Endian stream to uint24. */
#define BE_STREAM_TO_UINT24(u24, s) {                   \
        u24  = ((uint32_t)(*(s + 0)) << 16) +           \
               ((uint32_t)(*(s + 1)) <<  8) +           \
               ((uint32_t)(*(s + 2)) <<  0);            \
        s   += 3;                                       \
    }

/** Big Endian stream to uint32. */
#define BE_STREAM_TO_UINT32(u32, s) {                   \
        u32  = ((uint32_t)(*(s + 0)) << 24) +           \
               ((uint32_t)(*(s + 1)) << 16) +           \
               ((uint32_t)(*(s + 2)) <<  8) +           \
               ((uint32_t)(*(s + 3)) <<  0);            \
        s   += 4;                                       \
    }

/** Big Endian uint8 to stream. */
#define BE_UINT8_TO_STREAM(s, u8)   {                   \
        *s++ = (uint8_t)(u8);                           \
    }

/** Big Endian uint16 to stream. */
#define BE_UINT16_TO_STREAM(s, u16) {                   \
        *s++ = (uint8_t)((u16) >> 8);                   \
        *s++ = (uint8_t)((u16) >> 0);                   \
    }

/** Big Endian uint24 to stream. */
#define BE_UINT24_TO_STREAM(s, u24) {                   \
        *s++ = (uint8_t)((u24) >> 16);                  \
        *s++ = (uint8_t)((u24) >>  8);                  \
        *s++ = (uint8_t)((u24) >>  0);                  \
    }

/** Big Endian uint32 to stream. */
#define BE_UINT32_TO_STREAM(s, u32) {                   \
        *s++ = (uint8_t)((u32) >> 24);                  \
        *s++ = (uint8_t)((u32) >> 16);                  \
        *s++ = (uint8_t)((u32) >>  8);                  \
        *s++ = (uint8_t)((u32) >>  0);                  \
    }

/** Big Endian array to uint8. */
#define BE_ARRAY_TO_UINT8(u8, a)    {                   \
        u8  = (uint8_t)(*(a + 0));                      \
    }

/** Big Endian array to uint16. */
#define BE_ARRAY_TO_UINT16(u16, a)  {                   \
        u16 = ((uint16_t)(*(a + 0)) << 8) +             \
              ((uint16_t)(*(a + 1)) << 0);              \
    }

/** Big Endian array to uint24. */
#define BE_ARRAY_TO_UINT24(u24, a)  {                   \
        u24 = ((uint32_t)(*(a + 0)) << 16) +            \
              ((uint32_t)(*(a + 1)) <<  8) +            \
              ((uint32_t)(*(a + 2)) <<  0);             \
    }

/** Big Endian array to uint32. */
#define BE_ARRAY_TO_UINT32(u32, a)  {                   \
        u32 = ((uint32_t)(*(a + 0)) << 24) +            \
              ((uint32_t)(*(a + 1)) << 16) +            \
              ((uint32_t)(*(a + 2)) <<  8) +            \
              ((uint32_t)(*(a + 3)) <<  0);             \
    }

/** Big Endian uint8 to array. */
#define BE_UINT8_TO_ARRAY(a, u8)    {                   \
        *((uint8_t *)(a) + 0) = (uint8_t)(u8);          \
    }

/** Big Endian uint16 to array. */
#define BE_UINT16_TO_ARRAY(a, u16)  {                   \
        *((uint8_t *)(a) + 0) = (uint8_t)((u16) >> 8);  \
        *((uint8_t *)(a) + 1) = (uint8_t)((u16) >> 0);  \
    }

/** Big Endian uint24 to array. */
#define BE_UINT24_TO_ARRAY(a, u24)  {                   \
        *((uint8_t *)(a) + 0) = (uint8_t)((u24) >> 16); \
        *((uint8_t *)(a) + 1) = (uint8_t)((u24) >>  8); \
        *((uint8_t *)(a) + 2) = (uint8_t)((u24) >>  0); \
    }

/** Big Endian uint32 to array. */
#define BE_UINT32_TO_ARRAY(a, u32)  {                   \
        *((uint8_t *)(a) + 0) = (uint8_t)((u32) >> 24); \
        *((uint8_t *)(a) + 1) = (uint8_t)((u32) >> 16); \
        *((uint8_t *)(a) + 2) = (uint8_t)((u32) >>  8); \
        *((uint8_t *)(a) + 3) = (uint8_t)((u32) >>  0); \
    }

/** End of BT_BYTE_ORDER
 * @}
 */

#ifdef __cplusplus
}
#endif

/** End of BT_TYPES
 * @}
 */

#endif /* _BT_TYPES_H_ */
