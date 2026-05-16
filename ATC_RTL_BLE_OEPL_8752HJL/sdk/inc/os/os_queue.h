/**
*********************************************************************************************************
*               Copyright(c) 2024, Realtek Semiconductor Corporation. All rights reserved.
*********************************************************************************************************
* @file      os_queue.h
* @brief     Header file for os queue API.
* @details   This file is used for queue init, in, out and peek.
* @author    rui_yu
* @date      2024-12-30
* @version   v1.0
* *********************************************************************************************************
*/

#ifndef _OS_QUEUE_H_
#define _OS_QUEUE_H_

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \defgroup OS_QUEUE List Queue
 *
 * \brief   Initialize and manage List Queue functions.
 * \details List Queue is designed as a FIFO-like list, which can enqueue, dequeue and peek
 *          the list. While, List Queue also keeps these functionalities such as deleting
 *          and inserting the specified list item.
 *
 * \image html OS-queue-overview.jpg "List Queue Overview" width=50px height=28px
 *
 */

/*============================================================================*
 *                         Types
 *============================================================================*/
/** @defgroup OS_QUEUE_Exported_Types OS Queue Exported Types
  * \ingroup  OS_QUEUE
  * @brief
  * @{
  */

/**
 *
 * \brief   The element structure of List Queue.
 *
 */
typedef struct t_os_queue_elem
{
    struct t_os_queue_elem *p_next; /**< Pointer to next list queue element. */
} T_OS_QUEUE_ELEM;

/**
 *
 * \brief   The header structure of List Queue.
 *
 */
typedef struct t_os_queue
{
    T_OS_QUEUE_ELEM *p_first;   /**< Pointer to the first queue element. */
    T_OS_QUEUE_ELEM *p_last;    /**< Pointer to the last queue element. */
    uint16_t         count;     /**< The queue element count. */
    uint16_t         flags;     /**< The flags for customer usage. */
} T_OS_QUEUE;

/** End of group WDG_Exported_Types
  * @}
  */

/*============================================================================*
 *                              Functions
*============================================================================*/
/** @defgroup OS_QUEUE_Exported_Functions OS Queue Exported Functions
  * \ingroup  OS_QUEUE
  * @{
  */

/**
 *
 * \brief   Initialize the list queue.
 *
 * \param[out]   p_queue  Pointer to the list queue header.
 *
 * <b>Example usage</b>
 * \code{.c}
 * T_OS_QUEUE  test_queue;
 *
 * int test(void)
 * {
 *     // Initialize the queue before operating it.
 *     os_queue_init(&test_queue);
 * }
 * \endcode
 *
 */
void  os_queue_init(T_OS_QUEUE *p_queue);

/**
 * os_queue.h
 *
 * \brief   Enqueue an element to the end of the list queue.
 *
 * \param[in]   p_queue Pointer to the list queue header.
 *
 * \param[in]   p_elem  The pointer of list queue element being enqueued.
 *
 * <b>Example usage</b>
 * \code{.c}
 * struct test_item
 * {
 *     struct test_item *p_next; // Pointer to the next item, must be the first field.
 *     uint32_t          id;
 *     uint8_t           data[10];
 * }
 *
 * T_OS_QUEUE  test_queue;
 *
 * struct test_item a_item;
 *
 * int test(void)
 * {
 *     // Initialize the queue before operating it.
 *     os_queue_init(&test_queue);
 *
 *     // Enqueue the item.
 *     os_queue_in(&test_queue, &a_item);
 * }
 * \endcode
 *
 */
void  os_queue_in(T_OS_QUEUE *p_queue, void *p_elem);

/**
 *
 * \brief   Dequeue an element from the front of the list queue.
 *
 * \param[in]   p_queue  Pointer to the list queue header.
 *
 * \return  The pointer of first element from the list queue. If the returned address is
 *          NULL, the list queue is empty.
 *
 * <b>Example usage</b>
 * \code{.c}
 * struct test_item
 * {
 *     struct test_item *p_next; // Pointer to the next item, must be the first field.
 *     uint32_t          id;
 *     uint8_t           data[10];
 * }
 *
 * T_OS_QUEUE  test_queue;
 *
 * struct test_item a_item;
 *
 * int test(void)
 * {
 *     struct test_item *p_item;
 *
 *     // Initialize the queue before operating it.
 *     os_queue_init(&test_queue);
 *
 *     // Enqueue the item.
 *     os_queue_in(&test_queue, &a_item);
 *
 *     // Then dequeue the item from the list queue.
 *     p_item = os_queue_out(&test_queue);
 * }
 * \endcode
 *
 */
void *os_queue_out(T_OS_QUEUE *p_queue);

/**
 *
 * \brief  Peek an element from the list queue. The peeked element will not be dequeued.
 *
 * \param[in]   p_queue Pointer to the list queue header.
 *
 * \return  The pointer of first element from the list queue. If the returned address is
 *          NULL, the list queue is empty.
 *
 * <b>Example usage</b>
 * \code{.c}
 * struct test_item
 * {
 *     struct test_item *p_next; // Pointer to the next item, must be the first field.
 *     uint32_t          id;
 *     uint8_t           data[10];
 * }
 *
 * T_OS_QUEUE  test_queue;
 *
 * struct test_item a_item;
 *
 * int test(void)
 * {
 *     struct test_item *p_item;
 *
 *     // Initialize the queue before operating it.
 *     os_queue_init(&test_queue);
 *
 *     // Enqueue the item.
 *     os_queue_in(&test_queue, &a_item);
 *
 *     // Peek but not remove the first item from the list queue.
 *     p_item = os_queue_peek(&test_queue);
 * }
 * \endcode
 *
 */
void *os_queue_peek(T_OS_QUEUE *p_queue);

/**
 *
 * \brief   Insert an element into the list such that the new element is inserted after a given element already in the list.
 *
 * \param[in]   p_queue     Pointer to the list queue header.
 *
 * \param[in]   p_elem      The pointer of element which the new element to be inserted behind.
 *
 * \param[in]   p_new_elem  The pointer of inserted element.
 *
 * <b>Example usage</b>
 * \code{.c}
 * struct test_item
 * {
 *     struct test_item *p_next; // Pointer to the next item, must be the first field.
 *     uint32_t          id;
 *     uint8_t           data[10];
 * }
 *
 * T_OS_QUEUE  test_queue;
 *
 * struct test_item item1;
 * struct test_item item2;
 * struct test_item item3;
 *
 * int test(void)
 * {
 *     struct test_item *p_item;
 *
 *     // Initialize the queue before operating it.
 *     os_queue_init(&test_queue);
 *
 *     // Enqueue the item 1.
 *     os_queue_in(&test_queue, &item1);
 *
 *     // Enqueue the item 2.
 *     os_queue_in(&test_queue, &item2);
 *
 *     // Insert the item 3 behind item 1 but before item 2.
 *     os_queue_insert(&test_queue, &item1, &item3);
 * }
 * \endcode
 *
 */
void  os_queue_insert(T_OS_QUEUE *p_queue, void *p_elem, void *p_new_elem);

/**
 *
 * \brief   Delete an element from the list queue.
 *
 * \param[in]   p_queue  Pointer to the list queue header.
 *
 * \param[in]   p_elem   The pointer of element to be deleted from the list queue.
 *
 * \return          The status of queue element deletion.
 * \retval true     Queue element was deleted successfully.
 * \retval false    Queue element was failed to delete when the queue is empty
 *                  or the being deleted queue element is not belonged to the queue.
 *
 * <b>Example usage</b>
 * \code{.c}
 * struct test_item
 * {
 *     struct test_item *p_next; // Pointer to the next item, must be the first field.
 *     uint32_t          id;
 *     uint8_t           data[10];
 * }
 *
 * T_OS_QUEUE  test_queue;
 *
 * struct test_item item1;
 * struct test_item item2;
 * struct test_item item3;
 *
 * int test(void)
 * {
 *     struct test_item *p_item;
 *
 *     // Initialize the queue before operating it.
 *     os_queue_init(&test_queue);
 *
 *     // Enqueue the item 1.
 *     os_queue_in(&test_queue, &item1);
 *
 *     // Enqueue the item 2.
 *     os_queue_in(&test_queue, &item2);
 *
 *     // Enqueue the item 3.
 *     os_queue_in(&test_queue, &item3);
 *
 *     // Then delete the item 2.
 *     os_queue_delete(&test_queue, &item2);
 * }
 * \endcode
 *
 */
bool  os_queue_delete(T_OS_QUEUE *p_queue, void *p_elem);

/** End of group OS_QUEUE_Exported_Functions
  * @}
  */

/** End of OS_QUEUE
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif  /* _OS_QUEUE_H_ */
