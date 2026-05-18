/**
*****************************************************************************************
*     Copyright(c) 2017, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
  * @file     sps.h
  * @brief    Header file for using scan parameters service.
  * @details  SPS data structs and external functions declaration.
  * @author
  * @date
  * @version  v1.0
  * *************************************************************************************
  */

/* Define to prevent recursive inclusion */
#ifndef _SPS_H_
#define _SPS_H_

#ifdef  __cplusplus
extern "C" {
#endif      /* __cplusplus */

/* Add Includes here */
#include "profile_server.h"


/** @defgroup SPS Scan Parameters Service
  * @brief Scan parameters service
  * @details

    The Scan Parameters Service enables a GATT Server device to expose a characteristic for the GATT Client
    to write its scan interval and scan window on the GATT Server device, and enables a GATT Server to
    request a refresh of the GATT Client scan interval and scan window. The Scan Parameter Service makes up the
    services of HID Device, together with HID Service. Its role is to implement the interaction of data
    information when needing to change Scan parameters.

    Scan Parameter Service contains two Characteristics: one is scan interval and scan window, and it is used
    to store Scan Parameters of the Client; the other is Scan Refresh, and it is used to notify the Client to
    update the value of Scan parameter according to recent data by notification.

    The specific configuration can be achieved by modifying @ref sps_config.h.

    Applications shall register scan parameter service during initialization through @ref sps_add_service function.

    Applications can set the scan refresh value through @ref sps_set_parameter function.

    Applications can notify refreshed value to the client through the @ref sps_scan_interval_window_value_notify function.

  * @{
  */
/*============================================================================*
 *                              Macros
 *============================================================================*/
/** @defgroup SPS_Exported_Macros SPS Exported Macros
  * @brief
  * @{
  */

/** @defgroup SPS_Write_Info SPS Write Info
  * @brief  Parameter for writing characteristic value.
  * @{
  */
#define SPS_WRITE_SCAN_INTERVAL_WINDOW              1
/** @} */

/** @defgroup SPS_Notify_Indicate_Info SPS Notify Indicate Info
  * @brief  Parameter for enabling or disabling notification or indication.
  * @{
  */
#define SPS_NOTIFY_INDICATE_SCAN_REFRESH_ENABLE     1
#define SPS_NOTIFY_INDICATE_SCAN_REFRESH_DISABLE    2
/** @} */

/** @} End of SPS_Exported_Macros */

/*============================================================================*
 *                              Types
 *============================================================================*/
/** @defgroup SPS_Exported_Types SPS Exported Types
  * @brief
  * @{
  */

/** SPS parameter types. */
typedef enum
{
    SPS_PARAM_SCAN_REFRESH
} T_SPS_PARAM_TYPE;

/** SPS scan interval window structure.*/
typedef struct
{
    uint16_t    scan_interval;
    uint16_t    scan_window;
} T_SPS_SCAN_INTERVAL_WINDOW;


/** SPS write parameter.*/
typedef union
{
    T_SPS_SCAN_INTERVAL_WINDOW scan;
} T_SPS_WRITE_PARAMETER;

/** SPS write message.*/
typedef struct
{
    uint8_t write_type;
    T_SPS_WRITE_PARAMETER write_parameter;
} T_SPS_WRITE_MSG;

/** SPS upstream message data.*/
typedef union
{
    uint8_t notification_indification_index;
    T_SPS_WRITE_MSG write;
} T_SPS_UPSTREAM_MSG_DATA;

/** SPS callback data.*/
typedef struct
{
    uint8_t                 conn_id;
    T_SERVICE_CALLBACK_TYPE msg_type;
    T_SPS_UPSTREAM_MSG_DATA msg_data;
} T_SPS_CALLBACK_DATA;

/** @} End of SPS_Exported_Types */

/*============================================================================*
 *                              Functions
 *============================================================================*/
/** @defgroup SPS_Exported_Functions SPS Exported Functions
  * @brief
  * @{
  */

/**
 * @brief       Set a scan parameter service parameter.
 *
 * This function can be called with a scan parameter service parameter type and it will set the
 *                      scan parameter service parameter. Scan parameter service parameters are defined in @ref T_SPS_PARAM_TYPE.
 *                      If the "len" field is set to the size of a "uint16_t", the
 *                      "p_value" field must point to data of type "uint16_t".
 *
 * @param[in]   param_type   Scan parameter service parameter type: @ref T_SPS_PARAM_TYPE.
 * @param[in]   len       Length of data to write.
 * @param[in]   p_value Pointer to data to write. This is dependent on
 *                      the parameter type and will be cast to the appropriate
 *                      data type (For example: if data type of param is uint16_t, p_value will be cast to
 *                      a pointer of uint16_t).
 *
 * @return Operation result.
 * @retval true Operation success.
 * @retval false Operation failure.
 *
 * <b>Example usage</b>
 * \code{.c}
    void test(void)
    {
        bool ret = sps_set_parameter(SPS_PARAM_SCAN_REFRESH, 1, &refresh_value);
    }
 * \endcode
 */
bool sps_set_parameter(T_SPS_PARAM_TYPE param_type, uint8_t len, void *p_value);

/**
  * @brief Add scan parameters service to the Bluetooth Host.
  *
  * @param[in]   p_func  Callback when service attribute was read, write or CCCD update.
  * @return Service ID generated by the Bluetooth Host: @ref T_SERVER_ID.
  * @retval 0xFF Operation failure.
  * @retval others Service ID assigned by Bluetooth Host.
  *
  * <b>Example usage</b>
  * \code{.c}
    void profile_init()
    {
        server_init(service_num);
        sps_id = sps_add_service(app_handle_profile_message);
    }
  * \endcode
  */
T_SERVER_ID sps_add_service(void *p_func);

/**
  * @brief Send notification.
  *
  * @param[in] conn_id   Connection ID.
  * @param[in] service_id   Service ID of service.
  * @param[in] sps_refresh_value   Characteristic value to notify.
  * @return Notification action result.
  * @retval true Operation success.
  * @retval false Operation failed.
  *
  * <b>Example usage</b>
  * \code{.c}
     void test(void)
     {
          bool ret = sps_scan_interval_window_value_notify(conn_id, sps_id, sps_refresh_value);
     }
  * \endcode
  */
bool sps_scan_interval_window_value_notify(uint8_t conn_id, uint8_t service_id,
                                           uint8_t sps_refresh_value);
/** @} End of SPS_Exported_Functions */

/** @} End of SPS */

#ifdef  __cplusplus
}
#endif      /*  __cplusplus */

#endif /* _SPS_H_ */

