/**
*********************************************************************************************************
*               Copyright(c) 2024, Realtek Semiconductor Corporation. All rights reserved.
*********************************************************************************************************
* @file      os_mem.h
* @brief     Header file for os memory API.
* @details   This file is used for memory malloc and free.
* @author    rui_yu
* @date      2024-12-30
* @version   v1.0
* *********************************************************************************************************
*/

#ifndef _OS_MEM_H_
#define _OS_MEM_H_

#include <stdint.h>
#include <stddef.h>
#include <mem_types.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \defgroup OS_MEM Memory Management
 *
 * \brief   Allocate, free, and peek memory functions.
 * \details The Memory Management function group allows allocating, freeing, and peeking heap
 *          memory in the system.\n
 *
 */


void *os_mem_alloc_intern(RAM_TYPE ram_type, size_t size,
                          const char *p_func, uint32_t file_line);

void *os_mem_zalloc_intern(RAM_TYPE ram_type, size_t size,
                           const char *p_func, uint32_t file_line);

void *os_mem_aligned_alloc_intern(RAM_TYPE ram_type, size_t size, uint8_t alignment,
                                  const char *p_func, uint32_t file_line);

/*============================================================================*
 *                              Functions
*============================================================================*/
/** @defgroup OS_MEM_Exported_Functions OS Memory Exported Functions
  * \ingroup  OS_MEM
  * @{
  */

/**
 *
 * \brief    Allocate a memory block with required size, using the given RAM type.
 *           If required size is not aligned to 8, the actual allocated block size will
 *           be automatically aligned to 8.
 *
 * \param[in]   ram_type RAM type for allocation, refer to @ref RAM_TYPE.
 *
 * \param[in]   size     Required memory size.
 *
 * \return     The address of the allocated memory block. If the address is NULL, the
 *             memory allocation failed.
 *
 * <b>Example usage</b>
 * \code{.c}
 * int test(void)
 * {
 *     size_t mem_size = 0x1000;
 *     void *p_mem = NULL;
 *
 *     p_mem = os_mem_alloc(RAM_TYPE_DATA_ON, mem_size);
 *     if (p_mem != NULL)
 *     {
 *         // Memory allocation succeeded, and free it.
 *         os_mem_free(p_mem);
 *     }
 *     else
 *     {
 *         // Memory allocation failed.
 *         return -1;
 *     }
 *
 *     return 0;
 * }
 * \endcode
 *
 */
#define os_mem_alloc(ram_type, size)    \
    os_mem_alloc_intern(ram_type, size, __func__, __LINE__)

/**
 *
 * \brief    Allocate and clear a memory block with required size, using the given RAM type.
 *           If required size is not aligned to 8, the actual allocated block size will
 *           be automatically aligned to 8.
 *
 * \param[in]   ram_type RAM type for allocation, refer to @ref RAM_TYPE.
 *
 * \param[in]   size     Required memory size.
 *
 * \return     The address of the allocated memory block. If the address is NULL, the
 *             memory allocation failed.
 *
 * <b>Example usage</b>
 * \code{.c}
 * int test(void)
 * {
 *     size_t mem_size = 0x1000;
 *     void *p_mem = NULL;
 *
 *     p_mem = os_mem_zalloc(RAM_TYPE_DATA_ON, mem_size);
 *     if (p_mem != NULL)
 *     {
 *         // Memory allocation succeeded, and free it.
 *         os_mem_free(p_mem);
 *     }
 *     else
 *     {
 *         // Memory allocation failed.
 *         return -1;
 *     }
 *
 *     return 0;
 * }
 * \endcode
 *
 */
#define os_mem_zalloc(ram_type, size)   \
    os_mem_zalloc_intern(ram_type, size, __func__, __LINE__)

/**
 *
 * \brief    Allocate an aligned memory block with required size.
 *
 * \param[in]   ram_type    RAM type for allocation, refer to @ref RAM_TYPE.
 *
 * \param[in]   size        Required memory size.
 *
 * \param[in]   alignment   Memory alignment in 2^N bytes. If alignment is 0, use
 *                      system default memory alignment (8 Bytes for FreeRTOS).
 *                      The aligned memory block must use os_mem_aligned_free() API function to free.
 *
 * \return     The address of the allocated memory block. If the address is NULL, the
 *             memory allocation failed.
 *
 * <b>Example usage</b>
 * \code{.c}
 * int test(void)
 * {
 *     size_t mem_size = 0x1000;
 *     uint8_t mem_alignment = 16;
 *     void *p_mem = NULL;
 *
 *     p_mem = os_mem_aligned_alloc(RAM_TYPE_DATA_ON, mem_size, mem_alignment);
 *     if (p_mem != NULL)
 *     {
 *         // Aligned memory allocation succeeded, and free it.
 *         os_mem_aligned_free(p_mem);
 *     }
 *     else
 *     {
 *         // Aligned memory allocation failed.
 *         return -1;
 *     }
 *
 *     return 0;
 * }
 * \endcode
 *
 */
#define os_mem_aligned_alloc(ram_type, size, alignment) \
    os_mem_aligned_alloc_intern(ram_type, size, alignment, __func__, __LINE__)

/**
 *
 * \brief    Free a memory block that had been allocated.
 *
 * \param[in]   p_block  The address of memory block being freed.
 *
 * <b>Example usage</b>
 * \code{.c}
 * int test(void)
 * {
 *     size_t mem_size = 0x1000;
 *     void *p_mem = NULL;
 *
 *     p_mem = os_mem_alloc(RAM_TYPE_DATA_ON, mem_size);
 *     if (p_mem != NULL)
 *     {
 *         // Memory allocation succeeded, and free it.
 *         os_mem_free(p_mem);
 *     }
 *     else
 *     {
 *         // Memory allocation failed.
 *         return -1;
 *     }
 *
 *     return 0;
 * }
 * \endcode
 *
 */
void os_mem_free(void *p_block);

/**
 *
 * \brief    Free a memory block that had been aligned allocated.
 *
 * \param[in]    p_block  The address of memory block being freed.
 *
 *
 * <b>Example usage</b>
 * \code{.c}
 * int test(void)
 * {
 *     size_t mem_size = 0x1000;
 *     uint8_t mem_alignment = 16;
 *     void *p_mem = NULL;
 *
 *     p_mem = os_mem_aligned_alloc(RAM_TYPE_DATA_ON, mem_size, mem_alignment);
 *     if (p_mem != NULL)
 *     {
 *         // Aligned memory allocation succeeded, and free it.
 *         os_mem_aligned_free(p_mem);
 *     }
 *     else
 *     {
 *         // Aligned memory allocation failed.
 *         return -1;
 *     }
 *
 *     return 0;
 * }
 * \endcode
 *
 */
void os_mem_aligned_free(void *p_block);

/**
 *
 * \brief    Peek the total unused memory size in the specified RAM type.
 *
 * \param[in]   ram_type            RAM type for peek, refer to @ref RAM_TYPE.
 *
 * \return     The total unused memory size in bytes.
 *
 * <b>Example usage</b>
 * \code{.c}
 * int test(void)
 * {
 *     size_t unused_data_on;
 *     size_t unused_data_off;
 *
 *     // Peek unused DATA ON memory size.
 *     unused_size = os_mem_peek(RAM_TYPE_DATA_ON);
 *
 *     // Peek unused buffer on memory size.
 *     unused_size = os_mem_peek(RAM_TYPE_BUFFER_ON);
 *
 *     return 0;
 * }
 * \endcode
 *
 */
size_t os_mem_peek(RAM_TYPE ram_type);

/** End of group OS_MEM_Exported_Functions
  * @}
  */

/**
  * \cond     private
  * \defgroup OS_MEM_Private_Functions
  * \{
  */


void os_mem_check_heap_usage(void);

/**
  *  End of OS_MEM_Private_Functions
  * \}
  * \endcond
  */

/** End of OS_MEM
  * @}
  */


#ifdef __cplusplus
}
#endif

#endif /* _OS_MEM_H_ */
