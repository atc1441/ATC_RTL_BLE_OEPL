#ifndef OTP_CONFIG_H
#define OTP_CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif

#include "rtl876x_wdg.h"
/*============================================================================*
 *                        debug configuration
 *============================================================================*/
/** @brief just for debug */
#define SYSTEM_TRACE_ENABLE                        0


/*============================================================================*
 *                        flash configuration
 *============================================================================*/
/** @brief support for puran flash*/
#define FTL_APP_CALLBACK_ENABLE                    0
/** @brief enable BP, set lock level depend on flash layout and selected flash id */
#define FLASH_BLOCK_PROTECT_ENABLE                 1
/** @brief modify delay time for wakeup flash from power down mode to standby mode*/
#define AFTER_TOGGLE_CS_DELAY                      6


/*============================================================================*
 *                        platform configuration
 *============================================================================*/
/** @brief default enable swd pinmux */
#define SWD_PINMUX_ENABLE                          1
/** @brief default disable watch dog in rom */
#define ROM_WATCH_DOG_ENABLE                       0
/** @brief set wdg mode, default reset all */
#define ROM_WATCH_DOG_MODE                         RESET_ALL
/** @brief Watch Dog Timer Config, default 4s timeout
   * div_factor: 16Bit: 32.768k/(1+divfactor).
   * cnt_limit: 2^(cnt_limit+1) - 1 ; max 11~15 = 0xFFF.
   * wdg_mode:
   *            1: RESET_ALL_EXCEPT_AON
   *            3: RESET_ALL
**/
#define ROM_WATCH_DOG_CFG_DIV_FACTOR               31
#define ROM_WATCH_DOG_CFG_CNT_LIMIT                15

/*before wdg system reset, write reset reason to specific flash addr if enable*/
#define WRITE_REASON_TO_FLASH_BEFORE_RESET_ENABLE     0
#if (WRITE_REASON_TO_FLASH_BEFORE_RESET_ENABLE > 0)
/*write reset reason to specific flash address*/
#define REBOOT_REASON_RECORD_ADDRESS                  0x8cb000
/*max number of reboot record (2^n), one reset reason need 4 bytes, occupy flash size equal 2^(n+2)*/
#define REBOOT_REASON_RECORD_LIMIT_POWERT2            10  //reserve 4K
#endif

/** @brief to fix bug need disable dump callstack info before WDG_SystemReset, default enable */
#define DUMP_INFO_BEFORE_RESET_DISABLE             1


/*============================================================================*
 *                        upperstack configuration
 *============================================================================*/
//add more here


/*============================================================================*
 *                        app configuration
 *============================================================================*/
#define OTA_TIMEOUT_TOTAL                          240
#define OTA_TIMEOUT_WAIT4_CONN                     60
#define OTA_TIMEOUT_WAIT4_IMAGE_TRANS              200
#define OTA_TIMEOUT_CTITTV                         0xFF
//add more here



#ifdef __cplusplus
}
#endif


/** @} */ /* End of group OTP_CONFIG */
#endif
