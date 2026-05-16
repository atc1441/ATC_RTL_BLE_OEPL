/**
*****************************************************************************************
*     Copyright(c) 2017, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
  * @file    dlps.h
  * @brief   DLPS implementation head file.
  * @author  lory_xu
  * @date    2014-08-05
  * @version v1.0
  * *************************************************************************************
   * @attention
   * <h2><center>&copy; COPYRIGHT 2017 Realtek Semiconductor Corporation</center></h2>
   * *************************************************************************************
  */

/*============================================================================*
 *               Define to prevent recursive inclusion
 *============================================================================*/
#ifndef __DLPS_H
#define __DLPS_H


/*============================================================================*
 *                               Header Files
*============================================================================*/
#include <stdint.h>
#include <stdbool.h>
#include "os_queue.h"
#include "otp.h"
#include "system_rtl876x.h"

#ifdef __cplusplus
extern "C" {
#endif




/** @defgroup DLPS_PLATFORM DLPS Platform
  * @brief Deep low power state support
  * @{
  */

/*============================================================================*
 *                              Types
*============================================================================*/
/** @defgroup DLPS_PLATFORM_Exported_Types DLPS Platform Exported Types
  * @{
  */

/** @brief This CB is used for any module which needs to be checked before entering DLPS */
typedef bool (*DLPSEnterCheckFunc)();

/** @brief This CB is used for any module which needs to control the hw before entering or after exiting from DLPS */
typedef void (*DLPSHWControlFunc)();

/*============================================================================*
 *                              Platform Unit
*============================================================================*/

/** @brief PlatformPMStage struct */
typedef enum
{
    PLATFORM_PM_CHECK       = 0,
    PLATFORM_PM_STORE       = 1,
    PLATFORM_PM_ENTER       = 2,
    PLATFORM_PM_EXIT        = 3,
    PLATFORM_PM_RESTORE     = 4,
    PLATFORM_PM_PEND        = 5,
} PlatformPMStage;


/** @brief PlatformPowerMode struct */
typedef enum
{
    PLATFORM_POWERDOWN      = 1,   /**< Power down */
    PLATFORM_DLPS_PFM       = 2,   /**< DLPS (PFM) */
    PLATFORM_DLPS_RET       = 3,   /**< DLPS (RET) */
    PLATFORM_ACTIVE         = 5,   /**< Active     */
} PlatformPowerMode;

typedef enum
{
    PLATFORM_PM_WAKEUP_UNKNOWN              = 0x0000,
    PLATFORM_PM_WAKEUP_USER                 = 0x0001,
    PLATFORM_PM_WAKEUP_OS                   = 0x0002,
    PLATFORM_PM_WAKEUP_PRE_SYSTEM_LEVEL     = 0x0003,
    PLATFORM_PM_WAKEUP_PF_RTC               = 0x0100,
    PLATFORM_PM_WAKEUP_RTC                  = 0x0200,
    PLATFORM_PM_WAKEUP_MAC                  = 0x0400,
    PLATFORM_PM_WAKEUP_GPIO                 = 0x0800,
    PLATFORM_PM_WAKEUP_CTC                  = 0x1000,
} PlatformWakeupReason;

typedef enum
{
    PLATFORM_PM_ERROR_UNKNOWN               = 0x0,
    PLATFORM_PM_ERROR_POWER_MODE            = 0x1,
    PLATFORM_PM_ERROR_DISABLE_DLPS_TIME     = 0x2,
    PLATFORM_PM_ERROR_32K_CHECK_LOCK        = 0x3,
    PLATFORM_PM_ERROR_LOG_DMA_NOT_IDLE      = 0x4,
    PLATFORM_PM_ERROR_CALLBACK_CHECK        = 0x5,
    PLATFORM_PM_ERROR_INTERRUPT_OCCURRED    = 0x6,
    PLATFORM_PM_ERROR_WAKEUP_TIME           = 0x7,
    PLATFORM_PM_ERROR_DATA_UART             = 0x8,
} PlatformPowerModeErrorCode;

typedef enum
{
    BTMAC_PM_WAKEUP_UNKNOWN                 = 0x0,
    BTMAC_PM_WAKEUP_LEGACY                  = 0x1,
    BTMAC_PM_WAKEUP_LE                      = 0x2,
    BTMAC_PM_WAKEUP_PRE_SYSTEM_LEVEL        = 0x3,
} BtmacWakeupReason;

typedef enum
{
    BTMAC_PM_ERROR_UNKNOWN                  = 0x0,
    BTMAC_PM_ERROR_POWER_MODE               = 0x1,
    BTMAC_PM_ERROR_ROLE_SWITCH              = 0x3,
    BTMAC_PM_ERROE_BQB                      = 0x4,
    BTMAC_PM_ERROR_PSD                      = 0x5,
    BTMAC_PM_ERROR_CSB_ENABLE               = 0x6,
    BTMAC_PM_ERROR_NOT_EMPTY_QUEUE_OF_LOWER = 0x7,
    BTMAC_PM_ERROR_CONTROLLER_TO_HOST_BUSY  = 0x8,
    BTMAC_PM_ERROR_TX_BUSY                  = 0x9,
    BTMAC_PM_ERROR_LE_REG_S_INST            = 0xB,
    BTMAC_PM_ERROR_ADV_STATE_NOT_IDLE       = 0xC,
    BTMAC_PM_ERROR_SCAN_STATE_NOT_IDLE      = 0xD,
    BTMAC_PM_ERROR_INITIATOR_UNIT_ENABLE    = 0xE,
    BTMAC_PM_ERROR_CHANNEL_MAP_UPDATE       = 0xF,
    BTMAC_PM_ERROR_CONNECTION_UPDATE        = 0x10,
    BTMAC_PM_ERROR_PHY_UPDATE               = 0x11,
    BTMAC_PM_ERROR_CONN_STATE_NOT_IDLE      = 0x12,
    BTMAC_PM_ERROR_LE_SCHE_NOT_READY        = 0x13,
    BTMAC_PM_ERROR_INTERRUPT_PENDING        = 0x14,
    BTMAC_PM_ERROR_WAKEUP_TIME              = 0x15,
    BTMAC_PM_ERROR_32K_CHECK_LOCK           = 0x16,
} BtmacPowerModeErrorCode;

typedef enum
{
    NO_DATARAM_SHUTDOWN,
    LAST_4K_DATARAM_SHUTDOWN,    /**< 4k */
    LAST_8K_DATARAM_SHUTDOWN,    /**< 4k + 4k */
    LAST_24K_DATARAM_SHUTDOWN,   /**< 16k + 4k + 4k */
    LAST_56K_DATARAM_SHUTDOWN    /**< 32k + 16k + 4k + 4k */
} DATARAM_SHUTDOWN_LEVEL;

/*============================================================================*
 *                              BTMAC Unit
*============================================================================*/

typedef enum
{
    BTMAC_DEEP_SLEEP        = 0,   /**< Deep sleep */
    BTMAC_ACTIVE            = 1,   /**< Active     */
} BtmacPowerMode;

/** @brief DLPS error code*/


/** @} */ /* End of group DLPS_PLATFORM_Exported_Types */

/*============================================================================*
 *                              Variables
*============================================================================*/
/** @defgroup DLPS_PLATFORM_Exported_Variables DLPS Platform Exported Variables
  * @{
  */


/** @} */ /* End of group DLPS_PLATFORM_Exported_Variables */

/*============================================================================*
 *                              Functions
*============================================================================*/

extern void platform_pm_set_power_mode(PlatformPowerMode pf_power_mode_user);
extern PlatformPowerMode platform_pm_get_power_mode(void);
extern void btmac_pm_set_power_mode(BtmacPowerMode bz_power_mode_user);
extern void power_manager_suspend_all(void);
extern void power_manager_resume_all(void);
extern void platform_pm_register_callback_func(void *cb_func, PlatformPMStage pf_pm_stage);
extern void platform_pm_get_statistics(uint32_t *wakeup_count, uint32_t *last_wakeup_clk,
                                       uint32_t *last_sleep_clk);
extern void btmac_pm_get_statistics(uint32_t *wakeup_count, uint32_t *last_wakeup_clk,
                                    uint32_t *last_sleep_clk);
extern void platform_pm_stop_all_non_excluded_timer(void);

extern PlatformPowerModeErrorCode platform_pm_get_error_code(void);
extern uint32_t *platform_pm_get_refuse_reason(void);
extern PlatformWakeupReason platform_pm_get_wakeup_reason(void);

extern BtmacPowerModeErrorCode btmac_pm_get_error_code(void);
extern BtmacWakeupReason btmac_pm_get_wakeup_reason(void);
/** @defgroup DLPS_PLATFORM_Exported_Functions DLPS Platform Exported Functions
  * @{
  */

/**
 * @brief Register Check CB to DlpsPlatform which will call it before entering Dlps.
 * @param  func DLPSEnterCheckFunc
 * @return  Status of Operation.
 * @retval true success
 * @retval false fail
*/
static inline bool dlps_check_cb_reg(DLPSEnterCheckFunc func)
{
    platform_pm_register_callback_func(func, PLATFORM_PM_CHECK);
    return true;
}

/**
 * @brief Register HW Control CB to DlpsPlatform which will call it before entering Dlps or after exiting from Dlps (according to dlpsState) .
 * @param  func DLPSHWControlFunc
 * @param  dlpsState tell the DlpsPlatform the CB should be called when DLPS_ENTER or DLPS_EXITx_XXX.
 * @return  Status of Operation.
 * @retval true success
 * @retval false fail
*/
static inline bool dlps_hw_control_cb_reg(DLPSHWControlFunc func, PlatformPMStage dlpsState)
{
    platform_pm_register_callback_func(func, dlpsState);
    return true;
}

/**
 * @brief Keep platform in @ref LPM_ACTIVE_MODE which means will stop platform enter any lower power mode.
 * @param  none
 * @return  none
*/
static inline void lps_mode_pause(void)
{
    power_manager_suspend_all();
}

/**
 * @brief Restore to original LPSMode.
 * @param  none
 * @return  none
*/
static inline void lps_mode_resume(void)
{
    power_manager_resume_all();
}

/**
 * @brief Set active time after boot before entering into dlps
 * @param  active_time_ms  time to keep active, unit ms
 * @return  none
*/
void set_boot_active_time(uint32_t active_time_ms);

/**
 * @brief  LPSMode Set .
 * @param  mode LPSMode
 * @return  none
*/
static inline void lps_mode_set(PlatformPowerMode mode)
{
    btmac_pm_set_power_mode(BTMAC_DEEP_SLEEP);
    platform_pm_set_power_mode(mode);
}

/**
 * @brief  LPSMode Get .
 * @param  none
 * @return  @ref LPSMode
*/
static inline PlatformPowerMode lps_mode_get(void)
{
    return platform_pm_get_power_mode();
}

/**
 * @brief  Return dlps wakeup counts .
 * @param  none
 * @return  count value
*/
static inline  uint32_t lps_wakeup_count_get(void)
{
    uint32_t wakeup_count, last_wakeup_clk, last_sleep_clk;
    platform_pm_get_statistics(&wakeup_count, &last_wakeup_clk, &last_sleep_clk);
    return wakeup_count;
}

/**
 * @brief  Set data ram to shut down instead of deep sleep when enter dlps to reduce power consumption.
           Data ram(totol 88k) divides into 5 blocks: 32k + 32k + 16k + 4k + 4k.
 * @param  level
 * @return none
  */
static inline void set_dataram_to_shutdown(DATARAM_SHUTDOWN_LEVEL level)
{
    switch (level)
    {
    case LAST_4K_DATARAM_SHUTDOWN:
        {
            OTP->ram_cfg[MEMCFG_DEEPSLEEP].mcu_data_sram = 0xf;
            OTP->ram_cfg[MEMCFG_SHUTDOWN].mcu_data_sram = 0x10;
        }
        break;
    case LAST_8K_DATARAM_SHUTDOWN:
        {
            OTP->ram_cfg[MEMCFG_DEEPSLEEP].mcu_data_sram = 0x7;
            OTP->ram_cfg[MEMCFG_SHUTDOWN].mcu_data_sram = 0x18;
        }
        break;
    case LAST_24K_DATARAM_SHUTDOWN:
        {
            OTP->ram_cfg[MEMCFG_DEEPSLEEP].mcu_data_sram = 0x3;
            OTP->ram_cfg[MEMCFG_SHUTDOWN].mcu_data_sram = 0x1c;
        }
        break;
    case LAST_56K_DATARAM_SHUTDOWN:
        {
            OTP->ram_cfg[MEMCFG_DEEPSLEEP].mcu_data_sram = 0x1;
            OTP->ram_cfg[MEMCFG_SHUTDOWN].mcu_data_sram = 0x1e;
        }
        break;
    default:
        break;
    }
}

/**
 * @brief  LPSMode enter powerdown mode directly .
 * @param  none
 * @return  none
*/
static inline void lps_mode_enter_powerdown_directly(void)
{
    SystemCall(SYSTEM_CALL_ENTER_POWERDOWN_DIRECTLY, 0);
}

/** @} */ /* End of group DLPS_PLATFORM_Exported_Functions */

/** @} */ /* End of group DLPS_PLATFORM */


#ifdef __cplusplus
}
#endif

#endif  /* __DLPS_PLATFORM_H */
