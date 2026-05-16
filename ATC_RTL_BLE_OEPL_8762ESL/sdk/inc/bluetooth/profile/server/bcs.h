/**
*****************************************************************************************
*     Copyright(c) 2017, Realtek Semiconductor Corporation. All rigbcs reserved.
*****************************************************************************************
  * @file     bcs.h
  * @brief    Variables and interfaces for using Body Composition Service.
  * @details  Body Composition Service data structs and functions.
  * @author
  * @date     2017-09-20
  * @version  v1.0
  * *************************************************************************************
  */

/* define to prevent recursive inclusion */
#ifndef _BCS_H_
#define _BCS_H_

#ifdef __cplusplus
extern "C"  {
#endif      /* __cplusplus */

/* Add Includes here */
#include "stdint.h"
#include "profile_server.h"
#include "rtl876x.h"


/** @defgroup BCS Body Composition Service
  * @brief  Body Composition service
   * @details

     The Body Composition service exposes data related to body composition from a body composition analyzer intended for consumer healthcare and sports/fitness applications.
     Application shall register Body Composition service when initialization through @ref bcs_add_service function.

     The Body Composition Feature characteristic shall be used to describe the supported features of the Server.
     Application can set a body composition feature through @ref bcs_set_parameter function.

     The Body Composition Measurement characteristic is used to send body composition-related data to the Client.     for display purposes while the measurement is in progress.
     Application can send body composition measurement values through @ref bcs_body_composition_measurement_value_indicate function.

  * @{
  */

/*============================================================================*
 *                         Macros
 *============================================================================*/
/** @defgroup BCS_Exported_Macros BCS Exported Macros
  * @brief
  * @{
  */

#define GATT_UUID_BODY_COMPOSITION                           0x181B
#define GATT_UUID_CHAR_BODY_COMPOSITION_FEATURE              0x2A9B
#define GATT_UUID_CHAR_BODY_COMPOSITION_MEASUREMENT          0x2A9C

/** @brief The Maximum Length of Body Composition Measurement Value*/
#define BCS_MEASUREMENT_VALUE_MAX_LEN                       30

/**
*  @brief Body Composition Feature field  bit 11 to 14
*         bcs_Weight_measurement_resolution_bit
*/
#define BCS_FEATURE_WEIGHT_MEAS_RES_NOT_SPECIFIED                       0x00
#define BCS_FEATURE_WEIGHT_MEAS_RES_0_5_KG_OR_1_LB                      0x01
#define BCS_FEATURE_WEIGHT_MEAS_RES_0_2_KG_OR_0_5_LB                    0x02
#define BCS_FEATURE_WEIGHT_MEAS_RES_0_1_KG_OR_0_2_LB                    0x03
#define BCS_FEATURE_WEIGHT_MEAS_RES_0_05_KG_OR_0_1_LB                   0x04
#define BCS_FEATURE_WEIGHT_MEAS_RES_0_02_KG_OR_0_05_LB                  0x05
#define BCS_FEATURE_WEIGHT_MEAS_RES_0_01_KG_OR_0_02_LB                  0x06
#define BCS_FEATURE_WEIGHT_MEAS_RES_0_005_KG_OR_0_01_LB                 0x07

/**
*  @brief Body Composition Feature field  bit 15 to 17
*         bcs_height_measurement_resolution_bit
*/
#define BCS_FEATURE_HEIGHT_MEAS_RES_NOT_SPECIFIED                       0x00
#define BCS_FEATURE_HEIGHT_MEAS_RES_0_01_METER_OR_1_INCH                0x01
#define BCS_FEATURE_WEIGHT_MEAS_RES_0_005_METER_OR_5_INCH               0x02
#define BCS_FEATURE_WEIGHT_MEAS_RES_0_001_METER_OR_0_1_INCH             0x03

/** @defgroup BCS_Notify_Indicate_Info BCS Notify Indicate Info
  * @brief  Parameter for enable or disable notification or indication.
  * @{
  */
#define BCS_INDICATE_BODY_COMPOSITIONT_MEASUREMENT_ENABLE     1
#define BCS_INDICATE_BODY_COMPOSITIONT_MEASUREMENT_DISABLE    2
/** @} */

/**
*  @brief Body Composition Measurement flags field
*         bcs_measurement_units_bit
*/
#define BCS_MEASUREMENT_UNITS_SI             0    /* Weight and Mass in units of kilogram (kg) and Height in units of meter */
#define BCS_MEASUREMENT_UNITS_IMPERIAL       1    /* Weight and Mass in units of pound (lb) and Height in units of inch (in) */

#define BCS_MEASUREMENT_BODY_FAT_PCT_UNSUCCESS    0xffff    /* body_fat_percentage field: measurement unsuccessful */

#define BCS_MEASUREMENT_USER_ID_UNKNOW            0xff      /* user_id field: unknown user */

/** End of BCS_Exported_Macros
* @}
*/

/*============================================================================*
 *                         Types
 *============================================================================*/
/** @defgroup BCS_Exported_Types BCS Exported Types
  * @brief
  * @{
  */

/**
*  @brief  Body Composition parameter type
*/
typedef enum
{
    BCS_PARAM_BODY_COMPOSITION_FEATURE
} T_BCS_PARAM_TYPE;

/**
*  @brief  Body Composition Feature
*/
typedef struct
{
    uint32_t bcs_feature_time_stamp_support_bit: 1;
    uint32_t bcs_feature_multiple_users_support_bit: 1;
    uint32_t bcs_feature_basal_metabolism_support_bit: 1;
    uint32_t bcs_feature_muscle_percentage_support_bit: 1;
    uint32_t bcs_feature_muscle_mass_support_bit: 1;
    uint32_t bcs_feature_fat_free_mass_support_bit: 1;
    uint32_t bcs_feature_soft_lean_mass_support_bit: 1;
    uint32_t bcs_feature_body_water_mass_support_bit: 1;
    uint32_t bcs_feature_impedance_support_bit: 1;
    uint32_t bcs_feature_weight_support_bit: 1;
    uint32_t bcs_feature_height_support_bit: 1;
    uint32_t bcs_weight_measurement_resolution_bit: 4;
    uint32_t bcs_height_measurement_resolution_bit: 3;
    uint32_t rfu: 14;
} T_BODY_COMPOSITION_FEATURE;

/**
*  @brief  Body Composition Measurement Flag
*/
typedef struct
{
    uint16_t bcs_measurement_units_bit: 1;
    uint16_t bcs_time_stamp_present_bit: 1;
    uint16_t bcs_user_id_bit: 1;
    uint16_t bcs_basal_metabolism_present_bit: 1;
    uint16_t bcs_muscle_percentage_present_bit: 1;
    uint16_t bcs_muscle_mass_present_bit: 1;
    uint16_t bcs_fat_free_mass_present_bit: 1;
    uint16_t bcs_soft_lean_mass_present_bit: 1;
    uint16_t bcs_body_water_mass_present_bit: 1;
    uint16_t bcs_impedance_present_bit: 1;
    uint16_t bcs_weight_present_bit: 1;
    uint16_t bcs_height_present_bit: 1;
    uint16_t bcs_multiple_packet_measurement_bit: 1;
    uint16_t rfu: 3;
} T_BODY_COMPOSITION_MEASUREMENT_FLAG;

typedef struct
{
    uint16_t year;
    uint8_t  month;
    uint8_t  day;
    uint8_t  hours;
    uint8_t  minutes;
    uint8_t  seconds;
} T_BCS_TIME_STAMP;

typedef struct
{
    T_BODY_COMPOSITION_MEASUREMENT_FLAG bcs_measurement_flag;
    uint16_t body_fat_percentage;
    T_BCS_TIME_STAMP time_stamp;
    uint8_t user_id;
    uint16_t basal_metabolism;
    uint16_t muscle_percentage;
    uint16_t muscle_mass;
    uint16_t fat_free_mass;
    uint16_t soft_lean_mass;
    uint16_t body_water_mass;
    uint16_t impedance;
    uint16_t weight;
    uint16_t height;
} T_BCS_BODY_COMPOSITION_MEASUREMENT;

/**
*  @brief  Body Composition service upper stream message data
*/
typedef union
{
    uint8_t notification_indification_index;
    uint8_t read_value_index;
} T_BCS_UPSTREAM_MSG_DATA;

/**
*  @brief  Body Composition service callback data
*/
typedef struct
{
    uint8_t                     conn_id;
    T_SERVICE_CALLBACK_TYPE     msg_type;
    T_BCS_UPSTREAM_MSG_DATA     msg_data;
} T_BCS_CALLBACK_DATA;

/** End of BCS_Exported_Types
* @}
*/

/*============================================================================*
 *                         Functions
 *============================================================================*/
/** @defgroup BCS_Exported_Functions BCS Exported Functions
  * @brief
  * @{
  */

/**
  * @brief       Add body composition service to the BLE stack database.
  *
  *
  * @param[in]   p_func  Callback when service attribute was read, write or cccd update.
  * @return Service id generated by the BLE stack: @ref T_SERVER_ID.
  * @retval 0xFF Operation failure.
  * @retval others Service id assigned by stack.
  *
  * <b>Example usage</b>
  * \code{.c}
     void profile_init()
     {
         server_init(1);
         bcs_id = bcs_add_service(app_handle_profile_message);
     }
  * \endcode
  */
uint8_t bcs_add_service(void *p_func);


/**
 * @brief       Set a body composition service parameter.
 *
 *              NOTE: You can call this function with a body composition service parameter type and it will set the
 *                    body composition service parameter. Body Composition service parameters are defined
 *                    in @ref T_BCS_PARAM_TYPE.
 *                    If the "len" field sets to the size of a "uint16_t", the
 *                    "p_value" field must point to a data with type of "uint16_t".
 *
 * @param[in]   param_type   Body composition service parameter type: @ref T_BCS_PARAM_TYPE
 * @param[in]   len       Length of data to write
 * @param[in]   p_value Pointer to data to write. This is dependent on
 *                      the parameter type and WILL be cast to the appropriate
 *                      data type (For example: if data type of param is uint16_t, p_value will be cast to
 *                      pointer of uint16_t).
 *
 * @return Operation result.
 * @retval true Operation success.
 * @retval false Operation failure.
 *
 * <b>Example usage</b>
 * \code{.c}
    void test(void)
    {
        uint32_t bcs_body_composition_feature = 0x000007ff;

        bcs_set_parameter(BCS_PARAM_BODY_COMPOSITION_FEATURE, sizeof(bcs_body_composition_feature),
                          &bcs_body_composition_feature);
    }
 * \endcode
 */
bool bcs_set_parameter(T_BCS_PARAM_TYPE param_type, uint8_t len, void *p_value);

/**
 * @brief       Send measurement value indication data .
 *
 * @param[in]   conn_id  Connection id.
 * @param[in]   service_id  Service id.
 * @param[in]   p_value  Pointer to data to indicate.
                         Type is @ref T_BCS_BODY_COMPOSITION_MEASUREMENT.
 *
 * @return Operation result.
 * @retval true Operation success.
 * @retval false Operation failure.
 *
 * <b>Example usage</b>
 * \code{.c}
    void test(void)
    {
      T_BCS_BODY_COMPOSITION_MEASUREMENT bcs_meas_value = {0};
      uint8_t conn_id = 0;
      uint16_t bcs_measurement_flag = 0x7f;
      memcpy(&bcs_meas_value.bcs_measurement_flag, &bcs_measurement_flag, 2);
      bcs_meas_value.body_fat_percentage = 20;
      bcs_meas_value.time_stamp = (T_BCS_TIME_STAMP) {2017, 9, 20, 20, 6, 8};
      bcs_meas_value.user_id = 0;
      bcs_meas_value.basal_metabolism = 50;
      bcs_meas_value.muscle_percentage = 30;
      bcs_meas_value.muscle_mass = 10;
      bcs_meas_value.fat_free_mass = 10;

      bcs_body_composition_measurement_value_indicate(conn_id, bcs_id,
                                                      &bcs_meas_value);
    }
 * \endcode
 */
bool bcs_body_composition_measurement_value_indicate(uint8_t conn_id, T_SERVER_ID service_id,
                                                     T_BCS_BODY_COMPOSITION_MEASUREMENT *p_data);

/**
 * @brief       Send consecutive measurement value indications data.
 *
 *              NOTE:If the measurement value indication data exceeds the current MTU size,
 *                   the remaining optional fields shall be sent in the subsequent indication
 *                   (also known as a 'continuation packet').
 *                   You shall call this function when processing the complete event callback
 *                   of the first indication @ref PROFILE_EVT_SEND_DATA_COMPLETE after calling
 *                   function @ref bcs_body_composition_measurement_value_indicate.
 *
 *
 * @param[in]   conn_id  Connection id.
 * @param[in]   service_id  Service id.
 * @param[in]   p_value  Pointer to data to indicate.
                         Type is @ref T_BCS_BODY_COMPOSITION_MEASUREMENT.
 *
 * @return Operation result.
 * @retval true Operation success.
 * @retval false Operation failure.
 *
 * <b>Example usage</b>
 * \code{.c}
    void test(void)
    {
        T_BCS_BODY_COMPOSITION_MEASUREMENT bcs_meas_value = {0};
        uint8_t conn_id = 0;
        uint16_t bcs_measurement_flag = 0x1fff;
        memcpy(&bcs_meas_value.bcs_measurement_flag, &bcs_measurement_flag, 2);
        bcs_meas_value.body_fat_percentage = 20;
        bcs_meas_value.time_stamp = (T_BCS_TIME_STAMP) {2017, 9, 20, 20, 6, 8};
        bcs_meas_value.user_id = 0;
        bcs_meas_value.basal_metabolism = 50;
        bcs_meas_value.muscle_percentage = 30;
        bcs_meas_value.muscle_mass = 10;
        bcs_meas_value.fat_free_mass = 10;
        bcs_meas_value.soft_lean_mass = 20;
        bcs_meas_value.body_water_mass = 70;
        bcs_meas_value.impedance = 10;
        bcs_meas_value.weight = 100;
        bcs_meas_value.height = 163;

        bcs_body_composition_measurement_value_indicate(conn_id, bcs_id,
                                                        &bcs_meas_value);
    }

    T_APP_RESULT app_handle_bcs_notify_indicate_cb_message(T_SERVER_ID service_id, void *p_data)
    {
        T_APP_RESULT result = APP_RESULT_SUCCESS;
        if (service_id == SERVICE_PROFILE_GENERAL_ID)
        {
            T_SERVER_APP_CB_DATA *p_para = (T_SERVER_APP_CB_DATA *)p_data;
            switch (p_para->eventId)
            {
            case PROFILE_EVT_SEND_DATA_COMPLETE:
                if (p_para->event_data.send_data_result.service_id == bcs_id)
                {
                    uint8_t conn_id = p_para->event_data.send_data_result.conn_id;
                    uint8_t serv_id = p_para->event_data.send_data_result.service_id;

                    if (bcs_measurement_value_consecutive_indicate(conn_id, serv_id))
                    {
                        //data exceeds, send first indication complete
                        //sending second indication
                        app_bcs_measurement_value_consecutive_indicate = true;
                        APP_PRINT_INFO1("PROFILE_EVT_SEND_DATA_COMPLETE sending second indication app_bcs_measurement_value_consecutive_indicate %d",
                                        app_bcs_measurement_value_consecutive_indicate);
                    }
                    else if (app_bcs_measurement_value_consecutive_indicate)
                    {
                        //data exceeds, send second indication complete
                        APP_PRINT_INFO1("PROFILE_EVT_SEND_DATA_COMPLETE send second indication complete app_bcs_measurement_value_consecutive_indicate %d",
                                        app_bcs_measurement_value_consecutive_indicate);
                        app_bcs_measurement_value_consecutive_indicate = false;
                    }
                    else
                    {
                        //data doesn't exceed, send whole indication complete
                        APP_PRINT_INFO1("PROFILE_EVT_SEND_DATA_COMPLETE send only one indication app_bcs_measurement_value_consecutive_indicate %d",
                                        app_bcs_measurement_value_consecutive_indicate);
                    }
                }

                break;

            default:
                break;
            }
        }
        return result;
    }

    T_APP_RESULT app_handle_profile_message(T_SERVER_ID service_id, void *p_data)
    {
        T_APP_RESULT result = APP_RESULT_SUCCESS;
        if (service_id == SERVICE_PROFILE_GENERAL_ID)
        {
            T_SERVER_APP_CB_DATA *p_para = (T_SERVER_APP_CB_DATA *)p_data;
            switch (p_para->eventId)
            {
            case PROFILE_EVT_SEND_DATA_COMPLETE:
                if (p_para->event_data.send_data_result.service_id == bcs_id)
                {
                    return app_handle_bcs_notify_indicate_cb_message(service_id, p_para);
                }
                break;

            default:
                break;
            }
        ...
        }
    }
 * \endcode
 */
bool bcs_measurement_value_consecutive_indicate(uint8_t conn_id, T_SERVER_ID service_id);

/** @} End of BCS_Exported_Functions */

/** @} End of BCS */


#ifdef __cplusplus
}
#endif

#endif // _BCS_H_
