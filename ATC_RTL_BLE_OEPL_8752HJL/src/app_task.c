#include <trace.h>
#include "log_uart.h"
#include <os_msg.h>
#include <os_task.h>
#include <os_sched.h>
#include <os_timer.h>
#include <stdio.h>
#include <gap.h>
#include <gap_le.h>
#include <app_task.h>
#include <app_msg.h>
#include <peripheral_app.h>
#include "battery.h"
#include <rtl876x_aon_wdg.h>

/** @defgroup  PERIPH_APP_TASK Peripheral App Task
 * @brief This file handles the implementation of application task related functions.
 *
 * Create App task and handle events & messages
 * @{
 */
/*============================================================================*
 *                              Macros
 *============================================================================*/
#define APP_TASK_PRIORITY   1
#define APP_TASK_STACK_SIZE (512 * 16) /* 8 KB — needed for BLE stack frames + printf */

#define MAX_NUMBER_OF_GAP_MESSAGE 0x20                                                     //!<  GAP message queue size
#define MAX_NUMBER_OF_IO_MESSAGE 0x20                                                      //!<  IO message queue size
#define MAX_NUMBER_OF_EVENT_MESSAGE (MAX_NUMBER_OF_GAP_MESSAGE + MAX_NUMBER_OF_IO_MESSAGE) //!< Event message queue size

/*============================================================================*
 *                              Variables
 *============================================================================*/
void *app_task_handle;  //!< APP Task handle
void *evt_queue_handle; //!< Event queue handle
void *io_queue_handle;  //!< IO queue handle
void *adv_timer_handle; //!< Advertising update timer handle

/*============================================================================*
 *                              Functions
 *============================================================================*/
void app_main_task(void *p_param);
void adv_timer_callback(void *p_timer);

/**
 * @brief  Initialize App task
 * @return void
 */
void app_task_init()
{
    os_task_create(&app_task_handle, "app", app_main_task, 0, APP_TASK_STACK_SIZE, APP_TASK_PRIORITY);
}

/**
 * @brief  Advertising update timer callback
 * @param  p_timer  Timer handle
 * @return void
 */
bool app_send_msg_to_apptask(T_IO_MSG *p_msg)
{
    uint8_t event = EVENT_IO_TO_APP;
    if (!os_msg_send(io_queue_handle, p_msg, 0)) {
        printf("ERR: io_queue full\n");
        return false;
    }
    if (!os_msg_send(evt_queue_handle, &event, 0)) {
        printf("ERR: evt_queue full\n");
        return false;
    }
    return true;
}

void adv_timer_callback(void *p_timer)
{
    printf("Timer: Updating adv data\n");
    set_adv_data(battery_measure_mv());
}

/**
 * @brief        App task to handle events & messages
 * @param[in]    p_param    Parameters sending to the task
 * @return       void
 */
uint32_t loop_count = 0;
void app_main_task(void *p_param)
{
    uint8_t event;
    os_msg_queue_create(&io_queue_handle, MAX_NUMBER_OF_IO_MESSAGE, sizeof(T_IO_MSG));
    os_msg_queue_create(&evt_queue_handle, MAX_NUMBER_OF_EVENT_MESSAGE, sizeof(uint8_t));

    gap_start_bt_stack(evt_queue_handle, io_queue_handle, MAX_NUMBER_OF_GAP_MESSAGE);

    /* Create and start periodic advertising update timer (5 seconds) */
    os_timer_create(&adv_timer_handle, "adv_timer", 0, 10*60000, true, adv_timer_callback);
    os_timer_start(&adv_timer_handle);

    driver_init();

    /* Start the AON watchdog (runs through DLPS) with a 60-second timeout.
     * Fed on every DLPS exit and in adv_timer_callback. */
    aon_wdg_init(1, 30);
    aon_wdg_enable();

    printf("App task started\n");
    while (true)
    {
        loop_count++;
        if (os_msg_recv(evt_queue_handle, &event, 0xFFFFFFFF) == true)
        {
            if (event == EVENT_IO_TO_APP)
            {
                T_IO_MSG io_msg;
                if (os_msg_recv(io_queue_handle, &io_msg, 0) == true)
                {
                    app_handle_io_msg(io_msg);
                }
            }
            else
            {
                gap_handle_msg(event);
            }
        }
    }
}
