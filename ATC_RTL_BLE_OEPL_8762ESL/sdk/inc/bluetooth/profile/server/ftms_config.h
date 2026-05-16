#ifndef _FTMS_CONFIG_H_
#define _FTMS_CONFIG_H_

/** @defgroup Fitness Machine Service configuration file
  * @brief Fitness Machine Service configuration file
  * @{
  */
#include <stdint.h>
#include <stdbool.h>

/*============================================================================*
 *                         Macros
 *============================================================================*/
/** @defgroup FTMS_Common_Exported_Macros FTMS Common Exported Macros
  * @{
  */

/** @details
 *
*/
/** @details
    Set FTMS_CHAR_TREADMILL_DATA_SUPPORT to 1 to support Treadmill Data characteristic,
    otherwise set it to 0.
*/
#define FTMS_CHAR_TREADMILL_DATA_SUPPORT                    1

/** @details
    Set FTMS_CHAR_STEP_CLIMBER_DATA_SUPPORT to 1 to support Step Climber Data characteristic,
    otherwise set it to 0.
*/
#define FTMS_CHAR_STEP_CLIMBER_DATA_SUPPORT                 1

/** @details
    Set FTMS_CHAR_ROWER_DATA_SUPPORT to 1 to support Rower Data characteristic,
    otherwise set it to 0.
*/
#define FTMS_CHAR_ROWER_DATA_SUPPORT                        1

/** @details
    Set FTMS_CHAR_INDOOR_BIKE_DATA_SUPPORT to 1 to support Indoor Bike Data characteristic,
    otherwise set it to 0.
*/
#define FTMS_CHAR_INDOOR_BIKE_DATA_SUPPORT                  1

/** @details
    Set FTMS_CHAR_CROSS_TRAINER_DATA_SUPPORT to 1 to support Cross Trainer Data characteristic,
    otherwise set it to 0.
*/
#define FTMS_CHAR_CROSS_TRAINER_DATA_SUPPORT                1

/** @details
    Set FTMS_CHAR_STAIR_CLIMBER_DATA_SUPPORT to 1 to support Cross Trainer Data characteristic,
    otherwise set it to 0.
*/
#define FTMS_CHAR_STAIR_CLIMBER_DATA_SUPPORT                1

/** @details
    Set FTMS_CHAR_INDOOR_BIKE_DATA_SUPPORT to 1 to support Indoor Bike Data characteristic,
    otherwise set it to 0.
*/
#define FTMS_CHAR_TRAINING_STATUS_SUPPORT                   1

/** @details
    Set FTMS_CHAR_SUPPORT_SPEED_RANGE_SUPPORT to 1 to support Supported Speed Range characteristic,
    otherwise set it to 0.
*/
#define FTMS_CHAR_SUPPORT_SPEED_RANGE_SUPPORT               1

/** @details
    Set FTMS_CHAR_SUPPORT_INCLINATION_RANGE_SUPPORT to 1 to support Supported Inclination Range
    characteristic, otherwise set it to 0.
*/
#define FTMS_CHAR_SUPPORT_INCLINATION_RANGE_SUPPORT         1

/** @details
    Set FTMS_CHAR_SUPPORT_RESISTANCE_LEVEL_RANGE_SUPPORT to 1 to support Supported Resistance Level
    Range characteristic, otherwise set it to 0.
*/
#define FTMS_CHAR_SUPPORT_RESISTANCE_LEVEL_RANGE_SUPPORT    1

/** @details
    Set FTMS_CHAR_SUPPORT_HR_RANGE_SUPPORT to 1 to support Supported Heart Rate Range characteristic,
    otherwise set it to 0.
*/
#define FTMS_CHAR_SUPPORT_HR_RANGE_SUPPORT                  1

/** @details
    Set FTMS_CHAR_SUPPORT_POWER_RANGE_SUPPORT to 1 to support Supported Power Range characteristic,
    otherwise set it to 0.
*/
#define FTMS_CHAR_SUPPORT_POWER_RANGE_SUPPORT               1

/** @details
    Set FTMS_CHAR_FTMS_CTRL_PNT_SUPPORT to 1 to support Fitness Machine Control Point characteristic,
    otherwise set it to 0.
*/
#define FTMS_CHAR_FTMS_CTRL_PNT_SUPPORT                     1

/** @details
    If Fitness Machine Control Point characteristic is supported, set FTMS_CHAR_CP_SET_WHEEL_CIRCUMFERENCE_SUPPORT
    to 1 to support Set Wheel Circumference opcode, otherwise set it to 0.
*/
#define FTMS_CHAR_CP_SET_WHEEL_CIRCUMFERENCE_SUPPORT        1

/** @details
    If Fitness Machine Control Point characteristic is supported, set FTMS_CHAR_CP_SPIN_DOWN_CTRL_SUPPORT
    to 1 to support Spin Down Control opcode, otherwise set it to 0.
*/
#define FTMS_CHAR_CP_SPIN_DOWN_CTRL_SUPPORT                 1

/** @details
    When FTMS_CHAR_FTMS_CTRL_PNT_SUPPORT is set to 0, set FTMS_CHAR_FTMS_STATUS_SUPPORT to 1 to
    support Fitness Machine Control Point characteristic, otherwise set it to 0.
*/
#define FTMS_CHAR_FTMS_STATUS_SUPPORT                       1

/** @} End of FTMS_Common_Exported_Macros */

/** @} End of FTMS_CONFIG */

#endif /* _FTMS_CONFIG_H_ */
