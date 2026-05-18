/**
*****************************************************************************************
*     Copyright(c) 2016, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
  * @file     gaps_client.h
  * @brief    Header file for using GAP service client.
  * @details  GAP service client data structs and external functions declaration.
  * @author   jane
  * @date     2016-02-18
  * @version  v1.0
  * *************************************************************************************
  */

/* Define to prevent recursive inclusion */
#ifndef _GAPS_CLIENT_H_
#define _GAPS_CLIENT_H_

#ifdef  __cplusplus
extern "C" {
#endif      /* __cplusplus */

/* Add Includes here */
#include <profile_client.h>



/** @defgroup GAPS_Client GAP Service Client
  * @brief GAP service client
  * @details
     Applications shall register GAPS client during initialization through @ref gaps_add_client function.

     Applications can start discovering GAP service through @ref gaps_start_discovery function.

     Applications can read GAP service referenced values through @ref gaps_read function.

     Applications shall handle callback function registered by @ref gaps_add_client.
  * \code{.c}
    T_APP_RESULT app_client_callback(T_CLIENT_ID client_id, uint8_t conn_id, void *p_data)
    {
        if (client_id == gaps_client_id)
        {
            T_GAPS_CLIENT_CB_DATA *p_gaps_cb_data = (T_GAPS_CLIENT_CB_DATA *)p_data;
            switch (p_gaps_cb_data->cb_type)
            {
            case GAPS_CLIENT_CB_TYPE_DISC_STATE:
                switch (p_gaps_cb_data->cb_content.disc_state)
                {
                case DISC_GAPS_DONE:
                ......
        }
    }
  * \endcode
  * @{
  */

/** @defgroup GAPS_Client_Exported_Macros GAPS Client Exported Macros
  * @brief
  * @{
  */
/** @brief  Define links number. */
#define GAPS_MAX_LINKS  4

/** @brief  GAP service UUID */
#define GATT_UUID_GAP                                   0x1800
/** @brief  Resolvable private address only characteristic UUID */
#define GATT_UUID_CHAR_RESOLVABLE_PRIVATE_ADDRESS_ONLY  0x2AC9
/** End of GAPS_Client_Exported_Macros * @} */


/** @defgroup GAPS_Client_Exported_Types GAPS Client Exported Types
  * @brief
  * @{
  */
/** @brief GAPS client handle type */
typedef enum
{
    HDL_GAPS_SRV_START,                   //!< Start handle of GAP service.
    HDL_GAPS_SRV_END,                     //!< End handle of GAP service.
    HDL_GAPS_DEVICE_NAME,                 //!< Device name value handle.
    HDL_GAPS_APPEARANCE,                  //!< Appearance value handle.
    HDL_GAPS_CENTRAL_ADDR_RESOLUTION,     //!< Central address resolution value handle.
    HDL_GAPS_RESOLVABLE_PRIVATE_ADDR_ONLY,//!< Resolvable private address only value handle.
    HDL_GAPS_CACHE_LEN                    //!< Handle cache length.
} T_GAP_CLIENT_HANDLE_TYPE;


/** @brief GAPS client discovery state */
typedef enum
{
    DISC_GAPS_IDLE,
    DISC_GAPS_START,
    DISC_GAPS_DONE,
    DISC_GAPS_FAILED
} T_GAPS_DISC_STATE;


/** @brief GAPS client read type */
typedef enum
{
    GAPS_READ_DEVICE_NAME,
    GAPS_READ_APPEARANCE,
    GAPS_READ_CENTRAL_ADDR_RESOLUTION,
} T_GAPS_READ_TYPE;

/** @brief Device name value */
typedef struct
{
    uint16_t value_size;
    uint8_t *p_value;
} T_DEVICE_NAME;

/** @brief GAPS client read content */
typedef union
{
    uint16_t appearance;
    T_DEVICE_NAME device_name;
    uint8_t central_addr_res;
} T_GAPS_READ_DATA;

/** @brief GAPS client read data, used to inform APP read response data */
typedef struct
{
    T_GAPS_READ_TYPE type;
    T_GAPS_READ_DATA data;
    uint16_t cause;
} T_GAPS_READ_RESULT;


/** @brief GAPS client callback type */
typedef enum
{
    GAPS_CLIENT_CB_TYPE_DISC_STATE,          //!< Discovery procedure state, done or pending.
    GAPS_CLIENT_CB_TYPE_READ_RESULT,         //!< Read request's result data, responded from server.
    GAPS_CLIENT_CB_TYPE_INVALID              //!< Invalid callback type, no practical usage.
} T_GAPS_CLIENT_CB_TYPE;

/** @brief GAPS client callback content */
typedef union
{
    T_GAPS_DISC_STATE disc_state;
    T_GAPS_READ_RESULT read_result;
} T_GAPS_CLIENT_CB_CONTENT;


/** @brief GAPS client callback data */
typedef struct
{
    T_GAPS_CLIENT_CB_TYPE     cb_type;
    T_GAPS_CLIENT_CB_CONTENT  cb_content;
} T_GAPS_CLIENT_CB_DATA;

/** End of GAPS_Client_Exported_Types * @} */

/** @defgroup GAPS_Client_Exported_Functions GAPS Client Exported Functions
  * @brief
  * @{
  */
/**
  * @brief  Add GAP service client to application.
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
        gaps_client_id  = gaps_add_client(app_client_callback, APP_MAX_LINKS);
    }
 * \endcode
  */
T_CLIENT_ID gaps_add_client(P_FUN_GENERAL_APP_CB app_cb, uint8_t link_num);

/**
  * @brief  Used by application, to start the discovery procedure of GAP service.
  * @param[in]  conn_id Connection ID.
  * @retval true Send request to Bluetooth Host success.
  * @retval false Send request to Bluetooth Host failed.
  *
  * <b>Example usage</b>
  * \code{.c}
    void test(void)
    {
        bool ret = gaps_start_discovery(conn_id);
    }
 * \endcode
  */
bool gaps_start_discovery(uint8_t conn_id);

/**
  * @brief  Used by application, to read data from server by using handles.
  * @param[in]  conn_id Connection ID.
  * @param[in]  read_type One of characteristic that has the readable property.
  * @retval true Send request to Bluetooth Host success.
  * @retval false Send request to Bluetooth Host failed.
  *
  * <b>Example usage</b>
  * \code{.c}
    void test(void)
    {
        bool ret = gaps_read(conn_id, read_type);
    }
 * \endcode
  */
bool gaps_read(uint8_t conn_id, T_GAPS_READ_TYPE read_type);

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
        uint16_t hdl_cache[HDL_GAPS_CACHE_LEN];
        bool ret = gaps_get_hdl_cache(conn_id, hdl_cache, sizeof(uint16_t) * HDL_GAPS_CACHE_LEN);
    }
 * \endcode
  */
bool gaps_get_hdl_cache(uint8_t conn_id, uint16_t *p_hdl_cache, uint8_t len);

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
        if (app_srvs_table.srv_found_flags & APP_DISCOV_GAPS_FLAG)
        {
            gaps_set_hdl_cache(conn_id, app_srvs_table.gaps_hdl_cache, sizeof(uint16_t) * HDL_GAPS_CACHE_LEN);
        }
        ......
    }
 * \endcode
  */
bool gaps_set_hdl_cache(uint8_t conn_id, uint16_t *p_hdl_cache, uint8_t len);

/**
  * @brief  Used by application, to check Resolvable Private Address Only Characteristics whether existing.
  * @param[in]  conn_id Connection ID.
  * @param[in,out]  p_is_exist Whether existing.
  * @retval true Success.
  * @retval false Failed.
  */
bool gaps_check_resolvable_private_addr_only_char(uint8_t conn_id, bool *p_is_exist);

/** @} End of GAPS_Client_Exported_Functions */

/** @} End of GAPS_Client */


#ifdef  __cplusplus
}
#endif      /*  __cplusplus */

#endif  /* _GAPS_CLIENT_H_ */
