/*****************************************************************************************
*     Copyright(c) 2017, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
  * @file     hrs.h
  * @brief    Variables and interfaces for using the Heart Rate Service.
  * @details  Heart Rate Service data structures and functions.
  * @author
  * @date     2017-09-21
  * @version  v0.1
  * *************************************************************************************
  */

/* Define to prevent recursive inclusion */
#ifndef _HRS_DEFINE_H
#define _HRS_DEFINE_H

#ifdef __cplusplus
extern "C"  {
#endif      /* __cplusplus */

/* Add Includes here */
#include "profile_server.h"


/** @defgroup HRS Heart Rate Service
  * @brief  Heart Rate Service
  * @{
  */

/** @defgroup HRS_DEFINE  HRS Define
  * @brief Heart rate service define file.
  * @{
  */

/*============================================================================*
 *                         Macros
 *============================================================================*/
/** @defgroup HRS_Common_Exported_Macros HRS Common Exported Macros
  * @brief
  * @{
  */
/** @defgroup HRS_Common_DEFs HRS Common Definitions
  * @{
  */
#define HRS_BODY_SENSOR_LOCATION_CHAR_SUPPORT
#define HRS_ENERGY_EXPENDED_FEATURE_SUPPORT

#define HRS_HEART_RATE_MEASUREMENT_VALUE_INDEX              2//notify
#define HRS_BODY_SENSOR_LOCATION_VALUE_INDEX                5//read
#define HRS_HEART_RATE_CP_VALUE_INDEX                       7//write

#define HRS_HEART_RATE_MEASUREMENT_CCCD_INDEX               3

#define GATT_UUID_SERVICE_HEART_RATE                        0x180D
#define GATT_UUID_CHAR_HRS_HEART_RATE_MEASUREMENT           0x2A37
#define GATT_UUID_CHAR_HRS_BODY_SENSOR_LOCATION             0x2A38
#define GATT_UUID_CHAR_HRS_HEART_RATE_CP                    0x2A39

#define HRS_CP_RSPCODE_RESERVED                             0x00
#define HRS_CP_RSPCODE_SUCCESS                              0x01
#define HRS_CP_RSPCODE_OPCODE_UNSUPPORT                     0x02
#define HRS_CP_RSPCODE_INVALID_PARAMETER                    0x03
#define HRS_CP_RSPCODE_OPERATION_FAILED                     0x04

#define HRS_CTL_PNT_OPERATE_ACTIVE(x)                     \
    (x == HRS_HEART_RATE_CP_OPCODE_RESET_ENERGY_EXPENDED)

#define Heart_Rate_Value_Format_UINT8                       0
#define Heart_Rate_Value_Format_UINT16                      1

#define HRS_HEART_RATE_MEASUREMENT_VALUE_MAX_LEN            7

#define HRS_MAX_CTL_PNT_VALUE                               1
/** @} */

/** @defgroup HRS_Read_Info HRS Read Info
  * @brief  Parameter for reading characteristic value.
  * @{
  */
#define HRS_READ_BODY_SENSOR_LOCATION_VALUE                 1
/** @} */

/** @defgroup HRS_Sensor_Location HRS Sensor Location
  * @brief  Body Sensor Location Value
  * @{
  */
#define  BODY_SENSOR_LOCATION_VALUE_OTHER                   0
#define  BODY_SENSOR_LOCATION_VALUE_CHEST                   1
#define  BODY_SENSOR_LOCATION_VALUE_WRIST                   2
#define  BODY_SENSOR_LOCATION_VALUE_FINGER                  3
#define  BODY_SENSOR_LOCATION_VALUE_HAND                    4
#define  BODY_SENSOR_LOCATION_VALUE_EAR_LOBE                5
#define  BODY_SENSOR_LOCATION_VALUE_FOOT                    6
/** @} */

/** End of HRS_Common_Exported_Macros
* @}
*/

/*============================================================================*
 *                         Types
 *============================================================================*/
/** @defgroup HRS_Common_Exported_Types HRS Common Exported Types
  * @brief
  * @{
  */
typedef enum
{
    HRS_HEART_RATE_CP_OPCODE_RESERVED = 0,
    HRS_HEART_RATE_CP_OPCODE_RESET_ENERGY_EXPENDED = 1
} T_HRS_HEART_RATE_CP_OPCODE;


/** Notification indication flag. */
typedef struct
{
    uint8_t heart_rate_measurement_notify_enable: 1;
    uint8_t rfu: 7;
} HRS_NOTIFY_INDICATE_FLAG;

/** Heart Rate Measurement Value Flag. */
typedef struct
{
    uint8_t heart_rate_value_format_bit: 1;
    uint8_t sensor_contact_status_bits: 2;
    uint8_t energy_expended_status_bit: 1;
    uint8_t rr_interval_bit: 1;
    uint8_t rfu: 3;
} T_HEART_RATE_MEASUREMENT_VALUE_FLAG;

/** Heart Rate Measurement Value. */
typedef struct
{
    T_HEART_RATE_MEASUREMENT_VALUE_FLAG flag;
    uint16_t heart_rate_measurement_value;
    uint16_t energy_expended;
    uint16_t rr_interval;
} T_HEART_RATE_MEASUREMENT_VALUE;

/**
 * @brief HRS Control Point data, variable length during connection, max can reach 17 octets.
 *
 * HRS Control Point data used to store the Control Point Command received from the client.
*/
typedef struct
{
    uint8_t    cur_length; /**<  Length of current CSC Control Point data. */
    uint8_t    value[HRS_MAX_CTL_PNT_VALUE]; /**<  Value of current CSC Control Point data. */
} T_HRS_CONTROL_POINT;

/** End of HRS_Common_Exported_Types
* @}
*/


/** @} End of HRS_DEFINE */

/** @} End of HRS */

#ifdef __cplusplus
}
#endif

#endif /* _HRS_SERVICE_DEF_H */

