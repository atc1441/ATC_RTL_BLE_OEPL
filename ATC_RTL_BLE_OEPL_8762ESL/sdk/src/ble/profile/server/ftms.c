/****************************************************************************************
*     Copyright(c) 2017, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************

  * @file     ftms.c
  * @brief    fitness machine service source file.
  * @details  Interface to access the fitness machine service.
  * @author
  * @date
  * @version  v1.0
  * *************************************************************************************
  */

#include "trace.h"
#include <string.h>
#include "gatt.h"
#include "srv_uuid.h"
#include "gap_conn_le.h"
#include "os_mem.h"
#include "bt_types.h"
#include "ftms.h"

/********************************************************************************************************
* local static variables defined here, only used in this source file.
********************************************************************************************************/
#if FTMS_CHAR_TREADMILL_DATA_SUPPORT
#define FTMS_TM_DATA_FLAG_LEN                       2
#define FTMS_TM_TOTAL_DISTANCE_LEN                  3
#define FTMS_TM_INCLINATION_RAMP_ANGLE_LEN          4
#define FTMS_TM_ELEVATION_GAIN_LEN                  4
#define FTMS_TM_EXPENDED_ENERGY_LEN                 5
#define FTMS_TM_BELT_FORCE_POWER_OUTPUT_LEN         4
#define FTMS_TM_INSTANTANEOUS_SPEED_LEN             2
#endif
#if FTMS_CHAR_STEP_CLIMBER_DATA_SUPPORT
#define FTMS_STEPC_DATA_FLAG_LEN                    2
#define FTMS_STEPC_FLOOR_STEP_COUNT_LEN             4
#define FTMS_STEPC_EXPENDED_ENERGY_LEN              5
#endif
#if FTMS_CHAR_ROWER_DATA_SUPPORT
#define FTMS_RW_DATA_FLAG_LEN                       2
#define FTMS_RW_STROKE_RATE_COUNT_LEN               3
#define FTMS_RW_TOTAL_DISTANCE_LEN                  3
#define FTMS_RW_EXPENDED_ENERGY_LEN                 5
#endif
#if FTMS_CHAR_INDOOR_BIKE_DATA_SUPPORT
#define FTMS_IB_DATA_FLAG_LEN                       2
#define FTMS_IB_TOTAL_DISTANCE_LEN                  3
#define FTMS_IB_EXPENDED_ENERGY_LEN                 5
#define FTMS_IB_INSTANTANEOUS_SPEED_LEN             2
#endif
#if FTMS_CHAR_CROSS_TRAINER_DATA_SUPPORT
#define FTMS_CT_DATA_FLAG_LEN                       3
#define FTMS_CT_TOTAL_DISTANCE_LEN                  3
#define FTMS_CT_STEP_PER_MINUTE_STEP_RATE_LEN       4
#define FTMS_CT_ELEVATION_GAIN_LEN                  4
#define FTMS_CT_INCLINATION_RAMP_ANGLE_LEN          4
#define FTMS_CT_EXPENDED_ENERGY_LEN                 5
#define FTMS_CT_INSTANTANEOUS_SPEED_LEN             2
#endif
#if FTMS_CHAR_STAIR_CLIMBER_DATA_SUPPORT
#define FTMS_STAIRC_DATA_FLAG_LEN                   2
#define FTMS_STAIRC_EXPENDED_ENERGY_LEN             5
#define FTMS_STAIRC_FLOORS_LEN                      2
#endif

#define FTMS_MAX_CTL_PNT_PARAM_LEN                  18

typedef struct
{
    uint16_t cur_flag;
    uint16_t send_flag;
    uint16_t data_len;
    uint16_t offset;
    uint8_t no_more_data;
    uint32_t ct_cur_flag;
    uint32_t ct_send_flag;
} T_FTMS_NOTIFY_FLAG;

typedef enum
{
    FTMS_STATUS_START_RESUME = 0x01,
    FTMS_STATUS_STOP_PAUSE,
} T_FTMS_STATUS;

typedef struct
{
    uint8_t cur_length;
    uint8_t param[FTMS_MAX_CTL_PNT_PARAM_LEN];
} T_FTMS_CONTROL_POINT;

typedef struct
{
    uint16_t ftms_treadmill_data_notify_enable: 1;
    uint16_t ftms_step_climber_data_notify_enable: 1;
    uint16_t ftms_rower_data_notify_enable: 1;
    uint16_t ftms_indoor_bike_data_notify_enable: 1;
    uint16_t ftms_cp_indicate_enable: 1;
    uint16_t ftms_status_notify_enable: 1;
    uint16_t ftms_train_status_notify_enable: 1;
    uint16_t ftms_cross_trainer_data_notify_enable: 1;
    uint16_t ftms_stair_climber_data_notify_enable: 1;
    uint16_t rfu: 7;
} T_FTMS_NOTIFY_INDICATE_FLAG;

typedef struct
{
    T_FTMS_FEATURE ftms_feature;
#if FTMS_CHAR_SUPPORT_SPEED_RANGE_SUPPORT
    T_FTMS_SUPPORT_SPEED_RANGE support_speed_range;
#endif
#if FTMS_CHAR_SUPPORT_INCLINATION_RANGE_SUPPORT
    T_FTMS_SUPPORT_INCLINATION_RANGE support_inclination_range;
#endif
#if FTMS_CHAR_SUPPORT_RESISTANCE_LEVEL_RANGE_SUPPORT
    T_FTMS_SUPPORT_RESISTANCE_LEVEL_RANGE support_resistance_level_range;
#endif
#if FTMS_CHAR_SUPPORT_POWER_RANGE_SUPPORT
    T_FTMS_SUPPORT_POWER_RANGE support_power_range;
#endif
#if FTMS_CHAR_SUPPORT_HR_RANGE_SUPPORT
    T_FTMS_SUPPORT_HR_RANGE support_heart_rate_range;
#endif
#if FTMS_CHAR_FTMS_CTRL_PNT_SUPPORT
    T_FTMS_CONTROL_POINT ftms_control_point;
    bool control_permission;
    uint8_t ftms_cur_status;
    uint8_t ftms_cp_indication_status_flag;
#if FTMS_CHAR_CP_SPIN_DOWN_CTRL_SUPPORT
    T_FTMS_SPIN_DOWN_RESP_PARAM ftms_sd_rsp_param;
#endif
#endif
    T_FTMS_NOTIFY_INDICATE_FLAG ftms_notify_indicate_flag;
#if FTMS_CHAR_TREADMILL_DATA_SUPPORT
    uint16_t treadmill_data_flag;
    uint8_t *ftms_treadmill_notify_data;
    uint8_t *ftms_treadmill_send_data;
    T_FTMS_NOTIFY_FLAG ftms_treadmill_notify_flag;
#endif
#if FTMS_CHAR_STEP_CLIMBER_DATA_SUPPORT
    uint16_t step_climber_data_flag;
    uint8_t *ftms_step_climber_notify_data;
    uint8_t *ftms_step_climber_send_data;
    T_FTMS_NOTIFY_FLAG ftms_step_climber_notify_flag;
#endif
#if FTMS_CHAR_ROWER_DATA_SUPPORT
    uint16_t rower_data_flag;
    uint8_t *ftms_rower_notify_data;
    uint8_t *ftms_rower_send_data;
    T_FTMS_NOTIFY_FLAG ftms_rower_notify_flag;
#endif
#if FTMS_CHAR_INDOOR_BIKE_DATA_SUPPORT
    uint16_t indoor_bike_data_flag;
    uint8_t *ftms_indoor_bike_notify_data;
    uint8_t *ftms_indoor_bike_send_data;
    T_FTMS_NOTIFY_FLAG ftms_indoor_bike_notify_flag;
#endif
#if FTMS_CHAR_CROSS_TRAINER_DATA_SUPPORT
    uint32_t cross_trainer_data_flag;
    uint8_t *ftms_cross_trainer_notify_data;
    uint8_t *ftms_cross_trainer_send_data;
    T_FTMS_NOTIFY_FLAG ftms_cross_trainer_notify_flag;
#endif
#if FTMS_CHAR_STAIR_CLIMBER_DATA_SUPPORT
    uint16_t stair_climber_data_flag;
    uint8_t *ftms_stair_climber_notify_data;
    uint8_t *ftms_stair_climber_send_data;
    T_FTMS_NOTIFY_FLAG ftms_stair_climber_notify_flag;
#endif
    T_SRV_CHAR_TBL *ftms_srv_char_tbl;
} T_FTMS_PARAM;

static T_FTMS_PARAM ftms_var;
P_FUN_SERVER_GENERAL_CB pfn_ftms_cb;

/** @brief  profile/service definition.  */
const T_ATTRIB_APPL ftms_att_tbl[] =
{
    /* <<Primary Service>>, ..0 */
    {
        (ATTRIB_FLAG_VALUE_INCL | ATTRIB_FLAG_LE),  /* wFlags     */
        {                                           /* bTypeValue */
            LO_WORD(GATT_UUID_PRIMARY_SERVICE),
            HI_WORD(GATT_UUID_PRIMARY_SERVICE),
            LO_WORD(GATT_UUID_FITNESS_MACHINE),      /* service UUID */
            HI_WORD(GATT_UUID_FITNESS_MACHINE)
        },
        UUID_16BIT_SIZE,                            /* bValueLen     */
        NULL,                                       /* pValueContext */
        GATT_PERM_READ                              /* wPermissions  */
    },
    /* <<Characteristic>>, .. Fitness Machine Feature*/
    {
        ATTRIB_FLAG_VALUE_INCL,                     /* wFlags */
        {                                           /* bTypeValue */
            LO_WORD(GATT_UUID_CHARACTERISTIC),
            HI_WORD(GATT_UUID_CHARACTERISTIC),

            (GATT_CHAR_PROP_READ)                   /* characteristic properties */
            /* characteristic UUID not needed here, is UUID of next attrib. */
        },
        1,                                          /* bValueLen */
        NULL,
        GATT_PERM_READ                              /* wPermissions */
    },
    /*---Fitness Machine Feature characteristic value ---*/
    {
        ATTRIB_FLAG_VALUE_APPL,                     /* wFlags */
        {                                           /* bTypeValue */
            LO_WORD(GATT_UUID_FITNESS_MACHINE_FEATURE),
            HI_WORD(GATT_UUID_FITNESS_MACHINE_FEATURE)
        },
        0,                                          /* bValueLen */
        NULL,
        (GATT_PERM_READ)                            /* wPermissions */
    },
#if FTMS_CHAR_TREADMILL_DATA_SUPPORT
    /* <<Characteristic>>, .. Treadmill Data*/
    {
        ATTRIB_FLAG_VALUE_INCL,                     /* wFlags */
        {                                           /* bTypeValue */
            LO_WORD(GATT_UUID_CHARACTERISTIC),
            HI_WORD(GATT_UUID_CHARACTERISTIC),
            GATT_CHAR_PROP_NOTIFY                   /* characteristic properties */
            /* characteristic UUID not needed here, is UUID of next attrib. */
        },
        1,                                          /* bValueLen */
        NULL,
        GATT_PERM_READ                              /* wPermissions */
    },
    /*--- Treadmill Data characteristic value ---*/
    {
        ATTRIB_FLAG_VALUE_APPL,                     /* wFlags */
        {                                           /* bTypeValue */
            LO_WORD(GATT_UUID_TREADMILL_DATA),
            HI_WORD(GATT_UUID_TREADMILL_DATA)
        },
        0,                                          /* variable size */
        NULL,
        GATT_PERM_NOTIF_IND                         /* wPermissions */
    },
    /* client characteristic configuration */
    {
        (ATTRIB_FLAG_VALUE_INCL |                   /* wFlags */
         ATTRIB_FLAG_CCCD_APPL),
        {                                           /* bTypeValue */
            LO_WORD(GATT_UUID_CHAR_CLIENT_CONFIG),
            HI_WORD(GATT_UUID_CHAR_CLIENT_CONFIG),
            /* NOTE: this value has an instantiation for each client, a write to */
            /* this attribute does not modify this default value:                */
            LO_WORD(GATT_CLIENT_CHAR_CONFIG_DEFAULT), /* client char. config. bit field */
            HI_WORD(GATT_CLIENT_CHAR_CONFIG_DEFAULT)
        },
        2,                                          /* bValueLen */
        NULL,
        (GATT_PERM_READ | GATT_PERM_WRITE)          /* wPermissions */
    },
#endif
#if FTMS_CHAR_STEP_CLIMBER_DATA_SUPPORT
    /* <<Characteristic>>, .. Step Climber Data*/
    {
        ATTRIB_FLAG_VALUE_INCL,                     /* wFlags */
        {                                           /* bTypeValue */
            LO_WORD(GATT_UUID_CHARACTERISTIC),
            HI_WORD(GATT_UUID_CHARACTERISTIC),
            GATT_CHAR_PROP_NOTIFY                   /* characteristic properties */
            /* characteristic UUID not needed here, is UUID of next attrib. */
        },
        1,                                          /* bValueLen */
        NULL,
        GATT_PERM_READ                              /* wPermissions */
    },
    /*--- Step Climber Data characteristic value ---*/
    {
        ATTRIB_FLAG_VALUE_APPL,                     /* wFlags */
        {                                           /* bTypeValue */
            LO_WORD(GATT_UUID_STEP_CLIMBER_DATA),
            HI_WORD(GATT_UUID_STEP_CLIMBER_DATA),
        },
        0,                                          /* bValueLen */
        NULL,
        GATT_PERM_NOTIF_IND                         /* wPermissions */
    },
    /* client characteristic configuration */
    {
        (ATTRIB_FLAG_VALUE_INCL |                   /* wFlags */
         ATTRIB_FLAG_CCCD_APPL),
        {                                           /* bTypeValue */
            LO_WORD(GATT_UUID_CHAR_CLIENT_CONFIG),
            HI_WORD(GATT_UUID_CHAR_CLIENT_CONFIG),
            /* NOTE: this value has an instantiation for each client, a write to */
            /* this attribute does not modify this default value:                */
            LO_WORD(GATT_CLIENT_CHAR_CONFIG_DEFAULT), /* client char. config. bit field */
            HI_WORD(GATT_CLIENT_CHAR_CONFIG_DEFAULT)
        },
        2,                                          /* bValueLen */
        NULL,
        (GATT_PERM_READ | GATT_PERM_WRITE)          /* wPermissions */
    },
#endif
#if FTMS_CHAR_ROWER_DATA_SUPPORT
    /* <<Characteristic>>, .. Rower Data*/
    {
        ATTRIB_FLAG_VALUE_INCL,                     /* wFlags */
        {                                           /* bTypeValue */
            LO_WORD(GATT_UUID_CHARACTERISTIC),
            HI_WORD(GATT_UUID_CHARACTERISTIC),
            GATT_CHAR_PROP_NOTIFY                   /* characteristic properties */
            /* characteristic UUID not needed here, is UUID of next attrib. */
        },
        1,                                          /* bValueLen */
        NULL,
        GATT_PERM_READ                              /* wPermissions */
    },
    /*--- Rower Data characteristic value ---*/
    {
        ATTRIB_FLAG_VALUE_APPL,                     /* wFlags */
        {                                           /* bTypeValue */
            LO_WORD(GATT_UUID_ROWER_DATA),
            HI_WORD(GATT_UUID_ROWER_DATA)
        },
        0,                                          /* bValueLen, 0 : variable length */
        NULL,
        GATT_PERM_NOTIF_IND                         /* wPermissions */
    },
    /* client characteristic configuration */
    {
        (ATTRIB_FLAG_VALUE_INCL |                   /* wFlags */
         ATTRIB_FLAG_CCCD_APPL),
        {                                           /* bTypeValue */
            LO_WORD(GATT_UUID_CHAR_CLIENT_CONFIG),
            HI_WORD(GATT_UUID_CHAR_CLIENT_CONFIG),
            /* NOTE: this value has an instantiation for each client, a write to */
            /* this attribute does not modify this default value:                */
            LO_WORD(GATT_CLIENT_CHAR_CONFIG_DEFAULT), /* client char. config. bit field */
            HI_WORD(GATT_CLIENT_CHAR_CONFIG_DEFAULT)
        },
        2,                                          /* bValueLen */
        NULL,
        (GATT_PERM_READ | GATT_PERM_WRITE)          /* wPermissions */
    },
#endif
#if GATT_UUID_INDOOR_BIKE_DATA
    /* <<Characteristic>>, .. Indoor Bike Data*/
    {
        ATTRIB_FLAG_VALUE_INCL,                     /* wFlags */
        {                                           /* bTypeValue */
            LO_WORD(GATT_UUID_CHARACTERISTIC),
            HI_WORD(GATT_UUID_CHARACTERISTIC),
            GATT_CHAR_PROP_NOTIFY                   /* characteristic properties */
            /* characteristic UUID not needed here, is UUID of next attrib. */
        },
        1,                                          /* bValueLen */
        NULL,
        GATT_PERM_READ                              /* wPermissions */
    },
    /*--- Indoor Bike Data characteristic value ---*/
    {
        ATTRIB_FLAG_VALUE_APPL,                     /* wFlags */
        {                                           /* bTypeValue */
            LO_WORD(GATT_UUID_INDOOR_BIKE_DATA),
            HI_WORD(GATT_UUID_INDOOR_BIKE_DATA)
        },
        0,                                          /* bValueLen, 0 : variable length */
        NULL,
        GATT_PERM_NOTIF_IND                         /* wPermissions */
    },
    /* client characteristic configuration */
    {
        (ATTRIB_FLAG_VALUE_INCL |                   /* wFlags */
         ATTRIB_FLAG_CCCD_APPL),
        {                                           /* bTypeValue */
            LO_WORD(GATT_UUID_CHAR_CLIENT_CONFIG),
            HI_WORD(GATT_UUID_CHAR_CLIENT_CONFIG),
            /* NOTE: this value has an instantiation for each client, a write to */
            /* this attribute does not modify this default value:                */
            LO_WORD(GATT_CLIENT_CHAR_CONFIG_DEFAULT), /* client char. config. bit field */
            HI_WORD(GATT_CLIENT_CHAR_CONFIG_DEFAULT)
        },
        2,                                          /* bValueLen */
        NULL,
        (GATT_PERM_READ | GATT_PERM_WRITE)          /* wPermissions */
    },
#endif
#if FTMS_CHAR_CROSS_TRAINER_DATA_SUPPORT
    /* <<Characteristic>>, .. Cross Trainer Data*/
    {
        ATTRIB_FLAG_VALUE_INCL,                     /* wFlags */
        {                                           /* bTypeValue */
            LO_WORD(GATT_UUID_CHARACTERISTIC),
            HI_WORD(GATT_UUID_CHARACTERISTIC),
            GATT_CHAR_PROP_NOTIFY                   /* characteristic properties */
            /* characteristic UUID not needed here, is UUID of next attrib. */
        },
        1,                                          /* bValueLen */
        NULL,
        GATT_PERM_READ                              /* wPermissions */
    },
    /*--- Cross Trainer Data characteristic value ---*/
    {
        ATTRIB_FLAG_VALUE_APPL,                     /* wFlags */
        {                                           /* bTypeValue */
            LO_WORD(GATT_UUID_CROSS_TRAINER_DATA),
            HI_WORD(GATT_UUID_CROSS_TRAINER_DATA)
        },
        0,                                          /* bValueLen, 0 : variable length */
        NULL,
        GATT_PERM_NOTIF_IND                         /* wPermissions */
    },
    /* client characteristic configuration */
    {
        (ATTRIB_FLAG_VALUE_INCL |                   /* wFlags */
         ATTRIB_FLAG_CCCD_APPL),
        {                                           /* bTypeValue */
            LO_WORD(GATT_UUID_CHAR_CLIENT_CONFIG),
            HI_WORD(GATT_UUID_CHAR_CLIENT_CONFIG),
            /* NOTE: this value has an instantiation for each client, a write to */
            /* this attribute does not modify this default value:                */
            LO_WORD(GATT_CLIENT_CHAR_CONFIG_DEFAULT), /* client char. config. bit field */
            HI_WORD(GATT_CLIENT_CHAR_CONFIG_DEFAULT)
        },
        2,                                          /* bValueLen */
        NULL,
        (GATT_PERM_READ | GATT_PERM_WRITE)          /* wPermissions */
    },
#endif
#if FTMS_CHAR_STAIR_CLIMBER_DATA_SUPPORT
    /* <<Characteristic>>, .. Stair Climber Data*/
    {
        ATTRIB_FLAG_VALUE_INCL,                     /* wFlags */
        {                                           /* bTypeValue */
            LO_WORD(GATT_UUID_CHARACTERISTIC),
            HI_WORD(GATT_UUID_CHARACTERISTIC),
            GATT_CHAR_PROP_NOTIFY                   /* characteristic properties */
            /* characteristic UUID not needed here, is UUID of next attrib. */
        },
        1,                                          /* bValueLen */
        NULL,
        GATT_PERM_READ                              /* wPermissions */
    },
    /*--- Stair Climber Data characteristic value ---*/
    {
        ATTRIB_FLAG_VALUE_APPL,                     /* wFlags */
        {                                           /* bTypeValue */
            LO_WORD(GATT_UUID_STAIR_CLIMBER_DATA),
            HI_WORD(GATT_UUID_STAIR_CLIMBER_DATA)
        },
        0,                                          /* bValueLen, 0 : variable length */
        NULL,
        GATT_PERM_NOTIF_IND                         /* wPermissions */
    },
    /* client characteristic configuration */
    {
        (ATTRIB_FLAG_VALUE_INCL |                   /* wFlags */
         ATTRIB_FLAG_CCCD_APPL),
        {                                           /* bTypeValue */
            LO_WORD(GATT_UUID_CHAR_CLIENT_CONFIG),
            HI_WORD(GATT_UUID_CHAR_CLIENT_CONFIG),
            /* NOTE: this value has an instantiation for each client, a write to */
            /* this attribute does not modify this default value:                */
            LO_WORD(GATT_CLIENT_CHAR_CONFIG_DEFAULT), /* client char. config. bit field */
            HI_WORD(GATT_CLIENT_CHAR_CONFIG_DEFAULT)
        },
        2,                                          /* bValueLen */
        NULL,
        (GATT_PERM_READ | GATT_PERM_WRITE)          /* wPermissions */
    },
#endif
#if FTMS_CHAR_TRAINING_STATUS_SUPPORT
    /* <<Characteristic>>, .. Training Status */
    {
        ATTRIB_FLAG_VALUE_INCL,                     /* wFlags */
        {                                           /* bTypeValue */
            LO_WORD(GATT_UUID_CHARACTERISTIC),
            HI_WORD(GATT_UUID_CHARACTERISTIC),
            (GATT_CHAR_PROP_READ |                  /* characteristic properties */
             GATT_CHAR_PROP_NOTIFY)
            /* characteristic UUID not needed here, is UUID of next attrib. */
        },
        1,                                          /* bValueLen */
        NULL,
        GATT_PERM_READ                              /* wPermissions */
    },
    /*--- Training Status characteristic value ---*/
    {
        ATTRIB_FLAG_VALUE_APPL,                     /* wFlags */
        {                                           /* bTypeValue */
            LO_WORD(GATT_UUID_TRAINING_STATUS),
            HI_WORD(GATT_UUID_TRAINING_STATUS)
        },
        0,                                          /* bValueLen, 0 : variable length */
        NULL,
        (GATT_PERM_READ | GATT_PERM_NOTIF_IND)      /* wPermissions */
    },
    /* client characteristic configuration */
    {
        (ATTRIB_FLAG_VALUE_INCL |                   /* wFlags */
         ATTRIB_FLAG_CCCD_APPL),
        {                                           /* bTypeValue */
            LO_WORD(GATT_UUID_CHAR_CLIENT_CONFIG),
            HI_WORD(GATT_UUID_CHAR_CLIENT_CONFIG),
            /* NOTE: this value has an instantiation for each client, a write to */
            /* this attribute does not modify this default value:                */
            LO_WORD(GATT_CLIENT_CHAR_CONFIG_DEFAULT), /* client char. config. bit field */
            HI_WORD(GATT_CLIENT_CHAR_CONFIG_DEFAULT)
        },
        2,                                          /* bValueLen */
        NULL,
        (GATT_PERM_READ | GATT_PERM_WRITE)          /* wPermissions */
    },
#endif
#if FTMS_CHAR_SUPPORT_SPEED_RANGE_SUPPORT
    /* <<Characteristic>>, .. Supported Speed Range*/
    {
        ATTRIB_FLAG_VALUE_INCL,                     /* wFlags */
        {                                           /* bTypeValue */
            LO_WORD(GATT_UUID_CHARACTERISTIC),
            HI_WORD(GATT_UUID_CHARACTERISTIC),
            GATT_CHAR_PROP_READ                     /* characteristic properties */
            /* characteristic UUID not needed here, is UUID of next attrib. */
        },
        1,                                          /* bValueLen */
        NULL,
        GATT_PERM_READ                              /* wPermissions */
    },
    /*--- Supported Speed Range characteristic value ---*/
    {
        ATTRIB_FLAG_VALUE_APPL,                     /* wFlags */
        {                                           /* bTypeValue */
            LO_WORD(GATT_UUID_SUPPORT_SPEED_RANGE),
            HI_WORD(GATT_UUID_SUPPORT_SPEED_RANGE)
        },
        0,                                          /* bValueLen, 0 : variable length */
        NULL,
        GATT_PERM_READ                         /* wPermissions */
    },
#endif
#if FTMS_CHAR_SUPPORT_INCLINATION_RANGE_SUPPORT
    /* <<Characteristic>>, .. Supported Inclination Range*/
    {
        ATTRIB_FLAG_VALUE_INCL,                     /* wFlags */
        {                                           /* bTypeValue */
            LO_WORD(GATT_UUID_CHARACTERISTIC),
            HI_WORD(GATT_UUID_CHARACTERISTIC),
            GATT_CHAR_PROP_READ                     /* characteristic properties */
            /* characteristic UUID not needed here, is UUID of next attrib. */
        },
        1,                                          /* bValueLen */
        NULL,
        GATT_PERM_READ                              /* wPermissions */
    },
    /*--- Supported Inclination Range characteristic value ---*/
    {
        ATTRIB_FLAG_VALUE_APPL,                     /* wFlags */
        {                                           /* bTypeValue */
            LO_WORD(GATT_UUID_SUPPORT_INCLINATION_RANGE),
            HI_WORD(GATT_UUID_SUPPORT_INCLINATION_RANGE)
        },
        0,                                          /* bValueLen, 0 : variable length */
        NULL,
        GATT_PERM_READ                         /* wPermissions */
    },
#endif
#if FTMS_CHAR_SUPPORT_RESISTANCE_LEVEL_RANGE_SUPPORT
    /* <<Characteristic>>, .. Supported Resistance level Range*/
    {
        ATTRIB_FLAG_VALUE_INCL,                     /* wFlags */
        {                                           /* bTypeValue */
            LO_WORD(GATT_UUID_CHARACTERISTIC),
            HI_WORD(GATT_UUID_CHARACTERISTIC),
            GATT_CHAR_PROP_READ                     /* characteristic properties */
            /* characteristic UUID not needed here, is UUID of next attrib. */
        },
        1,                                          /* bValueLen */
        NULL,
        GATT_PERM_READ                              /* wPermissions */
    },
    /*--- Supported Resistance level Range characteristic value ---*/
    {
        ATTRIB_FLAG_VALUE_APPL,                     /* wFlags */
        {                                           /* bTypeValue */
            LO_WORD(GATT_UUID_SUPPORT_RESISTANCE_LEVEL_RANGE),
            HI_WORD(GATT_UUID_SUPPORT_RESISTANCE_LEVEL_RANGE)
        },
        0,                                          /* bValueLen, 0 : variable length */
        NULL,
        GATT_PERM_READ                         /* wPermissions */
    },
#endif
#if FTMS_CHAR_SUPPORT_HR_RANGE_SUPPORT
    /* <<Characteristic>>, .. Supported Heart Rate Range*/
    {
        ATTRIB_FLAG_VALUE_INCL,                     /* wFlags */
        {                                           /* bTypeValue */
            LO_WORD(GATT_UUID_CHARACTERISTIC),
            HI_WORD(GATT_UUID_CHARACTERISTIC),
            GATT_CHAR_PROP_READ                     /* characteristic properties */
            /* characteristic UUID not needed here, is UUID of next attrib. */
        },
        1,                                          /* bValueLen */
        NULL,
        GATT_PERM_READ                              /* wPermissions */
    },
    /*--- Supported Heart Rate Range characteristic value ---*/
    {
        ATTRIB_FLAG_VALUE_APPL,                     /* wFlags */
        {                                           /* bTypeValue */
            LO_WORD(GATT_UUID_SUPPORT_HEART_RATE_RANGE),
            HI_WORD(GATT_UUID_SUPPORT_HEART_RATE_RANGE)
        },
        0,                                          /* bValueLen, 0 : variable length */
        NULL,
        GATT_PERM_READ                         /* wPermissions */
    },
#endif
#if FTMS_CHAR_SUPPORT_POWER_RANGE_SUPPORT
    /* <<Characteristic>>, .. Supported Power Range*/
    {
        ATTRIB_FLAG_VALUE_INCL,                     /* wFlags */
        {                                           /* bTypeValue */
            LO_WORD(GATT_UUID_CHARACTERISTIC),
            HI_WORD(GATT_UUID_CHARACTERISTIC),
            GATT_CHAR_PROP_READ                     /* characteristic properties */
            /* characteristic UUID not needed here, is UUID of next attrib. */
        },
        1,                                          /* bValueLen */
        NULL,
        GATT_PERM_READ                              /* wPermissions */
    },
    /*--- Supported Power Range characteristic value ---*/
    {
        ATTRIB_FLAG_VALUE_APPL,                     /* wFlags */
        {                                           /* bTypeValue */
            LO_WORD(GATT_UUID_SUPPORT_POWER_RANGE),
            HI_WORD(GATT_UUID_SUPPORT_POWER_RANGE)
        },
        0,                                          /* bValueLen, 0 : variable length */
        NULL,
        GATT_PERM_READ                         /* wPermissions */
    },
#endif
#if FTMS_CHAR_FTMS_CTRL_PNT_SUPPORT
    /* <<Characteristic>>, .. Fitness Machine Control Point*/
    {
        ATTRIB_FLAG_VALUE_INCL,                     /* wFlags */
        {                                           /* bTypeValue */
            LO_WORD(GATT_UUID_CHARACTERISTIC),
            HI_WORD(GATT_UUID_CHARACTERISTIC),
            (GATT_CHAR_PROP_WRITE |                 /* characteristic properties */
             GATT_CHAR_PROP_INDICATE)
            /* characteristic UUID not needed here, is UUID of next attrib. */
        },
        1,                                          /* bValueLen */
        NULL,
        GATT_PERM_READ                              /* wPermissions */
    },
    /*--- Fitness Machine Control Point characteristic value ---*/
    {
        ATTRIB_FLAG_VALUE_APPL,                     /* wFlags */
        {                                           /* bTypeValue */
            LO_WORD(GATT_UUID_FTMS_CONTROL_POINT),
            HI_WORD(GATT_UUID_FTMS_CONTROL_POINT)
        },
        0,                                          /* bValueLen, 0 : variable length */
        NULL,
        (GATT_PERM_WRITE_ENCRYPTED_REQ |            /* wPermissions */
         GATT_PERM_NOTIF_IND_ENCRYPTED_REQ)
    },
    /* client characteristic configuration */
    {
        (ATTRIB_FLAG_VALUE_INCL |                   /* wFlags */
         ATTRIB_FLAG_CCCD_APPL),
        {                                           /* bTypeValue */
            LO_WORD(GATT_UUID_CHAR_CLIENT_CONFIG),
            HI_WORD(GATT_UUID_CHAR_CLIENT_CONFIG),
            /* NOTE: this value has an instantiation for each client, a write to */
            /* this attribute does not modify this default value:                */
            LO_WORD(GATT_CLIENT_CHAR_CONFIG_DEFAULT), /* client char. config. bit field */
            HI_WORD(GATT_CLIENT_CHAR_CONFIG_DEFAULT)
        },
        2,                                          /* bValueLen */
        NULL,
        (GATT_PERM_READ | GATT_PERM_WRITE)          /* wPermissions */
    },
#endif
#if (FTMS_CHAR_FTMS_CTRL_PNT_SUPPORT || FTMS_CHAR_FTMS_STATUS_SUPPORT)
    /* <<Characteristic>>, .. Fitness Machine Status*/
    {
        ATTRIB_FLAG_VALUE_INCL,                     /* wFlags */
        {                                           /* bTypeValue */
            LO_WORD(GATT_UUID_CHARACTERISTIC),
            HI_WORD(GATT_UUID_CHARACTERISTIC),
            GATT_CHAR_PROP_NOTIFY                   /* characteristic properties */
            /* characteristic UUID not needed here, is UUID of next attrib. */
        },
        1,                                          /* bValueLen */
        NULL,
        GATT_PERM_READ                              /* wPermissions */
    },
    /*--- Fitness Machine Status characteristic value ---*/
    {
        ATTRIB_FLAG_VALUE_APPL,                     /* wFlags */
        {                                           /* bTypeValue */
            LO_WORD(GATT_UUID_FITNESS_MACHINE_STATUS),
            HI_WORD(GATT_UUID_FITNESS_MACHINE_STATUS)
        },
        0,                                          /* bValueLen, 0 : variable length */
        NULL,
        GATT_PERM_NOTIF_IND                         /* wPermissions */
    },
    /* client characteristic configuration */
    {
        (ATTRIB_FLAG_VALUE_INCL |                   /* wFlags */
         ATTRIB_FLAG_CCCD_APPL),
        {                                           /* bTypeValue */
            LO_WORD(GATT_UUID_CHAR_CLIENT_CONFIG),
            HI_WORD(GATT_UUID_CHAR_CLIENT_CONFIG),
            /* NOTE: this value has an instantiation for each client, a write to */
            /* this attribute does not modify this default value:                */
            LO_WORD(GATT_CLIENT_CHAR_CONFIG_DEFAULT), /* client char. config. bit field */
            HI_WORD(GATT_CLIENT_CHAR_CONFIG_DEFAULT)
        },
        2,                                          /* bValueLen */
        NULL,
        (GATT_PERM_READ | GATT_PERM_WRITE)          /* wPermissions */
    },
#endif
};

const static uint16_t ftms_attr_tbl_size = sizeof(ftms_att_tbl);

bool ftms_set_parameter(T_FTMS_PARAM_TYPE param_type, uint8_t len, void *p_value)
{
    bool ret = true;

    PROFILE_PRINT_INFO1("ftms_set_parameter: param_type 0x%x", param_type);

    switch (param_type)
    {
    case FTMS_PARAM_FTMS_FEATURE:
        {
            memcpy(&ftms_var.ftms_feature, p_value, len);
        }
        break;

#if FTMS_CHAR_SUPPORT_SPEED_RANGE_SUPPORT
    case FTMS_PARAM_SUPPORT_SPEED_RANGE:
        {
            if (ftms_var.ftms_feature.target_setting_field & FTMS_TGT_SET_SPEED_SPT_MASK)
            {
                memcpy(&ftms_var.support_speed_range, p_value, len);
            }
            else
            {
                PROFILE_PRINT_ERROR0("ftms_set_parameter:supported speed range not support!");
            }
        }
        break;
#endif

#if FTMS_CHAR_SUPPORT_INCLINATION_RANGE_SUPPORT
    case FTMS_PARAM_SUPPORT_INCLINATION_RANGE:
        {
            if (ftms_var.ftms_feature.target_setting_field & FTMS_TGT_SET_INCLINATION_SPT_MASK)
            {
                memcpy(&ftms_var.support_inclination_range, p_value, len);
            }
            else
            {
                PROFILE_PRINT_ERROR0("ftms_set_parameter:supported inclination range not support!");
            }
        }
        break;
#endif

#if FTMS_CHAR_SUPPORT_RESISTANCE_LEVEL_RANGE_SUPPORT
    case FTMS_PARAM_SUPPORT_RESISTANCE_LEVEL_RANGE:
        {
            if (ftms_var.ftms_feature.target_setting_field & FTMS_TGT_SET_RESISTANCE_SPT_MASK)
            {
                memcpy(&ftms_var.support_resistance_level_range, p_value, len);
            }
            else
            {
                PROFILE_PRINT_ERROR0("ftms_set_parameter:supported resistance level range not support!");
            }
        }
        break;
#endif

#if FTMS_CHAR_SUPPORT_POWER_RANGE_SUPPORT
    case FTMS_PARAM_SUPPORT_POWER_RANGE:
        {
            if (ftms_var.ftms_feature.target_setting_field & FTMS_TGT_SET_POWER_SPT_MASK)
            {
                memcpy(&ftms_var.support_power_range, p_value, len);
            }
            else
            {
                PROFILE_PRINT_ERROR0("ftms_set_parameter:supported power range not support!");
            }
        }
        break;
#endif

#if FTMS_CHAR_SUPPORT_HR_RANGE_SUPPORT
    case FTMS_PARAM_SUPPORT_HR_RANGE:
        {
            if (ftms_var.ftms_feature.target_setting_field & FTMS_TGT_SET_HR_SPT_MASK)
            {
                memcpy(&ftms_var.support_heart_rate_range, p_value, len);
            }
            else
            {
                PROFILE_PRINT_ERROR0("ftms_set_parameter:supported heart rate range not support!");
            }
        }
        break;
#endif

#if FTMS_CHAR_TREADMILL_DATA_SUPPORT
    case FTMS_PARAM_TREADMILL_DATA_FLAG:
        {
            uint8_t *p = (uint8_t *)p_value;
            LE_STREAM_TO_UINT16(ftms_var.treadmill_data_flag, p);
        }
        break;
#endif

#if FTMS_CHAR_STEP_CLIMBER_DATA_SUPPORT
    case FTMS_PARAM_STEP_CLIMBER_DATA_FLAG:
        {
            uint8_t *p = (uint8_t *)p_value;
            LE_STREAM_TO_UINT16(ftms_var.step_climber_data_flag, p);
        }
        break;
#endif

#if FTMS_CHAR_ROWER_DATA_SUPPORT
    case FTMS_PARAM_ROWER_DATA_FLAG:
        {
            uint8_t *p = (uint8_t *)p_value;
            LE_STREAM_TO_UINT16(ftms_var.rower_data_flag, p);
        }
        break;
#endif

#if FTMS_CHAR_INDOOR_BIKE_DATA_SUPPORT
    case FTMS_PARAM_INDOOR_BIKE_DATA_FLAG:
        {
            uint8_t *p = (uint8_t *)p_value;
            LE_STREAM_TO_UINT16(ftms_var.indoor_bike_data_flag, p);
        }
        break;
#endif

#if FTMS_CHAR_FTMS_CTRL_PNT_SUPPORT
    case FTMS_PARAM_CTL_PNT_PROG_CLR:
        {
            ftms_var.ftms_control_point.param[0] = FTMS_CP_OPCODE_RESERVED;
        }
        break;
#endif

#if FTMS_CHAR_CP_SPIN_DOWN_CTRL_SUPPORT
    case FTMS_PARAM_SD_RESP_PARAM:
        {
            memcpy(&ftms_var.ftms_sd_rsp_param, p_value, len);
        }
        break;
#endif

#if FTMS_CHAR_CROSS_TRAINER_DATA_SUPPORT
    case FTMS_PARAM_CROSS_TRAINER_DATA_FLAG:
        {
            uint8_t *p = (uint8_t *)p_value;
            LE_STREAM_TO_UINT24(ftms_var.cross_trainer_data_flag, p);
        }
        break;
#endif

#if FTMS_CHAR_STAIR_CLIMBER_DATA_SUPPORT
    case FTMS_PARAM_STAIR_CLIMBER_DATA_FLAG:
        {
            uint8_t *p = (uint8_t *)p_value;
            LE_STREAM_TO_UINT16(ftms_var.stair_climber_data_flag, p);
        }
        break;
#endif

    default:
        {
            ret = false;
        }
        break;
    }

    return ret;
}

#if FTMS_CHAR_TREADMILL_DATA_SUPPORT
uint16_t ftms_treadmill_data_length(void)
{
    uint16_t data_length = FTMS_TM_INSTANTANEOUS_SPEED_LEN;
    if (ftms_var.treadmill_data_flag & FTMS_TM_AVE_SPEED_PRESENT_MASK)
    {
        data_length += sizeof(uint16_t);
    }
    if (ftms_var.treadmill_data_flag & FTMS_TM_TOTAL_DISTANCE_PRESENT_MASK)
    {
        data_length += FTMS_TM_TOTAL_DISTANCE_LEN;
    }
    if (ftms_var.treadmill_data_flag & FTMS_TM_INCLINATION_RAMP_ANGLE_SET_PRESENT_MASK)
    {
        data_length += FTMS_TM_INCLINATION_RAMP_ANGLE_LEN;
    }
    if (ftms_var.treadmill_data_flag & FTMS_TM_ELEVATION_GAIN_PRESENT_MASK)
    {
        data_length += FTMS_TM_ELEVATION_GAIN_LEN;
    }
    if (ftms_var.treadmill_data_flag & FTMS_TM_INSTANTANEOUS_PACE_PRESENT_MASK)
    {
        data_length += sizeof(uint8_t);
    }
    if (ftms_var.treadmill_data_flag & FTMS_TM_AVERAGE_PACE_PRESENT_MASK)
    {
        data_length += sizeof(uint8_t);
    }
    if (ftms_var.treadmill_data_flag & FTMS_TM_EXPENDED_ENERGY_PRESENT_MASK)
    {
        data_length += FTMS_TM_EXPENDED_ENERGY_LEN;
    }
    if (ftms_var.treadmill_data_flag & FTMS_TM_HEART_RATE_PRESENT_MASK)
    {
        data_length += sizeof(uint8_t);
    }
    if (ftms_var.treadmill_data_flag & FTMS_TM_METABOLIC_EQUIVALENT_PRESENT_MASK)
    {
        data_length += sizeof(uint8_t);
    }
    if (ftms_var.treadmill_data_flag & FTMS_TM_ELAPSED_TIME_PRESENT_MASK)
    {
        data_length += sizeof(int16_t);
    }
    if (ftms_var.treadmill_data_flag & FTMS_TM_REMAIN_TIME_PRESENT_MASK)
    {
        data_length += sizeof(int16_t);
    }
    if (ftms_var.treadmill_data_flag & FTMS_TM_BELT_FORCE_POWER_OUTPUT_PRESENT_MASK)
    {
        data_length += FTMS_TM_BELT_FORCE_POWER_OUTPUT_LEN;
    }

    return data_length;
}

void ftms_save_treadmill_notify_data(uint8_t conn_id, uint16_t data_length,
                                     T_FTMS_TREADMILL_DATA *p_treadmill_data)
{
    ftms_var.ftms_treadmill_notify_data = os_mem_zalloc(RAM_TYPE_DATA_ON, data_length);

    uint16_t offset = 0;
    uint8_t *p;
    p = ftms_var.ftms_treadmill_notify_data + offset;
    LE_UINT16_TO_STREAM(p, p_treadmill_data->instantaneous_speed);
    offset += 2;

    ftms_var.ftms_treadmill_notify_flag.offset = FTMS_TM_INSTANTANEOUS_SPEED_LEN;

    if (ftms_var.treadmill_data_flag & FTMS_TM_AVE_SPEED_PRESENT_MASK)
    {
        if (ftms_var.ftms_feature.features_field & FTMS_FEAT_AVERAGE_SPEED_SPT_MASK)
        {
            p = ftms_var.ftms_treadmill_notify_data + offset;
            LE_UINT16_TO_STREAM(p, p_treadmill_data->average_speed);
        }
        else
        {
            memset(ftms_var.ftms_treadmill_notify_data + offset, 0xFF, 2);
        }

        offset += 2;
    }

    if (ftms_var.treadmill_data_flag & FTMS_TM_TOTAL_DISTANCE_PRESENT_MASK)
    {
        if (ftms_var.ftms_feature.features_field & FTMS_FEAT_TOTAL_DISTANCE_SPT_MASK)
        {
            p = ftms_var.ftms_treadmill_notify_data + offset;
            LE_UINT24_TO_STREAM(p, p_treadmill_data->total_distance);
        }
        else
        {
            memset(ftms_var.ftms_treadmill_notify_data + offset, 0xFF, 3);
        }

        offset += 3;
    }

    if (ftms_var.treadmill_data_flag & FTMS_TM_INCLINATION_RAMP_ANGLE_SET_PRESENT_MASK)
    {
        if (ftms_var.ftms_feature.features_field & FTMS_FEAT_INCLINATION_SPT_MASK)
        {
            p = ftms_var.ftms_treadmill_notify_data + offset;
            LE_UINT16_TO_STREAM(p, p_treadmill_data->inclination);
            offset += 2;

            p = ftms_var.ftms_treadmill_notify_data + offset;
            LE_UINT16_TO_STREAM(p, p_treadmill_data->ramp_angle_set);
            offset += 2;
        }
        else
        {
            uint16_t invalid_data = 0x7FFF;
            p = ftms_var.ftms_treadmill_notify_data + offset;
            LE_UINT16_TO_STREAM(p, invalid_data);
            offset += 2;

            p = ftms_var.ftms_treadmill_notify_data + offset;
            LE_UINT16_TO_STREAM(p, invalid_data);
            offset += 2;
        }
    }

    if (ftms_var.treadmill_data_flag & FTMS_TM_ELEVATION_GAIN_PRESENT_MASK)
    {
        if (ftms_var.ftms_feature.features_field & FTMS_FEAT_ELEVATION_GAIN_SPT_MASK)
        {
            p = ftms_var.ftms_treadmill_notify_data + offset;
            LE_UINT16_TO_STREAM(p, p_treadmill_data->positive_elevation_gain);
            offset += 2;

            p = ftms_var.ftms_treadmill_notify_data + offset;
            LE_UINT16_TO_STREAM(p, p_treadmill_data->negative_elevation_gain);
            offset += 2;
        }
        else
        {
            memset(ftms_var.ftms_treadmill_notify_data + offset, 0xFF, 4);
            offset += 4;
        }
    }

    if (ftms_var.treadmill_data_flag & FTMS_TM_INSTANTANEOUS_PACE_PRESENT_MASK)
    {
        if (ftms_var.ftms_feature.features_field & FTMS_FEAT_PACE_SPT_MASK)
        {
            ftms_var.ftms_treadmill_notify_data[offset] = p_treadmill_data->instantaneous_pace;
        }
        else
        {
            memset(ftms_var.ftms_treadmill_notify_data + offset, 0xFF, 1);
        }

        offset += 1;
    }

    if (ftms_var.treadmill_data_flag & FTMS_TM_AVERAGE_PACE_PRESENT_MASK)
    {
        if (ftms_var.ftms_feature.features_field & FTMS_FEAT_PACE_SPT_MASK)
        {
            ftms_var.ftms_treadmill_notify_data[offset] = p_treadmill_data->average_pace;
        }
        else
        {
            memset(ftms_var.ftms_treadmill_notify_data + offset, 0xFF, 1);
        }

        offset += 1;
    }

    if (ftms_var.treadmill_data_flag & FTMS_TM_EXPENDED_ENERGY_PRESENT_MASK)
    {
        if (ftms_var.ftms_feature.features_field & FTMS_FEAT_EXPENDED_ENERGY_SPT_MASK)
        {
            p = ftms_var.ftms_treadmill_notify_data + offset;
            LE_UINT16_TO_STREAM(p, p_treadmill_data->total_energy);
            offset += 2;

            p = ftms_var.ftms_treadmill_notify_data + offset;
            LE_UINT16_TO_STREAM(p, p_treadmill_data->energy_per_hour);
            offset += 2;

            ftms_var.ftms_treadmill_notify_data[offset] = p_treadmill_data->energy_per_minute;
            offset += 1;
        }
        else
        {
            memset(ftms_var.ftms_treadmill_notify_data + offset, 0xFF, 5);
            offset += 5;
        }
    }

    if (ftms_var.treadmill_data_flag & FTMS_TM_HEART_RATE_PRESENT_MASK)
    {
        if (ftms_var.ftms_feature.features_field & FTMS_FEAT_HR_MEASUREMENT_SPT_MASK)
        {
            ftms_var.ftms_treadmill_notify_data[offset] = p_treadmill_data->heart_rate;
        }
        else
        {
            memset(ftms_var.ftms_treadmill_notify_data + offset, 0xFF, 1);
        }

        offset += 1;
    }

    if (ftms_var.treadmill_data_flag & FTMS_TM_METABOLIC_EQUIVALENT_PRESENT_MASK)
    {
        if (ftms_var.ftms_feature.features_field & FTMS_FEAT_METABOLIC_EQUIVALENT_SPT_MASK)
        {
            ftms_var.ftms_treadmill_notify_data[offset] = p_treadmill_data->metabolic_equivalent;
        }
        else
        {
            memset(ftms_var.ftms_treadmill_notify_data + offset, 0xFF, 1);
        }

        offset += 1;
    }

    if (ftms_var.treadmill_data_flag & FTMS_TM_ELAPSED_TIME_PRESENT_MASK)
    {
        if (ftms_var.ftms_feature.features_field & FTMS_FEAT_ELAPSED_TIME_SPT_MASK)
        {
            p = ftms_var.ftms_treadmill_notify_data + offset;
            LE_UINT16_TO_STREAM(p, p_treadmill_data->elapsed_time);
        }
        else
        {
            memset(ftms_var.ftms_treadmill_notify_data + offset, 0xFF, 2);
        }

        offset += 2;
    }

    if (ftms_var.treadmill_data_flag & FTMS_TM_REMAIN_TIME_PRESENT_MASK)
    {
        if (ftms_var.ftms_feature.features_field & FTMS_FEAT_REMAIN_TIME_SPT_MASK)
        {
            p = ftms_var.ftms_treadmill_notify_data + offset;
            LE_UINT16_TO_STREAM(p, p_treadmill_data->remain_time);
        }
        else
        {
            memset(ftms_var.ftms_treadmill_notify_data + offset, 0xFF, 2);
        }

        offset += 2;
    }

    if (ftms_var.treadmill_data_flag & FTMS_TM_BELT_FORCE_POWER_OUTPUT_PRESENT_MASK)
    {
        if (ftms_var.ftms_feature.features_field & FTMS_FEAT_BELT_FORCE_POWER_OUTPUT_SPT_MASK)
        {
            p = ftms_var.ftms_treadmill_notify_data + offset;
            LE_UINT16_TO_STREAM(p, p_treadmill_data->force_on_belt);
            offset += 2;

            p = ftms_var.ftms_treadmill_notify_data + offset;
            LE_UINT16_TO_STREAM(p, p_treadmill_data->power_output);
        }
        else
        {
            uint16_t invalid_data = 0x7FFF;
            p = ftms_var.ftms_treadmill_notify_data + offset;
            LE_UINT16_TO_STREAM(p, invalid_data);
            offset += 2;

            p = ftms_var.ftms_treadmill_notify_data + offset;
            LE_UINT16_TO_STREAM(p, invalid_data);
        }
    }
}

void ftms_get_treadmill_data(uint8_t conn_id, uint16_t data_length)
{
    uint16_t mtu_size;
    le_get_conn_param(GAP_PARAM_CONN_MTU_SIZE, &mtu_size, conn_id);

    uint16_t len = 0;
    uint16_t offset = 0;

    if (data_length <= (mtu_size - 5))
    {
        len = data_length + FTMS_TM_DATA_FLAG_LEN;
        ftms_var.ftms_treadmill_send_data = os_mem_zalloc(RAM_TYPE_DATA_ON, len);

        if (ftms_var.ftms_treadmill_send_data == NULL)
        {
            PROFILE_PRINT_ERROR0("ftms_get_treadmill_data: get data fail");
            return;
        }

        ftms_var.treadmill_data_flag &= FTMS_TM_NO_MORE_DATA_PRESENT;
        ftms_var.ftms_treadmill_notify_flag.send_flag = ftms_var.treadmill_data_flag;
        offset += 2;
        memcpy(ftms_var.ftms_treadmill_send_data + offset, ftms_var.ftms_treadmill_notify_data,
               data_length);
        ftms_var.ftms_treadmill_notify_flag.data_len = len;

        if (ftms_var.ftms_treadmill_notify_data != NULL)
        {
            os_mem_free(ftms_var.ftms_treadmill_notify_data);
            ftms_var.ftms_treadmill_notify_data = NULL;
        }
    }
    else
    {
        uint16_t total_offset = 0;
        len = mtu_size - 3;

        ftms_var.ftms_treadmill_send_data = os_mem_zalloc(RAM_TYPE_DATA_ON, len);

        if (ftms_var.ftms_treadmill_send_data == NULL)
        {
            PROFILE_PRINT_ERROR0("ftms_get_treadmill_data: get data fail");
            return;
        }

        total_offset = ftms_var.ftms_treadmill_notify_flag.offset;

        if (ftms_var.ftms_treadmill_notify_flag.no_more_data == 0)
        {
            ftms_var.ftms_treadmill_notify_flag.send_flag |= FTMS_TM_MORE_DATA_MASK;
            offset += FTMS_TM_DATA_FLAG_LEN;
        }
        else
        {
            ftms_var.ftms_treadmill_notify_flag.send_flag &= FTMS_TM_NO_MORE_DATA_PRESENT;
            offset += FTMS_TM_DATA_FLAG_LEN;
            memcpy(ftms_var.ftms_treadmill_send_data + offset, ftms_var.ftms_treadmill_notify_data, 2);
            offset += 2;
        }

        if (ftms_var.ftms_treadmill_notify_flag.cur_flag & FTMS_TM_AVE_SPEED_PRESENT_MASK)
        {
            if ((len - offset) >= sizeof(uint16_t))
            {
                memcpy(ftms_var.ftms_treadmill_send_data + offset,
                       ftms_var.ftms_treadmill_notify_data + total_offset, 2);
                ftms_var.ftms_treadmill_notify_flag.send_flag |= FTMS_TM_AVE_SPEED_PRESENT_MASK;
                offset += 2;
                total_offset += 2;
                ftms_var.ftms_treadmill_notify_flag.cur_flag &= (~FTMS_TM_AVE_SPEED_PRESENT_MASK);
            }
            else
            {
                ftms_var.ftms_treadmill_notify_flag.data_len = offset;
                ftms_var.ftms_treadmill_notify_flag.offset = total_offset;

                if ((data_length - total_offset + FTMS_TM_INSTANTANEOUS_SPEED_LEN) <=
                    (len - FTMS_TM_DATA_FLAG_LEN))
                {
                    ftms_var.ftms_treadmill_notify_flag.no_more_data = 1;
                }
                else
                {
                    ftms_var.ftms_treadmill_notify_flag.no_more_data = 0;
                }

                return;
            }
        }

        if (ftms_var.ftms_treadmill_notify_flag.cur_flag & FTMS_TM_TOTAL_DISTANCE_PRESENT_MASK)
        {
            if ((len - offset) >= FTMS_TM_TOTAL_DISTANCE_LEN)
            {
                memcpy(ftms_var.ftms_treadmill_send_data + offset,
                       ftms_var.ftms_treadmill_notify_data + total_offset, 3);
                ftms_var.ftms_treadmill_notify_flag.send_flag |= FTMS_TM_TOTAL_DISTANCE_PRESENT_MASK;
                offset += 3;
                total_offset += 3;
                ftms_var.ftms_treadmill_notify_flag.cur_flag &= (~FTMS_TM_TOTAL_DISTANCE_PRESENT_MASK);
            }
            else
            {
                ftms_var.ftms_treadmill_notify_flag.data_len = offset;
                ftms_var.ftms_treadmill_notify_flag.offset = total_offset;

                if ((data_length - total_offset + FTMS_TM_INSTANTANEOUS_SPEED_LEN) <=
                    (len - FTMS_TM_DATA_FLAG_LEN))
                {
                    ftms_var.ftms_treadmill_notify_flag.no_more_data = 1;
                }
                else
                {
                    ftms_var.ftms_treadmill_notify_flag.no_more_data = 0;
                }

                return;
            }
        }

        if (ftms_var.ftms_treadmill_notify_flag.cur_flag & FTMS_TM_INCLINATION_RAMP_ANGLE_SET_PRESENT_MASK)
        {
            if ((len - offset) >= FTMS_TM_INCLINATION_RAMP_ANGLE_LEN)
            {
                memcpy(ftms_var.ftms_treadmill_send_data + offset,
                       ftms_var.ftms_treadmill_notify_data + total_offset, 4);
                offset += 4;
                total_offset += 4;
                ftms_var.ftms_treadmill_notify_flag.send_flag |= FTMS_TM_INCLINATION_RAMP_ANGLE_SET_PRESENT_MASK;
                ftms_var.ftms_treadmill_notify_flag.cur_flag &= (~FTMS_TM_INCLINATION_RAMP_ANGLE_SET_PRESENT_MASK);
            }
            else
            {
                ftms_var.ftms_treadmill_notify_flag.data_len = offset;
                ftms_var.ftms_treadmill_notify_flag.offset = total_offset;

                if ((data_length - total_offset + FTMS_TM_INSTANTANEOUS_SPEED_LEN) <=
                    (len - FTMS_TM_DATA_FLAG_LEN))
                {
                    ftms_var.ftms_treadmill_notify_flag.no_more_data = 1;
                }
                else
                {
                    ftms_var.ftms_treadmill_notify_flag.no_more_data = 0;
                }

                return;
            }
        }

        if (ftms_var.ftms_treadmill_notify_flag.cur_flag & FTMS_TM_ELEVATION_GAIN_PRESENT_MASK)
        {
            if ((len - offset) >= FTMS_TM_ELEVATION_GAIN_LEN)
            {
                memcpy(ftms_var.ftms_treadmill_send_data + offset,
                       ftms_var.ftms_treadmill_notify_data + total_offset, 4);
                offset += 4;
                total_offset += 4;
                ftms_var.ftms_treadmill_notify_flag.send_flag |= FTMS_TM_ELEVATION_GAIN_PRESENT_MASK;
                ftms_var.ftms_treadmill_notify_flag.cur_flag &= (~FTMS_TM_ELEVATION_GAIN_PRESENT_MASK);
            }
            else
            {
                ftms_var.ftms_treadmill_notify_flag.data_len = offset;
                ftms_var.ftms_treadmill_notify_flag.offset = total_offset;

                if ((data_length - total_offset + FTMS_TM_INSTANTANEOUS_SPEED_LEN) <=
                    (len - FTMS_TM_DATA_FLAG_LEN))
                {
                    ftms_var.ftms_treadmill_notify_flag.no_more_data = 1;
                }
                else
                {
                    ftms_var.ftms_treadmill_notify_flag.no_more_data = 0;
                }

                return;
            }
        }

        if (ftms_var.ftms_treadmill_notify_flag.cur_flag & FTMS_TM_INSTANTANEOUS_PACE_PRESENT_MASK)
        {
            if ((len - offset) >= sizeof(uint8_t))
            {
                memcpy(ftms_var.ftms_treadmill_send_data + offset,
                       ftms_var.ftms_treadmill_notify_data + total_offset, 1);
                ftms_var.ftms_treadmill_notify_flag.send_flag |= FTMS_TM_INSTANTANEOUS_PACE_PRESENT_MASK;
                offset += 1;
                total_offset += 1;
                ftms_var.ftms_treadmill_notify_flag.cur_flag &= (~FTMS_TM_INSTANTANEOUS_PACE_PRESENT_MASK);
            }
            else
            {
                ftms_var.ftms_treadmill_notify_flag.data_len = offset;
                ftms_var.ftms_treadmill_notify_flag.offset = total_offset;

                if ((data_length - total_offset + FTMS_TM_INSTANTANEOUS_SPEED_LEN) <=
                    (len - FTMS_TM_DATA_FLAG_LEN))
                {
                    ftms_var.ftms_treadmill_notify_flag.no_more_data = 1;
                }
                else
                {
                    ftms_var.ftms_treadmill_notify_flag.no_more_data = 0;
                }

                return;
            }
        }

        if (ftms_var.ftms_treadmill_notify_flag.cur_flag & FTMS_TM_AVERAGE_PACE_PRESENT_MASK)
        {
            if ((len - offset) >= sizeof(uint8_t))
            {
                memcpy(ftms_var.ftms_treadmill_send_data + offset,
                       ftms_var.ftms_treadmill_notify_data + total_offset, 1);
                ftms_var.ftms_treadmill_notify_flag.send_flag |= FTMS_TM_AVERAGE_PACE_PRESENT_MASK;
                offset += 1;
                total_offset += 1;
                ftms_var.ftms_treadmill_notify_flag.cur_flag &= (~FTMS_TM_AVERAGE_PACE_PRESENT_MASK);
            }
            else
            {
                ftms_var.ftms_treadmill_notify_flag.data_len = offset;
                ftms_var.ftms_treadmill_notify_flag.offset = total_offset;

                if ((data_length - total_offset + FTMS_TM_INSTANTANEOUS_SPEED_LEN) <=
                    (len - FTMS_TM_DATA_FLAG_LEN))
                {
                    ftms_var.ftms_treadmill_notify_flag.no_more_data = 1;
                }
                else
                {
                    ftms_var.ftms_treadmill_notify_flag.no_more_data = 0;
                }

                return;
            }
        }

        if (ftms_var.ftms_treadmill_notify_flag.cur_flag & FTMS_TM_EXPENDED_ENERGY_PRESENT_MASK)
        {
            if ((len - offset) >= FTMS_TM_EXPENDED_ENERGY_LEN)
            {
                memcpy(ftms_var.ftms_treadmill_send_data + offset,
                       ftms_var.ftms_treadmill_notify_data + total_offset, 5);
                offset += 5;
                total_offset += 5;
                ftms_var.ftms_treadmill_notify_flag.send_flag |= FTMS_TM_EXPENDED_ENERGY_PRESENT_MASK;
                ftms_var.ftms_treadmill_notify_flag.cur_flag &= (~FTMS_TM_EXPENDED_ENERGY_PRESENT_MASK);
            }
            else
            {
                ftms_var.ftms_treadmill_notify_flag.data_len = offset;
                ftms_var.ftms_treadmill_notify_flag.offset = total_offset;

                if ((data_length - total_offset + FTMS_TM_INSTANTANEOUS_SPEED_LEN) <=
                    (len - FTMS_TM_DATA_FLAG_LEN))
                {
                    ftms_var.ftms_treadmill_notify_flag.no_more_data = 1;
                }
                else
                {
                    ftms_var.ftms_treadmill_notify_flag.no_more_data = 0;
                }

                return;
            }
        }

        if (ftms_var.ftms_treadmill_notify_flag.cur_flag & FTMS_TM_HEART_RATE_PRESENT_MASK)
        {
            if ((len - offset) >= sizeof(uint8_t))
            {
                memcpy(ftms_var.ftms_treadmill_send_data + offset,
                       ftms_var.ftms_treadmill_notify_data + total_offset, 1);
                ftms_var.ftms_treadmill_notify_flag.send_flag |= FTMS_TM_HEART_RATE_PRESENT_MASK;
                offset += 1;
                total_offset += 1;
                ftms_var.ftms_treadmill_notify_flag.cur_flag &= (~FTMS_TM_HEART_RATE_PRESENT_MASK);
            }
            else
            {
                ftms_var.ftms_treadmill_notify_flag.data_len = offset;
                ftms_var.ftms_treadmill_notify_flag.offset = total_offset;

                if ((data_length - total_offset + FTMS_TM_INSTANTANEOUS_SPEED_LEN) <=
                    (len - FTMS_TM_DATA_FLAG_LEN))
                {
                    ftms_var.ftms_treadmill_notify_flag.no_more_data = 1;
                }
                else
                {
                    ftms_var.ftms_treadmill_notify_flag.no_more_data = 0;
                }

                return;
            }
        }

        if (ftms_var.ftms_treadmill_notify_flag.cur_flag & FTMS_TM_METABOLIC_EQUIVALENT_PRESENT_MASK)
        {
            if ((len - offset) >= sizeof(uint8_t))
            {
                memcpy(ftms_var.ftms_treadmill_send_data + offset,
                       ftms_var.ftms_treadmill_notify_data + total_offset, 1);
                ftms_var.ftms_treadmill_notify_flag.send_flag |= FTMS_TM_METABOLIC_EQUIVALENT_PRESENT_MASK;
                offset += 1;
                total_offset += 1;
                ftms_var.ftms_treadmill_notify_flag.cur_flag &= (~FTMS_TM_METABOLIC_EQUIVALENT_PRESENT_MASK);
            }
            else
            {
                ftms_var.ftms_treadmill_notify_flag.data_len = offset;
                ftms_var.ftms_treadmill_notify_flag.offset = total_offset;

                if ((data_length - total_offset + FTMS_TM_INSTANTANEOUS_SPEED_LEN) <=
                    (len - FTMS_TM_DATA_FLAG_LEN))
                {
                    ftms_var.ftms_treadmill_notify_flag.no_more_data = 1;
                }
                else
                {
                    ftms_var.ftms_treadmill_notify_flag.no_more_data = 0;
                }

                return;
            }
        }

        if (ftms_var.ftms_treadmill_notify_flag.cur_flag & FTMS_TM_ELAPSED_TIME_PRESENT_MASK)
        {
            if ((len - offset) >= sizeof(uint16_t))
            {
                memcpy(ftms_var.ftms_treadmill_send_data + offset,
                       ftms_var.ftms_treadmill_notify_data + total_offset, 2);
                ftms_var.ftms_treadmill_notify_flag.send_flag |= FTMS_TM_ELAPSED_TIME_PRESENT_MASK;
                offset += 2;
                total_offset += 2;
                ftms_var.ftms_treadmill_notify_flag.cur_flag &= (~FTMS_TM_ELAPSED_TIME_PRESENT_MASK);
            }
            else
            {
                ftms_var.ftms_treadmill_notify_flag.data_len = offset;
                ftms_var.ftms_treadmill_notify_flag.offset = total_offset;

                if ((data_length - total_offset + FTMS_TM_INSTANTANEOUS_SPEED_LEN) <=
                    (len - FTMS_TM_DATA_FLAG_LEN))
                {
                    ftms_var.ftms_treadmill_notify_flag.no_more_data = 1;
                }
                else
                {
                    ftms_var.ftms_treadmill_notify_flag.no_more_data = 0;
                }

                return;
            }
        }

        if (ftms_var.ftms_treadmill_notify_flag.cur_flag & FTMS_TM_REMAIN_TIME_PRESENT_MASK)
        {
            if ((len - offset) >= sizeof(uint16_t))
            {
                memcpy(ftms_var.ftms_treadmill_send_data + offset,
                       ftms_var.ftms_treadmill_notify_data + total_offset, 2);
                ftms_var.ftms_treadmill_notify_flag.send_flag |= FTMS_TM_REMAIN_TIME_PRESENT_MASK;
                offset += 2;
                total_offset += 2;
                ftms_var.ftms_treadmill_notify_flag.cur_flag &= (~FTMS_TM_REMAIN_TIME_PRESENT_MASK);
            }
            else
            {
                ftms_var.ftms_treadmill_notify_flag.data_len = offset;
                ftms_var.ftms_treadmill_notify_flag.offset = total_offset;

                if ((data_length - total_offset + FTMS_TM_INSTANTANEOUS_SPEED_LEN) <=
                    (len - FTMS_TM_DATA_FLAG_LEN))
                {
                    ftms_var.ftms_treadmill_notify_flag.no_more_data = 1;
                }
                else
                {
                    ftms_var.ftms_treadmill_notify_flag.no_more_data = 0;
                }

                return;
            }
        }

        if (ftms_var.ftms_treadmill_notify_flag.cur_flag & FTMS_TM_BELT_FORCE_POWER_OUTPUT_PRESENT_MASK)
        {
            if ((len - offset) >= FTMS_TM_BELT_FORCE_POWER_OUTPUT_LEN)
            {
                memcpy(ftms_var.ftms_treadmill_send_data + offset,
                       ftms_var.ftms_treadmill_notify_data + total_offset, 4);
                offset += 4;
                total_offset += 4;
                ftms_var.ftms_treadmill_notify_flag.send_flag |= FTMS_TM_BELT_FORCE_POWER_OUTPUT_PRESENT_MASK;
                ftms_var.ftms_treadmill_notify_flag.cur_flag &= (~FTMS_TM_BELT_FORCE_POWER_OUTPUT_PRESENT_MASK);
            }
            else
            {
                ftms_var.ftms_treadmill_notify_flag.data_len = offset;
                ftms_var.ftms_treadmill_notify_flag.offset = total_offset;

                if ((data_length - total_offset + FTMS_TM_INSTANTANEOUS_SPEED_LEN) <=
                    (len - FTMS_TM_DATA_FLAG_LEN))
                {
                    ftms_var.ftms_treadmill_notify_flag.no_more_data = 1;
                }
                else
                {
                    ftms_var.ftms_treadmill_notify_flag.no_more_data = 0;
                }

                return;
            }
        }

        ftms_var.ftms_treadmill_notify_flag.data_len = offset;
        ftms_var.ftms_treadmill_notify_flag.offset = 0;
        ftms_var.ftms_treadmill_notify_flag.no_more_data = 0;

        if (ftms_var.ftms_treadmill_notify_data != NULL)
        {
            os_mem_free(ftms_var.ftms_treadmill_notify_data);
            ftms_var.ftms_treadmill_notify_data = NULL;
        }
    }
}

bool ftms_send_treadmill_data(uint8_t conn_id, T_SERVER_ID service_id, uint16_t data_length)
{
    uint16_t mtu_size;
    le_get_conn_param(GAP_PARAM_CONN_MTU_SIZE, &mtu_size, conn_id);

    bool ret = false;
    T_SRV_UUID_TBL *p_char;
    p_char = srv_find_index_by_uuid(ftms_var.ftms_srv_char_tbl, GATT_UUID_TREADMILL_DATA);
    uint16_t attrib_index = p_char->char_index;

    if (data_length <= (mtu_size - 5))
    {
        ftms_get_treadmill_data(conn_id, data_length);

        if (ftms_var.ftms_treadmill_send_data == NULL)
        {
            return ret;
        }

        uint8_t *p = ftms_var.ftms_treadmill_send_data;
        LE_UINT16_TO_STREAM(p, ftms_var.ftms_treadmill_notify_flag.send_flag);

        if (server_send_data(conn_id, service_id, attrib_index, ftms_var.ftms_treadmill_send_data,
                             ftms_var.ftms_treadmill_notify_flag.data_len, GATT_PDU_TYPE_NOTIFICATION))
        {
            ret = true;
        }

        if (ftms_var.ftms_treadmill_send_data != NULL)
        {
            os_mem_free(ftms_var.ftms_treadmill_send_data);
            ftms_var.ftms_treadmill_send_data = NULL;
        }

        ftms_var.ftms_treadmill_notify_flag.send_flag = 0;
        ftms_var.ftms_treadmill_notify_flag.data_len = 0;
    }
    else
    {
        ftms_var.ftms_treadmill_notify_flag.cur_flag =
            ftms_var.treadmill_data_flag & FTMS_TM_NO_MORE_DATA_PRESENT;

        while (ftms_var.ftms_treadmill_notify_flag.cur_flag)
        {
            ftms_get_treadmill_data(conn_id, data_length);

            if (ftms_var.ftms_treadmill_send_data == NULL)
            {
                ret = false;
                return ret;
            }

            uint8_t *p = ftms_var.ftms_treadmill_send_data;
            LE_UINT16_TO_STREAM(p, ftms_var.ftms_treadmill_notify_flag.send_flag);

            if (server_send_data(conn_id, service_id, attrib_index, ftms_var.ftms_treadmill_send_data,
                                 ftms_var.ftms_treadmill_notify_flag.data_len, GATT_PDU_TYPE_NOTIFICATION))
            {
                if (ftms_var.ftms_treadmill_send_data != NULL)
                {
                    os_mem_free(ftms_var.ftms_treadmill_send_data);
                    ftms_var.ftms_treadmill_send_data = NULL;
                }

                ftms_var.ftms_treadmill_notify_flag.send_flag = 0;
                ftms_var.ftms_treadmill_notify_flag.data_len = 0;
                ret = true;
            }
            else
            {
                if (ftms_var.ftms_treadmill_send_data != NULL)
                {
                    os_mem_free(ftms_var.ftms_treadmill_send_data);
                    ftms_var.ftms_treadmill_send_data = NULL;
                }

                ftms_var.ftms_treadmill_notify_flag.send_flag = 0;
                ftms_var.ftms_treadmill_notify_flag.data_len = 0;
                ret = false;
                return ret;
            }
        }
    }

    return ret;
}

bool ftms_treadmill_data_notify(uint8_t conn_id, T_SERVER_ID service_id,
                                T_FTMS_TREADMILL_DATA *p_treadmill_data)
{
    bool ret = true;

    if (ftms_var.ftms_notify_indicate_flag.ftms_treadmill_data_notify_enable == 0)
    {
        ret = false;
        PROFILE_PRINT_INFO0("ftms_treadmill_data_notify:FTMS_APP_RESULT_CCCD_NOT_ENABLED");
        return ret;
    }

    uint16_t data_length = ftms_treadmill_data_length();
    ftms_save_treadmill_notify_data(conn_id, data_length, p_treadmill_data);

    return ftms_send_treadmill_data(conn_id, service_id, data_length);
}
#endif

#if FTMS_CHAR_STEP_CLIMBER_DATA_SUPPORT
uint16_t ftms_step_climber_data_length(void)
{
    uint16_t data_length = FTMS_STEPC_FLOOR_STEP_COUNT_LEN;
    if (ftms_var.step_climber_data_flag & FTMS_STEPC_STEP_PER_MINUTE_PRESENT_MASK)
    {
        data_length += sizeof(uint16_t);
    }
    if (ftms_var.step_climber_data_flag & FTMS_STEPC_AVE_STEP_RATE_PRESENT_MASK)
    {
        data_length += sizeof(uint16_t);
    }
    if (ftms_var.step_climber_data_flag & FTMS_STEPC_POSITIVE_ELEVATION_GAIN_PRESENT_MASK)
    {
        data_length += sizeof(uint16_t);
    }
    if (ftms_var.step_climber_data_flag & FTMS_STEPC_EXPEND_ENERGY_PRESENT_MASK)
    {
        data_length += FTMS_STEPC_EXPENDED_ENERGY_LEN;
    }
    if (ftms_var.step_climber_data_flag & FTMS_STEPC_HEART_RATE_PRESENT_MASK)
    {
        data_length += sizeof(uint8_t);
    }
    if (ftms_var.step_climber_data_flag & FTMS_STEPC_METABOLIC_EQUIVALENT_PRESENT_MASK)
    {
        data_length += sizeof(uint8_t);
    }
    if (ftms_var.step_climber_data_flag & FTMS_STEPC_ELAPSED_TIME_PRESENT_MASK)
    {
        data_length += sizeof(uint16_t);
    }
    if (ftms_var.step_climber_data_flag & FTMS_STEPC_REMAIN_TIME_PRESENT_MASK)
    {
        data_length += sizeof(uint16_t);
    }

    return data_length;
}

void ftms_save_step_climber_notify_data(uint8_t conn_id, uint16_t data_length,
                                        T_FTMS_STEP_CLIMBER_DATA *p_step_climber_data)
{
    ftms_var.ftms_step_climber_notify_data = os_mem_zalloc(RAM_TYPE_DATA_ON, data_length);
    uint16_t offset = 0;
    uint8_t *p;
    p = ftms_var.ftms_step_climber_notify_data + offset;
    LE_UINT16_TO_STREAM(p, p_step_climber_data->floor);
    offset += 2;

    p = ftms_var.ftms_step_climber_notify_data + offset;
    LE_UINT16_TO_STREAM(p, p_step_climber_data->step_count);
    offset += 2;

    ftms_var.ftms_step_climber_notify_flag.offset = FTMS_STEPC_FLOOR_STEP_COUNT_LEN;


    if (ftms_var.step_climber_data_flag & FTMS_STEPC_STEP_PER_MINUTE_PRESENT_MASK)
    {
        if (ftms_var.ftms_feature.features_field & FTMS_FEAT_STEP_COUNT_SPT_MASK)
        {
            p = ftms_var.ftms_step_climber_notify_data + offset;
            LE_UINT16_TO_STREAM(p, p_step_climber_data->step_count);
        }
        else
        {
            memset(ftms_var.ftms_step_climber_notify_data + offset, 0xFF, 2);
        }

        offset += 2;
    }

    if (ftms_var.step_climber_data_flag & FTMS_STEPC_AVE_STEP_RATE_PRESENT_MASK)
    {
        if (ftms_var.ftms_feature.features_field & FTMS_FEAT_STEP_COUNT_SPT_MASK)
        {
            p = ftms_var.ftms_step_climber_notify_data + offset;
            LE_UINT16_TO_STREAM(p, p_step_climber_data->average_step_rate);
        }
        else
        {
            memset(ftms_var.ftms_step_climber_notify_data + offset, 0xFF, 2);
        }

        offset += 2;
    }

    if (ftms_var.step_climber_data_flag & FTMS_STEPC_POSITIVE_ELEVATION_GAIN_PRESENT_MASK)
    {
        if (ftms_var.ftms_feature.features_field & FTMS_FEAT_ELEVATION_GAIN_SPT_MASK)
        {
            p = ftms_var.ftms_step_climber_notify_data + offset;
            LE_UINT16_TO_STREAM(p, p_step_climber_data->positive_elevation_gain);
        }
        else
        {
            memset(ftms_var.ftms_step_climber_notify_data + offset, 0xFF, 2);
        }

        offset += 2;
    }

    if (ftms_var.step_climber_data_flag & FTMS_STEPC_EXPEND_ENERGY_PRESENT_MASK)
    {
        if (ftms_var.ftms_feature.features_field & FTMS_FEAT_EXPENDED_ENERGY_SPT_MASK)
        {
            p = ftms_var.ftms_step_climber_notify_data + offset;
            LE_UINT16_TO_STREAM(p, p_step_climber_data->positive_elevation_gain);
            offset += 2;

            p = ftms_var.ftms_step_climber_notify_data + offset;
            LE_UINT16_TO_STREAM(p, p_step_climber_data->positive_elevation_gain);
            offset += 2;

            ftms_var.ftms_step_climber_notify_data[offset] = p_step_climber_data->energy_per_minute;
            offset += 1;
        }
        else
        {
            memset(ftms_var.ftms_step_climber_notify_data + offset, 0xFF, 5);
            offset += 5;
        }
    }

    if (ftms_var.step_climber_data_flag & FTMS_STEPC_HEART_RATE_PRESENT_MASK)
    {
        if (ftms_var.ftms_feature.features_field & FTMS_FEAT_HR_MEASUREMENT_SPT_MASK)
        {
            ftms_var.ftms_step_climber_notify_data[offset] = p_step_climber_data->heart_rate;
        }
        else
        {
            memset(ftms_var.ftms_step_climber_notify_data + offset, 0xFF, 1);
        }

        offset += 1;
    }

    if (ftms_var.step_climber_data_flag & FTMS_STEPC_METABOLIC_EQUIVALENT_PRESENT_MASK)
    {
        if (ftms_var.ftms_feature.features_field & FTMS_FEAT_METABOLIC_EQUIVALENT_SPT_MASK)
        {
            ftms_var.ftms_step_climber_notify_data[offset] = p_step_climber_data->metabolic_equivalent;

        }
        else
        {
            memset(ftms_var.ftms_step_climber_notify_data + offset, 0xFF, 1);
        }

        offset += 1;
    }

    if (ftms_var.step_climber_data_flag & FTMS_STEPC_ELAPSED_TIME_PRESENT_MASK)
    {
        if (ftms_var.ftms_feature.features_field & FTMS_FEAT_ELAPSED_TIME_SPT_MASK)
        {
            p = ftms_var.ftms_step_climber_notify_data + offset;
            LE_UINT16_TO_STREAM(p, p_step_climber_data->elapsed_time);
        }
        else
        {
            memset(ftms_var.ftms_step_climber_notify_data + offset, 0xFF, 2);
        }

        offset += 2;
    }

    if (ftms_var.step_climber_data_flag & FTMS_STEPC_REMAIN_TIME_PRESENT_MASK)
    {
        if (ftms_var.ftms_feature.features_field & FTMS_FEAT_REMAIN_TIME_SPT_MASK)
        {
            p = ftms_var.ftms_step_climber_notify_data + offset;
            LE_UINT16_TO_STREAM(p, p_step_climber_data->remain_time);
        }
        else
        {
            memset(ftms_var.ftms_step_climber_notify_data + offset, 0xFF, 2);
        }
    }
}

void ftms_get_step_climber_data(uint8_t conn_id, uint16_t data_length)
{
    uint16_t mtu_size;
    le_get_conn_param(GAP_PARAM_CONN_MTU_SIZE, &mtu_size, conn_id);

    uint16_t len = 0;
    uint16_t offset = 0;

    if (data_length <= (mtu_size - 5))
    {
        len = data_length + FTMS_STEPC_DATA_FLAG_LEN;
        ftms_var.ftms_step_climber_send_data = os_mem_zalloc(RAM_TYPE_DATA_ON, len);

        if (ftms_var.ftms_step_climber_send_data == NULL)
        {
            PROFILE_PRINT_ERROR0("ftms_get_step_climber_data: get data fail");
            return;
        }

        ftms_var.step_climber_data_flag &= FTMS_STEPC_NO_MORE_DATA_PRESENT;
        ftms_var.ftms_step_climber_notify_flag.send_flag = ftms_var.step_climber_data_flag;
        offset += 2;
        memcpy(ftms_var.ftms_step_climber_send_data + offset, ftms_var.ftms_step_climber_notify_data,
               data_length);
        ftms_var.ftms_step_climber_notify_flag.data_len = len;

        if (ftms_var.ftms_step_climber_notify_data != NULL)
        {
            os_mem_free(ftms_var.ftms_step_climber_notify_data);
            ftms_var.ftms_step_climber_notify_data = NULL;
        }
    }
    else
    {
        uint16_t total_offset = 0;
        len = mtu_size - 3;

        ftms_var.ftms_step_climber_send_data = os_mem_zalloc(RAM_TYPE_DATA_ON, len);

        if (ftms_var.ftms_step_climber_send_data == NULL)
        {
            PROFILE_PRINT_ERROR0("ftms_get_step_climber_data: get data fail");
            return;
        }

        total_offset = ftms_var.ftms_step_climber_notify_flag.offset;

        if (ftms_var.ftms_step_climber_notify_flag.no_more_data == 0)
        {
            ftms_var.ftms_step_climber_notify_flag.send_flag |= FTMS_STEPC_MORE_DATA_MASK;
            offset += FTMS_STEPC_DATA_FLAG_LEN;
        }
        else
        {
            ftms_var.ftms_step_climber_notify_flag.send_flag &= FTMS_STEPC_NO_MORE_DATA_PRESENT;
            offset += FTMS_STEPC_DATA_FLAG_LEN;
            memcpy(ftms_var.ftms_step_climber_send_data + offset, ftms_var.ftms_step_climber_notify_data, 4);
            offset += FTMS_STEPC_FLOOR_STEP_COUNT_LEN;
        }

        if (ftms_var.ftms_step_climber_notify_flag.cur_flag & FTMS_STEPC_STEP_PER_MINUTE_PRESENT_MASK)
        {
            if ((len - offset) >= sizeof(uint16_t))
            {
                memcpy(ftms_var.ftms_step_climber_send_data + offset,
                       ftms_var.ftms_step_climber_notify_data + total_offset, 2);
                ftms_var.ftms_step_climber_notify_flag.send_flag |= FTMS_STEPC_STEP_PER_MINUTE_PRESENT_MASK;
                offset += 2;
                total_offset += 2;
                ftms_var.ftms_step_climber_notify_flag.cur_flag &= (~FTMS_STEPC_STEP_PER_MINUTE_PRESENT_MASK);
            }
            else
            {
                ftms_var.ftms_step_climber_notify_flag.data_len = offset;
                ftms_var.ftms_step_climber_notify_flag.offset = total_offset;

                if ((data_length - offset + FTMS_STEPC_FLOOR_STEP_COUNT_LEN) <=
                    (len - FTMS_STEPC_FLOOR_STEP_COUNT_LEN))
                {
                    ftms_var.ftms_step_climber_notify_flag.no_more_data = 1;
                }
                else
                {
                    ftms_var.ftms_step_climber_notify_flag.no_more_data = 0;
                }

                return;
            }
        }

        if (ftms_var.ftms_step_climber_notify_flag.cur_flag & FTMS_STEPC_AVE_STEP_RATE_PRESENT_MASK)
        {
            if ((len - offset) >= sizeof(uint16_t))
            {
                memcpy(ftms_var.ftms_step_climber_send_data + offset,
                       ftms_var.ftms_step_climber_notify_data + total_offset, 2);
                ftms_var.ftms_step_climber_notify_flag.send_flag |= FTMS_STEPC_AVE_STEP_RATE_PRESENT_MASK;
                offset += 2;
                total_offset += 2;
                ftms_var.ftms_step_climber_notify_flag.cur_flag &= (~FTMS_TM_TOTAL_DISTANCE_PRESENT_MASK);
            }
            else
            {
                ftms_var.ftms_step_climber_notify_flag.data_len = offset;
                ftms_var.ftms_step_climber_notify_flag.offset = total_offset;

                if ((data_length - offset + FTMS_STEPC_FLOOR_STEP_COUNT_LEN) <=
                    (len - FTMS_STEPC_FLOOR_STEP_COUNT_LEN))
                {
                    ftms_var.ftms_step_climber_notify_flag.no_more_data = 1;
                }
                else
                {
                    ftms_var.ftms_step_climber_notify_flag.no_more_data = 0;
                }

                return;
            }
        }

        if (ftms_var.ftms_step_climber_notify_flag.cur_flag &
            FTMS_STEPC_POSITIVE_ELEVATION_GAIN_PRESENT_MASK)
        {
            if ((len - offset) >= sizeof(uint16_t))
            {
                memcpy(ftms_var.ftms_step_climber_send_data + offset,
                       ftms_var.ftms_step_climber_notify_data + total_offset, 2);
                offset += 2;
                total_offset += 2;
                ftms_var.ftms_step_climber_notify_flag.send_flag |= FTMS_STEPC_POSITIVE_ELEVATION_GAIN_PRESENT_MASK;
                ftms_var.ftms_step_climber_notify_flag.cur_flag &=
                    (~FTMS_STEPC_POSITIVE_ELEVATION_GAIN_PRESENT_MASK);
            }
            else
            {
                ftms_var.ftms_step_climber_notify_flag.data_len = offset;
                ftms_var.ftms_step_climber_notify_flag.offset = total_offset;

                if ((data_length - offset + FTMS_STEPC_FLOOR_STEP_COUNT_LEN) <=
                    (len - FTMS_STEPC_FLOOR_STEP_COUNT_LEN))
                {
                    ftms_var.ftms_step_climber_notify_flag.no_more_data = 1;
                }
                else
                {
                    ftms_var.ftms_step_climber_notify_flag.no_more_data = 0;
                }

                return;
            }
        }

        if (ftms_var.ftms_step_climber_notify_flag.cur_flag & FTMS_STEPC_EXPEND_ENERGY_PRESENT_MASK)
        {
            if ((len - offset) >= FTMS_STEPC_EXPENDED_ENERGY_LEN)
            {
                memcpy(ftms_var.ftms_step_climber_send_data + offset,
                       ftms_var.ftms_step_climber_notify_data + total_offset, 5);
                offset += 5;
                total_offset += 5;
                ftms_var.ftms_step_climber_notify_flag.send_flag |= FTMS_STEPC_EXPEND_ENERGY_PRESENT_MASK;
                ftms_var.ftms_step_climber_notify_flag.cur_flag &= (~FTMS_STEPC_EXPEND_ENERGY_PRESENT_MASK);
            }
            else
            {
                ftms_var.ftms_step_climber_notify_flag.data_len = offset;
                ftms_var.ftms_step_climber_notify_flag.offset = total_offset;

                if ((data_length - offset + FTMS_STEPC_FLOOR_STEP_COUNT_LEN) <=
                    (len - FTMS_STEPC_FLOOR_STEP_COUNT_LEN))
                {
                    ftms_var.ftms_step_climber_notify_flag.no_more_data = 1;
                }
                else
                {
                    ftms_var.ftms_step_climber_notify_flag.no_more_data = 0;
                }

                return;
            }
        }

        if (ftms_var.ftms_step_climber_notify_flag.cur_flag & FTMS_STEPC_HEART_RATE_PRESENT_MASK)
        {
            if ((len - offset) >= sizeof(uint8_t))
            {
                memcpy(ftms_var.ftms_step_climber_send_data + offset,
                       ftms_var.ftms_step_climber_notify_data + total_offset, 1);
                ftms_var.ftms_step_climber_notify_flag.send_flag |= FTMS_STEPC_HEART_RATE_PRESENT_MASK;
                offset += 1;
                total_offset += 1;
                ftms_var.ftms_step_climber_notify_flag.cur_flag &= (~FTMS_STEPC_HEART_RATE_PRESENT_MASK);
            }
            else
            {
                ftms_var.ftms_step_climber_notify_flag.data_len = offset;
                ftms_var.ftms_step_climber_notify_flag.offset = total_offset;

                if ((data_length - offset + FTMS_STEPC_FLOOR_STEP_COUNT_LEN) <=
                    (len - FTMS_STEPC_FLOOR_STEP_COUNT_LEN))
                {
                    ftms_var.ftms_step_climber_notify_flag.no_more_data = 1;
                }
                else
                {
                    ftms_var.ftms_step_climber_notify_flag.no_more_data = 0;
                }

                return;
            }
        }

        if (ftms_var.ftms_step_climber_notify_flag.cur_flag & FTMS_STEPC_METABOLIC_EQUIVALENT_PRESENT_MASK)
        {
            if ((len - offset) >= sizeof(uint8_t))
            {
                memcpy(ftms_var.ftms_step_climber_send_data + offset,
                       ftms_var.ftms_step_climber_notify_data + total_offset, 1);
                ftms_var.ftms_step_climber_notify_flag.send_flag |= FTMS_STEPC_METABOLIC_EQUIVALENT_PRESENT_MASK;
                offset += 1;
                total_offset += 1;
                ftms_var.ftms_step_climber_notify_flag.cur_flag &= (~FTMS_STEPC_METABOLIC_EQUIVALENT_PRESENT_MASK);
            }
            else
            {
                ftms_var.ftms_step_climber_notify_flag.data_len = offset;
                ftms_var.ftms_step_climber_notify_flag.offset = total_offset;

                if ((data_length - offset + FTMS_STEPC_FLOOR_STEP_COUNT_LEN) <=
                    (len - FTMS_STEPC_FLOOR_STEP_COUNT_LEN))
                {
                    ftms_var.ftms_step_climber_notify_flag.no_more_data = 1;
                }
                else
                {
                    ftms_var.ftms_step_climber_notify_flag.no_more_data = 0;
                }

                return;
            }
        }

        if (ftms_var.ftms_step_climber_notify_flag.cur_flag & FTMS_STEPC_ELAPSED_TIME_PRESENT_MASK)
        {
            if ((len - offset) >= sizeof(uint16_t))
            {
                memcpy(ftms_var.ftms_step_climber_send_data + offset,
                       ftms_var.ftms_step_climber_notify_data + total_offset, 2);
                ftms_var.ftms_step_climber_notify_flag.send_flag |= FTMS_STEPC_ELAPSED_TIME_PRESENT_MASK;
                offset += 2;
                total_offset += 2;
                ftms_var.ftms_step_climber_notify_flag.cur_flag &= (~FTMS_STEPC_ELAPSED_TIME_PRESENT_MASK);
            }
            else
            {
                ftms_var.ftms_step_climber_notify_flag.data_len = offset;
                ftms_var.ftms_step_climber_notify_flag.offset = total_offset;

                if ((data_length - offset + FTMS_STEPC_FLOOR_STEP_COUNT_LEN) <=
                    (len - FTMS_STEPC_FLOOR_STEP_COUNT_LEN))
                {
                    ftms_var.ftms_step_climber_notify_flag.no_more_data = 1;
                }
                else
                {
                    ftms_var.ftms_step_climber_notify_flag.no_more_data = 0;
                }

                return;
            }
        }

        if (ftms_var.ftms_step_climber_notify_flag.cur_flag & FTMS_STEPC_REMAIN_TIME_PRESENT_MASK)
        {
            if ((len - offset) >= sizeof(uint16_t))
            {
                memcpy(ftms_var.ftms_step_climber_send_data + offset,
                       ftms_var.ftms_step_climber_notify_data + total_offset, 2);
                ftms_var.ftms_step_climber_notify_flag.send_flag |= FTMS_STEPC_REMAIN_TIME_PRESENT_MASK;
                offset += 2;
                total_offset += 2;
                ftms_var.ftms_step_climber_notify_flag.cur_flag &= (~FTMS_STEPC_REMAIN_TIME_PRESENT_MASK);
            }
            else
            {
                ftms_var.ftms_step_climber_notify_flag.data_len = offset;
                ftms_var.ftms_step_climber_notify_flag.offset = total_offset;

                if ((data_length - offset + FTMS_STEPC_FLOOR_STEP_COUNT_LEN) <=
                    (len - FTMS_STEPC_FLOOR_STEP_COUNT_LEN))
                {
                    ftms_var.ftms_step_climber_notify_flag.no_more_data = 1;
                }
                else
                {
                    ftms_var.ftms_step_climber_notify_flag.no_more_data = 0;
                }

                return;
            }
        }

        ftms_var.ftms_step_climber_notify_flag.data_len = offset;
        ftms_var.ftms_step_climber_notify_flag.offset = 0;
        ftms_var.ftms_step_climber_notify_flag.no_more_data = 0;

        if (ftms_var.ftms_step_climber_notify_data != NULL)
        {
            os_mem_free(ftms_var.ftms_step_climber_notify_data);
            ftms_var.ftms_step_climber_notify_data = NULL;
        }
    }
}

bool ftms_send_step_climber_data(uint8_t conn_id, T_SERVER_ID service_id, uint16_t data_length)
{
    uint16_t mtu_size;
    le_get_conn_param(GAP_PARAM_CONN_MTU_SIZE, &mtu_size, conn_id);

    bool ret = false;
    T_SRV_UUID_TBL *p_char;
    p_char = srv_find_index_by_uuid(ftms_var.ftms_srv_char_tbl, GATT_UUID_STEP_CLIMBER_DATA);
    uint16_t attrib_index = p_char->char_index;

    if (data_length <= (mtu_size - 5))
    {
        ftms_get_step_climber_data(conn_id, data_length);

        if (ftms_var.ftms_step_climber_send_data == NULL)
        {
            return ret;
        }

        uint8_t *p = ftms_var.ftms_step_climber_send_data;
        LE_UINT16_TO_STREAM(p, ftms_var.ftms_step_climber_notify_flag.send_flag);

        if (server_send_data(conn_id, service_id, attrib_index, ftms_var.ftms_step_climber_send_data,
                             ftms_var.ftms_step_climber_notify_flag.data_len, GATT_PDU_TYPE_NOTIFICATION))
        {
            ret = true;
        }

        if (ftms_var.ftms_step_climber_send_data != NULL)
        {
            os_mem_free(ftms_var.ftms_step_climber_send_data);
            ftms_var.ftms_step_climber_send_data = NULL;
        }

        ftms_var.ftms_step_climber_notify_flag.send_flag = 0;
        ftms_var.ftms_step_climber_notify_flag.data_len = 0;
    }
    else
    {
        ftms_var.ftms_step_climber_notify_flag.cur_flag =
            ftms_var.step_climber_data_flag & FTMS_STEPC_NO_MORE_DATA_PRESENT;

        while (ftms_var.ftms_step_climber_notify_flag.cur_flag)
        {
            ftms_get_step_climber_data(conn_id, data_length);

            if (ftms_var.ftms_step_climber_send_data == NULL)
            {
                ret = false;
                return ret;
            }

            uint8_t *p = ftms_var.ftms_step_climber_send_data;
            LE_UINT16_TO_STREAM(p, ftms_var.ftms_step_climber_notify_flag.send_flag);

            if (server_send_data(conn_id, service_id, attrib_index, ftms_var.ftms_step_climber_send_data,
                                 ftms_var.ftms_step_climber_notify_flag.data_len, GATT_PDU_TYPE_NOTIFICATION))
            {
                if (ftms_var.ftms_step_climber_send_data != NULL)
                {
                    os_mem_free(ftms_var.ftms_step_climber_send_data);
                    ftms_var.ftms_step_climber_send_data = NULL;
                }

                ftms_var.ftms_step_climber_notify_flag.send_flag = 0;
                ftms_var.ftms_step_climber_notify_flag.data_len = 0;
                ret = true;
            }
            else
            {
                if (ftms_var.ftms_step_climber_send_data != NULL)
                {
                    os_mem_free(ftms_var.ftms_step_climber_send_data);
                    ftms_var.ftms_step_climber_send_data = NULL;
                }

                ftms_var.ftms_step_climber_notify_flag.send_flag = 0;
                ftms_var.ftms_step_climber_notify_flag.data_len = 0;
                ret = false;
                return ret;
            }
        }
    }

    return ret;
}

bool ftms_step_climber_data_notify(uint8_t conn_id, T_SERVER_ID service_id,
                                   T_FTMS_STEP_CLIMBER_DATA *step_climber_data)
{
    bool ret = true;
    if (ftms_var.ftms_notify_indicate_flag.ftms_step_climber_data_notify_enable == 0)
    {
        ret = false;
        PROFILE_PRINT_INFO0("ftms_step_climber_data_notify:FTMS_APP_RESULT_CCCD_NOT_ENABLED");
        return ret;
    }

    uint16_t data_length = ftms_step_climber_data_length();
    ftms_save_step_climber_notify_data(conn_id, data_length, step_climber_data);

    return ftms_send_step_climber_data(conn_id, service_id, data_length);
}
#endif

#if FTMS_CHAR_ROWER_DATA_SUPPORT
uint16_t ftms_rower_data_length(void)
{
    uint16_t data_length = FTMS_RW_STROKE_RATE_COUNT_LEN;
    if (ftms_var.rower_data_flag & FTMS_RW_AVE_STROKE_RATE_PRESENT_MASK)
    {
        data_length += sizeof(uint8_t);
    }
    if (ftms_var.rower_data_flag & FTMS_RW_TOTAL_DISTANCE_PRESENT_MASK)
    {
        data_length += FTMS_RW_TOTAL_DISTANCE_LEN;
    }
    if (ftms_var.rower_data_flag & FTMS_RW_INSTANTANEOUS_PACE_PRESENT_MASK)
    {
        data_length += sizeof(uint16_t);
    }
    if (ftms_var.rower_data_flag & FTMS_RW_AVE_PACE_PRESENT_MASK)
    {
        data_length += sizeof(uint16_t);
    }
    if (ftms_var.rower_data_flag & FTMS_RW_INSTANTANEOUS_POWER_PRESENT_MASK)
    {
        data_length += sizeof(int16_t);
    }
    if (ftms_var.rower_data_flag & FTMS_RW_AVE_POWER_PRESENT_MASK)
    {
        data_length += sizeof(int16_t);
    }
    if (ftms_var.rower_data_flag & FTMS_RW_RESISTANCE_LEVEL_MASK)
    {
        data_length += sizeof(int16_t);
    }
    if (ftms_var.rower_data_flag & FTMS_RW_EXPEND_ENERGY_PRESENT_MASK)
    {
        data_length += FTMS_RW_EXPENDED_ENERGY_LEN;
    }
    if (ftms_var.rower_data_flag & FTMS_RW_HEART_RATE_PRESENT_MASK)
    {
        data_length += sizeof(uint8_t);
    }
    if (ftms_var.rower_data_flag & FTMS_RW_METABOLIC_EQUIVALENT_PRESENT_MASK)
    {
        data_length += sizeof(uint8_t);
    }
    if (ftms_var.rower_data_flag & FTMS_RW_ELAPSED_TIME_PRESENT_MASK)
    {
        data_length += sizeof(uint16_t);
    }
    if (ftms_var.rower_data_flag & FTMS_RW_REMAIN_TIME_PRESENT_MASK)
    {
        data_length += sizeof(uint16_t);
    }

    return data_length;
}

void ftms_save_rower_notify_data(uint8_t conn_id, uint16_t data_length,
                                 T_FTMS_ROWER_DATA *p_rower_data)
{
    ftms_var.ftms_rower_notify_data = os_mem_zalloc(RAM_TYPE_DATA_ON, data_length);
    uint16_t offset = 0;
    uint8_t *p;
    ftms_var.ftms_rower_notify_data[offset] = p_rower_data->stroke_rate;
    offset += 1;

    p = ftms_var.ftms_rower_notify_data + offset;
    LE_UINT16_TO_STREAM(p, p_rower_data->stroke_count);
    offset += 2;

    ftms_var.ftms_rower_notify_flag.offset = FTMS_RW_STROKE_RATE_COUNT_LEN;

    if (ftms_var.rower_data_flag & FTMS_RW_AVE_STROKE_RATE_PRESENT_MASK)
    {
        if (ftms_var.ftms_feature.features_field & FTMS_FEAT_CADENCE_SPT_MASK)
        {
            ftms_var.ftms_rower_notify_data[offset] = p_rower_data->average_stroke_rate;
        }
        else
        {
            memset(ftms_var.ftms_rower_notify_data + offset, 0xFF, 1);
        }

        offset += 1;
    }

    if (ftms_var.rower_data_flag & FTMS_RW_TOTAL_DISTANCE_PRESENT_MASK)
    {
        if (ftms_var.ftms_feature.features_field & FTMS_FEAT_TOTAL_DISTANCE_SPT_MASK)
        {
            p = ftms_var.ftms_rower_notify_data + offset;
            LE_UINT24_TO_STREAM(p, p_rower_data->total_distance);
        }
        else
        {
            memset(ftms_var.ftms_rower_notify_data + offset, 0xFF, 3);
        }

        offset += 3;
    }

    if (ftms_var.rower_data_flag & FTMS_RW_INSTANTANEOUS_PACE_PRESENT_MASK)
    {
        if (ftms_var.ftms_feature.features_field & FTMS_FEAT_PACE_SPT_MASK)
        {
            p = ftms_var.ftms_rower_notify_data + offset;
            LE_UINT16_TO_STREAM(p, p_rower_data->instantaneous_pace);
        }
        else
        {
            memset(ftms_var.ftms_rower_notify_data + offset, 0xFF, 2);
        }

        offset += 2;
    }

    if (ftms_var.rower_data_flag & FTMS_RW_AVE_PACE_PRESENT_MASK)
    {
        if (ftms_var.ftms_feature.features_field & FTMS_FEAT_PACE_SPT_MASK)
        {
            p = ftms_var.ftms_rower_notify_data + offset;
            LE_UINT16_TO_STREAM(p, p_rower_data->average_pace);
        }
        else
        {
            memset(ftms_var.ftms_rower_notify_data + offset, 0xFF, 2);
        }

        offset += 2;
    }

    if (ftms_var.rower_data_flag & FTMS_RW_INSTANTANEOUS_POWER_PRESENT_MASK)
    {
        if (ftms_var.ftms_feature.features_field & FTMS_FEAT_POWER_MEASUREMENT_SPT_MASK)
        {
            p = ftms_var.ftms_rower_notify_data + offset;
            LE_UINT16_TO_STREAM(p, p_rower_data->instantaneous_power);
        }
        else
        {
            uint16_t invalid_data = 0x7FFF;
            p = ftms_var.ftms_rower_notify_data + offset;
            LE_UINT16_TO_STREAM(p, invalid_data);
        }

        offset += 2;
    }

    if (ftms_var.rower_data_flag & FTMS_RW_AVE_POWER_PRESENT_MASK)
    {
        if (ftms_var.ftms_feature.features_field & FTMS_FEAT_POWER_MEASUREMENT_SPT_MASK)
        {
            p = ftms_var.ftms_rower_notify_data + offset;
            LE_UINT16_TO_STREAM(p, p_rower_data->average_power);
        }
        else
        {
            uint16_t invalid_data = 0x7FFF;
            p = ftms_var.ftms_rower_notify_data + offset;
            LE_UINT16_TO_STREAM(p, invalid_data);
        }

        offset += 2;
    }

    if (ftms_var.rower_data_flag & FTMS_RW_RESISTANCE_LEVEL_MASK)
    {
        if (ftms_var.ftms_feature.features_field & FTMS_FEAT_RESISTANCE_LEVEL_SPT_MASK)
        {
            p = ftms_var.ftms_rower_notify_data + offset;
            LE_UINT16_TO_STREAM(p, p_rower_data->resistance_level);
        }
        else
        {
            uint16_t invalid_data = 0x7FFF;
            p = ftms_var.ftms_rower_notify_data + offset;
            LE_UINT16_TO_STREAM(p, invalid_data);
        }

        offset += 2;
    }

    if (ftms_var.rower_data_flag & FTMS_RW_EXPEND_ENERGY_PRESENT_MASK)
    {
        if (ftms_var.ftms_feature.features_field & FTMS_FEAT_EXPENDED_ENERGY_SPT_MASK)
        {
            p = ftms_var.ftms_rower_notify_data + offset;
            LE_UINT16_TO_STREAM(p, p_rower_data->total_energy);
            offset += 2;

            p = ftms_var.ftms_rower_notify_data + offset;
            LE_UINT16_TO_STREAM(p, p_rower_data->energy_per_hour);
            offset += 2;

            ftms_var.ftms_rower_notify_data[offset] = p_rower_data->energy_per_minute;
            offset += 1;
        }
        else
        {
            memset(ftms_var.ftms_rower_notify_data + offset, 0xFF, 5);
            offset += 5;
        }
    }

    if (ftms_var.rower_data_flag & FTMS_RW_HEART_RATE_PRESENT_MASK)
    {
        if (ftms_var.ftms_feature.features_field & FTMS_FEAT_HR_MEASUREMENT_SPT_MASK)
        {
            ftms_var.ftms_rower_notify_data[offset] = p_rower_data->heart_rate;
        }
        else
        {
            memset(ftms_var.ftms_rower_notify_data + offset, 0xFF, 1);
        }

        offset += 1;
    }

    if (ftms_var.rower_data_flag & FTMS_RW_METABOLIC_EQUIVALENT_PRESENT_MASK)
    {
        if (ftms_var.ftms_feature.features_field & FTMS_FEAT_METABOLIC_EQUIVALENT_SPT_MASK)
        {
            ftms_var.ftms_rower_notify_data[offset] = p_rower_data->metabolic_equivalent;
        }
        else
        {
            memset(ftms_var.ftms_rower_notify_data + offset, 0xFF, 1);
        }

        offset += 1;
    }

    if (ftms_var.rower_data_flag & FTMS_RW_ELAPSED_TIME_PRESENT_MASK)
    {
        if (ftms_var.ftms_feature.features_field & FTMS_FEAT_ELAPSED_TIME_SPT_MASK)
        {
            p = ftms_var.ftms_rower_notify_data + offset;
            LE_UINT16_TO_STREAM(p, p_rower_data->elapsed_time);
        }
        else
        {
            memset(ftms_var.ftms_rower_notify_data + offset, 0xFF, 2);
        }

        offset += 2;
    }

    if (ftms_var.rower_data_flag & FTMS_RW_REMAIN_TIME_PRESENT_MASK)
    {
        if (ftms_var.ftms_feature.features_field & FTMS_FEAT_REMAIN_TIME_SPT_MASK)
        {
            p = ftms_var.ftms_rower_notify_data + offset;
            LE_UINT16_TO_STREAM(p, p_rower_data->remain_time);
        }
        else
        {
            memset(ftms_var.ftms_rower_notify_data + offset, 0xFF, 2);
        }
    }
}

void ftms_get_rower_data(uint8_t conn_id, uint16_t data_length)
{
    uint16_t mtu_size;
    le_get_conn_param(GAP_PARAM_CONN_MTU_SIZE, &mtu_size, conn_id);

    uint16_t len = 0;
    uint16_t offset = 0;

    if (data_length <= (mtu_size - 5))
    {
        len = data_length + FTMS_RW_DATA_FLAG_LEN;
        ftms_var.ftms_rower_send_data = os_mem_zalloc(RAM_TYPE_DATA_ON, len);

        if (ftms_var.ftms_rower_send_data == NULL)
        {
            PROFILE_PRINT_ERROR0("ftms_get_rower_data: get data fail");
            return;
        }

        ftms_var.ftms_rower_notify_flag.send_flag = ftms_var.rower_data_flag &=
                                                        FTMS_RW_NO_MORE_DATA_PRESENT;
        offset += FTMS_RW_DATA_FLAG_LEN;
        memcpy(ftms_var.ftms_rower_send_data + offset,
               ftms_var.ftms_rower_notify_data, data_length);
        ftms_var.ftms_rower_notify_flag.data_len = len;

        if (ftms_var.ftms_rower_notify_data != NULL)
        {
            os_mem_free(ftms_var.ftms_rower_notify_data);
            ftms_var.ftms_rower_notify_data = NULL;
        }
    }
    else
    {
        uint16_t total_offset = 0;
        len = mtu_size - 3;

        ftms_var.ftms_rower_send_data = os_mem_zalloc(RAM_TYPE_DATA_ON, len);

        if (ftms_var.ftms_rower_send_data == NULL)
        {
            PROFILE_PRINT_ERROR0("ftms_get_rower_data: get data fail");
            return;
        }

        total_offset = ftms_var.ftms_rower_notify_flag.offset;

        if (ftms_var.ftms_rower_notify_flag.no_more_data == 0)
        {
            ftms_var.ftms_rower_notify_flag.send_flag |= FTMS_RW_MORE_DATA_MASK;
            offset += FTMS_RW_DATA_FLAG_LEN;
        }
        else
        {
            ftms_var.ftms_rower_notify_flag.send_flag &= FTMS_RW_NO_MORE_DATA_PRESENT;
            offset += FTMS_RW_DATA_FLAG_LEN;
            memcpy(ftms_var.ftms_rower_send_data + offset, ftms_var.ftms_rower_notify_data, 3);
            offset += FTMS_RW_STROKE_RATE_COUNT_LEN;
        }

        if (ftms_var.ftms_rower_notify_flag.cur_flag & FTMS_RW_AVE_STROKE_RATE_PRESENT_MASK)
        {
            if ((len - offset) >= sizeof(uint8_t))
            {
                memcpy(ftms_var.ftms_rower_send_data + offset, ftms_var.ftms_rower_notify_data + total_offset, 1);
                ftms_var.ftms_rower_notify_flag.send_flag |= FTMS_RW_AVE_STROKE_RATE_PRESENT_MASK;
                offset += 1;
                total_offset += 1;
                ftms_var.ftms_rower_notify_flag.cur_flag &= (~FTMS_RW_AVE_STROKE_RATE_PRESENT_MASK);
            }
            else
            {
                ftms_var.ftms_rower_notify_flag.data_len = offset;
                ftms_var.ftms_rower_notify_flag.offset = total_offset;

                if ((data_length - offset + FTMS_RW_STROKE_RATE_COUNT_LEN) <= (len - FTMS_RW_DATA_FLAG_LEN))
                {
                    ftms_var.ftms_rower_notify_flag.no_more_data = 1;
                }
                else
                {
                    ftms_var.ftms_rower_notify_flag.no_more_data = 0;
                }

                return;
            }
        }

        if (ftms_var.ftms_rower_notify_flag.cur_flag & FTMS_RW_TOTAL_DISTANCE_PRESENT_MASK)
        {
            if ((len - offset) >= FTMS_RW_TOTAL_DISTANCE_LEN)
            {
                memcpy(ftms_var.ftms_rower_send_data + offset, ftms_var.ftms_rower_notify_data + total_offset, 3);
                ftms_var.ftms_rower_notify_flag.send_flag |= FTMS_RW_TOTAL_DISTANCE_PRESENT_MASK;
                offset += 3;
                total_offset += 3;
                ftms_var.ftms_rower_notify_flag.cur_flag &= (~FTMS_RW_TOTAL_DISTANCE_PRESENT_MASK);
            }
            else
            {
                ftms_var.ftms_rower_notify_flag.data_len = offset;
                ftms_var.ftms_rower_notify_flag.offset = total_offset;

                if ((data_length - offset + FTMS_RW_STROKE_RATE_COUNT_LEN) <= (len - FTMS_RW_DATA_FLAG_LEN))
                {
                    ftms_var.ftms_rower_notify_flag.no_more_data = 1;
                }
                else
                {
                    ftms_var.ftms_rower_notify_flag.no_more_data = 0;
                }

                return;
            }
        }

        if (ftms_var.ftms_rower_notify_flag.cur_flag & FTMS_RW_INSTANTANEOUS_PACE_PRESENT_MASK)
        {
            if ((len - offset) >= sizeof(uint16_t))
            {
                memcpy(ftms_var.ftms_rower_send_data + offset, ftms_var.ftms_rower_notify_data + total_offset, 2);
                offset += 2;
                total_offset += 2;
                ftms_var.ftms_rower_notify_flag.send_flag |= FTMS_RW_INSTANTANEOUS_PACE_PRESENT_MASK;
                ftms_var.ftms_rower_notify_flag.cur_flag &= (~FTMS_RW_INSTANTANEOUS_PACE_PRESENT_MASK);
            }
            else
            {
                ftms_var.ftms_rower_notify_flag.data_len = offset;
                ftms_var.ftms_rower_notify_flag.offset = total_offset;

                if ((data_length - offset + FTMS_RW_STROKE_RATE_COUNT_LEN) <= (len - FTMS_RW_DATA_FLAG_LEN))
                {
                    ftms_var.ftms_rower_notify_flag.no_more_data = 1;
                }
                else
                {
                    ftms_var.ftms_rower_notify_flag.no_more_data = 0;
                }

                return;
            }
        }

        if (ftms_var.ftms_rower_notify_flag.cur_flag & FTMS_RW_AVE_PACE_PRESENT_MASK)
        {
            if ((len - offset) >= sizeof(uint16_t))
            {
                memcpy(ftms_var.ftms_rower_send_data + offset, ftms_var.ftms_rower_notify_data + total_offset, 2);
                offset += 2;
                total_offset += 2;
                ftms_var.ftms_rower_notify_flag.send_flag |= FTMS_RW_AVE_PACE_PRESENT_MASK;
                ftms_var.ftms_rower_notify_flag.cur_flag &= (~FTMS_RW_AVE_PACE_PRESENT_MASK);
            }
            else
            {
                ftms_var.ftms_rower_notify_flag.data_len = offset;
                ftms_var.ftms_rower_notify_flag.offset = total_offset;

                if ((data_length - offset + FTMS_RW_STROKE_RATE_COUNT_LEN) <= (len - FTMS_RW_DATA_FLAG_LEN))
                {
                    ftms_var.ftms_rower_notify_flag.no_more_data = 1;
                }
                else
                {
                    ftms_var.ftms_rower_notify_flag.no_more_data = 0;
                }

                return;
            }
        }

        if (ftms_var.ftms_rower_notify_flag.cur_flag & FTMS_RW_INSTANTANEOUS_POWER_PRESENT_MASK)
        {
            if ((len - offset) >= sizeof(int16_t))
            {
                memcpy(ftms_var.ftms_rower_send_data + offset, ftms_var.ftms_rower_notify_data + total_offset, 2);
                ftms_var.ftms_rower_notify_flag.send_flag |= FTMS_RW_INSTANTANEOUS_POWER_PRESENT_MASK;
                offset += 2;
                total_offset += 2;
                ftms_var.ftms_rower_notify_flag.cur_flag &= (~FTMS_RW_INSTANTANEOUS_POWER_PRESENT_MASK);
            }
            else
            {
                ftms_var.ftms_rower_notify_flag.data_len = offset;
                ftms_var.ftms_rower_notify_flag.offset = total_offset;

                if ((data_length - offset + FTMS_RW_STROKE_RATE_COUNT_LEN) <= (len - FTMS_RW_DATA_FLAG_LEN))
                {
                    ftms_var.ftms_rower_notify_flag.no_more_data = 1;
                }
                else
                {
                    ftms_var.ftms_rower_notify_flag.no_more_data = 0;
                }

                return;
            }
        }

        if (ftms_var.ftms_rower_notify_flag.cur_flag & FTMS_RW_AVE_POWER_PRESENT_MASK)
        {
            if ((len - offset) >= sizeof(int16_t))
            {
                memcpy(ftms_var.ftms_rower_send_data + offset, ftms_var.ftms_rower_notify_data + total_offset, 2);
                ftms_var.ftms_rower_notify_flag.send_flag |= FTMS_RW_AVE_POWER_PRESENT_MASK;
                offset += 2;
                total_offset += 2;
                ftms_var.ftms_rower_notify_flag.cur_flag &= (~FTMS_RW_AVE_POWER_PRESENT_MASK);
            }
            else
            {
                ftms_var.ftms_rower_notify_flag.data_len = offset;
                ftms_var.ftms_rower_notify_flag.offset = total_offset;

                if ((data_length - offset + FTMS_RW_STROKE_RATE_COUNT_LEN) <= (len - FTMS_RW_DATA_FLAG_LEN))
                {
                    ftms_var.ftms_rower_notify_flag.no_more_data = 1;
                }
                else
                {
                    ftms_var.ftms_rower_notify_flag.no_more_data = 0;
                }

                return;
            }
        }

        if (ftms_var.ftms_rower_notify_flag.cur_flag & FTMS_RW_RESISTANCE_LEVEL_MASK)
        {
            if ((len - offset) >= sizeof(int16_t))
            {
                memcpy(ftms_var.ftms_rower_send_data + offset, ftms_var.ftms_rower_notify_data + total_offset, 2);
                offset += 2;
                total_offset += 2;
                ftms_var.ftms_rower_notify_flag.send_flag |= FTMS_RW_RESISTANCE_LEVEL_MASK;
                ftms_var.ftms_rower_notify_flag.cur_flag &= (~FTMS_RW_RESISTANCE_LEVEL_MASK);
            }
            else
            {
                ftms_var.ftms_rower_notify_flag.data_len = offset;
                ftms_var.ftms_rower_notify_flag.offset = total_offset;

                if ((data_length - offset + FTMS_RW_STROKE_RATE_COUNT_LEN) <= (len - FTMS_RW_DATA_FLAG_LEN))
                {
                    ftms_var.ftms_rower_notify_flag.no_more_data = 1;
                }
                else
                {
                    ftms_var.ftms_rower_notify_flag.no_more_data = 0;
                }

                return;
            }
        }

        if (ftms_var.ftms_rower_notify_flag.cur_flag & FTMS_RW_EXPEND_ENERGY_PRESENT_MASK)
        {
            if ((len - offset) >= FTMS_RW_EXPENDED_ENERGY_LEN)
            {
                memcpy(ftms_var.ftms_rower_send_data + offset, ftms_var.ftms_rower_notify_data + total_offset, 5);
                ftms_var.ftms_rower_notify_flag.send_flag |= FTMS_RW_EXPEND_ENERGY_PRESENT_MASK;
                offset += 5;
                total_offset += 5;
                ftms_var.ftms_rower_notify_flag.cur_flag &= (~FTMS_RW_EXPEND_ENERGY_PRESENT_MASK);
            }
            else
            {
                ftms_var.ftms_rower_notify_flag.data_len = offset;
                ftms_var.ftms_rower_notify_flag.offset = total_offset;

                if ((data_length - offset + FTMS_RW_STROKE_RATE_COUNT_LEN) <= (len - FTMS_RW_DATA_FLAG_LEN))
                {
                    ftms_var.ftms_rower_notify_flag.no_more_data = 1;
                }
                else
                {
                    ftms_var.ftms_rower_notify_flag.no_more_data = 0;
                }

                return;
            }
        }

        if (ftms_var.ftms_rower_notify_flag.cur_flag & FTMS_RW_HEART_RATE_PRESENT_MASK)
        {
            if ((len - offset) >= sizeof(uint8_t))
            {
                memcpy(ftms_var.ftms_rower_send_data + offset, ftms_var.ftms_rower_notify_data + total_offset, 1);
                ftms_var.ftms_rower_notify_flag.send_flag |= FTMS_RW_HEART_RATE_PRESENT_MASK;
                offset += 1;
                total_offset += 1;
                ftms_var.ftms_rower_notify_flag.cur_flag &= (~FTMS_RW_HEART_RATE_PRESENT_MASK);
            }
            else
            {
                ftms_var.ftms_rower_notify_flag.data_len = offset;
                ftms_var.ftms_rower_notify_flag.offset = total_offset;

                if ((data_length - offset + FTMS_RW_STROKE_RATE_COUNT_LEN) <= (len - FTMS_RW_DATA_FLAG_LEN))
                {
                    ftms_var.ftms_rower_notify_flag.no_more_data = 1;
                }
                else
                {
                    ftms_var.ftms_rower_notify_flag.no_more_data = 0;
                }

                return;
            }
        }

        if (ftms_var.ftms_rower_notify_flag.cur_flag & FTMS_RW_METABOLIC_EQUIVALENT_PRESENT_MASK)
        {
            if ((len - offset) >= sizeof(uint8_t))
            {
                memcpy(ftms_var.ftms_rower_send_data + offset, ftms_var.ftms_rower_notify_data + total_offset, 1);
                ftms_var.ftms_rower_notify_flag.send_flag |= FTMS_RW_METABOLIC_EQUIVALENT_PRESENT_MASK;
                offset += 1;
                total_offset += 1;
                ftms_var.ftms_rower_notify_flag.cur_flag &= (~FTMS_RW_METABOLIC_EQUIVALENT_PRESENT_MASK);
            }
            else
            {
                ftms_var.ftms_rower_notify_flag.data_len = offset;
                ftms_var.ftms_rower_notify_flag.offset = total_offset;

                if ((data_length - offset + FTMS_RW_STROKE_RATE_COUNT_LEN) <= (len - FTMS_RW_DATA_FLAG_LEN))
                {
                    ftms_var.ftms_rower_notify_flag.no_more_data = 1;
                }
                else
                {
                    ftms_var.ftms_rower_notify_flag.no_more_data = 0;
                }

                return;
            }
        }

        if (ftms_var.ftms_rower_notify_flag.cur_flag & FTMS_RW_ELAPSED_TIME_PRESENT_MASK)
        {
            if ((len - offset) >= sizeof(uint16_t))
            {
                memcpy(ftms_var.ftms_rower_send_data + offset, ftms_var.ftms_rower_notify_data + total_offset, 2);
                ftms_var.ftms_rower_notify_flag.send_flag |= FTMS_RW_ELAPSED_TIME_PRESENT_MASK;
                offset += 2;
                total_offset += 2;
                ftms_var.ftms_rower_notify_flag.cur_flag &= (~FTMS_RW_ELAPSED_TIME_PRESENT_MASK);
            }
            else
            {
                ftms_var.ftms_rower_notify_flag.data_len = offset;
                ftms_var.ftms_rower_notify_flag.offset = total_offset;

                if ((data_length - offset + FTMS_RW_STROKE_RATE_COUNT_LEN) <= (len - FTMS_RW_DATA_FLAG_LEN))
                {
                    ftms_var.ftms_rower_notify_flag.no_more_data = 1;
                }
                else
                {
                    ftms_var.ftms_rower_notify_flag.no_more_data = 0;
                }

                return;
            }
        }

        if (ftms_var.ftms_rower_notify_flag.cur_flag & FTMS_RW_REMAIN_TIME_PRESENT_MASK)
        {
            if ((len - offset) >= sizeof(uint16_t))
            {
                memcpy(ftms_var.ftms_rower_send_data + offset, ftms_var.ftms_rower_notify_data + total_offset, 2);
                offset += 2;
                total_offset += 2;
                ftms_var.ftms_rower_notify_flag.send_flag |= FTMS_RW_REMAIN_TIME_PRESENT_MASK;
                ftms_var.ftms_rower_notify_flag.cur_flag &= (~FTMS_RW_REMAIN_TIME_PRESENT_MASK);
            }
            else
            {
                ftms_var.ftms_rower_notify_flag.data_len = offset;
                ftms_var.ftms_rower_notify_flag.offset = total_offset;

                if ((data_length - offset + FTMS_RW_STROKE_RATE_COUNT_LEN) <= (len - FTMS_RW_DATA_FLAG_LEN))
                {
                    ftms_var.ftms_rower_notify_flag.no_more_data = 1;
                }
                else
                {
                    ftms_var.ftms_rower_notify_flag.no_more_data = 0;
                }

                return;
            }
        }

        ftms_var.ftms_rower_notify_flag.data_len = offset;
        ftms_var.ftms_rower_notify_flag.offset = 0;
        ftms_var.ftms_rower_notify_flag.no_more_data = 0;

        if (ftms_var.ftms_rower_notify_data != NULL)
        {
            os_mem_free(ftms_var.ftms_rower_notify_data);
            ftms_var.ftms_rower_notify_data = NULL;
        }
    }
}

bool ftms_send_rower_data(uint8_t conn_id, T_SERVER_ID service_id, uint16_t data_length)
{
    bool ret = false;
    uint16_t mtu_size;
    le_get_conn_param(GAP_PARAM_CONN_MTU_SIZE, &mtu_size, conn_id);

    T_SRV_UUID_TBL *p_char;
    p_char = srv_find_index_by_uuid(ftms_var.ftms_srv_char_tbl, GATT_UUID_ROWER_DATA);
    uint16_t attrib_index = p_char->char_index;

    if (data_length <= (mtu_size - 5))
    {
        ftms_get_rower_data(conn_id, data_length);

        if (ftms_var.ftms_rower_send_data == NULL)
        {
            return ret;
        }

        uint8_t *p = ftms_var.ftms_rower_send_data;
        LE_UINT16_TO_STREAM(p, ftms_var.ftms_rower_notify_flag.send_flag);

        if (server_send_data(conn_id, service_id, attrib_index, ftms_var.ftms_rower_send_data,
                             ftms_var.ftms_rower_notify_flag.data_len, GATT_PDU_TYPE_NOTIFICATION))
        {
            ret = true;
        }

        if (ftms_var.ftms_rower_send_data != NULL)
        {
            os_mem_free(ftms_var.ftms_rower_send_data);
            ftms_var.ftms_rower_send_data = NULL;
        }

        ftms_var.ftms_rower_notify_flag.send_flag = 0;
        ftms_var.ftms_rower_notify_flag.data_len = 0;
    }
    else
    {
        ftms_var.ftms_rower_notify_flag.cur_flag =
            ftms_var.rower_data_flag & FTMS_RW_NO_MORE_DATA_PRESENT;

        while (ftms_var.ftms_rower_notify_flag.cur_flag)
        {
            ftms_get_rower_data(conn_id, data_length);

            if (ftms_var.ftms_rower_send_data == NULL)
            {
                ret = false;
                return ret;
            }

            uint8_t *p = ftms_var.ftms_rower_send_data;
            LE_UINT16_TO_STREAM(p, ftms_var.ftms_rower_notify_flag.send_flag);

            if (server_send_data(conn_id, service_id, attrib_index, ftms_var.ftms_rower_send_data,
                                 ftms_var.ftms_rower_notify_flag.data_len, GATT_PDU_TYPE_NOTIFICATION))
            {
                if (ftms_var.ftms_rower_send_data != NULL)
                {
                    os_mem_free(ftms_var.ftms_rower_send_data);
                    ftms_var.ftms_rower_send_data = NULL;
                }

                ftms_var.ftms_rower_notify_flag.send_flag = 0;
                ftms_var.ftms_rower_notify_flag.data_len = 0;
                ret = true;
            }
            else
            {
                if (ftms_var.ftms_rower_send_data != NULL)
                {
                    os_mem_free(ftms_var.ftms_rower_send_data);
                    ftms_var.ftms_rower_send_data = NULL;
                }

                ftms_var.ftms_rower_notify_flag.send_flag = 0;
                ftms_var.ftms_rower_notify_flag.data_len = 0;
                ret = false;
                return ret;
            }
        }
    }

    return ret;
}

bool ftms_rower_data_notify(uint8_t conn_id, T_SERVER_ID service_id,
                            T_FTMS_ROWER_DATA *p_rower_data)
{
    bool ret = true;
    if (ftms_var.ftms_notify_indicate_flag.ftms_rower_data_notify_enable == 0)
    {
        ret = false;
        PROFILE_PRINT_INFO0("ftms_rower_data_notify:FTMS_APP_RESULT_CCCD_NOT_ENABLED");
        return ret;
    }

    uint16_t data_length = ftms_rower_data_length();
    ftms_save_rower_notify_data(conn_id, data_length, p_rower_data);

    return ftms_send_rower_data(conn_id, service_id, data_length);
}
#endif

#if FTMS_CHAR_INDOOR_BIKE_DATA_SUPPORT
uint16_t ftms_indoor_bike_data_length(void)
{
    uint16_t data_length = FTMS_IB_INSTANTANEOUS_SPEED_LEN;
    if (ftms_var.indoor_bike_data_flag & FTMS_IB_AVE_SPEED_PRESENT_MASK)
    {
        data_length += sizeof(uint16_t);
    }
    if (ftms_var.indoor_bike_data_flag & FTMS_IB_INSTANTANEOUS_CADENCE_MASK)
    {
        data_length += sizeof(uint16_t);
    }
    if (ftms_var.indoor_bike_data_flag & FTMS_IB_AVE_CADENCE_PRESENT_MASK)
    {
        data_length += sizeof(uint16_t);
    }
    if (ftms_var.indoor_bike_data_flag & FTMS_IB_TOTAL_DISTANCE_PRESENT_MASK)
    {
        data_length += FTMS_IB_TOTAL_DISTANCE_LEN;
    }
    if (ftms_var.indoor_bike_data_flag & FTMS_IB_RESISTANCE_LEVEL_PRESENT_MASK)
    {
        data_length += sizeof(int16_t);
    }
    if (ftms_var.indoor_bike_data_flag & FTMS_IB_INSTANTANEOUS_POWER_PRESENT_MASK)
    {
        data_length += sizeof(int16_t);
    }
    if (ftms_var.indoor_bike_data_flag & FTMS_IB_AVE_POWER_PRESENT_MASK)
    {
        data_length += sizeof(uint16_t);
    }
    if (ftms_var.indoor_bike_data_flag & FTMS_IB_EXPEND_ENERGY_PRESENT_MASK)
    {
        data_length += FTMS_IB_EXPENDED_ENERGY_LEN;
    }
    if (ftms_var.indoor_bike_data_flag & FTMS_IB_HEART_RATE_PRESENT_MASK)
    {
        data_length += sizeof(uint8_t);
    }
    if (ftms_var.indoor_bike_data_flag & FTMS_IB_METABOLIC_EQUIVALENT_PRESENT_MASK)
    {
        data_length += sizeof(uint8_t);
    }
    if (ftms_var.indoor_bike_data_flag & FTMS_IB_ELAPSED_TIME_PRESENT_MASK)
    {
        data_length += sizeof(uint16_t);
    }
    if (ftms_var.indoor_bike_data_flag & FTMS_IB_REMAIN_TIME_PRESENT_MASK)
    {
        data_length += sizeof(uint16_t);
    }

    return data_length;
}

void ftms_save_indoor_bike_notify_data(uint8_t conn_id, uint16_t data_length,
                                       T_FTMS_INDOOR_BIKE_DATA *p_indoor_bike_data)
{
    ftms_var.ftms_indoor_bike_notify_data = os_mem_zalloc(RAM_TYPE_DATA_ON, data_length);
    uint16_t offset = 0;
    uint8_t *p;
    p = ftms_var.ftms_indoor_bike_notify_data + offset;
    LE_UINT16_TO_STREAM(p, p_indoor_bike_data->instantaneous_speed);
    offset += 2;

    ftms_var.ftms_indoor_bike_notify_flag.offset = FTMS_IB_INSTANTANEOUS_SPEED_LEN;

    if (ftms_var.indoor_bike_data_flag & FTMS_IB_AVE_SPEED_PRESENT_MASK)
    {
        if (ftms_var.ftms_feature.features_field & FTMS_FEAT_AVERAGE_SPEED_SPT_MASK)
        {
            p = ftms_var.ftms_indoor_bike_notify_data + offset;
            LE_UINT16_TO_STREAM(p, p_indoor_bike_data->average_speed);
        }
        else
        {
            memset(ftms_var.ftms_indoor_bike_notify_data + offset, 0xFF, 2);
        }

        offset += 2;
    }

    if (ftms_var.indoor_bike_data_flag & FTMS_IB_INSTANTANEOUS_CADENCE_MASK)
    {
        if (ftms_var.ftms_feature.features_field & FTMS_FEAT_CADENCE_SPT_MASK)
        {
            p = ftms_var.ftms_indoor_bike_notify_data + offset;
            LE_UINT16_TO_STREAM(p, p_indoor_bike_data->instantaneous_cadence);
        }
        else
        {
            memset(ftms_var.ftms_indoor_bike_notify_data + offset, 0xFF, 2);
        }

        offset += 2;
    }
    if (ftms_var.indoor_bike_data_flag & FTMS_IB_AVE_CADENCE_PRESENT_MASK)
    {
        if (ftms_var.ftms_feature.features_field & FTMS_FEAT_CADENCE_SPT_MASK)
        {
            p = ftms_var.ftms_indoor_bike_notify_data + offset;
            LE_UINT16_TO_STREAM(p, p_indoor_bike_data->average_cadence);
        }
        else
        {
            memset(ftms_var.ftms_indoor_bike_notify_data + offset, 0xFF, 2);
        }

        offset += 2;
    }
    if (ftms_var.indoor_bike_data_flag & FTMS_IB_TOTAL_DISTANCE_PRESENT_MASK)
    {
        if (ftms_var.ftms_feature.features_field & FTMS_FEAT_TOTAL_DISTANCE_SPT_MASK)
        {
            p = ftms_var.ftms_indoor_bike_notify_data + offset;
            LE_UINT24_TO_STREAM(p, p_indoor_bike_data->total_distance);
        }
        else
        {
            memset(ftms_var.ftms_indoor_bike_notify_data + offset, 0xFF, 3);
        }

        offset += 3;
    }
    if (ftms_var.indoor_bike_data_flag & FTMS_IB_RESISTANCE_LEVEL_PRESENT_MASK)
    {
        if (ftms_var.ftms_feature.features_field & FTMS_FEAT_RESISTANCE_LEVEL_SPT_MASK)
        {
            p = ftms_var.ftms_indoor_bike_notify_data + offset;
            LE_UINT16_TO_STREAM(p, p_indoor_bike_data->resistance_level);
        }
        else
        {
            uint16_t invalid_data = 0x7FFF;
            p = ftms_var.ftms_indoor_bike_notify_data + offset;
            LE_UINT16_TO_STREAM(p, invalid_data);
        }

        offset += 2;
    }
    if (ftms_var.indoor_bike_data_flag & FTMS_IB_INSTANTANEOUS_POWER_PRESENT_MASK)
    {
        if (ftms_var.ftms_feature.features_field & FTMS_FEAT_POWER_MEASUREMENT_SPT_MASK)
        {
            p = ftms_var.ftms_indoor_bike_notify_data + offset;
            LE_UINT16_TO_STREAM(p, p_indoor_bike_data->instantaneous_power);
        }
        else
        {
            uint16_t invalid_data = 0x7FFF;
            p = ftms_var.ftms_indoor_bike_notify_data + offset;
            LE_UINT16_TO_STREAM(p, invalid_data);
        }

        offset += 2;
    }
    if (ftms_var.indoor_bike_data_flag & FTMS_IB_AVE_POWER_PRESENT_MASK)
    {
        if (ftms_var.ftms_feature.features_field & FTMS_FEAT_POWER_MEASUREMENT_SPT_MASK)
        {
            p = ftms_var.ftms_indoor_bike_notify_data + offset;
            LE_UINT16_TO_STREAM(p, p_indoor_bike_data->average_power);
        }
        else
        {
            uint16_t invalid_data = 0x7FFF;
            p = ftms_var.ftms_indoor_bike_notify_data + offset;
            LE_UINT16_TO_STREAM(p, invalid_data);
        }

        offset += 2;
    }
    if (ftms_var.indoor_bike_data_flag & FTMS_IB_EXPEND_ENERGY_PRESENT_MASK)
    {
        if (ftms_var.ftms_feature.features_field & FTMS_FEAT_EXPENDED_ENERGY_SPT_MASK)
        {
            p = ftms_var.ftms_indoor_bike_notify_data + offset;
            LE_UINT16_TO_STREAM(p, p_indoor_bike_data->total_energy);
            offset += 2;

            p = ftms_var.ftms_indoor_bike_notify_data + offset;
            LE_UINT16_TO_STREAM(p, p_indoor_bike_data->energy_per_hour);
            offset += 2;

            ftms_var.ftms_indoor_bike_notify_data[offset] = p_indoor_bike_data->energy_per_minute;
            offset += 1;
        }
        else
        {
            memset(ftms_var.ftms_indoor_bike_notify_data + offset, 0xFF, 5);
            offset += 5;
        }
    }
    if (ftms_var.indoor_bike_data_flag & FTMS_IB_HEART_RATE_PRESENT_MASK)
    {
        if (ftms_var.ftms_feature.features_field & FTMS_FEAT_HR_MEASUREMENT_SPT_MASK)
        {
            ftms_var.ftms_indoor_bike_notify_data[offset] = p_indoor_bike_data->heart_rate;
        }
        else
        {
            memset(ftms_var.ftms_indoor_bike_notify_data + offset, 0xFF, 1);
        }

        offset += 1;
    }
    if (ftms_var.indoor_bike_data_flag & FTMS_IB_METABOLIC_EQUIVALENT_PRESENT_MASK)
    {
        if (ftms_var.ftms_feature.features_field & FTMS_FEAT_METABOLIC_EQUIVALENT_SPT_MASK)
        {
            ftms_var.ftms_indoor_bike_notify_data[offset] = p_indoor_bike_data->metabolic_equivalent;
        }
        else
        {
            memset(ftms_var.ftms_indoor_bike_notify_data + offset, 0xFF, 1);
        }

        offset += 1;
    }
    if (ftms_var.indoor_bike_data_flag & FTMS_IB_ELAPSED_TIME_PRESENT_MASK)
    {
        if (ftms_var.ftms_feature.features_field & FTMS_FEAT_ELAPSED_TIME_SPT_MASK)
        {
            p = ftms_var.ftms_indoor_bike_notify_data + offset;
            LE_UINT16_TO_STREAM(p, p_indoor_bike_data->elapsed_time);
        }
        else
        {
            memset(ftms_var.ftms_indoor_bike_notify_data + offset, 0xFF, 2);
        }

        offset += 2;
    }
    if (ftms_var.indoor_bike_data_flag & FTMS_IB_REMAIN_TIME_PRESENT_MASK)
    {
        if (ftms_var.ftms_feature.features_field & FTMS_FEAT_REMAIN_TIME_SPT_MASK)
        {
            p = ftms_var.ftms_indoor_bike_notify_data + offset;
            LE_UINT16_TO_STREAM(p, p_indoor_bike_data->remain_time);
        }
        else
        {
            memset(ftms_var.ftms_indoor_bike_notify_data + offset, 0xFF, 2);
        }
    }
}

void ftms_get_indoor_bike_data(uint8_t conn_id, uint16_t data_length)
{
    uint16_t mtu_size;
    le_get_conn_param(GAP_PARAM_CONN_MTU_SIZE, &mtu_size, conn_id);

    uint16_t len = 0;
    uint16_t offset = 0;

    if (data_length <= (mtu_size - 5))
    {
        len = data_length + FTMS_IB_DATA_FLAG_LEN;
        ftms_var.ftms_indoor_bike_send_data = os_mem_zalloc(RAM_TYPE_DATA_ON, len);

        if (ftms_var.ftms_indoor_bike_send_data == NULL)
        {
            PROFILE_PRINT_ERROR0("ftms_get_indoor_bike_data: get data fail");
            return;
        }

        ftms_var.indoor_bike_data_flag &= FTMS_IB_NO_MORE_DATA_PRESENT;
        ftms_var.ftms_indoor_bike_notify_flag.send_flag = ftms_var.indoor_bike_data_flag;
        offset += FTMS_IB_DATA_FLAG_LEN;
        memcpy(ftms_var.ftms_indoor_bike_send_data + offset, ftms_var.ftms_indoor_bike_notify_data,
               data_length);
        ftms_var.ftms_indoor_bike_notify_flag.data_len = len;

        if (ftms_var.ftms_indoor_bike_notify_data != NULL)
        {
            os_mem_free(ftms_var.ftms_indoor_bike_notify_data);
            ftms_var.ftms_indoor_bike_notify_data = NULL;
        }
    }
    else
    {
        uint16_t total_offset = 0;
        len = mtu_size - 3;

        ftms_var.ftms_indoor_bike_send_data = os_mem_zalloc(RAM_TYPE_DATA_ON, len);

        if (ftms_var.ftms_indoor_bike_send_data == NULL)
        {
            PROFILE_PRINT_ERROR0("ftms_get_indoor_bike_data: get data fail");
            return;
        }

        total_offset = ftms_var.ftms_indoor_bike_notify_flag.offset;

        if (ftms_var.ftms_indoor_bike_notify_flag.no_more_data == 0)
        {
            ftms_var.ftms_indoor_bike_notify_flag.send_flag |= FTMS_IB_MORE_DATA_MASK;
            offset += FTMS_IB_DATA_FLAG_LEN;
        }
        else
        {
            ftms_var.ftms_indoor_bike_notify_flag.send_flag &= FTMS_IB_NO_MORE_DATA_PRESENT;
            offset += FTMS_IB_DATA_FLAG_LEN;
            memcpy(ftms_var.ftms_indoor_bike_send_data + offset, ftms_var.ftms_indoor_bike_notify_data, 2);
            offset += FTMS_IB_INSTANTANEOUS_SPEED_LEN;
        }

        if (ftms_var.ftms_indoor_bike_notify_flag.cur_flag & FTMS_IB_AVE_SPEED_PRESENT_MASK)
        {
            if ((len - offset) >= sizeof(uint16_t))
            {
                memcpy(ftms_var.ftms_indoor_bike_send_data + offset,
                       ftms_var.ftms_indoor_bike_notify_data + total_offset, 2);
                ftms_var.ftms_indoor_bike_notify_flag.send_flag |= FTMS_IB_AVE_SPEED_PRESENT_MASK;
                offset += 2;
                total_offset += 2;
                ftms_var.ftms_indoor_bike_notify_flag.cur_flag &= (~FTMS_IB_AVE_SPEED_PRESENT_MASK);
            }
            else
            {
                ftms_var.ftms_indoor_bike_notify_flag.data_len = offset;
                ftms_var.ftms_indoor_bike_notify_flag.offset = total_offset;

                if ((data_length - offset + FTMS_IB_INSTANTANEOUS_SPEED_LEN) <= (len - FTMS_IB_DATA_FLAG_LEN))
                {
                    ftms_var.ftms_indoor_bike_notify_flag.no_more_data = 1;
                }
                else
                {
                    ftms_var.ftms_indoor_bike_notify_flag.no_more_data = 0;
                }

                return;
            }
        }

        if (ftms_var.ftms_indoor_bike_notify_flag.cur_flag & FTMS_IB_INSTANTANEOUS_CADENCE_MASK)
        {
            if ((len - offset) >= sizeof(uint16_t))
            {
                memcpy(ftms_var.ftms_indoor_bike_send_data + offset,
                       ftms_var.ftms_indoor_bike_notify_data + total_offset, 2);
                ftms_var.ftms_indoor_bike_notify_flag.send_flag |= FTMS_IB_INSTANTANEOUS_CADENCE_MASK;
                offset += 2;
                total_offset += 2;
                ftms_var.ftms_indoor_bike_notify_flag.cur_flag &= (~FTMS_IB_INSTANTANEOUS_CADENCE_MASK);
            }
            else
            {
                ftms_var.ftms_indoor_bike_notify_flag.data_len = offset;
                ftms_var.ftms_indoor_bike_notify_flag.offset = total_offset;

                if ((data_length - offset + FTMS_IB_INSTANTANEOUS_SPEED_LEN) <= (FTMS_IB_DATA_FLAG_LEN))
                {
                    ftms_var.ftms_indoor_bike_notify_flag.no_more_data = 1;
                }
                else
                {
                    ftms_var.ftms_indoor_bike_notify_flag.no_more_data = 0;
                }

                return;
            }
        }

        if (ftms_var.ftms_indoor_bike_notify_flag.cur_flag & FTMS_IB_AVE_CADENCE_PRESENT_MASK)
        {
            if ((len - offset) >= sizeof(uint16_t))
            {
                memcpy(ftms_var.ftms_indoor_bike_send_data + offset,
                       ftms_var.ftms_indoor_bike_notify_data + total_offset, 2);
                offset += 2;
                total_offset += 2;
                ftms_var.ftms_indoor_bike_notify_flag.send_flag |= FTMS_IB_AVE_CADENCE_PRESENT_MASK;
                ftms_var.ftms_indoor_bike_notify_flag.cur_flag &= (~FTMS_IB_AVE_CADENCE_PRESENT_MASK);
            }
            else
            {
                ftms_var.ftms_indoor_bike_notify_flag.data_len = offset;
                ftms_var.ftms_indoor_bike_notify_flag.offset = total_offset;

                if ((data_length - offset + FTMS_IB_INSTANTANEOUS_SPEED_LEN) <= (FTMS_IB_DATA_FLAG_LEN))
                {
                    ftms_var.ftms_indoor_bike_notify_flag.no_more_data = 1;
                }
                else
                {
                    ftms_var.ftms_indoor_bike_notify_flag.no_more_data = 0;
                }

                return;
            }
        }

        if (ftms_var.ftms_indoor_bike_notify_flag.cur_flag & FTMS_IB_TOTAL_DISTANCE_PRESENT_MASK)
        {
            if ((len - offset) >= FTMS_IB_TOTAL_DISTANCE_LEN)
            {
                memcpy(ftms_var.ftms_indoor_bike_send_data + offset,
                       ftms_var.ftms_indoor_bike_notify_data + total_offset, 3);
                offset += 3;
                total_offset += 3;
                ftms_var.ftms_indoor_bike_notify_flag.send_flag |= FTMS_IB_TOTAL_DISTANCE_PRESENT_MASK;
                ftms_var.ftms_indoor_bike_notify_flag.cur_flag &= (~FTMS_IB_TOTAL_DISTANCE_PRESENT_MASK);
            }
            else
            {
                ftms_var.ftms_indoor_bike_notify_flag.data_len = offset;
                ftms_var.ftms_indoor_bike_notify_flag.offset = total_offset;

                if ((data_length - offset + FTMS_IB_INSTANTANEOUS_SPEED_LEN) <= (FTMS_IB_DATA_FLAG_LEN))
                {
                    ftms_var.ftms_indoor_bike_notify_flag.no_more_data = 1;
                }
                else
                {
                    ftms_var.ftms_indoor_bike_notify_flag.no_more_data = 0;
                }

                return;
            }
        }

        if (ftms_var.ftms_indoor_bike_notify_flag.cur_flag & FTMS_IB_RESISTANCE_LEVEL_PRESENT_MASK)
        {
            if ((len - offset) >= sizeof(int16_t))
            {
                memcpy(ftms_var.ftms_indoor_bike_send_data + offset,
                       ftms_var.ftms_indoor_bike_notify_data + total_offset, 2);
                ftms_var.ftms_indoor_bike_notify_flag.send_flag |= FTMS_IB_RESISTANCE_LEVEL_PRESENT_MASK;
                offset += 2;
                total_offset += 2;
                ftms_var.ftms_indoor_bike_notify_flag.cur_flag &= (~FTMS_IB_RESISTANCE_LEVEL_PRESENT_MASK);
            }
            else
            {
                ftms_var.ftms_indoor_bike_notify_flag.data_len = offset;
                ftms_var.ftms_indoor_bike_notify_flag.offset = total_offset;

                if ((data_length - offset + FTMS_IB_INSTANTANEOUS_SPEED_LEN) <= (FTMS_IB_DATA_FLAG_LEN))
                {
                    ftms_var.ftms_indoor_bike_notify_flag.no_more_data = 1;
                }
                else
                {
                    ftms_var.ftms_indoor_bike_notify_flag.no_more_data = 0;
                }

                return;
            }
        }

        if (ftms_var.ftms_indoor_bike_notify_flag.cur_flag & FTMS_IB_INSTANTANEOUS_POWER_PRESENT_MASK)
        {
            if ((len - offset) >= sizeof(uint16_t))
            {
                memcpy(ftms_var.ftms_indoor_bike_send_data + offset,
                       ftms_var.ftms_indoor_bike_notify_data + total_offset, 2);
                ftms_var.ftms_indoor_bike_notify_flag.send_flag |= FTMS_IB_INSTANTANEOUS_POWER_PRESENT_MASK;
                offset += 2;
                total_offset += 2;
                ftms_var.ftms_indoor_bike_notify_flag.cur_flag &= (~FTMS_IB_INSTANTANEOUS_POWER_PRESENT_MASK);
            }
            else
            {
                ftms_var.ftms_indoor_bike_notify_flag.data_len = offset;
                ftms_var.ftms_indoor_bike_notify_flag.offset = total_offset;

                if ((data_length - offset + FTMS_IB_INSTANTANEOUS_SPEED_LEN) <= (FTMS_IB_DATA_FLAG_LEN))
                {
                    ftms_var.ftms_indoor_bike_notify_flag.no_more_data = 1;
                }
                else
                {
                    ftms_var.ftms_indoor_bike_notify_flag.no_more_data = 0;
                }

                return;
            }
        }

        if (ftms_var.ftms_indoor_bike_notify_flag.cur_flag & FTMS_IB_AVE_POWER_PRESENT_MASK)
        {
            if ((len - offset) >= sizeof(uint16_t))
            {
                memcpy(ftms_var.ftms_indoor_bike_send_data + offset,
                       ftms_var.ftms_indoor_bike_notify_data + total_offset, 2);
                offset += 2;
                total_offset += 2;
                ftms_var.ftms_indoor_bike_notify_flag.send_flag |= FTMS_IB_AVE_POWER_PRESENT_MASK;
                ftms_var.ftms_indoor_bike_notify_flag.cur_flag &= (~FTMS_IB_AVE_POWER_PRESENT_MASK);
            }
            else
            {
                ftms_var.ftms_indoor_bike_notify_flag.data_len = offset;
                ftms_var.ftms_indoor_bike_notify_flag.offset = total_offset;

                if ((data_length - offset + FTMS_IB_INSTANTANEOUS_SPEED_LEN) <= (FTMS_IB_DATA_FLAG_LEN))
                {
                    ftms_var.ftms_indoor_bike_notify_flag.no_more_data = 1;
                }
                else
                {
                    ftms_var.ftms_indoor_bike_notify_flag.no_more_data = 0;
                }

                return;
            }
        }

        if (ftms_var.ftms_indoor_bike_notify_flag.cur_flag & FTMS_IB_EXPEND_ENERGY_PRESENT_MASK)
        {
            if ((len - offset) >= FTMS_IB_EXPENDED_ENERGY_LEN)
            {
                memcpy(ftms_var.ftms_indoor_bike_send_data + offset,
                       ftms_var.ftms_indoor_bike_notify_data + total_offset, 5);
                ftms_var.ftms_indoor_bike_notify_flag.send_flag |= FTMS_IB_EXPEND_ENERGY_PRESENT_MASK;
                offset += 5;
                total_offset += 5;
                ftms_var.ftms_indoor_bike_notify_flag.cur_flag &= (~FTMS_IB_EXPEND_ENERGY_PRESENT_MASK);
            }
            else
            {
                ftms_var.ftms_indoor_bike_notify_flag.data_len = offset;
                ftms_var.ftms_indoor_bike_notify_flag.offset = total_offset;

                if ((data_length - offset + FTMS_IB_INSTANTANEOUS_SPEED_LEN) <= (FTMS_IB_DATA_FLAG_LEN))
                {
                    ftms_var.ftms_indoor_bike_notify_flag.no_more_data = 1;
                }
                else
                {
                    ftms_var.ftms_indoor_bike_notify_flag.no_more_data = 0;
                }

                return;
            }
        }

        if (ftms_var.ftms_indoor_bike_notify_flag.cur_flag & FTMS_IB_HEART_RATE_PRESENT_MASK)
        {
            if ((len - offset) >= sizeof(uint8_t))
            {
                memcpy(ftms_var.ftms_indoor_bike_send_data + offset,
                       ftms_var.ftms_indoor_bike_notify_data + total_offset, 1);
                ftms_var.ftms_indoor_bike_notify_flag.send_flag |= FTMS_IB_HEART_RATE_PRESENT_MASK;
                offset += 1;
                total_offset += 1;
                ftms_var.ftms_indoor_bike_notify_flag.cur_flag &= (~FTMS_IB_HEART_RATE_PRESENT_MASK);
            }
            else
            {
                ftms_var.ftms_indoor_bike_notify_flag.data_len = offset;
                ftms_var.ftms_indoor_bike_notify_flag.offset = total_offset;

                if ((data_length - offset + FTMS_IB_INSTANTANEOUS_SPEED_LEN) <= (FTMS_IB_DATA_FLAG_LEN))
                {
                    ftms_var.ftms_indoor_bike_notify_flag.no_more_data = 1;
                }
                else
                {
                    ftms_var.ftms_indoor_bike_notify_flag.no_more_data = 0;
                }

                return;
            }
        }

        if (ftms_var.ftms_indoor_bike_notify_flag.cur_flag & FTMS_IB_METABOLIC_EQUIVALENT_PRESENT_MASK)
        {
            if ((len - offset) >= sizeof(uint8_t))
            {
                memcpy(ftms_var.ftms_indoor_bike_send_data + offset,
                       ftms_var.ftms_indoor_bike_notify_data + total_offset, 1);
                ftms_var.ftms_indoor_bike_notify_flag.send_flag |= FTMS_IB_METABOLIC_EQUIVALENT_PRESENT_MASK;
                offset += 1;
                total_offset += 1;
                ftms_var.ftms_indoor_bike_notify_flag.cur_flag &= (~FTMS_IB_METABOLIC_EQUIVALENT_PRESENT_MASK);
            }
            else
            {
                ftms_var.ftms_indoor_bike_notify_flag.data_len = offset;
                ftms_var.ftms_indoor_bike_notify_flag.offset = total_offset;

                if ((data_length - offset + FTMS_IB_INSTANTANEOUS_SPEED_LEN) <= (FTMS_IB_DATA_FLAG_LEN))
                {
                    ftms_var.ftms_indoor_bike_notify_flag.no_more_data = 1;
                }
                else
                {
                    ftms_var.ftms_indoor_bike_notify_flag.no_more_data = 0;
                }

                return;
            }
        }

        if (ftms_var.ftms_indoor_bike_notify_flag.cur_flag & FTMS_IB_ELAPSED_TIME_PRESENT_MASK)
        {
            if ((len - offset) >= sizeof(uint16_t))
            {
                memcpy(ftms_var.ftms_indoor_bike_send_data + offset,
                       ftms_var.ftms_indoor_bike_notify_data + total_offset, 2);
                ftms_var.ftms_indoor_bike_notify_flag.send_flag |= FTMS_IB_ELAPSED_TIME_PRESENT_MASK;
                offset += 2;
                total_offset += 2;
                ftms_var.ftms_indoor_bike_notify_flag.cur_flag &= (~FTMS_IB_ELAPSED_TIME_PRESENT_MASK);
            }
            else
            {
                ftms_var.ftms_indoor_bike_notify_flag.data_len = offset;
                ftms_var.ftms_indoor_bike_notify_flag.offset = total_offset;

                if ((data_length - offset + FTMS_IB_INSTANTANEOUS_SPEED_LEN) <= (FTMS_IB_DATA_FLAG_LEN))
                {
                    ftms_var.ftms_indoor_bike_notify_flag.no_more_data = 1;
                }
                else
                {
                    ftms_var.ftms_indoor_bike_notify_flag.no_more_data = 0;
                }

                return;
            }
        }

        if (ftms_var.ftms_indoor_bike_notify_flag.cur_flag & FTMS_IB_REMAIN_TIME_PRESENT_MASK)
        {
            if ((len - offset) >= sizeof(uint16_t))
            {
                memcpy(ftms_var.ftms_indoor_bike_send_data + offset,
                       ftms_var.ftms_indoor_bike_notify_data + total_offset, 2);
                offset += 2;
                total_offset += 2;
                ftms_var.ftms_indoor_bike_notify_flag.send_flag |= FTMS_IB_REMAIN_TIME_PRESENT_MASK;
                ftms_var.ftms_indoor_bike_notify_flag.cur_flag &= (~FTMS_IB_REMAIN_TIME_PRESENT_MASK);
            }
            else
            {
                ftms_var.ftms_indoor_bike_notify_flag.data_len = offset;
                ftms_var.ftms_indoor_bike_notify_flag.offset = total_offset;

                if ((data_length - offset + FTMS_IB_INSTANTANEOUS_SPEED_LEN) <= (FTMS_IB_DATA_FLAG_LEN))
                {
                    ftms_var.ftms_indoor_bike_notify_flag.no_more_data = 1;
                }
                else
                {
                    ftms_var.ftms_indoor_bike_notify_flag.no_more_data = 0;
                }

                return;
            }
        }

        ftms_var.ftms_indoor_bike_notify_flag.data_len = offset;
        ftms_var.ftms_indoor_bike_notify_flag.offset = 0;
        ftms_var.ftms_indoor_bike_notify_flag.no_more_data = 0;

        if (ftms_var.ftms_indoor_bike_notify_data != NULL)
        {
            os_mem_free(ftms_var.ftms_indoor_bike_notify_data);
            ftms_var.ftms_indoor_bike_notify_data = NULL;
        }
    }
}

bool ftms_send_indoor_bike_data(uint8_t conn_id, T_SERVER_ID service_id, uint16_t data_length)
{
    bool ret = false;
    uint16_t mtu_size;
    le_get_conn_param(GAP_PARAM_CONN_MTU_SIZE, &mtu_size, conn_id);

    T_SRV_UUID_TBL *p_char;
    p_char = srv_find_index_by_uuid(ftms_var.ftms_srv_char_tbl, GATT_UUID_INDOOR_BIKE_DATA);
    uint16_t attrib_index = p_char->char_index;

    if (data_length <= (mtu_size - 5))
    {
        ftms_get_indoor_bike_data(conn_id, data_length);

        if (ftms_var.ftms_indoor_bike_send_data == NULL)
        {
            return ret;
        }

        uint8_t *p = ftms_var.ftms_indoor_bike_send_data;
        LE_UINT16_TO_STREAM(p, ftms_var.ftms_indoor_bike_notify_flag.send_flag);

        if (server_send_data(conn_id, service_id, attrib_index, ftms_var.ftms_indoor_bike_send_data,
                             ftms_var.ftms_indoor_bike_notify_flag.data_len, GATT_PDU_TYPE_NOTIFICATION))
        {
            ret = true;
        }

        if (ftms_var.ftms_indoor_bike_send_data != NULL)
        {
            os_mem_free(ftms_var.ftms_indoor_bike_send_data);
            ftms_var.ftms_indoor_bike_send_data = NULL;
        }

        ftms_var.ftms_indoor_bike_notify_flag.send_flag = 0;
        ftms_var.ftms_indoor_bike_notify_flag.data_len = 0;
    }
    else
    {
        ftms_var.ftms_indoor_bike_notify_flag.cur_flag =
            ftms_var.indoor_bike_data_flag & FTMS_IB_NO_MORE_DATA_PRESENT;

        while (ftms_var.ftms_indoor_bike_notify_flag.cur_flag)
        {
            ftms_get_indoor_bike_data(conn_id, data_length);

            if (ftms_var.ftms_indoor_bike_send_data == NULL)
            {
                ret = false;
                return ret;
            }

            uint8_t *p = ftms_var.ftms_indoor_bike_send_data;
            LE_UINT16_TO_STREAM(p, ftms_var.ftms_indoor_bike_notify_flag.send_flag);

            if (server_send_data(conn_id, service_id, attrib_index, ftms_var.ftms_indoor_bike_send_data,
                                 ftms_var.ftms_indoor_bike_notify_flag.data_len, GATT_PDU_TYPE_NOTIFICATION))
            {
                if (ftms_var.ftms_indoor_bike_send_data != NULL)
                {
                    os_mem_free(ftms_var.ftms_indoor_bike_send_data);
                    ftms_var.ftms_indoor_bike_send_data = NULL;
                }

                ftms_var.ftms_indoor_bike_notify_flag.send_flag = 0;
                ftms_var.ftms_indoor_bike_notify_flag.data_len = 0;
                ret = true;
            }
            else
            {
                if (ftms_var.ftms_indoor_bike_send_data != NULL)
                {
                    os_mem_free(ftms_var.ftms_indoor_bike_send_data);
                    ftms_var.ftms_indoor_bike_send_data = NULL;
                }

                ftms_var.ftms_indoor_bike_notify_flag.send_flag = 0;
                ftms_var.ftms_indoor_bike_notify_flag.data_len = 0;
                ret = false;
                return ret;
            }
        }
    }

    return ret;
}

bool ftms_indoor_bike_data_notify(uint8_t conn_id, T_SERVER_ID service_id,
                                  T_FTMS_INDOOR_BIKE_DATA *p_indoor_bike_data)
{
    bool ret = true;
    if (ftms_var.ftms_notify_indicate_flag.ftms_indoor_bike_data_notify_enable == 0)
    {
        ret = false;
        PROFILE_PRINT_INFO0("ftms_indoor_bike_data_notify:FTMS_APP_RESULT_CCCD_NOT_ENABLED");
        return ret;
    }

    uint16_t data_length = ftms_indoor_bike_data_length();
    ftms_save_indoor_bike_notify_data(conn_id, data_length, p_indoor_bike_data);

    return ftms_send_indoor_bike_data(conn_id, service_id, data_length);
}
#endif

#if FTMS_CHAR_CROSS_TRAINER_DATA_SUPPORT
uint16_t ftms_cross_trainer_data_length(void)
{
    uint16_t data_length = FTMS_CT_INSTANTANEOUS_SPEED_LEN;
    if (ftms_var.cross_trainer_data_flag & FTMS_CT_AVE_SPEED_PRESENT_MASK)
    {
        data_length += sizeof(uint16_t);
    }
    if (ftms_var.cross_trainer_data_flag & FTMS_CT_TOTAL_DISTANCE_PRESENT_MASK)
    {
        data_length += FTMS_CT_TOTAL_DISTANCE_LEN;
    }
    if (ftms_var.cross_trainer_data_flag & FTMS_CT_STEP_COUNT_PRESENT_MASK)
    {
        data_length += FTMS_CT_STEP_PER_MINUTE_STEP_RATE_LEN;
    }
    if (ftms_var.cross_trainer_data_flag & FTMS_CT_STRIDE_COUNT_PRESENT_MASK)
    {
        data_length += sizeof(uint16_t);
    }
    if (ftms_var.cross_trainer_data_flag & FTMS_CT_ELEVATION_GAIN_PRESENT_MASK)
    {
        data_length += FTMS_CT_ELEVATION_GAIN_LEN;
    }
    if (ftms_var.cross_trainer_data_flag & FTMS_CT_INCLINATION_RAMP_ANGLE_SET_PRESENT_MASK)
    {
        data_length += FTMS_CT_INCLINATION_RAMP_ANGLE_LEN;
    }
    if (ftms_var.cross_trainer_data_flag & FTMS_CT_RESISTANCE_LEVEL_PRESENT_MASK)
    {
        data_length += sizeof(int16_t);
    }
    if (ftms_var.cross_trainer_data_flag & FTMS_CT_INSTANTANEOUS_POWER_PRESENT_MASK)
    {
        data_length += sizeof(int16_t);
    }
    if (ftms_var.cross_trainer_data_flag & FTMS_CT_AVE_POWER_PRESENT_MASK)
    {
        data_length += sizeof(int16_t);
    }
    if (ftms_var.cross_trainer_data_flag & FTMS_CT_EXPEND_ENERGY_PRESENT_MASK)
    {
        data_length += FTMS_CT_EXPENDED_ENERGY_LEN;
    }
    if (ftms_var.cross_trainer_data_flag & FTMS_CT_HEART_RATE_PRESENT_MASK)
    {
        data_length += sizeof(uint8_t);
    }
    if (ftms_var.cross_trainer_data_flag & FTMS_CT_METABOLIC_EQUIVALENT_PRESENT_MASK)
    {
        data_length += sizeof(uint8_t);
    }
    if (ftms_var.cross_trainer_data_flag & FTMS_CT_ELAPSED_TIME_PRESENT_MASK)
    {
        data_length += sizeof(uint16_t);
    }
    if (ftms_var.cross_trainer_data_flag & FTMS_CT_REMAIN_TIME_PRESENT_MASK)
    {
        data_length += sizeof(uint16_t);
    }

    return data_length;
}

void ftms_save_cross_trainer_notify_data(uint8_t conn_id, uint16_t data_length,
                                         T_FTMS_CROSS_TRAINER_DATA *p_cross_trainer_data)
{
    ftms_var.ftms_cross_trainer_notify_data = os_mem_zalloc(RAM_TYPE_DATA_ON, data_length);
    uint16_t offset = 0;
    uint8_t *p;
    p = ftms_var.ftms_cross_trainer_notify_data + offset;

    LE_UINT16_TO_STREAM(p, p_cross_trainer_data->instantaneous_speed);

    offset += FTMS_CT_INSTANTANEOUS_SPEED_LEN;

    ftms_var.ftms_cross_trainer_notify_flag.offset = FTMS_CT_INSTANTANEOUS_SPEED_LEN;

    if (ftms_var.cross_trainer_data_flag & FTMS_CT_AVE_SPEED_PRESENT_MASK)
    {
        if (ftms_var.ftms_feature.features_field & FTMS_FEAT_AVERAGE_SPEED_SPT_MASK)
        {
            p = ftms_var.ftms_cross_trainer_notify_data + offset;
            LE_UINT16_TO_STREAM(p, p_cross_trainer_data->average_speed);
        }
        else
        {
            memset(ftms_var.ftms_cross_trainer_notify_data + offset, 0xFF, 2);
        }

        offset += 2;
    }

    if (ftms_var.cross_trainer_data_flag & FTMS_CT_TOTAL_DISTANCE_PRESENT_MASK)
    {
        if (ftms_var.ftms_feature.features_field & FTMS_FEAT_TOTAL_DISTANCE_SPT_MASK)
        {
            p = ftms_var.ftms_cross_trainer_notify_data + offset;
            LE_UINT24_TO_STREAM(p, p_cross_trainer_data->total_distance);
        }
        else
        {
            memset(ftms_var.ftms_cross_trainer_notify_data + offset, 0xFF, 3);
        }

        offset += 3;
    }

    if (ftms_var.cross_trainer_data_flag & FTMS_CT_STEP_COUNT_PRESENT_MASK)
    {
        if (ftms_var.ftms_feature.features_field & FTMS_FEAT_STEP_COUNT_SPT_MASK)
        {
            p = ftms_var.ftms_cross_trainer_notify_data + offset;
            LE_UINT16_TO_STREAM(p, p_cross_trainer_data->inclination);
            offset += 2;
            p = ftms_var.ftms_cross_trainer_notify_data + offset;
            LE_UINT16_TO_STREAM(p, p_cross_trainer_data->ramp_angle_set);
            offset += 2;
        }
        else
        {
            uint16_t invalid_data = 0x7FFF;
            p = ftms_var.ftms_cross_trainer_notify_data + offset;
            LE_UINT16_TO_STREAM(p, invalid_data);
            offset += 2;
            p = ftms_var.ftms_cross_trainer_notify_data + offset;
            LE_UINT16_TO_STREAM(p, invalid_data);
        }
    }

    if (ftms_var.cross_trainer_data_flag & FTMS_CT_STRIDE_COUNT_PRESENT_MASK)
    {
        if (ftms_var.ftms_feature.features_field & FTMS_FEAT_STRIDE_COUNT_SPT_MASK)
        {
            p = ftms_var.ftms_cross_trainer_notify_data + offset;
            LE_UINT16_TO_STREAM(p, p_cross_trainer_data->stride_count);
        }
        else
        {
            memset(ftms_var.ftms_cross_trainer_notify_data + offset, 0xFF, 2);
        }

        offset += 2;
    }

    if (ftms_var.cross_trainer_data_flag & FTMS_CT_ELEVATION_GAIN_PRESENT_MASK)
    {
        if (ftms_var.ftms_feature.features_field & FTMS_FEAT_ELEVATION_GAIN_SPT_MASK)
        {
            p = ftms_var.ftms_cross_trainer_notify_data + offset;
            LE_UINT16_TO_STREAM(p, p_cross_trainer_data->positive_elevation_gain);
            offset += 2;
            p = ftms_var.ftms_cross_trainer_notify_data + offset;
            LE_UINT16_TO_STREAM(p, p_cross_trainer_data->negative_elevation_gain);
            offset += 2;
        }
        else
        {
            memset(ftms_var.ftms_cross_trainer_notify_data + offset, 0xFF, 4);
            offset += 4;
        }
    }

    if (ftms_var.cross_trainer_data_flag & FTMS_CT_INCLINATION_RAMP_ANGLE_SET_PRESENT_MASK)
    {
        if (ftms_var.ftms_feature.features_field & FTMS_FEAT_INCLINATION_SPT_MASK)
        {
            p = ftms_var.ftms_cross_trainer_notify_data + offset;
            LE_UINT16_TO_STREAM(p, p_cross_trainer_data->inclination);
            offset += 2;
            p = ftms_var.ftms_cross_trainer_notify_data + offset;
            LE_UINT16_TO_STREAM(p, p_cross_trainer_data->ramp_angle_set);
            offset += 2;
        }
        else
        {
            uint16_t invalid_data = 0x7FFF;
            p = ftms_var.ftms_cross_trainer_notify_data + offset;
            LE_UINT16_TO_STREAM(p, invalid_data);
            offset += 2;
            p = ftms_var.ftms_cross_trainer_notify_data + offset;
            LE_UINT16_TO_STREAM(p, invalid_data);
            offset += 2;
        }
    }

    if (ftms_var.cross_trainer_data_flag & FTMS_CT_RESISTANCE_LEVEL_PRESENT_MASK)
    {
        if (ftms_var.ftms_feature.features_field & FTMS_FEAT_RESISTANCE_LEVEL_SPT_MASK)
        {
            p = ftms_var.ftms_cross_trainer_notify_data + offset;
            LE_UINT16_TO_STREAM(p, p_cross_trainer_data->resistance_level);
        }
        else
        {
            uint16_t invalid_data = 0x7FFF;
            p = ftms_var.ftms_cross_trainer_notify_data + offset;
            LE_UINT16_TO_STREAM(p, invalid_data);
        }

        offset += 2;
    }

    if (ftms_var.cross_trainer_data_flag & FTMS_CT_INSTANTANEOUS_POWER_PRESENT_MASK)
    {
        if (ftms_var.ftms_feature.features_field & FTMS_FEAT_POWER_MEASUREMENT_SPT_MASK)
        {
            p = ftms_var.ftms_cross_trainer_notify_data + offset;
            LE_UINT16_TO_STREAM(p, p_cross_trainer_data->instantaneous_power);
        }
        else
        {
            uint16_t invalid_data = 0x7FFF;
            p = ftms_var.ftms_cross_trainer_notify_data + offset;
            LE_UINT16_TO_STREAM(p, invalid_data);
        }

        offset += 2;
    }

    if (ftms_var.cross_trainer_data_flag & FTMS_CT_AVE_POWER_PRESENT_MASK)
    {
        if (ftms_var.ftms_feature.features_field & FTMS_FEAT_POWER_MEASUREMENT_SPT_MASK)
        {
            p = ftms_var.ftms_cross_trainer_notify_data + offset;
            LE_UINT16_TO_STREAM(p, p_cross_trainer_data->average_power);
        }
        else
        {
            uint16_t invalid_data = 0x7FFF;
            p = ftms_var.ftms_cross_trainer_notify_data + offset;
            LE_UINT16_TO_STREAM(p, invalid_data);
        }

        offset += 2;
    }

    if (ftms_var.cross_trainer_data_flag & FTMS_CT_EXPEND_ENERGY_PRESENT_MASK)
    {
        if (ftms_var.ftms_feature.features_field & FTMS_FEAT_EXPENDED_ENERGY_SPT_MASK)
        {
            p = ftms_var.ftms_cross_trainer_notify_data + offset;
            LE_UINT16_TO_STREAM(p, p_cross_trainer_data->total_energy);
            offset += 2;
            p = ftms_var.ftms_cross_trainer_notify_data + offset;
            LE_UINT16_TO_STREAM(p, p_cross_trainer_data->energy_per_hour);
            offset += 2;
            p = ftms_var.ftms_cross_trainer_notify_data + offset;
            LE_UINT8_TO_STREAM(p, p_cross_trainer_data->energy_per_minute);
            offset += 1;
        }
        else
        {
            memset(ftms_var.ftms_cross_trainer_notify_data + offset, 0xFF, 5);
            offset += 5;
        }
    }

    if (ftms_var.cross_trainer_data_flag & FTMS_CT_HEART_RATE_PRESENT_MASK)
    {
        if (ftms_var.ftms_feature.features_field & FTMS_FEAT_HR_MEASUREMENT_SPT_MASK)
        {
            ftms_var.ftms_cross_trainer_notify_data[offset] = p_cross_trainer_data->heart_rate;
        }
        else
        {
            memset(ftms_var.ftms_cross_trainer_notify_data + offset, 0xFF, 1);
        }

        offset += 1;
    }

    if (ftms_var.cross_trainer_data_flag & FTMS_CT_METABOLIC_EQUIVALENT_PRESENT_MASK)
    {
        if (ftms_var.ftms_feature.features_field & FTMS_FEAT_METABOLIC_EQUIVALENT_SPT_MASK)
        {
            ftms_var.ftms_cross_trainer_notify_data[offset] = p_cross_trainer_data->metabolic_equivalent;
        }
        else
        {
            memset(ftms_var.ftms_cross_trainer_notify_data + offset, 0xFF, 1);
        }

        offset += 1;
    }

    if (ftms_var.cross_trainer_data_flag & FTMS_CT_ELAPSED_TIME_PRESENT_MASK)
    {
        if (ftms_var.ftms_feature.features_field & FTMS_FEAT_ELAPSED_TIME_SPT_MASK)
        {
            p = ftms_var.ftms_cross_trainer_notify_data + offset;
            LE_UINT16_TO_STREAM(p, p_cross_trainer_data->elapsed_time);
        }
        else
        {
            memset(ftms_var.ftms_cross_trainer_notify_data + offset, 0xFF, 2);
        }

        offset += 2;
    }

    if (ftms_var.cross_trainer_data_flag & FTMS_CT_REMAIN_TIME_PRESENT_MASK)
    {
        if (ftms_var.ftms_feature.features_field & FTMS_FEAT_REMAIN_TIME_SPT_MASK)
        {
            p = ftms_var.ftms_cross_trainer_notify_data + offset;
            LE_UINT16_TO_STREAM(p, p_cross_trainer_data->remain_time);
        }
        else
        {
            memset(ftms_var.ftms_cross_trainer_notify_data + offset, 0xFF, 2);
        }
    }
}

void ftms_get_cross_trainer_data(uint8_t conn_id, uint16_t data_length)
{
    uint16_t mtu_size;
    le_get_conn_param(GAP_PARAM_CONN_MTU_SIZE, &mtu_size, conn_id);

    uint16_t len = 0;
    uint16_t offset = 0;

    if (data_length <= (mtu_size - 5))
    {
        len = data_length + FTMS_CT_DATA_FLAG_LEN;
        ftms_var.ftms_cross_trainer_send_data = os_mem_zalloc(RAM_TYPE_DATA_ON, len);

        if (ftms_var.ftms_cross_trainer_send_data == NULL)
        {
            PROFILE_PRINT_ERROR0("ftms_get_cross_trainer_data: get data fail");
            return;
        }

        ftms_var.cross_trainer_data_flag &= FTMS_CT_NO_MORE_DATA_PRESENT;
        ftms_var.ftms_cross_trainer_notify_flag.ct_send_flag = ftms_var.cross_trainer_data_flag;
        offset += FTMS_CT_DATA_FLAG_LEN;

        memcpy(ftms_var.ftms_cross_trainer_send_data + offset, ftms_var.ftms_cross_trainer_notify_data,
               data_length);
        ftms_var.ftms_cross_trainer_notify_flag.data_len = len;

        if (ftms_var.ftms_cross_trainer_notify_data != NULL)
        {
            os_mem_free(ftms_var.ftms_cross_trainer_notify_data);
            ftms_var.ftms_cross_trainer_notify_data = NULL;
        }
    }
    else
    {
        uint16_t total_offset = 0;
        len = mtu_size - 3;

        ftms_var.ftms_cross_trainer_send_data = os_mem_zalloc(RAM_TYPE_DATA_ON, len);

        if (ftms_var.ftms_cross_trainer_send_data == NULL)
        {
            PROFILE_PRINT_ERROR0("ftms_get_cross_trainer_data: get data fail");
            return;
        }

        total_offset = ftms_var.ftms_cross_trainer_notify_flag.offset;

        if (ftms_var.ftms_cross_trainer_notify_flag.no_more_data == 0)
        {
            ftms_var.ftms_cross_trainer_notify_flag.ct_send_flag |= FTMS_CT_MORE_DATA_MASK;
            offset += FTMS_CT_DATA_FLAG_LEN;
        }
        else
        {
            ftms_var.ftms_cross_trainer_notify_flag.ct_send_flag &= FTMS_CT_NO_MORE_DATA_PRESENT;
            offset += FTMS_CT_DATA_FLAG_LEN;
            memcpy(ftms_var.ftms_cross_trainer_send_data + offset,
                   ftms_var.ftms_cross_trainer_notify_data, 2);
            offset += FTMS_CT_INSTANTANEOUS_SPEED_LEN;
        }

        if (ftms_var.ftms_cross_trainer_notify_flag.ct_cur_flag & FTMS_CT_AVE_SPEED_PRESENT_MASK)
        {
            if ((len - offset) >= sizeof(uint16_t))
            {
                memcpy(ftms_var.ftms_cross_trainer_send_data + offset,
                       ftms_var.ftms_cross_trainer_notify_data + total_offset, 2);
                ftms_var.ftms_cross_trainer_notify_flag.ct_send_flag |= FTMS_CT_AVE_SPEED_PRESENT_MASK;
                offset += 2;
                total_offset += 2;
                ftms_var.ftms_cross_trainer_notify_flag.ct_cur_flag &= (~FTMS_CT_AVE_SPEED_PRESENT_MASK);
            }
            else
            {
                ftms_var.ftms_cross_trainer_notify_flag.data_len = offset;
                ftms_var.ftms_cross_trainer_notify_flag.offset = total_offset;

                if ((data_length - offset + FTMS_CT_INSTANTANEOUS_SPEED_LEN) <= (len - FTMS_CT_DATA_FLAG_LEN))
                {
                    ftms_var.ftms_cross_trainer_notify_flag.no_more_data = 1;
                }
                else
                {
                    ftms_var.ftms_cross_trainer_notify_flag.no_more_data = 0;
                }

                return;
            }
        }

        if (ftms_var.ftms_cross_trainer_notify_flag.ct_cur_flag & FTMS_CT_TOTAL_DISTANCE_PRESENT_MASK)
        {
            if ((len - offset) >= FTMS_CT_TOTAL_DISTANCE_LEN)
            {
                memcpy(ftms_var.ftms_cross_trainer_send_data + offset,
                       ftms_var.ftms_cross_trainer_notify_data + total_offset, 3);
                ftms_var.ftms_cross_trainer_notify_flag.ct_send_flag |= FTMS_CT_TOTAL_DISTANCE_PRESENT_MASK;
                offset += 3;
                total_offset += 3;
                ftms_var.ftms_cross_trainer_notify_flag.ct_cur_flag &= (~FTMS_CT_TOTAL_DISTANCE_PRESENT_MASK);
            }
            else
            {
                ftms_var.ftms_cross_trainer_notify_flag.data_len = offset;
                ftms_var.ftms_cross_trainer_notify_flag.offset = total_offset;

                if ((data_length - offset + FTMS_CT_INSTANTANEOUS_SPEED_LEN) <= (len - FTMS_CT_DATA_FLAG_LEN))
                {
                    ftms_var.ftms_cross_trainer_notify_flag.no_more_data = 1;
                }
                else
                {
                    ftms_var.ftms_cross_trainer_notify_flag.no_more_data = 0;
                }

                return;
            }
        }

        if (ftms_var.ftms_cross_trainer_notify_flag.ct_cur_flag & FTMS_CT_STEP_COUNT_PRESENT_MASK)
        {
            if ((len - offset) >= FTMS_CT_STEP_PER_MINUTE_STEP_RATE_LEN)
            {
                memcpy(ftms_var.ftms_cross_trainer_send_data + offset,
                       ftms_var.ftms_cross_trainer_notify_data + total_offset, 4);
                ftms_var.ftms_cross_trainer_notify_flag.ct_send_flag |= FTMS_CT_STEP_COUNT_PRESENT_MASK;
                offset += 4;
                total_offset += 4;
                ftms_var.ftms_cross_trainer_notify_flag.ct_cur_flag &= (~FTMS_CT_STEP_COUNT_PRESENT_MASK);
            }
            else
            {
                ftms_var.ftms_cross_trainer_notify_flag.data_len = offset;
                ftms_var.ftms_cross_trainer_notify_flag.offset = total_offset;

                if ((data_length - offset + FTMS_CT_INSTANTANEOUS_SPEED_LEN) <= (len - FTMS_CT_DATA_FLAG_LEN))
                {
                    ftms_var.ftms_cross_trainer_notify_flag.no_more_data = 1;
                }
                else
                {
                    ftms_var.ftms_cross_trainer_notify_flag.no_more_data = 0;
                }

                return;
            }
        }

        if (ftms_var.ftms_cross_trainer_notify_flag.ct_cur_flag & FTMS_CT_STRIDE_COUNT_PRESENT_MASK)
        {
            if ((len - offset) >= sizeof(uint16_t))
            {
                memcpy(ftms_var.ftms_cross_trainer_send_data + offset,
                       ftms_var.ftms_cross_trainer_notify_data + total_offset, 2);
                ftms_var.ftms_cross_trainer_notify_flag.ct_send_flag |= FTMS_CT_STRIDE_COUNT_PRESENT_MASK;
                offset += 2;
                total_offset += 2;
                ftms_var.ftms_cross_trainer_notify_flag.ct_cur_flag &= (~FTMS_CT_STRIDE_COUNT_PRESENT_MASK);
            }
            else
            {
                ftms_var.ftms_cross_trainer_notify_flag.data_len = offset;
                ftms_var.ftms_cross_trainer_notify_flag.offset = total_offset;

                if ((data_length - offset + FTMS_CT_INSTANTANEOUS_SPEED_LEN) <= (len - FTMS_CT_DATA_FLAG_LEN))
                {
                    ftms_var.ftms_cross_trainer_notify_flag.no_more_data = 1;
                }
                else
                {
                    ftms_var.ftms_cross_trainer_notify_flag.no_more_data = 0;
                }

                return;
            }
        }

        if (ftms_var.ftms_cross_trainer_notify_flag.ct_cur_flag & FTMS_CT_ELEVATION_GAIN_PRESENT_MASK)
        {
            if ((len - offset) >= FTMS_CT_ELEVATION_GAIN_LEN)
            {
                memcpy(ftms_var.ftms_cross_trainer_send_data + offset,
                       ftms_var.ftms_cross_trainer_notify_data + total_offset, 4);
                ftms_var.ftms_cross_trainer_notify_flag.ct_send_flag |= FTMS_CT_ELEVATION_GAIN_PRESENT_MASK;
                offset += 4;
                total_offset += 4;
                ftms_var.ftms_cross_trainer_notify_flag.ct_cur_flag &= (~FTMS_CT_ELEVATION_GAIN_PRESENT_MASK);
            }
            else
            {
                ftms_var.ftms_cross_trainer_notify_flag.data_len = offset;
                ftms_var.ftms_cross_trainer_notify_flag.offset = total_offset;

                if ((data_length - offset + FTMS_CT_INSTANTANEOUS_SPEED_LEN) <= (len - FTMS_CT_DATA_FLAG_LEN))
                {
                    ftms_var.ftms_cross_trainer_notify_flag.no_more_data = 1;
                }
                else
                {
                    ftms_var.ftms_cross_trainer_notify_flag.no_more_data = 0;
                }

                return;
            }
        }

        if (ftms_var.ftms_cross_trainer_notify_flag.ct_cur_flag &
            FTMS_CT_INCLINATION_RAMP_ANGLE_SET_PRESENT_MASK)
        {
            if ((len - offset) >= FTMS_CT_INCLINATION_RAMP_ANGLE_LEN)
            {
                memcpy(ftms_var.ftms_cross_trainer_send_data + offset,
                       ftms_var.ftms_cross_trainer_notify_data + total_offset, 4);
                ftms_var.ftms_cross_trainer_notify_flag.ct_send_flag |=
                    FTMS_CT_INCLINATION_RAMP_ANGLE_SET_PRESENT_MASK;
                offset += 4;
                total_offset += 4;
                ftms_var.ftms_cross_trainer_notify_flag.ct_cur_flag &=
                    (~FTMS_CT_INCLINATION_RAMP_ANGLE_SET_PRESENT_MASK);
            }
            else
            {
                ftms_var.ftms_cross_trainer_notify_flag.data_len = offset;
                ftms_var.ftms_cross_trainer_notify_flag.offset = total_offset;

                if ((data_length - offset + FTMS_CT_INSTANTANEOUS_SPEED_LEN) <= (len - FTMS_CT_DATA_FLAG_LEN))
                {
                    ftms_var.ftms_cross_trainer_notify_flag.no_more_data = 1;
                }
                else
                {
                    ftms_var.ftms_cross_trainer_notify_flag.no_more_data = 0;
                }

                return;
            }
        }

        if (ftms_var.ftms_cross_trainer_notify_flag.ct_cur_flag & FTMS_CT_RESISTANCE_LEVEL_PRESENT_MASK)
        {
            if ((len - offset) >= sizeof(int16_t))
            {
                memcpy(ftms_var.ftms_cross_trainer_send_data + offset,
                       ftms_var.ftms_cross_trainer_notify_data + total_offset, 2);
                ftms_var.ftms_cross_trainer_notify_flag.ct_send_flag |= FTMS_CT_RESISTANCE_LEVEL_PRESENT_MASK;
                offset += 2;
                total_offset += 2;
                ftms_var.ftms_cross_trainer_notify_flag.ct_cur_flag &= (~FTMS_CT_RESISTANCE_LEVEL_PRESENT_MASK);
            }
            else
            {
                ftms_var.ftms_cross_trainer_notify_flag.data_len = offset;
                ftms_var.ftms_cross_trainer_notify_flag.offset = total_offset;

                if ((data_length - offset + FTMS_CT_INSTANTANEOUS_SPEED_LEN) <= (len - FTMS_CT_DATA_FLAG_LEN))
                {
                    ftms_var.ftms_cross_trainer_notify_flag.no_more_data = 1;
                }
                else
                {
                    ftms_var.ftms_cross_trainer_notify_flag.no_more_data = 0;
                }

                return;
            }
        }

        if (ftms_var.ftms_cross_trainer_notify_flag.ct_cur_flag & FTMS_CT_INSTANTANEOUS_POWER_PRESENT_MASK)
        {
            if ((len - offset) >= sizeof(int16_t))
            {
                memcpy(ftms_var.ftms_cross_trainer_send_data + offset,
                       ftms_var.ftms_cross_trainer_notify_data + total_offset, 2);
                ftms_var.ftms_cross_trainer_notify_flag.ct_send_flag |= FTMS_CT_INSTANTANEOUS_POWER_PRESENT_MASK;
                offset += 2;
                total_offset += 2;
                ftms_var.ftms_cross_trainer_notify_flag.ct_cur_flag &= (~FTMS_CT_INSTANTANEOUS_POWER_PRESENT_MASK);
            }
            else
            {
                ftms_var.ftms_cross_trainer_notify_flag.data_len = offset;
                ftms_var.ftms_cross_trainer_notify_flag.offset = total_offset;

                if ((data_length - offset + FTMS_CT_INSTANTANEOUS_SPEED_LEN) <= (len - FTMS_CT_DATA_FLAG_LEN))
                {
                    ftms_var.ftms_cross_trainer_notify_flag.no_more_data = 1;
                }
                else
                {
                    ftms_var.ftms_cross_trainer_notify_flag.no_more_data = 0;
                }

                return;
            }
        }

        if (ftms_var.ftms_cross_trainer_notify_flag.ct_cur_flag & FTMS_CT_AVE_POWER_PRESENT_MASK)
        {
            if ((len - offset) >= sizeof(int16_t))
            {
                memcpy(ftms_var.ftms_cross_trainer_send_data + offset,
                       ftms_var.ftms_cross_trainer_notify_data + total_offset, 2);
                ftms_var.ftms_cross_trainer_notify_flag.ct_send_flag |= FTMS_CT_AVE_POWER_PRESENT_MASK;
                offset += 2;
                total_offset += 2;
                ftms_var.ftms_cross_trainer_notify_flag.ct_cur_flag &= (~FTMS_CT_AVE_POWER_PRESENT_MASK);
            }
            else
            {
                ftms_var.ftms_cross_trainer_notify_flag.data_len = offset;
                ftms_var.ftms_cross_trainer_notify_flag.offset = total_offset;

                if ((data_length - offset + FTMS_CT_INSTANTANEOUS_SPEED_LEN) <= (len - FTMS_CT_DATA_FLAG_LEN))
                {
                    ftms_var.ftms_cross_trainer_notify_flag.no_more_data = 1;
                }
                else
                {
                    ftms_var.ftms_cross_trainer_notify_flag.no_more_data = 0;
                }

                return;
            }
        }

        if (ftms_var.ftms_cross_trainer_notify_flag.ct_cur_flag & FTMS_CT_EXPEND_ENERGY_PRESENT_MASK)
        {
            if ((len - offset) >= FTMS_CT_EXPENDED_ENERGY_LEN)
            {
                memcpy(ftms_var.ftms_cross_trainer_send_data + offset,
                       ftms_var.ftms_cross_trainer_notify_data + total_offset, 5);
                ftms_var.ftms_cross_trainer_notify_flag.ct_send_flag |= FTMS_CT_EXPEND_ENERGY_PRESENT_MASK;
                offset += 5;
                total_offset += 5;
                ftms_var.ftms_cross_trainer_notify_flag.ct_cur_flag &= (~FTMS_CT_EXPEND_ENERGY_PRESENT_MASK);
            }
            else
            {
                ftms_var.ftms_cross_trainer_notify_flag.data_len = offset;
                ftms_var.ftms_cross_trainer_notify_flag.offset = total_offset;

                if ((data_length - offset + FTMS_CT_INSTANTANEOUS_SPEED_LEN) <= (len - FTMS_CT_DATA_FLAG_LEN))
                {
                    ftms_var.ftms_cross_trainer_notify_flag.no_more_data = 1;
                }
                else
                {
                    ftms_var.ftms_cross_trainer_notify_flag.no_more_data = 0;
                }

                return;
            }
        }

        if (ftms_var.ftms_cross_trainer_notify_flag.ct_cur_flag & FTMS_CT_HEART_RATE_PRESENT_MASK)
        {
            if ((len - offset) >= sizeof(uint8_t))
            {
                memcpy(ftms_var.ftms_cross_trainer_send_data + offset,
                       ftms_var.ftms_cross_trainer_notify_data + total_offset, 1);
                ftms_var.ftms_cross_trainer_notify_flag.ct_send_flag |= FTMS_CT_HEART_RATE_PRESENT_MASK;
                offset += 1;
                total_offset += 1;
                ftms_var.ftms_cross_trainer_notify_flag.ct_cur_flag &= (~FTMS_CT_HEART_RATE_PRESENT_MASK);
            }
            else
            {
                ftms_var.ftms_cross_trainer_notify_flag.data_len = offset;
                ftms_var.ftms_cross_trainer_notify_flag.offset = total_offset;

                if ((data_length - offset + FTMS_CT_INSTANTANEOUS_SPEED_LEN) <= (len - FTMS_CT_DATA_FLAG_LEN))
                {
                    ftms_var.ftms_cross_trainer_notify_flag.no_more_data = 1;
                }
                else
                {
                    ftms_var.ftms_cross_trainer_notify_flag.no_more_data = 0;
                }

                return;
            }
        }

        if (ftms_var.ftms_cross_trainer_notify_flag.ct_cur_flag & FTMS_CT_METABOLIC_EQUIVALENT_PRESENT_MASK)
        {
            if ((len - offset) >= sizeof(uint8_t))
            {
                memcpy(ftms_var.ftms_cross_trainer_send_data + offset,
                       ftms_var.ftms_cross_trainer_notify_data + total_offset, 1);
                ftms_var.ftms_cross_trainer_notify_flag.ct_send_flag |= FTMS_CT_METABOLIC_EQUIVALENT_PRESENT_MASK;
                offset += 1;
                total_offset += 1;
                ftms_var.ftms_cross_trainer_notify_flag.ct_cur_flag &= (~FTMS_CT_METABOLIC_EQUIVALENT_PRESENT_MASK);
            }
            else
            {
                ftms_var.ftms_cross_trainer_notify_flag.data_len = offset;
                ftms_var.ftms_cross_trainer_notify_flag.offset = total_offset;

                if ((data_length - offset + FTMS_CT_INSTANTANEOUS_SPEED_LEN) <= (len - FTMS_CT_DATA_FLAG_LEN))
                {
                    ftms_var.ftms_cross_trainer_notify_flag.no_more_data = 1;
                }
                else
                {
                    ftms_var.ftms_cross_trainer_notify_flag.no_more_data = 0;
                }

                return;
            }
        }

        if (ftms_var.ftms_cross_trainer_notify_flag.ct_cur_flag & FTMS_CT_ELAPSED_TIME_PRESENT_MASK)
        {
            if ((len - offset) >= sizeof(uint16_t))
            {
                memcpy(ftms_var.ftms_cross_trainer_send_data + offset,
                       ftms_var.ftms_cross_trainer_notify_data + total_offset, 2);
                ftms_var.ftms_cross_trainer_notify_flag.ct_send_flag |= FTMS_CT_ELAPSED_TIME_PRESENT_MASK;
                offset += 2;
                total_offset += 2;
                ftms_var.ftms_cross_trainer_notify_flag.ct_cur_flag &= (~FTMS_CT_ELAPSED_TIME_PRESENT_MASK);
            }
            else
            {
                ftms_var.ftms_cross_trainer_notify_flag.data_len = offset;
                ftms_var.ftms_cross_trainer_notify_flag.offset = total_offset;

                if ((data_length - offset + FTMS_CT_INSTANTANEOUS_SPEED_LEN) <= (len - FTMS_CT_DATA_FLAG_LEN))
                {
                    ftms_var.ftms_cross_trainer_notify_flag.no_more_data = 1;
                }
                else
                {
                    ftms_var.ftms_cross_trainer_notify_flag.no_more_data = 0;
                }

                return;
            }
        }

        if (ftms_var.ftms_cross_trainer_notify_flag.ct_cur_flag & FTMS_CT_REMAIN_TIME_PRESENT_MASK)
        {
            if ((len - offset) >= sizeof(uint16_t))
            {
                memcpy(ftms_var.ftms_cross_trainer_send_data + offset,
                       ftms_var.ftms_cross_trainer_notify_data + total_offset, 2);
                ftms_var.ftms_cross_trainer_notify_flag.ct_send_flag |= FTMS_CT_REMAIN_TIME_PRESENT_MASK;
                offset += 2;
                total_offset += 2;
                ftms_var.ftms_cross_trainer_notify_flag.ct_cur_flag &= (~FTMS_CT_REMAIN_TIME_PRESENT_MASK);
            }
            else
            {
                ftms_var.ftms_cross_trainer_notify_flag.data_len = offset;
                ftms_var.ftms_cross_trainer_notify_flag.offset = total_offset;

                if ((data_length - offset + FTMS_CT_INSTANTANEOUS_SPEED_LEN) <= (len - FTMS_CT_DATA_FLAG_LEN))
                {
                    ftms_var.ftms_cross_trainer_notify_flag.no_more_data = 1;
                }
                else
                {
                    ftms_var.ftms_cross_trainer_notify_flag.no_more_data = 0;
                }

                return;
            }
        }

        ftms_var.ftms_cross_trainer_notify_flag.data_len = offset;
        ftms_var.ftms_cross_trainer_notify_flag.offset = 0;
        ftms_var.ftms_cross_trainer_notify_flag.no_more_data = 0;

        if (ftms_var.ftms_cross_trainer_notify_data != NULL)
        {
            os_mem_free(ftms_var.ftms_cross_trainer_notify_data);
            ftms_var.ftms_cross_trainer_notify_data = NULL;
        }
    }
}

bool ftms_send_cross_trainer_data(uint8_t conn_id, T_SERVER_ID service_id, uint16_t data_length)
{
    bool ret = false;
    uint16_t mtu_size;
    le_get_conn_param(GAP_PARAM_CONN_MTU_SIZE, &mtu_size, conn_id);

    T_SRV_UUID_TBL *p_char;
    p_char = srv_find_index_by_uuid(ftms_var.ftms_srv_char_tbl, GATT_UUID_CROSS_TRAINER_DATA);
    uint16_t attrib_index = p_char->char_index;

    if (data_length <= (mtu_size - 5))
    {
        ftms_get_cross_trainer_data(conn_id, data_length);

        if (ftms_var.ftms_cross_trainer_send_data == NULL)
        {
            return ret;
        }

        uint8_t *p = ftms_var.ftms_cross_trainer_send_data;
        LE_UINT24_TO_STREAM(p, ftms_var.ftms_cross_trainer_notify_flag.ct_send_flag);

        if (server_send_data(conn_id, service_id, attrib_index, ftms_var.ftms_cross_trainer_send_data,
                             ftms_var.ftms_cross_trainer_notify_flag.data_len, GATT_PDU_TYPE_NOTIFICATION))
        {
            ret = true;
        }

        if (ftms_var.ftms_cross_trainer_send_data != NULL)
        {
            os_mem_free(ftms_var.ftms_cross_trainer_send_data);
            ftms_var.ftms_cross_trainer_send_data = NULL;
        }

        ftms_var.ftms_cross_trainer_notify_flag.ct_send_flag = 0;
        ftms_var.ftms_cross_trainer_notify_flag.data_len = 0;
    }
    else
    {
        ftms_var.ftms_cross_trainer_notify_flag.ct_cur_flag =
            ftms_var.cross_trainer_data_flag & FTMS_CT_NO_MORE_DATA_PRESENT;

        while (ftms_var.ftms_cross_trainer_notify_flag.ct_cur_flag & (~FTMS_CT_MOVEMENT_DIRECTION_MASK))
        {
            ftms_get_cross_trainer_data(conn_id, data_length);

            if (ftms_var.ftms_cross_trainer_send_data == NULL)
            {
                ret = false;
                return ret;
            }

            uint8_t *p = ftms_var.ftms_cross_trainer_send_data;
            LE_UINT24_TO_STREAM(p, ftms_var.ftms_cross_trainer_notify_flag.ct_send_flag);

            if (server_send_data(conn_id, service_id, attrib_index, ftms_var.ftms_cross_trainer_send_data,
                                 ftms_var.ftms_cross_trainer_notify_flag.data_len, GATT_PDU_TYPE_NOTIFICATION))
            {
                if (ftms_var.ftms_cross_trainer_send_data != NULL)
                {
                    os_mem_free(ftms_var.ftms_cross_trainer_send_data);
                    ftms_var.ftms_cross_trainer_send_data = NULL;
                }

                ftms_var.ftms_cross_trainer_notify_flag.ct_send_flag = 0;
                ftms_var.ftms_cross_trainer_notify_flag.data_len = 0;
                ret = true;
            }
            else
            {
                if (ftms_var.ftms_cross_trainer_send_data != NULL)
                {
                    os_mem_free(ftms_var.ftms_cross_trainer_send_data);
                    ftms_var.ftms_cross_trainer_send_data = NULL;
                }

                ftms_var.ftms_cross_trainer_notify_flag.ct_send_flag = 0;
                ftms_var.ftms_cross_trainer_notify_flag.data_len = 0;
                ret = false;
                return ret;
            }
        }
    }

    return ret;
}

bool ftms_cross_trainer_data_notify(uint8_t conn_id, T_SERVER_ID service_id,
                                    T_FTMS_CROSS_TRAINER_DATA *p_cross_trainer_data)
{
    bool ret = true;
    if (ftms_var.ftms_notify_indicate_flag.ftms_cross_trainer_data_notify_enable == 0)
    {
        ret = false;
        PROFILE_PRINT_INFO0("ftms_cross_trainer_data_notify:FTMS_APP_RESULT_CCCD_NOT_ENABLED");
        return ret;
    }

    uint16_t data_length = ftms_cross_trainer_data_length();
    ftms_save_cross_trainer_notify_data(conn_id, data_length, p_cross_trainer_data);

    return ftms_send_cross_trainer_data(conn_id, service_id, data_length);
}
#endif

#if FTMS_CHAR_STAIR_CLIMBER_DATA_SUPPORT
uint16_t ftms_stair_climber_data_length(void)
{
    uint16_t data_length = FTMS_STAIRC_FLOORS_LEN;
    if (ftms_var.stair_climber_data_flag & FTMS_STAIRC_STEP_PER_MINUTE_PRESENT_MASK)
    {
        data_length += sizeof(uint16_t);
    }
    if (ftms_var.stair_climber_data_flag & FTMS_STAIRC_AVE_STEP_RATE_PRESENT_MASK)
    {
        data_length += sizeof(uint16_t);
    }
    if (ftms_var.stair_climber_data_flag & FTMS_STAIRC_POSITIVE_ELEVATION_GAIN_PRESENT_MASK)
    {
        data_length += sizeof(uint16_t);
    }
    if (ftms_var.stair_climber_data_flag & FTMS_STAIRC_STRIDE_COUNT_PRESENT_MASK)
    {
        data_length += sizeof(uint16_t);
    }
    if (ftms_var.stair_climber_data_flag & FTMS_STAIRC_EXPEND_ENERGY_PRESENT_MASK)
    {
        data_length += FTMS_STAIRC_EXPENDED_ENERGY_LEN;
    }
    if (ftms_var.stair_climber_data_flag & FTMS_STAIRC_HEART_RATE_PRESENT_MASK)
    {
        data_length += sizeof(uint8_t);
    }
    if (ftms_var.stair_climber_data_flag & FTMS_STAIRC_METABOLIC_EQUIVALENT_PRESENT_MASK)
    {
        data_length += sizeof(uint8_t);
    }
    if (ftms_var.stair_climber_data_flag & FTMS_STAIRC_ELAPSED_TIME_PRESENT_MASK)
    {
        data_length += sizeof(uint16_t);
    }
    if (ftms_var.stair_climber_data_flag & FTMS_STAIRC_REMAIN_TIME_PRESENT_MASK)
    {
        data_length += sizeof(uint16_t);
    }

    return data_length;
}

void ftms_save_stair_climber_notify_data(uint8_t conn_id, uint16_t data_length,
                                         T_FTMS_STAIR_CLIMBER_DATA *p_stair_climber_data)
{
    ftms_var.ftms_stair_climber_notify_data = os_mem_zalloc(RAM_TYPE_DATA_ON, data_length);
    uint16_t offset = 0;
    uint8_t *p;
    p = ftms_var.ftms_stair_climber_notify_data + offset;
    LE_UINT16_TO_STREAM(p, p_stair_climber_data->floors);
    offset += 2;

    ftms_var.ftms_stair_climber_notify_flag.offset = FTMS_STAIRC_FLOORS_LEN;

    if (ftms_var.stair_climber_data_flag & FTMS_STAIRC_STEP_PER_MINUTE_PRESENT_MASK)
    {
        if (ftms_var.ftms_feature.features_field & FTMS_FEAT_STEP_COUNT_SPT_MASK)
        {
            p = ftms_var.ftms_stair_climber_notify_data + offset;
            LE_UINT16_TO_STREAM(p, p_stair_climber_data->step_per_minute);
        }
        else
        {
            memset(ftms_var.ftms_stair_climber_notify_data + offset, 0xFF, 2);
        }

        offset += 2;
    }

    if (ftms_var.stair_climber_data_flag & FTMS_STAIRC_AVE_STEP_RATE_PRESENT_MASK)
    {
        if (ftms_var.ftms_feature.features_field & FTMS_FEAT_STEP_COUNT_SPT_MASK)
        {
            p = ftms_var.ftms_stair_climber_notify_data + offset;
            LE_UINT16_TO_STREAM(p, p_stair_climber_data->average_step_rate);
        }
        else
        {
            memset(ftms_var.ftms_stair_climber_notify_data + offset, 0xFF, 2);
        }

        offset += 2;
    }

    if (ftms_var.stair_climber_data_flag & FTMS_STAIRC_POSITIVE_ELEVATION_GAIN_PRESENT_MASK)
    {
        if (ftms_var.ftms_feature.features_field & FTMS_FEAT_ELEVATION_GAIN_SPT_MASK)
        {
            p = ftms_var.ftms_stair_climber_notify_data + offset;
            LE_UINT16_TO_STREAM(p, p_stair_climber_data->positive_elevation_gain);
        }
        else
        {
            memset(ftms_var.ftms_stair_climber_notify_data + offset, 0xFF, 2);
        }

        offset += 2;
    }

    if (ftms_var.stair_climber_data_flag & FTMS_STAIRC_STRIDE_COUNT_PRESENT_MASK)
    {
        if (ftms_var.ftms_feature.features_field & FTMS_FEAT_STRIDE_COUNT_SPT_MASK)
        {
            p = ftms_var.ftms_stair_climber_notify_data + offset;
            LE_UINT16_TO_STREAM(p, p_stair_climber_data->stride_count);
        }
        else
        {
            memset(ftms_var.ftms_stair_climber_notify_data + offset, 0xFF, 2);
        }

        offset += 2;
    }

    if (ftms_var.stair_climber_data_flag & FTMS_STAIRC_EXPEND_ENERGY_PRESENT_MASK)
    {
        if (ftms_var.ftms_feature.features_field & FTMS_FEAT_EXPENDED_ENERGY_SPT_MASK)
        {
            p = ftms_var.ftms_stair_climber_notify_data + offset;
            LE_UINT16_TO_STREAM(p, p_stair_climber_data->total_energy);
            offset += 2;
            p = ftms_var.ftms_stair_climber_notify_data + offset;
            LE_UINT16_TO_STREAM(p, p_stair_climber_data->energy_per_hour);
            offset += 2;
            p = ftms_var.ftms_stair_climber_notify_data + offset;
            LE_UINT8_TO_STREAM(p, p_stair_climber_data->energy_per_minute);
            offset += 1;
        }
        else
        {
            memset(ftms_var.ftms_stair_climber_notify_data + offset, 0xFF, 5);
            offset += 5;
        }
    }

    if (ftms_var.stair_climber_data_flag & FTMS_STAIRC_HEART_RATE_PRESENT_MASK)
    {
        if (ftms_var.ftms_feature.features_field & FTMS_FEAT_HR_MEASUREMENT_SPT_MASK)
        {
            ftms_var.ftms_stair_climber_notify_data[offset] = p_stair_climber_data->heart_rate;
        }
        else
        {
            memset(ftms_var.ftms_stair_climber_notify_data + offset, 0xFF, 1);
        }

        offset += 1;
    }

    if (ftms_var.stair_climber_data_flag & FTMS_STAIRC_METABOLIC_EQUIVALENT_PRESENT_MASK)
    {
        if (ftms_var.ftms_feature.features_field & FTMS_FEAT_METABOLIC_EQUIVALENT_SPT_MASK)
        {
            ftms_var.ftms_stair_climber_notify_data[offset] =
                p_stair_climber_data->metabolic_equivalent;
        }
        else
        {
            memset(ftms_var.ftms_stair_climber_notify_data + offset, 0xFF, 1);
        }

        offset += 1;
    }

    if (ftms_var.stair_climber_data_flag & FTMS_STAIRC_ELAPSED_TIME_PRESENT_MASK)
    {
        if (ftms_var.ftms_feature.features_field & FTMS_FEAT_ELAPSED_TIME_SPT_MASK)
        {
            p = ftms_var.ftms_stair_climber_notify_data + offset;
            LE_UINT16_TO_STREAM(p, p_stair_climber_data->elapsed_time);
        }
        else
        {
            memset(ftms_var.ftms_stair_climber_notify_data + offset, 0xFF, 2);
        }

        offset += 2;
    }

    if (ftms_var.stair_climber_data_flag & FTMS_STAIRC_REMAIN_TIME_PRESENT_MASK)
    {
        if (ftms_var.ftms_feature.features_field & FTMS_FEAT_REMAIN_TIME_SPT_MASK)
        {
            p = ftms_var.ftms_stair_climber_notify_data + offset;
            LE_UINT16_TO_STREAM(p, p_stair_climber_data->remain_time);
        }
        else
        {
            memset(ftms_var.ftms_stair_climber_notify_data + offset, 0xFF, 2);
        }
    }
}

void ftms_get_stair_climber_data(uint8_t conn_id, uint16_t data_length)
{
    uint16_t mtu_size;
    le_get_conn_param(GAP_PARAM_CONN_MTU_SIZE, &mtu_size, conn_id);

    uint16_t len = 0;
    uint16_t offset = 0;

    if (data_length <= (mtu_size - 5))
    {
        len = data_length + FTMS_STAIRC_DATA_FLAG_LEN;
        ftms_var.ftms_stair_climber_send_data = os_mem_zalloc(RAM_TYPE_DATA_ON, len);

        if (ftms_var.ftms_stair_climber_send_data == NULL)
        {
            PROFILE_PRINT_ERROR0("ftms_get_stair_climber_data: get data fail");
            return;
        }

        ftms_var.stair_climber_data_flag &= FTMS_STAIRC_NO_MORE_DATA_PRESENT;
        ftms_var.ftms_stair_climber_notify_flag.send_flag = ftms_var.stair_climber_data_flag;
        offset += FTMS_STAIRC_DATA_FLAG_LEN;
        memcpy(ftms_var.ftms_stair_climber_send_data + offset, ftms_var.ftms_stair_climber_notify_data,
               data_length);
        ftms_var.ftms_stair_climber_notify_flag.data_len = len;

        if (ftms_var.ftms_stair_climber_notify_data != NULL)
        {
            os_mem_free(ftms_var.ftms_stair_climber_notify_data);
            ftms_var.ftms_stair_climber_notify_data = NULL;
        }
    }
    else
    {
        uint16_t total_offset = 0;
        len = mtu_size - 3;

        ftms_var.ftms_stair_climber_send_data = os_mem_zalloc(RAM_TYPE_DATA_ON, len);

        if (ftms_var.ftms_stair_climber_send_data == NULL)
        {
            PROFILE_PRINT_ERROR0("ftms_get_stair_climber_data: get data fail");
            return;
        }

        total_offset = ftms_var.ftms_stair_climber_notify_flag.offset;

        if (ftms_var.ftms_stair_climber_notify_flag.no_more_data == 0)
        {
            ftms_var.ftms_stair_climber_notify_flag.send_flag |= FTMS_STAIRC_MORE_DATA_MASK;
            offset += FTMS_STAIRC_DATA_FLAG_LEN;
        }
        else
        {
            ftms_var.ftms_stair_climber_notify_flag.send_flag &= FTMS_STAIRC_NO_MORE_DATA_PRESENT;
            offset += FTMS_STAIRC_DATA_FLAG_LEN;
            memcpy(ftms_var.ftms_stair_climber_send_data + offset,
                   ftms_var.ftms_stair_climber_notify_data, 2);
            offset += FTMS_STAIRC_FLOORS_LEN;
        }

        if (ftms_var.ftms_stair_climber_notify_flag.cur_flag & FTMS_STAIRC_STEP_PER_MINUTE_PRESENT_MASK)
        {
            if ((len - offset) >= sizeof(uint16_t))
            {
                memcpy(ftms_var.ftms_stair_climber_send_data + offset,
                       ftms_var.ftms_stair_climber_notify_data + total_offset, 2);
                ftms_var.ftms_stair_climber_notify_flag.send_flag |= FTMS_STAIRC_STEP_PER_MINUTE_PRESENT_MASK;
                offset += 2;
                total_offset += 2;
                ftms_var.ftms_stair_climber_notify_flag.cur_flag &= (~FTMS_STAIRC_STEP_PER_MINUTE_PRESENT_MASK);
            }
            else
            {
                ftms_var.ftms_stair_climber_notify_flag.data_len = offset;
                ftms_var.ftms_stair_climber_notify_flag.offset = total_offset;

                if ((data_length - offset + FTMS_STAIRC_FLOORS_LEN) <= (len - FTMS_STAIRC_DATA_FLAG_LEN))
                {
                    ftms_var.ftms_stair_climber_notify_flag.no_more_data = 1;
                }
                else
                {
                    ftms_var.ftms_stair_climber_notify_flag.no_more_data = 0;
                }

                return;
            }
        }

        if (ftms_var.ftms_stair_climber_notify_flag.cur_flag & FTMS_STAIRC_AVE_STEP_RATE_PRESENT_MASK)
        {
            if ((len - offset) >= sizeof(uint16_t))
            {
                memcpy(ftms_var.ftms_stair_climber_send_data + offset,
                       ftms_var.ftms_stair_climber_notify_data + total_offset, 2);
                ftms_var.ftms_stair_climber_notify_flag.send_flag |= FTMS_STAIRC_AVE_STEP_RATE_PRESENT_MASK;
                offset += 2;
                total_offset += 2;
                ftms_var.ftms_stair_climber_notify_flag.cur_flag &= (~FTMS_STAIRC_AVE_STEP_RATE_PRESENT_MASK);
            }
            else
            {
                ftms_var.ftms_stair_climber_notify_flag.data_len = offset;
                ftms_var.ftms_stair_climber_notify_flag.offset = total_offset;

                if ((data_length - offset + FTMS_STAIRC_FLOORS_LEN) <= (len - FTMS_STAIRC_DATA_FLAG_LEN))
                {
                    ftms_var.ftms_stair_climber_notify_flag.no_more_data = 1;
                }
                else
                {
                    ftms_var.ftms_stair_climber_notify_flag.no_more_data = 0;
                }

                return;
            }
        }

        if (ftms_var.ftms_stair_climber_notify_flag.cur_flag &
            FTMS_STAIRC_POSITIVE_ELEVATION_GAIN_PRESENT_MASK)
        {
            if ((len - offset) >= sizeof(uint16_t))
            {
                memcpy(ftms_var.ftms_stair_climber_send_data + offset,
                       ftms_var.ftms_stair_climber_notify_data + total_offset, 2);
                ftms_var.ftms_stair_climber_notify_flag.send_flag |=
                    FTMS_STAIRC_POSITIVE_ELEVATION_GAIN_PRESENT_MASK;
                offset += 2;
                total_offset += 2;
                ftms_var.ftms_stair_climber_notify_flag.cur_flag &=
                    (~FTMS_STAIRC_POSITIVE_ELEVATION_GAIN_PRESENT_MASK);
            }
            else
            {
                ftms_var.ftms_stair_climber_notify_flag.data_len = offset;
                ftms_var.ftms_stair_climber_notify_flag.offset = total_offset;

                if ((data_length - offset + FTMS_STAIRC_FLOORS_LEN) <= (len - FTMS_STAIRC_DATA_FLAG_LEN))
                {
                    ftms_var.ftms_stair_climber_notify_flag.no_more_data = 1;
                }
                else
                {
                    ftms_var.ftms_stair_climber_notify_flag.no_more_data = 0;
                }

                return;
            }
        }

        if (ftms_var.ftms_stair_climber_notify_flag.cur_flag & FTMS_STAIRC_STRIDE_COUNT_PRESENT_MASK)
        {
            if ((len - offset) >= sizeof(uint16_t))
            {
                memcpy(ftms_var.ftms_stair_climber_send_data + offset,
                       ftms_var.ftms_stair_climber_notify_data + total_offset, 2);
                ftms_var.ftms_stair_climber_notify_flag.send_flag |= FTMS_STAIRC_STRIDE_COUNT_PRESENT_MASK;
                offset += 2;
                total_offset += 2;
                ftms_var.ftms_stair_climber_notify_flag.cur_flag &= (~FTMS_STAIRC_STRIDE_COUNT_PRESENT_MASK);
            }
            else
            {
                ftms_var.ftms_stair_climber_notify_flag.data_len = offset;
                ftms_var.ftms_stair_climber_notify_flag.offset = total_offset;

                if ((data_length - offset + FTMS_STAIRC_FLOORS_LEN) <= (len - FTMS_STAIRC_DATA_FLAG_LEN))
                {
                    ftms_var.ftms_stair_climber_notify_flag.no_more_data = 1;
                }
                else
                {
                    ftms_var.ftms_stair_climber_notify_flag.no_more_data = 0;
                }

                return;
            }
        }

        if (ftms_var.ftms_stair_climber_notify_flag.cur_flag & FTMS_STAIRC_EXPEND_ENERGY_PRESENT_MASK)
        {
            if ((len - offset) >= FTMS_STAIRC_EXPENDED_ENERGY_LEN)
            {
                memcpy(ftms_var.ftms_stair_climber_send_data + offset,
                       ftms_var.ftms_stair_climber_notify_data + total_offset, 5);
                ftms_var.ftms_stair_climber_notify_flag.send_flag |= FTMS_STAIRC_EXPEND_ENERGY_PRESENT_MASK;
                offset += 5;
                total_offset += 5;
                ftms_var.ftms_stair_climber_notify_flag.cur_flag &= (~FTMS_STAIRC_EXPEND_ENERGY_PRESENT_MASK);
            }
            else
            {
                ftms_var.ftms_stair_climber_notify_flag.data_len = offset;
                ftms_var.ftms_stair_climber_notify_flag.offset = total_offset;

                if ((data_length - offset + FTMS_STAIRC_FLOORS_LEN) <= (len - FTMS_STAIRC_DATA_FLAG_LEN))
                {
                    ftms_var.ftms_stair_climber_notify_flag.no_more_data = 1;
                }
                else
                {
                    ftms_var.ftms_stair_climber_notify_flag.no_more_data = 0;
                }

                return;
            }
        }

        if (ftms_var.ftms_stair_climber_notify_flag.cur_flag & FTMS_STAIRC_HEART_RATE_PRESENT_MASK)
        {
            if ((len - offset) >= sizeof(uint8_t))
            {
                memcpy(ftms_var.ftms_stair_climber_send_data + offset,
                       ftms_var.ftms_stair_climber_notify_data + total_offset, 1);
                ftms_var.ftms_stair_climber_notify_flag.send_flag |= FTMS_STAIRC_HEART_RATE_PRESENT_MASK;
                offset += 1;
                total_offset += 1;
                ftms_var.ftms_stair_climber_notify_flag.cur_flag &= (~FTMS_STAIRC_HEART_RATE_PRESENT_MASK);
            }
            else
            {
                ftms_var.ftms_stair_climber_notify_flag.data_len = offset;
                ftms_var.ftms_stair_climber_notify_flag.offset = total_offset;

                if ((data_length - offset + FTMS_STAIRC_FLOORS_LEN) <= (len - FTMS_STAIRC_DATA_FLAG_LEN))
                {
                    ftms_var.ftms_stair_climber_notify_flag.no_more_data = 1;
                }
                else
                {
                    ftms_var.ftms_stair_climber_notify_flag.no_more_data = 0;
                }

                return;
            }
        }

        if (ftms_var.ftms_stair_climber_notify_flag.cur_flag &
            FTMS_STAIRC_METABOLIC_EQUIVALENT_PRESENT_MASK)
        {
            if ((len - offset) >= sizeof(uint8_t))
            {
                memcpy(ftms_var.ftms_stair_climber_send_data + offset,
                       ftms_var.ftms_stair_climber_notify_data + total_offset, 1);
                ftms_var.ftms_stair_climber_notify_flag.send_flag |= FTMS_STAIRC_METABOLIC_EQUIVALENT_PRESENT_MASK;
                offset += 1;
                total_offset += 1;
                ftms_var.ftms_stair_climber_notify_flag.cur_flag &=
                    (~FTMS_STAIRC_METABOLIC_EQUIVALENT_PRESENT_MASK);
            }
            else
            {
                ftms_var.ftms_stair_climber_notify_flag.data_len = offset;
                ftms_var.ftms_stair_climber_notify_flag.offset = total_offset;

                if ((data_length - offset + FTMS_STAIRC_FLOORS_LEN) <= (len - FTMS_STAIRC_DATA_FLAG_LEN))
                {
                    ftms_var.ftms_stair_climber_notify_flag.no_more_data = 1;
                }
                else
                {
                    ftms_var.ftms_stair_climber_notify_flag.no_more_data = 0;
                }

                return;
            }
        }

        if (ftms_var.ftms_stair_climber_notify_flag.cur_flag & FTMS_STAIRC_ELAPSED_TIME_PRESENT_MASK)
        {
            if ((len - offset) >= sizeof(uint16_t))
            {
                memcpy(ftms_var.ftms_stair_climber_send_data + offset,
                       ftms_var.ftms_stair_climber_notify_data + total_offset, 2);
                ftms_var.ftms_stair_climber_notify_flag.send_flag |= FTMS_STAIRC_ELAPSED_TIME_PRESENT_MASK;
                offset += 2;
                total_offset += 2;
                ftms_var.ftms_stair_climber_notify_flag.cur_flag &= (~FTMS_STAIRC_ELAPSED_TIME_PRESENT_MASK);
            }
            else
            {
                ftms_var.ftms_stair_climber_notify_flag.data_len = offset;
                ftms_var.ftms_stair_climber_notify_flag.offset = total_offset;

                if ((data_length - offset + FTMS_STAIRC_FLOORS_LEN) <= (len - FTMS_STAIRC_DATA_FLAG_LEN))
                {
                    ftms_var.ftms_stair_climber_notify_flag.no_more_data = 1;
                }
                else
                {
                    ftms_var.ftms_stair_climber_notify_flag.no_more_data = 0;
                }

                return;
            }
        }

        if (ftms_var.ftms_stair_climber_notify_flag.cur_flag & FTMS_STAIRC_REMAIN_TIME_PRESENT_MASK)
        {
            if ((len - offset) >= sizeof(uint16_t))
            {
                memcpy(ftms_var.ftms_stair_climber_send_data + offset,
                       ftms_var.ftms_stair_climber_notify_data + total_offset, 2);
                ftms_var.ftms_stair_climber_notify_flag.send_flag |= FTMS_STAIRC_REMAIN_TIME_PRESENT_MASK;
                offset += 2;
                total_offset += 2;
                ftms_var.ftms_stair_climber_notify_flag.cur_flag &= (~FTMS_STAIRC_REMAIN_TIME_PRESENT_MASK);
            }
            else
            {
                ftms_var.ftms_stair_climber_notify_flag.data_len = offset;
                ftms_var.ftms_stair_climber_notify_flag.offset = total_offset;

                if ((data_length - offset + FTMS_STAIRC_FLOORS_LEN) <= (len - FTMS_STAIRC_DATA_FLAG_LEN))
                {
                    ftms_var.ftms_stair_climber_notify_flag.no_more_data = 1;
                }
                else
                {
                    ftms_var.ftms_stair_climber_notify_flag.no_more_data = 0;
                }

                return;
            }
        }

        ftms_var.ftms_stair_climber_notify_flag.data_len = offset;
        ftms_var.ftms_stair_climber_notify_flag.offset = 0;
        ftms_var.ftms_stair_climber_notify_flag.no_more_data = 0;

        if (ftms_var.ftms_stair_climber_notify_data != NULL)
        {
            os_mem_free(ftms_var.ftms_stair_climber_notify_data);
            ftms_var.ftms_stair_climber_notify_data = NULL;
        }
    }
}

bool ftms_send_stair_climber_data(uint8_t conn_id, T_SERVER_ID service_id, uint16_t data_length)
{
    bool ret = false;
    uint16_t mtu_size;
    le_get_conn_param(GAP_PARAM_CONN_MTU_SIZE, &mtu_size, conn_id);

    T_SRV_UUID_TBL *p_char;
    p_char = srv_find_index_by_uuid(ftms_var.ftms_srv_char_tbl, GATT_UUID_STAIR_CLIMBER_DATA);
    uint16_t attrib_index = p_char->char_index;

    if (data_length <= (mtu_size - 5))
    {
        ftms_get_stair_climber_data(conn_id, data_length);

        if (ftms_var.ftms_stair_climber_send_data == NULL)
        {
            return ret;
        }

        uint8_t *p = ftms_var.ftms_stair_climber_send_data;
        LE_UINT16_TO_STREAM(p, ftms_var.ftms_stair_climber_notify_flag.send_flag);

        if (server_send_data(conn_id, service_id, attrib_index, ftms_var.ftms_stair_climber_send_data,
                             ftms_var.ftms_stair_climber_notify_flag.data_len, GATT_PDU_TYPE_NOTIFICATION))
        {
            ret = true;
        }

        if (ftms_var.ftms_stair_climber_send_data != NULL)
        {
            os_mem_free(ftms_var.ftms_stair_climber_send_data);
            ftms_var.ftms_stair_climber_send_data = NULL;
        }

        ftms_var.ftms_stair_climber_notify_flag.send_flag = 0;
        ftms_var.ftms_stair_climber_notify_flag.data_len = 0;
    }
    else
    {
        ftms_var.ftms_stair_climber_notify_flag.cur_flag =
            ftms_var.stair_climber_data_flag & FTMS_STAIRC_NO_MORE_DATA_PRESENT;

        while (ftms_var.ftms_stair_climber_notify_flag.cur_flag)
        {
            ftms_get_stair_climber_data(conn_id, data_length);

            if (ftms_var.ftms_stair_climber_send_data == NULL)
            {
                ret = false;
                return ret;
            }

            uint8_t *p = ftms_var.ftms_stair_climber_send_data;
            LE_UINT16_TO_STREAM(p, ftms_var.ftms_stair_climber_notify_flag.send_flag);

            if (server_send_data(conn_id, service_id, attrib_index, ftms_var.ftms_stair_climber_send_data,
                                 ftms_var.ftms_stair_climber_notify_flag.data_len, GATT_PDU_TYPE_NOTIFICATION))
            {
                if (ftms_var.ftms_stair_climber_send_data != NULL)
                {
                    os_mem_free(ftms_var.ftms_stair_climber_send_data);
                    ftms_var.ftms_stair_climber_send_data = NULL;
                }

                ftms_var.ftms_stair_climber_notify_flag.send_flag = 0;
                ftms_var.ftms_stair_climber_notify_flag.data_len = 0;
                ret = true;
            }
            else
            {
                if (ftms_var.ftms_stair_climber_send_data != NULL)
                {
                    os_mem_free(ftms_var.ftms_stair_climber_send_data);
                    ftms_var.ftms_stair_climber_send_data = NULL;
                }

                ftms_var.ftms_stair_climber_notify_flag.send_flag = 0;
                ftms_var.ftms_stair_climber_notify_flag.data_len = 0;
                ret = false;
                return ret;
            }
        }
    }

    return ret;
}

bool ftms_stair_climber_data_notify(uint8_t conn_id, T_SERVER_ID service_id,
                                    T_FTMS_STAIR_CLIMBER_DATA *p_stair_climber_data)
{
    bool ret = true;
    if (ftms_var.ftms_notify_indicate_flag.ftms_stair_climber_data_notify_enable == 0)
    {
        ret = false;
        PROFILE_PRINT_INFO0("ftms_stair_climber_data_notify:FTMS_APP_RESULT_CCCD_NOT_ENABLED");
        return ret;
    }

    uint16_t data_length = ftms_stair_climber_data_length();
    ftms_save_stair_climber_notify_data(conn_id, data_length, p_stair_climber_data);

    return ftms_send_stair_climber_data(conn_id, service_id, data_length);
}
#endif

#if (FTMS_CHAR_FTMS_CTRL_PNT_SUPPORT || FTMS_CHAR_FTMS_STATUS_SUPPORT)
bool ftms_status_notify(uint8_t conn_id, T_SERVER_ID service_id, uint8_t opcode,
                        T_FTMS_CP_PARAMETER param, uint8_t param_len)
{
    bool ret = true;

    if (ftms_var.ftms_notify_indicate_flag.ftms_status_notify_enable == 0)
    {
        ret = false;
        PROFILE_PRINT_INFO0("ftms_status_notify: FTMS_APP_RESULT_CCCD_NOT_ENABLED");
        return ret;
    }

    T_SRV_UUID_TBL *p_char;
    p_char = srv_find_index_by_uuid(ftms_var.ftms_srv_char_tbl, GATT_UUID_FITNESS_MACHINE_STATUS);
    uint16_t attrib_index = p_char->char_index;
    uint16_t data_length = param_len + 1;
    uint8_t status_data[param_len + 1];
    status_data[0] = opcode;
    uint8_t offset = 1;

    switch (opcode)
    {
    case FTMS_STATUS_OP_RESET:
        {}
        break;

    case FTMS_STATUS_OP_STOP_PAUSE_BY_USER:
        {
            status_data[offset] = param.ctrl_information;
        }
        break;

    case FTMS_STATUS_OP_STOP_BY_SAFETY_KEY:
        {}
        break;

    case FTMS_STATUS_OP_START_RESUME_BY_USER:
        {}
        break;

    case FTMS_STATUS_OP_TGT_SPEED_CHANGE:
        {
            LE_UINT16_TO_ARRAY(&status_data[offset], param.target_speed);
        }
        break;

    case FTMS_STATUS_OP_TGT_INCLINE_CHANGE:
        {
            LE_UINT16_TO_ARRAY(&status_data[offset], param.target_inclination);
        }
        break;

    case FTMS_STATUS_OP_TGT_RESISTANCE_LEVEL_CHANGE:
        {
            LE_UINT16_TO_ARRAY(&status_data[offset], param.target_resistance_level);
        }
        break;

    case FTMS_STATUS_OP_TGT_POWER_CHANGE:
        {
            LE_UINT16_TO_ARRAY(&status_data[offset], param.target_power);
        }
        break;

    case FTMS_STATUS_OP_TGT_HR_CHANGE:
        {
            status_data[offset] = param.target_heart_rate;
        }
        break;

    case FTMS_STATUS_OP_TGT_EXPEND_ENERGY_CHANGE:
        {
            LE_UINT16_TO_ARRAY(&status_data[offset], param.target_expended_energy);
        }
        break;

    case FTMS_STATUS_OP_TGT_STEP_NUM_CHANGE:
        {
            LE_UINT16_TO_ARRAY(&status_data[offset], param.target_step_num);
        }
        break;

    case FTMS_STATUS_OP_TGT_STRIDE_NUM_CHANGE:
        {
            LE_UINT16_TO_ARRAY(&status_data[offset], param.target_stride_num);
        }
        break;

    case FTMS_STATUS_OP_TGT_DISTANCE_CHANGE:
        {
            LE_UINT24_TO_ARRAY(&status_data[offset], param.target_distance);
        }
        break;

    case FTMS_STATUS_OP_TGT_TRAIN_TIME_CHANGE:
        {
            LE_UINT16_TO_ARRAY(&status_data[offset], param.target_training_time);
        }
        break;

    case FTMS_STATUS_OP_TWO_HR_ZONE_TIME_CHANGE:
        {
            LE_UINT16_TO_ARRAY(&status_data[offset],
                               param.target_two_zones_hr_time.fat_burn_zone_time);
            offset += 2;
            LE_UINT16_TO_ARRAY(&status_data[offset],
                               param.target_two_zones_hr_time.fat_burn_zone_time);
        }
        break;

    case FTMS_STATUS_OP_THREE_HR_ZONE_TIME_CHANGE:
        {
            LE_UINT16_TO_ARRAY(&status_data[offset],
                               param.target_three_zones_hr_time.light_zone_time);
            offset += 2;
            LE_UINT16_TO_ARRAY(&status_data[offset],
                               param.target_three_zones_hr_time.moderate_zone_time);
            offset += 2;
            LE_UINT16_TO_ARRAY(&status_data[offset],
                               param.target_three_zones_hr_time.hard_zone_time);
        }
        break;

    case FTMS_STATUS_OP_FIVE_HR_ZONE_TIME_CHANGE:
        {
            LE_UINT16_TO_ARRAY(&status_data[offset],
                               param.target_five_zones_hr_time.very_light_zone_time);
            offset += 2;
            LE_UINT16_TO_ARRAY(&status_data[offset],
                               param.target_five_zones_hr_time.light_zone_time);
            offset += 2;
            LE_UINT16_TO_ARRAY(&status_data[offset],
                               param.target_five_zones_hr_time.moderate_zone_time);
            offset += 2;
            LE_UINT16_TO_ARRAY(&status_data[offset],
                               param.target_five_zones_hr_time.hard_zone_time);
            offset += 2;
            LE_UINT16_TO_ARRAY(&status_data[offset],
                               param.target_five_zones_hr_time.maximum_zone_time);
        }
        break;

    case FTMS_STATUS_OP_IB_SIMULATE_PARAM_CHANGE:
        {
            LE_UINT16_TO_ARRAY(&status_data[offset],
                               param.set_indoor_bike_simulation_param.wind_speed);
            offset += 2;
            LE_UINT16_TO_ARRAY(&status_data[offset],
                               param.set_indoor_bike_simulation_param.grade);
            offset += 2;
            LE_UINT8_TO_ARRAY(&status_data[offset],
                              param.set_indoor_bike_simulation_param.rolling_resistance_coefficient);
            offset += 1;
            LE_UINT8_TO_ARRAY(&status_data[offset],
                              param.set_indoor_bike_simulation_param.wind_resistance_coefficient);
        }
        break;

    case FTMS_STATUS_OP_WHEEL_CIRCUMFERENCE_CHANGE:
        {
            LE_UINT16_TO_ARRAY(&status_data[offset], param.wheel_circumference);
        }
        break;

    case FTMS_STATUS_OP_SPIN_DOWN_STATUS:
        {
            status_data[offset] = param.spin_down_status_value;
        }
        break;

    case FTMS_STATUS_OP_TGT_CADENCE_CHANGE:
        {
            LE_UINT16_TO_ARRAY(&status_data[offset], param.target_cadence);
        }
        break;

    case FTMS_STATUS_OP_CONTROL_PERMISSION_LOST:
        {
#if FTMS_CHAR_FTMS_CTRL_PNT_SUPPORT
            ftms_var.control_permission = false;
#endif
        }
        break;

    default:
        {
            ret = false;
            PROFILE_PRINT_ERROR1("ftms_status_notify:invalid opcode 0x%x", opcode);
        }
        break;
    }

    if (ret)
    {
        ret = server_send_data(conn_id, service_id, attrib_index, status_data, data_length,
                               GATT_PDU_TYPE_NOTIFICATION);
    }


    return ret;
}
#endif

#if FTMS_CHAR_FTMS_CTRL_PNT_SUPPORT
static void ftms_ctl_pnt_display_rsp(T_FTMS_CONTROL_POINT *ftms_ctl_pnt_ptr)
{
    PROFILE_PRINT_INFO1("ftms cp response: req_op_code=0x%x", ftms_ctl_pnt_ptr->param[1]);
    PROFILE_PRINT_INFO1("ftms rsp_code = 0x%x", ftms_ctl_pnt_ptr->param[2]);
}

bool ftms_ctl_pnt_indication(uint8_t conn_id, T_SERVER_ID service_id, uint8_t opcode,
                             uint8_t rsp_code)
{
    bool ret = true;

    if (ftms_var.ftms_notify_indicate_flag.ftms_cp_indicate_enable == 0)
    {
        ret = false;
        ftms_var.ftms_control_point.param[0] = FTMS_CP_OPCODE_RESERVED;
        PROFILE_PRINT_ERROR0("ftms_ctl_pnt_indication: FTMS_APP_RESULT_CCCD_NOT_ENABLED");
        return ret;
    }

    T_SRV_UUID_TBL *p_char;
    p_char = srv_find_index_by_uuid(ftms_var.ftms_srv_char_tbl, GATT_UUID_FTMS_CONTROL_POINT);
    uint16_t attrib_index = p_char->char_index;

    ftms_var.ftms_control_point.param[1] = opcode;
    ftms_var.ftms_control_point.param[2] = rsp_code;
    ftms_var.ftms_control_point.param[0] = FTMS_CP_OPCODE_RESPONSE_CODE;

    if (opcode == FTMS_CP_OPCODE_SPIN_DOWN_CTRL)
    {
#if FTMS_CHAR_CP_SPIN_DOWN_CTRL_SUPPORT
        uint8_t offset = 3;
        ftms_var.ftms_control_point.cur_length = 3 * sizeof(uint8_t) +
                                                 sizeof(T_FTMS_SPIN_DOWN_RESP_PARAM);
        LE_UINT16_TO_ARRAY(&ftms_var.ftms_control_point.param[offset],
                           ftms_var.ftms_sd_rsp_param.target_speed_low);
        offset += 2;
        LE_UINT16_TO_ARRAY(&ftms_var.ftms_control_point.param[offset],
                           ftms_var.ftms_sd_rsp_param.target_speed_high);
#endif
    }
    else
    {
        ftms_var.ftms_control_point.cur_length = 3 * sizeof(uint8_t);
    }

    ftms_ctl_pnt_display_rsp(&ftms_var.ftms_control_point);

    if (server_send_data(conn_id, service_id, attrib_index, ftms_var.ftms_control_point.param,
                         ftms_var.ftms_control_point.cur_length, GATT_PDU_TYPE_INDICATION))
    {
        PROFILE_PRINT_INFO0("ftms_ctl_pnt_indication:server_send_data and set status doing success");
    }
    else
    {
        ret = false;
        PROFILE_PRINT_ERROR0("ftms_ctl_pnt_indication:server_send_data fail");
    }

    ftms_var.ftms_control_point.param[0] = FTMS_CP_OPCODE_RESERVED;
    return ret;
}

static void ftms_ctl_pnt_request_control(uint8_t conn_id, T_SERVER_ID service_id, uint8_t rsp_code)
{
    uint8_t op_code = FTMS_CP_OPCODE_REQUEST_CONTROL;
    ftms_var.control_permission = true;
    ftms_ctl_pnt_indication(conn_id, service_id, op_code, rsp_code);
}

static void ftms_ctl_pnt_reset(uint8_t conn_id, T_SERVER_ID service_id, uint8_t rsp_code)
{
    uint8_t op_code = FTMS_CP_OPCODE_RESET;
    ftms_var.ftms_cur_status = 0;
    ftms_var.control_permission = false;
    ftms_ctl_pnt_indication(conn_id, service_id, op_code, rsp_code);
}

static void ftms_ctl_pnt_start_resume(uint8_t conn_id, T_SERVER_ID service_id, uint8_t rsp_code)
{
    uint8_t op_code = FTMS_CP_OPCODE_START_RESUME;
    if (ftms_var.ftms_cur_status == FTMS_STATUS_START_RESUME)
    {
        rsp_code = FTMS_CP_RSPCODE_OPERATION_FAILED;
    }
    else
    {
        ftms_var.ftms_cur_status = FTMS_STATUS_START_RESUME;
    }
    ftms_ctl_pnt_indication(conn_id, service_id, op_code, rsp_code);
}

static void ftms_ctl_pnt_stop_pause(uint8_t conn_id, T_SERVER_ID service_id, uint8_t rsp_code)
{
    uint8_t op_code = FTMS_CP_OPCODE_STOP_PAUSE;
    if (ftms_var.ftms_cur_status == FTMS_STATUS_STOP_PAUSE)
    {
        rsp_code = FTMS_CP_RSPCODE_OPERATION_FAILED;
    }
    else
    {
        ftms_var.ftms_cur_status = FTMS_STATUS_STOP_PAUSE;
    }
    ftms_ctl_pnt_indication(conn_id, service_id, op_code, rsp_code);
}

#if FTMS_CHAR_CP_SPIN_DOWN_CTRL_SUPPORT
static void ftms_ctl_pnt_spin_down_ctrl(uint8_t conn_id, T_SERVER_ID service_id, uint8_t rsp_code)
{
    uint8_t op_code = FTMS_CP_OPCODE_SPIN_DOWN_CTRL;
    ftms_ctl_pnt_indication(conn_id, service_id, op_code, rsp_code);
}
#endif

static void ftms_ctl_pnt_write_target_data(uint8_t conn_id, T_SERVER_ID service_id, uint8_t op_code,
                                           uint8_t rsp_code)
{
    ftms_ctl_pnt_indication(conn_id, service_id, op_code, rsp_code);
}

static uint8_t ftms_handle_ctl_pnt_proc(uint8_t conn_id, T_SERVER_ID service_id,
                                        uint16_t attrib_index,
                                        uint16_t write_length, uint8_t *p_value)
{
    T_FTMS_CALLBACK_DATA callback_data;
    T_SRV_UUID_TBL *p_char;
    p_char = srv_find_uuid_by_index(ftms_var.ftms_srv_char_tbl, attrib_index);
    uint8_t resp_code = FTMS_CP_RSPCODE_SUCCESS;
    uint16_t parameter_length = 0;
    uint8_t offset = 1;

    memcpy(ftms_var.ftms_control_point.param, p_value, write_length);
    callback_data.conn_id = conn_id;
    callback_data.char_uuid = p_char->char_uuid16;
    callback_data.msg_type = SERVICE_CALLBACK_TYPE_WRITE_CHAR_VALUE;
    callback_data.msg_data.write.opcode = ftms_var.ftms_control_point.param[0];

    if (write_length >= 1)
    {
        parameter_length = write_length - 1;
    }

    if (ftms_var.ftms_control_point.param[0] == FTMS_CP_OPCODE_REQUEST_CONTROL)
    {
        if (parameter_length == 0)
        {
        }
        else
        {
            resp_code = FTMS_CP_RSPCODE_INVALID_PARAMETER;
        }
    }
    else
    {
        switch (ftms_var.ftms_control_point.param[0])
        {
        case FTMS_CP_OPCODE_RESET:
            {
                if (!ftms_var.control_permission)
                {
                    resp_code = FTMS_CP_RSPCODE_CONTROL_NOT_PERMITTED;
                }
                else
                {
                    if (parameter_length == 0)
                    {
                    }
                    else
                    {
                        resp_code = FTMS_CP_RSPCODE_INVALID_PARAMETER;
                    }
                }
            }
            break;

        case FTMS_CP_OPCODE_START_RESUME:
            {
                if (!ftms_var.control_permission)
                {
                    resp_code = FTMS_CP_RSPCODE_CONTROL_NOT_PERMITTED;
                }
                else
                {
                    if (parameter_length == 0)
                    {
                    }
                    else
                    {
                        resp_code = FTMS_CP_RSPCODE_INVALID_PARAMETER;
                    }
                }
            }
            break;

        case FTMS_CP_OPCODE_STOP_PAUSE:
            {
                if (!ftms_var.control_permission)
                {
                    resp_code = FTMS_CP_RSPCODE_CONTROL_NOT_PERMITTED;
                }
                else
                {
                    if (parameter_length == 1)
                    {
                        memcpy(&callback_data.msg_data.write.cp_parameter.ctrl_information,
                               &ftms_var.ftms_control_point.param[offset], 1);
                    }
                    else
                    {
                        resp_code = FTMS_CP_RSPCODE_INVALID_PARAMETER;
                    }
                }
            }
            break;

        case FTMS_CP_OPCODE_SET_TGT_SPEED:
            {
                if (ftms_var.ftms_feature.target_setting_field & FTMS_TGT_SET_SPEED_SPT_MASK)
                {
                    if (!ftms_var.control_permission)
                    {
                        resp_code = FTMS_CP_RSPCODE_CONTROL_NOT_PERMITTED;
                    }
                    else
                    {
                        if (parameter_length == 2)
                        {
                            LE_ARRAY_TO_UINT16(callback_data.msg_data.write.cp_parameter.target_speed,
                                               &ftms_var.ftms_control_point.param[offset]);
                            if ((callback_data.msg_data.write.cp_parameter.target_speed >
                                 ftms_var.support_speed_range.max_speed) ||
                                (callback_data.msg_data.write.cp_parameter.target_speed <
                                 ftms_var.support_speed_range.min_speed))
                            {
                                resp_code = FTMS_CP_RSPCODE_INVALID_PARAMETER;
                            }
                        }
                        else
                        {
                            resp_code = FTMS_CP_RSPCODE_INVALID_PARAMETER;
                        }
                    }
                }
                else
                {
                    resp_code = FTMS_CP_RSPCODE_OPCODE_NOT_SUPPORT;
                }
            }
            break;

        case FTMS_CP_OPCODE_SET_TGT_INCLINATION:
            {
                if (ftms_var.ftms_feature.target_setting_field & FTMS_TGT_SET_INCLINATION_SPT_MASK)
                {
                    if (!ftms_var.control_permission)
                    {
                        resp_code = FTMS_CP_RSPCODE_CONTROL_NOT_PERMITTED;
                    }
                    else
                    {
                        if (parameter_length == 2)
                        {
                            LE_ARRAY_TO_UINT16(callback_data.msg_data.write.cp_parameter.target_inclination,
                                               &ftms_var.ftms_control_point.param[offset]);
                            if ((callback_data.msg_data.write.cp_parameter.target_inclination >
                                 ftms_var.support_inclination_range.max_inclination) ||
                                (callback_data.msg_data.write.cp_parameter.target_inclination <
                                 ftms_var.support_inclination_range.min_inclination))
                            {
                                resp_code = FTMS_CP_RSPCODE_INVALID_PARAMETER;
                            }
                        }
                        else
                        {
                            resp_code = FTMS_CP_RSPCODE_INVALID_PARAMETER;
                        }
                    }
                }
                else
                {
                    resp_code = FTMS_CP_RSPCODE_OPCODE_NOT_SUPPORT;
                }
            }
            break;

        case FTMS_CP_OPCODE_SET_TGT_RESISTANCE_LEVEL:
            {
                if (ftms_var.ftms_feature.target_setting_field & FTMS_TGT_SET_RESISTANCE_SPT_MASK)
                {
                    if (!ftms_var.control_permission)
                    {
                        resp_code = FTMS_CP_RSPCODE_CONTROL_NOT_PERMITTED;
                    }
                    else
                    {
                        if (parameter_length == 2)
                        {
                            LE_ARRAY_TO_UINT16(callback_data.msg_data.write.cp_parameter.target_resistance_level,
                                               &ftms_var.ftms_control_point.param[offset]);
                            if ((callback_data.msg_data.write.cp_parameter.target_resistance_level >
                                 ftms_var.support_resistance_level_range.max_resistance_level) ||
                                (callback_data.msg_data.write.cp_parameter.target_resistance_level <
                                 ftms_var.support_resistance_level_range.min_resistance_level))
                            {
                                resp_code = FTMS_CP_RSPCODE_INVALID_PARAMETER;
                            }
                        }
                        else
                        {
                            resp_code = FTMS_CP_RSPCODE_INVALID_PARAMETER;
                        }
                    }
                }
                else
                {
                    resp_code = FTMS_CP_RSPCODE_OPCODE_NOT_SUPPORT;
                }
            }
            break;

        case FTMS_CP_OPCODE_SET_TGT_POWER:
            {
                if (ftms_var.ftms_feature.target_setting_field & FTMS_TGT_SET_POWER_SPT_MASK)
                {
                    if (!ftms_var.control_permission)
                    {
                        resp_code = FTMS_CP_RSPCODE_CONTROL_NOT_PERMITTED;
                    }
                    else
                    {
                        if (parameter_length == 2)
                        {
                            LE_ARRAY_TO_UINT16(callback_data.msg_data.write.cp_parameter.target_power,
                                               &ftms_var.ftms_control_point.param[offset]);
                            if ((callback_data.msg_data.write.cp_parameter.target_power >
                                 ftms_var.support_power_range.max_power) ||
                                (callback_data.msg_data.write.cp_parameter.target_power <
                                 ftms_var.support_power_range.min_power))
                            {
                                resp_code = FTMS_CP_RSPCODE_INVALID_PARAMETER;
                            }
                        }
                        else
                        {
                            resp_code = FTMS_CP_RSPCODE_INVALID_PARAMETER;
                        }
                    }
                }
                else
                {
                    resp_code = FTMS_CP_RSPCODE_OPCODE_NOT_SUPPORT;
                }
            }
            break;

        case FTMS_CP_OPCODE_SET_TGT_HEART_RATE:
            {
                if (ftms_var.ftms_feature.target_setting_field & FTMS_TGT_SET_HR_SPT_MASK)
                {
                    if (!ftms_var.control_permission)
                    {
                        resp_code = FTMS_CP_RSPCODE_CONTROL_NOT_PERMITTED;
                    }
                    else
                    {
                        if (parameter_length == 1)
                        {
                            callback_data.msg_data.write.cp_parameter.target_heart_rate =
                                ftms_var.ftms_control_point.param[offset];
                            if ((callback_data.msg_data.write.cp_parameter.target_heart_rate >
                                 ftms_var.support_heart_rate_range.max_heart_rate) ||
                                (callback_data.msg_data.write.cp_parameter.target_heart_rate <
                                 ftms_var.support_heart_rate_range.min_heart_rate))
                            {
                                resp_code = FTMS_CP_RSPCODE_INVALID_PARAMETER;
                            }
                        }
                        else
                        {
                            resp_code = FTMS_CP_RSPCODE_INVALID_PARAMETER;
                        }
                    }
                }
                else
                {
                    resp_code = FTMS_CP_RSPCODE_OPCODE_NOT_SUPPORT;
                }
            }
            break;

        case FTMS_CP_OPCODE_SET_TGT_EXPEND_ENERGY:
            {
                if (ftms_var.ftms_feature.target_setting_field & FTMS_TGT_EXPENDED_ENERGY_CONFIG_SPT_MASK)
                {
                    if (!ftms_var.control_permission)
                    {
                        resp_code = FTMS_CP_RSPCODE_CONTROL_NOT_PERMITTED;
                    }
                    else
                    {
                        if (parameter_length == 2)
                        {
                            LE_ARRAY_TO_UINT16(callback_data.msg_data.write.cp_parameter.target_expended_energy,
                                               &ftms_var.ftms_control_point.param[offset]);
                        }
                        else
                        {
                            resp_code = FTMS_CP_RSPCODE_INVALID_PARAMETER;
                        }
                    }
                }
                else
                {
                    resp_code = FTMS_CP_RSPCODE_OPCODE_NOT_SUPPORT;
                }
            }
            break;

        case FTMS_CP_OPCODE_SET_TGT_STEP_NUM:
            {
                if (ftms_var.ftms_feature.target_setting_field & FTMS_TGT_STEP_NUM_CONFIG_SPT_MASK)
                {
                    if (!ftms_var.control_permission)
                    {
                        resp_code = FTMS_CP_RSPCODE_CONTROL_NOT_PERMITTED;
                    }
                    else
                    {
                        if (parameter_length == 2)
                        {
                            LE_ARRAY_TO_UINT16(callback_data.msg_data.write.cp_parameter.target_step_num,
                                               &ftms_var.ftms_control_point.param[offset]);
                        }
                        else
                        {
                            resp_code = FTMS_CP_RSPCODE_INVALID_PARAMETER;
                        }
                    }
                }
                else
                {
                    resp_code = FTMS_CP_RSPCODE_OPCODE_NOT_SUPPORT;
                }
            }
            break;

        case FTMS_CP_OPCODE_SET_TGT_STRIDE_NUM:
            {
                if (ftms_var.ftms_feature.target_setting_field & FTMS_TGT_STRIDE_NUM_CONFIG_SPT_MASK)
                {
                    if (!ftms_var.control_permission)
                    {
                        resp_code = FTMS_CP_RSPCODE_CONTROL_NOT_PERMITTED;
                    }
                    else
                    {
                        if (parameter_length == 2)
                        {
                            LE_ARRAY_TO_UINT16(callback_data.msg_data.write.cp_parameter.target_stride_num,
                                               &ftms_var.ftms_control_point.param[offset]);
                        }
                        else
                        {
                            resp_code = FTMS_CP_RSPCODE_INVALID_PARAMETER;
                        }
                    }
                }
                else
                {
                    resp_code = FTMS_CP_RSPCODE_OPCODE_NOT_SUPPORT;
                }
            }
            break;

        case FTMS_CP_OPCODE_SET_TGT_DISTANCE:
            {
                if (ftms_var.ftms_feature.target_setting_field & FTMS_TGT_DISTANCE_CONFIG_SPT_MASK)
                {
                    if (!ftms_var.control_permission)
                    {
                        resp_code = FTMS_CP_RSPCODE_CONTROL_NOT_PERMITTED;
                    }
                    else
                    {
                        if (parameter_length == 3)
                        {
                            LE_ARRAY_TO_UINT24(callback_data.msg_data.write.cp_parameter.target_distance,
                                               &ftms_var.ftms_control_point.param[offset]);
                        }
                        else
                        {
                            resp_code = FTMS_CP_RSPCODE_INVALID_PARAMETER;
                        }
                    }
                }
                else
                {
                    resp_code = FTMS_CP_RSPCODE_OPCODE_NOT_SUPPORT;
                }
            }
            break;

        case FTMS_CP_OPCODE_SET_TGT_TRAINING_TIME:
            {
                if (ftms_var.ftms_feature.target_setting_field & FTMS_TGT_TRAINING_TIME_CONFIG_SPT_MASK)
                {
                    if (!ftms_var.control_permission)
                    {
                        resp_code = FTMS_CP_RSPCODE_CONTROL_NOT_PERMITTED;
                    }
                    else
                    {
                        if (parameter_length == 2)
                        {
                            LE_ARRAY_TO_UINT16(callback_data.msg_data.write.cp_parameter.target_training_time,
                                               &ftms_var.ftms_control_point.param[offset]);
                        }
                        else
                        {
                            resp_code = FTMS_CP_RSPCODE_INVALID_PARAMETER;
                        }
                    }
                }
                else
                {
                    resp_code = FTMS_CP_RSPCODE_OPCODE_NOT_SUPPORT;
                }
            }
            break;

        case FTMS_CP_OPCODE_SET_TGT_TWO_HR_ZONES_TIME:
            {
                if (ftms_var.ftms_feature.target_setting_field & FTMS_TGT_TWO_HR_ZONES_TIME_CONFIG_SPT_MASK)
                {
                    if (!ftms_var.control_permission)
                    {
                        resp_code = FTMS_CP_RSPCODE_CONTROL_NOT_PERMITTED;
                    }
                    else
                    {
                        if (parameter_length == 4)
                        {
                            LE_ARRAY_TO_UINT16(
                                callback_data.msg_data.write.cp_parameter.target_two_zones_hr_time.fat_burn_zone_time,
                                &ftms_var.ftms_control_point.param[offset]);
                            offset += 2;
                            LE_ARRAY_TO_UINT16(
                                callback_data.msg_data.write.cp_parameter.target_two_zones_hr_time.fitness_zones_time,
                                &ftms_var.ftms_control_point.param[offset]);
                        }
                        else
                        {
                            resp_code = FTMS_CP_RSPCODE_INVALID_PARAMETER;
                        }
                    }
                }
                else
                {
                    resp_code = FTMS_CP_RSPCODE_OPCODE_NOT_SUPPORT;
                }
            }
            break;

        case FTMS_CP_OPCODE_SET_TGT_THREE_HR_ZONES_TIME:
            {
                if (ftms_var.ftms_feature.target_setting_field & FTMS_TGT_THREE_HR_ZONES_CONFIG_SPT_MASK)
                {
                    if (!ftms_var.control_permission)
                    {
                        resp_code = FTMS_CP_RSPCODE_CONTROL_NOT_PERMITTED;
                    }
                    else
                    {
                        if (parameter_length == 6)
                        {
                            LE_ARRAY_TO_UINT16(
                                callback_data.msg_data.write.cp_parameter.target_three_zones_hr_time.light_zone_time,
                                &ftms_var.ftms_control_point.param[offset]);
                            offset += 2;
                            LE_ARRAY_TO_UINT16(
                                callback_data.msg_data.write.cp_parameter.target_three_zones_hr_time.moderate_zone_time,
                                &ftms_var.ftms_control_point.param[offset]);
                            offset += 2;
                            LE_ARRAY_TO_UINT16(
                                callback_data.msg_data.write.cp_parameter.target_three_zones_hr_time.hard_zone_time,
                                &ftms_var.ftms_control_point.param[offset]);
                        }
                        else
                        {
                            resp_code = FTMS_CP_RSPCODE_INVALID_PARAMETER;
                        }
                    }
                }
                else
                {
                    resp_code = FTMS_CP_RSPCODE_OPCODE_NOT_SUPPORT;
                }
            }
            break;

        case FTMS_CP_OPCODE_SET_TGT_FIVE_HR_ZONES_TIME:
            {
                if (ftms_var.ftms_feature.target_setting_field & FTMS_TGT_FIVE_HR_ZONES_CONFIG_SPT_MASK)
                {
                    if (!ftms_var.control_permission)
                    {
                        resp_code = FTMS_CP_RSPCODE_CONTROL_NOT_PERMITTED;
                    }
                    else
                    {
                        if (parameter_length == 10)
                        {
                            LE_ARRAY_TO_UINT16(
                                callback_data.msg_data.write.cp_parameter.target_five_zones_hr_time.very_light_zone_time,
                                &ftms_var.ftms_control_point.param[offset]);
                            offset += 2;
                            LE_ARRAY_TO_UINT16(
                                callback_data.msg_data.write.cp_parameter.target_five_zones_hr_time.light_zone_time,
                                &ftms_var.ftms_control_point.param[offset]);
                            offset += 2;
                            LE_ARRAY_TO_UINT16(
                                callback_data.msg_data.write.cp_parameter.target_five_zones_hr_time.moderate_zone_time,
                                &ftms_var.ftms_control_point.param[offset]);
                            offset += 2;
                            LE_ARRAY_TO_UINT16(
                                callback_data.msg_data.write.cp_parameter.target_five_zones_hr_time.hard_zone_time,
                                &ftms_var.ftms_control_point.param[offset]);
                            offset += 2;
                            LE_ARRAY_TO_UINT16(
                                callback_data.msg_data.write.cp_parameter.target_five_zones_hr_time.maximum_zone_time,
                                &ftms_var.ftms_control_point.param[offset]);
                        }
                        else
                        {
                            resp_code = FTMS_CP_RSPCODE_INVALID_PARAMETER;
                        }
                    }
                }
                else
                {
                    resp_code = FTMS_CP_RSPCODE_OPCODE_NOT_SUPPORT;
                }
            }
            break;

        case FTMS_CP_OPCODE_SET_INDOOR_BIKE_SIMULATION_PARAM:
            {
                if (ftms_var.ftms_feature.target_setting_field & FTMS_INDOOR_BIKE_SIMULATION_PARAM_SPT_MASK)
                {
                    if (!ftms_var.control_permission)
                    {
                        resp_code = FTMS_CP_RSPCODE_CONTROL_NOT_PERMITTED;
                    }
                    else
                    {
                        if (parameter_length == 6)
                        {
                            LE_ARRAY_TO_UINT16(
                                callback_data.msg_data.write.cp_parameter.set_indoor_bike_simulation_param.wind_speed,
                                &ftms_var.ftms_control_point.param[offset]);
                            offset += 2;
                            LE_ARRAY_TO_UINT16(callback_data.msg_data.write.cp_parameter.set_indoor_bike_simulation_param.grade,
                                               &ftms_var.ftms_control_point.param[offset]);
                            offset += 2;
                            LE_ARRAY_TO_UINT8(
                                callback_data.msg_data.write.cp_parameter.set_indoor_bike_simulation_param.rolling_resistance_coefficient,
                                &ftms_var.ftms_control_point.param[offset]);
                            offset += 1;
                            LE_ARRAY_TO_UINT8(
                                callback_data.msg_data.write.cp_parameter.set_indoor_bike_simulation_param.wind_resistance_coefficient,
                                &ftms_var.ftms_control_point.param[offset]);
                        }
                        else
                        {
                            resp_code = FTMS_CP_RSPCODE_INVALID_PARAMETER;
                        }
                    }
                }
                else
                {
                    resp_code = FTMS_CP_RSPCODE_OPCODE_NOT_SUPPORT;
                }
            }
            break;

#if FTMS_CHAR_CP_SET_WHEEL_CIRCUMFERENCE_SUPPORT
        case FTMS_CP_OPCODE_SET_WHEEL_CIRCUMFERENCE:
            {
                if (!ftms_var.control_permission)
                {
                    resp_code = FTMS_CP_RSPCODE_CONTROL_NOT_PERMITTED;
                }
                else
                {
                    if (parameter_length == 2)
                    {
                        LE_ARRAY_TO_UINT16(callback_data.msg_data.write.cp_parameter.wheel_circumference,
                                           &ftms_var.ftms_control_point.param[offset]);
                    }
                    else
                    {
                        resp_code = FTMS_CP_RSPCODE_INVALID_PARAMETER;
                    }

                }

            }
            break;
#endif

#if FTMS_CHAR_CP_SPIN_DOWN_CTRL_SUPPORT
        case FTMS_CP_OPCODE_SPIN_DOWN_CTRL:
            {
                if (!ftms_var.control_permission)
                {
                    resp_code = FTMS_CP_RSPCODE_CONTROL_NOT_PERMITTED;
                }
                else
                {
                    if (parameter_length == 1)
                    {
                        memcpy(&callback_data.msg_data.write.cp_parameter.ctrl_param,
                               &ftms_var.ftms_control_point.param[offset], 1);
                    }
                    else
                    {
                        resp_code = FTMS_CP_RSPCODE_INVALID_PARAMETER;
                    }
                }
            }
            break;
#endif

        case FTMS_CP_OPCODE_SET_TGT_CADENCE:
            {
                if (ftms_var.ftms_feature.target_setting_field & FTMS_TGT_CADENCE_CONFIG_SPT_MASK)
                {
                    if (!ftms_var.control_permission)
                    {
                        resp_code = FTMS_CP_RSPCODE_CONTROL_NOT_PERMITTED;
                    }
                    else
                    {
                        if (parameter_length == 2)
                        {
                            LE_ARRAY_TO_UINT16(callback_data.msg_data.write.cp_parameter.target_cadence,
                                               &ftms_var.ftms_control_point.param[1]);
                        }
                        else
                        {
                            resp_code = FTMS_CP_RSPCODE_INVALID_PARAMETER;
                        }
                    }
                }
                else
                {
                    resp_code = FTMS_CP_RSPCODE_OPCODE_NOT_SUPPORT;
                }
            }
            break;

        default:
            {
                resp_code = FTMS_CP_RSPCODE_OPCODE_NOT_SUPPORT;
            }
            break;
        }
    }

    if (resp_code == FTMS_CP_RSPCODE_SUCCESS)
    {
        pfn_ftms_cb(service_id, (void *)&callback_data);
    }

    return resp_code;
}

static void ftms_ctl_pnt_handle_req(uint8_t conn_id, T_SERVER_ID service_id, uint16_t attrib_index,
                                    uint16_t write_length, uint8_t *p_value)
{
    uint8_t resp_code = FTMS_CP_RSPCODE_SUCCESS;
    memcpy(ftms_var.ftms_control_point.param, p_value, write_length);
    ftms_var.ftms_control_point.cur_length = write_length;
    resp_code = ftms_handle_ctl_pnt_proc(conn_id, service_id, attrib_index, write_length, p_value);

    if (resp_code == FTMS_CP_RSPCODE_SUCCESS)
    {
        switch (ftms_var.ftms_control_point.param[0])
        {
        case FTMS_CP_OPCODE_REQUEST_CONTROL:
            {
                ftms_ctl_pnt_request_control(conn_id, service_id, resp_code);
            }
            break;

        case FTMS_CP_OPCODE_RESET:
            {
                ftms_ctl_pnt_reset(conn_id, service_id, resp_code);
            }
            break;

        case FTMS_CP_OPCODE_START_RESUME:
            {
                ftms_ctl_pnt_start_resume(conn_id, service_id, resp_code);
            }
            break;

        case FTMS_CP_OPCODE_STOP_PAUSE:
            {
                ftms_ctl_pnt_stop_pause(conn_id, service_id, resp_code);
            }
            break;
#if FTMS_CHAR_CP_SPIN_DOWN_CTRL_SUPPORT
        case FTMS_CP_OPCODE_SPIN_DOWN_CTRL:
            {
                ftms_ctl_pnt_spin_down_ctrl(conn_id, service_id, resp_code);
            }
            break;
#endif

        default:
            {
                ftms_ctl_pnt_write_target_data(conn_id, service_id,
                                               ftms_var.ftms_control_point.param[0], resp_code);
            }
            break;
        }
    }
    else
    {
        ftms_ctl_pnt_indication(conn_id, service_id, ftms_var.ftms_control_point.param[0], resp_code);
    }
}
#endif


#if FTMS_CHAR_TRAINING_STATUS_SUPPORT
bool ftms_train_status_notify(uint8_t conn_id, T_SERVER_ID service_id, uint16_t string_len,
                              T_FTMS_TRAIN_STATUS *p_train_status_data)
{
    bool ret = false;

    if (ftms_var.ftms_notify_indicate_flag.ftms_train_status_notify_enable == 0)
    {
        PROFILE_PRINT_INFO0("ftms_train_status_notify: FTMS_APP_RESULT_CCCD_NOT_ENABLED");
        return ret;
    }

    T_SRV_UUID_TBL *p_char;
    p_char = srv_find_index_by_uuid(ftms_var.ftms_srv_char_tbl, GATT_UUID_TRAINING_STATUS);
    uint16_t attrib_index = p_char->char_index;
    uint8_t *p_data = NULL;
    uint16_t data_len = 0;
    uint8_t offset = 0;
    uint8_t *p;

    uint16_t mtu_size;
    le_get_conn_param(GAP_PARAM_CONN_MTU_SIZE, &mtu_size, conn_id);

    if (p_train_status_data->flag & FTMS_TS_STRING_PRESENT_MASK)
    {
        if (string_len && p_train_status_data->train_status_string == NULL)
        {
            PROFILE_PRINT_ERROR0("ftms_train_status_notify: no training status string!");
            return ret;
        }

        if (string_len <= (mtu_size - 5))
        {
            data_len = string_len + 2 * sizeof(uint8_t);
            p_data = os_mem_zalloc(RAM_TYPE_DATA_ON, data_len);
            p_train_status_data->flag &= (~FTMS_TS_EXTENDED_STRING_PRESENT_MASK);

            p = p_data + offset;
            LE_UINT8_TO_STREAM(p, p_train_status_data->flag);
            offset += 1;
            p = p_data + offset;
            LE_UINT8_TO_STREAM(p, p_train_status_data->train_status);
            offset += 1;
            memcpy(p_data + offset, p_train_status_data->train_status_string, string_len);
        }
        else
        {
            data_len = mtu_size - 3;
            p_data = os_mem_zalloc(RAM_TYPE_DATA_ON, data_len);
            p_train_status_data->flag |= FTMS_TS_EXTENDED_STRING_PRESENT_MASK;

            p = p_data + offset;
            LE_UINT8_TO_STREAM(p, p_train_status_data->flag);
            offset += 1;
            p = p_data + offset;
            LE_UINT8_TO_STREAM(p, p_train_status_data->train_status);
            offset += 1;
            memcpy(p_data + offset, p_train_status_data->train_status_string, string_len);
        }
    }
    else
    {
        data_len = 2 * sizeof(uint8_t);
        p_data = os_mem_zalloc(RAM_TYPE_DATA_ON, data_len);
        p_train_status_data->flag &= (~FTMS_TS_EXTENDED_STRING_PRESENT_MASK);

        p = p_data + offset;
        LE_UINT8_TO_STREAM(p, p_train_status_data->flag);
        offset += 1;
        p = p_data + offset;
        LE_UINT8_TO_STREAM(p, p_train_status_data->train_status);
        offset += 1;
    }

    if (server_send_data(conn_id, service_id, attrib_index, p_data, data_len,
                         GATT_PDU_TYPE_NOTIFICATION))
    {
        ret = true;
        PROFILE_PRINT_INFO0("ftms_train_status_notify:server_send_data and set status doing success");
    }
    else
    {
        PROFILE_PRINT_ERROR0("ftms_train_status_notify:server_send_data fail");
    }

    if (p_data != NULL)
    {
        os_mem_free(p_data);
    }

    return ret;
}

bool ftms_train_status_read_confirm(uint8_t conn_id, T_SERVER_ID service_id,
                                    uint8_t *p_data, uint16_t len, T_APP_RESULT cause)
{
    T_SRV_UUID_TBL *p_char;
    p_char = srv_find_index_by_uuid(ftms_var.ftms_srv_char_tbl, GATT_UUID_TRAINING_STATUS);
    uint16_t attrib_index = p_char->char_index;

    return server_attr_read_confirm(conn_id, service_id, attrib_index, p_data, len, cause);
}
#endif

T_APP_RESULT ftms_attr_read_cb(uint8_t conn_id, T_SERVER_ID service_id, uint16_t attrib_index,
                               uint16_t offset, uint16_t *p_length, uint8_t **pp_value)
{
    T_APP_RESULT cause = APP_RESULT_SUCCESS;
    T_SRV_UUID_TBL *p_char;
    p_char = srv_find_uuid_by_index(ftms_var.ftms_srv_char_tbl, attrib_index);

    uint8_t *p_data = NULL;
    uint8_t *p;
    uint16_t data_length = 0;

    PROFILE_PRINT_INFO1("ftms_attr_read_cb: char_uuid16 0x%x", p_char->char_uuid16);

    switch (p_char->char_uuid16)
    {
    case GATT_UUID_FITNESS_MACHINE_FEATURE:
        {
            data_length = sizeof(ftms_var.ftms_feature);
            p_data = os_mem_zalloc(RAM_TYPE_DATA_ON, data_length);
            p = p_data;
            LE_UINT32_TO_STREAM(p, ftms_var.ftms_feature.features_field);
            LE_UINT32_TO_STREAM(p, ftms_var.ftms_feature.target_setting_field);
        }
        break;

#if FTMS_CHAR_TRAINING_STATUS_SUPPORT
    case GATT_UUID_TRAINING_STATUS:
        {
            T_FTMS_CALLBACK_DATA callback_data;
            callback_data.conn_id = conn_id;
            callback_data.msg_type = SERVICE_CALLBACK_TYPE_READ_CHAR_VALUE;
            callback_data.char_uuid = GATT_UUID_TRAINING_STATUS;

            cause = pfn_ftms_cb(service_id, &callback_data);
        }
        break;
#endif

#if FTMS_CHAR_SUPPORT_SPEED_RANGE_SUPPORT
    case GATT_UUID_SUPPORT_SPEED_RANGE:
        {
            data_length = sizeof(ftms_var.support_speed_range);
            p_data = os_mem_zalloc(RAM_TYPE_DATA_ON, data_length);
            p = p_data;
            LE_UINT16_TO_STREAM(p, ftms_var.support_speed_range.min_speed);
            LE_UINT16_TO_STREAM(p, ftms_var.support_speed_range.max_speed);
            LE_UINT16_TO_STREAM(p, ftms_var.support_speed_range.min_speed_increment);
        }
        break;
#endif

#if FTMS_CHAR_SUPPORT_INCLINATION_RANGE_SUPPORT
    case GATT_UUID_SUPPORT_INCLINATION_RANGE:
        {
            data_length = sizeof(ftms_var.support_inclination_range);
            p_data = os_mem_zalloc(RAM_TYPE_DATA_ON, data_length);
            p = p_data;
            LE_UINT16_TO_STREAM(p, ftms_var.support_inclination_range.min_inclination);
            LE_UINT16_TO_STREAM(p, ftms_var.support_inclination_range.max_inclination);
            LE_UINT16_TO_STREAM(p, ftms_var.support_inclination_range.min_inclination_increment);
        }
        break;
#endif

#if FTMS_CHAR_SUPPORT_RESISTANCE_LEVEL_RANGE_SUPPORT
    case GATT_UUID_SUPPORT_RESISTANCE_LEVEL_RANGE:
        {
            data_length = sizeof(ftms_var.support_resistance_level_range);
            p_data = os_mem_zalloc(RAM_TYPE_DATA_ON, data_length);
            p = p_data;
            LE_UINT16_TO_STREAM(p, ftms_var.support_resistance_level_range.min_resistance_level);
            LE_UINT16_TO_STREAM(p, ftms_var.support_resistance_level_range.max_resistance_level);
            LE_UINT16_TO_STREAM(p, ftms_var.support_resistance_level_range.min_resistance_level_increment);
        }
        break;
#endif

#if FTMS_CHAR_SUPPORT_HR_RANGE_SUPPORT
    case GATT_UUID_SUPPORT_HEART_RATE_RANGE:
        {
            data_length = sizeof(ftms_var.support_heart_rate_range);
            p_data = os_mem_zalloc(RAM_TYPE_DATA_ON, data_length);
            p = p_data;
            LE_UINT8_TO_STREAM(p, ftms_var.support_heart_rate_range.min_heart_rate);
            LE_UINT8_TO_STREAM(p, ftms_var.support_heart_rate_range.max_heart_rate);
            LE_UINT8_TO_STREAM(p, ftms_var.support_heart_rate_range.min_hr_increment);
        }
        break;
#endif

#if FTMS_CHAR_SUPPORT_POWER_RANGE_SUPPORT
    case GATT_UUID_SUPPORT_POWER_RANGE:
        {
            data_length = sizeof(ftms_var.support_power_range);
            p_data = os_mem_zalloc(RAM_TYPE_DATA_ON, data_length);
            p = p_data;
            LE_UINT16_TO_STREAM(p, ftms_var.support_power_range.min_power);
            LE_UINT16_TO_STREAM(p, ftms_var.support_power_range.max_power);
            LE_UINT16_TO_STREAM(p, ftms_var.support_power_range.min_power_increment);
        }
        break;
#endif

    default:
        {
            cause = APP_RESULT_ATTR_NOT_FOUND;
            PROFILE_PRINT_ERROR1("ftms_attr_read_cb Error not found attribute char_uuid 0x%x",
                                 p_char->char_uuid16);
        }
        break;
    }

    if (cause == APP_RESULT_SUCCESS)
    {
        if (server_attr_read_confirm(conn_id, service_id, attrib_index, p_data,
                                     data_length, APP_RESULT_SUCCESS))
        {
            cause = APP_RESULT_PENDING;
        }

        if (p_data != NULL)
        {
            os_mem_free(p_data);
        }
    }

    return cause;
}

T_APP_RESULT ftms_attr_write_cb(uint8_t conn_id, T_SERVER_ID service_id, uint16_t attrib_index,
                                T_WRITE_TYPE write_type, uint16_t length, uint8_t *p_value,
                                P_FUN_WRITE_IND_POST_PROC *p_write_ind_post_proc)
{
    T_APP_RESULT cause = APP_RESULT_SUCCESS;
    T_SRV_UUID_TBL *p_char;
    p_char = srv_find_uuid_by_index(ftms_var.ftms_srv_char_tbl, attrib_index);

    PROFILE_PRINT_INFO2("ftms_attr_write_cb:attrib_index %d, char_uuid16 0x%x",
                        attrib_index, p_char->char_uuid16);

#if FTMS_CHAR_FTMS_CTRL_PNT_SUPPORT
    if (p_char->char_uuid16 == GATT_UUID_FTMS_CONTROL_POINT)
    {
        if ((length > FTMS_MAX_CTL_PNT_PARAM_LEN) || (p_value == NULL))
        {
            cause = APP_RESULT_INVALID_VALUE_SIZE;
        }
        else if (FTMS_CTL_PNT_OPERATE_ACTIVE(ftms_var.ftms_control_point.param[0]))
        {
            cause = APP_RESULT_PROC_ALREADY_IN_PROGRESS;
        }
        /* Make sure Control Point is configured indication enable. */
        else if (false == ftms_var.ftms_notify_indicate_flag.ftms_cp_indicate_enable)
        {
            cause = APP_RESULT_CCCD_IMPROPERLY_CONFIGURED;
        }
        else
        {
            *p_write_ind_post_proc = ftms_ctl_pnt_handle_req;
        }
    }
    else
    {
        PROFILE_PRINT_ERROR2("ftms_attr_write_cb Error char_uuid16 0x%x, length %d",
                             p_char->char_uuid16, length);
        cause = APP_RESULT_ATTR_NOT_FOUND;
    }
#endif

    return cause;
}

void ftms_cccd_update_cb(uint8_t conn_id, T_SERVER_ID service_id, uint16_t index, uint16_t ccc_bits)
{
    T_FTMS_CALLBACK_DATA callback_data;
    bool handle = true;
    T_SRV_UUID_TBL *p_char;

    callback_data.msg_type = SERVICE_CALLBACK_TYPE_INDIFICATION_NOTIFICATION;
    p_char = srv_find_uuid_by_index(ftms_var.ftms_srv_char_tbl, index - 1);

    PROFILE_PRINT_INFO2("ftms_cccd_update_cb:char_uuid16 0x%x ccc_bits %x", p_char->char_uuid16,
                        ccc_bits);

    switch (p_char->char_uuid16)
    {
#if FTMS_CHAR_TREADMILL_DATA_SUPPORT
    case GATT_UUID_TREADMILL_DATA:
        {
            if (ccc_bits & GATT_CLIENT_CHAR_CONFIG_NOTIFY)
            {
                ftms_var.ftms_notify_indicate_flag.ftms_treadmill_data_notify_enable = 1;
                callback_data.msg_data.notification_indication_index =
                    FTMS_NOTIFY_INDICATE_TREADMILL_DATA_ENABLE;
                PROFILE_PRINT_INFO0("ftms_cccd_update_cb: FTMS_NOTIFY_INDICATE_TREADMILL_DATA_ENABLE");
            }
            else
            {
                ftms_var.ftms_notify_indicate_flag.ftms_treadmill_data_notify_enable = 0;
                callback_data.msg_data.notification_indication_index =
                    FTMS_NOTIFY_INDICATE_TREADMILL_DATA_DISABLE;
            }
        }
        break;
#endif
#if FTMS_CHAR_CROSS_TRAINER_DATA_SUPPORT
    case GATT_UUID_CROSS_TRAINER_DATA:
        {
            if (ccc_bits & GATT_CLIENT_CHAR_CONFIG_NOTIFY)
            {
                ftms_var.ftms_notify_indicate_flag.ftms_cross_trainer_data_notify_enable = 1;
                callback_data.msg_data.notification_indication_index =
                    FTMS_NOTIFY_INDICATE_CROSS_TRAINER_DATA_ENABLE;
                PROFILE_PRINT_INFO0("ftms_cccd_update_cb: FTMS_NOTIFY_INDICATE_CROSS_TRAINER_DATA_ENABLE");
            }
            else
            {
                ftms_var.ftms_notify_indicate_flag.ftms_cross_trainer_data_notify_enable = 0;
                callback_data.msg_data.notification_indication_index =
                    FTMS_NOTIFY_INDICATE_CROSS_TRAINER_DATA_DISABLE;
            }
        }
        break;
#endif
#if FTMS_CHAR_STEP_CLIMBER_DATA_SUPPORT
    case GATT_UUID_STEP_CLIMBER_DATA:
        {
            if (ccc_bits & GATT_CLIENT_CHAR_CONFIG_NOTIFY)
            {
                ftms_var.ftms_notify_indicate_flag.ftms_step_climber_data_notify_enable = 1;
                callback_data.msg_data.notification_indication_index =
                    FTMS_NOTIFY_INDICATE_STEP_CLIMBER_DATA_ENABLE;
                PROFILE_PRINT_INFO0("ftms_cccd_update_cb: FTMS_NOTIFY_INDICATE_STEP_CLIMBER_DATA_ENABLE");
            }
            else
            {
                ftms_var.ftms_notify_indicate_flag.ftms_step_climber_data_notify_enable = 0;
                callback_data.msg_data.notification_indication_index =
                    FTMS_NOTIFY_INDICATE_STEP_CLIMBER_DATA_DISABLE;
            }
        }
        break;
#endif
#if FTMS_CHAR_STAIR_CLIMBER_DATA_SUPPORT
    case GATT_UUID_STAIR_CLIMBER_DATA:
        {
            if (ccc_bits & GATT_CLIENT_CHAR_CONFIG_NOTIFY)
            {
                ftms_var.ftms_notify_indicate_flag.ftms_stair_climber_data_notify_enable = 1;
                callback_data.msg_data.notification_indication_index =
                    FTMS_NOTIFY_INDICATE_STAIR_CLIMBER_DATA_ENABLE;
                PROFILE_PRINT_INFO0("ftms_cccd_update_cb: FTMS_NOTIFY_INDICATE_STEP_CLIMBER_DATA_ENABLE");
            }
            else
            {
                ftms_var.ftms_notify_indicate_flag.ftms_stair_climber_data_notify_enable = 0;
                callback_data.msg_data.notification_indication_index =
                    FTMS_NOTIFY_INDICATE_STAIR_CLIMBER_DATA_DISABLE;
            }

        }
        break;
#endif
#if FTMS_CHAR_ROWER_DATA_SUPPORT
    case GATT_UUID_ROWER_DATA:
        {
            if (ccc_bits & GATT_CLIENT_CHAR_CONFIG_NOTIFY)
            {
                ftms_var.ftms_notify_indicate_flag.ftms_rower_data_notify_enable = 1;
                callback_data.msg_data.notification_indication_index =
                    FTMS_NOTIFY_INDICATE_ROWER_DATA_ENABLE;
                PROFILE_PRINT_INFO0("ftms_cccd_update_cb: FTMS_NOTIFY_INDICATE_ROWER_DATA_ENABLE");
            }
            else
            {
                ftms_var.ftms_notify_indicate_flag.ftms_rower_data_notify_enable = 0;
                callback_data.msg_data.notification_indication_index =
                    FTMS_NOTIFY_INDICATE_ROWER_DATA_DISABLE;
            }
        }
        break;
#endif
#if FTMS_CHAR_INDOOR_BIKE_DATA_SUPPORT
    case GATT_UUID_INDOOR_BIKE_DATA:
        {
            if (ccc_bits & GATT_CLIENT_CHAR_CONFIG_NOTIFY)
            {
                ftms_var.ftms_notify_indicate_flag.ftms_indoor_bike_data_notify_enable = 1;
                callback_data.msg_data.notification_indication_index =
                    FTMS_NOTIFY_INDICATE_INDOOR_BIKE_DATA_ENABLE;
                PROFILE_PRINT_INFO0("ftms_cccd_update_cb: FTMS_NOTIFY_INDICATE_INDOOR_BIKE_DATA_ENABLE");
            }
            else
            {
                ftms_var.ftms_notify_indicate_flag.ftms_indoor_bike_data_notify_enable = 0;
                callback_data.msg_data.notification_indication_index =
                    FTMS_NOTIFY_INDICATE_INDOOR_BIKE_DATA_DISABLE;
            }
        }
        break;
#endif
#if FTMS_CHAR_FTMS_CTRL_PNT_SUPPORT
    case GATT_UUID_FTMS_CONTROL_POINT:
        {
            if (ccc_bits & GATT_CLIENT_CHAR_CONFIG_INDICATE)
            {
                ftms_var.ftms_notify_indicate_flag.ftms_cp_indicate_enable = 1;
                callback_data.msg_data.notification_indication_index =
                    FTMS_NOTIFY_INDICATE_FTMS_CP_ENABLE;
                PROFILE_PRINT_INFO0("ftms_cccd_update_cb: FTMS_NOTIFY_INDICATE_FTMS_CP_ENABLE");
            }
            else
            {
                ftms_var.ftms_notify_indicate_flag.ftms_cp_indicate_enable = 0;
                callback_data.msg_data.notification_indication_index =
                    FTMS_NOTIFY_INDICATE_FTMS_CP_DISABLE;
            }
        }
        break;
#endif
#if FTMS_CHAR_TRAINING_STATUS_SUPPORT
    case GATT_UUID_TRAINING_STATUS:
        {
            if (ccc_bits & GATT_CLIENT_CHAR_CONFIG_NOTIFY)
            {
                ftms_var.ftms_notify_indicate_flag.ftms_train_status_notify_enable = 1;
                callback_data.msg_data.notification_indication_index =
                    FTMS_NOTIFY_INDICATE_TRAIN_STATUS_ENABLE;
                PROFILE_PRINT_INFO0("ftms_cccd_update_cb: FTMS_NOTIFY_INDICATE_TRAIN_STATUS_ENABLE");
            }
            else
            {
                ftms_var.ftms_notify_indicate_flag.ftms_train_status_notify_enable = 0;
                callback_data.msg_data.notification_indication_index =
                    FTMS_NOTIFY_INDICATE_TRAIN_STATUS_DISABLE;
            }
        }
        break;
#endif
#if (FTMS_CHAR_FTMS_CTRL_PNT_SUPPORT || FTMS_CHAR_CP_SET_WHEEL_CIRCUMFERENCE_SUPPORT)
    case GATT_UUID_FITNESS_MACHINE_STATUS:
        {
            if (ccc_bits & GATT_CLIENT_CHAR_CONFIG_NOTIFY)
            {
                ftms_var.ftms_notify_indicate_flag.ftms_status_notify_enable = 1;
                callback_data.msg_data.notification_indication_index =
                    FTMS_NOTIFY_INDICATE_FTMS_STATUS_ENABLE;
                PROFILE_PRINT_INFO0("ftms_cccd_update_cb: FTMS_NOTIFY_INDICATE_FTMS_STATUS_ENABLE");
            }
            else
            {
                ftms_var.ftms_notify_indicate_flag.ftms_status_notify_enable = 0;
                callback_data.msg_data.notification_indication_index =
                    FTMS_NOTIFY_INDICATE_FTMS_STATUS_DISABLE;
            }
        }
        break;
#endif

    default:
        {
            handle = false;
            PROFILE_PRINT_ERROR1("ftms_cccd_update_cb: char_uuid16 0x%x", p_char->char_uuid16);
        }
        break;
    }

    if (pfn_ftms_cb && (handle == true))
    {
        pfn_ftms_cb(service_id, (void *)&callback_data);
    }
}

void ftms_flags_clear(void)
{
    PROFILE_PRINT_ERROR0("ftms_flags_clear");

#if FTMS_CHAR_TREADMILL_DATA_SUPPORT
    memset(&ftms_var.ftms_treadmill_notify_flag, 0, sizeof(T_FTMS_NOTIFY_FLAG));
    if (ftms_var.ftms_treadmill_notify_data != NULL)
    {
        os_mem_free(ftms_var.ftms_treadmill_notify_data);
        ftms_var.ftms_treadmill_notify_data = NULL;
    }

    if (ftms_var.ftms_treadmill_send_data != NULL)
    {
        os_mem_free(ftms_var.ftms_treadmill_send_data);
        ftms_var.ftms_treadmill_send_data = NULL;
    }

#endif

#if FTMS_CHAR_STEP_CLIMBER_DATA_SUPPORT
    memset(&ftms_var.ftms_step_climber_notify_flag, 0, sizeof(T_FTMS_NOTIFY_FLAG));
    if (ftms_var.ftms_step_climber_notify_data != NULL)
    {
        os_mem_free(ftms_var.ftms_step_climber_notify_data);
        ftms_var.ftms_step_climber_notify_data = NULL;
    }

    if (ftms_var.ftms_step_climber_send_data != NULL)
    {
        os_mem_free(ftms_var.ftms_step_climber_send_data);
        ftms_var.ftms_step_climber_send_data = NULL;
    }
#endif

#if FTMS_CHAR_ROWER_DATA_SUPPORT
    memset(&ftms_var.ftms_rower_notify_flag, 0, sizeof(T_FTMS_NOTIFY_FLAG));
    if (ftms_var.ftms_rower_notify_data != NULL)
    {
        os_mem_free(ftms_var.ftms_rower_notify_data);
        ftms_var.ftms_rower_notify_data = NULL;
    }

    if (ftms_var.ftms_rower_send_data != NULL)
    {
        os_mem_free(ftms_var.ftms_rower_send_data);
        ftms_var.ftms_rower_send_data = NULL;
    }
#endif

#if FTMS_CHAR_INDOOR_BIKE_DATA_SUPPORT
    memset(&ftms_var.ftms_indoor_bike_notify_flag, 0, sizeof(T_FTMS_NOTIFY_FLAG));
    if (ftms_var.ftms_indoor_bike_notify_data != NULL)
    {
        os_mem_free(ftms_var.ftms_indoor_bike_notify_data);
        ftms_var.ftms_indoor_bike_notify_data = NULL;
    }

    if (ftms_var.ftms_indoor_bike_send_data != NULL)
    {
        os_mem_free(ftms_var.ftms_indoor_bike_send_data);
        ftms_var.ftms_indoor_bike_send_data = NULL;
    }
#endif

#if FTMS_CHAR_CROSS_TRAINER_DATA_SUPPORT
    memset(&ftms_var.ftms_cross_trainer_notify_flag, 0, sizeof(T_FTMS_NOTIFY_FLAG));
    if (ftms_var.ftms_cross_trainer_notify_data != NULL)
    {
        os_mem_free(ftms_var.ftms_cross_trainer_notify_data);
        ftms_var.ftms_cross_trainer_notify_data = NULL;
    }

    if (ftms_var.ftms_cross_trainer_send_data != NULL)
    {
        os_mem_free(ftms_var.ftms_cross_trainer_send_data);
        ftms_var.ftms_cross_trainer_send_data = NULL;
    }
#endif

#if FTMS_CHAR_STAIR_CLIMBER_DATA_SUPPORT
    memset(&ftms_var.ftms_stair_climber_notify_flag, 0, sizeof(T_FTMS_NOTIFY_FLAG));
    if (ftms_var.ftms_stair_climber_notify_data != NULL)
    {
        os_mem_free(ftms_var.ftms_stair_climber_notify_data);
        ftms_var.ftms_stair_climber_notify_data = NULL;
    }

    if (ftms_var.ftms_stair_climber_send_data != NULL)
    {
        os_mem_free(ftms_var.ftms_stair_climber_send_data);
        ftms_var.ftms_stair_climber_send_data = NULL;
    }
#endif

#if FTMS_CHAR_FTMS_CTRL_PNT_SUPPORT
    ftms_var.ftms_control_point.param[0] = FTMS_CP_OPCODE_RESERVED;
    ftms_var.control_permission = false;
    ftms_var.ftms_cur_status = 0;
#endif
}

const T_FUN_GATT_SERVICE_CBS ftms_cbs =
{
    ftms_attr_read_cb,  // Read callback function pointer
    ftms_attr_write_cb, // Write callback function pointer
    ftms_cccd_update_cb // CCCD update callback function pointer
};

T_SERVER_ID ftms_add_service(void *p_func)
{
    T_SERVER_ID service_id;
    if (false == server_add_service(&service_id,
                                    (uint8_t *)ftms_att_tbl,
                                    ftms_attr_tbl_size,
                                    ftms_cbs))
    {
        PROFILE_PRINT_ERROR1("ftms_add_service: service_id %d", service_id);
        service_id = 0xff;
        return service_id;
    }

    ftms_var.ftms_srv_char_tbl = srv_uuid_create_tbl(ftms_att_tbl, ftms_attr_tbl_size);
#if FTMS_CHAR_FTMS_CTRL_PNT_SUPPORT
    ftms_var.control_permission = false;
    ftms_var.ftms_control_point.cur_length = 0;
    ftms_var.ftms_control_point.param[0] = FTMS_CP_OPCODE_RESERVED;
#endif
#if FTMS_CHAR_TREADMILL_DATA_SUPPORT
    memset(&ftms_var.ftms_treadmill_notify_flag, 0, sizeof(T_FTMS_NOTIFY_FLAG));
#endif
#if FTMS_CHAR_STEP_CLIMBER_DATA_SUPPORT
    memset(&ftms_var.ftms_step_climber_notify_flag, 0, sizeof(T_FTMS_NOTIFY_FLAG));
#endif
#if FTMS_CHAR_ROWER_DATA_SUPPORT
    memset(&ftms_var.ftms_rower_notify_flag, 0, sizeof(T_FTMS_NOTIFY_FLAG));
#endif
#if FTMS_CHAR_INDOOR_BIKE_DATA_SUPPORT
    memset(&ftms_var.ftms_indoor_bike_notify_flag, 0, sizeof(T_FTMS_NOTIFY_FLAG));
#endif
#if FTMS_CHAR_CROSS_TRAINER_DATA_SUPPORT
    memset(&ftms_var.ftms_cross_trainer_notify_flag, 0, sizeof(T_FTMS_NOTIFY_FLAG));
#endif
#if FTMS_CHAR_STAIR_CLIMBER_DATA_SUPPORT
    memset(&ftms_var.ftms_stair_climber_notify_flag, 0, sizeof(T_FTMS_NOTIFY_FLAG));
#endif

    pfn_ftms_cb = (P_FUN_SERVER_GENERAL_CB)p_func;

    return service_id;
}

/**
 * @brief       Prepare a new record in database.
 * @return void.
 *
 */
