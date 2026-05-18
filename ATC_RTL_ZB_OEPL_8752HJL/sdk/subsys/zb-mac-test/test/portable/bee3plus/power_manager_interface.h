/**
*********************************************************************************************************
*               Copyright(c) 2021, Realtek Semiconductor Corporation. All rights reserved.
*********************************************************************************************************
* @file         power_manager_interface.h
* @brief        Power Manager Interface implementation head file.
* @details
* @author       Kellan Ho, js.lin
* @date         2022-08-29
* @version      v1.0
*********************************************************************************************************
*/

/*============================================================================*
 *               Define to prevent recursive inclusion
 *============================================================================*/
#ifndef __POWER_MANAGER_INTERFACE_H
#define __POWER_MANAGER_INTERFACE_H


/*============================================================================*
 *                               Header Files
*============================================================================*/
#include "stdint.h"
#include "stdbool.h"

#ifdef __cplusplus
extern "C" {
#endif

/* for naming compatibility */
#define PM_SLAVE_BTMAC                  PM_SLAVE_0
#define PM_SLAVE_ZIGBEE                 PM_SLAVE_0
#define PM_SLAVE_PROPRIETARY            PM_SLAVE_0

#define PM_INVALID_WAKEUP_TIME_DIFF 0xffffffff

#define PM_UNIT_MAX_NUM             4 // equal to num of pf rtc comparators
#define PM_GET_UNIT_INDEX(unit_id)  (((unit_id >> 2) == PM_SYSTEM_LEVEL_PF) ? 3 : \
                                     ((unit_id >> 2) == PM_SYSTEM_LEVEL_1) ? 2 : (unit_id & 0x1))

typedef enum PMSlave
{
    PM_SLAVE_0                  = 0,
    PM_SLAVE_DEF_MAX,
} PMSlave;

typedef enum PMSystemLevel
{
    PM_SYSTEM_LEVEL_0           = 0,
    PM_SYSTEM_LEVEL_1           = 1,
    PM_SYSTEM_LEVEL_PF          = 2,
    PM_SYSTEM_LEVEL_DEF_MAX,
} PMSystemLevel;

typedef enum PMUnitStatus
{
    PM_UNIT_UNKNOWN             = 0,
    PM_UNIT_PEND                = 1,
    PM_UNIT_INACTIVE            = 2,
    PM_UNIT_ACTIVE              = 3,
} PMUnitStatus;

typedef enum PMCheckResult
{
    PM_CHECK_PEND               = 0,
    PM_CHECK_FAIL               = 1,
    PM_CHECK_PASS               = 2,
} PMCheckResult;

typedef enum PMAciton
{
    PM_ACTION_IDLE_CHECK_REQUEST,
    PM_ACTION_IDLE_CHECK_CONTINUE,
    PM_ACTION_IDLE_CHECK_RESPONSE,
    PM_ACTION_CHECK_REQUEST,
    PM_ACTION_CHECK_RESPONSE,
    PM_ACTION_STORE_ENTER_REQUEST,
    PM_ACTION_STORE_ENTER_RESPONSE,
    PM_ACTION_EXIT_RESTORE_REQUEST,
    PM_ACTION_EXIT_RESTORE_RESPONSE,
    PM_ACTION_DEF_MAX,
} PMAciton;

typedef enum
{
    PM_UNIT_PROPRIETARY      = (PM_SYSTEM_LEVEL_0 << 2 | 0),
    PM_UNIT_ZIGBEE           = (PM_SYSTEM_LEVEL_0 << 2 | 1),
    PM_UNIT_BTMAC            = (PM_SYSTEM_LEVEL_1 << 2 | 0),
    PM_UNIT_PLATFORM         = (PM_SYSTEM_LEVEL_PF << 2 | 0),
    PM_UNIT_HEADER_ID        = 0x80,
} PMUnitID;

typedef union PMM2SActivateMessage
{
    uint32_t d32[4];
    struct
    {
        uint32_t action:                            4;
        uint32_t unit_id:                           4;
        uint32_t dummy_0:                          23;
        uint32_t info_valid:                        1;

        uint32_t dummy_1;

        uint32_t dummy_2;

        uint32_t dummy_3;
    };
} PMM2SActivateMessage;

typedef union PMS2MActivateMessage
{
    uint32_t d32[1];
    struct
    {
        uint32_t action:                            4;
        uint32_t unit_id:                           4;
        uint32_t dummy_0:                          23;
        uint32_t info_valid:                        1;
    };
} PMS2MActivateMessage;

typedef union PMM2SInactivateMessage
{
    uint32_t d32[8];
    struct
    {
        uint32_t action:                            4;
        uint32_t unit_id:                           4;
        uint32_t enable_irq:                        1;
        uint32_t enter_wfi:                         1;
        uint32_t dummy_0:                          21;
        uint32_t info_valid:                        1;

        uint32_t check_clk;

        uint32_t pre_sys_lvl_wakeup_time_diff;

        uint32_t unit_pf_occupied_start_time_diff;

        uint32_t unit_pf_occupied_end_time_diff;

        uint32_t unit_0_occupied_time_diff;

        uint32_t unit_1_occupied_time_diff;

        uint32_t unit_2_occupied_time_diff;
    };
} PMM2SInactivateMessage;

typedef union PMS2MInactivateMessage
{
    uint32_t d32[3];
    struct
    {
        uint32_t action:                            4;
        uint32_t unit_id:                           4;
        uint32_t check_result:                      2;
        uint32_t dummy_0:                          21;
        uint32_t info_valid:                        1;

        uint32_t wakeup_time_diff;

        uint32_t dummy_2;
    };
} PMS2MInactivateMessage;

extern volatile PMM2SActivateMessage g_pm_m2s_act_msg;
extern volatile PMS2MActivateMessage g_pm_s2m_act_msg;
extern volatile PMM2SInactivateMessage g_pm_m2s_inact_msg;
extern volatile PMS2MInactivateMessage g_pm_s2m_inact_msg;

extern void (*power_manager_master_initiate_wakeup)(PMSlave slave, PMSystemLevel sys_lvl,
                                                    PMUnitID unit_id);
extern void (*power_manager_master_suspend_unit)(PMSlave slave, PMSystemLevel sys_lvl,
                                                 PMUnitID unit_id);
extern void (*power_manager_master_resume_unit)(PMSlave slave, PMSystemLevel sys_lvl,
                                                PMUnitID unit_id);
extern void (*power_manager_master_set_timing_guarantee_sys_lvl)(PMSlave slave,
                                                                 PMSystemLevel timing_guarantee_sys_lvl);
extern void (*power_manager_master_register_unit)(PMSlave slave, PMSystemLevel sys_lvl,
                                                  PMUnitID unit_id);
extern void (*power_manager_master_reset_slave)(PMSlave slave);

void power_manager_interface_init_unit_status(void);
PMUnitStatus power_manager_interface_get_unit_status(PMSlave slave, PMUnitID unit_id);
void power_manager_interface_set_unit_status(PMSlave slave, PMUnitID unit_id, PMUnitStatus status);
bool power_manager_interface_check_unit_active(PMSlave slave, PMUnitID unit_id);
bool power_manager_interface_check_unit_active_from_isr(PMSlave slave, PMUnitID unit_id);

#ifdef __cplusplus
}
#endif

#endif  /* __POWER_MANAGER_INTERFACE_H */

