/**
************************************************************************************************************
*               Copyright(c) 2016, Realtek Semiconductor Corporation. All rights reserved.
************************************************************************************************************
* @file     rtl876x_lib_platform.h
* @brief
* @details
* @author
* @date
* @version
*************************************************************************************************************
*/

#ifndef _RTL876X_LIB_PLATFORM_H_
#define _RTL876X_LIB_PLATFORM_H_

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "mem_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/** @defgroup  RTL876X_LIB_PLATFORM   RTL876x Lib Platform
  * @brief
  * @{
  */
/*============================================================================*
 *                       Functions
 *============================================================================*/
/** @defgroup RTL876X_LIB_PLATFORM_Exported_Functions RTL876x Lib Platform Exported Functions
  * @brief
  * @{
  */

/**
 * @brief  Disable SWD pin
*/
void swd_pin_disable(void);

/**
 * @brief  Get 14 bytes EUID
 *
 * @return The pointer to a copy of EUID.
  */
uint8_t *get_ic_euid(void);

/** End of RTL876X_LIB_PLATFORM_Exported_Functions
  * @}
  */

/** End of RTL876X_LIB_PLATFORM
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* _RTL876X_LIB_PLATFORM_H_ */
