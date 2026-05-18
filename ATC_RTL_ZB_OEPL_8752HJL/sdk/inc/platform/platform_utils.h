/**
*****************************************************************************************
*               Copyright(c) 2017, Realtek Semiconductor Corporation. All rights reserved.
************************************************************************************************************
  * @file    platform_utils.h
  * @brief   Utility helper function for user application
  * @author  lory_xu
  * @date    2017-02
  * @version v1.0
  ***************************************************************************************
    * @attention
  * <h2><center>&copy; COPYRIGHT 2017 Realtek Semiconductor Corporation</center></h2>
  ***************************************************************************************
  */

/*============================================================================*
 *               Define to prevent recursive inclusion
 *============================================================================*/
#ifndef _PLATFORM_UTILS_H_
#define _PLATFORM_UTILS_H_

#ifdef __cplusplus
extern "C" {
#endif

/*============================================================================*
 *                               Header Files
*============================================================================*/
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "rtl876x.h"

/** @defgroup  PLATFORM_UTILS Platform Utilities
    * @brief Utility helper functions
    * @{
    */

/*============================================================================*
 *                              Macros
 *============================================================================*/
/** @defgroup PLATFORM_UTILS_Exported_Macros Platform Utilities Exported Macros
  * @brief
  * @{
  */

#define CLOCK_100MHZ    100000000
#define CLOCK_90MHZ     90000000
#define CLOCK_40MHZ     40000000
#define CLOCK_53MHZ     53333333
#define CLOCK_80MHZ     80000000
#define CLOCK_20MHZ     20000000
#define CLOCK_10MHZ     10000000
#define CLOCK_5MHZ      5000000
#define CLOCK_2P5MHZ    2500000
#define CLOCK_1P25MHZ   1250000
#define CLOCK_625KHZ    625000

#ifndef ABS
#define ABS(x)     (((x) > 0) ? (x) : ((~(x))+1))
#endif

#define CLOCK_COUNT_DIFF(clk_begin, clk_end, bit_mask) (((((uint64_t)clk_end & ((uint64_t)bit_mask - 1)) | (uint64_t)bit_mask) - ((uint64_t)clk_begin & ((uint64_t)bit_mask - 1))) & ((uint64_t)bit_mask - 1))

#define BUILD_BUG_ON_ZERO(e)  (sizeof(struct { int _; int:-!!(e); }) - sizeof(struct { int _; }))

#ifndef SAME_TYPE
#define SAME_TYPE(a, b)  __builtin_types_compatible_p(typeof(a), typeof(b))
#endif

#define MUST_BE_ARRAY(a)  BUILD_BUG_ON_ZERO(SAME_TYPE((a), &(*a)))

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(a) ((sizeof(a) / sizeof(*a)) + MUST_BE_ARRAY(a))
#endif

/* =========================== vendor section ===========================*/

#define VENDOR_COUNTER_BASE_ADDRESS 0x40006000

#define VENDOR_COUNTER_READ(Vendor_offset) \
    ((uint32_t)*((volatile uint32_t*)(VENDOR_COUNTER_BASE_ADDRESS+(Vendor_offset))))
#define VENDOR_COUNTER_WRITE(Vendor_offset, Value) \
    ((*((volatile uint32_t*)(VENDOR_COUNTER_BASE_ADDRESS + (Vendor_offset)))) = (Value))

#define VENDOR_BASE_ADDRESS 0x40058000

#define VENDOR_REG_ADDR(offset) (VENDOR_BASE_ADDRESS + (offset))
#define VENDOR_READ(Vendor_offset) \
    ((uint32_t)*((volatile uint32_t*)(VENDOR_BASE_ADDRESS+(Vendor_offset))))
#define VENDOR_WRITE(Vendor_offset, Value) \
    ((*((volatile uint32_t*)(VENDOR_BASE_ADDRESS + (Vendor_offset)))) = (Value))
#define VENDOR_U16_READ(Vendor_offset) \
    ((uint16_t)*((volatile uint16_t*)(VENDOR_BASE_ADDRESS+(Vendor_offset))))
#define VENDOR_U16_WRITE(Vendor_offset, Value) \
    ((*((volatile uint16_t*)(VENDOR_BASE_ADDRESS + (Vendor_offset)))) = (Value))
#define VENDOR_BYTE_READ(Vendor_offset) \
    (*((volatile uint8_t*)(VENDOR_BASE_ADDRESS+(Vendor_offset))))
#define VENDOR_BYTE_WRITE(Vendor_offset, Value) \
    ((*((volatile uint8_t*)(VENDOR_BASE_ADDRESS + (Vendor_offset)))) = (Value))
#define VENDOR_U32_REG_UPDATE(Vendor_offset, Mask, Value) \
    {uint32_t temp; \
        temp = VENDOR_READ(Vendor_offset) & ~(Mask);\
        VENDOR_WRITE(Vendor_offset, (temp | ((Value) & (Mask))));}
#define VENDOR_U16_REG_UPDATE(Vendor_offset, Mask, Value) \
    {uint16_t temp; \
        temp = VENDOR_U16_READ(Vendor_offset) & ~(Mask);\
        VENDOR_U16_WRITE(Vendor_offset, (temp | ((Value) & (Mask))));}
#define VENDOR_U8_REG_UPDATE(Vendor_offset, Mask, Value) \
    {uint8_t temp; \
        temp = VENDOR_BYTE_READ(Vendor_offset) & ~(Mask);\
        VENDOR_BYTE_WRITE(Vendor_offset, (temp | ((Value) & (Mask))));}

/** End of PLATFORM_UTILS_Exported_Macros
  * @}
  */

/*============================================================================*
 *                         Types
 *============================================================================*/
/** @defgroup PLATFORM_UTILS_Exported_Types Platform Utilities Exported Types
  * @brief
  * @{
  */
typedef enum
{
    EXTERNAL_CLOCK  = 0,
    CORE_CLOCK      = 1
} SYSTICK_CLK_SRC_TYPE;

typedef union PON_F00_REG_S  /* Vendor 0xF00 */
{
    uint32_t d32;
    struct
    {
        uint32_t ind_32k_wdata: 16;     //[15:0]  R/W   (32k register write data. Read data)
        uint32_t ind_32k_addr: 6;       //[21:16] R/W   (32k register address)
        uint32_t ind_32k_rw: 1;         //[22]    W     (1: write 32k register)
        uint32_t rsvd: 9;               //[31:23] R/W
    };
} PON_F00_REG_S_TYPE;

/** End of PLATFORM_UTILS_Exported_Types
  * @}
  */

/*============================================================================*
 *                              Functions
 *============================================================================*/
/** @defgroup PLATFORM_UTILS_Exported_Functions Platform Utilities Exported Functions
    * @brief
    * @{
    */

/**
 * @brief Generate random number given max number allowed
 * @param[in] max   to specify max number that is allowed
 * @return random number
 */

extern uint32_t platform_random(uint32_t max);


#if defined ( __CC_ARM )
/**
 * @brief Busy delay for a specified microsecond
 * @param[in] t   to specify t microseconds to delay
 * @return none
 */
extern volatile void (*platform_delay_ms)(uint32_t t);
/**
 * @brief Busy delay for a specified millisecond
 * @param[in] t   to specify t milliseconds to delay
 * @return none
 */
extern volatile void (*platform_delay_us)(uint32_t t);
#elif defined (__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050) || defined (__GNUC__)
/**
 * @brief Busy delay for a specified microsecond
 * @param[in] t   to specify t microseconds to delay
 * @return none
 */
extern void (*platform_delay_ms)(uint32_t t);
/**
 * @brief Busy delay for a specified millisecond
 * @param[in] t   to specify t milliseconds to delay
 * @return none
 */
extern void (*platform_delay_us)(uint32_t t);
#endif

/**
 * @brief Get the vendor timer tick

 * @return the value of vendor timer tick
 *
 * @note Features of the vendor timer:
 *         (1) clock rate is 40M
 *         (2) width is 26 bits (max 0x3FFFFFF)
 *         (3) tick counter is incremental
 *
 */
static inline uint32_t platform_vendor_tick(void)
{
    return HAL_READ32(VENDOR_REG_BASE, 0x30);
}

/** @} */ /* End of group PLATFORM_UTILS_Exported_Functions */

/** @} */ /* End of group PLATFORM_UTILS */


#ifdef __cplusplus
}
#endif

#endif
