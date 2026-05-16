/**
*********************************************************************************************************
*               Copyright(c) 2024, Realtek Semiconductor Corporation. All rights reserved.
*********************************************************************************************************
* @file      os_sync.h
* @brief     Header file for os synchronization API.
* @details   This file is used for semaphore and mutex.
* @author    rui_yu
* @date      2024-12-30
* @version   v1.0
* *********************************************************************************************************
*/

#ifndef _OS_SYNC_H_
#define _OS_SYNC_H_

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \defgroup    OS_SYNC Inter-Task Communication
 *
 * \brief   Manage inter-task communication functions.
 * \details Tasks need to communicate with each other or access shared resources together.
 *          There are many ways to exchange data between tasks, for example using shared
 *          data, polling loops, and message passing.\n
 *          Many resources can be considered as serially-reusable. This means that they
 *          can be used repeatedly by different tasks, but only by one task at a time.\n
 *          The following mechanisms are available to the user:\n
 * \arg <b>Lock</b>
 * \arg <b>Semaphore</b>
 * \arg <b>Mutex</b>
 *
 */

/*============================================================================*
 *                              Functions
*============================================================================*/
/** @defgroup OS_SYNC_Exported_Functions OS Synchronization Exported Functions
  * \ingroup  OS_SYNC
  * @{
  */

/**
 *
 * \brief   Enter the critical region. Disable preemptive context switch and interrupts:
 *
 * 1), When called in task, irq mask flag will be ignored and always return 0. If uxCriticalNesting is 0, record current PRIMASK in variable uxPrimaskBackup. Then PRIMASK is set to 1, and uxCriticalNesting is added 1.
 *
 * 2), When called in isr, irq mask flag will record current PRIMASK before setting PRIMASK to 1.
 *
 * \return  Interrupt mask flag.
 *
 * <b>Example usage</b>
 * \code{.c}
 * int test(void)
 * {
 *     uint32_t s;
 *
 *     // Enter the critical section.
 *     s = os_lock();
 *     // Allow only one task or ISR to operate the list.
 *     list_add(p_list, &item);
 *     // Exit the critical section and restore ISR mask flag.
 *     os_unlock(s);
 * }
 * \endcode
 *
 */
uint32_t os_lock(void);

/**
 *
 * \brief   Exit the critical region. Enable preemptive context switch and interrupts:
 *
 * 1), When called in task, irq mask flag will be ignored. The uxCriticalNesting is subtracted 1. If uxCriticalNesting is subtracted to 0, set PRIMASK to the value of uxPrimaskBackup.
 *
 * 2), When called in isr, set PRIMASK to the value of isr mask flag.
 *
 * \param[in]   flag Interrupt mask flag to be restored.
 *
 *
 * <b>Example usage</b>
 * \code{.c}
 * int test(void)
 * {
 *     uint32_t s;
 *
 *     // Enter the critical section.
 *     s = os_lock();
 *     // Allow only one task or ISR to operate the list.
 *     list_add(p_list, &item);
 *     // Exit the critical section and restore ISR mask flag.
 *     os_unlock(s);
 * }
 * \endcode
 *
 */
void os_unlock(uint32_t flag);

/**
 *
 * \brief   Create a counting semaphore.
 *
 * \details Semaphores are used to manage and protect access to shared resources. A semaphore
 *          can be used to permit a fixed number of tasks to access a pool of shared resources.
 *
 *          A semaphore object should be initialized to the maximum number of available tokens.
 *          This number of available resources is specified as a parameter of the os_sem_create()
 *          function. Each time a semaphore token is obtained with os_sem_take(), the semaphore
 *          count is decremented. When the semaphore count is 0, no semaphore token can be obtained.
 *          The task that tries to obtain the semaphore token needs to wait until the next token
 *          is free. Semaphores are released with os_sem_give(), incrementing the semaphore count.
 *
 * \image html OS-semaphore-overview.jpg "Semaphore Overview" width=49px height=34px
 *
 * \param[out]  pp_handle   Pointer used to pass back the created semaphore handle.
 *
 * \param[in]   p_name      The name assigned to the semaphore when created, only for debug.
 *
 * \param[in]   init_count  The count value assigned to the semaphore when created.
 *
 * \param[in]   max_count   The maximum count value that can be reached. If the max_count is 1,
 *                          a binary semaphore is being created.
 *
 * \return           The status of the semaphore creation.
 * \retval true      Semaphore was created successfully.
 * \retval false     Semaphore was failed to create.
 *
 * <b>Example usage</b>
 * \code{.c}
 * int test(void)
 * {
 *     void *p_handle;
 *
 *     // Create a semaphore with initial value 0 and maximum value 10.
 *     if (os_sem_create(&p_handle, "sem", 0, 10) == true)
 *     {
 *         // Semaphore is created successfully.
 *     }
 *     else
 *     {
 *         // Semaphore failed to create.
 *         return -1;
 *     }
 *
 *     return 0;
 * }
 * \endcode
 *
 */
bool os_sem_create(void **pp_handle, const char *p_name, uint32_t init_count, uint32_t max_count);

/**
 *
 * \brief   Delete a semaphore.
 *
 * \param[in]   p_handle    The handle of the semaphore to be deleted.
 *
 * \return           The status of the semaphore deletion.
 * \retval true      Semaphore was deleted successfully.
 * \retval false     Semaphore failed to delete.
 *
 * <b>Example usage</b>
 * \code{.c}
 * int test(void)
 * {
 *     void *p_handle;
 *
 *     // Create a semaphore with initial value 0 and maximum value 10.
 *     if (os_sem_create(&p_handle, "sem", 0, 10) == true)
 *     {
 *         // Semaphore is created successfully.
 *     }
 *     else
 *     {
 *         // Semaphore failed to create.
 *         return -1;
 *     }
 *
 *     // Delete the created semaphore.
 *     os_sem_delete(p_handle);
 *
 *     return 0;
 * }
 * \endcode
 *
 */
bool os_sem_delete(void *p_handle);

/**
 *
 * \brief   Take a semaphore.
 *
 * \param[in]   p_handle    The handle of the semaphore to be taken.
 *
 * \param[in]   wait_ms     The time in milliseconds to wait for the semaphore to become
 *                          available.
 * \arg \c 0           No blocking and return immediately.
 * \arg \c 0xFFFFFFFF  Block infinitely until the semaphore is taken.
 * \arg \c others      The timeout value in milliseconds.
 *
 * \return           The status of the semaphore taking.
 * \retval true      Semaphore was taken successfully.
 * \retval false     Semaphore failed to be taken.
 *
 * <b>Example usage</b>
 * \code{.c}
 * void *p_handle = NULL;
 *
 * // One task creates a semaphore.
 * void task1(void *p_param)
 * {
 *     // Create a full binary semaphore.
 *     os_sem_create(&p_handle, 1, 1);
 * }
 *
 * // Another task uses the semaphore.
 * void task2(void *p_param)
 * {
 *     // See if we can obtain the semaphore. If the semaphore is
 *     // not available, wait for 100ms.
 *     if (os_sem_take(p_handle, 100) == true)
 *     {
 *         // Access the shared resource.
 *
 *         // Finish accessing the shared resource, then release the semaphore.
 *         os_sem_give(p_handle);
 *     }
 *     else
 *     {
 *         // Could not access the shared resource.
 *     }
 * }
 * \endcode
 *
 */
bool os_sem_take(void *p_handle, uint32_t wait_ms);

/**
 *
 * \brief   Give a semaphore.
 *
 * \param[in]   p_handle    The handle of the semaphore to be given.
 *
 * \return           The status of the semaphore giving.
 * \retval true      Semaphore was given successfully.
 * \retval false     Semaphore failed to be given.
 *
 * <b>Example usage</b>
 * \code{.c}
 * int test(void)
 * {
 *     void *p_handle = NULL;
 *
 *     // Create an empty binary semaphore.
 *     os_sem_create(&p_handle, 0, 1);
 *
 *     // Immediately obtaining the semaphore will fail.
 *     if (os_sem_take(p_handle, 0) == false)
 *     {
 *         // Failed.
 *     }
 *
 *     // Give the semaphore.
 *     if (os_sem_give(p_handle) == true)
 *     {
 *         // Now we can take the semaphore.
 *         os_sem_take(p_handle, 0);
 *
 *         // Again, taking the binary semaphore will fail.
 *         os_sem_take(p_handle, 0);
 *     }
 * }
 * \endcode
 *
 */
bool os_sem_give(void *p_handle);

/**
 *
 * \brief   Create a mutex.
 *
 * \details Mutex (Mutual Exclusion) is used to protect a shared resource that can be accessed
 *          only by one task at a time.
 *          A mutex is a special version of a binary empty semaphore. The advantage of a mutex
 *          is that it provides ownership of the task. When a task acquires a mutex and becomes its
 *          owner, subsequent mutex acquires from that task will succeed immediately. Thus, mutex
 *          acquires/releases can be nested.
 *
 * \image html OS-mutex-overview.jpg "Mutex Overview" width=45px height=19px
 *
 * \param[out]  pp_handle   Used to pass back the created mutex handle.
 *
 * \return           The status of the mutex creation.
 * \retval true      Mutex was created successfully.
 * \retval false     Mutex failed to be created.
 *
 * <b>Example usage</b>
 * \code{.c}
 * int test(void)
 * {
 *     void *p_handle = NULL;
 *
 *     // Create a mutex.
 *     if (os_mutex_create(&p_handle) == true)
 *     {
 *         // The mutex is created successfully.
 *         // Now it can be used.
 *     }
 * }
 * \endcode
 *
 */
bool os_mutex_create(void **pp_handle);

/**
 *
 * \brief   Delete a mutex.
 *
 * \param[in]   p_handle    The handle of the mutex to be deleted.
 *
 * \return           The status of the mutex deletion.
 * \retval true      Mutex was deleted successfully.
 * \retval false     Mutex failed to be deleted.
 *
 * <b>Example usage</b>
 * \code{.c}
 * int test(void)
 * {
 *     void *p_handle;
 *
 *     // Create a mutex.
 *     if (os_mutex_create(&p_handle) == true)
 *     {
 *         // Mutex is created successfully.
 *     }
 *     else
 *     {
 *         // Mutex failed to be created.
 *         return -1;
 *     }
 *
 *     // Delete the created mutex.
 *     os_mutex_delete(p_handle);
 *
 *     return 0;
 * }
 * \endcode
 *
 */
bool os_mutex_delete(void *p_handle);

/**
 *
 * \brief   Take a mutex.
 *
 * \param[in]   p_handle    The handle of the mutex to be taken.
 *
 * \param[in]   wait_ms     The time in milliseconds to wait for the mutex to become
 *                          available.
 * \arg \c 0           No blocking and return immediately.
 * \arg \c 0xFFFFFFFF  Block infinitely until the mutex is taken.
 * \arg \c others      The timeout value in milliseconds.
 *
 * \return           The status of the mutex taking.
 * \retval true      Mutex was taken successfully.
 * \retval false     Mutex failed to be taken.
 *
 * <b>Example usage</b>
 * \code{.c}
 * void *p_handle = NULL;
 *
 * // One task creates a mutex.
 * void task1(void *p_param)
 * {
 *     // Create a mutex.
 *     os_mutex_create(&p_handle);
 * }
 *
 * // Another task uses the mutex.
 * void task2(void *p_param)
 * {
 *     // See if we can obtain the mutex. If the mutex is
 *     // not available, wait for 100ms.
 *     if (os_mutex_take(p_handle, 100) == true)
 *     {
 *         // Access the shared resource.
 *
 *         // In real code, recursive calls of mutex may occur.
 *         os_mutex_take(p_handle, 100);
 *         os_mutex_take(p_handle, 200);
 *
 *         // The mutex has now been taken three times, so it will not be
 *         // available to another task until it has also been given back
 *         // three times.
 *         os_mutex_give(p_handle);
 *         os_mutex_give(p_handle);
 *         os_mutex_give(p_handle);
 *
 *         // Finish accessing the shared resource, and the mutex has been released.
 *         // Now the mutex can be taken by other tasks.
 *     }
 *     else
 *     {
 *         // Could not access the shared resource.
 *     }
 * }
 * \endcode
 *
 */
bool os_mutex_take(void *p_handle, uint32_t wait_ms);

/**
 *
 * \brief   Give a mutex.
 *
 * \param[in]   p_handle    The handle of the mutex to be given.
 *
 * \return           The status of the mutex giving.
 * \retval true      Mutex was given successfully.
 * \retval false     Mutex failed to be given.
 *
 * <b>Example usage</b>
 * \code{.c}
 * void *p_handle = NULL;
 *
 * // One task creates a mutex.
 * void task1(void *p_param)
 * {
 *     // Create a mutex.
 *     os_mutex_create(&p_handle);
 * }
 *
 * // Another task uses the mutex.
 * void task2(void *p_param)
 * {
 *     // See if we can obtain the mutex. If the mutex is
 *     // not available, wait for 100ms.
 *     if (os_mutex_take(p_handle, 100) == true)
 *     {
 *         // Access the shared resource.
 *
 *         // In real code, recursive calls of mutex may occur.
 *         os_mutex_take(p_handle, 100);
 *         os_mutex_take(p_handle, 200);
 *
 *         // The mutex has now been taken three times, so it will not be
 *         // available to another task until it has also been given back
 *         // three times.
 *         os_mutex_give(p_handle);
 *         os_mutex_give(p_handle);
 *         os_mutex_give(p_handle);
 *
 *         // Finish accessing the shared resource, then release the mutex.
 *         // Now the mutex can be taken by other tasks.
 *     }
 *     else
 *     {
 *         // Could not access the shared resource.
 *     }
 * }
 * \endcode
 *
 */
bool os_mutex_give(void *p_handle);

/** End of group OS_SYNC_Exported_Functions
  * @}
  */

/** End of OS_SYNC
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* _OS_SYNC_H_ */
