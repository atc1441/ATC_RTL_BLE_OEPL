/**
*****************************************************************************************
*     Copyright(c) 2022, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
  * @file    power_manager_slave.h
  * @brief   Power Manager Slave implementation head file.
  * @author  George Chang, js.lin
  * @date    2022-08-29
  * @version v1.0
  * *************************************************************************************
  */

/*============================================================================*
 *               Define to prevent recursive inclusion
 *============================================================================*/
#ifndef __POWER_MANAGER_SLAVE_H
#define __POWER_MANAGER_SLAVE_H


/*============================================================================*
 *                               Header Files
*============================================================================*/
#include <stdint.h>
#include <stdbool.h>
#include "power_manager_interface.h"
#include "platform_rtc.h"

#ifdef __cplusplus
extern "C" {
#endif

#define IRQ_TAG_FILL_WORD  ( 0x5a5a5a5aU )

#define POWER_MANAGER_GET_TIMESTAMP(stamp)                                                                  \
    do {                                                                                                    \
        stamp = platform_rtc_get_counter();                                                                 \
    } while(0)

#define POWER_MANAGER_GET_TIMEDIFF(start, end, diff)                                                        \
    do {                                                                                                    \
        diff = clk_cnt_diff(start, end, PF_RTC_BIT_NUM);                                                    \
    } while(0)

#define POWER_MANAGER_UPDATE_EXTREMUM(diff, diff_max, diff_min)                                             \
    do {                                                                                                    \
        diff_max = diff > diff_max ? diff : diff_max;                                                       \
        diff_min = diff < diff_min ? diff : diff_min;                                                       \
    } while(0)

#define PM_SYSTEM_LEVEL_UNIT_NUM    2

/*============================================================================*
 *                              Types
*============================================================================*/
/** @defgroup POWER_MANAGER_Exported_Types Power Manager Exported Types
  * @{
  */
typedef PMCheckResult(*PMUnitCheckFunc)(uint32_t, uint32_t, uint32_t *);
typedef void (*PMUnitStoreFunc)(void);
typedef void (*PMUnitRestoreFunc)(void);
typedef void (*PMUnitEnterFunc)(void);
typedef void (*PMUnitExitFunc)(void);

typedef PMCheckResult(*PMCheckWakeupTimeFunc)(uint32_t *);
typedef void (*PMFuncToReturn)(void);

typedef enum
{
    PROFILING_PM_SLAVE_CHECK        = 0,
    PROFILING_PM_SLAVE_STORE        = 1,
    PROFILING_PM_SLAVE_ENTER        = 2,
    PROFILING_PM_SLAVE_EXIT         = 3,
    PROFILING_PM_SLAVE_RESTORE      = 4,
    PROFILING_PM_SLAVE_STAGE_MAX    = 5
} ProfilingPMSlaveStage;

typedef struct
{
    uint32_t stage_start;
    uint32_t stage_end;
    uint32_t stage_time[PROFILING_PM_SLAVE_STAGE_MAX];
    uint32_t stage_time_max[PROFILING_PM_SLAVE_STAGE_MAX];
    uint32_t stage_time_min[PROFILING_PM_SLAVE_STAGE_MAX];
} ProfilingPMSlaveData;

typedef struct
{
    PMUnitCheckFunc check_func;
    PMUnitStoreFunc store_func;
    PMUnitRestoreFunc restore_func;
    PMUnitEnterFunc enter_func;
    PMUnitExitFunc exit_func;

    PMUnitID unit_id;

    ProfilingPMSlaveData *profiling_data;
} PowerManagerSlaveUnit;

typedef struct
{
    PMUnitID unit_id;
    uint32_t pon_delay_time;
    uint32_t pon_stage_time;
    uint32_t pof_delay_time;
    uint32_t pof_stage_time;
    uint32_t minimum_sleep_guard_time;
} PowerManagerSlaveRequestResource;

typedef struct
{
    uint32_t start_time_diff;
    uint32_t end_time_diff;
} PMTimePeriod;

typedef struct
{
    uint32_t cur_clk;
    PMTimePeriod occupied;
} PMOccupyTimeInfo;

typedef struct
{
    PowerManagerSlaveUnit *unit_array[PM_SYSTEM_LEVEL_DEF_MAX][PM_SYSTEM_LEVEL_UNIT_NUM];

    PMOccupyTimeInfo master[PM_UNIT_MAX_NUM];
    PMOccupyTimeInfo slave[PM_UNIT_MAX_NUM];

    uint32_t suspended;

    PMFuncToReturn func_to_return;
} PowerManagerSlaveSystem;

typedef union
{
    uint8_t value;
    struct
    {
        uint8_t profiling_mode:                     1;
        uint8_t irq_control_by_master:              1;
        uint8_t m2s_inact_msg_dbg:                  1;
        uint8_t s2m_inact_msg_dbg:                  1;
        uint8_t m2s_act_msg_dbg:                    1;
        uint8_t s2m_act_msg_dbg:                    1;
        uint8_t rsvd:                               2;
    };
} PowerManagerSlaveFeatureConfig;

/*============================================================================*
 *                              Variables
*============================================================================*/
/** @defgroup POWER_MANAGER_Exported_Variables Power Manager Exported Variables
  * @{
  */
extern PowerManagerSlaveFeatureConfig power_manager_slave_feature_cfg;
extern PowerManagerSlaveSystem power_manager_slave_system;

extern bool (*power_manager_slave_check)(const PMM2SInactivateMessage *);
extern void (*power_manager_slave_store_and_enter)(const PMM2SInactivateMessage *);
extern void (*power_manager_slave_exit_and_restore)(const PMM2SActivateMessage *);
extern void (*power_manager_slave_cpu_sleep)(bool);
extern bool (*power_manager_slave_check_interrupt_occur)(void);
extern void (*power_manager_slave_handle_irq)(bool, bool *);
extern void (*power_manager_slave_init)(void);

extern uint32_t (*power_manager_slave_align_clk)(const uint32_t, uint32_t *, uint32_t *, uint32_t);
extern PMCheckResult(*power_manager_slave_align_wakeup_time)(PMCheckWakeupTimeFunc, const uint32_t,
                                                             uint32_t *);

extern void (*power_manager_slave_update_master_occupied_time)(const PMM2SInactivateMessage *);
extern void (*power_manager_slave_set_occupied_time_descending)(const uint32_t,
                                                                const PMOccupyTimeInfo *, PMTimePeriod *, uint8_t);
extern bool (*power_manager_slave_check_occupied_time)(const PMTimePeriod *, const uint8_t,
                                                       uint32_t *, uint32_t, uint32_t);
extern void (*power_manager_slave_register_resource)(PMUnitID, uint32_t, uint32_t, uint32_t);
extern void (*power_manager_slave_unregister_resource)(PMUnitID);
extern bool (*power_manager_slave_request_resource)(const uint32_t, uint32_t *,
                                                    PowerManagerSlaveRequestResource *);

extern bool (*power_manager_slave_inact_msg_handler)(const PMM2SInactivateMessage *, bool *);
extern void (*power_manager_slave_act_msg_handler)(const PMM2SActivateMessage *);

extern void (*power_manager_slave_inact_action_handler)(void);
extern void (*power_manager_slave_act_action_handler)(void);
/** @} */ /* End of group POWER_MANAGER_Exported_Variables */

/*============================================================================*
 *                              Functions
*============================================================================*/

/** @defgroup POWER_MANAGER_Exported_Functions Power Manager Exported Functions
  * @{
  */
void power_manager_slave_register_function_to_return(PMFuncToReturn func);
void power_manager_slave_register_unit(PMUnitID unit_id, PowerManagerSlaveUnit *p_unit);
PowerManagerSlaveUnit *power_manager_slave_get_unit(PMUnitID unit_id);
void power_manager_slave_suspend_unit(PMUnitID unit_id);
void power_manager_slave_resume_unit(PMUnitID unit_id);
void power_manager_slave_suspend_all(void);
void power_manager_slave_resume_all(void);

/** @} */ /* End of group POWER_MANAGER_Exported_Functions */

/** @} */ /* End of group POWER_MANAGER */


#ifdef __cplusplus
}
#endif

#endif  /* __POWER_MANAGER_SLAVE_H */

