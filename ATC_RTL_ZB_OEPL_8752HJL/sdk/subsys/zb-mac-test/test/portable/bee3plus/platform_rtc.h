/**
*********************************************************************************************************
*               Copyright(c) 2020, Realtek Semiconductor Corporation. All rights reserved.
**********************************************************************************************************
* @file         platform_rtc.h
* @brief        Platfrom RTC implementation head file.
* @details
* @author       Po Yu Chen
* @date         2020-12-15
* @version      v0.1
*********************************************************************************************************
*/

/*============================================================================*
 *               Define to prevent recursive inclusion
 *============================================================================*/
#ifndef __PLATFORM_RTC_H
#define __PLATFORM_RTC_H

/*============================================================================*
 *                               Header Files
 *============================================================================*/
#include <stdint.h>
#include <stdbool.h>
#include "rtl876x.h"

#ifdef __cplusplus
extern "C" {
#endif

#define PF_RTC_BIT_NUM              BIT64(32)

/* base address = RTC_REG_BASE */
/* PF_RTC offset*/
#define AON_RTC_PF_COMP_CONTROLLER_REG  0x40
#define AON_RTC_PF_COMP0_REG            0x44
#define AON_RTC_PF_COMP1_REG            0x48
#define AON_RTC_PF_COMP2_REG            0x4C
#define AON_RTC_PF_COMP3_REG            0x50
#define AON_RTC_PF_CNT_REG              0x58
#define AON_RTC_PF_COMP_INT_WK_CLR_REG  0x5C
#define AON_RTC_PF_COMP_INT_WK_SR_REG   0x60
#define AON_RTC_PF_RTL_VERSION_REG      0x64
#define AON_RTC_PF_COMP_MANUAL_INT_REG  0x68

/* RTC register RW */
#define RTC_FAST_WDATA              0xF0
#define RTC_FAST_ADDR               0xF4
#define RTC_WR_STROBE               0xF8

/*============================================================================*
 *                              Types
 *============================================================================*/
/** @defgroup PLATFORM_RTC_Exported_Types Platform RTC Exported Types
  * @{
  */

typedef enum PFRTCComparator
{
    PF_RTC_COMP0        = 0x01,
    PF_RTC_COMP1        = 0x02,
    PF_RTC_COMP2        = 0x04,
    PF_RTC_COMP3        = 0x08,
} PFRTCComparator;

/* 0x40    0x4000_0140
    0       R/W RTC_PF_CNT_START                        1'b0
    1       R/W RTC_PF_CNT_RST                          1'b0
    7:2     R/W RTC_PF_COMP_CONTROLLER_DUMMY0           6'b0
    8       R/W RTC_PF_COMP0_WK_EN                      1'b0
    9       R/W RTC_PF_COMP1_WK_EN                      1'b0
    10      R/W RTC_PF_COMP2_WK_EN                      1'b0
    11      R/W RTC_PF_COMP3_WK_EN                      1'b0
    12      R/W RTC_PF_COMP0_INT_EN                     1'b0
    13      R/W RTC_PF_COMP1_INT_EN                     1'b0
    14      R/W RTC_PF_COMP2_INT_EN                     1'b0
    15      R/W RTC_PF_COMP3_INT_EN                     1'b0
    19:16   R/W RTC_PF_COMP_CONTROLLER_DUMMY1           4'b0
    20      R/W RTC_PF_COMP0_ISR_TYPE                   1'b0
    21      R/W RTC_PF_COMP1_ISR_TYPE                   1'b0
    22      R/W RTC_PF_COMP2_ISR_TYPE                   1'b0
    23      R/W RTC_PF_COMP3_ISR_TYPE                   1'b0
    28:24   R/W RTC_PF_COMP_CONTROLLER_DUMMY1           5'b0
    29      R/W RTC_PF_WK_EN                            1'b0
    30      R/W RTC_PF_INT_EN                           1'b0
    31      R/W RTC_PF_RST                              1'b0
 */
typedef volatile union _AON_RTC_PF_COMP_CONTROLLER_REG_TYPE
{
    uint32_t d32;
    struct
    {
        uint32_t RTC_PF_CNT_START: 1;
        uint32_t RTC_PF_CNT_RST: 1;
        uint32_t RTC_PF_COMP_CONTROLLER_DUMMY0: 6;
        uint32_t RTC_PF_COMP0_WK_EN: 1;
        uint32_t RTC_PF_COMP1_WK_EN: 1;
        uint32_t RTC_PF_COMP2_WK_EN: 1;
        uint32_t RTC_PF_COMP3_WK_EN: 1;
        uint32_t RTC_PF_COMP0_INT_EN: 1;
        uint32_t RTC_PF_COMP1_INT_EN: 1;
        uint32_t RTC_PF_COMP2_INT_EN: 1;
        uint32_t RTC_PF_COMP3_INT_EN: 1;
        uint32_t RTC_PF_COMP_CONTROLLER_DUMMY1_1: 4;
        uint32_t RTC_PF_COMP0_ISR_TYPE: 1;
        uint32_t RTC_PF_COMP1_ISR_TYPE: 1;
        uint32_t RTC_PF_COMP2_ISR_TYPE: 1;
        uint32_t RTC_PF_COMP3_ISR_TYPE: 1;
        uint32_t RTC_PF_COMP_CONTROLLER_DUMMY1: 5;
        uint32_t RTC_PF_WK_EN: 1;
        uint32_t RTC_PF_INT_EN: 1;
        uint32_t RTC_PF_RST: 1;
    };
    struct
    {
        uint32_t RTC_PF_COMP_CONTROLLER_ASSEMBLE0: 8;
        uint32_t RTC_PF_COMP_WK_EN: 4;
        uint32_t RTC_PF_COMP_INT_EN: 4;
        uint32_t RTC_PF_COMP_CONTROLLER_ASSEMBLE1: 4;
        uint32_t RTC_PF_COMP_ISR_TYPE: 4;
        uint32_t RTC_PF_COMP_CONTROLLER_ASSEMBLE2: 8;
    };
} AON_RTC_PF_COMP_CONTROLLER_REG_TYPE;

/* 0x44    0x4000_0144
    31:0    R/W RTC_PF_COMP0                            32'd0
 */
typedef volatile union _AON_RTC_PF_COMP0_REG_TYPE
{
    uint32_t d32;
    struct
    {
        uint32_t RTC_PF_COMP0;
    };
} AON_RTC_PF_COMP0_REG_TYPE;

/* 0x48    0x4000_0148
    31:0    R/W RTC_PF_COMP1                            32'd0
 */
typedef volatile union _AON_RTC_PF_COMP1_REG_TYPE
{
    uint32_t d32;
    struct
    {
        uint32_t RTC_PF_COMP1;
    };
} AON_RTC_PF_COMP1_REG_TYPE;

/* 0x4C    0x4000_014c
    31:0    R/W RTC_PF_COMP2                            32'd0
 */
typedef volatile union _AON_RTC_PF_COMP2_REG_TYPE
{
    uint32_t d32;
    struct
    {
        uint32_t RTC_PF_COMP2;
    };
} AON_RTC_PF_COMP2_REG_TYPE;

/* 0x50    0x4000_0150
    31:0    R/W RTC_PF_COMP3                            32'd0
 */
typedef volatile union _AON_RTC_PF_COMP3_REG_TYPE
{
    uint32_t d32;
    struct
    {
        uint32_t RTC_PF_COMP3;
    };
} AON_RTC_PF_COMP3_REG_TYPE;

/* 0x58    0x4000_0158
    31:0    R   RTC_PF_CNT                              32'd0
 */
typedef volatile union _AON_RTC_PF_CNT_REG_TYPE
{
    uint32_t d32;
    struct
    {
        uint32_t RTC_PF_CNT;
    };
} AON_RTC_PF_CNT_REG_TYPE;

/* 0x5C    0x4000_015c
    0       R/WACRTC_PF_COMP0_WK_CLR                     1'b0
    1       R/WACRTC_PF_COMP1_WK_CLR                     1'b0
    2       R/WACRTC_PF_COMP2_WK_CLR                     1'b0
    3       R/WACRTC_PF_COMP3_WK_CLR                     1'b0
    4       R/WACRTC_PF_COMP0_INT_CLR                    1'b0
    5       R/WACRTC_PF_COMP1_INT_CLR                    1'b0
    6       R/WACRTC_PF_COMP2_INT_CLR                    1'b0
    7       R/WACRTC_PF_COMP3_INT_CLR                    1'b0
    31:8    R/W RTC_PF_COMP_INT_WK_CLR_DUMMY            24'b0
 */
typedef volatile union _AON_RTC_PF_COMP_INT_WK_CLR_REG_TYPE
{
    uint32_t d32;
    struct
    {
        uint32_t RTC_PF_COMP0_WK_CLR: 1;
        uint32_t RTC_PF_COMP1_WK_CLR: 1;
        uint32_t RTC_PF_COMP2_WK_CLR: 1;
        uint32_t RTC_PF_COMP3_WK_CLR: 1;
        uint32_t RTC_PF_COMP0_INT_CLR: 1;
        uint32_t RTC_PF_COMP1_INT_CLR: 1;
        uint32_t RTC_PF_COMP2_INT_CLR: 1;
        uint32_t RTC_PF_COMP3_INT_CLR: 1;
        uint32_t RTC_PF_COMP_INT_WK_CLR_DUMMY: 24;
    };
    struct
    {
        uint32_t RTC_PF_COMP_WK_CLR: 4;
        uint32_t RTC_PF_COMP_INT_CLR: 4;
        uint32_t RTC_PF_COMP_INT_WK_CLR_ASSEMBLE: 24;
    };
} AON_RTC_PF_COMP_INT_WK_CLR_REG_TYPE;

/* 0x60    0x4000_0160
    0       R/W1CRTC_PF_COMP0_WK_SR                      1'b0
    1       R/W1CRTC_PF_COMP1_WK_SR                      1'b0
    2       R/W1CRTC_PF_COMP2_WK_SR                      1'b0
    3       R/W1CRTC_PF_COMP3_WK_SR                      1'b0
    4       R/W1CRTC_PF_COMP0_INT_SR                     1'b0
    5       R/W1CRTC_PF_COMP1_INT_SR                     1'b0
    6       R/W1CRTC_PF_COMP2_INT_SR                     1'b0
    7       R/W1CRTC_PF_COMP3_INT_SR                     1'b0
    31:8    R/W RTC_PF_COMP_INT_WK_SR_DUMMY             24'b0
 */
typedef volatile union _AON_RTC_PF_COMP_INT_WK_SR_REG_TYPE
{
    uint32_t d32;
    struct
    {
        uint32_t RTC_PF_COMP0_WK_SR: 1;
        uint32_t RTC_PF_COMP1_WK_SR: 1;
        uint32_t RTC_PF_COMP2_WK_SR: 1;
        uint32_t RTC_PF_COMP3_WK_SR: 1;
        uint32_t RTC_PF_COMP0_INT_SR: 1;
        uint32_t RTC_PF_COMP1_INT_SR: 1;
        uint32_t RTC_PF_COMP2_INT_SR: 1;
        uint32_t RTC_PF_COMP3_INT_SR: 1;
        uint32_t RTC_PF_COMP_INT_WK_SR_DUMMY: 24;
    };
    struct
    {
        uint32_t RTC_PF_COMP_WK_SR: 4;
        uint32_t RTC_PF_COMP_INT_SR: 4;
        uint32_t RTC_PF_COMP_INT_WK_SR_ASSEMBLE: 24;
    };
} AON_RTC_PF_COMP_INT_WK_SR_REG_TYPE;

/* 0x64    0x4000_0164
    31:0    R   RTC_PF_RTL_VERSION                      32'h2208230A
 */
typedef volatile union _AON_RTC_PF_RTL_VERSION_REG_TYPE
{
    uint32_t d32;
    struct
    {
        uint32_t RTC_PF_RTL_VERSION;
    };
} AON_RTC_PF_RTL_VERSION_REG_TYPE;

/* 0x68    0x4000_0168
    0       R/W RTC_PF_COMP0_MANUAL_INT                 1'b0
    1       R/W RTC_PF_COMP1_MANUAL_INT                 1'b0
    2       R/W RTC_PF_COMP2_MANUAL_INT                 1'b0
    3       R/W RTC_PF_COMP3_MANUAL_INT                 1'b0
    31:4    R/W RTC_PF_COMP_MANUAL_INT_DUMMY            28'b0
 */
typedef volatile union _AON_RTC_PF_COMP_MANUAL_INT_REG_TYPE
{
    uint32_t d32;
    struct
    {
        uint32_t RTC_PF_COMP0_MANUAL_INT: 1;
        uint32_t RTC_PF_COMP1_MANUAL_INT: 1;
        uint32_t RTC_PF_COMP2_MANUAL_INT: 1;
        uint32_t RTC_PF_COMP3_MANUAL_INT: 1;
        uint32_t RTC_PF_COMP_MANUAL_INT_DUMMY: 28;
    };
    struct
    {
        uint32_t RTC_PF_COMP_MANUAL_INT: 4;
        uint32_t RTC_PF_COMP_MANUAL_INT_ASSEMBLE: 28;
    };
} AON_RTC_PF_COMP_MANUAL_INT_REG_TYPE;

/** @} */ /* End of group PLATFORM_RTC_Exported_Types */

/*============================================================================*
 *                              Functions
 *============================================================================*/
/** @defgroup PLATFORM_RTC_Exported_Functions Platform RTC Exported Functions
  * @{
  */

extern void (*platform_rtc_write_safe)(uint32_t, uint32_t);
extern void (*platform_rtc_aon_init)(void);
extern void (*platform_rtc_non_aon_init)(void);
extern void (*platform_rtc_reset)(void);
extern void (*platform_rtc_run_cmd)(bool);
extern uint32_t (*platform_rtc_get_counter)(void);
extern void (*platform_rtc_set_comp_intr_config)(PFRTCComparator, bool, bool);
extern void (*platform_rtc_set_comp_wk_config)(PFRTCComparator, bool);
extern void (*platform_rtc_set_intr_group_config)(bool);
extern void (*platform_rtc_set_wk_group_config)(bool);
extern bool (*platform_rtc_get_intr_group_config)(void);
extern bool (*platform_rtc_get_wk_group_config)(void);
extern void (*platform_rtc_set_comp)(PFRTCComparator, uint32_t);
extern uint32_t (*platform_rtc_get_comp)(PFRTCComparator);
extern PFRTCComparator(*platform_rtc_get_comp_intr_status)(void);
extern PFRTCComparator(*platform_rtc_get_comp_wk_status)(void);
extern void (*platform_rtc_set_comp_manual_intr)(PFRTCComparator);
extern PFRTCComparator(*platform_rtc_get_comp_manual_intr)(void);
extern void (*platform_rtc_clear_comp_intr_status)(PFRTCComparator);
extern void (*platform_rtc_clear_comp_wk_status)(PFRTCComparator);

/** @} */ /* End of group PLATFORM_RTC_Exported_Functions */

#ifdef __cplusplus
}
#endif

#endif /* __PLATFORM_RTC_H */
