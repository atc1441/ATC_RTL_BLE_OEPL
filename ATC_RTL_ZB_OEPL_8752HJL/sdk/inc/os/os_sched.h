/**
*********************************************************************************************************
*               Copyright(c) 2024, Realtek Semiconductor Corporation. All rights reserved.
*********************************************************************************************************
* @file      os_sched.h
* @brief     Header file for os scheduler API.
* @details   This file is used for scheduler start, suspend and resume.
* @author    rui_yu
* @date      2024-12-30
* @version   v1.0
* *********************************************************************************************************
*/

#ifndef _OS_SCHED_H_
#define _OS_SCHED_H_

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif


/**
 * \defgroup    OS_SCHED Kernel Scheduler
 *
 * \brief       Manage the kernel scheduler functions.
 *
 */

/*============================================================================*
*                              Types
*============================================================================*/
/** @defgroup OS_SCHED_Exported_Types OS Scheduler Exported Types
  * \ingroup  OS_SCHED
  * @{
  */

typedef enum
{
    SCHEDULER_SUSPENDED = 0,     /**< Scheduler has been suspended.  */
    SCHEDULER_NOT_STARTED = 1,   /**< Scheduler has not started.  */
    SCHEDULER_RUNNING = 2        /**< Scheduler is running.  */
} SCHEDULER_STATE;

/** End of group OS_SCHED_Exported_Types
  * @}
  */

/*============================================================================*
 *                              Functions
*============================================================================*/
/** @defgroup OS_SCHED_Exported_Functions OS Scheduler Exported Functions
  * \ingroup  OS_SCHED
  * @{
  */


/**
 *
 * \brief       Delay current task for a given period in milliseconds.
 *
 * \param[in]       ms  The amount of time in milliseconds that the current task
 *                  should block.
 *
 * <b>Example usage</b>
 * \code{.c}
 * // Task routine implementation.
 * void task_routine(void *p_param)
 * {
 *     for (;;)
 *     {
 *          // Task code goes here.
 *          os_delay(100);
 *
 *          // Do something here per 100ms.
 *     }
 * }
 * \endcode
 *
 */
void os_delay(uint32_t ms);

/**
 *
 * \brief   Get the time in milliseconds since os_sched_start() API function was called. The precision of time is affected by the precision of the OS tick.
 *
 * \return  The time in milliseconds. Note the time represented by a 64-bit integer
 *          may be overflowed.
 *
 * <b>Example usage</b>
 * \code{.c}
 * // Task routine implementation.
 * void task_routine(void *p_param)
 * {
 *     uint64_t last_time;
 *
 *     // Get the last timestamp.
 *     last_time = os_sys_time_get();
 *
 *     for (;;)
 *     {
 *          // Wait for the next cycle.
 *          os_delay(100);
 *
 *          // Do something here per 100ms.
 *     }
 * }
 * \endcode
 *
 */
uint64_t os_sys_time_get(void);

/**
 *
 * \brief   Get the time in OS tick counts (default 1 tick = 10ms) since os_sched_start() API function was called.
 *
 * \return  The time in OS tick counts. Note the time represented by a 64-bit integer
 *          may be overflowed.
 *
 * <b>Example usage</b>
 * \code{.c}
 * // Task routine implementation.
 * void task_routine(void *p_param)
 * {
 *     uint64_t last_tick;
 *
 *     // Get the last timestamp.
 *     last_tick = os_sys_tick_get();
 *
 *     for (;;)
 *     {
 *          // Wait for the next cycle.
 *          os_delay(100);
 *
 *          // Do something here per 100ms.
 *     }
 * }
 * \endcode
 *
 */
uint64_t os_sys_tick_get(void);

/**
 *
 * \brief   Start the RTOS kernel scheduler.
 *
 * \return  The status of starting kernel scheduler.
 * \retval  true      Scheduler was started successfully.
 * \retval  false     Scheduler failed to start.
 *
 * <b>Example usage</b>
 * \code{.c}
 * int test(void)
 * {
 *     // Create at least one task before starting kernel scheduler.
 *     if (os_task_create(&p_handle, "task", task_routine,
 *                        NULL, STACK_SIZE, TASK_PRIORITY) == true)
 *     {
 *         // Task was created successfully.
 *     }
 *     else
 *     {
 *         // Task creation failed.
 *         return -1;
 *     }
 *
 *     // Start the kernel scheduler.
 *     os_sched_start();
 *
 *     // Will not get here.
 * }
 * \endcode
 *
 */
bool os_sched_start(void);

/**
 *
 * \brief   Stop the RTOS kernel scheduler. All created tasks will be automatically
 *          deleted and multitasking (either preemptive or cooperative) stops.
 *
 * \return  The status of stopping kernel scheduler.
 * \retval  true      Scheduler was stopped successfully.
 * \retval  false     Scheduler failed to stop.
 *
 * <b>Example usage</b>
 * \code{.c}
 * // Task routine implementation.
 * void task_routine(void *p_param)
 * {
 *     for (;;)
 *     {
 *          // Task code goes here.
 *
 *          // At some points, we want to end the real-time kernel processing.
 *          os_sched_stop();
 *     }
 * }
 *
 * int test(void)
 * {
 *     // Create at least one task before starting kernel scheduler.
 *     if (os_task_create(&p_handle, "task", task_routine,
 *                        NULL, STACK_SIZE, TASK_PRIORITY) == true)
 *     {
 *         // Task is created successfully.
 *     }
 *     else
 *     {
 *         // Task failed to create.
 *         return -1;
 *     }
 *
 *     // Start the kernel scheduler.
 *     os_sched_start();
 *
 *     // Will not get here unless a task calls os_sched_stop().
 * }
 * \endcode
 *
 */
bool os_sched_stop(void);

/**
 *
 * \brief   Suspends the kernel scheduler without disabling interrupts. Context
 *          switches will not occur while the scheduler is suspended. After calling
 *          os_sched_suspend(), the calling task will continue to execute without
 *          risk of being swapped out until a call to os_sched_resume() has been made.
 *
 * \return  The status of suspending kernel scheduler.
 * \retval  true      Scheduler was suspended successfully.
 * \retval  false     Scheduler failed to suspend.
 *
 * <b>Example usage</b>
 * \code{.c}
 * // Task routine implementation.
 * void task_routine(void *p_param)
 * {
 *     for (;;)
 *     {
 *          // Task code goes here.
 *
 *          // At some points, the task wants to perform a long operation, and does not
 *          // want to get swapped out.
 *          os_sched_suspend();
 *
 *          // The long operation.
 *
 *          // The operation is completed, and resume the scheduler.
 *          os_sched_resume();
 *     }
 * }
 * \endcode
 *
 */
bool os_sched_suspend(void);

/**
 *
 * \brief   Resume the kernel scheduler after it was suspended by a call
 *          to os_sched_suspend().
 *
 * \return  The status of resuming kernel scheduler.
 * \retval  true      Scheduler was resumed successfully.
 * \retval  false     Scheduler failed to resume.
 *
 * <b>Example usage</b>
 * \code{.c}
 * // Task routine implementation.
 * void task_routine(void *p_param)
 * {
 *     for (;;)
 *     {
 *          // Task code goes here.
 *
 *          // At some points, the task wants to perform a long operation and does not
 *          // want to get swapped out.
 *          os_sched_suspend();
 *
 *          // The long operation.
 *
 *          // The operation is completed, and resume the scheduler.
 *          os_sched_resume();
 *     }
 * }
 * \endcode
 *
 */
bool os_sched_resume(void);

/**
  * @brief  Check if OS scheduler has already been started.
  * @return The status of OS scheduler.
  * \retval  true      Scheduler has already been started.
  * \retval  false     Scheduler has not been started.
  */
bool os_sched_is_start(void);

/**
  * @brief  Get state of OS scheduler.
  * @return The result of OS scheduler state get, refer to @ref SCHEDULER_STATE.
  */
SCHEDULER_STATE os_sched_state_get(void);

/** End of group OS_SCHED_Exported_Functions
  * @}
  */

/**
  * \cond     private
  * \defgroup OS_SCHED_Private_Functions
  * \{
  */

void os_systick_handler(void);

uint64_t os_sys_tick_increase(uint32_t tick_increment);

void os_vector_table_update(void);

void os_init(void);

/**
  *  End of OS_SCHED_Private_Functions
  * \}
  * \endcond
  */

/** End of OS_SCHED
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* _OS_SCHED_H_ */
