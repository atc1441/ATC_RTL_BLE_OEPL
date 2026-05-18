/**
*****************************************************************************************
*     Copyright(c) 2016, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
  * @file     simple_ble_client.h
  * @brief    Header file for using Simple Bluetooth LE Client.
  * @details  Simple Bluetooth LE data structs and external functions declaration.
  * @author   jane
  * @date     2016-02-18
  * @version  v0.1
  * *************************************************************************************
  */

/* Define to prevent recursive inclusion */
#ifndef _SIMPLE_BLE_CLIENT_H_
#define _SIMPLE_BLE_CLIENT_H_

#ifdef  __cplusplus
extern "C" {
#endif      /* __cplusplus */

/* Add Includes here */
#include <profile_client.h>
#include <simple_ble_config.h>


/** @defgroup SIMP_Client Simple Bluetooth LE Service Client
  * @brief SIMP client
  * @{
  */
/*============================================================================*
 *                         Macros
 *============================================================================*/
/** @defgroup SIMP_Client_Exported_Macros SIMP Client Exported Macros
  * @brief
  * @{
  */

/** @brief  Define links number. */
#define SIMP_MAX_LINKS  4
/** End of SIMP_Client_Exported_Macros
  * @}
  */


/*============================================================================*
 *                         Types
 *============================================================================*/
/** @defgroup SIMP_Client_Exported_Types SIMP Client Exported Types
  * @brief
  * @{
  */

/** @brief SIMP client handle type*/
typedef enum
{
    HDL_SIMBLE_SRV_START,           //!< Start handle of Simple Bluetooth LE service.
    HDL_SIMBLE_SRV_END,             //!< End handle of Simple Bluetooth LE service.
    HDL_SIMBLE_V1_READ,             //!< V1 read characteristic value handle.
    HDL_SIMBLE_V2_WRITE,            //!< V2 write characteristic value handle.
    HDL_SIMBLE_V3_NOTIFY,           //!< V3 notify characteristic value handle.
    HDL_SIMBLE_V3_NOTIFY_CCCD,      //!< V3 notify characteristic CCCD handle.
    HDL_SIMBLE_V4_INDICATE,         //!< V4 indicate characteristic value handle.
    HDL_SIMBLE_V4_INDICATE_CCCD,    //!< V4 indicate characteristic CCCD handle.
    HDL_SIMBLE_CACHE_LEN            //!< Handle cache length.
} T_SIMP_HANDLE_TYPE;

/** @brief SIMP client discovery state*/
typedef enum
{
    DISC_SIMP_IDLE,
    DISC_SIMP_START,
    DISC_SIMP_DONE,
    DISC_SIMP_FAILED
} T_SIMP_DISC_STATE;

/** @brief SIMP client read type*/
typedef enum
{
    SIMP_READ_V1_READ,
    SIMP_READ_V3_NOTIFY_CCCD,
    SIMP_READ_V4_INDICATE_CCCD,
} T_SIMP_READ_TYPE;

/** @brief SIMP client read value*/
typedef struct
{
    uint16_t value_size;
    uint8_t *p_value;
} T_SIMP_READ_VALUE;

/** @brief SIMP client read data*/
typedef union
{
    T_SIMP_READ_VALUE v1_read;
    bool v3_notify_cccd;
    bool v4_indicate_cccd;
} T_SIMP_READ_DATA;

/** @brief SIMP client read result*/
typedef struct
{
    T_SIMP_READ_TYPE type;
    T_SIMP_READ_DATA data;
    uint16_t cause;
} T_SIMP_READ_RESULT;


/** @brief SIMP client write type*/
typedef enum
{
    SIMP_WRITE_V2_WRITE,
    SIMP_WRITE_V3_NOTIFY_CCCD,
    SIMP_WRITE_V4_INDICATE_CCCD,
} T_SIMP_WRTIE_TYPE;

/** @brief SIMP client write result*/
typedef struct
{
    T_SIMP_WRTIE_TYPE type;
    uint16_t cause;
} T_SIMP_WRITE_RESULT;

/** @brief SIMP client notification/indication receive type*/
typedef enum
{
    SIMP_V3_NOTIFY,
    SIMP_V4_INDICATE,
} T_SIMP_NOTIF_IND_TYPE;

/** @brief SIMP client notification/indication receive data*/
typedef struct
{
    uint16_t value_size;
    uint8_t *p_value;
} T_SIMP_NOTIF_IND_VALUE;

/** @brief SIMP client notification/indication receive content*/
typedef struct
{
    T_SIMP_NOTIF_IND_TYPE type;
    T_SIMP_NOTIF_IND_VALUE data;
} T_SIMP_NOTIF_IND_DATA;

/** @brief SIMP client callback type*/
typedef enum
{
    SIMP_CLIENT_CB_TYPE_DISC_STATE,          //!< Discovery procedure state, done or pending.
    SIMP_CLIENT_CB_TYPE_READ_RESULT,         //!< Read request's result data, responded from server.
    SIMP_CLIENT_CB_TYPE_WRITE_RESULT,        //!< Write result, success or fail.
    SIMP_CLIENT_CB_TYPE_NOTIF_IND_RESULT,    //!< Notification or indication data received from server.
    SIMP_CLIENT_CB_TYPE_INVALID              //!< Invalid callback type, no practical usage.
} T_SIMP_CLIENT_CB_TYPE;

/** @brief SIMP client callback content*/
typedef union
{
    T_SIMP_DISC_STATE      disc_state;
    T_SIMP_READ_RESULT     read_result;
    T_SIMP_WRITE_RESULT    write_result;
    T_SIMP_NOTIF_IND_DATA  notif_ind_data;
} T_SIMP_CLIENT_CB_CONTENT;

/** @brief SIMP client callback data*/
typedef struct
{
    T_SIMP_CLIENT_CB_TYPE     cb_type;
    T_SIMP_CLIENT_CB_CONTENT  cb_content;
} T_SIMP_CLIENT_CB_DATA;

/** End of SIMP_Client_Exported_Types * @} */

/** @defgroup SIMP_Client_Exported_Functions SIMP Client Exported Functions
  * @{
  */

/**
  * @brief      Add Simple Bluetooth LE service client to application.
  * @param[in]  app_cb Pointer of APP callback function to handle specific client module data.
  * @param[in]  link_num Initialize link num.
  * @return Client ID of the specific client module.
  * @retval 0xff Failed.
  * @retval other Success.
  *
  * <b>Example usage</b>
  * \code{.c}
    void app_le_profile_init(void)
    {
        client_init(client_num);
        simple_ble_client_id = simp_ble_add_client(app_client_callback, APP_MAX_LINKS);
    }
  * \endcode
  */
T_CLIENT_ID simp_ble_add_client(P_FUN_GENERAL_APP_CB app_cb, uint8_t link_num);

/**
  * @brief  Used by application, to start the discovery procedure of Simple Bluetooth LE server.
  * @param[in]  conn_id Connection ID.
  * @retval true Send request to Bluetooth Host success.
  * @retval false Send request to Bluetooth Host failed.
  */
bool simp_ble_client_start_discovery(uint8_t conn_id);

/**
  * @brief  Used by application, to read data from server by using handles.
  * @param[in]  conn_id Connection ID.
  * @param[in]  read_type One of characteristic that has the readable property.
  * @retval true Send request to Bluetooth Host success.
  * @retval false Send request to Bluetooth Host failed.
  */
bool simp_ble_client_read_by_handle(uint8_t conn_id, T_SIMP_READ_TYPE read_type);

/**
  * @brief  Used by application, to read data from server by using UUIDs.
  * @param[in]  conn_id Connection ID.
  * @param[in]  read_type One of characteristic that has the readable property.
  * @retval true Send request to Bluetooth Host success.
  * @retval false Send request to Bluetooth Host failed.
  */
bool simp_ble_client_read_by_uuid(uint8_t conn_id, T_SIMP_READ_TYPE read_type);

/**
  * @brief  Used by application, to write data of V2 write Characteristic.
  * @param[in]  conn_id Connection ID.
  * @param[in]  length  Write data length.
  * @param[in]  p_value Point the value to write.
  * @param[in]  type    Write type.
  * @retval true Send request to Bluetooth Host success.
  * @retval false Send request to Bluetooth Host failed.
  */
bool simp_ble_client_write_v2_char(uint8_t conn_id, uint16_t length, uint8_t *p_value,
                                   T_GATT_WRITE_TYPE type);

/**
  * @brief  Used by application, to enable or disable the notification of peer server's V3 Notify Characteristic.
  * @param[in]  conn_id Connection ID.
  * @param[in]  notify @arg 0 Disable the notification.
                       @arg 1 Enable the notification.
  * @retval true Send request to Bluetooth Host success.
  * @retval false Send request to Bluetooth Host failed.
  */
bool simp_ble_client_set_v3_notify(uint8_t conn_id, bool notify);

/**
  * @brief  Used by application, to enable or disable the indication of peer server's V4 Indicate Characteristic.
  * @param[in]  conn_id Connection ID.
  * @param[in]  ind @arg 0 Disable the indication.
                    @arg 1 Enable the indication.
  * @retval true Send request to Bluetooth Host success.
  * @retval false Send request to Bluetooth Host failed.
  */
bool simp_ble_client_set_v4_ind(uint8_t conn_id, bool ind);

/**
  * @brief  Used by the application to get the handle cache.
  * @param[in]  conn_id Connection ID.
  * @param[in,out]  p_hdl_cache Pointer to the handle cache table.
  * @param[in]  len The length of the handle cache table.
  * @retval true Success.
  * @retval false Failed.
  */
bool simp_ble_client_get_hdl_cache(uint8_t conn_id, uint16_t *p_hdl_cache, uint8_t len);

/**
  * @brief  Used by the application to set the handle cache.
  * @param[in]  conn_id Connection ID.
  * @param[in]  p_hdl_cache Pointer to the handle cache table.
  * @param[in]  len The length of the handle cache table.
  * @retval true Success.
  * @retval false Failed.
  */
bool simp_ble_client_set_hdl_cache(uint8_t conn_id, uint16_t *p_hdl_cache, uint8_t len);

/** @} End of SIMP_Client_Exported_Functions */

/** @} End of SIMP_Client */


#ifdef  __cplusplus
}
#endif      /*  __cplusplus */

#endif  /* _SIMPLE_BLE_CLIENT_H_ */
