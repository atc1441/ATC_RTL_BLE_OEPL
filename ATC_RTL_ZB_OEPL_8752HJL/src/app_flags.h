#ifndef _APP_FLAGS_H_
#define _APP_FLAGS_H_

#include "upperstack_config.h"

/** @defgroup  PERIPH_Config Peripheral App Configuration
 * @brief This file is used to config app functions.
 * @{
 */
/*============================================================================*
 *                              Constants
 *============================================================================*/

/** @brief  Config APP LE link number */
#define APP_MAX_LINKS 1

/** @brief  UART debug log: 1-enable uart_printf + printf redirect + APP_PRINT_* to UART */
#define UART_LOG_EN 1
/** @brief  UART TRACE level log (very verbose): 1-enable, 0-disable. Ignored if UART_LOG_EN=0 */
#define UART_LOG_TRACE_EN 0

/** @brief  Config DLPS: 0-Disable DLPS, 1-Enable DLPS */
#define F_BT_DLPS_EN 1

/** @brief  SPI-Flash self-test on every boot: 1=enable, 0=disable.
 *  Writes a 16-byte test pattern to the last flash sector and reads it back.
 *  Set to 0 once the flash driver is verified and the driver is stable. */
#define FLASH_SELFTEST_EN 0

#define HW_TYPE 0x53
#define FIRMWARE_VERSION 0x0030

#define DEBUG_BUILD 0

#if DEBUG_BUILD
#define SLEEP_DELAY_ONLINE 1  // 40 seconds
#define SLEEP_DELAY_OFFLINE 1 // 15 minutes
#define DO_EPD_REFRESH 0 // for fast debugging disable EPD Refresh
#else
#define SLEEP_DELAY_ONLINE 40  // 40 seconds
#define SLEEP_DELAY_OFFLINE 15 // 15 minutes
#define DO_EPD_REFRESH 1 // for fast debugging disable EPD Refresh
#endif

/** @brief  Redirect printf() → uart_printf() globally.
 *
 *  This file is pre-included in EVERY translation unit via -include app_flags.h
 *  in the Makefile.  Files that include <stdio.h> (like eeprom.c, drawing.c,
 *  ble_cmd_handler.c) would otherwise call newlib's printf, which crashes in
 *  __sfp_r/__sinit when invoked from RTOS task context.  By defining the macro
 *  here, ALL printf calls in ALL files are redirected to uart_printf, which
 *  uses only vsnprintf+UART and never touches the newlib FILE infrastructure.
 */
#if UART_LOG_EN
extern int uart_printf(const char *fmt, ...); /* defined in retarget.c */
#define printf uart_printf
#endif

/** @} */ /* End of group PERIPH_Config */
#endif
