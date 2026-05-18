/**
*********************************************************************************************************
*               Copyright(c) 2024, Realtek Semiconductor Corporation. All rights reserved.
*********************************************************************************************************
* @file      os_pm.h
* @brief     Header file for os power manager API.
* @details   This file is used for power manager.
* @author    rui_yu
* @date      2024-12-30
* @version   v1.0
* *********************************************************************************************************
*/

#ifndef _OS_POWER_MANAGER_H_
#define _OS_POWER_MANAGER_H_
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "dlps.h"

#ifdef __cplusplus
extern "C" {
#endif

#define UPDATE_TICK_COUNT()\
    if (portNVIC_INT_CTRL_REG & SCB_ICSR_PENDSTSET_Msk)\
    {\
        os_systick_handler();\
        portNVIC_INT_CTRL_REG = SCB_ICSR_PENDSTCLR_Msk;\
    }

#define US_TO_TICK_COUNT(us, residual)\
    cal_quotient_remainder(1000000 * (uint64_t)(os_sys_tick_clk_get() / os_sys_tick_rate_get()), os_sys_tick_clk_get(), us, &residual)

#define US_TO_SYSTICK(us, residual)\
    cal_quotient_remainder(1000000, os_sys_tick_clk_get(), us, &residual)

#define TICK_COUNT_TO_US(tick_count)\
    cal_quotient_remainder(os_sys_tick_clk_get(), 1000000 * (uint64_t)(os_sys_tick_clk_get() / os_sys_tick_rate_get()), tick_count, NULL)

#define SYSTICK_TO_US(systick)\
    cal_quotient_remainder(os_sys_tick_clk_get(), 1000000, systick, NULL)

#define SYSTICK_RELOAD_VALUE    (os_sys_tick_clk_get() / os_sys_tick_rate_get() - 1)

/** @addtogroup DLPS_PLATFORM_Exported_Types
* @{
*/
typedef enum
{
    PLATFORM_PM_EXCLUDED_TIMER,     /**< Timer exclude type for power manager */
    PLATFORM_PM_EXCLUDED_TASK,      /**< Task exclude type for power manager */
    PLATFORM_PM_EXCLUDED_TYPE_MAX,  /**< Max exclude type number for power manager */
} PlatformExcludedHandleType;
/** @} */

typedef struct
{
    struct PlatformPMExcludedHandleQueueElem *pNext;
    void **handle;
} PlatformPMExcludedHandleQueueElem;

typedef union
{
    uint8_t value[1];
    struct
    {
        uint8_t os_pm_statistic:        1;
        uint8_t rsvd:                   7;
    };
} OSPMFeatureConfig;

typedef struct
{
    uint32_t last_sleep_clk;
    uint32_t last_sleep_systick;
    uint32_t tickcount_restore_remain_us;
} OSPMSystem;

extern bool (*os_sched_restore)(void);
extern void os_systick_handler(void);
extern uint32_t (*os_sys_tick_rate_get)(void);
extern uint32_t (*os_sys_tick_clk_get)(void);
extern uint64_t os_sys_tick_increase(uint32_t tick_increment);

extern uint32_t (*os_pm_next_timeout_value_get)(void);
extern void (*os_pm_store_tickcount)(void);
extern void (*os_pm_restore_tickcount)(void);
extern PMCheckResult(*os_pm_check)(uint32_t *wakeup_time_diff);
extern void (*os_pm_store)(void);
extern void (*os_pm_restore)(void);
extern void (*os_pm_init)(void);

/** @addtogroup DLPS_PLATFORM_Exported_Functions
* @{
*/

/**
 * @brief  Exclude SW timer or task from low power wake list, SW timer needs to be one-shot timer.
 * @param[in]  handle: Handle of SW timer or task.
 * @param[in]  type: Type of SW timer or task, refer to @ref PlatformExcludedHandleType.
 * @return Operation status
*/
extern bool (*os_register_pm_excluded_handle)(void **pp_handle, PlatformExcludedHandleType type);

/**
 * @brief  Cancel exclude SW timer or task from low power wake list.
 * @param[in]  handle: Handle of timer or task.
 * @param[in]  type: Type of timer or task, refer to @ref PlatformExcludedHandleType.
 * @return Operation status
*/
extern bool (*os_unregister_pm_excluded_handle)(void **pp_handle, PlatformExcludedHandleType type);
/** @} */

#ifdef __cplusplus
}
#endif

#endif /* _OS_POWER_MANAGER_H_ */
