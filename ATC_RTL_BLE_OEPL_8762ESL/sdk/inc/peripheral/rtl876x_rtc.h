/**
*********************************************************************************************************
*               Copyright(c) 2020, Realtek Semiconductor Corporation. All rights reserved.
*********************************************************************************************************
* \file     rtl876x_rtc.h
* \brief    The header file of the peripheral RTC driver.
* \details  This file provides all RTC firmware functions.
* \author   yuan
* \date     2020-06-01
* \version  v2.1.0
* *********************************************************************************************************
*/

#ifndef _RTL876X_RTC_H_
#define _RTL876X_RTC_H_

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \addtogroup  IO          Peripheral Drivers
 * \defgroup    RTC         RTC
 *
 * \brief       Manage the RTC peripheral functions.
 *
 * \ingroup     IO
 */


/*============================================================================*
 *                         Includes
 *============================================================================*/
#include "rtl876x.h"
#include "rtl876x_alias.h"


/*============================================================================*
 *                         Registers Definitions
 *============================================================================*/

/* Peripheral: RTC */
/* Description: Real time counter register defines */

/* Register: CR0 */
/* Description: RTC Control Register 0. Offset: 0x00. Address: 0x40000100. */

/* CR0[31] :RTC_RST. Enable interrupt signal to MCU.  0x1: Enable. 0x0: Disable. */
#define RTC_RST_Pos                     (31UL)
#define RTC_RST_Msk                     (0x1UL << RTC_RST_Pos)
#define RTC_RST_CLR                     (~(RTC_RST_Msk))

/* CR0[30] :RTC_NV_EN. Enable interrupt signal to MCU.  0x1: Enable. 0x0: Disable. */
#define RTC_NV_EN_Pos                   (30UL)
#define RTC_NV_EN_Msk                   (0x1UL << RTC_NV_EN_Pos)
#define RTC_NV_EN_CLR                   (~(RTC_NV_EN_Msk))

/* CR0[29] :RTC_WAKEUP_EN. Enable wakeup signal to AON register.  0x1: Enable. 0x0: Disable. */
#define RTC_WAKEUP_EN_Pos               (29UL)
#define RTC_WAKEUP_EN_Msk               (0x1UL << RTC_WAKEUP_EN_Pos)
#define RTC_WAKEUP_EN_CLR               (~(RTC_WAKEUP_EN_Msk))

/* CR0[23] :RTC_COMP3_WAKEUP_EN. Enable or disable compare3 wakeup function.  0x1: Enable. 0x0: Disable. */
#define RTC_COMP3_WAKEUP_EN_Pos         (23UL)
#define RTC_COMP3_WAKEUP_EN_Msk         (0x1UL << RTC_COMP3_WAKEUP_EN_Pos)
#define RTC_COMP3_WAKEUP_EN_Clr         (~(RTC_COMP3_WAKEUP_EN_Msk))

/* CR0[22] :RTC_INT_EN. Enable or disable compare2 wakeup function.  0x1: Enable. 0x0: Disable. */
#define RTC_COMP2_WAKEUP_EN_Pos         (22UL)
#define RTC_COMP2_WAKEUP_EN_Msk         (0x1UL << RTC_COMP2_WAKEUP_EN_Pos)
#define RTC_COMP2_WAKEUP_EN_Clr         (~(RTC_COMP2_WAKEUP_EN_Msk))

/* CR0[21] :RTC_INT_EN. Enable or disable compare1 wakeup function.  0x1: Enable. 0x0: Disable. */
#define RTC_COMP1_WAKEUP_EN_Pos         (21UL)
#define RTC_COMP1_WAKEUP_EN_Msk         (0x1UL << RTC_COMP1_WAKEUP_EN_Pos)
#define RTC_COMP1_WAKEUP_EN_Clr         (~(RTC_COMP1_WAKEUP_EN_Msk))

/* CR0[20] :RTC_INT_EN. Enable or disable compare0 wakeup function.  0x1: Enable. 0x0: Disable. */
#define RTC_COMP0_WAKEUP_EN_Pos         (20UL)
#define RTC_COMP0_WAKEUP_EN_Msk         (0x1UL << RTC_COMP0_WAKEUP_EN_Pos)
#define RTC_COMP0_WAKEUP_EN_Clr         (~(RTC_COMP0_WAKEUP_EN_Msk))

/* CR0[19] :RTC_INT_EN. Enable or disable compare3 interrupt.  0x1: Enable. 0x0: Disable. */
#define RTC_INT_COMP3_EN_Pos            (19UL)
#define RTC_INT_COMP3_EN_Msk            (0x1UL << RTC_INT_COMP3_EN_Pos)
#define RTC_INT_COMP3_EN_Clr            (~(RTC_INT_COMP3_EN_Msk))

/* CR0[18] :RTC_INT_EN. Enable or disable compare2 interrupt.  0x1: Enable. 0x0: Disable. */
#define RTC_INT_COMP2_EN_Pos            (18UL)
#define RTC_INT_COMP2_EN_Msk            (0x1UL << RTC_INT_COMP2_EN_Pos)
#define RTC_INT_COMP2_EN_Clr            (~(RTC_INT_COMP2_EN_Msk))

/* CR0[17] :RTC_INT_EN. Enable or disable compare1 interrupt.  0x1: Enable. 0x0: Disable. */
#define RTC_INT_COMP1_EN_Pos            (17UL)
#define RTC_INT_COMP1_EN_Msk            (0x1UL << RTC_INT_COMP1_EN_Pos)
#define RTC_INT_COMP1_EN_Clr            (~(RTC_INT_COMP1_EN_Msk))

/* CR0[16] :RTC_INT_EN. Enable or disable compare0 interrupt.  0x1: Enable. 0x0: Disable. */
#define RTC_INT_COMP0_EN_Pos            (16UL)
#define RTC_INT_COMP0_EN_Msk            (0x1UL << RTC_INT_COMP0_EN_Pos)
#define RTC_INT_COMP0_EN_Clr            (~(RTC_INT_COMP0_EN_Msk))

/* CR0[15] :RTC_INT_EN. Enable or disable compare3gt interrupt.  0x1: Enable. 0x0: Disable. */
#define RTC_COMP3GT_WAKEUP_EN_Pos       (15UL)
#define RTC_COMP3GT_WAKEUP_EN_Msk       (0x1UL << RTC_COMP3GT_WAKEUP_EN_Pos)
#define RTC_COMP3GT_WAKEUP_EN_Clr       (~(RTC_COMP3GT_WAKEUP_EN_Msk))

/* CR0[14] :RTC_INT_EN. Enable or disable compare2gt interrupt.  0x1: Enable. 0x0: Disable. */
#define RTC_COMP2GT_WAKEUP_EN_Pos       (14UL)
#define RTC_COMP2GT_WAKEUP_EN_Msk       (0x1UL << RTC_COMP2GT_WAKEUP_EN_Pos)
#define RTC_COMP2GT_WAKEUP_EN_Clr       (~(RTC_COMP2GT_WAKEUP_EN_Msk))

/* CR0[13] :RTC_INT_EN. Enable or disable compare1gt interrupt.  0x1: Enable. 0x0: Disable. */
#define RTC_COMP1GT_WAKEUP_EN_Pos       (13UL)
#define RTC_COMP1GT_WAKEUP_EN_Msk       (0x1UL << RTC_COMP1GT_WAKEUP_EN_Pos)
#define RTC_COMP1GT_WAKEUP_EN_Clr       (~(RTC_COMP1GT_WAKEUP_EN_Msk))

/* CR0[12] :RTC_INT_EN. Enable or disable compare0gt interrupt.  0x1: Enable. 0x0: Disable. */
#define RTC_COMP0GT_WAKEUP_EN_Pos       (12UL)
#define RTC_COMP0GT_WAKEUP_EN_Msk       (0x1UL << RTC_COMP0GT_WAKEUP_EN_Pos)
#define RTC_COMP0GT_WAKEUP_EN_Clr       (~(RTC_COMP0GT_WAKEUP_EN_Msk))

/* CR0[11] :RTC_INT_EN. Enable or disable prescale & comp3 interrupt.  0x1: Enable. 0x0: Disable. */
#define RTC_INT_PRE_COMP3_EN_Pos         (11UL)
#define RTC_INT_PRE_COMP3_EN_Msk         (0x1UL << RTC_INT_PRE_COMP3_EN_Pos)
#define RTC_INT_PRE_COMP3_EN_Clr         (~(RTC_INT_PRE_COMP3_EN_Msk))

/* CR0[10] :RTC_INT_EN. Enable or disable prescale comp interrupt.  0x1: Enable. 0x0: Disable. */
#define RTC_INT_PRE_COMP_EN_Pos          (10UL)
#define RTC_INT_PRE_COMP_EN_Msk          (0x1UL << RTC_INT_PRE_COMP_EN_Pos)
#define RTC_INT_PRE_COMP_EN_Clr          (~(RTC_INT_PRE_COMP_EN_Msk))

/* CR0[9] :RTC_MASK_TICK_INT. Mask RTC tick interrupt.  0x1: Unmask. 0x0: Mask. */
#define RTC_INT_OVERFLOW_EN_Pos         (9UL)
#define RTC_INT_OVERFLOW_EN_Msk         (0x1UL << RTC_INT_OVERFLOW_EN_Pos)
#define RTC_INT_OVERFLOW_EN_Clr         (~(RTC_INT_OVERFLOW_EN_Msk))

/* CR0[8] :RTC_INT_EN. Enable or disable tick interrupt.  0x1: Enable. 0x0: Disable. */
#define RTC_INT_TICK_EN_Pos             (8UL)
#define RTC_INT_TICK_EN_Msk             (0x1UL << RTC_INT_TICK_EN_Pos)
#define RTC_INT_TICK_EN_Clr             (~(RTC_INT_TICK_EN_Msk))

/* CR0[2] :RTC_PRE_COUNTER_RST. Reset Prescale Counter. 0x1: Reset Counter to 0. */
#define RTC_PRE_COUNTER_RST_Pos         (2UL)
#define RTC_PRE_COUNTER_RST_Msk         (0x1UL << RTC_PRE_COUNTER_RST_Pos)
#define RTC_PRE_COUNTER_RST_CLR         (~(RTC_PRE_COUNTER_RST_Msk))

/* CR0[1] :RTC_COUNTER_RST. Reset 24bit-RTC Counter. 0x1: Reset Counter to 0. */
#define RTC_COUNTER_RST_Pos             (1UL)
#define RTC_COUNTER_RST_Msk             (0x1UL << RTC_COUNTER_RST_Pos)
#define RTC_COUNTER_RST_CLR             (~(RTC_COUNTER_RST_Msk))

/* CR0[0] :RTC_START. Start or stop RTC 24bit-RTC Counter. 0x1: Start 24bit-RTC Counter.0x0: Stop 24bit-RTC Counter. */
#define RTC_START_Pos                   (0UL)
#define RTC_START_Msk                   (0x1UL << RTC_START_Pos)
#define RTC_START_CLR                   (~(RTC_START_Msk))

/* Register: INT_CLR */
/* Description: Interrupt clear register. Offset: 0x04. Address: 0x40000104. */

/* INT_CLR[15] :RTC_COMP3_WK_CLR. Clear Interrupt Status of Comparator1. */
/* This status is cleared by software.write 1 then write 0 after 2T to clear. */
#define RTC_COMP3_WK_CLR_Pos            (15UL)
#define RTC_COMP3_WK_CLR_SET            (0x1UL << RTC_COMP3_WK_CLR_Pos)
#define RTC_COMP3_WK_CLR_RESET          (~(RTC_COMP3_WK_CLR_SET))

/* INT_CLR[14] :RTC_COMP2_WK_CLR. Clear Interrupt Status of Comparator1. */
/* This status is cleared by software.write 1 then write 0 after 2T to clear. */
#define RTC_COMP2_WK_CLR_Pos            (14UL)
#define RTC_COMP2_WK_CLR_SET            (0x1UL << RTC_COMP2_WK_CLR_Pos)
#define RTC_COMP2_WK_CLR_RESET          (~(RTC_COMP2_WK_CLR_SET))

/* INT_CLR[13] :RTC_COMP1_WK_CLR. Clear Interrupt Status of Comparator1. */
/* This status is cleared by software.write 1 then write 0 after 2T to clear. */
#define RTC_COMP1_WK_CLR_Pos            (13UL)
#define RTC_COMP1_WK_CLR_SET            (0x1UL << RTC_COMP1_WK_CLR_Pos)
#define RTC_COMP1_WK_CLR_RESET          (~(RTC_COMP1_WK_CLR_SET))

/* INT_CLR[12] :RTC_COMP0_WK_CLR. Clear Interrupt Status of Comparator0. */
/* This status is cleared by software.write 1 then write 0 after 2T to clear. */
#define RTC_COMP0_WK_CLR_Pos            (12UL)
#define RTC_COMP0_WK_CLR_SET            (0x1UL << RTC_COMP0_WK_CLR_Pos)
#define RTC_COMP0_WK_CLR_RESET          (~(RTC_COMP0_WK_CLR_SET))

/* INT_CLR[11] :RTC_COMP3_CLR. Clear Interrupt Status of Comparator1. */
/* This interrupt is cleared by software.write 1 then write 0 after 2T to clear. */
#define RTC_COMP3_CLR_Pos               (11UL)
#define RTC_COMP3_CLR_SET               (0x1UL << RTC_COMP3_CLR_Pos)
#define RTC_COMP3_CLR_RESET             (~(RTC_COMP3_CLR_SET))

/* INT_CLR[10] :RTC_COMP2_CLR. Clear Interrupt Status of Comparator1. */
/* This interrupt is cleared by software.write 1 then write 0 after 2T to clear. */
#define RTC_COMP2_CLR_Pos               (10UL)
#define RTC_COMP2_CLR_SET               (0x1UL << RTC_COMP2_CLR_Pos)
#define RTC_COMP2_CLR_RESET             (~(RTC_COMP2_CLR_SET))

/* INT_CLR[9] :RTC_COMP1_CLR. Clear Interrupt Status of Comparator1. */
/* This interrupt is cleared by software.write 1 then write 0 after 2T to clear. */
#define RTC_COMP1_CLR_Pos               (9UL)
#define RTC_COMP1_CLR_SET               (0x1UL << RTC_COMP1_CLR_Pos)
#define RTC_COMP1_CLR_RESET             (~(RTC_COMP1_CLR_SET))

/* INT_CLR[8] :RTC_COMP0_CLR. Clear Interrupt Status of Comparator0. */
/* This interrupt is cleared by software.write 1 then write 0 after 2T to clear. */
#define RTC_COMP0_CLR_Pos               (8UL)
#define RTC_COMP0_CLR_SET               (0x1UL << RTC_COMP0_CLR_Pos)
#define RTC_COMP0_CLR_RESET             (~(RTC_COMP0_CLR_SET))

/* INT_CLR[7] :RTC_COMP3_CLR. Clear Interrupt Status of Comparator1. */
/* This interrupt is cleared by software.write 1 then write 0 after 2T to clear. */
#define RTC_COMP3GT_CLR_Pos             (7UL)
#define RTC_COMP3GT_CLR_SET             (0x1UL << RTC_COMP3GT_CLR_Pos)
#define RTC_COMP3GT_CLR_RESET           (~(RTC_COMP3GT_CLR_SET))

/* INT_CLR[6] :RTC_COMP2GT_CLR. Clear Interrupt Status of Comparator1. */
/* This interrupt is cleared by software.write 1 then write 0 after 2T to clear. */
#define RTC_COMP2GT_CLR_Pos             (6UL)
#define RTC_COMP2GT_CLR_SET             (0x1UL << RTC_COMP2GT_CLR_Pos)
#define RTC_COMP2GT_CLR_RESET           (~(RTC_COMP2GT_CLR_SET))

/* INT_CLR[5] :RTC_COMP1GT_CLR. Clear Interrupt Status of Comparator1. */
/* This interrupt is cleared by software.write 1 then write 0 after 2T to clear. */
#define RTC_COMP1GT_CLR_Pos             (5UL)
#define RTC_COMP1GT_CLR_SET             (0x1UL << RTC_COMP1GT_CLR_Pos)
#define RTC_COMP1GT_CLR_RESET           (~(RTC_COMP1GT_CLR_SET))

/* INT_CLR[4] :RTC_COMP0GT_CLR. Clear Interrupt Status of Comparator0. */
/* This interrupt is cleared by software.write 1 then write 0 after 2T to clear. */
#define RTC_COMP0GT_CLR_Pos             (4UL)
#define RTC_COMP0GT_CLR_SET             (0x1UL << RTC_COMP0GT_CLR_Pos)
#define RTC_COMP0GT_CLR_RESET           (~(RTC_COMP0GT_CLR_SET))

/* INT_CLR[3] :RTC_PRE_COMP3_CLR. Clear Interrupt Status of prescale and prescale_comp3. */
/* This interrupt is cleared by software.write 1 then write 0 after 2T to clear. */
#define RTC_PRE_COMP3_CLR_Pos           (3UL)
#define RTC_PRE_COMP3_CLR_SET           (0x1UL << RTC_PRE_COMP3_CLR_Pos)
#define RTC_PRE_COMP3_CLR_RESET         (~(RTC_PRE_COMP3_CLR_SET))

/* INT_CLR[6] :RTC_PRE_COMP_CLR. Clear Interrupt Status of Comparator1. */
/* This interrupt is cleared by software.write 1 then write 0 after 2T to clear. */
#define RTC_PRE_COMP_CLR_Pos            (2UL)
#define RTC_PRE_COMP_CLR_SET            (0x1UL << RTC_PRE_COMP_CLR_Pos)
#define RTC_PRE_COMP_CLR_RESET          (~(RTC_PRE_COMP_CLR_SET))

/* INT_CLR[1] :RTC_OVERFLOW_CLR. Clear Interrupt Status of Overflow. */
/* This interrupt is cleared by software.write 1 then write 0 after 2T to clear. */
#define RTC_OVERFLOW_CLR_Pos            (1UL)
#define RTC_OVERFLOW_CLR_SET            (0x1UL << RTC_OVERFLOW_CLR_Pos)
#define RTC_OVERFLOW_CLR_RESET          (~(RTC_OVERFLOW_CLR_SET))

/* INT_CLR[0] :RTC_TICK_CLR. Clear Interrupt Status of Tick. */
/* This interrupt is cleared by software.write 1 then write 0 after 2T to clear. */
#define RTC_TICK_CLR_Pos                (0UL)
#define RTC_TICK_CLR_SET                (0x1UL << RTC_TICK_CLR_Pos)
#define RTC_TICK_CLR_RESET              (~(RTC_TICK_CLR_SET))

#define RTC_COMP_INT_EN_OFFSET          (RTC_INT_COMP0_EN_Pos - RTC_COMP0_CLR_Pos)
#define RTC_PRE_COMP_INT_EN_OFFSET      (RTC_INT_PRE_COMP_EN_Pos - RTC_PRE_COMP_CLR_Pos)


/* Clear all interrupt */
#define RTC_ALL_INT_CLR_SET             (RTC_PRE_COMP3_CLR_SET | RTC_PRE_COMP_CLR_SET | \
                                         RTC_COMP3_CLR_SET | RTC_COMP2_CLR_SET | \
                                         RTC_COMP1_CLR_SET | RTC_COMP0_CLR_SET | \
                                         RTC_OVERFLOW_CLR_SET | RTC_TICK_CLR_SET)

/* Clear all wakeup */
#define RTC_ALL_WAKEUP_CLR_SET          (RTC_COMP3_WK_CLR_SET | RTC_COMP2_WK_CLR_SET | \
                                         RTC_COMP1_WK_CLR_SET | RTC_COMP0_WK_CLR_SET | \
                                         RTC_COMP3GT_CLR_SET | RTC_COMP2GT_CLR_SET | \
                                         RTC_COMP1GT_CLR_SET | RTC_COMP0GT_CLR_SET)

/*============================================================================*
 *                         Constants
 *============================================================================*/

/**
 * \defgroup    RTC_Exported_Constants Macro Definitions
 *
 * \ingroup     RTC
 */

/**
 * \defgroup    RTC_Interrupts_Definition RTC Interrupts Definition
 * \{
 * \ingroup     RTC_Exported_Constants
 */
typedef enum
{
    RTC_INT_TICK        = RTC_INT_TICK_EN_Msk,
    RTC_INT_OVF         = RTC_INT_OVERFLOW_EN_Msk,
    RTC_INT_PRE_COMP    = RTC_INT_PRE_COMP_EN_Msk,
    RTC_INT_PRE_COMP3   = RTC_INT_PRE_COMP3_EN_Msk,
    RTC_INT_COMP0       = RTC_INT_COMP0_EN_Msk,
    RTC_INT_COMP1       = RTC_INT_COMP1_EN_Msk,
    RTC_INT_COMP2       = RTC_INT_COMP2_EN_Msk,
    RTC_INT_COMP3       = RTC_INT_COMP3_EN_Msk,
} E_RTC_INT;

/** \} */

#define IS_RTC_INT(INT) (((INT) == RTC_INT_TICK) || \
                         ((INT) == RTC_INT_OVF) || \
                         ((INT) == RTC_INT_COMP0) || \
                         ((INT) == RTC_INT_COMP1) || \
                         ((INT) == RTC_INT_COMP2) || \
                         ((INT) == RTC_INT_COMP3) || \
                         ((INT) == RTC_INT_PRE_COMP) || \
                         ((INT) == RTC_INT_PRE_COMP3))

/**
 * \defgroup    RTC_Wakeup_Definition RTC Wakeup Definition
 * \{
 * \ingroup     RTC_Exported_Constants
 */
typedef enum
{
    RTC_WK_TICK         = RTC_INT_TICK_EN_Msk,
    RTC_WK_OVF          = RTC_INT_OVERFLOW_EN_Msk,
    RTC_WK_PRE_COMP     = RTC_INT_PRE_COMP_EN_Msk,
    RTC_WK_PRE_COMP3    = RTC_INT_PRE_COMP3_EN_Msk,
    RTC_WK_COMP0GT      = RTC_COMP0GT_WAKEUP_EN_Msk,
    RTC_WK_COMP1GT      = RTC_COMP1GT_WAKEUP_EN_Msk,
    RTC_WK_COMP2GT      = RTC_COMP2GT_WAKEUP_EN_Msk,
    RTC_WK_COMP3GT      = RTC_COMP3GT_WAKEUP_EN_Msk,
    RTC_WK_COMP0        = RTC_COMP0_WAKEUP_EN_Msk,
    RTC_WK_COMP1        = RTC_COMP1_WAKEUP_EN_Msk,
    RTC_WK_COMP2        = RTC_COMP2_WAKEUP_EN_Msk,
    RTC_WK_COMP3        = RTC_COMP3_WAKEUP_EN_Msk,
} E_RTC_WK;

/** \} */

#define IS_RTC_WK(WK) (((WK) == RTC_WK_TICK) || \
                       ((WK) == RTC_WK_OVF) || \
                       ((WK) == RTC_WK_PRE_COMP) || \
                       ((WK) == RTC_WK_PRE_COMP3) || \
                       ((WK) == RTC_WK_COMP0GT) || \
                       ((WK) == RTC_WK_COMP1GT) || \
                       ((WK) == RTC_WK_COMP2GT) || \
                       ((WK) == RTC_WK_COMP3GT) || \
                       ((WK) == RTC_WK_COMP0) || \
                       ((WK) == RTC_WK_COMP1) || \
                       ((WK) == RTC_WK_COMP2) || \
                       ((WK) == RTC_WK_COMP3))

/**
 * \defgroup    RTC_Comp_Definition RTC Comparator Definition
 * \{
 * \ingroup     RTC_Exported_Constants
 */
typedef enum
{
    RTC_COMP0 = 0,
    RTC_COMP1,
    RTC_COMP2,
    RTC_COMP3,
} E_RTC_COMP_INDEX;

/** \} */

#define IS_RTC_COMP(COMP) (((COMP) == RTC_COMP0) || \
                           ((COMP) == RTC_COMP1) || \
                           ((COMP) == RTC_COMP2) || \
                           ((COMP) == RTC_COMP3))

/**
 * \defgroup    RTC_CompGT_Definition RTC ComparatorGT Definition
 * \{
 * \ingroup     RTC_Exported_Constants
 */
typedef enum
{
    RTC_COMP0GT = 0,
    RTC_COMP1GT,
    RTC_COMP2GT,
    RTC_COMP3GT,
} E_RTC_COMPGT_INDEX;

/** \} */

/**
 * \defgroup    RTC_32K_Selction RTC Selction
 * \{
 * \ingroup     RTC_Exported_Constants
 */
typedef enum
{
    RTC_32K_XI,
    RTC_32k_P2_4,
    RTC_32K_GPIO_MAX,
}
RTC_32K_GPIO_IN;

#define IS_RTC_32K(GPIO) (((GPIO) == RTC_32K_XI) || \
                          ((GPIO) == RTC_32k_P2_4) || \
                          ((GPIO) == RTC_32K_GPIO_MAX))
/** \} */

/*============================================================================*
 *                         Functions
 *============================================================================*/

/**
 * \defgroup    RTC_Exported_Functions Peripheral APIs
 * \{
 * \ingroup     RTC
 */

/**
 * \brief     Fast write RTC register,internal function.
 * \param[in] regAddress: The register address.
 * \param[in] data: Data which write to register.
 * \return    None.
 */
void RTC_WriteReg(uint32_t regAddress, uint32_t data);

/**
 * \brief     Deinitializes the RTC peripheral registers to their default reset values(turn off clock).
 * \param[in] None.
 * \return    None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void driver_rtc_init(void)
 * {
 *     RTC_DeInit();
 * }
 * \endcode
 */
void RTC_DeInit(void);

/**
 * \brief     Set RTC prescaler value.
 * \param[in] value: The prescaler value to be set.Should be no more than 12 bits!
 * \return    None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * #define RTC_PRESCALER_VALUE     49
 * #define RTC_COMP_INDEX          RTC_COMP3
 * #define RTC_COMP_INDEX_INT      RTC_INT_COMP3
 * #define RTC_COMP_VALUE          (1000)
 *
 * void driver_rtc_init(void)
 * {
 *     RTC_DeInit();
 *
 *     RTC_SetPrescaler(RTC_PRESCALER_VALUE);
 *     RTC_SetCompValue(RTC_COMP_INDEX, RTC_COMP_VALUE);
 *
 *     RTC_MaskINTConfig(RTC_COMP_INDEX_INT, DISABLE);
 *     RTC_INTConfig(RTC_COMP_INDEX_INT, ENABLE);
 *
 *     RTC_NvCmd(ENABLE);
 *     RTC_Cmd(ENABLE);
 * }
 * \endcode
 */
void RTC_SetPrescaler(uint16_t value);

/**
 * \brief     Start or stop RTC peripheral.
 * \param[in] NewState: New state of RTC peripheral.
 *            This parameter can be one of the following values:
 *            \arg ENABLE: Start RTC.
 *            \arg DISABLE: Stop RTC.
 * \return None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * #define RTC_PRESCALER_VALUE     49
 * #define RTC_COMP_INDEX          RTC_COMP3
 * #define RTC_COMP_INDEX_INT      RTC_INT_COMP3
 * #define RTC_COMP_VALUE          (1000)
 *
 * void driver_rtc_init(void)
 * {
 *     RTC_DeInit();
 *
 *     RTC_SetPrescaler(RTC_PRESCALER_VALUE);
 *     RTC_SetCompValue(RTC_COMP_INDEX, RTC_COMP_VALUE);
 *
 *     RTC_MaskINTConfig(RTC_COMP_INDEX_INT, DISABLE);
 *     RTC_INTConfig(RTC_COMP_INDEX_INT, ENABLE);
 *
 *     RTC_NvCmd(ENABLE);
 *     RTC_Cmd(ENABLE);
 * }
 * \endcode
 */
void RTC_Cmd(FunctionalState NewState);

/**
 * \brief     Enable or disable the specified RTC interrupt source.
 * \param[in] RTC_INT: Specifies the RTC interrupt source which to be enabled or disabled.
 *            This parameter can be any combination of the following values:
 *            \arg RTC_INT_TICK: Tick interrupt source.
 *            \arg RTC_INT_OVF: counter overflow interrupt
 *            \arg RTC_INT_COMP0: Compare 0 interrupt source.
 *            \arg RTC_INT_COMP1: Compare 1 interrupt source.
 *            \arg RTC_INT_COMP2: Compare 2 interrupt source.
 *            \arg RTC_INT_COMP3: Compare 3 interrupt source.
 *            \arg RTC_INT_PRE_COMP: Prescale compare interrupt source.
 *            \arg RTC_INT_PRE_COMP3: Prescale & compare 3 interrupt source.
 * \param[in] NewState: New state of the specified RTC interrupt.
 *            This parameter can be: ENABLE or DISABLE.
 * \return    None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * #define RTC_PRESCALER_VALUE     49
 * #define RTC_COMP_INDEX          RTC_COMP3
 * #define RTC_COMP_INDEX_INT      RTC_INT_COMP3
 * #define RTC_COMP_VALUE          (1000)
 *
 * void driver_rtc_init(void)
 * {
 *     RTC_DeInit();
 *
 *     RTC_SetPrescaler(RTC_PRESCALER_VALUE);
 *     RTC_SetCompValue(RTC_COMP_INDEX, RTC_COMP_VALUE);
 *
 *     RTC_MaskINTConfig(RTC_COMP_INDEX_INT, DISABLE);
 *     RTC_INTConfig(RTC_COMP_INDEX_INT, ENABLE);
 *
 *     RTC_NvCmd(ENABLE);
 *     RTC_Cmd(ENABLE);
 * }
 * \endcode
 */
void RTC_INTConfig(E_RTC_INT RTC_INT, FunctionalState NewState);

/**
 * \brief  Enable or disable the specified RTC wakeup function.
 * \param  RTC_WK: specifies the RTC wakeup function to be enabled or disabled.
 *         This parameter can be any combination of the following values:
 *         \arg RTC_WK_TICK: tick wakeup function
 *         \arg RTC_WK_OVF: tick wakeup function
 *         \arg RTC_WK_PRE_CMP: prescale compare wakeup function
 *         \arg RTC_WK_PRE_CMP3: prescale & compare 3 wakeup function
 *         \arg RTC_WK_COMP0GT: compare 0 gt wakeup function
 *         \arg RTC_WK_COMP1GT: compare 1 gt wakeup function
 *         \arg RTC_WK_COMP2GT: compare 2 gt wakeup function
 *         \arg RTC_WK_COMP3GT: compare 3 gt wakeup function
 *         \arg RTC_WK_CMP0: compare 0 wakeup function
 *         \arg RTC_WK_CMP1: compare 1 wakeup function
 *         \arg RTC_WK_CMP2: compare 2 wakeup function
 *         \arg RTC_WK_CMP3: compare 3 wakeup function
 * \param  NewState: new state of the specified RTC wakeup function.
 *         This parameter can be: ENABLE or DISABLE.
 * \return None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * #define RTC_PRESCALER_VALUE     49
 * #define RTC_COMP_INDEX          RTC_COMP3
 * #define RTC_COMP_INDEX_INT      RTC_INT_COMP3
 * #define RTC_COMP_VALUE          (1000)
 *
 * void driver_rtc_init(void)
 * {
 *     RTC_DeInit();
 *
 *     RTC_SetPrescaler(RTC_PRESCALER_VALUE);
 *     RTC_SetCompValue(RTC_COMP_INDEX, RTC_COMP_VALUE);
 *
 *     RTC_MaskINTConfig(RTC_COMP_INDEX_INT, DISABLE);
 *     RTC_INTConfig(RTC_COMP_INDEX_INT, ENABLE);
 *
 *     RTC_NvCmd(ENABLE);
 *     RTC_Cmd(ENABLE);
 * }
 * \endcode
 */
void RTC_WKConfig(E_RTC_WK RTC_WK, FunctionalState NewState);

/**
 * \brief     Enable RTC interrupt signal to CPU NVIC.
 * \param[in] NewState: Enable or disable RTC interrupt signal to MCU.
 *            This parameter can be: ENABLE or DISABLE..
 * \return    None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * #define RTC_PRESCALER_VALUE     49
 * #define RTC_COMP_INDEX          RTC_COMP3
 * #define RTC_COMP_INDEX_INT      RTC_INT_COMP3
 * #define RTC_COMP_VALUE          (1000)
 *
 * void driver_rtc_init(void)
 * {
 *     RTC_DeInit();
 *
 *     RTC_SetPrescaler(RTC_PRESCALER_VALUE);
 *     RTC_SetCompValue(RTC_COMP_INDEX, RTC_COMP_VALUE);
 *
 *     RTC_MaskINTConfig(RTC_COMP_INDEX_INT, DISABLE);
 *     RTC_INTConfig(RTC_COMP_INDEX_INT, ENABLE);
 *
 *     RTC_NvCmd(ENABLE);
 *     RTC_Cmd(ENABLE);
 * }
 * \endcode
 */
void RTC_NvCmd(FunctionalState NewState);

/**
 * \brief     Enable or disable system wake up function of RTC.
 * \param[in] NewState: new state of the wake up function.
 *            This parameter can be: ENABLE or DISABLE.
 * \return    None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void rtc_demo(void)
 * {
 *     RTC_SystemWakeupConfig(ENABLE);
 * }
 * \endcode
 */
void RTC_SystemWakeupConfig(FunctionalState NewState);

/**
 * \brief     Reset counter value of RTC.
 * \param[in] None.
 * \return    None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void rtc_demo(void)
 * {
 *     RTC_ResetCounter();
 *     RTC_Cmd(ENABLE);
 * }
 * \endcode
 */
void RTC_ResetCounter(void);

/**
 * \brief     Reset prescaler counter value of RTC.
 * \param[in] None.
 * \return    None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void rtc_demo(void)
 * {
 *     RTC_ResetPrescalerCounter();
 *     RTC_Cmd(ENABLE);
 * }
 * \endcode
 */
void RTC_ResetPrescalerCounter(void);

/**
 * \brief  Check whether the specified RTC interrupt is set.
  * \param[in]  RTC_INT: Specifies the RTC interrupt source to be enabled or disabled.
 *   This parameter can be any combination of the following values:
 *            \arg RTC_INT_TICK: RTC tick interrupt source.
 *            \arg RTC_INT_COMP0: Compare 0 interrupt source.
 *            \arg RTC_INT_COMP1: Compare 1 interrupt source.
 *            \arg RTC_INT_COMP2: Compare 2 interrupt source.
 *            \arg RTC_INT_COMP3: Compare 3 interrupt source.
 *            \arg RTC_INT_PRE_COMP: Prescale compare interrupt source.
 *            \arg RTC_INT_PRE_COMP3: Prescale & compare 3 interrupt source.
 * \return The new state of RTC_INT (SET or RESET).
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void rtc_demo(void)
 * {
 *     ITStatus int_status = RTC_GetINTStatus(RTC_INT_COMP0);
 * }
 * \endcode
 */
ITStatus RTC_GetINTStatus(E_RTC_INT RTC_INT);

/**
 * \brief     Clear the interrupt pending bits of RTC.
 * \param[in] RTC_INT: specifies the RTC interrupt flag to clear.
 *            This parameter can be any combination of the following values:
 *            \arg RTC_INT_TICK: RTC tick interrupt source.
 *            \arg RTC_INT_OVF: RTC counter overflow interrupt source.
 *            \arg RTC_INT_COMP0: Compare 0 interrupt source.
 *            \arg RTC_INT_COMP1: Compare 1 interrupt source.
 *            \arg RTC_INT_COMP2: Compare 2 interrupt source.
 *            \arg RTC_INT_COMP3: Compare 3 interrupt source.
 *            \arg RTC_INT_PRE_COMP: Prescale compare interrupt source.
 *            \arg RTC_INT_PRE_COMP3: Prescale & compare 3 interrupt source.
 * \return None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void rtc_demo(void)
 * {
 *     RTC_ClearINTPendingBit(RTC_INT_COMP0);
 * }
 * \endcode
 */
void RTC_ClearINTPendingBit(E_RTC_INT RTC_INT);

/**
  * \brief  Checks whether the specified RTC wakeup state is set or not.
  * \param  RTC_WK: specifies the RTC interrupt source to be enabled or disabled.
  *         This parameter can be any combination of the following values:
  *         \arg RTC_WK_TICK: tick wakeup function
  *         \arg RTC_WK_OVF: tick wakeup function
  *         \arg RTC_WK_PRE_CMP: prescale compare wakeup function
  *         \arg RTC_WK_PRE_CMP3: prescale & compare 3 wakeup function
  *         \arg RTC_WK_COMP0GT: compare 0 gt wakeup function
  *         \arg RTC_WK_COMP1GT: compare 1 gt wakeup function
  *         \arg RTC_WK_COMP2GT: compare 2 gt wakeup function
  *         \arg RTC_WK_COMP3GT: compare 3 gt wakeup function
  *         \arg RTC_WK_CMP0: compare 0 wakeup function
  *         \arg RTC_WK_CMP1: compare 1 wakeup function
  *         \arg RTC_WK_CMP2: compare 2 wakeup function
  *         \arg RTC_WK_CMP3: compare 3 wakeup function
  * \return The new state of RTC_INT (SET or RESET).
  */
ITStatus RTC_GetWakeupStatus(E_RTC_WK RTC_WK);

/**
  * \brief  Clear the wakeup status bits of RTC.
  * \param  RTC_WK: specifies the RTC wakeup flag to clear.
  *         This parameter can be any combination of the following values:
  *         \arg RTC_WK_TICK: tick wakeup function
  *         \arg RTC_WK_OVF: tick wakeup function
  *         \arg RTC_WK_PRE_CMP: prescale compare wakeup function
  *         \arg RTC_WK_PRE_CMP3: prescale & compare 3 wakeup function
  *         \arg RTC_WK_COMP0GT: compare 0 gt wakeup function
  *         \arg RTC_WK_COMP1GT: compare 1 gt wakeup function
  *         \arg RTC_WK_COMP2GT: compare 2 gt wakeup function
  *         \arg RTC_WK_COMP3GT: compare 3 gt wakeup function
  *         \arg RTC_WK_CMP0: compare 0 wakeup function
  *         \arg RTC_WK_CMP1: compare 1 wakeup function
  *         \arg RTC_WK_CMP2: compare 2 wakeup function
  *         \arg RTC_WK_CMP3: compare 3 wakeup function
  * \return None.
  */
void RTC_ClearWakeupStatusBit(E_RTC_WK RTC_WK);

/**
 * \brief     Clear the interrupt pending bit of the select comparator of RTC.
 * \param[in] index: the comparator number.
 * \return    None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void rtc_demo(void)
 * {
 *     RTC_ClearCompINT(0);
 * }
 * \endcode
 */
void RTC_ClearCompINT(E_RTC_COMP_INDEX index);

/**
 * \brief     Clear the overflow interrupt pending bit of RTC.
 * \param[in] None.
 * \return    None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void rtc_demo(void)
 * {
 *     RTC_ClearOverFlowINT();
 * }
 * \endcode
 */
void RTC_ClearOverFlowINT(void);

/**
 * \brief     Clear the tick interrupt pending bit of RTC.
 * \param[in] None.
 * \return    None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void rtc_demo(void)
 * {
 *     RTC_ClearTickINT();
 * }
 * \endcode
 */
void RTC_ClearTickINT(void);

/**
  * @brief  Select source clock to gpio input of RTC.
  * @param  gpio: the selected gpio.
  * @retval None
  */
void RTC_SelectSrcToGpioInput(RTC_32K_GPIO_IN rtc_in);

/**
 * \brief     Set RTC comparator value.
 * \param[in] index: The comparator number,can be 0 ~ 3.
 * \param[in] value: The comparator value to be set.Should be no more than 24 bits!
 * \return    None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * #define RTC_PRESCALER_VALUE     49
 * #define RTC_COMP_INDEX          RTC_COMP3
 * #define RTC_COMP_INDEX_INT      RTC_INT_COMP3
 * #define RTC_COMP_VALUE          (1000)
 *
 * void driver_rtc_init(void)
 * {
 *     RTC_DeInit();
 *
 *     RTC_SetPrescaler(RTC_PRESCALER_VALUE);
 *     RTC_SetCompValue(RTC_COMP_INDEX, RTC_COMP_VALUE);
 *
 *     RTC_MaskINTConfig(RTC_COMP_INDEX_INT, DISABLE);
 *     RTC_INTConfig(RTC_COMP_INDEX_INT, ENABLE);
 *
 *     RTC_NvCmd(ENABLE);
 *     RTC_Cmd(ENABLE);
 * }
 * \endcode
 */
__STATIC_INLINE void RTC_SetCompValue(E_RTC_COMP_INDEX index, uint32_t value)
{
    /* Check the parameters */
    assert_param(IS_RTC_COMP(index));

    RTC_WriteReg((uint32_t)(&(RTC->COMP0) + index), (value & 0xFFFFFFFF));
}

/**
 * \brief     Set RTC comparator GT value.
 * \param[in] index: The comparator gt number, can be 0 ~ 3.
 * \param[in] value: The comparator value to be set.
 * \return    None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * #define RTC_PRESCALER_VALUE     49
 * #define RTC_COMP_INDEX          RTC_COMP3
 * #define RTC_COMP_INDEX_INT      RTC_INT_COMP3
 * #define RTC_COMP_VALUE          (1000)
 *
 * void driver_rtc_init(void)
 * {
 *     RTC_DeInit();
 *
 *     RTC_SetPrescaler(RTC_PRESCALER_VALUE);
 *     RTC_SetCompValue(RTC_COMP_INDEX, RTC_COMP_VALUE);
 *
 *     RTC_MaskINTConfig(RTC_COMP_INDEX_INT, DISABLE);
 *     RTC_INTConfig(RTC_COMP_INDEX_INT, ENABLE);
 *
 *     RTC_NvCmd(ENABLE);
 *     RTC_Cmd(ENABLE);
 * }
 * \endcode
 */
__STATIC_INLINE void RTC_SetCompGTValue(E_RTC_COMPGT_INDEX index, uint32_t value)
{
    /* Check the parameters */
    assert_param(IS_RTC_COMPGT(index));

    RTC_WriteReg((uint32_t)(&(RTC->COMP0GT) + index), (value & 0xFFFFFFFF));
}

/**
 * \brief     Set RTC prescaler comparator value.
 * \param[in] value: The comparator value to be set.Should be no more than 12 bits!
 * \return    None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * #define RTC_PRESCALER_VALUE     (3200 - 1)//max 4095
 * #define RTC_PRECOMP_VALUE       (320)//max 4095
 * #define RTC_COMP3_VALUE         (10)
 *
 * void driver_rtc_init(void)
 * {
 *     RTC_DeInit();
 *
 *     RTC_SetPrescaler(RTC_PRESCALER_VALUE);
 *     RTC_SetPreCompValue(RTC_PRECOMP_VALUE);
 *     RTC_SetCompValue(RTC_COMP3, RTC_COMP3_VALUE);
 *
 *     RTC_MaskINTConfig(RTC_INT_PRE_COMP3, DISABLE);
 *     RTC_INTConfig(RTC_INT_PRE_COMP3, ENABLE);
 *
 *     RTC_NvCmd(ENABLE);
 *     RTC_Cmd(ENABLE);
 * }
 * \endcode
 */
__STATIC_INLINE void RTC_SetPreCompValue(uint32_t value)
{
    RTC_WriteReg((uint32_t)(&(RTC->PRE_COMP)), (value & 0xFFF));
}

/**
 * \brief     Get counter value of RTC.
 * \param[in] None.
 * \return    The counter value.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void rtc_demo(void)
 * {
 *     uitn32_t counter = RTC_GetCounter();
 * }
 * \endcode
 */
__STATIC_INLINE uint32_t RTC_GetCounter(void)
{
    return RTC->CNT;
}

/**
 * \brief     Get prescaler counter value of RTC.
 * \param[in] None.
 * \return    The prescaler counter value.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void rtc_demo(void)
 * {
 *     uitn32_t pre_counter = RTC_GetPreCounter();
 * }
 * \endcode
 */
__STATIC_INLINE uint32_t RTC_GetPreCounter(void)
{
    return RTC->PRE_CNT;
}

/**
 * \brief     Get RTC comparator value.
 * \param[in] index: The comparator number.
 * \return    The comparator value.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void rtc_demo(void)
 * {
 *     uint32_t data = RTC_GetCompValue(RTC_COMP0);
 * }
 * \endcode
 */
__STATIC_INLINE uint32_t RTC_GetCompValue(E_RTC_COMP_INDEX index)
{
    return *((volatile uint32_t *)(&(RTC->COMP0) + index));
}

/**
 * \brief     Get RTC comparator gt value.
 * \param[in] index: The comparator number 0~3.
 * \return    The comparator value.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void rtc_demo(void)
 * {
 *     uitn32_t data = RTC_GetCompGTValue(0);
 * }
 * \endcode
 */
__STATIC_INLINE uint32_t RTC_GetCompGTValue(E_RTC_COMPGT_INDEX index)
{
    return *((volatile uint32_t *)(&(RTC->COMP0GT) + index));
}

/**
 * \brief     Get RTC prescaler comparator value.
 * \param[in] None.
 * \return    The prescaler comparator value.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void rtc_demo(void)
 * {
 *     uitn32_t data = RTC_GetPreCompValue();
 * }
 * \endcode
 */
__STATIC_INLINE uint32_t RTC_GetPreCompValue(void)
{
    return RTC->PRE_COMP;
}

/**
 * \brief     Write backup register for store time information.
 * \param[in] value: valuer=write to back up reister
 * \return    None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void rtc_demo(void)
 * {
 *     RTC_WriteBackupReg(0x01020304);
 * }
 * \endcode
 */
//void RTC_WriteBackupReg(uint32_t value);
__STATIC_INLINE void RTC_WriteBackupReg(uint32_t value)
{
    RTC_WriteReg((uint32_t)(&(RTC->BACKUP)), value);
}

/**
 * \brief     Read backup register.
 * \param[in] None.
 * \return    Register value.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void rtc_demo(void)
 * {
 *     uint32_t reg_data = RTC_ReadBackupReg();
 * }
 * \endcode
 */
//uint32_t RTC_ReadBackupReg(void);
__STATIC_INLINE uint32_t RTC_ReadBackupReg(void)
{
    return (RTC->BACKUP);
}

/** \} */ /* End of group RTC_Exported_Functions */

#ifdef __cplusplus
}
#endif

#endif /* _RTL876X_RTC_H_ */


/******************* (C) COPYRIGHT 2020 Realtek Semiconductor *****END OF FILE****/

