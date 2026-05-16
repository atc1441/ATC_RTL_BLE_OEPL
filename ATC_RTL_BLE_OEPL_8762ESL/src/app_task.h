#ifndef _APP_TASK_H_
#define _APP_TASK_H_

#include <stdbool.h>
#include <app_msg.h>

/** @defgroup PERIPH_APP_TASK Peripheral App Task
  * @brief Peripheral App Task
  * @{
  */

extern void driver_init(void);
extern void *io_queue_handle;
extern void *evt_queue_handle;

/**
 * @brief  Initialize App task
 * @return void
 */
void app_task_init(void);

/**
 * @brief  Send an IO message to the app task from any context (task or timer).
 *         Posts the message to io_queue_handle and signals EVENT_IO_TO_APP.
 * @return true on success, false if either queue is full.
 */
bool app_send_msg_to_apptask(T_IO_MSG *p_msg);

/** End of PERIPH_APP_TASK
* @}
*/

#endif
