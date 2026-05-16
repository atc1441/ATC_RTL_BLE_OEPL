/**
*****************************************************************************************
*     Copyright(c) 2017, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
  * @file     ftms.h
  * @brief    Head file for using Fitness Machine Service.
  * @details  FTMS data types and external functions declaration.
  * @author
  * @date
  * @version  v1.0
  * *************************************************************************************
  */

/* Define to prevent recursive inclusion */
#ifndef _FTMS_H_
#define _FTMS_H_

#ifdef __cplusplus
extern "C"  {
#endif      /* __cplusplus */

/* Add Includes here */
#include "profile_server.h"
#include "ftms_define.h"
#include "srv_uuid.h"

/** @defgroup Fitness Machine Service
  * @brief  Fitness Machine Service
   * @details

    The Fitness Machine Service (FTMS) exposes training-related data in the sports
    and fitness environment, which allows a Client to collect training data while
    a user is exercising with a fitness machine (Server).
    The service may also expose the Fitness Machine Status characteristic to send
    information to the Client about the status of the machine.
    The Fitness Machine Control Point may also be exposed by the Server in order to
    provide a mechanism to remotely control a fitness machine.
    The types of fitness machines that are currently supported are listed below:
      Treadmill
      Step Climber
      Rower
      Indoor Bike
      Cross Trainer
      Stair Climber

    The Fitness Machine Feature characteristic shall be used to describe the supported features of the Server.
    The Treadmill Data characteristic is used to send training-related data to the Client from a treadmill (Server).
    The Step Climber Data characteristic is used to send training-related data to the Client from a step climber (Server).
    The Rower Data characteristic is used to send training-related data to the Client from a rower (Server).
    The Indoor Bike Data characteristic is used to send training-related data to the Client from an indoor bike (Server).
    The Cross Trainer Data characteristic is used to send training-related data to the Client from a cross trainer (Server).
    The Stair Climber Data characteristic is used to send training-related data to the Client from a stair climber (Server).
    The Supported Speed Range characteristic shall be exposed by the Server if the Speed Target Setting feature is supported.
    The Supported Inclination Range characteristic shall be exposed by the Server if the Inclination Target Setting feature is supported.
    The Supported Resistance Level Range characteristic shall be exposed by the Server if the Resistance Control Target Setting feature is supported.
    The Supported Power Range characteristic shall be exposed by the Server if the Power Target Setting feature is supported.
    The Supported Heart Rate Range characteristic shall be exposed by the Server if the Heart Rate Target Setting feature is supported.
    The Fitness Machine Control Point characteristic is used to request a specific function to be executed on the Server.
    The Fitness Machine Status characteristic is used to send the status of the Server.

    The specific configuration process can be achieved by modifying file @ref ftms_config.h.

    Application shall register Fitness Machine service when initialization through @ref ftms_add_service function.

    Application can set the Fitness Machine parameters through @ref ftms_set_parameter function.

    Application can send the treadmill data through @ref ftms_treadmill_data_notify function.

    Application can send the step climber data through @ref ftms_step_climber_data_notify function.

    Application can send the rower data through @ref ftms_rower_data_notify function.

    Application can send the indoor bike data through @ref ftms_indoor_bike_data_notify function.

    Application can send the indoor bike data through @ref ftms_cross_trainer_data_notify function.

    Application can send the indoor bike data through @ref ftms_stair_climber_data_notify function.

    Application can send the fitness machine status through @ref ftms_status_notify function.

    Application can clear the fitness machine flags through @ref ftms_flags_clear function.

  * @{
  */

/*============================================================================*
 *                         Macros
 *============================================================================*/
/** @defgroup FTMS_Exported_Macros FTMS Exported Macros
  * @brief
  * @{
  */

#define FTMS_NOTIFY_INDICATE_TREADMILL_DATA_ENABLE             1
#define FTMS_NOTIFY_INDICATE_TREADMILL_DATA_DISABLE            2
#define FTMS_NOTIFY_INDICATE_STEP_CLIMBER_DATA_ENABLE          3
#define FTMS_NOTIFY_INDICATE_STEP_CLIMBER_DATA_DISABLE         4
#define FTMS_NOTIFY_INDICATE_ROWER_DATA_ENABLE                 5
#define FTMS_NOTIFY_INDICATE_ROWER_DATA_DISABLE                6
#define FTMS_NOTIFY_INDICATE_INDOOR_BIKE_DATA_ENABLE           7
#define FTMS_NOTIFY_INDICATE_INDOOR_BIKE_DATA_DISABLE          8
#define FTMS_NOTIFY_INDICATE_FTMS_CP_ENABLE                    9
#define FTMS_NOTIFY_INDICATE_FTMS_CP_DISABLE                   10
#define FTMS_NOTIFY_INDICATE_FTMS_STATUS_ENABLE                11
#define FTMS_NOTIFY_INDICATE_FTMS_STATUS_DISABLE               12
#define FTMS_NOTIFY_INDICATE_TRAIN_STATUS_ENABLE               13
#define FTMS_NOTIFY_INDICATE_TRAIN_STATUS_DISABLE              14
#define FTMS_NOTIFY_INDICATE_CROSS_TRAINER_DATA_ENABLE         15
#define FTMS_NOTIFY_INDICATE_CROSS_TRAINER_DATA_DISABLE        16
#define FTMS_NOTIFY_INDICATE_STAIR_CLIMBER_DATA_ENABLE         17
#define FTMS_NOTIFY_INDICATE_STAIR_CLIMBER_DATA_DISABLE        18

/** End of FTMS_Exported_Macros
* @}
*/

/*============================================================================*
 *                         Types
 *============================================================================*/
/** @defgroup FTMS_Exported_Types UDS Exported Types
  * @brief
  * @{
  */

/**
*  @brief Fitness Machine Service parameter type
*/

typedef union
{
    uint16_t target_speed;                   /* opcode FTMS_CP_OPCODE_SET_TGT_SPEED*/
    int16_t target_inclination;              /* opcode FTMS_CP_OPCODE_SET_TGT_INCLINATION*/
    int16_t target_resistance_level;         /* opcode FTMS_CP_OPCODE_SET_TGT_RESISTANCE_LEVEL*/
    int16_t target_power;                    /* opcode FTMS_CP_OPCODE_SET_TGT_POWER*/
    uint8_t target_heart_rate;               /* opcode FTMS_CP_OPCODE_SET_TGT_HEART_RATE*/
    uint8_t ctrl_information;                /* opcode FTMS_CP_OPCODE_STOP_PAUSE*/
    uint16_t target_expended_energy;         /* opcode FTMS_CP_OPCODE_SET_TGT_EXPEND_ENERGY*/
    uint16_t target_step_num;                /* opcode FTMS_CP_OPCODE_SET_TGT_STEP_NUM*/
    uint16_t target_stride_num;              /* opcode FTMS_CP_OPCODE_SET_TGT_STRIDE_NUM*/
    uint32_t target_distance;                /* opcode FTMS_CP_OPCODE_SET_TGT_DISTANCE (uint24) */
    uint16_t target_training_time;           /* opcode FTMS_CP_OPCODE_SET_TGT_TRAINING_TIME*/
    T_FTMS_TWO_ZONES_HR_TIME
    target_two_zones_hr_time;                /* opcode FTMS_CP_OPCODE_SET_TGT_TWO_HR_ZONES_TIME*/
    T_FTMS_THREE_ZONES_HR_TIME
    target_three_zones_hr_time;              /* opcode FTMS_CP_OPCODE_SET_TGT_THREE_HR_ZONES_TIME*/
    T_FTMS_FIVE_ZONES_HR_TIME
    target_five_zones_hr_time;               /* opcode FTMS_CP_OPCODE_SET_TGT_FIVE_HR_ZONES_TIME*/
    T_FTMS_INDOOR_BIKE_SIMULATION_PARAM
    set_indoor_bike_simulation_param;        /* opcode FTMS_CP_OPCODE_SET_INDOOR_BIKE_SIMULATION_PARAM*/
    uint16_t wheel_circumference;            /* opcode FTMS_CP_OPCODE_SET_WHEEL_CIRCUMFERENCE */
    uint8_t ctrl_param;                      /* opcode FTMS_CP_OPCODE_SPIN_DOWN_CTRL */
    uint8_t spin_down_status_value;
    uint16_t target_cadence;                 /* opcode FTMS_CP_OPCODE_SET_TGT_CADENCE*/
} T_FTMS_CP_PARAMETER;

typedef enum
{
    FTMS_PARAM_FTMS_FEATURE = 0x01,                 /* Set fitness machine feature */
    FTMS_PARAM_SUPPORT_SPEED_RANGE,                 /* Set supported speed range */
    FTMS_PARAM_SUPPORT_INCLINATION_RANGE,           /* Set supported inclination range */
    FTMS_PARAM_SUPPORT_RESISTANCE_LEVEL_RANGE,      /* Set supported resistance level range */
    FTMS_PARAM_SUPPORT_POWER_RANGE,                 /* Set supported power range */
    FTMS_PARAM_SUPPORT_HR_RANGE,                    /* Set supported heart rate range */
    FTMS_PARAM_TREADMILL_DATA_FLAG,                 /* Set treadmill data flag field */
    FTMS_PARAM_STEP_CLIMBER_DATA_FLAG,              /* Set step climber data flag field */
    FTMS_PARAM_ROWER_DATA_FLAG,                     /* Set rower data flag field */
    FTMS_PARAM_INDOOR_BIKE_DATA_FLAG,               /* Set indoor bike data flag field */
    FTMS_PARAM_CTL_PNT_PROG_CLR,                    /* Set control point opcode to reserved */
    FTMS_PARAM_SD_RESP_PARAM,                       /* Set spin down response parameter */
    FTMS_PARAM_CROSS_TRAINER_DATA_FLAG,             /* Set cross trainer data flag field */
    FTMS_PARAM_STAIR_CLIMBER_DATA_FLAG              /* Set stair climber data flag field */
} T_FTMS_PARAM_TYPE;

typedef struct
{
    uint8_t opcode;
    T_FTMS_CP_PARAMETER cp_parameter;
} T_FTMS_WRITE_MSG;

typedef struct
{
    uint8_t notification_indication_index;
    T_FTMS_WRITE_MSG write;
} T_FTMS_UPSTREAM_MSG_DATA;

typedef struct
{
    uint8_t                    conn_id;
    uint16_t                   char_uuid;
    T_SERVICE_CALLBACK_TYPE    msg_type;
    T_FTMS_UPSTREAM_MSG_DATA   msg_data;
} T_FTMS_CALLBACK_DATA;

/** End of FTMS_Exported_Types
* @}
*/

/*============================================================================*
 *                         Functions
 *============================================================================*/
/** @defgroup FTMS_Exported_Functions FTMS Exported Functions
  * @brief
  * @{
  */

/**
 * @brief       Add fitness machine service to the BLE stack database.
 *
 *
 * @param[in]   p_func  Callback when service attribute was read, write or cccd update.
 * @return Service id generated by the BLE stack: @ref T_SERVER_ID.
 * @retval 0xFF Operation failure.
 * @retval Others Service id assigned by stack.
 *
 * <b>Example usage</b>
 * \code{.c}
    void profile_init()
    {
        server_init(1);
        ftms_id = ftms_add_service(app_handle_profile_message);
    }
 * \endcode
 */
T_SERVER_ID ftms_add_service(void *p_func);

/**
 * @brief       Set a fitness machine service parameter.
 *
 *              NOTE: You can call this function with a fitness machine service parameter type and it will set the
 *                      fitness machine service parameter.  Fitness machine service parameters are defined in @ref T_FTMS_PARAM_TYPE.
 *                      If the "len" field sets to the size of a "uint16_t" ,the "p_value" field must point to a data
 *                      with type of "uint16_t".
 *
 * @param[in]   param_type   Fitness machine service parameter type: @ref T_FTMS_PARAM_TYPE
 * @param[in]   len          Length of data to write.
 * @param[in]   p_value      Pointer to data to write.
 *                           This is dependent on the parameter type and will be cast to the appropriate data type.
                             If param_type is set to @ref FTMS_PARAM_FTMS_FEATURE, p_value pointer to the current
                             fitness machine feature. Type is org.bluetooth.characteristic.fitness_machine_feature.
 *
 * @return Operation result.
 * @retval true Operation success.
 * @retval false Operation failure.
 *
 * <b>Example usage</b>
 * \code{.c}
    void test(void)
    {
        T_FTMS_FEATURE ftms_feature;
        ftms_feature.features_field = FTMS_FEAT_ALL_FEATURE_SUPPORTED;
        ftms_feature.target_setting_field = FTMS_TGT_SET_ALL_FEATURE_SUPPORTED;
        ftms_set_parameter(FTMS_PARAM_FTMS_FEATURE, sizeof(T_FTMS_FEATURE), &ftms_feature);
    }
 * \endcode
 */
bool ftms_set_parameter(T_FTMS_PARAM_TYPE param_type, uint8_t len, void *p_value);

/**
 * @brief       Send treadmill data notification data .
 *
 *
 * @param[in]   conn_id           Connection id.
 * @param[in]   service_id        Service id.
 * @param[in]   p_treadmill_data  Pointer to data to notify.
                                  Type is org.bluetooth.characteristic.treadmill_data.
 * @return Operation result.
 * @retval true Operation success.
 * @retval false Operation failure.
 *
 * <b>Example usage</b>
 * \code{.c}
    T_FTMS_TREADMILL_DATA ftms_treadmill_data = {350, 250, 1000, 15, 12, 100, 200, 10, 12,
                                                 2400, 1200, 20, 87, 30, 1200, 6000, 50,
                                                 2400};
    void test(uint8_t conn_id)
    {
        T_FTMS_TREADMILL_DATA data = {370, 260, 2000, 15, 12, 100, 200, 10, 12, 2800, 1200,
                                      20, 87, 30, 1250, 5950, 50, 2400};
        memcpy(&ftms_treadmill_data, &data, sizeof(T_FTMS_TREADMILL_DATA));
        ftms_treadmill_data_notify(conn_id, ftms_id, &ftms_treadmill_data);
    }
 * \endcode
 */
#if FTMS_CHAR_TREADMILL_DATA_SUPPORT
bool ftms_treadmill_data_notify(uint8_t conn_id, T_SERVER_ID service_id,
                                T_FTMS_TREADMILL_DATA *p_treadmill_data);
#endif

/**
 * @brief       Send step climber data notification data .
 *
 *
 * @param[in]   conn_id              Connection id.
 * @param[in]   service_id           Service id.
 * @param[in]   p_step_climber_data  Pointer to data to indicate.
                                     Type is org.bluetooth.characteristic.step_climber_data.
 * @return Operation result.
 * @retval true Operation success.
 * @retval false Operation failure.
 *
 * <b>Example usage</b>
 * \code{.c}
    T_FTMS_STEP_CLIMBER_DATA ftms_step_climber_data = {30, 3000, 50, 500, 100, 2400, 1200,
                                                       20, 87, 30, 1200, 6000};
    void test(uint8_t conn_id)
    {
        T_FTMS_STEP_CLIMBER_DATA step_climber_data = {35, 3500, 50, 500, 100, 2800, 1200,
                                                      20, 89, 30, 1250, 5950};
        memcpy(&ftms_step_climber_data, &step_climber_data, sizeof(T_FTMS_STEP_CLIMBER_DATA));
        ftms_step_climber_data_notify(conn_id, ftms_id, &ftms_step_climber_data);
    }
 * \endcode
 */
#if FTMS_CHAR_STEP_CLIMBER_DATA_SUPPORT
bool ftms_step_climber_data_notify(uint8_t conn_id, T_SERVER_ID service_id,
                                   T_FTMS_STEP_CLIMBER_DATA *p_step_climber_data);
#endif

/**
 * @brief       Send rower data notification data .
 *
 *
 * @param[in]   conn_id       Connection id.
 * @param[in]   service_id    Service id.
 * @param[in]   p_rower_data  Pointer to data to indicate.
                              Type is org.bluetooth.characteristic.rower_data.
 * @return Operation result.
 * @retval true Operation success.
 * @retval false Operation failure.
 *
 * <b>Example usage</b>
 * \code{.c}
    T_FTMS_ROWER_DATA ftms_rower_data = {20, 3000, 18, 1000, 10, 12, 350, 400, 20, 2400,
                                         1200, 20, 87, 30, 1200, 6000};
    void test(uint8_t conn_id)
    {
        T_FTMS_ROWER_DATA rower_data = {25, 3500, 18, 2000, 10, 12, 350, 400, 20, 2800,
                                        1200, 20, 89, 30, 1250, 5950};
        memcpy(&ftms_rower_data, &rower_data, sizeof(T_FTMS_ROWER_DATA));
        ftms_rower_data_notify(conn_id, ftms_id, &ftms_rower_data);
    }
 * \endcode
 */
#if FTMS_CHAR_ROWER_DATA_SUPPORT
bool ftms_rower_data_notify(uint8_t conn_id, T_SERVER_ID service_id,
                            T_FTMS_ROWER_DATA *p_rower_data);
#endif

/**
 * @brief       Send indoor bike data notification data .
 *
 *
 * @param[in]   conn_id             Connection id.
 * @param[in]   service_id          Service id.
 * @param[in]   p_indoor_bike_data  Pointer to data to indicate.
                                    Type is org.bluetooth.characteristic.indoor_bike_data.
 * @return Operation result.
 * @retval true Operation success.
 * @retval false Operation failure.
 *
 * <b>Example usage</b>
 * \code{.c}
    T_FTMS_INDOOR_BIKE_DATA ftms_indoor_bike_data = {350, 250, 30, 35, 1000, 20, 350, 400,
                                                     2400, 1200, 20, 87, 30, 1200, 6000};
    void test(uint8_t conn_id)
    {
        T_FTMS_INDOOR_BIKE_DATA indoor_bike_data = {380, 260, 30, 35, 2000, 20, 350, 400,
                                                    2800, 1200, 20, 89, 30, 1250, 5950};
        memcpy(&ftms_indoor_bike_data, &indoor_bike_data, sizeof(T_FTMS_INDOOR_BIKE_DATA));
        ftms_indoor_bike_data_notify(conn_id, ftms_id, &ftms_indoor_bike_data);
    }
 * \endcode
 */
#if FTMS_CHAR_INDOOR_BIKE_DATA_SUPPORT
bool ftms_indoor_bike_data_notify(uint8_t conn_id, T_SERVER_ID service_id,
                                  T_FTMS_INDOOR_BIKE_DATA *p_indoor_bike_data);
#endif

/**
 * @brief       Send indoor bike data notification data .
 *
 *
 * @param[in]   conn_id               Connection id.
 * @param[in]   service_id            Service id.
 * @param[in]   p_cross_trainer_data  Pointer to data to indicate.
                                      Type is org.bluetooth.characteristic.cross_trainer_data.
 * @return Operation result.
 * @retval true Operation success.
 * @retval false Operation failure.
 *
 * <b>Example usage</b>
 * \code{.c}
    T_FTMS_CROSS_TRAINER_DATA ftms_cross_trainer_data = {350, 250, 1000, 70, 65, 2000, 100,
                                                         200, 15, 12, 20, 350, 400, 2400,
                                                         1200, 20, 87, 30, 1200, 6000};
    void test(uint8_t conn_id)
    {
        T_FTMS_CROSS_TRAINER_DATA cross_trainer_data = {380, 260, 1000, 70, 65, 2000, 100,
                                                        200, 15, 12, 20, 350, 400, 2400,
                                                        1200, 20, 89, 30, 1250, 5950};
        memcpy(&ftms_cross_trainer_data, &cross_trainer_data, sizeof(T_FTMS_CROSS_TRAINER_DATA));
        cmd_ftms_cross_trainer_data_notify(conn_id, ftms_id, &ftms_cross_trainer_data);
    }
 * \endcode
 */
#if FTMS_CHAR_CROSS_TRAINER_DATA_SUPPORT
bool ftms_cross_trainer_data_notify(uint8_t conn_id, T_SERVER_ID service_id,
                                    T_FTMS_CROSS_TRAINER_DATA *p_cross_trainer_data);
#endif

/**
 * @brief       Send indoor bike data notification data .
 *
 *
 * @param[in]   conn_id               Connection id.
 * @param[in]   service_id            Service id.
 * @param[in]   p_stair_climber_data  Pointer to data to indicate.
                                      Type is org.bluetooth.characteristic.stair_climber_data.
 * @return Operation result.
 * @retval true Operation success.
 * @retval false Operation failure.
 *
 * <b>Example usage</b>
 * \code{.c}
    T_FTMS_STAIR_CLIMBER_DATA ftms_stair_climber_data = {30, 70, 65, 15, 20, 2400, 1200, 20,
                                                     87, 30, 1200, 6000};
    void test(uint8_t conn_id)
    {
        T_FTMS_STAIR_CLIMBER_DATA stair_climber_data = {40, 70, 65, 15, 20, 2400, 1200, 20,
                                                    89, 30, 1250, 5950};
        memcpy(&ftms_stair_climber_data, &stair_climber_data, sizeof(T_FTMS_STAIR_CLIMBER_DATA));
        ftms_stair_climber_data_notify(conn_id, ftms_id, &stair_climber_data);
    }
 * \endcode
 */
#if FTMS_CHAR_STAIR_CLIMBER_DATA_SUPPORT
bool ftms_stair_climber_data_notify(uint8_t conn_id, T_SERVER_ID service_id,
                                    T_FTMS_STAIR_CLIMBER_DATA *p_stair_climber_data);
#endif

/**
 * @brief       Send fitness machine control point indication data .
 *
 *
 * @param[in]   conn_id     Connection id.
 * @param[in]   service_id  Service id.
 * @param[in]   op_code     Op code.
 * @param[in]   rsp_code    Response code.
 * @return Operation result.
 * @retval true Operation success.
 * @retval false Operation failure.
 *
 * <b>Example usage</b>
 * \code{.c}
    void test(uint8_t conn_id, T_SERVER_ID service_id, uint8_t rsp_code)
    {
        uint8_t op_code = FTMS_CP_OPCODE_REQUEST_CONTROL;
        ftms_var.control_permission = true;
        ftms_ctl_pnt_indication(conn_id, service_id, op_code, rsp_code);
    }
 * \endcode
 */
#if FTMS_CHAR_FTMS_CTRL_PNT_SUPPORT
bool ftms_ctl_pnt_indication(uint8_t conn_id, T_SERVER_ID service_id, uint8_t opcode,
                             uint8_t rsp_code);
#endif

/**
 * @brief       Send fitness machine status notification data.
 *
 *
 * @param[in]   conn_id        Connection id.
 * @param[in]   service_id     Service id.
 * @param[in]   opcode         Op code.
 * @param[in]   param          Data to notify.
 * @param[in]   param_len      Length of data to notify.
 * @return Operation result.
 * @retval true Operation success.
 * @retval false Operation failure.
 *
 * <b>Example usage</b>
 * \code{.c}
    void test(uint8_t conn_id)
    {
        T_FTMS_CP_PARAMETER status_value;
        uint8_t opcode = FTMS_STATUS_OP_TGT_SPEED_CHANGE;
        uint16_t data_length = sizeof(uint16_t);

        status_value.target_speed = ftms_target_param.target_speed;
        ftms_status_notify(conn_id, ftms_id, opcode, status_value, data_length);
    }
 * \endcode
 */
#if (FTMS_CHAR_FTMS_CTRL_PNT_SUPPORT || FTMS_CHAR_FTMS_STATUS_SUPPORT)
bool ftms_status_notify(uint8_t conn_id, T_SERVER_ID service_id, uint8_t opcode,
                        T_FTMS_CP_PARAMETER param, uint8_t param_len);
#endif

#if FTMS_CHAR_TRAINING_STATUS_SUPPORT
bool ftms_train_status_read_confirm(uint8_t conn_id, T_SERVER_ID service_id,
                                    uint8_t *p_data, uint16_t len, T_APP_RESULT cause);

/**
 * @brief       Send fitness machine status notification data.
 *
 *
 * @param[in]   conn_id               Connection id.
 * @param[in]   service_id            Service id.
 * @param[in]   string_len            Training status string length.
 * @param[in]   p_train_status_data   Pointer to data to notify.
 * @return Operation result.
 * @retval true Operation success.
 * @retval false Operation failure.
 *
 * <b>Example usage</b>
 * \code{.c}
    void test(uint8_t conn_id)
    {
        uint8_t string[25] = {'f', 't', 'm', 's', 't', 'r', 'a', 'i', 'n', 's',
                                  't', 'a', 't', 'u', 's'};
        uint16_t string_len = sizeof(string);
        ftms_train_status.train_status_string = os_mem_zalloc(RAM_TYPE_DATA_ON, string_len);
        memcpy(ftms_train_status.train_status_string, string, string_len);
        ftms_train_status_notify(conn_id, ftms_id, string_len, &ftms_train_status);
    }
 * \endcode
 */
bool ftms_train_status_notify(uint8_t conn_id, T_SERVER_ID service_id, uint16_t string_len,
                              T_FTMS_TRAIN_STATUS *p_train_status_data);
#endif

/**
 * @brief       Clear flags if procedure fail or disconnect.
 *
 * <b>Example usage</b>
 * \code{.c}
    void app_handle_conn_state_evt(uint8_t conn_id, T_GAP_CONN_STATE new_state, uint16_t disc_cause)
    {
        APP_PRINT_INFO4("app_handle_conn_state_evt: conn_id %d old_state %d new_state %d, disc_cause 0x%x",
                        conn_id, gap_conn_state, new_state, disc_cause);
        switch (new_state)
        {
        case GAP_CONN_STATE_DISCONNECTED:
            {
                if ((disc_cause != (HCI_ERR | HCI_ERR_REMOTE_USER_TERMINATE))
                    && (disc_cause != (HCI_ERR | HCI_ERR_LOCAL_HOST_TERMINATE)))
                {
                    APP_PRINT_ERROR1("app_handle_conn_state_evt: connection lost cause 0x%x", disc_cause);
                }

                ftms_flags_clear();
            }
            break;
        }
    }
 * \endcode
 */
void ftms_flags_clear(void);

/** @} End of FTMS_Exported_Functions */

/** @} End of FTMS */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _FTMS_H_ */
