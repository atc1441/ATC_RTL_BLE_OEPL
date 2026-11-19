#ifndef OTP_CONFIG_H
#define OTP_CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif

#include "rtl876x_wdg.h"
/*============================================================================*
 *                        debug configuration
 *============================================================================*/
/** @brief just for OTA Demo test */
#define OTA_TEST                                   0
/** @brief just for OTA Demo test */
#define OTA_TEST_BANK_SWITCH                       0
#if (OTA_TEST_BANK_SWITCH == 0)
#define OTA_TEST_IMAGE_VERSION                     0
#endif

/** @brief just for debug */
#define SYSTEM_TRACE_ENABLE                        0


/*============================================================================*
 *                        flash configuration
 *============================================================================*/
/** @brief support for puran flash*/
#define FTL_APP_CALLBACK_ENABLE                    0
/** @brief enable BP, set lock level depend on flash layout and selected flash id */
#define FLASH_BLOCK_PROTECT_ENABLE                 0
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

/** @brief enable aon wdg which continue work in dlps state */
#define AON_WDG_ENABLE                             0
/** @brief set aon wdg timeout period in seconds, max value is 65s */
#define AON_WDG_TIME_OUT_PERIOD_SECOND             10

/**************************************************/
/** @brief config enable write hardfault record to flash example */
//config enable write hardfault record to flash
//if enable, must define 1; if disable, define 0 or not define
#define WRITE_HARDFAULT_RECORD_TO_FLASH_ENABLE        0
//if enable must define hardfault record begin and end flash addr, and record cfg depend on user flash usage
#define HARDFAULT_RECORD_BEG_ADDR                  0x00880000  //change to user unused space
#define HARDFAULT_RECORD_END_ADDR                 (HARDFAULT_RECORD_BEG_ADDR + 2 * FMC_PAGE_SIZE)
#define HARDFAULT_RECORD_CFG                      ( BIT_ENABLE_SAVE_HARDFAULT )

/*before wdg system reset, write reset reason to specific flash addr if enable*/
#define WRITE_REASON_TO_FLASH_BEFORE_RESET_ENABLE     0
#if (WRITE_REASON_TO_FLASH_BEFORE_RESET_ENABLE > 0)
#undef ROM_WATCH_DOG_MODE
#define ROM_WATCH_DOG_MODE                            INTERRUPT_CPU
/*write reset reason to specific flash address*/
#define REBOOT_REASON_RECORD_ADDRESS                  0x00882000
/*reboot info record occupy flash size equal (2^n)*/
#define REBOOT_REASON_RECORD_LIMIT_POWERT2            12//reserve 4K
#endif
/**************************************************/


/*============================================================================*
 *                        upperstack configuration
 *============================================================================*/
//add more here


/*============================================================================*
*                        OTA configuration
*============================================================================*/
/*If support unsafe single bank ota user data, must define the following macros */
#define SUPPORT_SINGLE_BANK_OTA_USER_DATA
#ifdef SUPPORT_SINGLE_BANK_OTA_USER_DATA
#define USER_DATA_START_ADDR                       (0x00800000 + 8 * 1024 * 1024)  //back 8M
#define USER_DATA_MAX_SIZE                         (8 * 1024 * 1024)  //8M
#define DISABLE_AES_OTA                             0
#endif

/*normal ota timeout settings*/
#define OTA_TIMEOUT_TOTAL                          240
#define OTA_TIMEOUT_WAIT4_CONN                     60
#define OTA_TIMEOUT_WAIT4_IMAGE_TRANS              200
#define OTA_TIMEOUT_CTITTV                         0xFF


/*============================================================================*
 *                        app configuration
 *============================================================================*/
//add more here



#ifdef __cplusplus
}
#endif


/** @} */ /* End of group OTP_CONFIG */
#endif
