/**************************************************************************//**
 * @file     power_manager_unit_zbmac.h
 * @brief    Zigbee MAC Power Manager Unit Functions header file.
 *
 * @version  V1.00
 * @date     2022-11-29
 *
 * @note
 *
 ******************************************************************************
 *
 * Copyright(c) 2007 - 2022 Realtek Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************************/

#ifndef PWR_MANAG_UNIT_ZBMAC_H
#define PWR_MANAG_UNIT_ZBMAC_H

#ifdef __cplusplus
extern "C" {
#endif

#include "power_manager_interface.h"

typedef enum zbmac_power_mode_e
{
    ZBMAC_DEEP_SLEEP        = 0,   /**< Deep sleep */
    ZBMAC_ACTIVE            = 1,   /**< Active     */
} zbmac_power_mode_t, *pzbmac_power_mode_t;

/* power manager state(stage) transition cycle:
  Check -> Store -> Enter -> Exit -> Restore */
typedef enum zbmac_pm_state_e
{
    ZBMAC_PM_CHECK          = 0,
    ZBMAC_PM_STORE          = 1,
    ZBMAC_PM_ENTER          = 2,
    ZBMAC_PM_EXIT           = 3,
    ZBMAC_PM_RESTORE        = 4,
    ZBMAC_PM_STAGE_MAX      = 5
} zbmac_pm_state_t;

typedef enum
{
    ZBMAC_PM_WAKEUP_UNKNOWN                 = 0x0,
    ZBMAC_PM_WAKEUP_RESUME                  = 0x1,
    ZBMAC_PM_WAKEUP_PRE_SYSTEM_LEVEL        = 0x2,
} zbmac_wakeup_reason_t;

typedef enum
{
    ZBMAC_PM_ERROR_UNKNOWN                  = 0x00,
    ZBMAC_PM_ERROR_POWER_MODE               = 0x01,
    ZBMAC_PM_ERROR_RADIO_BUSY               = 0x02,
    ZBMAC_PM_ERROR_RX_BUSY                  = 0x03,
    ZBMAC_PM_ERROR_SCANNING                 = 0x04,
    ZBMAC_PM_ERROR_INTERRUPT_PENDING        = 0x05,
    ZBMAC_PM_ERROR_WAKEUP_TIME              = 0x06,
    ZBMAC_PM_ERROR_32K_CHECK_LOCK           = 0x07,
    ZBMAC_PM_ERROR_HW_TIMER_RUNNING         = 0x08,
    ZBMAC_PM_ERROR_OCCUPIED_TIME            = 0x09,
} zbmac_power_mode_err_code_t;

typedef void (*zbpm_callback_t)(void);

typedef struct zbpm_adapter_s
{
    zbmac_power_mode_t power_mode;  /* Current power_mode,
                                       App set power_mode = ZBMAC_DEEP_SLEEP to enter DLPS */
    zbmac_wakeup_reason_t wakeup_reason;    // what cause ZB MAC waken
    zbmac_power_mode_err_code_t error_code; // the reason cannot enter sleep mode
    uint16_t stage_time[ZBMAC_PM_STAGE_MAX];  /* the time, in unit of 32K ticks, will take for
                                                 each stage */
    uint16_t minimum_sleep_time;    /* minimum time sleep time, in unit of 32K ticks. the duration
                                       from current time to wakeup time cannot smaller than this value */
    uint16_t learning_guard_time;   /* guard time, in 32K ticks, for learned stage time */
    struct
    {
        uint16_t wake_interval_en : 1;     // is wakeup periodical interval enabled
        uint16_t stage_time_learned : 1;   // is to learn (measure) PM stages consumed time
        uint16_t reserved : 14;
    } cfg;

    uint32_t wakeup_time_us;  // the time stamp of next wakeup time, in unit of us by BT clock
    uint32_t wakeup_interval_us;  // the sleep interval, in unit of us
    uint32_t scheduled_wakeup_time;
    zbpm_callback_t enter_callback; // call back function when in enter stage
    zbpm_callback_t exit_callback;  // call back function when in exit stage
    uint32_t enter_time_us;  // the time stamp of entering ZBMAC_PM_ENTER stage, in unit of us by BT clock
    void *pretain_buf;  // the MAC registers retainion buffer
    uint8_t pm_unit_id; // unit ID to register to the power manager
    uint8_t zb_pm_idx; // index of Zigbee power adapter
    zbpm_callback_t enter_callback_app;  /* call application's back function when the entering
                                           DLPS, i.e. the before the store function is called */
    zbpm_callback_t exit_callback_app;  /* call application's back function when the wakeup process
                                           is completed, i.e. the restore function is done */
} zbpm_adapter_t, *pzbpm_adapter_t;

// System Call OP code to Lower Stack, copy from lower stack for power manager adaption
typedef enum
{
    LOWERSTACK_SYSCALL_OPCODE_READ_BT_CLK_INDEX = 1,
    LOWERSTACK_SYSCALL_OPCODE_SET_ZB_ACTIVE = 10,
    LOWERSTACK_SYSCALL_OPCODE_GET_ZB_ENTRY_WINDOW = 11,
    LOWERSTACK_SYSCALL_OPCODE_REGISTER_MAC_LPS_CALLBACK = 14,
} LOWERSTACK_SYSCALL_OPCODE;

typedef enum
{
    DUAL_MAC_REGISTER_MAC0_CB = 0,
    DUAL_MAC_REGISTER_MAC1_CB = 1,
    DSM_MANAGER_REGISTER_CB = 2
} LOWERSTACK_SYSCALL_REGISTER_MAC_LPS_CALLBACK_SUBCODE;

typedef enum _MacPMCallbackStage
{
    MAC_PM_CALLBACK_CHECK = 0,
    MAC_PM_CALLBACK_STORE = 1,
    MAC_PM_CALLBACK_ENTER = 2,
    MAC_PM_CALLBACK_EXIT = 3,
    MAC_PM_CALLBACK_RESTORE = 4,
    MAC_PM_CALLBACK_STAGE_NUM
} MacPMCallbackStage;

extern PMCheckResult(*zbmac_pm_check)(const uint32_t, const uint32_t, uint32_t *);
extern void (*zbmac_pm_store)(void);
extern void (*zbmac_pm_restore)(void);
extern void (*zbmac_pm_enter)(void);
extern void (*zbmac_pm_exit)(void);
extern void (*zbmac_pm_init)(zbpm_adapter_t *padapter);
extern zbpm_adapter_t *pzbpm_adapter;
#ifdef __cplusplus
}
#endif

#endif /* PWR_MANAG_UNIT_ZBMAC_H */
