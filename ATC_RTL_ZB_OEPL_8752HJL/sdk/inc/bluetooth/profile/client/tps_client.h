/**
*****************************************************************************************
*     Copyright(c) 2016, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
  * @file     tps_client.h
  * @brief    Header file for using TPS Client.
  * @details  TPS data structures and external functions declaration.
  * @author   Ken
  * @date     2017-12-05
  * @version  v0.1
  * *************************************************************************************
  */

/* Define to prevent recursive inclusion */
#ifndef _TPS_CLIENT_H_
#define _TPS_CLIENT_H_

#ifdef  __cplusplus
extern "C" {
#endif      /* __cplusplus */

/* Add Includes here */
#include <profile_client.h>


/** @defgroup TPS_Client Tx Power Service Client
  * @brief TPS service client
  * @{
  */
/*============================================================================*
 *                         Macros
 *============================================================================*/
/** @defgroup TPS_Client_Exported_Macros TPS Client Exported Macros
  * @brief
  * @{
  */
/** @defgroup TPS_UUIDs TPS UUIDs
  * @brief TPS LE Profile UUID definitions
  * @{
  */
#define GATT_UUID_TX_POWER_SERVICE              0x1804
#define GATT_UUID_CHAR_TX_LEVEL                 0x2A07
/** @} End of TPS_UUIDs */

/** @defgroup TPS_DEFs TPS Definitions
  * @{
  */
/** @brief  Define links number. */
#define TPS_MAX_LINKS  2
/** End of TPS_DEFs
  * @}
  */

/** End of TPS_Client_Exported_Macros
  * @}
  */


/*============================================================================*
 *                         Types
 *============================================================================*/
/** @defgroup TPS_Client_Exported_Types TPS Client Exported Types
  * @brief
  * @{
  */

/** @brief TPS client handle type*/
typedef enum
{
    HDL_TPS_SRV_START,           //!< Start handle of TPS service.
    HDL_TPS_SRV_END,             //!< End handle of TPS service.
    HDL_TPS_PARA,                //!< TPS parameter read/write characteristic value handle.
    HDL_TPS_CACHE_LEN            //!< Handle cache length.
} T_TPS_HANDLE_TYPE;

/** @brief TPS client discovery state*/
typedef enum
{
    DISC_TPS_IDLE,
    DISC_TPS_START,
    DISC_TPS_DONE,
    DISC_TPS_FAILED
} T_TPS_DISC_STATE;

/** @brief TPS client read type*/
typedef enum
{
    TPS_READ_PARA,
} T_TPS_READ_TYPE;


/** @brief TPS client read data */
typedef union
{
    uint8_t txpower_level;
} T_TPS_READ_DATA;


/** @brief TPS client read result */
typedef struct
{
    T_TPS_READ_TYPE type;
    T_TPS_READ_DATA data;
    uint16_t cause;
} T_TPS_READ_RESULT;

/** @brief TPS client callback type */
typedef enum
{
    TPS_CLIENT_CB_TYPE_DISC_STATE,          //!< Discovery procedure state, done or pending.
    TPS_CLIENT_CB_TYPE_READ_RESULT,         //!< Read request's result data, responded from server.
    TPS_CLIENT_CB_TYPE_INVALID              //!< Invalid callback type, no practical usage.
} T_TPS_CLIENT_CB_TYPE;

/** @brief TPS client callback content */
typedef union
{
    T_TPS_DISC_STATE      disc_state;
    T_TPS_READ_RESULT     read_result;
} T_TPS_CLIENT_CB_CONTENT;

/** @brief TPS client callback data */
typedef struct
{
    T_TPS_CLIENT_CB_TYPE     cb_type;
    T_TPS_CLIENT_CB_CONTENT  cb_content;
} T_TPS_CLIENT_CB_DATA;

/** End of TPS_Client_Exported_Types * @} */

/** @defgroup TPS_Client_Exported_Functions TPS Client Exported Functions
  * @{
  */

/**
  * @brief      Add TPS service client to application.
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
        tps_client_id = tps_add_client(app_client_callback, APP_MAX_LINKS);
    }
  * \endcode
  */
T_CLIENT_ID tps_add_client(P_FUN_GENERAL_APP_CB app_cb, uint8_t link_num);

/**
  * @brief  Used by application, to start the discovery procedure of TPS server.
  * @param[in]  conn_id Connection ID.
  * @retval true Send request to Bluetooth Host success.
  * @retval false Send request to Bluetooth Host failed.
  */
bool tps_start_discovery(uint8_t conn_id);

/**
  * @brief  Used by application, to read power level from server.
  * @param[in]  conn_id Connection ID.
  * @retval true Send request to Bluetooth Host success.
  * @retval false Send request to Bluetooth Host failed.
  */
bool tps_read_power_level(uint8_t conn_id);

/**
  * @brief  Used by the application, to get the handle cache.
  * @param[in]  conn_id Connection ID.
  * @param[in,out]  p_hdl_cache Pointer to the handle cache table.
  * @param[in]  len Length of the handle cache table.
  * @retval true Success.
  * @retval false Failed.
  */
bool tps_client_get_hdl_cache(uint8_t conn_id, uint16_t *p_hdl_cache, uint8_t len);

/**
  * @brief  Used by the application, to set the handle cache.
  * @param[in]  conn_id Connection ID.
  * @param[in]  p_hdl_cache Pointer to the handle cache table.
  * @param[in]  len Length of the handle cache table.
  * @retval true Success.
  * @retval false Failed.
  */
bool tps_client_set_hdl_cache(uint8_t conn_id, uint16_t *p_hdl_cache, uint8_t len);

/** @} End of LLS_Client_Exported_Functions */

/** @} End of LLS_Client */


#ifdef  __cplusplus
}
#endif      /*  __cplusplus */

#endif  /* _KNS_CLIENT_H_ */
