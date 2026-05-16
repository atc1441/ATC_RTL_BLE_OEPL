/**
*****************************************************************************************
*     Copyright(c) 2016, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
  * @file     ias_client.h
  * @brief    Header file for using IAS LE Client.
  * @details  IAS data structures and external functions declaration.
  * @author   ken
  * @date     2017-12-04
  * @version  v0.1
  * *************************************************************************************
  */

/* Define to prevent recursive inclusion */
#ifndef _IAS_CLIENT_H_
#define _IAS_CLIENT_H_

#ifdef  __cplusplus
extern "C" {
#endif      /* __cplusplus */

/* Add Includes here */
#include <profile_client.h>


/** @defgroup IAS_Client Immediate Alert Service Client
  * @brief IAS client
  * @{
  */
/*============================================================================*
 *                         Macros
 *============================================================================*/
/** @defgroup IAS_Client_Exported_Macros IAS Client Exported Macros
  * @brief
  * @{
  */
/** @defgroup IAS_UUIDs IAS UUIDs
  * @brief IAS LE Profile UUID definitions
  * @{
  */

#define GATT_UUID_IMMEDIATE_ALERT_SERVICE       0x1802
#define GATT_UUID_CHAR_ALERT_LEVEL              0x2A06

/** @} End of IAS_UUIDs */


/** @defgroup IAS_DEFs IAS Definitions
  * @{
  */
/** @brief  Define links number. */
#define IAS_MAX_LINKS  2
/** End of IAS_DEFs
  * @}
  */

/** End of IAS_Client_Exported_Macros
  * @}
  */


/*============================================================================*
 *                         Types
 *============================================================================*/
/** @defgroup IAS_Client_Exported_Types IAS Client Exported Types
  * @brief
  * @{
  */

/** @brief IAS client handle type*/
typedef enum
{
    HDL_IAS_SRV_START,           //!< Start handle of simple LE service
    HDL_IAS_SRV_END,             //!< End handle of simple LE service
    HDL_IAS_WRITE,            //!< V2 write characteristic value handle
    HDL_IAS_CACHE_LEN            //!< Handle cache length
} T_IAS_HANDLE_TYPE;

/** @brief IAS client discovery state*/
typedef enum
{
    DISC_IAS_IDLE,
    DISC_IAS_START,
    DISC_IAS_DONE,
    DISC_IAS_FAILED
} T_IAS_DISC_STATE;

/** @brief IAS client write type*/
typedef enum
{
    IAS_WRITE_ALERT,
} T_IAS_WRTIE_TYPE;

/** @brief IAS client write result*/
typedef struct
{
    T_IAS_WRTIE_TYPE type;
    uint16_t cause;
} T_IAS_WRITE_RESULT;

/** @brief IAS client callback type*/
typedef enum
{
    IAS_CLIENT_CB_TYPE_DISC_STATE,          //!< Discovery procedure state, done or pending.
    IAS_CLIENT_CB_TYPE_WRITE_RESULT,        //!< Write request result, success or fail.
    IAS_CLIENT_CB_TYPE_INVALID              //!< Invalid callback type, no practical usage.
} T_IAS_CLIENT_CB_TYPE;

/** @brief IAS client callback content*/
typedef union
{
    T_IAS_DISC_STATE      disc_state;
    T_IAS_WRITE_RESULT    write_result;
} T_IAS_CLIENT_CB_CONTENT;

/** @brief IAS client callback data*/
typedef struct
{
    T_IAS_CLIENT_CB_TYPE     cb_type;
    T_IAS_CLIENT_CB_CONTENT  cb_content;
} T_IAS_CLIENT_CB_DATA;

/** End of IAS_Client_Exported_Types * @} */

/** @defgroup IAS_Client_Exported_Functions IAS Client Exported Functions
  * @{
  */

/**
  * @brief      Add IAS service client to application.
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
        ias_client_id = ias_add_client(app_client_callback, APP_MAX_LINKS);
    }
  * \endcode
  */
T_CLIENT_ID ias_add_client(P_FUN_GENERAL_APP_CB app_cb, uint8_t link_num);

/**
  * @brief  Used by application, to start the discovery procedure of IAS server.
  * @param[in]  conn_id Connection ID.
  * @retval true Send request to Bluetooth Host success.
  * @retval false Send request to Bluetooth Host failed.
  */
bool ias_client_start_discovery(uint8_t conn_id);

/**
  * @brief  Used by application, to write data of characteristic.
  * @param[in]  conn_id Connection ID.
  * @param[in]  length  Write data length.
  * @param[in]  p_value Point the value to write.
  * @param[in]  type    Write type.
  * @retval true Send request to Bluetooth Host success.
  * @retval false Send request to Bluetooth Host failed.
  */
bool ias_client_write_char(uint8_t conn_id, uint16_t length, uint8_t *p_value,
                           T_GATT_WRITE_TYPE type);

/**
  * @brief  Used by application to get handle cache.
  * @param[in]  conn_id Connection ID.
  * @param[in,out]  p_hdl_cache Pointer of the handle cache table.
  * @param[in]  len Length of handle cache table.
  * @retval true Success.
  * @retval false Failed.
  */
bool ias_client_get_hdl_cache(uint8_t conn_id, uint16_t *p_hdl_cache, uint8_t len);

/**
  * @brief  Used by application to set handle cache.
  * @param[in]  conn_id Connection ID.
  * @param[in]  p_hdl_cache Pointer of the handle cache table.
  * @param[in]  len Length of handle cache table.
  * @retval true Success.
  * @retval false Failed.
  */
bool ias_client_set_hdl_cache(uint8_t conn_id, uint16_t *p_hdl_cache, uint8_t len);

/** @} End of IAS_Client_Exported_Functions */

/** @} End of IAS_Client */


#ifdef  __cplusplus
}
#endif      /*  __cplusplus */

#endif  /* _IAS_CLIENT_H_ */
