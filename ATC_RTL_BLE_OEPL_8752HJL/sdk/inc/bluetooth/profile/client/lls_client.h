/**
*****************************************************************************************
*     Copyright(c) 2016, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
  * @file     lls_client.h
  * @brief    Header file for using LLS Client.
  * @details  LLS data structures and external functions declaration.
  * @author   Ken
  * @date     2017-12-05
  * @version  v0.1
  * *************************************************************************************
  */

/* Define to prevent recursive inclusion */
#ifndef _LLS_CLIENT_H_
#define _LLS_CLIENT_H_

#ifdef  __cplusplus
extern "C" {
#endif      /* __cplusplus */

/* Add Includes here */
#include <profile_client.h>


/** @defgroup LLS_Client Link Loss Service Client
  * @brief LLS client
  * @{
  */
/*============================================================================*
 *                         Macros
 *============================================================================*/
/** @defgroup LLS_Client_Exported_Macros LLS Client Exported Macros
  * @brief
  * @{
  */
/** @defgroup LLS_UUIDs LLS UUIDs
  * @brief LLS LE Profile UUID definitions
  * @{
  */
#define GATT_UUID_LINK_LOSS_SERVICE             0x1803
#define GATT_UUID_CHAR_ALERT_LEVEL              0x2A06
/** @} End of LLS_UUIDs */

/** @defgroup LLS_DEFs LLS Definitions
  * @{
  */
/** @brief  Define links number. */
#define LLS_MAX_LINKS  2
/** End of LLS_DEFs
  * @}
  */

/** End of LLS_Client_Exported_Macros
  * @}
  */


/*============================================================================*
 *                         Types
 *============================================================================*/
/** @defgroup LLS_Client_Exported_Types LLS Client Exported Types
  * @brief
  * @{
  */

/** @brief LLS client handle type*/
typedef enum
{
    HDL_LLS_SRV_START,           //!< Start handle of LLS service
    HDL_LLS_SRV_END,             //!< End handle of LLS service
    HDL_LLS_PARA,                //!< LLS parameter read/write characteristic value handle
    HDL_LLS_CACHE_LEN            //!< Handle cache length
} T_LLS_HANDLE_TYPE;

/** @brief LLS client discovery state*/
typedef enum
{
    DISC_LLS_IDLE,
    DISC_LLS_START,
    DISC_LLS_DONE,
    DISC_LLS_FAILED
} T_LLS_DISC_STATE;

/** @brief LLS client read type*/
typedef enum
{
    LLS_READ_PARA,
} T_LLS_READ_TYPE;

/** @brief LLS client read value*/
typedef struct
{
    uint16_t value_size;
    uint8_t *p_value;
} T_LLS_READ_VALUE;

/** @brief LLS client read data*/
typedef union
{
    T_LLS_READ_VALUE v1_read;
} T_LLS_READ_DATA;

/** @brief LLS client read result*/
typedef struct
{
    T_LLS_READ_TYPE type;
    T_LLS_READ_DATA data;
    uint16_t cause;
} T_LLS_READ_RESULT;


/** @brief LLS client write type*/
typedef enum
{
    LLS_WRITE_PARA,
} T_LLS_WRTIE_TYPE;

/** @brief LLS client write result*/
typedef struct
{
    T_LLS_WRTIE_TYPE type;
    uint16_t cause;
} T_LLS_WRITE_RESULT;

/** @brief LLS client callback type*/
typedef enum
{
    LLS_CLIENT_CB_TYPE_DISC_STATE,          //!< Discovery procedure state, done or pending.
    LLS_CLIENT_CB_TYPE_READ_RESULT,         //!< Read request's result data, responded from server.
    LLS_CLIENT_CB_TYPE_WRITE_RESULT,        //!< Write request result, success or fail.
    LLS_CLIENT_CB_TYPE_INVALID              //!< Invalid callback type, no practical usage.
} T_LLS_CLIENT_CB_TYPE;

/** @brief LLS client callback content*/
typedef union
{
    T_LLS_DISC_STATE      disc_state;
    T_LLS_READ_RESULT     read_result;
    T_LLS_WRITE_RESULT    write_result;
} T_LLS_CLIENT_CB_CONTENT;

/** @brief LLS client callback data*/
typedef struct
{
    T_LLS_CLIENT_CB_TYPE     cb_type;
    T_LLS_CLIENT_CB_CONTENT  cb_content;
} T_LLS_CLIENT_CB_DATA;

/** End of LLS_Client_Exported_Types * @} */


/*============================================================================*
 *                         Functions
 *============================================================================*/
/** @defgroup LLS_Client_Exported_Functions LLS Client Exported Functions
  * @{
  */

/**
  * @brief      Add LLS service client to application.
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
        lls_client_id = lls_add_client(app_client_callback, APP_MAX_LINKS);
    }
  * \endcode
  */
T_CLIENT_ID lls_add_client(P_FUN_GENERAL_APP_CB app_cb, uint8_t link_num);

/**
  * @brief  Used by application, to start the discovery procedure of LLS server.
  * @param[in]  conn_id Connection ID.
  * @retval true Send request to Bluetooth Host success.
  * @retval false Send request to Bluetooth Host failed.
  */
bool lls_client_start_discovery(uint8_t conn_id);

/**
  * @brief  Used by application, to read data from server by using handles.
  * @param[in]  conn_id Connection ID.
  * @param[in]  read_type One of characteristic that has the readable property.
  * @retval true Send request to Bluetooth Host success.
  * @retval false Send request to Bluetooth Host failed.
  */
bool lls_client_read_by_handle(uint8_t conn_id, T_LLS_READ_TYPE read_type);

/**
  * @brief  Used by application, to read data from server by using UUIDs.
  * @param[in]  conn_id Connection ID.
  * @param[in]  read_type One of characteristic that has the readable property.
  * @retval true Send request to Bluetooth Host success.
  * @retval false Send request to Bluetooth Host failed.
  */
bool lls_client_read_by_uuid(uint8_t conn_id, T_LLS_READ_TYPE read_type);

/**
  * @brief  Used by application, to write data of write characteristic.
  * @param[in]  conn_id Connection ID.
  * @param[in]  length  Write data length.
  * @param[in]  p_value Point the value to write.
  * @param[in]  type    Write type.
  * @retval true Send request to Bluetooth Host success.
  * @retval false Send request to Bluetooth Host failed.
  */
bool lls_client_write_char(uint8_t conn_id, uint16_t length, uint8_t *p_value,
                           T_GATT_WRITE_TYPE type);

/**
  * @brief  Used by the application to get handle cache.
  * @param[in]  conn_id Connection ID.
  * @param[in,out]  p_hdl_cache Pointer to the handle cache table.
  * @param[in]  len Length of the handle cache table.
  * @retval true Success.
  * @retval false Failed.
  */
bool lls_client_get_hdl_cache(uint8_t conn_id, uint16_t *p_hdl_cache, uint8_t len);

/**
  * @brief  Used by the application to set handle cache.
  * @param[in]  conn_id Connection ID.
  * @param[in]  p_hdl_cache Pointer to the handle cache table.
  * @param[in]  len Length of the handle cache table.
  * @retval true Success.
  * @retval false Failed.
  */
bool lls_client_set_hdl_cache(uint8_t conn_id, uint16_t *p_hdl_cache, uint8_t len);

/** @} End of LLS_Client_Exported_Functions */

/** @} End of LLS_Client */


#ifdef  __cplusplus
}
#endif      /*  __cplusplus */

#endif  /* _LLS_CLIENT_H_ */
