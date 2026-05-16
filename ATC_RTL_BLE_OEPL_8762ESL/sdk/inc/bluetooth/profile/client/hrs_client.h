/**
*****************************************************************************************
*     Copyright(c) 2016, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
  * @file     hrs_client.h
  * @brief    Head file for using heart rate service client.
  * @details  Heart rate service client data structs and external functions declaration.
  * @author   jane
  * @date     2016-02-18
  * @version  v1.0
  * *************************************************************************************
  */

/* Define to prevent recursive inclusion */
#ifndef _HRS_CLIENT_H_
#define _HRS_CLIENT_H_

#ifdef  __cplusplus
extern "C" {
#endif      /* __cplusplus */

/* Add Includes here */
#include <hrs_define.h>
#include <profile_client.h>
#include <stdint.h>
#include <stdbool.h>



/** @defgroup HRS_CLIENT Heart rate Service Client
  * @brief HRS client
  * @details
     Application shall register hrs client when initialization through @ref hrs_add_client function.

     Application can start discovery heart rate service through @ref hrs_start_discovery function.

     Application can config and read the notification flag through @ref hrs_set_heart_rate_measurement_notify function.

     Application shall handle callback function registered by hrs_add_client.
  * \code{.c}
    T_APP_RESULT app_client_callback(T_CLIENT_ID client_id, uint8_t conn_id, void *p_data)
    {
        T_APP_RESULT  result = APP_RESULT_SUCCESS;
        if (client_id == hrs_client_id)
        {
            T_HRS_CLIENT_CB_DATA *p_hrs_cb_data = (T_HRS_CLIENT_CB_DATA *)p_data;
            switch (p_hrs_cb_data->cb_type)
            {
            case HRS_CLIENT_CB_TYPE_DISC_STATE:
                switch (p_hrs_cb_data->cb_content.disc_state)
                {
                case DISC_HRS_DONE:
                ......
        }
    }
  * \endcode
  * @{
  */

/*============================================================================*
 *                         Macros
 *============================================================================*/
/** @addtogroup HRS_CLIENT_Exported_Macros HRS Client Exported Macros
  * @brief
  * @{
  */

/** @brief  Define links number. range: 0-4 */
#define HRS_MAX_LINKS  4

/** End of HRS_CLIENT_Exported_Macros
* @}
*/


/*============================================================================*
 *                         Types
 *============================================================================*/
/** @defgroup HRS_CLIENT__Exported_Types HRS Client Exported Types
  * @brief
  * @{
  */

/** @brief HRS client handle type*/
typedef enum
{
    HDL_HRS_SRV_START,           //!< start handle of heart rate service
    HDL_HRS_SRV_END,             //!< end handle of heart rate service
    HDL_HRS_HEART_RATE_MEASUREMENT,       //!< heart rate measurement characteristic handle
    HDL_HRS_HEART_RATE_MEASUREMENT_CCCD,  //!< Heart Rate Measurement characteristic CCCD handle
    HDL_HRS_BODY_SENSOR_LOCATION,//!< body sensor location characteristic handle
    HDL_HRS_CACHE_LEN            //!< handle cache length
} T_HRS_HANDLE_TYPE;

/** @brief HRS client discovery state*/
typedef enum
{
    DISC_HRS_IDLE,
    DISC_HRS_START,
    DISC_HRS_DONE,
    DISC_HRS_FAILED
} T_HRS_DISC_STATE;

/** @brief HRS client read data */
typedef union
{
    bool notify;
} T_HRS_READ_DATA;

/** @brief HRS client read type*/
typedef enum
{
    HRS_READ_NOTIFY,
} T_HRS_READ_TYPE;

/** @brief HRS client read result*/
typedef struct
{
    T_HRS_READ_TYPE type;
    T_HRS_READ_DATA data;
    uint16_t cause;
} T_HRS_READ_RESULT;

/** @brief HRS client data type*/
typedef enum
{
    HRS_FROM_HEART_RATE_MEASUREMENT,
} T_HRS_DATA_TYPE;

/** @brief HRS client notification data struct*/
typedef struct
{
    T_HRS_DATA_TYPE type;
    uint16_t value_size;
    uint8_t *p_value;
} T_HRS_NOTIFY_DATA;

/** @brief HRS client write type*/
typedef enum
{
    HRS_WRITE_HEART_RATE_MEASUREMENT_NOTIFY_ENABLE,
    HRS_WRITE_HEART_RATE_MEASUREMENT_NOTIFY_DISABLE,
} T_HRS_WRTIE_TYPE;

/** @brief HRS client write result*/
typedef struct
{
    T_HRS_WRTIE_TYPE type;
    uint16_t cause;
} T_HRS_WRITE_RESULT;

/** @brief HRS client callback type*/
typedef enum
{
    HRS_CLIENT_CB_TYPE_DISC_STATE,          //!< Discovery procedure state, done or pending.
    HRS_CLIENT_CB_TYPE_READ_RESULT,         //!< Read request's result data, responsed from server.
    HRS_CLIENT_CB_TYPE_WRITE_RESULT,        //!< Write request result, success or fail.
    HRS_CLIENT_CB_TYPE_NOTIF_IND_RESULT,    //!< Notification or indication data received from server.
    HRS_CLIENT_CB_TYPE_INVALID              //!< Invalid callback type, no practical usage.
} T_HRS_CLIENT_CB_TYPE;

/** @brief HRS client callback content*/
typedef union
{
    T_HRS_DISC_STATE disc_state;
    T_HRS_READ_RESULT read_result;
    T_HRS_NOTIFY_DATA notify_data;
    T_HRS_WRITE_RESULT write_result;
} T_HRS_CLIENT_CB_CONTENT;

/** @brief HRS client callback data*/
typedef struct
{
    T_HRS_CLIENT_CB_TYPE     cb_type;
    T_HRS_CLIENT_CB_CONTENT  cb_content;
} T_HRS_CLIENT_CB_DATA;
/** End of HRS_CLIENT_Exported_Types
* @}
*/


/*============================================================================*
 *                         Functions
 *============================================================================*/
/** @defgroup HRS_CLIENT_Exported_Functions HRS Client Exported Functions
  * @brief
  * @{
  */

/**
  * @brief      Add hrs client to application.
  * @param[in]  app_cb pointer of app callback function to handle specific client module data.
  * @param[in]  link_num initialize link num.
  * @return Client ID of the specific client module.
  * @retval 0xff failed.
  * @retval other success.
  *
  * <b>Example usage</b>
  * \code{.c}
    void app_le_profile_init(void)
    {
        client_init(1);
        hrs_client_id = hrs_add_client(app_client_callback, APP_MAX_LINKS);
    }
  * \endcode
  */
T_CLIENT_ID hrs_add_client(P_FUN_GENERAL_APP_CB app_cb, uint8_t link_num);

/**
  * @brief  Used by application, to start the discovery procedure of heart rate service.
  * @param[in]  conn_id connection ID.
  * @retval true send request to upper stack success.
  * @retval false send request to upper stack failed.
  *
  * <b>Example usage</b>
  * \code{.c}
    static T_USER_CMD_PARSE_RESULT cmd_hrsdis(T_USER_CMD_PARSED_VALUE *p_parse_value)
    {
        uint8_t conn_id = p_parse_value->dw_param[0];
        bool ret = hrs_start_discovery(conn_id);
        ......
    }

    T_APP_RESULT app_handle_client_message(T_CLIENT_ID client_id, uint8_t conn_id, void *p_data)
    {
        T_APP_RESULT  result = APP_RESULT_SUCCESS;
        ......
        else if (client_id == hrs_cl_id)
        {
            APP_PRINT_INFO0("app_handle_client_message: hrs_cl_id");

            T_HRS_CLIENT_CB_DATA *p_hrs_cb_data = (T_HRS_CLIENT_CB_DATA *)p_data;
            switch (p_hrs_cb_data->cb_type)
            {
            case HRS_CLIENT_CB_TYPE_DISC_STATE:
                if (p_hrs_cb_data->cb_content.disc_state == DISC_STATE_SRV_DONE)
                {
                    APP_PRINT_INFO0("app_handle_client_message: discovery service procedure done.");
                }
                else
                {
                    APP_PRINT_INFO0("app_handle_client_message: discovery state send to application directly.");
                }
                break;

            ......
        }
        ......
      }
  * \endcode
  */
bool hrs_start_discovery(uint8_t conn_id);

/**
  * @brief  Used by application, to set the notification flag of heart rate measurement.
  * @param[in]  conn_id connection ID.
  * @param[in]  notify value to enable or disable notify.
  * @retval true send request to upper stack success.
  * @retval false send request to upper stack failed.
  *
  * <b>Example usage</b>
  * \code{.c}
    static T_USER_CMD_PARSE_RESULT cmd_hrsmeascccd(T_USER_CMD_PARSED_VALUE *p_parse_value)
    {
        uint8_t conn_id = p_parse_value->dw_param[0];
        bool notify = p_parse_value->dw_param[1];
        bool ret;
        ret = hrs_set_heart_rate_measurement_notify(conn_id, notify);
        ......
    }

    T_APP_RESULT app_handle_client_message(T_CLIENT_ID client_id, uint8_t conn_id, void *p_data)
    {
        T_APP_RESULT  result = APP_RESULT_SUCCESS;
        ......
        else if (client_id == hrs_cl_id)
        {
            APP_PRINT_INFO0("app_handle_client_message: hrs_cl_id");

            T_HRS_CLIENT_CB_DATA *p_hrs_cb_data = (T_HRS_CLIENT_CB_DATA *)p_data;
            switch (p_hrs_cb_data->cb_type)
            {
            case HRS_CLIENT_CB_TYPE_WRITE_RESULT:
            switch (p_hrs_cb_data->cb_content.write_result.type)
            {
            case HRS_WRITE_HEART_RATE_MEASUREMENT_NOTIFY_ENABLE:
                APP_PRINT_INFO1("HRS_WRITE_HEART_RATE_MEASUREMENT_NOTIFY_ENABLE: write result 0x%x",
                                p_hrs_cb_data->cb_content.write_result.cause);
                break;
            case HRS_WRITE_HEART_RATE_MEASUREMENT_NOTIFY_DISABLE:
                APP_PRINT_INFO1("HRS_WRITE_HEART_RATE_MEASUREMENT_NOTIFY_DISABLE: write result 0x%x",
                                p_hrs_cb_data->cb_content.write_result.cause);
                break;
            default:
                break;
            }
            break;

        case HRS_CLIENT_CB_TYPE_NOTIF_IND_RESULT:
            {
                if (p_hrs_cb_data->cb_content.notify_data.type == HRS_FROM_HEART_RATE_MEASUREMENT)
                {
                    T_HEART_RATE_MEASUREMENT_VALUE hrs_heart_rate_measurement_value;
                    uint8_t heart_rate_measurement_flag;

                    LE_STREAM_TO_UINT8(heart_rate_measurement_flag, p_hrs_cb_data->cb_content.notify_data.p_value);

                    memcpy(&hrs_heart_rate_measurement_value.flag, &heart_rate_measurement_flag, sizeof(uint8_t));

                    if (p_hrs_cb_data->cb_type == HRS_CLIENT_CB_TYPE_NOTIF_IND_RESULT)
                    {
                        APP_PRINT_INFO0("HRS_CLIENT_CB_TYPE_NOTIF_HEART_RATE_MEASUREMENT");
                    }

                    if (hrs_heart_rate_measurement_value.flag.heart_rate_value_format_bit)
                    {
                        LE_STREAM_TO_UINT16(hrs_heart_rate_measurement_value.heart_rate_measurement_value,
                                            p_hrs_cb_data->cb_content.notify_data.p_value);
                        APP_PRINT_INFO3("heart rate measurement flag 0x%x, heart_rate_value_format %d, heart_rate_measurement_value 0x%x",
                                        heart_rate_measurement_flag, Heart_Rate_Value_Format_UINT16,
                                        hrs_heart_rate_measurement_value.heart_rate_measurement_value);
                    }
                    else
                    {
                        LE_STREAM_TO_UINT8(hrs_heart_rate_measurement_value.heart_rate_measurement_value,
                                           p_hrs_cb_data->cb_content.notify_data.p_value);
                        APP_PRINT_INFO3("heart rate measurement flag 0x%x, heart_rate_value_format %d, heart_rate_measurement_value 0x%x",
                                        heart_rate_measurement_flag, Heart_Rate_Value_Format_UINT8,
                                        hrs_heart_rate_measurement_value.heart_rate_measurement_value);
                    }

                    if (hrs_heart_rate_measurement_value.flag.energy_expended_status_bit)
                    {
                        LE_STREAM_TO_UINT16(hrs_heart_rate_measurement_value.energy_expended,
                                            p_hrs_cb_data->cb_content.notify_data.p_value);
                        APP_PRINT_INFO1("heart rate measurement energy_expended 0x%x",
                                        hrs_heart_rate_measurement_value.energy_expended);
                    }

                    if (hrs_heart_rate_measurement_value.flag.rr_interval_bit)
                    {
                        LE_STREAM_TO_UINT16(hrs_heart_rate_measurement_value.rr_interval,
                                            p_hrs_cb_data->cb_content.notify_data.p_value);
                        APP_PRINT_INFO1("heart rate measurement rr_interval 0x%x",
                                        hrs_heart_rate_measurement_value.rr_interval);
                    }
                }
            }
            break;
                        ......
        }
        ......
      }
  * \endcode
  */
bool hrs_set_heart_rate_measurement_notify(uint8_t conn_id, bool notify);

/** @} End of HRS_CLIENT_Exported_Functions */

/** @} End of HRS_CLIENT */

#ifdef  __cplusplus
}
#endif      /*  __cplusplus */

#endif  /* _HRS_CLIENT_H_ */
