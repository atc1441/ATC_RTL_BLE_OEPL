/**
*********************************************************************************************************
*               Copyright(c) 2024, Realtek Semiconductor Corporation. All rights reserved.
*********************************************************************************************************
* @file      os_timer.h
* @brief     Header file for os timer API.
* @details   This file is used for timer create, start, restart and delete.
* @author    rui_yu
* @date      2024-12-30
* @version   v1.0
* *********************************************************************************************************
*/

#ifndef _OS_TIMER_H_
#define _OS_TIMER_H_

#include <stdint.h>
#include <stdbool.h>
//#include "timers.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \defgroup    OS_TIMER Timer Management
 *
 * \brief   Create and control software timer and timer callback functions.
 * \details The Timer Management function group allows creating, deleting, and controlling software
 *          timers in the system. The software timers can be configured as one-shot or periodic
 *          timers. When a timer expires, a callback function associated with the timer is
 *          executed.\n
 * Timers can be in the following two states:\n
 * \arg <b>Dormant</b> The timer first enters Dormant or Inactive state when created.
 *                     If a one-shot timer expires but is not restarted yet, or is stopped, the timer
 *                     will be transformed from Active state into Dormant state.
 * \arg <b>Active</b> The timer enters Active state if the timer starts or restarts. When
 *                    expired, the callback function associated with the timer will be executed.
 *
 * \image html OS-timer-state-transition.jpg "Software Timer State Transition" width=50px height=26px
 *
 */

/*============================================================================*
 *                              Functions
*============================================================================*/
/** @defgroup OS_TIMER_Exported_Functions OS Timer Exported Functions
  * \ingroup  OS_TIMER
  * @{
  */

/**
 *
 * \brief    Get the ID assigned to the timer when created.
 *
 * \param[in]   pp_handle   Pointer to the created timer handle.
 *
 * \param[out]  p_timer_id  Used to pass back the ID assigned to the timer.
 *
 * \return           The status of the timer ID getting.
 * \retval true      Timer ID was got successfully.
 * \retval false     Timer ID failed to get.
 *
 * <b>Example usage</b>
 * \code{.c}
 * // Timer callback function.
 * void timer_callback(void *p_handle)
 * {
 *     uint32_t timer_id;
 *
 *     // Which timer expired?
 *     os_timer_id_get(&p_handle, &timer_id);
 *
 *     if (timer_id == TIMER_ID)
 *     {
 *          // Delete the specified timer.
 *          os_timer_delete(&p_handle);
 *     }
 * }
 *
 * #define TIMER_ID 1
 *
 * // Timer to be created.
 * int test(void)
 * {
 *     void *p_handle = NULL;
 *
 *     if (os_timer_create(&p_handle, "timer", TIMER_ID,
 *                        INTERVAL_MS, false, timer_callback) == true)
 *     {
 *         // Timer created successfully, start the timer.
 *         os_timer_start(&p_handle);
 *     }
 *     else
 *     {
 *         // Timer failed to create.
 *         return -1;
 *     }
 *
 *     return 0;
 * }
 * \endcode
 *
 */
bool os_timer_id_get(void **pp_handle, uint32_t *p_timer_id);

/**
 *
 * \brief   Create a new software timer instance. This allocates the storage required by
 *          the new timer, initializes the new timer's internal state, and returns a handle
 *          by which the new timer can be referenced.
 *
 * \param[out]  pp_handle        Used to pass back a handle by which the created timer
 *                               can be referenced.
 *
 * \param[in]   p_timer_name     A descriptive name for the timer.
 *
 * \param[in]   timer_id         An identifier that is assigned to the timer being created.
 *                               Typically this would be used in the timer callback function to
 *                               identify which timer expired when the same callback function is
 *                               assigned to more than one timer.
 *
 * \param[in]   interval_ms      The timer period in milliseconds.
 *
 * \param[in]   reload           Used to set the timer as a periodic or one-shot timer.
 * \arg \c      true             Create a periodic timer.
 * \arg \c      false            Create a one-shot timer.
 *
 * \param   p_timer_callback The function to call when the timer expires. Callback functions
 *                               must have the prototype defined as 'void callback(void *)'.
 *
 * \return           The status of the timer creation.
 * \retval true      Timer was created successfully.
 * \retval false     Timer failed to create.
 *
 * <b>Example usage</b>
 * \code{.c}
 * // Timer callback function.
 * void timer_callback(void *p_handle)
 * {
 *     uint32_t timer_id;
 *
 *     // Which timer expired?
 *     os_timer_id_get(&p_handle, &timer_id);
 *
 *     if (timer_id == TIMER_ID)
 *     {
 *          // Delete the specified timer.
 *          os_timer_delete(&p_handle);
 *     }
 * }
 *
 * #define TIMER_ID 1
 *
 * // Timer to be created.
 * int test(void)
 * {
 *     void *p_handle = NULL;
 *
 *     if (os_timer_create(&p_handle, "timer", TIMER_ID,
 *                        INTERVAL_MS, false, timer_callback) == true)
 *     {
 *         // Timer created successfully, start the timer.
 *         os_timer_start(&p_handle);
 *     }
 *     else
 *     {
 *         // Timer failed to create.
 *         return -1;
 *     }
 *
 *     return 0;
 * }
 * \endcode
 *
 */
bool os_timer_create(void **pp_handle, const char *p_timer_name, uint32_t timer_id,
                     uint32_t interval_ms, bool reload, void (*p_timer_callback)());

/**
 *
 * \brief    Start a timer that was previously created using the os_timer_create() API
 *           function. If the timer had already been started and was in the active state,
 *           then os_timer_start() will restart it. If the timer was in the dormant state,
 *           os_timer_start() will change it to active state and restart.
 *
 * \param pp_handle Pointer to the created timer handle.
 *
 * \return           The status of the timer starting.
 * \retval true      Timer was started successfully.
 * \retval false     Timer failed to start.
 *
 * <b>Example usage</b>
 * \code{.c}
 * // Timer callback function.
 * void timer_callback(void *p_handle)
 * {
 *     uint32_t timer_id;
 *
 *     // Which timer expired?
 *     os_timer_id_get(&p_handle, &timer_id);
 *
 *     if (timer_id == TIMER_ID)
 *     {
 *          // Delete the specified timer.
 *          os_timer_delete(&p_handle);
 *     }
 * }
 *
 * #define TIMER_ID 1
 *
 * // Timer to be created.
 * int test(void)
 * {
 *     void *p_handle = NULL;
 *
 *     if (os_timer_create(&p_handle, "timer", TIMER_ID,
 *                        INTERVAL_MS, false, timer_callback) == true)
 *     {
 *         // Timer created successfully, start the timer.
 *         os_timer_start(&p_handle);
 *     }
 *     else
 *     {
 *         // Timer failed to create.
 *         return -1;
 *     }
 *
 *     return 0;
 * }
 * \endcode
 *
 */
bool os_timer_start(void **pp_handle);

/**
 *
 * \brief     Change timer periods and restart a timer that was previously created.
 *            If the timer had already been started and was already in the active
 *            state, then os_timer_restart() will cause the timer to re-evaluate its expiry
 *            time and restart. If the timer was in the dormant state then os_timer_restart()
 *            will change it to active state and start in the new timer period.
 *
 * \param pp_handle     Pointer to the created timer handle.
 *
 * \param interval_ms   The timer period in milliseconds.
 *
 * \return           The status of the timer restarting.
 * \retval true      Timer was restarted successfully.
 * \retval false     Timer failed to restart.
 *
 * <b>Example usage</b>
 * \code{.c}
 * // Timer callback function.
 * void timer_callback(void *p_handle)
 * {
 *     uint32_t timer_id;
 *
 *     // Which timer expired?
 *     os_timer_id_get(&p_handle, &timer_id);
 *
 *     if (timer_id == TIMER_ID)
 *     {
 *          // Restart the specified timer.
 *          os_timer_restart(&p_handle);
 *     }
 * }
 *
 * #define TIMER_ID 1
 *
 * // Timer to be created.
 * int test(void)
 * {
 *     void *p_handle = NULL;
 *
 *     if (os_timer_create(&p_handle, "timer", TIMER_ID,
 *                        INTERVAL_MS, false, timer_callback) == true)
 *     {
 *         // Timer created successfully, start the timer.
 *         os_timer_start(&p_handle);
 *     }
 *     else
 *     {
 *         // Timer failed to create.
 *         return -1;
 *     }
 *
 *     return 0;
 * }
 * \endcode
 *
 */
bool os_timer_restart(void **pp_handle, uint32_t interval_ms);

/**
 *
 * \brief     Stop a timer that was previously started using either of the os_timer_start(),
 *            os_timer_restart() API functions. Stopping a timer ensures the timer is not in
 *            the active state.
 *
 * \param pp_handle Pointer to the handle of timer being stopped.
 *
 * \return           The status of the timer stopping.
 * \retval true      Timer was stopped successfully.
 * \retval false     Timer failed to stop.
 *
 * <b>Example usage</b>
 * \code{.c}
 * // Timer callback function.
 * void timer_callback(void *p_handle)
 * {
 *     uint32_t timer_id;
 *
 *     // Which timer expired?
 *     os_timer_id_get(&p_handle, &timer_id);
 *
 *     if (timer_id == TIMER_ID)
 *     {
 *          // Delete the specified timer.
 *          os_timer_delete(&p_handle);
 *     }
 * }
 *
 * #define TIMER_ID 1
 *
 * // Timer to be created and stopped.
 * int test(void)
 * {
 *     void *p_handle = NULL;
 *
 *     if (os_timer_create(&p_handle, "timer", TIMER_ID,
 *                        INTERVAL_MS, false, timer_callback) == true)
 *     {
 *         // Timer created successfully, start the timer.
 *         os_timer_start(&p_handle);
 *     }
 *     else
 *     {
 *         // Timer failed to create.
 *         return -1;
 *     }
 *
 *     // Stop the timer before expiration.
 *     os_timer_stop(&p_handle);
 *
 *     return 0;
 * }
 * \endcode
 *
 */
bool os_timer_stop(void **pp_handle);

/**
 *
 * \brief      Delete a timer that was previously created using the os_timer_create()
 *             API function.
 *
 * \param pp_handle Pointer to the handle of timer being deleted.
 *
 * \return           The status of the timer deletion.
 * \retval true      Timer was deleted successfully.
 * \retval false     Timer failed to delete.
 *
 * <b>Example usage</b>
 * \code{.c}
 * // Timer callback function.
 * void timer_callback(void *p_handle)
 * {
 *     uint32_t timer_id;
 *
 *     // Which timer expired?
 *     os_timer_id_get(&p_handle, &timer_id);
 *
 *     if (timer_id == TIMER_ID)
 *     {
 *          // Delete the specified timer.
 *          os_timer_delete(&p_handle);
 *     }
 * }
 *
 * #define TIMER_ID 1
 *
 * // Timer to be created and deleted.
 * int test(void)
 * {
 *     void *p_handle = NULL;
 *
 *     if (os_timer_create(&p_handle, "timer", TIMER_ID,
 *                        INTERVAL_MS, false, timer_callback) == true)
 *     {
 *         // Timer created successfully, start the timer.
 *         os_timer_start(&p_handle);
 *     }
 *     else
 *     {
 *         // Timer failed to create.
 *         return -1;
 *     }
 *
 *     return 0;
 * }
 * \endcode
 *
 */
bool os_timer_delete(void **pp_handle);

/**
 *
  * @brief  Dump info for OS Timer.
  * @return Result of operation
  * \retval true  Get state of SW timer successfully.
  * \retval false Get state of SW timer failed.
 *
 * \ingroup  Timer
 */
bool os_timer_dump(void);

/**
 *
  * @brief  Get state of SW timer, similar to os_timer_is_timer_active() API.
  * @param  pp_handle Pointer to the handle of timer
  * @param  p_timer_state State of SW timer
  * @return Result of operation
  * \retval true  Get state of SW timer successfully.
  * \retval false Get state of SW timer failed.
 *
 */


bool os_timer_state_get(void **pp_handle, uint32_t *p_timer_state);

/** End of group OS_TIMER_Exported_Functions
  * @}
  */

/**
  * \cond     private
  * \defgroup OS_TIMER_Private_Functions
  * \{
  */

typedef void (*PendedFunctionOS_t)(void *para1, uint32_t para2);

void os_timer_init(void);

bool os_timer_number_get(void **pp_handle, uint32_t *p_timer_num);
bool os_timer_pend_function_call(PendedFunctionOS_t xFunctionToPend, void *para1, uint32_t para2);
bool os_timer_get_auto_reload(void **pp_handle, long *p_autoreload);
bool os_timer_is_timer_active(void **pp_handle);

/**
  *  End of OS_TIMER_Private_Functions
  * \}
  * \endcond
  */

/** End of OS_TIMER
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* _OS_TIMER_H_ */
