/**
*****************************************************************************************
*               Copyright(c) 2017, Realtek Semiconductor Corporation. All rights reserved.
************************************************************************************************************
  * @file    platform_utils.h
  * @brief   utility helper function for user application
  * @author  lory_xu
  * @date    2017-02
  * @version v1.0
  ***************************************************************************************
    * @attention
  * <h2><center>&copy; COPYRIGHT 2017 Realtek Semiconductor Corporation</center></h2>
  ***************************************************************************************
  */

#ifndef _PLATFORM_UTILS_H_
#define _PLATFORM_UTILS_H_

#ifdef __cplusplus
extern "C" {
#endif

/*============================================================================*
 *                               Header Files
*============================================================================*/
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "rtl876x.h"
/** @defgroup  PLATFORM_UTILS Platform Utilities
    * @brief Utility helper functions
    * @{
    */

#if (PLATFORM_SUPPORT_CLOCK_MANAGER == 1)
#include "clock_manager.h"
#else
#define CLOCK_100MHZ    100000000
#define CLOCK_90MHZ     90000000
#define CLOCK_40MHZ     40000000
#define CLOCK_53MHZ     53333333
#define CLOCK_80MHZ     80000000
#define CLOCK_20MHZ     20000000
#define CLOCK_10MHZ     10000000
#define CLOCK_5MHZ      5000000
#define CLOCK_2P5MHZ    2500000
#define CLOCK_1P25MHZ   1250000
#define CLOCK_625KHZ    625000
#endif



/*============================================================================*
 *                              Functions
 *============================================================================*/
/** @defgroup PLATFORM_UTILS_Exported_Functions Platform Utilities Exported Functions
    * @brief
    * @{
    */

/**
 * @brief Generate random number given max number allowed
 * @param max   to specify max number that allowed
 * @return random number
 */

extern uint32_t platform_random(uint32_t max);


#if defined ( __CC_ARM )
/**
 * @brief Busy delay for a specified number of milliseconds.
 * @param[in] t  The number of milliseconds to delay.
 * @return none
 */
extern volatile void (*platform_delay_ms)(uint32_t t);
/**
 * @brief Busy delay for a specified number of microseconds.
 * @param[in] t  The number of microseconds to delay.
 * @return none
 */
extern volatile void (*platform_delay_us)(uint32_t t);
#elif defined (__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050) || defined (__GNUC__)
/**
 * @brief Busy delay for a specified number of milliseconds.
 * @param[in] t  The number of milliseconds to delay.
 * @return none
 */
extern void (*platform_delay_ms)(uint32_t t);
/**
 * @brief Busy delay for a specified number of microseconds.
 * @param[in] t  The number of microseconds to delay.
 * @return none
 */
extern void (*platform_delay_us)(uint32_t t);
#endif

/**
 * @brief Get the vendor timer tick

 * @param none
 * @return none
 * @note Features of the vendor timer:
 *         (1) clock rate is 40M
 *         (2) width is 26 bits (max 0x3FFFFFF)
 *         (3) tick counter is incremental
 *
 */
static inline uint32_t platform_vendor_tick(void)
{
    return HAL_READ32(VENDOR_REG_BASE, 0x30);
}

/** @} */ /* End of group PLATFORM_UTILS_Exported_Functions */

/** @} */ /* End of group PLATFORM_UTILS */


#ifdef __cplusplus
}
#endif

#endif

