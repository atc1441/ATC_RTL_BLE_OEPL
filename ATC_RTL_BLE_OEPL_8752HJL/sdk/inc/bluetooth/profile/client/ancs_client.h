/**
*****************************************************************************************
*     Copyright(c) 2016, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
  * @file     ancs_client.h
  * @brief    Header file for using ANCS Client.
  * @details
  * @author   jane
  * @date     2016-02-18
  * @version  v1.0
  * *************************************************************************************
  */

/* Define to prevent recursive inclusion */
#ifndef _ANCS_CLIENT_H_
#define _ANCS_CLIENT_H_

#ifdef  __cplusplus
extern "C" {
#endif      /* __cplusplus */

/* Add Includes here */
#include <profile_client.h>
#include <stdint.h>
#include <stdbool.h>


/** @defgroup ANCS_CLIENT ANCS Client
* @brief ANCS client
* @{
*/

/*============================================================================*
 *                         Macros
 *============================================================================*/
/** @addtogroup ANCS_CLIENT_Exported_Macros ANCS Client Exported Macros
  * @brief
  * @{
  */

/** @brief  Define links number. */
#define ANCS_MAX_LINKS  4


/** End of ANCS_CLIENT_Exported_Macros
* @}
*/


/*============================================================================*
 *                         Types
 *============================================================================*/
/** @defgroup ANCS_CLIENT_Exported_Types ANCS Client Exported Types
  * @brief
  * @{
  */

/** @brief ANCS client handle type*/
typedef enum
{
    HDL_ANCS_SRV_START,                //!< Start handle of ANCS.
    HDL_ANCS_SRV_END,                  //!< End handle of ANCS.
    HDL_ANCS_CONTROL_POINT,            //!< Control point characteristic value handle.
    HDL_ANCS_NOTIFICATION_SOURCE,      //!< Notification source characteristic value handle.
    HDL_ANCS_NOTIFICATION_SOURCE_CCCD, //!< Notification source characteristic CCCD handle.
    HDL_ANCS_DATA_SOURCE,              //!< Data source characteristic value handle.
    HDL_ANCS_DATA_SOURCE_CCCD,         //!< Data source characteristic CCCD handle.
    HDL_ANCS_CACHE_LEN                 //!< Handle cache length.
} T_ANCS_HANDLE_TYPE;

/** @brief ANCS client control command ID*/
typedef enum
{
    CP_CMD_ID_GET_NOTIFICATION_ATTR = 0,             //!< Command ID to get the notification attribute.
    CP_CMD_ID_GET_APP_ATTR = 1,                      //!< Command ID to get the APP attribute.
    CP_CMD_ID_PERFORM_NOTIFICATION_ACTION = 2,       //!< Command ID to perform notification action.
    CP_CMD_ID_RESERVED = 255                         //!< Command ID reserved.
} T_ANCS_CP_CMD_ID;

/** @brief ANCS client discovery state*/
typedef enum
{
    DISC_ANCS_IDLE,     //!< Idle.
    DISC_ANCS_START,    //!< Start.
    DISC_ANCS_DONE,     //!< Finish.
    DISC_ANCS_FAILED    //!< Failed.
} T_ANCS_DISC_STATE;

/** @brief ANCS client data type*/
typedef enum
{
    ANCS_FROM_DATA_SOURCE,
    ANCS_FROM_NOTIFICATION_SOURCE,
} T_ANCS_DATA_TYPE;

/** @brief ANCS client notification data struct*/
typedef struct
{
    T_ANCS_DATA_TYPE type;
    uint16_t value_size;
    uint8_t *p_value;
} T_ANCS_NOTIFY_DATA;

/** @brief ANCS client write type*/
typedef enum
{
    ANCS_WRITE_NOTIFICATION_SOURCE_NOTIFY_ENABLE,
    ANCS_WRITE_NOTIFICATION_SOURCE_NOTIFY_DISABLE,
    ANCS_WRITE_DATA_SOURCE_NOTIFY_ENABLE,
    ANCS_WRITE_DATA_SOURCE_NOTIFY_DISABLE,
    ANCS_WRITE_CONTROL_POINT,
} T_ANCS_WRTIE_TYPE;

/** @brief ANCS client write result*/
typedef struct
{
    T_ANCS_WRTIE_TYPE type;
    uint16_t cause;
} T_ANCS_WRITE_RESULT;

/** @brief ANCS client callback type*/
typedef enum
{
    ANCS_CLIENT_CB_TYPE_DISC_STATE,          //!< Discovery procedure state, done or pending.
    ANCS_CLIENT_CB_TYPE_WRITE_RESULT,        //!< Write result, success or fail.
    ANCS_CLIENT_CB_TYPE_NOTIF_IND_RESULT,    //!< Notification or indication data received from server.
    ANCS_CLIENT_CB_TYPE_DISCONNECT_INFO,     //!< Information of disconnection.
    ANCS_CLIENT_CB_TYPE_INVALID              //!< Invalid callback type, no practical usage.
} T_ANCS_CB_TYPE;

/** @brief ANCS client callback content*/
typedef union
{
    T_ANCS_DISC_STATE disc_state;
    T_ANCS_NOTIFY_DATA notify_data;
    T_ANCS_WRITE_RESULT write_result;
} T_ANCS_CB_CONTENT;

/** @brief ANCS client callback data*/
typedef struct
{
    T_ANCS_CB_TYPE     cb_type;
    T_ANCS_CB_CONTENT    cb_content;
} T_ANCS_CB_DATA;
/** End of ANCS_CLIENT_Exported_Types
* @}
*/


/*============================================================================*
 *                         Functions
 *============================================================================*/
/** @defgroup ANCS_CLIENT_Exported_Functions ANCS Client Exported Functions
  * @brief
  * @{
  */

/**
 * @brief       Add ANCS client.
 *
 * @param[in]   app_cb  Callback to notify client read/write/notify/indicate events.
 * @param[in]   link_num Initialize link number.
 * @return Client ID of the specific client module.
 * @retval 0xff Failed.
 * @retval other Success.
 *
 * <b>Example usage</b>
 * \code{.c}
    void test(void)
    {
        ancs_client = ancs_add_client(ancs_client_cb, link_num);
    }
 * \endcode
 */
T_CLIENT_ID ancs_add_client(P_FUN_GENERAL_APP_CB app_cb, uint8_t link_num);


/**
  * @brief  Used by application, to start the discovery procedure of ANCS.
  * @param[in]  conn_id Connection ID.
  * @retval true  Send request to Bluetooth Host success.
  * @retval false  Send request to Bluetooth Host failed.
  */
bool ancs_start_discovery(uint8_t conn_id);

/**
  * @brief  Used by application, to get handle cache.
  * @param[in]  conn_id Connection ID.
  * @param[in,out]  p_hdl_cache Pointer of the handle cache table.
  * @param[in]  len The length of handle cache table.
  * @retval true Success.
  * @retval false Failed.
  *
  * <b>Example usage</b>
  * \code{.c}
    void test(void)
    {
        uint16_t hdl_cache[HDL_ANCS_CACHE_LEN];
        bool ret = ancs_get_hdl_cache(conn_id, hdl_cache,
                                     sizeof(uint16_t) * HDL_ANCS_CACHE_LEN);
    }
  * \endcode
  */
bool ancs_get_hdl_cache(uint8_t conn_id, uint16_t *p_hdl_cache, uint8_t len);

/**
  * @brief  Used by application, to set handle cache.
  * @param[in]  conn_id Connection ID.
  * @param[in]  p_hdl_cache Pointer of the handle cache table.
  * @param[in]  len The length of handle cache table.
  * @retval true Success.
  * @retval false Failed.
  *
  * <b>Example usage</b>
  * \code{.c}
    void test(void)
    {
        ......
        if (app_srvs_table.srv_found_flags & APP_DISCOV_ANCS_FLAG)
        {
            ancs_set_hdl_cache(conn_id, app_srvs_table.ancs_hdl_cache, sizeof(uint16_t) * HDL_ANCS_CACHE_LEN);
        }
        ......
    }
  * \endcode
  */
bool ancs_set_hdl_cache(uint8_t conn_id, uint16_t *p_hdl_cache, uint8_t len);

/**
  * @brief  Used by application, to set the Client Characteristic Configuration of notification source.
  * @param[in]  conn_id Connection ID.
  * @param[in]  notify  Value to enable or disable notify.
  * @retval true Send request to Bluetooth Host success.
  * @retval false Send request to Bluetooth Host failed.
  */
bool ancs_set_notification_source_notify(uint8_t conn_id, bool notify);

/**
  * @brief  Used by application, to set the Client Characteristic Configuration of data source.
  * @param[in]  conn_id Connection ID.
  * @param[in]  notify  Value to enable or disable notify.
  * @retval true Send request to Bluetooth Host success.
  * @retval false Send request to Bluetooth Host failed.
  */
bool ancs_set_data_source_notify(uint8_t conn_id, bool notify);

/**
  * @brief  Used by application, to get the notification attribute.
  * @param[in]  conn_id           Connection ID.
  * @param[in]  notification_uid  Notification UID.
  * @param[in]  p_attribute_ids   Pointer to attribute ids.
  * @param[in]  attribute_ids_len Length of attribute ids.
  * @retval true Send request to Bluetooth Host success.
  * @retval false Send request to Bluetooth Host failed.
  */
bool ancs_get_notification_attr(uint8_t conn_id, uint32_t notification_uid,
                                uint8_t *p_attribute_ids, uint8_t attribute_ids_len);
/**
  * @brief  Used by application, to get the APP attribute.
  * @param[in]  conn_id           Connection ID.
  * @param[in]  p_app_identifier  Pointer to APP identifier.
  * @param[in]  p_attribute_ids   Pointer to attribute ids.
  * @param[in]  attribute_ids_len Length of attribute ids.
  * @retval true Send request to Bluetooth Host success.
  * @retval false Send request to Bluetooth Host failed.
  */
bool ancs_get_app_attr(uint8_t conn_id, char *p_app_identifier, uint8_t *p_attribute_ids,
                       uint8_t attribute_ids_len);

/**
  * @brief  Used by application, to perform the notification action.
  * @param[in]  conn_id          Connection ID.
  * @param[in]  notification_uid Notification UID.
  * @param[in]  action_id        Action ID.
  * @retval true Send request to Bluetooth Host success.
  * @retval false Send request to Bluetooth Host failed.
  */
bool ancs_perform_notification_action(uint8_t conn_id, uint32_t notification_uid,
                                      uint8_t action_id);

/**
  * @brief  Get the handle for GATT characteristic.
  * @param[in]  conn_id          Connection ID.
  * @param[in]  handle_type      Pre-defined Handle Type.
  * @return The handle value of a GATT characteristic.
  */
uint16_t ancs_search_handle(uint8_t conn_id, T_ANCS_HANDLE_TYPE handle_type);

/** @} End of ANCS_CLIENT_Exported_Functions */

/** @} End of ANCS_CLIENT */


#ifdef  __cplusplus
}
#endif      /*  __cplusplus */

#endif  /* _ANCS_CLIENT_H_ */
