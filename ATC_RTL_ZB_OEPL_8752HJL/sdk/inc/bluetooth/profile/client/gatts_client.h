/**
*****************************************************************************************
*     Copyright(c) 2019, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
  * @file     gatts_client.h
  * @brief    Header file for using GATT service client.
  * @details  GATT service client data structs and external functions declaration.
  * @author
  * @date     2019-05-27
  * @version  v0.1
  * *************************************************************************************
  */

/* Define to prevent recursive inclusion */
#ifndef _GATTS_CLIENT_H_
#define _GATTS_CLIENT_H_

#ifdef  __cplusplus
extern "C" {
#endif      /* __cplusplus */

/* Add Includes here */
#include <profile_client.h>

/** @defgroup GATTS_Client GATT Service Client
  * @brief GATT service client
  * @details
     Applications shall register GATTS client during initialization through @ref gatts_add_client function.

     Applications can start discovering GATT service through @ref gatts_start_discovery function.

     Applications shall handle callback function registered by @ref gatts_add_client.
  * \code{.c}
    T_APP_RESULT app_client_callback(T_CLIENT_ID client_id, uint8_t conn_id, void *p_data)
    {
        if (client_id == gatts_client_id)
        {
            T_GATTS_CLIENT_CB_DATA *p_gatts_cb_data = (T_GATTS_CLIENT_CB_DATA *)p_data;
            switch (p_gatts_cb_data->cb_type)
            {
            case GATTS_CLIENT_CB_TYPE_DISC_STATE:
                switch (p_gatts_cb_data->cb_content.disc_state)
                {
                case DISC_GATTS_DONE:
                ......
        }
    }
  * \endcode
  * @{
  */

/** @defgroup GATTS_Client_Exported_Macros GATTS Client Exported Macros
  * @brief
  * @{
  */
/** @brief  Define links number. */
#define GATTS_MAX_LINKS  4

/** @brief  GATT service UUID */
#define GATT_UUID_GATT                                   0x1801
/** End of GATTS_Client_Exported_Macros * @} */

/** @defgroup GATTS_Client_Exported_Types GATTS Client Exported Types
  * @brief
  * @{
  */
/** @brief GATTS client handle type */
typedef enum
{
    HDL_GATTS_SRV_START,                   //!< Start handle of GAP service.
    HDL_GATTS_SRV_END,                     //!< End handle of GAP service.
    HDL_GATTS_SERVICE_CHANGED,             //!< Service changed handle.
    HDL_GATTS_SERVICE_CHANGED_CCCD,        //!< Service changed CCCD handle.
    HDL_GATTS_CLIENT_SUPPORTED_FEATURES,   //!< Client Supported Features handle.
    HDL_GATTS_DATABASE_HASH,               //!< Database Hash handle.
    HDL_GATTS_CACHE_LEN,                   //!< Handle cache length.
} T_GATTS_CLIENT_HANDLE_TYPE;

/** @brief GATTS client discovery state */
typedef enum
{
    DISC_GATTS_IDLE,
    DISC_GATTS_START,
    DISC_GATTS_DONE,
    DISC_GATTS_FAILED,
} T_GATTS_DISC_STATE;

/** @brief GATTS client read type */
typedef enum
{
    GATTS_READ_CLIENT_SUPPORTED_FEATURES,
    GATTS_READ_DATABASE_HASH,
} T_GATTS_READ_TYPE;

/** @brief Client Supported Features value */
typedef struct
{
    uint16_t value_size;
    uint8_t *p_value;
} T_CLIENT_SUPPORTED_FEATURES;

/** @brief GATTS client read content */
typedef union
{
    T_CLIENT_SUPPORTED_FEATURES client_supported_features;
    uint8_t database_hash[GATTS_DATABASE_HASH_LEN];
} T_GATTS_READ_DATA;

/** @brief GATTS client read data, used to inform APP read response data */
typedef struct
{
    T_GATTS_READ_TYPE type;
    T_GATTS_READ_DATA data;
    uint16_t cause;
} T_GATTS_READ_RESULT;

/** @brief GATTS client write type*/
typedef enum
{
    GATTS_WRITE_SERVICE_CHANGED_IND_ENABLE,
    GATTS_WRITE_SERVICE_CHANGED_IND_DISABLE,
    GATTS_WRITE_CLIENT_SUPPORTED_FEATURES,
} T_GATTS_WRTIE_TYPE;

/** @brief GATTS client write result*/
typedef struct
{
    T_GATTS_WRTIE_TYPE type;
    uint16_t cause;
} T_GATTS_WRITE_RESULT;

/** @brief GATTS client notification/indication receive type*/
typedef enum
{
    GATTS_SERVICE_CHANGED_INDICATE,
} T_GATTS_NOTIF_IND_TYPE;

/** @brief GATTS client notification/indication receive data */
typedef struct
{
    uint16_t value_size;
    uint8_t *p_value;
} T_GATTS_NOTIF_IND_VALUE;

/** @brief GATTS client notification/indication receive content*/
typedef struct
{
    T_GATTS_NOTIF_IND_TYPE type;
    T_GATTS_NOTIF_IND_VALUE data;
} T_GATTS_NOTIF_IND_DATA;

/** @brief GATTS client callback type */
typedef enum
{
    GATTS_CLIENT_CB_TYPE_DISC_STATE,          //!< Discovery procedure state, done or pending.
    GATTS_CLIENT_CB_TYPE_READ_RESULT,         //!< Read request's result data, responded from server.
    GATTS_CLIENT_CB_TYPE_WRITE_RESULT,        //!< Write result, success or fail.
    GATTS_CLIENT_CB_TYPE_NOTIF_IND_RESULT,    //!< Notification or indication data received from server.
    GATTS_CLIENT_CB_TYPE_INVALID,             //!< Invalid callback type, no practical usage.
} T_GATTS_CLIENT_CB_TYPE;

/** @brief GATTS client callback content */
typedef union
{
    T_GATTS_DISC_STATE     disc_state;
    T_GATTS_READ_RESULT     read_result;
    T_GATTS_WRITE_RESULT    write_result;
    T_GATTS_NOTIF_IND_DATA  notif_ind_data;
} T_GATTS_CLIENT_CB_CONTENT;

/** @brief GATTS client callback data */
typedef struct
{
    T_GATTS_CLIENT_CB_TYPE     cb_type;
    T_GATTS_CLIENT_CB_CONTENT  cb_content;
} T_GATTS_CLIENT_CB_DATA;

/** End of GATTS_Client_Exported_Types * @} */

/** @defgroup GATTS_Client_Exported_Functions GATTS Client Exported Functions
  * @brief
  * @{
  */
/**
  * @brief  Add GATTS service client to application.
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
        gatts_client_id  = gatts_add_client(app_client_callback, APP_MAX_LINKS);
    }
 * \endcode
  */
T_CLIENT_ID gatts_add_client(P_FUN_GENERAL_APP_CB app_cb, uint8_t link_num);

/**
  * @brief  Used by application to start the discovery procedure of GATT service.
  * @param[in]  conn_id Connection ID.
  * @retval true Send request to Bluetooth Host success.
  * @retval false Send request to Bluetooth Host failed.
  *
  * <b>Example usage</b>
  * \code{.c}
    void test(void)
    {
        bool ret = gatts_start_discovery(conn_id);
    }
 * \endcode
  */
bool gatts_start_discovery(uint8_t conn_id);

/**
  * @brief  Used by application to read data from server by using handles.
  * @param[in]  conn_id Connection ID.
  * @param[in]  read_type One of the characteristics that has the readable property.
  * @retval true Send request to Bluetooth Host success.
  * @retval false Send request to Bluetooth Host failed.
  *
  * <b>Example usage</b>
  * \code{.c}
    void test(void)
    {
        bool ret = gatts_read(conn_id, read_type);
    }
 * \endcode
  */
bool gatts_read(uint8_t conn_id, T_GATTS_READ_TYPE read_type);

/**
  * @brief  Used by application to set the Client Characteristic Configuration of service changed characteristic.
  * @param[in]  conn_id Connection ID.
  * @param[in]  ind     Value to enable or disable indication.
  * @retval true Send request to Bluetooth Host success.
  * @retval false Send request to Bluetooth Host failed.
  *
  * <b>Example usage</b>
  * \code{.c}
    void test(void)
    {
        bool ret = gatts_client_set_service_changed_ind(conn_id, ind);
    }
  * \endcode
  */
bool gatts_client_set_service_changed_ind(uint8_t conn_id, bool ind);

/**
  * @brief  Used by application to set the client supported features characteristic.
  * @param[in]  conn_id               Connection ID.
  * @param[in]  len                   Length of client supported features, @ref GATTS_CLIENT_SUPPORTED_FEATURES_LEN.
  * @param[in]  p_client_supp_feats   Point to value that informs the server which features are supported by the client.
  * @retval true Send request to Bluetooth Host success.
  * @retval false Send request to Bluetooth Host failed.
  *
  * <b>Example usage</b>
  * \code{.c}
    void test(void)
    {
        bool ret = gatts_client_set_client_supported_features(conn_id, len, client_supported_features);
    }
  * \endcode
  */
bool gatts_client_set_client_supported_features(uint8_t conn_id, uint16_t len,
                                                uint8_t *p_client_supp_feats);

/**
  * @brief  Used by application to get handle cache.
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
        uint16_t hdl_cache[HDL_GATTS_CACHE_LEN];
        bool ret = gatts_get_hdl_cache(conn_id, hdl_cache, sizeof(uint16_t) * HDL_GATTS_CACHE_LEN);
    }
 * \endcode
  */
bool gatts_get_hdl_cache(uint8_t conn_id, uint16_t *p_hdl_cache, uint8_t len);

/**
  * @brief  Used by application, to set handle cache.
  * @param[in]  conn_id Connection ID.
  * @param[in]  p_hdl_cache Pointer to the handle cache table.
  * @param[in]  len The length of the handle cache table.
  * @retval true Success.
  * @retval false Failed.
  *
  * <b>Example usage</b>
  * \code{.c}
    void test(void)
    {
        ......
        if (app_srvs_table.srv_found_flags & APP_DISCOV_GATTS_FLAG)
        {
            gatts_set_hdl_cache(conn_id, app_srvs_table.gatts_hdl_cache, sizeof(uint16_t) * HDL_GATTS_CACHE_LEN);
        }
        ......
    }
 * \endcode
  */
bool gatts_set_hdl_cache(uint8_t conn_id, uint16_t *p_hdl_cache, uint8_t len);

/** @} End of GATTS_Client_Exported_Functions */

/** @} End of GATTS_Client */


#ifdef  __cplusplus
}
#endif      /*  __cplusplus */

#endif  /* _GATTS_CLIENT_H_ */
