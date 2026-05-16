#ifndef _FTMS_DEFINE_H_
#define _FTMS_DEFINE_H_

/** @defgroup Fitness Machine Service define file
  * @brief Fitness Machine Service define file
  * @{
  */
#include <stdint.h>
#include <stdbool.h>
#include "ftms_config.h"

/*============================================================================*
 *                         Macros
 *============================================================================*/
/** @defgroup FTMS_Common_Exported_Macros FTMS Common Exported Macros
  * @{
  */

/** @details
 *
*/
/** @brief service related UUIDs. */

#define GATT_UUID_FITNESS_MACHINE                      0x1826
#define GATT_UUID_FITNESS_MACHINE_FEATURE              0x2ACC
#define GATT_UUID_TREADMILL_DATA                       0x2ACD
#define GATT_UUID_CROSS_TRAINER_DATA                   0x2ACE
#define GATT_UUID_STEP_CLIMBER_DATA                    0x2ACF
#define GATT_UUID_STAIR_CLIMBER_DATA                   0x2AD0
#define GATT_UUID_ROWER_DATA                           0x2AD1
#define GATT_UUID_INDOOR_BIKE_DATA                     0x2AD2
#define GATT_UUID_TRAINING_STATUS                      0x2AD3
#define GATT_UUID_SUPPORT_SPEED_RANGE                  0x2AD4
#define GATT_UUID_SUPPORT_INCLINATION_RANGE            0x2AD5
#define GATT_UUID_SUPPORT_RESISTANCE_LEVEL_RANGE       0x2AD6
#define GATT_UUID_SUPPORT_HEART_RATE_RANGE             0x2AD7
#define GATT_UUID_SUPPORT_POWER_RANGE                  0x2AD8
#define GATT_UUID_FTMS_CONTROL_POINT                   0x2AD9
#define GATT_UUID_FITNESS_MACHINE_STATUS               0x2ADA

/** @defgroup FTMS_Control_Point FTMS Control Point
  * @brief  Control Point
  * @{
  */

/** @defgroup FTMS_Control_Point_OpCodes FTMS Control Point OpCodes
  * @brief  Control Point OpCodes
  * @{
  */

#define FTMS_CP_OPCODE_REQUEST_CONTROL                       0x00  /* Request Control */
#define FTMS_CP_OPCODE_RESET                                 0x01  /* Reset */
#define FTMS_CP_OPCODE_SET_TGT_SPEED                         0x02  /* Set Target Speed */
#define FTMS_CP_OPCODE_SET_TGT_INCLINATION                   0x03  /* Set Target Inclination */
#define FTMS_CP_OPCODE_SET_TGT_RESISTANCE_LEVEL              0x04  /* Set Target Resistance Level */
#define FTMS_CP_OPCODE_SET_TGT_POWER                         0x05  /* Set Target Power */
#define FTMS_CP_OPCODE_SET_TGT_HEART_RATE                    0x06  /* Set Target Heart Rate */
#define FTMS_CP_OPCODE_START_RESUME                          0x07  /* Start or Resume */
#define FTMS_CP_OPCODE_STOP_PAUSE                            0x08  /* Stop or Pause */
#define FTMS_CP_OPCODE_SET_TGT_EXPEND_ENERGY                 0x09  /* Set Targeted Expended Energy */
#define FTMS_CP_OPCODE_SET_TGT_STEP_NUM                      0x0A  /* Set Targeted Number of Steps */
#define FTMS_CP_OPCODE_SET_TGT_STRIDE_NUM                    0x0B  /* Set Targeted Number of Strides */
#define FTMS_CP_OPCODE_SET_TGT_DISTANCE                      0x0C  /* Set Targeted Distance */
#define FTMS_CP_OPCODE_SET_TGT_TRAINING_TIME                 0x0D  /* Set Targeted Training Time */
#define FTMS_CP_OPCODE_SET_TGT_TWO_HR_ZONES_TIME             0x0E  /* Set Targeted Time in Two Heart Rate Zones */
#define FTMS_CP_OPCODE_SET_TGT_THREE_HR_ZONES_TIME           0x0F  /* Set Targeted Time in Three Heart Rate Zones */
#define FTMS_CP_OPCODE_SET_TGT_FIVE_HR_ZONES_TIME            0x10  /* Set Targeted Time in Five Heart Rate Zones */
#define FTMS_CP_OPCODE_SET_INDOOR_BIKE_SIMULATION_PARAM      0x11  /* Set Indoor Bike Simulation Parameters */
#define FTMS_CP_OPCODE_SET_WHEEL_CIRCUMFERENCE               0x12  /* Set Wheel Circumference */
#define FTMS_CP_OPCODE_SPIN_DOWN_CTRL                        0x13  /* Spin Down Control */
#define FTMS_CP_OPCODE_SET_TGT_CADENCE                       0x14  /* Set Targeted Cadence */
#define FTMS_CP_OPCODE_RESPONSE_CODE                         0x80  /* Response Code */
#define FTMS_CP_OPCODE_RESERVED                              0xFF  /* Reserved for Future Use */
/** @} */

/** @defgroup FTMS_Control_Point_Response_Codes FTMS Control Point Response Codes
  * @brief  Control Point Response Codes
  * @{
  */

#define FTMS_CP_RSPCODE_RESERVED                               0x00
#define FTMS_CP_RSPCODE_SUCCESS                                0x01
#define FTMS_CP_RSPCODE_OPCODE_NOT_SUPPORT                     0x02
#define FTMS_CP_RSPCODE_INVALID_PARAMETER                      0x03
#define FTMS_CP_RSPCODE_OPERATION_FAILED                       0x04
#define FTMS_CP_RSPCODE_CONTROL_NOT_PERMITTED                  0x05

/** @} */

///@cond
/** @brief  Judge FTMS Control Point operation is available or not. */
#define FTMS_CTL_PNT_OPERATE_ACTIVE(x)                     \
    ( (x == FTMS_CP_OPCODE_REQUEST_CONTROL)  || \
      (x == FTMS_CP_OPCODE_RESET)  || \
      (x == FTMS_CP_OPCODE_SET_TGT_SPEED) || \
      (x == FTMS_CP_OPCODE_SET_TGT_INCLINATION) || \
      (x == FTMS_CP_OPCODE_SET_TGT_RESISTANCE_LEVEL) || \
      (x == FTMS_CP_OPCODE_SET_TGT_POWER) || \
      (x == FTMS_CP_OPCODE_SET_TGT_HEART_RATE) || \
      (x == FTMS_CP_OPCODE_START_RESUME) || \
      (x == FTMS_CP_OPCODE_STOP_PAUSE) || \
      (x == FTMS_CP_OPCODE_SET_TGT_EXPEND_ENERGY) || \
      (x == FTMS_CP_OPCODE_SET_TGT_STEP_NUM) || \
      (x == FTMS_CP_OPCODE_SET_TGT_STRIDE_NUM) || \
      (x == FTMS_CP_OPCODE_SET_TGT_DISTANCE) || \
      (x == FTMS_CP_OPCODE_SET_TGT_TRAINING_TIME) || \
      (x == FTMS_CP_OPCODE_SET_TGT_TWO_HR_ZONES_TIME) || \
      (x == FTMS_CP_OPCODE_SET_TGT_THREE_HR_ZONES_TIME) || \
      (x == FTMS_CP_OPCODE_SET_TGT_FIVE_HR_ZONES_TIME) || \
      (x == FTMS_CP_OPCODE_SET_INDOOR_BIKE_SIMULATION_PARAM) || \
      (x == FTMS_CP_OPCODE_SET_TGT_CADENCE) || \
      (x == FTMS_CP_OPCODE_RESPONSE_CODE) )
///@endcond

/** @defgroup FTMS_Status_OpCodes FTMS Status OpCodes
  * @brief  Status OpCodes
  * @{
  */

#define FTMS_STATUS_OP_RESERVED                      0x00
#define FTMS_STATUS_OP_RESET                         0x01  /* Reset */
#define FTMS_STATUS_OP_STOP_PAUSE_BY_USER            0x02  /* Fitness Machine Stopped or Paused by the User */
#define FTMS_STATUS_OP_STOP_BY_SAFETY_KEY            0x03  /* Fitness Machine Stopped by Safety Key */
#define FTMS_STATUS_OP_START_RESUME_BY_USER          0x04  /* Fitness Machine Started or Resumed by the User */
#define FTMS_STATUS_OP_TGT_SPEED_CHANGE              0x05  /* Target Speed Changed */
#define FTMS_STATUS_OP_TGT_INCLINE_CHANGE            0x06  /* Target Incline Changed */
#define FTMS_STATUS_OP_TGT_RESISTANCE_LEVEL_CHANGE   0x07  /* Target Resistance Level Changed */
#define FTMS_STATUS_OP_TGT_POWER_CHANGE              0x08  /* Target Power Changed */
#define FTMS_STATUS_OP_TGT_HR_CHANGE                 0x09  /* Target Heart Rate Changed */
#define FTMS_STATUS_OP_TGT_EXPEND_ENERGY_CHANGE      0x0A  /* Targeted Expended Energy Changed */
#define FTMS_STATUS_OP_TGT_STEP_NUM_CHANGE           0x0B  /* Targeted Number of Steps Changed */
#define FTMS_STATUS_OP_TGT_STRIDE_NUM_CHANGE         0x0C  /* Targeted Number of Strides Changed */
#define FTMS_STATUS_OP_TGT_DISTANCE_CHANGE           0x0D  /* Targeted Distance Changed */
#define FTMS_STATUS_OP_TGT_TRAIN_TIME_CHANGE         0x0E  /* Targeted Training Time Changed */
#define FTMS_STATUS_OP_TWO_HR_ZONE_TIME_CHANGE       0x0F  /* Targeted Time in Two Heart Rate Zones Changed */
#define FTMS_STATUS_OP_THREE_HR_ZONE_TIME_CHANGE     0x10  /* Targeted Time in Three Heart Rate Zones Changed */
#define FTMS_STATUS_OP_FIVE_HR_ZONE_TIME_CHANGE      0x11  /* Targeted Time in Five Heart Rate Zones Changed */
#define FTMS_STATUS_OP_IB_SIMULATE_PARAM_CHANGE      0x12  /* Indoor Bike Simulation Parameters Changed */
#define FTMS_STATUS_OP_WHEEL_CIRCUMFERENCE_CHANGE    0x13  /* Wheel Circumference Changed */
#define FTMS_STATUS_OP_SPIN_DOWN_STATUS              0x14  /* Spin Down Status */
#define FTMS_STATUS_OP_TGT_CADENCE_CHANGE            0x15  /* Targeted Cadence Changed */
#define FTMS_STATUS_OP_CONTROL_PERMISSION_LOST       0xFF  /* Control Permission Lost */

/** @} */

/** @defgroup FTMS_Training_Status_Flags FTMS Training Status Flags
  * @brief  Training Status Flags
  * @{
  */

#define FTMS_TS_STRING_PRESENT_MASK                 (1)       /* Training Status String present */
#define FTMS_TS_EXTENDED_STRING_PRESENT_MASK        (1<<1)    /* Extended String present */

/** @} */

/** @defgroup FTMS_Training_Status_Values FTMS Training Status Values
  * @brief  Training Status Values
  * @{
  */

#define FTMS_TRAIN_STATUS_OTHER                        0x00  /* Other */
#define FTMS_TRAIN_STATUS_IDLE                         0x01  /* Idle */
#define FTMS_TRAIN_STATUS_WARMING_UP                   0x02  /* Warming Up */
#define FTMS_TRAIN_STATUS_LOW_INTENSITY_INTERVAL       0x03  /* Low Intensity Interval */
#define FTMS_TRAIN_STATUS_HIGH_INTENSITY_INTERVAL      0x04  /* High Intensity Interval */
#define FTMS_TRAIN_STATUS_RECOVERY_INTERVAL            0x05  /* Recovery Interval */
#define FTMS_TRAIN_STATUS_ISOMETRIC                    0x06  /* Isometric */
#define FTMS_TRAIN_STATUS_HEART_RATE_CTRL              0x07  /* Heart Rate Control */
#define FTMS_TRAIN_STATUS_FITNESS_TEST                 0x08  /* Fitness Test */
#define FTMS_TRAIN_STATUS_SPEED_OUT_CTRL_REGION_LOW    0x09  /* Speed Outside of Control Region - Low */
#define FTMS_TRAIN_STATUS_SPEED_OUT_CTRL_REGION_HIGH   0x0A  /* Speed Outside of Control Region - High */
#define FTMS_TRAIN_STATUS_COOL_DOWN                    0x0B  /* Cool Down */
#define FTMS_TRAIN_STATUS_WATT_CTRL                    0x0C  /* Watt Control */
#define FTMS_TRAIN_STATUS_MANUAL_MODE                  0x0D  /* Manual Mode (Quick Start) */
#define FTMS_TRAIN_STATUS_PRE_WORKOUT                  0x0E  /* Pre-Workout */
#define FTMS_TRAIN_STATUS_POST_WORKOUT                 0x0F  /* Post-Workout */
#define FTMS_TRAIN_STATUS_RESERVED                     0xFF

/** @} */

/** @defgroup FTMS_Feature_Field_Flag_Bit_Mask FTMS Feature Field Flag Bit Mask
  * @brief  FTMS feature field 'Flags' bit mask.
  * @{
  */

#define FTMS_FEAT_AVERAGE_SPEED_SPT_MASK             (1)        /* Average Speed Supported */
#define FTMS_FEAT_CADENCE_SPT_MASK                   (1<<1)     /* Cadence Supported */
#define FTMS_FEAT_TOTAL_DISTANCE_SPT_MASK            (1<<2)     /* Total Distance Supported */
#define FTMS_FEAT_INCLINATION_SPT_MASK               (1<<3)     /* Inclination Supported */
#define FTMS_FEAT_ELEVATION_GAIN_SPT_MASK            (1<<4)     /* Elevation Gain Supported */
#define FTMS_FEAT_PACE_SPT_MASK                      (1<<5)     /* Pace Supported */
#define FTMS_FEAT_STEP_COUNT_SPT_MASK                (1<<6)     /* Step Count Supported */
#define FTMS_FEAT_RESISTANCE_LEVEL_SPT_MASK          (1<<7)     /* Resistance Level Supported */
#define FTMS_FEAT_STRIDE_COUNT_SPT_MASK              (1<<8)     /* Stride Count Supported */
#define FTMS_FEAT_EXPENDED_ENERGY_SPT_MASK           (1<<9)     /* Expended Energy Supported */
#define FTMS_FEAT_HR_MEASUREMENT_SPT_MASK            (1<<10)    /* Heart Rate Measurement Supported */
#define FTMS_FEAT_METABOLIC_EQUIVALENT_SPT_MASK      (1<<11)    /* Metabolic Equivalent Supported */
#define FTMS_FEAT_ELAPSED_TIME_SPT_MASK              (1<<12)    /* Elapsed Time Supported */
#define FTMS_FEAT_REMAIN_TIME_SPT_MASK               (1<<13)    /* Remaining Time Supported */
#define FTMS_FEAT_POWER_MEASUREMENT_SPT_MASK         (1<<14)    /* Power Measurement Supported */
#define FTMS_FEAT_BELT_FORCE_POWER_OUTPUT_SPT_MASK   (1<<15)    /* Force on Belt and Power Output Supported */
#define FTMS_FEAT_USER_DATA_RETENTION_SPT_MASK       (1<<16)    /* User Data Retention Supported */
#define FTMS_FEAT_ALL_FEATURE_SUPPORTED              (0x1FFFF)  /* FTMS Support all features */

/** @} */

/** @defgroup FTMS_Target_Setting_Field_Flag_Bit_Mask FTMS Target Setting Field Flag Bit Mask
  * @brief  FTMS target setting field 'Flags' bit mask.
  * @{
  */

#define FTMS_TGT_SET_SPEED_SPT_MASK                  (1)        /* Speed Target Setting Supported */
#define FTMS_TGT_SET_INCLINATION_SPT_MASK            (1<<1)     /* Inclination Target Setting Supported */
#define FTMS_TGT_SET_RESISTANCE_SPT_MASK             (1<<2)     /* Resistance Target Setting Supported */
#define FTMS_TGT_SET_POWER_SPT_MASK                  (1<<3)     /* Power Target Setting Supported */
#define FTMS_TGT_SET_HR_SPT_MASK                     (1<<4)     /* Heart Rate Target Setting Supported */
#define FTMS_TGT_EXPENDED_ENERGY_CONFIG_SPT_MASK     (1<<5)     /* Targeted Expended Energy Configuration Supported */
#define FTMS_TGT_STEP_NUM_CONFIG_SPT_MASK            (1<<6)     /* Targeted Step Number Configuration Supported */
#define FTMS_TGT_STRIDE_NUM_CONFIG_SPT_MASK          (1<<7)     /* Targeted Stride Number Configuration Supported */
#define FTMS_TGT_DISTANCE_CONFIG_SPT_MASK            (1<<8)     /* Targeted Distance Configuration Supported */
#define FTMS_TGT_TRAINING_TIME_CONFIG_SPT_MASK       (1<<9)     /* Targeted Training Time Configuration Supported */
#define FTMS_TGT_TWO_HR_ZONES_TIME_CONFIG_SPT_MASK   (1<<10)    /* Targeted Time in Two Heart Rate Zones Configuration Supported */
#define FTMS_TGT_THREE_HR_ZONES_CONFIG_SPT_MASK      (1<<11)    /* Targeted Time in Three Heart Rate Zones Configuration Supported */
#define FTMS_TGT_FIVE_HR_ZONES_CONFIG_SPT_MASK       (1<<12)    /* Targeted Time in Five Heart Rate Zones Configuration Supported */
#define FTMS_INDOOR_BIKE_SIMULATION_PARAM_SPT_MASK   (1<<13)    /* Indoor Bike Simulation Parameters Supported */
#define FTMS_WHEEL_CIRCUMFERENCE_CONFIG_SPT_MASK     (1<<14)    /* Wheel Circumference Configuration Supported */
#define FTMS_SPIN_DOWN_CTL_SPT_MASK                  (1<<15)    /* Spin Down Control Supported */
#define FTMS_TGT_CADENCE_CONFIG_SPT_MASK             (1<<16)    /* Targeted Cadence Configuration Supported */
#define FTMS_TGT_SET_ALL_FEATURE_SUPPORTED           (0x1FFFF)  /* FTMS Support all features */

/** @} */

/** @defgroup FTMS_Treadmill_Data_Flag_Bit_Mask FTMS Treadmill Data Flag Bit Mask
  * @brief  FTMS treadmill data 'Flags' bit mask.
  * @{
  */
#if FTMS_CHAR_TREADMILL_DATA_SUPPORT
#define FTMS_TM_MORE_DATA_MASK                            (1)        /* More Data */
#define FTMS_TM_AVE_SPEED_PRESENT_MASK                    (1<<1)     /* Average Speed Present */
#define FTMS_TM_TOTAL_DISTANCE_PRESENT_MASK               (1<<2)     /* Total Distance Present */
#define FTMS_TM_INCLINATION_RAMP_ANGLE_SET_PRESENT_MASK   (1<<3)     /* Inclination and Ramp Angle Setting Present */
#define FTMS_TM_ELEVATION_GAIN_PRESENT_MASK               (1<<4)     /* Elevation Gain Present */
#define FTMS_TM_INSTANTANEOUS_PACE_PRESENT_MASK           (1<<5)     /* Instantaneous Pace Present */
#define FTMS_TM_AVERAGE_PACE_PRESENT_MASK                 (1<<6)     /* Average Pace Present */
#define FTMS_TM_EXPENDED_ENERGY_PRESENT_MASK              (1<<7)     /* Expended Energy Present */
#define FTMS_TM_HEART_RATE_PRESENT_MASK                   (1<<8)     /* Heart Rate Present */
#define FTMS_TM_METABOLIC_EQUIVALENT_PRESENT_MASK         (1<<9)     /* Metabolic Equivalent Present */
#define FTMS_TM_ELAPSED_TIME_PRESENT_MASK                 (1<<10)    /* Elapsed Time Present */
#define FTMS_TM_REMAIN_TIME_PRESENT_MASK                  (1<<11)    /* Remaining Time Present */
#define FTMS_TM_BELT_FORCE_POWER_OUTPUT_PRESENT_MASK      (1<<12)    /* Force on Belt and Power Output Present */
#define FTMS_TM_SET_ALL_FEATURE_SUPPORT                   (0x1FFE)   /* Support all treadmill data features */
#define FTMS_TM_NO_MORE_DATA_PRESENT                      (0xFFFE)
#endif
/** @} */

/** @defgroup FTMS_Step_Climber_Data_Flag_Bit_Mask FTMS Step Climber Data Flag Bit Mask
  * @brief  FTMS step climber data 'Flags' bit mask.
  * @{
  */
#if FTMS_CHAR_STEP_CLIMBER_DATA_SUPPORT
#define FTMS_STEPC_MORE_DATA_MASK                         (1)       /* More Data */
#define FTMS_STEPC_STEP_PER_MINUTE_PRESENT_MASK           (1<<1)    /* Step per Minute present */
#define FTMS_STEPC_AVE_STEP_RATE_PRESENT_MASK             (1<<2)    /* Average Step Rate Present */
#define FTMS_STEPC_POSITIVE_ELEVATION_GAIN_PRESENT_MASK   (1<<3)    /* Positive Elevation Gain Present */
#define FTMS_STEPC_EXPEND_ENERGY_PRESENT_MASK             (1<<4)    /* Expended Energy Present */
#define FTMS_STEPC_HEART_RATE_PRESENT_MASK                (1<<5)    /* Heart Rate Present */
#define FTMS_STEPC_METABOLIC_EQUIVALENT_PRESENT_MASK      (1<<6)    /* Metabolic Equivalent Present */
#define FTMS_STEPC_ELAPSED_TIME_PRESENT_MASK              (1<<7)    /* Elapsed Time Present */
#define FTMS_STEPC_REMAIN_TIME_PRESENT_MASK               (1<<8)    /* Remaining Time Present */
#define FTMS_STEPC_SET_ALL_FEATURE_SUPPORT                (0x01FE)  /* Support all step climber data features */
#define FTMS_STEPC_NO_MORE_DATA_PRESENT                   (0xFFFE)
#endif
/** @} */

/** @defgroup FTMS_Rower_Data_Flag_Bit_Mask FTMS Rower Data Flag Bit Mask
  * @brief  FTMS rower data 'Flags' bit mask.
  * @{
  */
#if FTMS_CHAR_ROWER_DATA_SUPPORT
#define FTMS_RW_MORE_DATA_MASK                            (1)      /* More Data */
#define FTMS_RW_AVE_STROKE_RATE_PRESENT_MASK              (1<<1)   /* Average Stroke Rate present */
#define FTMS_RW_TOTAL_DISTANCE_PRESENT_MASK               (1<<2)   /* Total Distance Present */
#define FTMS_RW_INSTANTANEOUS_PACE_PRESENT_MASK           (1<<3)   /* Instantaneous Pace Present */
#define FTMS_RW_AVE_PACE_PRESENT_MASK                     (1<<4)   /* Average Pace Present */
#define FTMS_RW_INSTANTANEOUS_POWER_PRESENT_MASK          (1<<5)   /* Instantaneous Power Present */
#define FTMS_RW_AVE_POWER_PRESENT_MASK                    (1<<6)   /* Average Power Present */
#define FTMS_RW_RESISTANCE_LEVEL_MASK                     (1<<7)   /* Resistance Level */
#define FTMS_RW_EXPEND_ENERGY_PRESENT_MASK                (1<<8)   /* Expended Energy Present */
#define FTMS_RW_HEART_RATE_PRESENT_MASK                   (1<<9)   /* Heart Rate Present */
#define FTMS_RW_METABOLIC_EQUIVALENT_PRESENT_MASK         (1<<10)  /* Metabolic Equivalent Present */
#define FTMS_RW_ELAPSED_TIME_PRESENT_MASK                 (1<<11)  /* Elapsed Time Present */
#define FTMS_RW_REMAIN_TIME_PRESENT_MASK                  (1<<12)  /* Remaining Time Present */
#define FTMS_RW_SET_ALL_FEATURE_SUPPORT                   (0x1FFE) /* Support all rower data features */
#define FTMS_RW_NO_MORE_DATA_PRESENT                      (0xFFFE)
#endif
/** @} */

/** @defgroup FTMS_Indoor_Bike_Data_Flag_Bit_Mask FTMS Indoor Bike Data Flag Bit Mask
  * @brief  FTMS indoor bike data 'Flags' bit mask.
  * @{
  */
#if FTMS_CHAR_INDOOR_BIKE_DATA_SUPPORT
#define FTMS_IB_MORE_DATA_MASK                            (1)      /* More Data */
#define FTMS_IB_AVE_SPEED_PRESENT_MASK                    (1<<1)   /* Average Speed present */
#define FTMS_IB_INSTANTANEOUS_CADENCE_MASK                (1<<2)   /* Instantaneous Cadence */
#define FTMS_IB_AVE_CADENCE_PRESENT_MASK                  (1<<3)   /* Average Cadence present */
#define FTMS_IB_TOTAL_DISTANCE_PRESENT_MASK               (1<<4)   /* Total Distance Present */
#define FTMS_IB_RESISTANCE_LEVEL_PRESENT_MASK             (1<<5)   /* Resistance Level Present */
#define FTMS_IB_INSTANTANEOUS_POWER_PRESENT_MASK          (1<<6)   /* Instantaneous Power Present */
#define FTMS_IB_AVE_POWER_PRESENT_MASK                    (1<<7)   /* Average Power Present */
#define FTMS_IB_EXPEND_ENERGY_PRESENT_MASK                (1<<8)   /* Expended Energy Present */
#define FTMS_IB_HEART_RATE_PRESENT_MASK                   (1<<9)   /* Heart Rate Present */
#define FTMS_IB_METABOLIC_EQUIVALENT_PRESENT_MASK         (1<<10)  /* Metabolic Equivalent Present */
#define FTMS_IB_ELAPSED_TIME_PRESENT_MASK                 (1<<11)  /* Elapsed Time Present */
#define FTMS_IB_REMAIN_TIME_PRESENT_MASK                  (1<<12)  /* Remaining Time Present */
#define FTMS_IB_SET_ALL_FEATURE_SUPPORT                   (0x1FFE) /* Support all indoor bike data features */
#define FTMS_IB_NO_MORE_DATA_PRESENT                      (0xFFFE)
#endif
/** @} */

/** @defgroup FTMS_Cross_Trainer_Data_Flag_Bit_Mask FTMS Cross Trainer Data Flag Bit Mask
  * @brief  FTMS cross trainer data 'Flags' bit mask.
  * @{
  */
#if FTMS_CHAR_CROSS_TRAINER_DATA_SUPPORT
#define FTMS_CT_MORE_DATA_MASK                            (1)      /* More Data */
#define FTMS_CT_AVE_SPEED_PRESENT_MASK                    (1<<1)   /* Average Speed present */
#define FTMS_CT_TOTAL_DISTANCE_PRESENT_MASK               (1<<2)   /* Total Distance Present */
#define FTMS_CT_STEP_COUNT_PRESENT_MASK                   (1<<3)   /* Step Count Present */
#define FTMS_CT_STRIDE_COUNT_PRESENT_MASK                 (1<<4)   /* Stride Count Present */
#define FTMS_CT_ELEVATION_GAIN_PRESENT_MASK               (1<<5)   /* Elevation Gain Present */
#define FTMS_CT_INCLINATION_RAMP_ANGLE_SET_PRESENT_MASK   (1<<6)   /* Inclination and Ramp Angle Setting Present */
#define FTMS_CT_RESISTANCE_LEVEL_PRESENT_MASK             (1<<7)   /* Resistance Level Present */
#define FTMS_CT_INSTANTANEOUS_POWER_PRESENT_MASK          (1<<8)   /* Instantaneous Power Present */
#define FTMS_CT_AVE_POWER_PRESENT_MASK                    (1<<9)   /* Average Power Present */
#define FTMS_CT_EXPEND_ENERGY_PRESENT_MASK                (1<<10)  /* Expended Energy Present */
#define FTMS_CT_HEART_RATE_PRESENT_MASK                   (1<<11)  /* Heart Rate Present */
#define FTMS_CT_METABOLIC_EQUIVALENT_PRESENT_MASK         (1<<12)  /* Metabolic Equivalent Present */
#define FTMS_CT_ELAPSED_TIME_PRESENT_MASK                 (1<<13)  /* Elapsed Time Present */
#define FTMS_CT_REMAIN_TIME_PRESENT_MASK                  (1<<14)  /* Remaining Time Present */
#define FTMS_CT_MOVEMENT_DIRECTION_MASK                   (1<<15)  /* Movement Direction, 0 is Forward, 1 is Backward */
#define FTMS_CT_SET_ALL_FEATURE_SUPPORT                   (0x00FFFE) /* Support all cross trainer data features */
#define FTMS_CT_NO_MORE_DATA_PRESENT                      (0xFFFFFE)
#endif
/** @} */

/** @defgroup FTMS_Stair_Climber_Data_Flag_Bit_Mask FTMS Stair Climber Data Flag Bit Mask
  * @brief  FTMS stair climber data 'Flags' bit mask.
  * @{
  */
#if FTMS_CHAR_STAIR_CLIMBER_DATA_SUPPORT
#define FTMS_STAIRC_MORE_DATA_MASK                          (1)      /* More Data */
#define FTMS_STAIRC_STEP_PER_MINUTE_PRESENT_MASK            (1<<1)   /* Step per Minute present */
#define FTMS_STAIRC_AVE_STEP_RATE_PRESENT_MASK              (1<<2)   /* Average Step Rate Present */
#define FTMS_STAIRC_POSITIVE_ELEVATION_GAIN_PRESENT_MASK    (1<<3)   /* Positive Elevation Gain Present */
#define FTMS_STAIRC_STRIDE_COUNT_PRESENT_MASK               (1<<4)   /* Stride Count Present */
#define FTMS_STAIRC_EXPEND_ENERGY_PRESENT_MASK              (1<<5)   /* Expended Energy Present */
#define FTMS_STAIRC_HEART_RATE_PRESENT_MASK                 (1<<6)   /* Heart Rate Present */
#define FTMS_STAIRC_METABOLIC_EQUIVALENT_PRESENT_MASK       (1<<7)   /* Metabolic Equivalent Present */
#define FTMS_STAIRC_ELAPSED_TIME_PRESENT_MASK               (1<<8)   /* Elapsed Time Present */
#define FTMS_STAIRC_REMAIN_TIME_PRESENT_MASK                (1<<9)   /* Remaining Time Present */
#define FTMS_STAIRC_SET_ALL_FEATURE_SUPPORT                 (0x03FE) /* Support all indoor bike data features */
#define FTMS_STAIRC_NO_MORE_DATA_PRESENT                    (0xFFFE)
#endif
/** @} */

/** @} End of FTMS_Common_Exported_Macros */

/*============================================================================*
 *                         Types
 *============================================================================*/
/** @defgroup FTMS_Exported_Types FTMS Exported Types
  * @brief
  * @{
  */

/**
*  @brief Fitness Machine Service parameter type
*/

typedef enum
{
    FTMS_CTRL_RESERVED = 0x00,
    FTMS_CTRL_STOP,             /* Control point write to initiate stop procedure */
    FTMS_CTRL_PAUSE             /* Control point write to initiate pause procedure */
} T_FTMS_CTRL_INFO;

typedef struct
{
    uint16_t fat_burn_zone_time;  /* Targeted time in fat burn zone */
    uint16_t fitness_zones_time;  /* Targeted time in fitness zone */
} T_FTMS_TWO_ZONES_HR_TIME;

typedef struct
{
    uint16_t light_zone_time;     /* Targeted time in light zone */
    uint16_t moderate_zone_time;  /* Targeted time in moderate zone */
    uint16_t hard_zone_time;      /* Targeted time in hard zone */
} T_FTMS_THREE_ZONES_HR_TIME;

typedef struct
{
    uint16_t very_light_zone_time;  /* Targeted time in very light zone */
    uint16_t light_zone_time;       /* Targeted time in light zone */
    uint16_t moderate_zone_time;    /* Targeted time in moderate zone */
    uint16_t hard_zone_time;        /* Targeted time in hard zone */
    uint16_t maximum_zone_time;     /* Targeted time in maximum zone */
} T_FTMS_FIVE_ZONES_HR_TIME;

typedef struct
{
    int16_t wind_speed;                      /* Wind speed: Meters Per Second (mps) with a resolution of 0.001 */
    int16_t grade;                           /* Grade: Percentage with a resolution of 0.01 */
    uint8_t rolling_resistance_coefficient;  /* Crr(Coefficient of rolling resistance): Unitless with a resolution of 0.001 */
    uint8_t wind_resistance_coefficient;     /* Cw(Wind resistance coefficient): Kilogram per Meter(Kg/m) with a resolution of 0.01 */
} T_FTMS_INDOOR_BIKE_SIMULATION_PARAM;

typedef enum
{
    FTMS_SD_CTRL_RESERVED = 0x00,
    FTMS_SD_CTRL_START,            /* Write to start spin down control procedure */
    FTMS_SD_CTRL_IGNORE            /* Write to ignore spin down control procedure */
} T_FTMS_SPIN_DOWN_CTRL_PARAM;

typedef struct
{
    uint16_t target_speed_low;    /* km/h with a resolution of 0.01 km/h */
    uint16_t target_speed_high;   /* km/h with a resolution of 0.01 km/h */
} T_FTMS_SPIN_DOWN_RESP_PARAM;

typedef enum
{
    FTMS_SD_STATUS_RESERVED = 0x00,   /* Reserved for Future Use */
    FTMS_SD_REQ,                      /* Spin Down Requested */
    FTMS_SD_SUCCESS,                  /* Success */
    FTMS_SD_ERROR,                    /* Error */
    FTMS_SD_STOP_PEDAL                /* Stop Pedaling */
} T_FTMS_SPIN_DOWN_STATUS_VALUE;

typedef struct
{
    uint32_t features_field;           /* Fitness Machine Features */
    uint32_t target_setting_field;     /* Target Setting Features */
} T_FTMS_FEATURE;

#if FTMS_CHAR_TRAINING_STATUS_SUPPORT
typedef struct
{
    uint8_t flag;                      /* Training Status Flags */
    uint8_t train_status;              /* Training Status */
    uint8_t *train_status_string;      /* Training Status String (if present) */
} T_FTMS_TRAIN_STATUS;
#endif

#if FTMS_CHAR_SUPPORT_SPEED_RANGE_SUPPORT
typedef struct
{
    uint16_t min_speed;             /* Kilometer per hour with a resolution of 0.01 */
    uint16_t max_speed;             /* Kilometer per hour with a resolution of 0.01 */
    uint16_t min_speed_increment;   /* Kilometer per hour with a resolution of 0.01 */
} T_FTMS_SUPPORT_SPEED_RANGE;
#endif

#if FTMS_CHAR_SUPPORT_INCLINATION_RANGE_SUPPORT
typedef struct
{
    int16_t min_inclination;              /* Percent with a resolution of 0.1 */
    int16_t max_inclination;              /* Percent with a resolution of 0.1 */
    uint16_t min_inclination_increment;   /* Percent with a resolution of 0.1 */
} T_FTMS_SUPPORT_INCLINATION_RANGE;
#endif

#if FTMS_CHAR_SUPPORT_RESISTANCE_LEVEL_RANGE_SUPPORT
typedef struct
{
    int16_t min_resistance_level;             /* Unitless with a resolution of 0.1 */
    int16_t max_resistance_level;             /* Unitless with a resolution of 0.1 */
    uint16_t min_resistance_level_increment;  /* Unitless with a resolution of 0.1 */
} T_FTMS_SUPPORT_RESISTANCE_LEVEL_RANGE;
#endif

#if FTMS_CHAR_SUPPORT_POWER_RANGE_SUPPORT
typedef struct
{
    int16_t min_power;              /* Watt  with a resolution of 1 */
    int16_t max_power;              /* Watt  with a resolution of 1 */
    uint16_t min_power_increment;   /* Watt  with a resolution of 1 */
} T_FTMS_SUPPORT_POWER_RANGE;
#endif

#if FTMS_CHAR_SUPPORT_HR_RANGE_SUPPORT
typedef struct
{
    uint8_t min_heart_rate;     /* Beats per minute with a resolution of 1 */
    uint8_t max_heart_rate;     /* Beats per minute with a resolution of 1 */
    uint8_t min_hr_increment;   /* Beats per minute with a resolution of 1 */
} T_FTMS_SUPPORT_HR_RANGE;
#endif

#if FTMS_CHAR_TREADMILL_DATA_SUPPORT
typedef struct
{
    uint16_t instantaneous_speed;       /* Kilometer per hour with a resolution of 0.01 */
    uint16_t average_speed;             /* Kilometer per hour with a resolution of 0.01 */
    uint32_t total_distance;            /* Meters with a resolution of 1 */
    int16_t inclination;                /* Percent with a resolution of 0.1 */
    int16_t ramp_angle_set;             /* Degree with a resolution of 0.1 */
    uint16_t positive_elevation_gain;   /* Meters with a resolution of 0.1 */
    uint16_t negative_elevation_gain;   /* Meters with a resolution of 0.1 */
    uint8_t instantaneous_pace;         /* Kilometer per hour with a resolution of 0.01 */
    uint8_t average_pace;               /* Kilometer per minute with a resolution of 0.1 */
    uint16_t total_energy;              /* Kilo Calorie with a resolution of 1 */
    uint16_t energy_per_hour;           /* Kilo Calorie with a resolution of 1 */
    uint8_t energy_per_minute;          /* Kilo Calorie with a resolution of 1 */
    uint8_t heart_rate;                 /* Beats per minute with a resolution of 1 */
    uint8_t metabolic_equivalent;       /* Metabolic Equivalent with a resolution of 0.1 */
    uint16_t elapsed_time;              /* Second with a resolution of 1 */
    uint16_t remain_time;               /* Second with a resolution of 1 */
    int16_t force_on_belt;              /* Newton with a resolution of 1 */
    int16_t power_output;               /* Watts with a resolution of 1 */
} T_FTMS_TREADMILL_DATA;
#endif

#if FTMS_CHAR_STEP_CLIMBER_DATA_SUPPORT
typedef struct
{
    uint16_t floor;                     /* Unitless with a resolution of 1 */
    uint16_t step_count;                /* Unitless with a resolution of 1 */
    uint16_t step_per_minute;           /* Step/minute with a resolution of 1 */
    uint16_t average_step_rate;         /* Step/minute with a resolution of 1 */
    uint16_t positive_elevation_gain;   /* Meters with a resolution of 1 */
    uint16_t total_energy;              /* Kilo Calorie with a resolution of 1 */
    uint16_t energy_per_hour;           /* Kilo Calorie with a resolution of 1 */
    uint8_t energy_per_minute;          /* Kilo Calorie with a resolution of 1 */
    uint8_t heart_rate;                 /* Beats per minute with a resolution of 1 */
    uint8_t metabolic_equivalent;       /* Metabolic Equivalent with a resolution of 0.1 */
    uint16_t elapsed_time;              /* Second with a resolution of 1 */
    uint16_t remain_time;               /* Second with a resolution of 1 */
} T_FTMS_STEP_CLIMBER_DATA;
#endif

#if FTMS_CHAR_ROWER_DATA_SUPPORT
typedef struct
{
    uint8_t stroke_rate;            /* stroke/minute with a resolution of 0.5 */
    uint16_t stroke_count;          /* Unitless with a resolution of 1 */
    uint8_t average_stroke_rate;    /* 1/minute with a resolution of 0.5 */
    uint32_t total_distance;        /* Meters with a resolution of 1 (uint24) */
    uint16_t instantaneous_pace;    /* Second with a resolution of 1 */
    uint16_t average_pace;          /* Second with a resolution of 1 */
    int16_t instantaneous_power;    /* Watts with a resolution of 1 */
    int16_t average_power;          /* Watts with a resolution of 1 */
    int16_t resistance_level;       /* Unitless with a resolution of 1 */
    uint16_t total_energy;          /* Kilo Calorie with a resolution of 1 */
    uint16_t energy_per_hour;       /* Kilo Calorie with a resolution of 1 */
    uint8_t energy_per_minute;      /* Kilo Calorie with a resolution of 1 */
    uint8_t heart_rate;             /* Beats per minute with a resolution of 1 */
    uint8_t metabolic_equivalent;   /* Metabolic Equivalent with a resolution of 0.1 */
    uint16_t elapsed_time;          /* Second with a resolution of 1 */
    uint16_t remain_time;           /* Second with a resolution of 1 */
} T_FTMS_ROWER_DATA;
#endif

#if FTMS_CHAR_INDOOR_BIKE_DATA_SUPPORT
typedef struct
{
    uint16_t instantaneous_speed;      /* Kilometer per hour with a resolution of 0.01 */
    uint16_t average_speed;            /* Kilometer per hour with a resolution of 0.01 */
    uint16_t instantaneous_cadence;    /* 1/minute with a resolution of 0.5 */
    uint16_t average_cadence;          /* 1/minute with a resolution of 0.5 */
    uint32_t total_distance;           /* Meters with a resolution of 1 (uint24) */
    int16_t resistance_level;          /* Unitless with a resolution of 1 */
    int16_t instantaneous_power;       /* Watts with a resolution of 1 */
    int16_t average_power;             /* Watts with a resolution of 1 */
    uint16_t total_energy;             /* Kilo Calorie with a resolution of 1 */
    uint16_t energy_per_hour;          /* Kilo Calorie with a resolution of 1 */
    uint8_t energy_per_minute;         /* Kilo Calorie with a resolution of 1 */
    uint8_t heart_rate;                /* Beats per minute with a resolution of 1 */
    uint8_t metabolic_equivalent;      /* Metabolic Equivalent with a resolution of 0.1 */
    uint16_t elapsed_time;             /* Second with a resolution of 1 */
    uint16_t remain_time;              /* Second with a resolution of 1 */
} T_FTMS_INDOOR_BIKE_DATA;
#endif

#if FTMS_CHAR_CROSS_TRAINER_DATA_SUPPORT
typedef struct
{
    uint16_t instantaneous_speed;        /* Kilometer per hour with a resolution of 0.01 */
    uint16_t average_speed;              /* Kilometer per hour with a resolution of 0.01 */
    uint32_t total_distance;             /* Meters with a resolution of 1 (uint24) */
    uint16_t step_per_minute;            /* Step/minute with a resolution of 1 */
    uint16_t average_step_rate;          /* Step/minute with a resolution of 1 */
    uint16_t stride_count;               /* Unitless with a resolution of 0.1 */
    uint16_t positive_elevation_gain;    /* Meters with a resolution of 1 */
    uint16_t negative_elevation_gain;    /* Meters with a resolution of 1 */
    int16_t inclination;                 /* Percent with a resolution of 0.1 */
    int16_t ramp_angle_set;              /* Degree with a resolution of 0.1 */
    int16_t resistance_level;            /* Unitless with a resolution of 0.1 */
    int16_t instantaneous_power;         /* Watts with a resolution of 1 */
    int16_t average_power;               /* Watts with a resolution of 1 */
    uint16_t total_energy;               /* Kilo Calorie with a resolution of 1 */
    uint16_t energy_per_hour;            /* Kilo Calorie with a resolution of 1 */
    uint8_t energy_per_minute;           /* Kilo Calorie with a resolution of 1 */
    uint8_t heart_rate;                  /* Beats per minute with a resolution of 1 */
    uint8_t metabolic_equivalent;        /* Metabolic Equivalent with a resolution of 0.1 */
    uint16_t elapsed_time;               /* Second with a resolution of 1 */
    uint16_t remain_time;                /* Second with a resolution of 1 */
} T_FTMS_CROSS_TRAINER_DATA;
#endif

#if FTMS_CHAR_STAIR_CLIMBER_DATA_SUPPORT
typedef struct
{
    uint16_t floors;                     /* Unitless with a resolution of 1 */
    uint16_t step_per_minute;            /* Step/minute with a resolution of 1 */
    uint16_t average_step_rate;          /* Step/minute with a resolution of 1 */
    uint16_t positive_elevation_gain;    /* Meters with a resolution of 1 */
    uint16_t stride_count;               /* Unitless with a resolution of 1 */
    uint16_t total_energy;               /* Kilo Calorie with a resolution of 1 */
    uint16_t energy_per_hour;            /* Kilo Calorie with a resolution of 1 */
    uint16_t energy_per_minute;          /* Kilo Calorie with a resolution of 1 */
    uint8_t heart_rate;                  /* Beats per minute with a resolution of 1 */
    uint8_t metabolic_equivalent;        /* Metabolic Equivalent with a resolution of 0.1 */
    uint16_t elapsed_time;               /* Second with a resolution of 1 */
    uint16_t remain_time;                /* Second with a resolution of 1 */
} T_FTMS_STAIR_CLIMBER_DATA;
#endif

/** End of UDS_Exported_Types
* @}
*/

/** @} End of FTMS_DEFINE */

#endif /* _FTMS_DEFINE_H_ */
