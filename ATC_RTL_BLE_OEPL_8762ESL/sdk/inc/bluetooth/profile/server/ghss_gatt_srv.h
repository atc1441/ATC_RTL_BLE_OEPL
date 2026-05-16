/**
*****************************************************************************************
*     Copyright(c) 2017, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
  * @file     ghss_gatt_svc.h
  * @brief    Head file for using Generic Health Sensor Service.
  * @details  GHSS data structs and external functions declaration developed based on bt_gatt_svc.h.
  * @author
  * @date
  * @version  v1.0
  * *************************************************************************************
  */

/* Define to prevent recursive inclusion */
#ifndef _GHSS_GATT_SVC_H_
#define _GHSS_GATT_SVC_H_

#ifdef  __cplusplus
extern "C" {
#endif      /* __cplusplus */

/* Add Includes here */
#include "os_mem.h"
#include "os_queue.h"
#include "bt_gatt_svc.h"


/** @defgroup GHSS_GATT_SVC GHSS Device Information GATT Service
  * @brief  Generic Health Sensor Service
   * @details

     The Generic Health Sensor (GHS) specifications define a protocol by which personal health device data can be exchanged over
     a Bluetooth connection.

     GHSS communicates the semantic content contained in the Abstract Content Information Model (ACOM) defined in IEEE 11073-10206
     using the Generic Attribute Protocol (GATT).

     The default supported feature provided by GHSS is the notify and indicate property of the Live Health Observations characteristic,
     the notify and indicate property of the Stored Health Observations characteristic, the write and indicate property of the RACP
     characteristic, and the write and indicate property of the GHS Control Point characteristic.

     The specific configuration process can be achieved by modifying Macro @ref GHSS_SVC_FEATURE_SUPPORT.

    * <b>Example usage</b>
    * \code{.c}

        #define GHSS_CHAR_LIVE_GHSS_HEALTH_OBS_PROP_NOTIFY_SUPPORT                   1

    * \endcode

     Applications shall register the Generic Health Sensor Service during initialization through the @ref ghss_reg_srv function.

     Applications can format GHSS Health Observation Body value length through the @ref ghss_format_health_obs_body_length function.

     Applications can format GHSS Health Observations value through the @ref ghss_format_health_obs_sending_value function.

     Applications can format GHSS Health Observations segments through the @ref ghss_format_health_obs_segment function.

     Applications can report Health Observations data with a notification or indication through the @ref ghss_send_health_obs_report function.

     Applications can report Control Point data with an indication through the @ref ghss_send_cp_report function.

  * @{
  */

/*============================================================================*
 *                         Macros
 *============================================================================*/
/** @defgroup GHSS_GATT_Exported_Macros GHSS GATT Exported Macros
  * @brief
  * @{
  */
/** @defgroup GHSS_SVC_FEATURE_SUPPORT
  * @brief GHSS server feature support
  * @{
  */
/** @details
    Set GHSS_CHAR_LIVE_GHSS_HEALTH_OBS_PROP_NOTIFY_SUPPORT to 1 to support Live Health Observations
    characteristic with notify.
    Set GHSS_CHAR_LIVE_GHSS_HEALTH_OBS_PROP_INDICATE_SUPPORT to 1 to support Live Health Observations
    characteristic with indicate.
    Mandatory to support indications or notifications. Supporting both is also permitted.
*/
#define GHSS_CHAR_LIVE_GHSS_HEALTH_OBS_PROP_NOTIFY_SUPPORT                   1
#define GHSS_CHAR_LIVE_GHSS_HEALTH_OBS_PROP_INDICATE_SUPPORT                 1

/** @details
    Set GHSS_CHAR_STORED_GHSS_HEALTH_OBS_PROP_NOTIFY_SUPPORT to 1 to support Stored Health Observations
    characteristic with notify.
    Set GHSS_CHAR_STORED_GHSS_HEALTH_OBS_PROP_NOTIFY_INDICATE_SUPPORT to 1 to support Stored Health
    Observations characteristic with indicate.
    Mandatory to support indications or notifications. Supporting both is also permitted.
*/
#define GHSS_CHAR_STORED_GHSS_HEALTH_OBS_PROP_NOTIFY_SUPPORT                 1
#define GHSS_CHAR_STORED_GHSS_HEALTH_OBS_PROP_INDICATE_SUPPORT               1

/** @} End of GHSS_SVC_FEATURE_SUPPORT */

/** @defgroup GHSS_SVC_UUID
  * @brief GHSS service UUID
  * @{
  */
#define GATT_UUID_GENERIC_HEALTH_SENSOR                            0x1840

#define GATT_UUID_CHAR_LIVE_HEALTH_OBSERVATIONS                    0x2B8B
#define GATT_UUID_CHAR_STORED_HEALTH_OBSERVATIONS                  0x2BDD
#define GATT_UUID_CHAR_RECORD_ACCESS_CONTROL_POINT                 0x2A52
#define GATT_UUID_CHAR_GHS_CONTROL_POINT                           0x2BF4
/** @} End of GHSS_SVC_UUID */

/** @defgroup GHSS_SVC_ERR
  * @brief GHSS service error code
  * @{
  */
#define ATT_ERR_GHS_COMMAND_NOT_SUPPORTED                          (ATT_ERR | 0x81)
/** @} End of GHSS_SVC_ERR */

/** @defgroup GHSS_SVC_CB_MSG
  * @brief GHSS server callback messages
  * @{
  */
#define GATT_MSG_GHSS_SERVER_WRITE_IND          0x01
#define GATT_MSG_GHSS_SERVER_CCCD_UPDATE        0x02
/** @} End of GHSS_SVC_CB_MSG  */

/** @defgroup GHSS_SVC_DEF
  * @brief GHSS server define numerical value
  * @{
  */
#define GHSS_MAX_ROLLING_SEGMENT_COUNTER       63 /** The Rolling Segment Counter is incremented per segment during a connection. If the Rolling Segment Counter is equal to a value of 63, then the counter rolls over to 0 when it is next incremented. */
/** @} End of GHSS_SVC_DEF  */

/** End of GHSS_GATT_Exported_Macros
* @}
*/

/*============================================================================*
 *                         Types
 *============================================================================*/
/** @defgroup GHSS_GATT_Exported_Types GHSS GATT Exported Types
  * @brief
  * @{
  */
/**
 * @brief P_FUN_GHSS_SERVER_APP_CB GHSS Server Callback Function Point Definition Function
 *        pointer used in GHSS server module, to send events to specific server module.
 *        The events @ref GHSS_SVC_CB_MSG.
 */
typedef T_APP_RESULT(*P_FUN_GHSS_SERVER_APP_CB)(uint8_t conn_id, uint8_t type, void *p_data);

typedef enum
{
    GHSS_COMPONENT_TYPE_HOBV_COMPOUND_OBS,
    GHSS_COMPONENT_TYPE_HOBV_OBS_BUNDLE,
} T_GHSS_COMPONENT_TYPE;

/** @brief  GHSS Record Access Control Point OpCodes. */
typedef enum
{
    GHSS_RACP_OPCODE_RESERVED = 0,
    GHSS_RACP_OPCODE_DELETE_RECS = 0x02,
    GHSS_RACP_OPCODE_ABORT_OPERATION = 0x03,
    GHSS_RACP_OPCODE_REPORT_NBR_OF_RECS = 0x04,
    GHSS_RACP_OPCODE_NBR_OF_RECS_RESP = 0x05,
    GHSS_RACP_OPCODE_RESP_CODE = 0x06,
    GHSS_RACP_OPCODE_COMB_REPORT_RECS = 0x07,
    GHSS_RACP_OPCODE_COMB_REPORT_RESP = 0x08
} T_GHSS_RACP_OPCODE;

/** @brief  Check GHSS Record Access Control Point operation is available or not. */
#define GHSS_RACP_OPERATION_ACTIVE(x)        \
    ((x >= GHSS_RACP_OPCODE_DELETE_RECS) &&  \
     (x <= GHSS_RACP_OPCODE_COMB_REPORT_RESP))

/** @brief  GHSS Record Access Control Point Response Codes. */
typedef enum
{
    GHSS_RACP_RESP_RESERVED = 0x00,
    GHSS_RACP_RESP_SUCCESS = 0x01,
    GHSS_RACP_RESP_OPCODE_NOT_SUPPORTED = 0x02,
    GHSS_RACP_RESP_INVALID_OPERATOR = 0x03,
    GHSS_RACP_RESP_OPERATOR_NOT_SUPPORTED = 0x04,
    GHSS_RACP_RESP_INVALID_OPERAND = 0x05,
    GHSS_RACP_RESP_NO_RECS_FOUND = 0x06,
    GHSS_RACP_RESP_ABORT_UNSUCCESSFUL = 0x07,
    GHSS_RACP_RESP_PROC_NOT_COMPLETED = 0x08,
    GHSS_RACP_RESP_OPERAND_NOT_SUPPORTED = 0x09,
    GHSS_RACP_RESP_SERVER_BUSY = 0x0A
} T_GHSS_RACP_RESP_CODES;

/** @brief  GHSS Record Access Control Point Operator. */
typedef enum
{
    GHSS_RACP_OPERATOR_NULL = 0x00,
    GHSS_RACP_OPERATOR_ALL_RECS = 0x01,
    GHSS_RACP_OPERATOR_LT_EQ = 0x02,
    GHSS_RACP_OPERATOR_GT_EQ = 0x03,
    GHSS_RACP_OPERATOR_RANGE = 0x04,
    GHSS_RACP_OPERATOR_FIRST = 0x05,
    GHSS_RACP_OPERATOR_LAST = 0x06
} T_GHSS_RACP_OPERATOR;

/** @brief  GHSS Record Access Control Point Operand Filter Type. */
typedef enum
{
    GHSS_RACP_OPERAND_RFU = 0x00,
    GHSS_RACP_OPERAND_REC_NUM = 0x01,
    GHSS_RACP_OPERAND_TIME = 0x02,
} T_GHSS_RACP_OPERAND_FILTER_TYPE;

/** @brief  GHSS Record Access Control Point. */
typedef struct
{
    T_GHSS_RACP_OPCODE opcode;
    T_GHSS_RACP_OPERATOR operator;
    uint8_t operand[18];
} T_GHSS_RACP;

/** @brief  GHSS GHS Control Point OpCodes. */
typedef enum
{
    GHSS_GHS_CP_OPCODE_RFU = 0,
    GHSS_GHS_CP_OPCODE_START_SEND_LIVE_OBS = 0x01,
    GHSS_GHS_CP_OPCODE_STOP_SEND_LIVE_OBS = 0x02,
    GHSS_GHS_CP_OPCODE_SUCCESS = 0x80,
} T_GHSS_GHS_CP_OPCODE;

typedef struct
{
    uint8_t first_segment: 1;
    uint8_t last_segment: 1;
    uint8_t rolling_segment_counter: 6;
} T_GHSS_HEALTH_OBS_SEG_HEADER;

typedef enum
{
    GHSS_HOB_CLASS_TYPE_NUMERIC_OBSERVATION = 1,
    GHSS_HOB_CLASS_TYPE_SIMPLE_DISCRETE_OBSERVATION,
    GHSS_HOB_CLASS_TYPE_STRING_OBSERVATION,
    GHSS_HOB_CLASS_TYPE_SAMPLE_ARRAY_OBSERVATION,
    GHSS_HOB_CLASS_TYPE_COMPOUND_DISCRETE_EVENT_OBSERVATION,
    GHSS_HOB_CLASS_TYPE_COMPOUND_STATE_EVENT_OBSERVATION,
    GHSS_HOB_CLASS_TYPE_COMPOUND_OBSERVATION,
    GHSS_HOB_CLASS_TYPE_TLV_ENCODED_OBSERVATION,
    GHSS_HOB_CLASS_TYPE_OBSERVATION_BUNDLE = 255,
} T_GHSS_HOB_CLASS_TYPE;

typedef struct
{
    uint16_t observation_type_present: 1;
    uint16_t time_stamp_present: 1;
    uint16_t measurement_duration_present: 1;
    uint16_t measurement_status_present: 1;
    uint16_t observation_id_present: 1;
    uint16_t patient_present: 1;
    uint16_t supplemental_information_present: 1;
    uint16_t derived_from_present: 1;
    uint16_t is_member_of_present: 1;
    uint16_t tlvs_present: 1;
    uint16_t rfu: 6;
} T_GHSS_HOB_FLAGS;

typedef struct
{
    uint8_t flags;
    uint8_t time_value[6];
    uint8_t time_sync_source_type;
    uint8_t tz_dst_offset;
} T_GHSS_HOB_TIME_STAMP;

typedef struct
{
    uint16_t invalid: 1;
    uint16_t questionable: 1;
    uint16_t not_available: 1;
    uint16_t calibrating: 1;
    uint16_t test_data: 1;
    uint16_t early_estimate: 1;
    uint16_t manually_entered: 1;
    uint16_t setting: 1;
    uint16_t rfu: 6;
    uint16_t threshold_error: 1;
    uint16_t thresholding_disabled: 1;
} T_GHSS_HOB_MEAS_STATUS;

typedef struct
{
    uint8_t count;
    uint32_t *p_codes;
} T_GHSS_HOB_SUPPL_INFO;

typedef struct
{
    uint8_t count;
    uint32_t *p_obs_ids;
} T_GHSS_HOB_DERIVED_FROM;

typedef struct
{
    uint8_t count;
    uint32_t *p_mem_ids;
} T_GHSS_HOB_IS_MEM_OF;

typedef struct
{
    uint32_t type;
    uint16_t length;
    uint8_t format_type;
    void *p_encoded_value;
} T_GHSS_HOB_TLVS_VALUE;

typedef struct
{
    uint8_t num_of_tlv_attr;
    T_GHSS_HOB_TLVS_VALUE *p_tlvs_value;
} T_GHSS_HOB_TLVS;

typedef struct
{
    uint16_t unit_code;
    uint32_t value;    //Change float to uint32_t for sending
} T_GHSS_HOBV_NUMERIC_OBS;

typedef struct
{
    uint32_t value;
} T_GHSS_HOBV_SIMPLE_DISCRETE_OBS;

typedef struct
{
    uint16_t length;
    char *p_value;
} T_GHSS_HOBV_STRING_OBS;

typedef struct
{
    uint16_t unit_code;
    uint32_t scale_factor;  //Change float to uint32_t for sending
    uint32_t offset;        //Change float to uint32_t for sending
    uint32_t sample_period; //Change float to uint32_t for sending
    uint8_t number_of_samples_per_period;
    uint8_t bytes_per_period;
    uint32_t number_of_samples;
    void *p_sample;
} T_GHSS_HOBV_SAMPLE_ARRAY_OBS;

typedef struct
{
    uint8_t number_of_components;
    uint32_t *p_value;
} T_GHSS_HOBV_COMPOUND_DISCRETE_EVENT_OBS;

typedef struct
{
    uint8_t size;
    uint8_t *p_mask_bits;
    uint8_t *p_state_event_mask_bits;
    uint8_t *p_value;
} T_GHSS_HOBV_COMPOUND_STATE_EVENT_OBS;

typedef struct
{
    uint32_t component_type;
    T_GHSS_HOB_CLASS_TYPE component_value_type; //Only 1 - 6
    void *p_value;  //Structure depends on component_value_type
} T_GHSS_HOBV_COMPOUND_OBS_COMPONENT;

typedef struct
{
    uint8_t number_of_components;
    T_GHSS_HOBV_COMPOUND_OBS_COMPONENT *p_component_value;
} T_GHSS_HOBV_COMPOUND_OBS;

typedef struct
{
    uint8_t number_of_obs;
    void *p_obs_bdl_value; // Structure @ref T_GHSS_HEALTH_OBS_BODY
} T_GHSS_HOBV_OBS_BUNDLE;

/** @brief  Structure depends on the Observation Class Type. */
typedef union
{
    T_GHSS_HOBV_NUMERIC_OBS numeric_obs;
    T_GHSS_HOBV_SIMPLE_DISCRETE_OBS simple_discrete_obs;
    T_GHSS_HOBV_STRING_OBS string_obs;
    T_GHSS_HOBV_SAMPLE_ARRAY_OBS sample_array_obs;
    T_GHSS_HOBV_COMPOUND_DISCRETE_EVENT_OBS compound_discrete_event_obs;
    T_GHSS_HOBV_COMPOUND_STATE_EVENT_OBS compound_state_event_obs;
    T_GHSS_HOBV_COMPOUND_OBS compound_obs;
    T_GHSS_HOB_TLVS tlv_encoded_obs;
    T_GHSS_HOBV_OBS_BUNDLE obs_bundle;
} T_GHSS_HOB_VALUE;

/** @brief  Health Observation Body. */
typedef struct
{
    T_GHSS_HOB_CLASS_TYPE obs_class_type;
    T_GHSS_HOB_FLAGS flags;
    uint32_t obs_type;
    T_GHSS_HOB_TIME_STAMP time_stamp;
    uint32_t meas_duration;    //Change float to uint32_t for sending
    T_GHSS_HOB_MEAS_STATUS meas_status;
    uint32_t obs_id;
    uint8_t patient;
    T_GHSS_HOB_SUPPL_INFO suppl_info;
    T_GHSS_HOB_DERIVED_FROM derived_from;
    T_GHSS_HOB_IS_MEM_OF is_mem_of;
    T_GHSS_HOB_TLVS tlvs;
    T_GHSS_HOB_VALUE obs_value;
} T_GHSS_HEALTH_OBS_BODY;

/** @brief  Health Observation. */
typedef struct
{
    T_GHSS_HEALTH_OBS_SEG_HEADER seg_header;
    T_GHSS_HEALTH_OBS_BODY obs_body;
} T_GHSS_HEALTH_OBS;

typedef enum
{
    GHSS_OBS_REPORT_TYPE_LIVE_HEALTH_OBS,
    GHSS_OBS_REPORT_TYPE_STORED_HEALTH_OBS,
} T_GHSS_OBS_REPORT_TYPE;

typedef struct
{
    uint8_t obs_segment_len;
    uint8_t *p_obs_segment;
} T_GHSS_REPORT_OBS_SEGMENT;

typedef struct
{
    T_GHSS_OBS_REPORT_TYPE report_type;
    bool is_notify;
    T_GHSS_REPORT_OBS_SEGMENT obs_segment;
} T_GHSS_CHAR_OBS_REPORT;

typedef enum
{
    GHSS_CP_REPORT_TYPE_RACP,
    GHSS_CP_REPORT_TYPE_GHS_CP,
} T_GHSS_CP_REPORT_TYPE;

typedef struct
{
    uint8_t racp_len;
    T_GHSS_RACP *p_racp;
} T_GHSS_REPORT_RACP;

typedef struct
{
    T_GHSS_CP_REPORT_TYPE report_type;
    union
    {
        T_GHSS_REPORT_RACP report_racp;
        T_GHSS_GHS_CP_OPCODE *p_ghs_cp;
    } value;
} T_GHSS_CHAR_CP_REPORT;

/** @brief GHSS server write data indication
 *         The message data for GATT_MSG_GHSS_SERVER_WRITE_IND.
*/
typedef struct
{
    T_SERVER_ID service_id;
    uint16_t char_uuid;
    T_WRITE_TYPE write_type;
    union
    {
        T_GHSS_RACP ghss_racp;
        T_GHSS_GHS_CP_OPCODE ghss_ghs_cp;
    } data;
} T_GHSS_SERVER_WRITE_IND;

/** @brief GHSS server cccd data update info
 *         The message data for GATT_MSG_GHSS_SERVER_CCCD_UPDATE.
*/
typedef struct
{
    T_SERVER_ID service_id;
    uint16_t char_uuid;
    uint16_t cccd_cfg;
} T_GHSS_SERVER_CCCD_UPDATE;

/** End of GHSS_GATT_Exported_Types
* @}
*/

/*============================================================================*
 *                         Functions
 *============================================================================*/
/** @defgroup GHSS_GATT_Exported_Functions GHSS GATT Exported Functions
  * @brief
  * @{
  */

/**
 * @brief       Add Generic Health Sensor Service to the Host database.
 *
 *
 * @param[in]   app_cb            Callback when service attribute was read, write or cccd update.
 * @return Service id generated by the Host: @ref T_SERVER_ID.
 * @retval 0xFF Operation failure.
 * @retval Others Service id assigned by Host.
 *
 * <b>Example usage</b>
 * \code{.c}
    void profile_init()
    {
        server_init(service_num);
        ghss_id = ghss_reg_srv(app_ghss_handle_server_cb);
    }
 * \endcode
 */
T_SERVER_ID ghss_reg_srv(P_FUN_GHSS_SERVER_APP_CB app_cb);

/**
 * @brief       Format GHSS Health Observation Body value length.
 *
 *
 * @param[in]       p_value         GHSS Health Observation Body value.
 *
 * @return Total length of Health Observation Body.
 */
uint16_t ghss_format_health_obs_body_length(T_GHSS_HEALTH_OBS_BODY *p_value);

/**
 * @brief       Format GHSS Health Observations value.
 *
 * @note    When all segments of the Health Observations value have been sent complete,
 *          APP shall free the value memory.
 *
 * @param[in]       type            Report type @ref T_GHSS_OBS_REPORT_TYPE.
 * @param[in]       idx             Report idx of Stored Health Observation Body. \n
 *                                  If type equals @ref GHSS_OBS_REPORT_TYPE_LIVE_HEALTH_OBS, this parameter is not effective.
 * @param[in]       p_value         GHSS Health Observation Body value.
 * @param[in,out]   pp_temp_value   Pointer to pointer of value needs to be sent.
 *
 * @return Total length of Health Observations value.
 */
uint16_t ghss_format_health_obs_sending_value(T_GHSS_OBS_REPORT_TYPE type, uint32_t idx,
                                              T_GHSS_HEALTH_OBS_BODY *p_value,
                                              uint8_t **pp_temp_value);

/**
 * @brief       Format GHSS Health Observation segments.
 *
 * @note    When segment has been sent, APP shall free the segment memory.
 *
 * @param[in]       seg_count           Current count of Health Observation segment needs to be sent.
 * @param[in]       seg_num             The number of all segments need to be sent.
 * @param[in]       report_mtu          Report MTU size.
 * @param[in]       rolling_seg_count   Rolling Segment Counter.
 * @param[in]       value_len           Total length of Health Observation value.
 * @param[in]       p_value             Pointer to value needs to be sent.
 * @param[in,out]   pp_seg              Pointer to pointer of segment needs to be sent.
 *
 * @return  Length of segment needs to be sent.
 */
uint16_t ghss_format_health_obs_segment(uint8_t seg_count, uint8_t seg_num, uint16_t report_mtu,
                                        uint8_t rolling_seg_count, uint16_t value_len,
                                        uint8_t *p_value, uint8_t **pp_seg);

/**
 * @brief        Report Health Observations data with a notification or indication.
 *
 *
 * @param[in]   conn_id         Connection ID.
 * @param[in]   service_id      Service ID.
 * @param[in]   report_data     Report data @ref T_GHSS_CHAR_OBS_REPORT.
 *
 * @return Operation result.
 * @retval true Operation success.
 * @retval false Operation failure.
 */
bool ghss_send_health_obs_report(uint8_t conn_id, uint8_t service_id,
                                 T_GHSS_CHAR_OBS_REPORT report_data);

/**
 * @brief        Report Control Point data with an indication.
 *
 *
 * @param[in]   conn_id         Connection ID.
 * @param[in]   service_id      Service ID.
 * @param[in]   report_data     Report data @ref T_GHSS_CHAR_CP_REPORT.
 *
 * @return Operation result.
 * @retval true Operation success.
 * @retval false Operation failure.
 */
bool ghss_send_cp_report(uint8_t conn_id, uint8_t service_id, T_GHSS_CHAR_CP_REPORT report_data);

/** @} End of GHSS_GATT_Exported_Functions */

/** @} End of GHSS_GATT_SVC */


#ifdef  __cplusplus
}
#endif      /*  __cplusplus */

#endif  /* _GHSS_GATT_SVC_H_ */
