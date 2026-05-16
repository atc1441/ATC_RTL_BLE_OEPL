
/**
*****************************************************************************************
*     Copyright(c) 2020, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
 * @file    rtl876x.h
 * @brief   CMSIS Cortex-M0+ Peripheral Access Layer Header File for
 *          RTL876X from Realtek Semiconductor.
 * @date    2020.10.10
 * @version v1.0

 * @date     3. March 2015
 *
 * @note     Generated with SVDConv Vx.xxp
 *           from CMSIS SVD File 'RTL876X.xml' Version x.xC,
 *
 * @par      Copyright (c) 2020 Realtek Semiconductor. All Rights Reserved.
 *
 *           The information contained herein is property of Realtek Semiconductor.
 *           Terms and conditions of usage are described in detail in Realtek
 *           SEMICONDUCTOR STANDARD SOFTWARE LICENSE AGREEMENT.
 *
 *           Licensees are granted free, non-transferable use of the information. NO
 *           WARRANTY of ANY KIND is provided. This heading must NOT be removed from
 *           the file.
 *
 *

 * *************************************************************************************
 * @attention
 * <h2><center>&copy; COPYRIGHT 2020 Realtek Semiconductor Corporation</center></h2>
 * *************************************************************************************
*/

#ifndef RTL876X_H
#define RTL876X_H

#ifdef __cplusplus
extern "C" {
#endif


/** @defgroup RTL876X Rtl876x
  * @brief    CMSIS Cortex-M0+ peripheral access layer header file for
  *           RTL876X from Realtek Semiconductor
  * @{
  */

/*============================================================================*
 *                              Types
*============================================================================*/
/** @defgroup RTL876x_Exported_types RTL876X Exported types
  * @{
  */

/** brief Interrupt Number Definition */
typedef enum IRQn
{
    /* -------------------  Cortex-M0+ Processor Exceptions Numbers  ------------------- */
    NonMaskableInt_IRQn           = -14,      /**<  2 Non Maskable Interrupt */
    HardFault_IRQn                = -13,      /**<  3 HardFault Interrupt */
    SVCall_IRQn                   =  -5,      /**< 11 SV Call Interrupt */
    DebugMonitor_IRQn             =  -4,      /**< 12 Debug Monitor Interrupt */
    PendSV_IRQn                   =  -2,      /**< 14 Pend SV Interrupt */
    SysTick_IRQn                  =  -1,      /**< 15 System Tick Interrupt */

    System_IRQn = 0,             /**<  [0] System Interrupt */
    WDG_IRQn,                    /**<  [1] Watch Dog Interrupt */
    BTMAC_IRQn,                  /**<  [2] BTMAC Interrupt ( an Extension of interrupt ) */
    Timer3_IRQn,                 /**<  [3] Timer3 global interrupt */
    Timer2_IRQn,                 /**<  [4] Timer2 global interrupt */
    Platform_IRQn,               /**<  [5] Platform error interrupt */
    I2S0_RX_IRQn,                /**<  [6] I2S0 RX interrupt */
    I2S0_TX_IRQn,                /**<  [7] I2S0 TX interrupt */
    Timer4_5_IRQn,               /**<  [8] Timer 4 to 5 interrupt  ( an Extension of interrupt ) */
    GPIO4_IRQn,                  /**<  [9] GPIO 4 interrupt */
    GPIO5_IRQn,                  /**<  [10] GPIO 5 interrupt */
    UART1_IRQn,                  /**<  [11] UART1 interrupt */
    UART0_IRQn,                  /**<  [12] UART0 interrupt */
    RTC_IRQn,                    /**<  [13] Realtime counter interrupt */
    SPI0_IRQn,                   /**<  [14] SPI0 interrupt */
    SPI1_IRQn,                   /**<  [15] SPI1 interrupt */
    I2C0_IRQn,                   /**<  [16] I2C0 interrupt */
    I2C1_IRQn,                   /**<  [17] I2C1 interrupt */
    ADC_IRQn,                    /**<  [18] ADC global interrupt */
    Peripheral_IRQn,             /**<  [19] Peripheral Interrupt  ( an Extension of interrupt ) */
    GDMA0_Channel0_IRQn,         /**<  [20] RTK-DMA0 channel 0 global interrupt */
    GDMA0_Channel1_IRQn,         /**<  [21] RTK-DMA0 channel 1 global interrupt */
    GDMA0_Channel2_IRQn,         /**<  [22] RTK-DMA0 channel 2 global interrupt */
    GDMA0_Channel3_IRQn,         /**<  [23] RTK-DMA0 channel 3 global interrupt */
    ENHANCED_TIMER0_IRQn,        /**<  [24] Enhanced timer0 interrupt */
    ENHANCED_TIMER1_IRQn,        /**<  [25] Enhanced timer1 interrupt */
    GPIO_Group3_IRQn,            /**<  [26] GPIO Group3 Interrupt ( an Extension of interrupt ) */
    GPIO_Group2_IRQn,            /**<  [27] GPIO Group2 Interrupt ( an Extension of interrupt )  */
    IR_IRQn,                     /**<  [28] IR Interrupt */
    GPIO_Group1_IRQn,            /**<  [29] GPIO Group1 Interrupt ( an Extension of interrupt )  */
    GPIO_Group0_IRQn,            /**<  [30] GPIO Group0 Interrupt ( an Extension of interrupt )  */
    Reserved_IRQn,               /**<  [31] Reserved */
    /******  Bee3 Extension Interrupt Numbers ************/
    TIMER4_IRQ                      = 8,
    TIMER5_IRQ                      = 8,
    GPIO3_IRQn                      = 26,
    GPIO7_IRQn                      = 26,
    GPIO11_IRQn                     = 26,
    GPIO15_IRQn                     = 26,
    GPIO19_IRQn                     = 26,
    GPIO23_IRQn                     = 26,
    GPIO27_IRQn                     = 26,
    GPIO31_IRQn                     = 26,
    GPIO2_IRQn                      = 27,
    GPIO6_IRQn                      = 27,
    GPIO10_IRQn                     = 27,
    GPIO14_IRQn                     = 27,
    GPIO18_IRQn                     = 27,
    GPIO22_IRQn                     = 27,
    GPIO26_IRQn                     = 27,
    GPIO30_IRQn                     = 27,
    GPIO1_IRQn                      = 29,
    GPIO9_IRQn                      = 29,
    GPIO13_IRQn                     = 29,
    GPIO17_IRQn                     = 29,
    GPIO21_IRQn                     = 29,
    GPIO25_IRQn                     = 29,
    GPIO29_IRQn                     = 29,
    GPIO0_IRQn                      = 30,
    GPIO8_IRQn                      = 30,
    GPIO12_IRQn                     = 30,
    GPIO16_IRQn                     = 30,
    GPIO20_IRQn                     = 30,
    GPIO24_IRQn                     = 30,
    GPIO28_IRQn                     = 30,
    Peripheral_First_IRQn           = 50,
    SPIC0_IRQn                      = 50,
    qdecode_IRQn                    = 51,
    KeyScan_IRQn                    = 52,
    SPI2W_IRQn                      = 53,
    LPCOMP_IRQn                     = 54,
    PTA_Mailbox_IRQn                = 55,
    CAP_TOUCH_IRQn                  = 56,
    TRNG_IRQn                       = 57,
} IRQn_Type, *PIRQn_Type;
/** @} */ /* End of group RTL876x_Exported_types */

/** @defgroup Configuration_of_CMSIS Configuration of CMSIS
  * @brief   Configuration of the cm4 Processor and Core Peripherals
  * @{
  */
/* ----------------Configuration of the cm0+ Processor and Core Peripherals---------------- */
#define __FPU_PRESENT                  0    /* FPU present                                                             */
#define __NVIC_PRIO_BITS               3    /*!< Number of Bits used for Priority Levels                               */
#define __Vendor_SysTickConfig         0    /*!< Set to 1 if different SysTick Config is used                          */
/** @} */ /* End of group Configuration_of_CMSIS */

/*============================================================================*
 *                               Header Files
*============================================================================*/
/* Processor and core peripherals */
#include "core_cm0plus.h"
#include "system_rtl876x.h"
#include "rtl876x_ic_type.h"

/*============================================================================*
 *                              Types
*============================================================================*/
/** @addtogroup RTL876x_Exported_types RTL876X Exported types
  * @{
  */

typedef enum
{
    RESET = 0,
    SET = !RESET
} FlagStatus, ITStatus;

typedef enum
{
    DISABLE = 0,
    ENABLE = !DISABLE
} FunctionalState;

#define IS_FUNCTIONAL_STATE(STATE) (((STATE) == DISABLE) || ((STATE) == ENABLE))
//typedef enum {ERROR = 0, SUCCESS = !ERROR} ErrorStatus;

/** @} */ /* End of group RTL876x_Exported_types */


/*============================================================================*
 *                              RTL876X Pin Number
*============================================================================*/
/** @defgroup RTL876X_Pin_Number RTL876X Pin Number
  * @{
  */
#if (IC_TYPE == IC_TYPE_BEE3)

#define P0_0    0   /**<GPIO0   */
#define P0_1    1   /**<GPIO1   */
#define P0_2    2   /**<GPIO2   */
#define P0_3    3   /**<GPIO3   */
#define P0_4    4   /**<GPIO4   */
#define P0_5    5   /**<GPIO5   */
#define P0_6    6   /**<GPIO6   */
#define P0_7    7   /**<GPIO7   */
#define P1_0    8   /**<GPIO8   */
#define P1_1    9   /**<GPIO9   */
#define P1_3    11
#define P1_4    12
#define P1_6    14   /**<GPIO14   */
#define P1_7    15   /**<GPIO15   */
#define P2_0    16   /**<GPIO16   */
#define P2_1    17   /**<GPIO17   */
#define P2_2    18   /**<GPIO18   */
#define P2_3    19   /**<GPIO19   */
#define P2_4    20   /**<GPIO20   */
#define P2_5    21   /**<GPIO21   */
#define P2_6    22   /**<GPIO22   */
#define P2_7    23   /**<GPIO23   */
#define P3_0    24   /**<GPIO24   */
#define P3_1    25   /**<GPIO25   */
#define P3_2    26   /**<GPIO26   */
#define P3_3    27   /**<GPIO27   */
#define P3_4    28   /**<GPIO28   */
#define P3_5    29   /**<GPIO29   */
#define P3_6    30   /**<GPIO30   */
#define P4_0    32   /**<GPIO13   */
#define P4_1    33   /**<GPIO29   */
#define P4_2    34   /**<GPIO30   */
#define P4_3    35   /**<GPIO31   */
#define H_0     36   /**<GPIO10   MICBIAS   */
#define P5_1    37   /**<GPIO11   */
#define P5_2    38   /**<GPIO12   */

#else

#define P0_0    0   /**<GPIO0   */
#define P0_1    1   /**<GPIO1   */
#define P0_2    2   /**<GPIO2   */
#define P0_3    3   /**<GPIO3   */
#define P0_4    4   /**<GPIO4   */
#define P0_5    5   /**<GPIO5   */
#define P0_6    6   /**<GPIO6   */
#define P0_7    7   /**<GPIO7   */
#define P1_0    8   /**<GPIO8   */
#define P1_1    9   /**<GPIO9   */
#define P1_2    10   /**<GPIO10   */
#define P1_3    11   /**<GPIO11   */
#define P1_4    12   /**<GPIO12   */
#define P1_5    13   /**<GPIO13   */
#define P1_6    14   /**<GPIO14   */
#define P1_7    15   /**<GPIO15   */
#define P2_0    16   /**<GPIO16   */
#define P2_1    17   /**<GPIO17   */
#define P2_2    18   /**<GPIO18   */
#define P2_3    19   /**<GPIO19   */
#define P2_4    20   /**<GPIO20   */
#define P2_5    21   /**<GPIO21   */
#define P2_6    22   /**<GPIO22   */
#define P2_7    23   /**<GPIO23   */
#define P3_0    24   /**<GPIO24   */
#define P3_1    25   /**<GPIO25   */
#define P3_2    26   /**<GPIO26   */
#define P3_3    27   /**<GPIO27   */
#define P3_4    28   /**<GPIO28   */
#define P3_5    29   /**<GPIO29   */
#define P3_6    30   /**<GPIO30   */
#define P4_0    32   /**<GPIO28   */
#define P4_1    33   /**<GPIO29   */
#define P4_2    34   /**<GPIO30   */
#define P4_3    35   /**<GPIO31   */
#define H_0     36   /**<GPIO25   MICBIAS   */
#define H_1     37   /**<GPIO26   32K_XI   */
#define H_2     38   /**<GPIO27   32K_XO   */
#endif

/** @} */ /* End of group RTL876X_Pin_Number */


/* ================================================================================ */
/* ================    Peripheral Registers Structures Section     ================ */
/* ================================================================================ */

/** @defgroup RTL876X_Peripheral_Registers_Structures RTL876X Register Structure
  * @{
  */

/* ================================================================================ */
/* ================                       ADC                      ================ */
/* ================================================================================ */

/**
  * @brief Analog to digital converter. (ADC)
  */

typedef struct                  /*!< ADC Structure */
{
    __O  uint32_t FIFO;         //!<0x00
    __IO uint32_t CR;          //!<0x04
    __IO uint32_t SCHCR;       //!<0x08
    __IO uint32_t INTCR;       //!<0x0C
    __IO uint32_t SCHTAB0;     //!<0x10
    __IO uint32_t SCHTAB1;     //!<0x14
    __IO uint32_t SCHTAB2;     //!<0x18
    __IO uint32_t SCHTAB3;     //!<0x1C
    __IO uint32_t SCHTAB4;     //!<0x20
    __IO uint32_t SCHTAB5;     //!<0x24
    __IO uint32_t SCHTAB6;     //!<0x28
    __IO uint32_t SCHTAB7;     //!<0x2C
    __IO uint32_t SCHD0;       //!<0x30
    __IO uint32_t SCHD1;       //!<0x34
    __IO uint32_t SCHD2;       //!<0x38
    __IO uint32_t SCHD3;       //!<0x3C
    __IO uint32_t SCHD4;       //!<0x40
    __IO uint32_t SCHD5;       //!<0x44
    __IO uint32_t SCHD6;       //!<0x48
    __IO uint32_t SCHD7;       //!<0x4C
    __IO uint32_t PWRDLY;      //!<0x50
    __IO uint32_t DATCLK;      //!<0x54
    __IO uint32_t ANACTL;      //!<0x58
    __IO uint32_t SAMTIM;      //!<0x5C

} ADC_TypeDef;

/* ================================================================================ */
/* ================                      CODEC                     ================ */
/* ================================================================================ */

typedef struct
{
    __IO uint32_t ANA_CR0;          /*!< 0x00 */
    __IO uint32_t ANA_CR1;          /*!< 0x04 */
    __IO uint32_t ANA_CR2;          /*!< 0x08 */
} CODEC_AnalogTypeDef;

typedef struct
{
    __IO uint32_t CR0;              /*!< 0x00 */
    __IO uint32_t ANA_CR1;          /*!< 0x04 */
    __IO uint32_t CLK_CR1;          /*!< 0x08 */
    __IO uint32_t CLK_CR2;          /*!< 0x0C */
    __IO uint32_t CLK_CR3;          /*!< 0x10 */
    __IO uint32_t RSVD0[2];
    __IO uint32_t I2S_CTRL;         /*!< 0x1C */
    __IO uint32_t ADC0_CTRL0;       /*!< 0x20 */
    __IO uint32_t ADC0_CTRL1;       /*!< 0x24 */
} CODEC_TypeDef;

/* ================================================================================ */
/* ================                    CAP TOUCH                   ================ */
/* ================================================================================ */

typedef struct
{
    __IO uint32_t CR;               /*!< 0x00 */
    __IO uint32_t SCAN_PERIOD;      /*!< 0x04 */
    __IO uint32_t ETC_CR;           /*!< 0x08 */
    __IO uint32_t SNR_INF;          /*!< 0x0C */
    __IO uint32_t DEBUG_CR;         /*!< 0x10 */
    __IO uint32_t FIFO;             /*!< 0x14 */
    __IO uint32_t RAW_FIFO;         /*!< 0x18 */
    __IO uint32_t GUARD_CNT;        /*!< 0x1C */
    __IO uint32_t INT_EN;           /*!< 0x20 */
    __IO uint32_t INT_SR;           /*!< 0x24 */
    __IO uint32_t RAW_INT_SR;       /*!< 0x28 */
    __IO uint32_t RSVD0;            /*!< 0x2C */
    __IO uint32_t INT_CLR;          /*!< 0x30 */
    __IO uint32_t INT_SCLR;         /*!< 0x34 */
    __IO uint32_t RSVD1[2];
    __IO uint32_t DBG_SEL;          /*!< 0x40 */
    __IO uint32_t DBG_PORT;         /*!< 0x44 */
    __IO uint32_t ECO_INF0;         /*!< 0x48 */
    __IO uint32_t ECO_INF1;         /*!< 0x4C */
    __IO uint32_t VERSION;          /*!< 0x50 */
} CTC_TypeDef;

typedef struct
{
    __IO uint32_t CR;               /*!< 0x00 */
    __IO uint32_t TOUCH_TH;         /*!< 0x04 */
    __IO uint32_t MBIAS;            /*!< 0x08 */
    __IO uint32_t DATA;             /*!< 0x0C */
} CTC_ChannelTypeDef;

typedef struct
{
    __IO uint32_t SCAN_INI;         /*!< 0x00 */
    __IO uint32_t SET_CNT;          /*!< 0x04 */
    __O  uint32_t TOUCH_CNT01;      /*!< 0x08 */
    __O  uint32_t TOUCH_CNT23;      /*!< 0x0C */
    __O  uint32_t TOUCH_SR;         /*!< 0x10 */
} CTC_FSModeTypeDef;

/* ================================================================================ */
/* ================                      GDMA                      ================ */
/* ================================================================================ */
/**
  * @brief GDMA
  */
typedef struct
{
    __I uint32_t RAW_TFR;
    uint32_t RSVD0;
    __I uint32_t RAW_BLOCK;
    uint32_t RSVD1;
    __I uint32_t RAW_SRC_TRAN;
    uint32_t RSVD2;
    __I uint32_t RAW_DST_TRAN;
    uint32_t RSVD3;
    __I uint32_t RAW_ERR;
    uint32_t RSVD4;

    __I uint32_t STATUS_TFR;
    uint32_t RSVD5;
    __I uint32_t STATUS_BLOCK;
    uint32_t RSVD6;
    __I uint32_t STATUS_SRC_TRAN;
    uint32_t RSVD7;
    __I uint32_t STATUS_DST_TRAN;
    uint32_t RSVD8;
    __I uint32_t STATUS_ERR;
    uint32_t RSVD9;

    __IO uint32_t MASK_TFR;
    uint32_t RSVD10;
    __IO uint32_t MASK_BLOCK;
    uint32_t RSVD11;
    __IO uint32_t MASK_SRC_TRAN;
    uint32_t RSVD12;
    __IO uint32_t MASK_DST_TRAN;
    uint32_t RSVD13;
    __IO uint32_t MASK_ERR;
    uint32_t RSVD14;

    __O uint32_t CLEAR_TFR;
    uint32_t RSVD15;
    __O uint32_t CLEAR_BLOCK;
    uint32_t RSVD16;
    __O uint32_t CLEAR_SRC_TRAN;
    uint32_t RSVD17;
    __O uint32_t CLEAR_DST_TRAN;
    uint32_t RSVD18;
    __O uint32_t CLEAR_ERR;
    uint32_t RSVD19;
    __O uint32_t StatusInt;
    uint32_t RSVD191;

    __IO uint32_t ReqSrcReg;
    uint32_t RSVD20;
    __IO uint32_t ReqDstReg;
    uint32_t RSVD21;
    __IO uint32_t SglReqSrcReg;
    uint32_t RSVD22;
    __IO uint32_t SglReqDstReg;
    uint32_t RSVD23;
    __IO uint32_t LstSrcReg;
    uint32_t RSVD24;
    __IO uint32_t LstDstReg;
    uint32_t RSVD25;

    __IO uint32_t DmaCfgReg;
    uint32_t RSVD26;
    __IO uint32_t ChEnReg;
    uint32_t RSVD27;
    __I uint32_t DmaIdReg;
    uint32_t RSVD28;
    __IO uint32_t DmaTestReg;
    uint32_t RSVD29;
} GDMA_TypeDef;

typedef struct
{
    __IO uint32_t SAR;
    uint32_t RSVD0;
    __IO uint32_t DAR;
    uint32_t RSVD1;
    __IO uint32_t LLP;
    uint32_t RSVD2;
    __IO uint32_t CTL_LOW;
    __IO uint32_t CTL_HIGH;
    __IO uint32_t SSTAT;
    uint32_t RSVD4;
    __IO uint32_t DSTAT;
    uint32_t RSVD5;
    __IO uint32_t SSTATAR;
    uint32_t RSVD6;
    __IO uint32_t DSTATAR;
    uint32_t RSVD7;
    __IO uint32_t CFG_LOW;
    __IO uint32_t CFG_HIGH;
    __IO uint32_t SGR;
    uint32_t RSVD9;
    __IO uint32_t DSR;
    uint32_t RSVD10;
} GDMA_ChannelTypeDef;

/* ================================================================================ */
/* ================                      GPIO                      ================ */
/* ================================================================================ */

/**
  * @brief General purpose input and output. (GPIO)
  */

typedef struct
{
    __IO uint32_t DATAOUT;              /*!< Data register: data output */
    __IO uint32_t DATADIR;              /*!< Data direction register */
    __IO uint32_t DATASRC;              /*!< Data source register  */
    uint32_t RSVD[9];
    __IO uint32_t INTEN;                /*!< Interrupt enable register */
    __IO uint32_t INTMASK;              /*!< Interrupt mask register */
    __IO uint32_t INTTYPE;              /*!< Interrupt level register */
    __IO uint32_t INTPOLARITY;          /*!< Interrupt polarity register */
    __IO uint32_t INTSTATUS;            /*!< Interrupt status of Port A  */
    __IO uint32_t RAWINTSTATUS;         /*!< Raw interrupt status of Port A (premasking) */
    __IO uint32_t DEBOUNCE;             /*!< Debounce enable register */
    __O  uint32_t INTCLR;               /*!< clear interrupt register */
    __I  uint32_t DATAIN;               /*!< external port register */
    uint32_t RSVD1[3];
    __IO uint32_t LSSYNC;               /*!< Level-sensitive synchronization enable register*/
    __I  uint32_t IDCODE;               /*!< ID code register */
    __IO uint32_t INTBOTHEDGE;          /*!< Both edge to trigger interrupt*/
} GPIO_TypeDef;

/* ================================================================================ */
/* ================                      I2C                      ================= */
/* ================================================================================ */

/**
  * @brief I2C
  */
typedef struct
{
    __IO uint32_t IC_CON;
    __IO uint32_t IC_TAR;
    __IO uint32_t IC_SAR;
    __IO uint32_t IC_HS_MADDR;
    __IO uint32_t IC_DATA_CMD;
    __IO uint32_t IC_SS_SCL_HCNT;
    __IO uint32_t IC_SS_SCL_LCNT;
    __IO uint32_t IC_FS_SCL_HCNT;
    __IO uint32_t IC_FS_SCL_LCNT;
    __IO uint32_t IC_HS_SCL_HCNT;
    __IO uint32_t IC_HS_SCL_LCNT;
    __I uint32_t IC_INTR_STAT;
    __IO uint32_t IC_INTR_MASK;
    __I uint32_t IC_RAW_INTR_STAT;
    __IO uint32_t IC_RX_TL;
    __IO uint32_t IC_TX_TL;
    __I uint32_t IC_CLR_INTR;
    __I uint32_t IC_CLR_RX_UNDER;
    __I uint32_t IC_CLR_RX_OVER;
    __I uint32_t IC_CLR_TX_OVER;
    __I uint32_t IC_CLR_RD_REQ;
    __I uint32_t IC_CLR_TX_ABRT;
    __I uint32_t IC_CLR_RX_DONE;
    __I uint32_t IC_CLR_ACTIVITY;
    __I uint32_t IC_CLR_STOP_DET;
    __I uint32_t IC_CLR_START_DET;
    __I uint32_t IC_CLR_GEN_CALL;
    __IO uint32_t IC_ENABLE;
    __I uint32_t IC_STATUS;
    __I uint32_t IC_TXFLR;
    __I uint32_t IC_RXFLR;
    __IO uint32_t IC_SDA_HOLD;
    __I uint32_t IC_TX_ABRT_SOURCE;
    __IO uint32_t IC_SLV_DATA_NACK_ONLY;
    __IO uint32_t IC_DMA_CR;
    __IO uint32_t IC_DMA_TDLR;
    __IO uint32_t IC_DMA_RDLR;
    __IO uint32_t IC_SDA_SETUP;
    __IO uint32_t IC_ACK_GENERAL_CALL;
    __IO uint32_t IC_ENABLE_STATUS;
} I2C_TypeDef;

/* ================================================================================ */
/* ================                      I2S                      ================= */
/* ================================================================================ */

/**
  * @brief I2S
  */
typedef struct
{
    __O  uint32_t TX_DR;            /*!< 0x00 */
    __IO uint32_t CTRL0;            /*!< 0x04 */
    __IO uint32_t CTRL1;            /*!< 0x08 */
    __IO uint32_t DSP_INT_CR;       /*!< 0x0C */
    __I  uint32_t RX_DR;            /*!< 0x10 */
    __I  uint32_t FIFO_SR;          /*!< 0x14 */
    __IO uint32_t ERROR_CNT_SR;     /*!< 0x18 */
    __IO uint32_t BCLK_DIV;         /*!< 0x1C */
    __IO uint32_t DMA_TRDLR;        /*!< 0x20 */
    __I  uint32_t SR;               /*!< 0x24 */
} I2S_TypeDef;

/* ================================================================================ */
/* ================                 IR                             ================ */
/* ================================================================================ */

/**
  * @brief IR.
  */

typedef struct
{
    __IO  uint32_t CLK_DIV;         /*!< 0x00 */
    __IO  uint32_t TX_CONFIG;       /*!< 0x04 */
    __IO  uint32_t TX_SR;           /*!< 0x08 */
    __IO  uint32_t RSVD0;           /*!< 0x0c */
    __IO  uint32_t TX_INT_CLR;      /*!< 0x10 */
    __IO  uint32_t TX_FIFO;         /*!< 0x14 */
    __IO  uint32_t RX_CONFIG;       /*!< 0x18 */
    __IO  uint32_t RX_SR;           /*!< 0x1c */
    __IO  uint32_t RX_INT_CLR;      /*!< 0x20 */
    __IO  uint32_t RX_CNT_INT_SEL;  /*!< 0x24 */
    __IO  uint32_t RX_FIFO;         /*!< 0x28 */
    __IO  uint32_t IR_VERSION;      /*!< 0x2c */
    __O   uint32_t RX_CUR_CNT;      /*!< 0x30 */
    __IO  uint32_t RX_EX_INT;       /*!< 0x34 */
    __IO  uint32_t RSVD1[4];
    __IO  uint32_t IR_TX_COMPE;     /*!< 0x48 */
    __IO  uint32_t RSVD2;           /*!< 0x4C */
    __IO  uint32_t DMA_CONFIG;      /*!< 0x50 */
} IR_TypeDef;

/* ================================================================================ */
/* ================                    Key Scan                    ================ */
/* ================================================================================ */

/**
  * @brief Key Scan. (KeyScan)
  */

typedef struct                          /*!< KeyScan Structure */
{

    __IO uint32_t CLKDIV;               /*!< 0x00 */
    __IO uint32_t TIMERCR;              /*!< 0x04 */
    __IO uint32_t CR;                   /*!< 0x08 */
    __IO uint32_t COLCR;                /*!< 0x0C */
    __IO uint32_t ROWCR;                /*!< 0x10 */
    __I  uint32_t FIFODATA;             /*!< 0x14 */
    __IO uint32_t INTMASK;              /*!< 0x18 */
    __IO uint32_t INTCLR;               /*!< 0x1C */
    __I  uint32_t STATUS;               /*!< 0x20 */

} KEYSCAN_TypeDef;

/* ================================================================================ */
/* ================                      LPC                      ================= */
/* ================================================================================ */

/**
  * @brief Real time for LP (RTC)
  */
typedef struct
{
    __IO uint32_t LPC_CR0;
    __IO uint32_t LPC_SR;
    __IO uint32_t LPC_CMP;
    __IO uint32_t LPC_CNT;
} LPC_TypeDef;

/* ================================================================================ */
/* ================                     Pinmux                     ================ */
/* ================================================================================ */

/**
  * @brief Pinmux. (Pinmux)
  */

typedef struct                      /*!< Pinmux Structure */
{
    __IO uint32_t
    CFG[10];                        /*!<  */
} PINMUX_TypeDef;

/* ================================================================================ */
/* ================                      PWM                       ================ */
/* ================================================================================ */

/**
  * @brief PWM
  */
typedef struct
{
    __IO uint32_t CR;               /*!< 0x00*/
} PWM_TypeDef;

typedef struct
{
    __IO uint32_t CR;               /*!< 0x00*/
} ENHPWM_TypeDef;

/* ================================================================================ */
/* ================                      QDEC                      ================ */
/* ================================================================================ */

/**
  * @brief Rotary decoder. (QDEC)
  */

typedef struct                      /*!< QDEC Structure           */
{
    __IO uint32_t   REG_DIV;        /*!< 0x00 */
    __IO uint32_t   REG_CR_X;       /*!< 0x04 */
    __IO uint32_t   REG_SR_X;       /*!< 0x08 */
    __IO uint32_t   REG_CR_Y;       /*!< 0x0C */
    __IO uint32_t   REG_SR_Y;       /*!< 0x10 */
    __IO uint32_t   REG_CR_Z;       /*!< 0x14 */
    __IO uint32_t   REG_SR_Z;       /*!< 0x18 */
    __IO uint32_t   INT_MASK;       /*!< 0x1C */
    __IO uint32_t   INT_SR;         /*!< 0x20 */
    __IO uint32_t   INT_CLR;        /*!< 0x24 */
    __IO uint32_t   REG_DBG;        /*!< 0x28 */
    __IO uint32_t   REG_VERSION;    /*!< 0x2C  */
} QDEC_TypeDef;

/* ================================================================================ */
/* ================                       RTC                      ================ */
/* ================================================================================ */

/**
  * @brief Real time counter 0. (RTC)
  */
typedef struct                      /*!< RTC Structure */
{
    __IO uint32_t CR0;              /*!< 0x00 */
    __IO uint32_t INT_CLR;          /*!< 0x04 */
    __IO uint32_t INT_SR;           /*!< 0x08 */
    __IO uint32_t PRESCALER;        /*!< 0x0C */
    __IO uint32_t COMP0;            /*!< 0x10 */
    __IO uint32_t COMP1;            /*!< 0x14 */
    __IO uint32_t COMP2;            /*!< 0x18 */
    __IO uint32_t COMP3;            /*!< 0x1C */
    __IO uint32_t COMP0GT;          /*!< 0x20 */
    __IO uint32_t COMP1GT;          /*!< 0x24 */
    __IO uint32_t COMP2GT;          /*!< 0x28 */
    __IO uint32_t COMP3GT;          /*!< 0x2C */
    __I  uint32_t CNT;              /*!< 0x30 */
    __IO uint32_t PRE_CNT;          /*!< 0x34 */
    __IO uint32_t PRE_COMP;         /*!< 0x38 */
    __IO uint32_t BACKUP;           /*!< 0x3C */
} RTC_TypeDef;

/* ================================================================================ */
/* ================                 2WIRE_SPI                      ================ */
/* ================================================================================ */

/**
  * @brief 2wire spi, mostly used with mouse sensor. (2WIRE_SPI)
  */

typedef struct                                      /*!< 3WIRE_SPI Structure */
{
    __IO  uint32_t RSVD0[12];       /*!< 0x00 -- 0x2C for Q-decode */
    __IO  uint32_t CFGR;            /*!< 0x30 */
    __IO  uint32_t CR;
    __IO  uint32_t INTCR;
    __I   uint32_t SR;
    __IO  uint32_t RD0;             /*!< 0x40 */
    __IO  uint32_t RD1;
    __IO  uint32_t RD2;
    __IO  uint32_t RD3;
} SPI3WIRE_TypeDef;

/* ================================================================================ */
/* ================                       SPI                      ================ */
/* ================================================================================ */

/**
  * @brief SPI master 0. (SPI)
  */
typedef struct                                          /*!< SPI Structure  */
{
    __IO  uint32_t    CTRLR0;                              /*!< 0x00 */
    __IO  uint32_t    CTRLR1;                              /*!< 0x04 */
    __IO  uint32_t    SSIENR;                              /*!< 0x08 */
    __IO  uint32_t    RSVD_0C;                             /*!< 0x0C */
    __IO  uint32_t    SER;                                 /*!< 0x10 */
    __IO  uint32_t    BAUDR;                               /*!< 0x14 */
    __IO  uint32_t    TXFTLR;                              /*!< 0x18 */
    __IO  uint32_t    RXFTLR;                              /*!< 0x1C */
    __I  uint32_t    TXFLR;                                /*!< 0x20 */
    __I  uint32_t    RXFLR;                                /*!< 0x24 */
    __I  uint32_t    SR;                                   /*!< 0x28 */
    __IO  uint32_t    IMR;                                 /*!< 0x2C */
    __I  uint32_t    ISR;                                  /*!< 0x30 */
    __I  uint32_t    RISR;                                 /*!< 0x34 */
    __I  uint32_t    TXOICR;                               /*!< 0x38 */
    __I  uint32_t    RXOICR;                               /*!< 0x3C */
    __I  uint32_t    RXUICR;                               /*!< 0x40 */
    __I  uint32_t    FAEICR;/*RSVD_44;*/                   /*!< 0x44 */
    __I  uint32_t    ICR;                                  /*!< 0x48 */
    __IO  uint32_t    DMACR;                               /*!< 0x4C */
    __IO  uint32_t    DMATDLR;                             /*!< 0x50 */
    __IO  uint32_t    DMARDLR;                             /*!< 0x54 */
    __I  uint32_t    TXUICR;/*IDR;*/                       /*!< 0x58 */
    __I  uint32_t    SSRICR;                               /*!< 0x5C */
    __IO  uint32_t    DR[36];                              /*!< 0x60 - 0xEC */
    __IO  uint32_t    RX_SAMPLE_DLY;                       /*!< 0xF0 */
} SPI_TypeDef;

/* ================================================================================ */
/* ================                      TIM                      ================ */
/* ================================================================================ */

/**
  * @brief TIM
  */
typedef struct
{
    __IO uint32_t LoadCount;        /*!< 0x00*/
    __I  uint32_t CurrentValue;     /*!< 0x04*/
    __IO uint32_t ControlReg;       /*!< 0x08*/
    __I  uint32_t EOI;              /*!< 0x0C*/
    __I  uint32_t IntStatus;        /*!< 0x10*/
} TIM_TypeDef;

/* ================================================================================ */
/* ================                      ENH_TIM                   ================ */
/* ================================================================================ */

/**
  * @brief ENHTIMER
  */
typedef struct
{
    __O  uint32_t CUR_CNT;          /*!< 0x00*/
    __O  uint32_t LATCH_CNT0;       /*!< 0x04*/
    __O  uint32_t LATCH_CNT1;       /*!< 0x08*/
    __O  uint32_t LATCH_CNT2;       /*!< 0x0C*/
    __IO uint32_t CR;               /*!< 0x10*/
    __IO uint32_t MAX_CNT;          /*!< 0x14*/
    __IO uint32_t CCR;              /*!< 0x18*/
    __IO uint32_t CCR_FIFO;         /*!< 0x1C*/
} ENHTIM_TypeDef;

/**
  * @brief ENHTIMER
  */
typedef struct
{
    __O  uint32_t FIFO_SR0;         /*!< 0x00*/
    __O  uint32_t RSVD0;            /*!< 0x04*/
    __O  uint32_t FIFO_SR2;         /*!< 0x08*/
    __IO uint32_t FIFO_CLR;         /*!< 0x0C*/
    __IO uint32_t CMD;              /*!< 0x10*/
    __IO uint32_t INT_CMD;          /*!< 0x14*/
    __IO uint32_t INT_SR;           /*!< 0x18*/
    __O  uint32_t MASK_INT_SR;      /*!< 0x1C*/
    __IO uint32_t LC_INT_CMD0;      /*!< 0x20*/
    __IO uint32_t RSVD1;            /*!< 0x24*/
    __IO uint32_t LC_INT_CMD2;      /*!< 0x28*/
    __IO uint32_t RSVD2;            /*!< 0x2C*/
    __O  uint32_t LC_FIFO_LEVEL0;   /*!< 0x30*/
    __O  uint32_t LC_FIFO_LEVEL1;   /*!< 0x34*/

} ENHTIM_ShareTypeDef;

/* ================================================================================ */
/* ================                      UART                      ================ */
/* ================================================================================ */

/**
  * @brief Universal Asynchronous Receiver/Transmitter, version 1.0. (UART)
  */

typedef struct                      /*!< UART Structure */
{
    __IO  uint32_t DLL;
    __IO  uint32_t DLH_INTCR;
    __IO  uint32_t INTID_FCR;
    __IO  uint32_t LCR;
    __IO  uint32_t MCR;             /*!<0x10 */
    __I   uint32_t LSR;
    __I   uint32_t MSR;
    __IO  uint32_t SPR;
    __IO  uint32_t STSR;            /*!<0x20 */
    __IO  uint32_t RB_THR;
    __IO  uint32_t MISCR;           /*!<may not be seen for client */
    __IO  uint32_t TXPLSR;
    __IO  uint32_t RSVD0;           /*!<0x30 */
    __IO  uint32_t RSVD1;           /*!<0x34 */
    __IO  uint32_t RSVD2;           /*!<0x38 */
    __IO  uint32_t DBG_UART;        /*!<0x3C */
    __IO  uint32_t RX_IDLE_TOCR;    /*!<0x40 */
    __IO  uint32_t RX_IDLE_INTSR;
    __IO  uint32_t RX_IDLE_INTCR;
    __I   uint32_t FIFO_LEVEL;
    __IO  uint32_t INTMASK;         /*!<0x50 */
    __I   uint32_t TX_DONE_INTSR;
    __I   uint32_t TX_THR_INTSR;
} UART_TypeDef;

typedef enum
{
    LOG_CHANNEL_UART0,
    LOG_CHANNEL_UART1,
    LOG_CHANNEL_UART2
} LogChannel_TypeDef;

/* ================================================================================ */
/* ================                     Watch Dog                  ================ */
/* ================================================================================ */

/**
  * @brief Watch Dog. (WDG)
  */

typedef struct                      /*!< WDG Structure           */
{
    __IO uint32_t WDG_CTL;          /*!< 0x00 */
} WDG_TypeDef;

/**
  * @brief AON Watchdog
  */
typedef struct
{
    union
    {
        __IO uint32_t CRT; /*!< 0x00 (R/W) Control Register                                       */
        struct
        {
            uint32_t EN: 2;     /*!< R/W WDT2 Enable, 2'b10: OFF, the other value: ON             */
            uint32_t CNT_CTL: 1; /*!< R/W Counter Control,
                                     1: WDG continue count in DLPS/Power Down/Hibernate,
                                     0: WDG stop count in DLPS/Power Down/Hibernate               */
            uint32_t RST_LVL: 1; /*!< R/W Reset Level,
                                   1: Level 2 reset (reset whole chip),
                                   0: Level 1 reset (reset whole chip except partial AON and RTC) */
            uint32_t CNT_RELOAD: 1; /*!< R/W Reload Counter,
                                        1: Reload counter when exit DLPS/Power Down/Hibernate,
                                        0: Not reload counter when exit DLPS/Power Down/Hibernate */
            uint32_t RSVD0: 3;
            uint32_t COMP: 18;  /*!< R/W Set the period of AON WDG : 1/450Hz ~ 1/2.3KHz           */
            uint32_t RSVD1: 6;
        } CRT_BITS;
    } u;
    __IO uint32_t CNT_CLR; /*!< 0x04 (R/W) Clear Register, write 1 to reload AON WDG counter      */
    __IO uint32_t WP;      /*!< 0x08 (R/W) Write Protect Register, write 1 to enable write CRT    */
} AON_WDG_TypeDef;

/* ================================================================================ */
/* ================                     random generator           ================ */
/* ================================================================================ */

/**
  * @brief random generator. (RAN_GEN)
  */

typedef struct                                      /*!< RAN_GEN Structure */
{
    union
    {
        __IO uint32_t CTL;                                        /*!< 0x00              */
        struct
        {
            __IO uint32_t rand_gen_en: 1;
            __IO uint32_t seed_upd: 1;
            __IO uint32_t random_req: 1;
            __IO uint32_t opt_rand_upd: 1;
            __IO uint32_t soft_rst: 1;
            __IO uint32_t rsvd: 27;
        } CTL_BITS;
    } u;
    __IO uint32_t POLYNOMIAL;                                     /*!< 0x04              */
    __IO uint32_t SEED;                                           /*!< 0x08              */
    __IO uint32_t RAN_NUM;                                        /*!< 0x0C              */
} RAN_GEN_TypeDef;

/* ================================================================================ */
/* ================            System Block Control            ================ */
/* ================================================================================ */

/**
  * @brief System Block Control. (SYS_BLKCTRL)
  */

typedef struct                                          /*!< SYS_BLKCTRL Structure      */
{
    /* 0x0200       0x4000_0200
    0       R/W    r_cpu_slow_en                   0
    1       R/W    r_cpu_slow_opt_wfi              0
    2       R/W    r_cpu_slow_opt_at_tx            1
    3       R/W    r_cpu_slow_opt_at_rx            1
    4       R/W    r_cpu_slow_opt_lcd              0
    5       R/W    r_auto_slow_opt                 0
    6       W1O    reg_zone1_wrp_en                0
    7       W1O    reg_zone2_wrp_en                0
    8       W1O    reg_wrp_lock                    0
    12:9    R/W    r_cpu_low_rate_valid_num        4'h3
    13      R/W    r_cpu_auto_slow_force_update    0
    14      R/W    r_cpu_slow_opt_spi0             0
    18:15   R/W    r_cpu_low_rate_valid_num1       4'h3
    24:19   R/W    r_bt_ahb_wait_cnt               6'h0C
    27:25   R/W    DUMMY                           3'h0
    28      R/W    r_aon_rd_opt                    1
    29      R/W    r_btaon_acc_no_block            0
    30      R/W    bzdma_autoslow_eco_disable      0
    31      R/W    r_clk_cpu_32k_en                0
    */
    union
    {
        __IO uint32_t PERION_REG_SYS_CLK_SEL_0;
        struct
        {
            __IO uint32_t r_cpu_slow_en: 1;
            __IO uint32_t r_cpu_slow_opt_wfi: 1;
            __IO uint32_t r_cpu_slow_opt_at_tx: 1;
            __IO uint32_t r_cpu_slow_opt_at_rx: 1;
            __IO uint32_t r_cpu_slow_opt_lcd: 1;
            __IO uint32_t r_auto_slow_opt: 1;
            __IO uint32_t reg_zone1_wrp_en: 1;
            __IO uint32_t reg_zone2_wrp_en: 1;
            __IO uint32_t reg_wrp_lock: 1;
            __IO uint32_t r_cpu_low_rate_valid_num: 4;
            __IO uint32_t r_cpu_auto_slow_force_update: 1;
            __IO uint32_t r_cpu_slow_opt_spi0: 1;
            __IO uint32_t r_cpu_low_rate_valid_num1: 4;
            __IO uint32_t r_bt_ahb_wait_cnt: 6;
            __IO uint32_t RESERVED_0: 3;
            __IO uint32_t r_aon_rd_opt: 1;
            __IO uint32_t r_btaon_acc_no_block: 1;
            __IO uint32_t bzdma_autoslow_eco_disable: 1;
            __IO uint32_t r_clk_cpu_32k_en: 1;
        } BITS_200;
    } u_200;

    /* 0x0204       0x4000_0204
        3:0     R      RF_RL_ID                        4'd0
        7:4     R      RF_RTL_ID                       4'd0
        31:8    R      RSVD                            24'd0
    */
    union
    {
        __IO uint32_t PERION_REG_ID;
        struct
        {
            __I uint32_t RF_RL_ID: 4;
            __I uint32_t RF_RTL_ID: 4;
            __I uint32_t RESERVED_0: 24;
        } BITS_204;
    } u_204;

    /* 0x0208       0x4000_0208
        3:0     R/W    r_cpu_div_sel                   4'h0
        7:4     R/W    r_cpu_div_sel_slow              4'h0
        8       R/W    r_cpu_div_en                    1
        9       R/W    r_CPU_CLK_SRC_EN                1
        10      R/W    r_cpu_auto_slow_filter_en       1
        11      R/W    r_cpu_clk_mux_0_cg_en           1
        12      R/W    DUMMY                           1
        13      R/W    r_cpu_xtal_clk_cg_en            1
        14      R/W    r_cpu_ocs40_clk_cg_en           1
        15      R/W    r_cpu_div_en_slow               1
        16      W1O    DUMMY                           0
        17      W1O    r_cpu_div_sel_opt               0
        18      W1O    r_spi0_div_sel_opt              0
        19      W1O    r_spi1_div_sel_opt              0
        20      W1O    RSVD                            0
        21      W1O    RSVD                            0
        22      W1O    RSVD                            0
        23      W1O    r_spi0_div_sel_opt1             0
        24      W1O    r_spi1_div_sel_opt1             0
        25      W1O    DUMMY                           0
        26      W1O    DUMMY                           0
        27      R/W    r_CKE_SYSON                     0
        28      R/W    r_cpu_auto_slow_filter1_en      0
        31:29   R/W    DUMMY                           3'h0
    */
    union
    {
        __IO uint32_t PERION_REG_SYS_CLK_SEL_1;
        struct
        {
            __IO uint32_t r_cpu_div_sel: 4;
            __IO uint32_t r_cpu_div_sel_slow: 4;
            __IO uint32_t r_cpu_div_en: 1;
            __IO uint32_t r_CPU_CLK_SRC_EN: 1;
            __IO uint32_t r_cpu_auto_slow_filter_en: 1;
            __IO uint32_t r_cpu_clk_mux_0_cg_en: 1;
            __IO uint32_t RESERVED_7: 1;
            __IO uint32_t r_cpu_xtal_clk_cg_en: 1;
            __IO uint32_t r_cpu_ocs40_clk_cg_en: 1;
            __IO uint32_t r_cpu_div_en_slow: 1;
            __IO uint32_t RESERVED_6: 1;
            __IO uint32_t r_cpu_div_sel_opt: 1;
            __IO uint32_t r_spi0_div_sel_opt: 1;
            __IO uint32_t r_spi1_div_sel_opt: 1;
            __IO uint32_t RESERVED_5: 1;
            __IO uint32_t RESERVED_4: 1;
            __IO uint32_t RESERVED_3: 1;
            __IO uint32_t r_spi0_div_sel_opt1: 1;
            __IO uint32_t r_spi1_div_sel_opt1: 1;
            __IO uint32_t RESERVED_2: 1;
            __IO uint32_t RESERVED_1: 1;
            __IO uint32_t r_CKE_SYSON: 1;
            __IO uint32_t r_cpu_auto_slow_filter1_en: 1;
            __IO uint32_t RESERVED_0: 3;
        } BITS_208;
    } u_208;

    /* 0x020C       0x4000_020c
        1:0     R/W    r_flash_div_sel                 2'h0
        3:2     R/W    DUMMY                           2'h0
        4       R/W    r_flash_div_en                  0
        5       R/W    r_FLASH_CLK_SRC_EN              1
        6       R/W    r_flash_clk_src_sel_1           0
        7       R/W    r_flash_clk_src_sel_0           0
        8       R/W    r_flash_mux_1_clk_cg_en         1
        9       R/W    r_rng_sfosc_sel                 0
        12:10   R/W    r_rng_sfosc_div_sel             3'h0
        15:13   R/W    DUMMY                           0
        18:16   R/W    DUMMY                           3'h0
        19      R/W    DUMMY                           0
        20      R/W    DUMMY                           0
        21      R/W    DUMMY                           1
        22      R/W    r_CLK_40M_SRC_EN                1
        23      R/W    DUMMY                           0
        24      R/W    r_40m_clk_src_sel_0             1
        25      R/W    DUMMY                           0
        26      R/W    r_CLK_40M_SRC_DIV_EN            1
        27      R/W    r_CLK_20M_SRC_EN                0
        28      R/W    r_CLK_10M_SRC_EN                1
        29      R/W    r_CLK_5M_SRC_EN                 0
        31:30   R/W    DUMMY                           2'h0
    */
    union
    {
        __IO uint32_t PERION_REG_SYS_CLK_SEL_2;
        struct
        {
            __IO uint32_t r_flash_div_sel: 2;
            __IO uint32_t RESERVED_8: 2;
            __IO uint32_t r_flash_div_en: 1;
            __IO uint32_t r_FLASH_CLK_SRC_EN: 1;
            __IO uint32_t r_flash_clk_src_sel_1: 1;
            __IO uint32_t r_flash_clk_src_sel_0: 1;
            __IO uint32_t r_flash_mux_1_clk_cg_en: 1;
            __IO uint32_t r_rng_sfosc_sel: 1;
            __IO uint32_t r_rng_sfosc_div_sel: 3;
            __IO uint32_t RESERVED_7: 3;
            __IO uint32_t RESERVED_6: 3;
            __IO uint32_t RESERVED_5: 1;
            __IO uint32_t RESERVED_4: 1;
            __IO uint32_t RESERVED_3: 1;
            __IO uint32_t r_CLK_40M_SRC_EN: 1;
            __IO uint32_t RESERVED_2: 1;
            __IO uint32_t r_40m_clk_src_sel_0: 1;
            __IO uint32_t RESERVED_1: 1;
            __IO uint32_t r_CLK_40M_SRC_DIV_EN: 1;
            __IO uint32_t r_CLK_20M_SRC_EN: 1;
            __IO uint32_t r_CLK_10M_SRC_EN: 1;
            __IO uint32_t r_CLK_5M_SRC_EN: 1;
            __IO uint32_t RESERVED_0: 2;
        } BITS_20C;
    } u_20C;

    /* 0x210        0x4000_0210
        0       R/W    DUMMY                           0
        1       R/W    DUMMY                           0
        2       R/W    r_PON_FEN_LXBUS                 1
        3       R/W    DUMMY                           0
        4       R/W    r_PON_FEN_FLASH                 1
        5       R/W    DUMMY                           1
        6       R/W    DUMMY                           0
        7       R/W    DUMMY                           0
        8       R/W    DUMMY                           0
        11:9    R/W    DUMMY                           3'h0
        12      R/W    r_PON_FEN_UART                  0
        13      R/W    r_PON_FEN_GDMA0                 0
        14      R/W    DUMMY                           0
        15      R/W    DUMMY                           0
        16      R/W    r_PON_FEN_TIMER                 0
        19:17   R/W    DUMMY                           0
        20      R/W    r_PON_FEN_AAC_XTAL              0
        21      R/W    r_PON_FEN_CAP_TOUCH             0
        22      R/W    DUMMY                           0
        23      R/W    DUMMY                           0
        31:24   R      DUMMY                           8'd0
    */
    union
    {
        __IO uint32_t PERION_REG_SOC_FUNC_EN;
        struct
        {
            __IO uint32_t RESERVED_13: 1;
            __IO uint32_t RESERVED_12: 1;
            __IO uint32_t r_PON_FEN_LXBUS: 1;
            __IO uint32_t RESERVED_11: 1;
            __IO uint32_t r_PON_FEN_FLASH: 1;
            __IO uint32_t RESERVED_10: 1;
            __IO uint32_t RESERVED_9: 1;
            __IO uint32_t RESERVED_8: 1;
            __IO uint32_t RESERVED_7: 1;
            __IO uint32_t RESERVED_6: 3;
            __IO uint32_t r_PON_FEN_UART: 1;
            __IO uint32_t r_PON_FEN_GDMA0: 1;
            __IO uint32_t RESERVED_5: 1;
            __IO uint32_t RESERVED_4: 1;
            __IO uint32_t r_PON_FEN_TIMER: 1;
            __IO uint32_t RESERVED_3: 3;
            __IO uint32_t r_PON_FEN_AAC_XTAL: 1;
            __IO uint32_t r_PON_FEN_CAP_TOUCH: 1;
            __IO uint32_t RESERVED_2: 1;
            __IO uint32_t RESERVED_1: 1;
            __I uint32_t RESERVED_0: 8;
        } BITS_210;
    } u_210;

    /* 0x0214       0x4000_0214
        31:0    R      RSVD                            32'hEAEAEAEA
    */
    __IO uint32_t PERION_REG_RSVD_214;

    /* 0x0218       0x4000_0218
        0       R/W    r_PON_FEN_URT0_GLB              0
        1       R/W    DUMMY                           0
        2       R/W    r_PON_FEN_AES                   0
        3       R/W    r_PON_FEN_RNG                   0
        7:4     R/W    DUMMY                           4'h0
        8       R/W    r_PON_FEN_SPI0                  0
        9       R/W    r_PON_FEN_SPI1                  0
        10      R/W    r_PON_FEN_IRRC                  0
        15:11   R/W    DUMMY                           5'd0
        16      R/W    r_PON_FEN_I2C0                  0
        17      R/W    r_PON_FEN_I2C1                  0
        18      R/W    r_PON_FEN_QDEC                  0
        19      R/W    r_PON_FEN_KEYSCAN               0
        23:20   R/W    DUMMY                           4'h0
        24      R/W    r_PON_FEN_SPI2W                 0
        25      R/W    DUMMY                           1
        26      R/W    r_PON_FEN_DSP_IPC               1
        27      R/W    r_PON_FEN_DSP_MEM               1
        28      R/W    r_PON_FEN_ASRC                  0
        29      R/W    r_PON_FEN_DSP_WDT               0
        30      R/W    r_PON_FEN_EFUSE                 0
        31      R/W    DUMMY                           0
    */
    union
    {
        __IO uint32_t PERION_REG_SOC_PERI_FUNC0_EN;
        struct
        {
            __IO uint32_t r_PON_FEN_URT0_GLB: 1;
            __IO uint32_t RESERVED_5: 1;
            __IO uint32_t r_PON_FEN_AES: 1;
            __IO uint32_t r_PON_FEN_RNG: 1;
            __IO uint32_t RESERVED_4: 4;
            __IO uint32_t r_PON_FEN_SPI0: 1;
            __IO uint32_t r_PON_FEN_SPI1: 1;
            __IO uint32_t r_PON_FEN_IRRC: 1;
            __IO uint32_t RESERVED_3: 5;
            __IO uint32_t r_PON_FEN_I2C0: 1;
            __IO uint32_t r_PON_FEN_I2C1: 1;
            __IO uint32_t r_PON_FEN_QDEC: 1;
            __IO uint32_t r_PON_FEN_KEYSCAN: 1;
            __IO uint32_t RESERVED_2: 4;
            __IO uint32_t r_PON_FEN_SPI2W: 1;
            __IO uint32_t RESERVED_1: 1;
            __IO uint32_t r_PON_FEN_DSP_IPC: 1;
            __IO uint32_t r_PON_FEN_DSP_MEM: 1;
            __IO uint32_t r_PON_FEN_ASRC: 1;
            __IO uint32_t r_PON_FEN_DSP_WDT: 1;
            __IO uint32_t r_PON_FEN_EFUSE: 1;
            __IO uint32_t RESERVED_0: 1;
        } BITS_218;
    } u_218;

    /* 0x021C       0x4000_021c
        0       R/W    r_PON_FEN_ADC0                  0
        7:1     R/W    DUMMY                           6'd0
        8       R/W    r_PON_FEN_GPIO0                 0
        31 :9   R/W    DUMMY                           23'd0
    */
    union
    {
        __IO uint32_t PERION_REG_SOC_PERI_FUNC1_EN;
        struct
        {
            __IO uint32_t r_PON_FEN_ADC0: 1;
            __IO uint32_t RESERVED_1: 7;
            __IO uint32_t r_PON_FEN_GPIO0: 1;
            __IO uint32_t RESERVED_0: 23;
        } BITS_21C;
    } u_21C;

    /* 0x0220       0x4000_0220
        0       R/W    r_PON_FEN_AUDIO                 0
        1       R/W    r_PON_FEN_SPORT0                0
        2       R/W    DUMMY                           0
        3       R/W    DUMMY                           0
        4       R/W    r_CLK_EN_AUDIO                  0
        5       R/W    r_CLK_EN_SPORT0                 0
        6       R/W    DUMMY                           0
        7       R/W    DUMMY                           0
        8       R/W    r_CLK_EN_SPORT_40M              0
        9       R/W    r_CLK_EN_AUDIO_REG              1
        31:10   R/W    DUMMY                           22'd0
    */
    union
    {
        __IO uint32_t PERION_REG_SOC_AUDIO_IF_EN;
        struct
        {
            __IO uint32_t r_PON_FEN_AUDIO: 1;
            __IO uint32_t r_PON_FEN_SPORT0: 1;
            __IO uint32_t RESERVED_4: 1;
            __IO uint32_t RESERVED_3: 1;
            __IO uint32_t r_CLK_EN_AUDIO: 1;
            __IO uint32_t r_CLK_EN_SPORT0: 1;
            __IO uint32_t RESERVED_2: 1;
            __IO uint32_t RESERVED_1: 1;
            __IO uint32_t r_CLK_EN_SPORT_40M: 1;
            __IO uint32_t r_CLK_EN_AUDIO_REG: 1;
            __IO uint32_t RESERVED_0: 22;
        } BITS_220;
    } u_220;

    /* 0x0224       0x4000_0224
        2:0     R/W    DUMMY                           3'h4
        3       R/W    r_SPORT0_EXT_CODEC              0
        6:4     R/W    DUMMY                           3'h1
        7       R/W    r_CODEC_STANDALONE              0
        31:8    R/W    DUMMY                           24'd0
    */
    union
    {
        __IO uint32_t PERION_REG_SOC_AUDIO_CLK_CTRL_A;
        struct
        {
            __IO uint32_t RESERVED_2: 3;
            __IO uint32_t r_SPORT0_EXT_CODEC: 1;
            __IO uint32_t RESERVED_1: 3;
            __IO uint32_t r_CODEC_STANDALONE: 1;
            __IO uint32_t RESERVED_0: 24;
        } BITS_224;
    } u_224;

    /* 0x0228       0x4000_0228
        3:0     R/W    DUMMY                           4'h1
        4       R/W    r_SPORT0_MCLK_OUT               0
        6:5     R/W    DUMMY                           0
        7       R/W    DUMMY                           0
        31:8    R/W    DUMMY                           24'd0
    */
    union
    {
        __IO uint32_t PERION_REG_SOC_AUDIO_CLK_CTRL_B;
        struct
        {
            __IO uint32_t RESERVED_3: 4;
            __IO uint32_t r_SPORT0_MCLK_OUT: 1;
            __IO uint32_t RESERVED_2: 2;
            __IO uint32_t RESERVED_1: 1;
            __IO uint32_t RESERVED_0: 24;
        } BITS_228;
    } u_228;

    /* 0x022C       0x4000_022c
        31:0    R      RSVD                            32'hEAEAEAEA
    */
    __IO uint32_t PERION_REG_RSVD_22C;

    /* 0x0230       0x4000_0230
        0       R/W    DUMMY                           1
        1       R/W    DUMMY                           0
        2       R/W    r_PON_CKE_PLFM                  1
        3       R/W    DUMMY                           1
        4       R/W    r_CKE_BUS_RAM_SLP               0
        5       R/W    r_CKE_BT_VEN                    1
        6       R/W    r_PON_ACTCKE_VDREG              0
        7       R/W    r_PON_SLPCKE_VDREG              0
        8       R/W    r_PON_ACTCKE_FLASH              1
        9       R/W    r_PON_SLPCKE_FLASH              0
        10      R/W    DUMMY                           0
        11      R/W    DUMMY                           0
        12      R/W    r_PON_ACTCKE_LUART              0
        13      R/W    r_PON_SLPCKE_LUART              0
        14      R/W    r_PON_ACTCKE_TIMER              0
        15      R/W    r_PON_SLPCKE_TIMER              0
        16      R/W    r_PON_ACTCKE_GDMA0              0
        17      R/W    r_PON_SLPCKE_GDMA0              0
        23:18   R/W    DUMMY                           6'd0
        24      R/W    r_PON_ACTCKE_GPIO               0
        25      R/W    r_PON_SLPCKE_GPIO               0
        26      R/W    DUMMY                           0
        27      R/W    DUMMY                           0
        28      R/W    DUMMY                           0
        29      R/W    DUMMY                           0
        30      R/W    DUMMY                           0
        31      R/W    DUMMY                           0
    */
    union
    {
        __IO uint32_t PERION_REG_PESOC_CLK_CTRL;
        struct
        {
            __IO uint32_t RESERVED_11: 1;
            __IO uint32_t RESERVED_10: 1;
            __IO uint32_t r_PON_CKE_PLFM: 1;
            __IO uint32_t RESERVED_9: 1;
            __IO uint32_t r_CKE_BUS_RAM_SLP: 1;
            __IO uint32_t r_CKE_BT_VEN: 1;
            __IO uint32_t r_PON_ACTCKE_VDREG: 1;
            __IO uint32_t r_PON_SLPCKE_VDREG: 1;
            __IO uint32_t r_PON_ACTCKE_FLASH: 1;
            __IO uint32_t r_PON_SLPCKE_FLASH: 1;
            __IO uint32_t RESERVED_8: 1;
            __IO uint32_t RESERVED_7: 1;
            __IO uint32_t r_PON_ACTCKE_LUART: 1;
            __IO uint32_t r_PON_SLPCKE_LUART: 1;
            __IO uint32_t r_PON_ACTCKE_TIMER: 1;
            __IO uint32_t r_PON_SLPCKE_TIMER: 1;
            __IO uint32_t r_PON_ACTCKE_GDMA0: 1;
            __IO uint32_t r_PON_SLPCKE_GDMA0: 1;
            __IO uint32_t RESERVED_6: 6;
            __IO uint32_t r_PON_ACTCKE_GPIO: 1;
            __IO uint32_t r_PON_SLPCKE_GPIO: 1;
            __IO uint32_t RESERVED_5: 1;
            __IO uint32_t RESERVED_4: 1;
            __IO uint32_t RESERVED_3: 1;
            __IO uint32_t RESERVED_2: 1;
            __IO uint32_t RESERVED_1: 1;
            __IO uint32_t RESERVED_0: 1;
        } BITS_230;
    } u_230;

    /* 0x0234       0x4000_0234
        0       R/W    r_PON_ACTCKE_UART0              0
        1       R/W    r_PON_SLPCKE_UART0              0
        2       R/W    DUMMY                           0
        3       R/W    DUMMY                           0
        4       R/W    r_CKE_MODEM                     1
        5       R/W    r_CKE_CAL32K                    0
        6       R/W    r_CKE_RNG                       0
        7       R/W    DUMMY                           0
        8       R/W    DUMMY                           0
        9       R/W    r_CKE_PDCK                      0
        10      R/W    r_CKE_AAC_XTAL                  0
        11      R/W    r_PON_CKE_BT_FREQ_DET           0
        12      R/W    r_PON_CKE_CAP_TOUCH             0
        15:13   R/W    DUMMY                           3'h0
        16      R/W    r_PON_ACTCK_SPI0                0
        17      R/W    r_PON_SLPCK_SPI0                0
        18      R/W    r_PON_ACTCK_SPI1                0
        19      R/W    r_PON_SLPCK_SPI1                0
        20      R/W    r_PON_ACTCK_IRRC                0
        21      R/W    r_PON_SLPCKE_IRRC               0
        23:22   R/W    DUMMY                           2'h0
        24      R/W    DUMMY                           0
        25      R/W    DUMMY                           0
        26      R/W    DUMMY                           0
        27      R/W    DUMMY                           0
        31:28   R/W    DUMMY                           4'h0
    */
    union
    {
        __IO uint32_t PERION_REG_PESOC_PERI_CLK_CTRL0;
        struct
        {
            __IO uint32_t r_PON_ACTCKE_UART0: 1;
            __IO uint32_t r_PON_SLPCKE_UART0: 1;
            __IO uint32_t RESERVED_10: 1;
            __IO uint32_t RESERVED_9: 1;
            __IO uint32_t r_CKE_MODEM: 1;
            __IO uint32_t r_CKE_CAL32K: 1;
            __IO uint32_t r_CKE_RNG: 1;
            __IO uint32_t RESERVED_8: 1;
            __IO uint32_t RESERVED_7: 1;
            __IO uint32_t r_CKE_PDCK: 1;
            __IO uint32_t r_CKE_AAC_XTAL: 1;
            __IO uint32_t r_PON_CKE_BT_FREQ_DET: 1;
            __IO uint32_t r_PON_CKE_CAP_TOUCH: 1;
            __IO uint32_t RESERVED_6: 3;
            __IO uint32_t r_PON_ACTCK_SPI0: 1;
            __IO uint32_t r_PON_SLPCK_SPI0: 1;
            __IO uint32_t r_PON_ACTCK_SPI1: 1;
            __IO uint32_t r_PON_SLPCK_SPI1: 1;
            __IO uint32_t r_PON_ACTCK_IRRC: 1;
            __IO uint32_t r_PON_SLPCKE_IRRC: 1;
            __IO uint32_t RESERVED_5: 2;
            __IO uint32_t RESERVED_4: 1;
            __IO uint32_t RESERVED_3: 1;
            __IO uint32_t RESERVED_2: 1;
            __IO uint32_t RESERVED_1: 1;
            __IO uint32_t RESERVED_0: 4;
        } BITS_234;
    } u_234;

    /* 0x0238       0x4000_0238
        0       R/W    r_PON_ACTCKE_I2C0               0
        1       R/W    r_PON_SLPCKE_I2C0               0
        2       R/W    r_PON_ACTCKE_I2C1               0
        3       R/W    r_PON_SLPCKE_I2C1               0
        4       R/W    r_PON_ACTCKE_QDEC               0
        5       R/W    r_PON_SLPCKE_QDEC               0
        6       R/W    r_PON_ACTCKE_KEYSCAN            0
        7       R/W    r_PON_SLPCKE_KEYSCAN            0
        8       R/W    r_PON_ACTCKE_AES                0
        9       R/W    r_PON_SLPCKE_AES                0
        15:10   R/W    DUMMY                           6'h0
        16      R/W    r_PON_ACTCKE_SPI2W              0
        17      R/W    r_PON_SLPCKE_SPI2W              0
        19:18   R/W    DUMMY                           2'h0
        20      R/W    DUMMY                           0
        21      R/W    DUMMY                           0
        22      R/W    DUMMY                           0
        23      R/W    DUMMY                           0
        24      R/W    r_PON_ACTCKE_ADC                0
        25      R/W    r_PON_SLPCKE_ADC                0
        26      R/W    DUMMY                           0
        27      R/W    DUMMY                           0
        28      R/W    DUMMY                           0
        29      R/W    DUMMY                           0
        30      R/W    r_PON_CKE_DSP_WDT               0
        31      R/W    r_PON_CKE_EFUSE                 0
    */
    union
    {
        __IO uint32_t PERION_r_PON_PERI_CLK_CTRL1;
        struct
        {
            __IO uint32_t r_PON_ACTCKE_I2C0: 1;
            __IO uint32_t r_PON_SLPCKE_I2C0: 1;
            __IO uint32_t r_PON_ACTCKE_I2C1: 1;
            __IO uint32_t r_PON_SLPCKE_I2C1: 1;
            __IO uint32_t r_PON_ACTCKE_QDEC: 1;
            __IO uint32_t r_PON_SLPCKE_QDEC: 1;
            __IO uint32_t r_PON_ACTCKE_KEYSCAN: 1;
            __IO uint32_t r_PON_SLPCKE_KEYSCAN: 1;
            __IO uint32_t r_PON_ACTCKE_AES: 1;
            __IO uint32_t r_PON_SLPCKE_AES: 1;
            __IO uint32_t RESERVED_9: 6;
            __IO uint32_t r_PON_ACTCKE_SPI2W: 1;
            __IO uint32_t r_PON_SLPCKE_SPI2W: 1;
            __IO uint32_t RESERVED_8: 2;
            __IO uint32_t RESERVED_7: 1;
            __IO uint32_t RESERVED_6: 1;
            __IO uint32_t RESERVED_5: 1;
            __IO uint32_t RESERVED_4: 1;
            __IO uint32_t r_PON_ACTCKE_ADC: 1;
            __IO uint32_t r_PON_SLPCKE_ADC: 1;
            __IO uint32_t RESERVED_3: 1;
            __IO uint32_t RESERVED_2: 1;
            __IO uint32_t RESERVED_1: 1;
            __IO uint32_t RESERVED_0: 1;
            __IO uint32_t r_PON_CKE_DSP_WDT: 1;
            __IO uint32_t r_PON_CKE_EFUSE: 1;
        } BITS_238;
    } u_238;

    /* 0x023C       0x4000_023c
        31:0    R      RSVD                            32'hEAEAEAEA
    */
    __IO uint32_t PERION_REG_RSVD_23C;

    /* 0x0240       0x4000_0240
        31:0    R      RSVD                            32'hEAEAEAEA
    */
    __IO uint32_t PERION_REG_RSVD_240;

    /* 0x0244       0x4000_0244
        0       R/W    r_PON_ACTCKE_BTBUS              1
        1       R/W    r_PON_SLPCKE_BTBUS              1
        15:2    R/W    DUMMY                           14'h0
        21:16   R/W    DUMMY                           6'h0
        22      R/W    r_LS_mcu_rom                    0
        31:23   R/W    DUMMY                           9'h0
    */
    union
    {
        __IO uint32_t PERION_r_OFF_MEM_PWR_CRTL_r_PON_COM_CLK_CTRL;
        struct
        {
            __IO uint32_t r_PON_ACTCKE_BTBUS: 1;
            __IO uint32_t r_PON_SLPCKE_BTBUS: 1;
            __IO uint32_t RESERVED_2: 14;
            __IO uint32_t RESERVED_1: 6;
            __IO uint32_t r_LS_mcu_rom: 1;
            __IO uint32_t RESERVED_0: 9;
        } BITS_244;
    } u_244;

    /* 0x0248       0x4000_0248
        31:0    R      RSVD                            32'hEAEAEAEA
    */
    __IO uint32_t PERION_REG_RSVD_248;

    /* 0x024C       0x4000_024c
        31:0    R      RSVD                            32'hEAEAEAEA
    */
    __IO uint32_t PERION_REG_RSVD_24C;

    /* 0x0250       0x4000_0250
        31:0    R      RSVD                            32'hEAEAEAEA
    */
    __IO uint32_t PERION_REG_RSVD_250;

    /* 0x0254       0x4000_0254
        7:0     R/W    DUMMY                           8'h1
        13:8    R/W    DUMMY                           6'h0
        14      R/W    DUMMY                           0
        15      R/W    DUMMY                           0
        31:16   R/W    DUMMY                           16'h0
    */
    union
    {
        __IO uint32_t PERION_r_DSP_TOP_CTRL;
        struct
        {
            __IO uint32_t RESERVED_4: 8;
            __IO uint32_t RESERVED_3: 6;
            __IO uint32_t RESERVED_2: 1;
            __IO uint32_t RESERVED_1: 1;
            __IO uint32_t RESERVED_0: 16;
        } BITS_254;
    } u_254;

    /* 0x0258       0x4000_0258
        1:0     RW     CORE_XTAL_LPS_CAP_STEP          2'b01
        3:2     RW     CORE_XTAL_LPS_CAP_CYC           2'b00
        6:4     RW     CORE_XTAL_OV_UNIT               3'b000
        8:7     RW     CORE_XTAL_OV_RATIO              2'b01
        11:9    RW     CORE_XTAL_MODE                  3'b100
        13:12   RW     CORE_XTAL_CTRL1_2t1             2'b00
        31:14   R      RSVD                            18'h0
    */
    union
    {
        __IO uint32_t PERION_CORE_SWR_PAR_TAB_0_1;
        struct
        {
            __IO uint32_t CORE_XTAL_LPS_CAP_STEP: 2;
            __IO uint32_t CORE_XTAL_LPS_CAP_CYC: 2;
            __IO uint32_t CORE_XTAL_OV_UNIT: 3;
            __IO uint32_t CORE_XTAL_OV_RATIO: 2;
            __IO uint32_t CORE_XTAL_MODE: 3;
            __IO uint32_t CORE_XTAL_CTRL1_2t1: 2;
            __I uint32_t RESERVED_0: 18;
        } BITS_258;
    } u_258;

    /* 0x025C       0x4000_025c
        1:0     RW     CORE_XTAL_LPS_CAP_STEP          2'b01
        3:2     RW     CORE_XTAL_LPS_CAP_CYC           2'b00
        6:4     RW     CORE_XTAL_OV_UNIT               3'b000
        8:7     RW     CORE_XTAL_OV_RATIO              2'b01
        11:9    RW     CORE_XTAL_MODE                  3'b100
        13:12   RW     CORE_XTAL_CTRL1_2t1             2'b00
        31:14   R      RSVD                            18'h0
    */
    union
    {
        __IO uint32_t PERION_CORE_SWR_PAR_TAB_4_1;
        struct
        {
            __IO uint32_t CORE_XTAL_LPS_CAP_STEP: 2;
            __IO uint32_t CORE_XTAL_LPS_CAP_CYC: 2;
            __IO uint32_t CORE_XTAL_OV_UNIT: 3;
            __IO uint32_t CORE_XTAL_OV_RATIO: 2;
            __IO uint32_t CORE_XTAL_MODE: 3;
            __IO uint32_t CORE_XTAL_CTRL1_2t1: 2;
            __I uint32_t RESERVED_0: 18;
        } BITS_25C;
    } u_25C;

    /* 0x0260       0x4000_0260
        1:0     RW     CORE_XTAL_LPS_CAP_STEP          2'b01
        3:2     RW     CORE_XTAL_LPS_CAP_CYC           2'b00
        6:4     RW     CORE_XTAL_OV_UNIT               3'b000
        8:7     RW     CORE_XTAL_OV_RATIO              2'b01
        11:9    RW     CORE_XTAL_MODE                  3'b100
        13:12   RW     CORE_XTAL_CTRL1_2t1             2'b00
        31:14   R      RSVD                            18'h0
    */
    union
    {
        __IO uint32_t PERION_CORE_SWR_PAR_TAB_5_1;
        struct
        {
            __IO uint32_t CORE_XTAL_LPS_CAP_STEP: 2;
            __IO uint32_t CORE_XTAL_LPS_CAP_CYC: 2;
            __IO uint32_t CORE_XTAL_OV_UNIT: 3;
            __IO uint32_t CORE_XTAL_OV_RATIO: 2;
            __IO uint32_t CORE_XTAL_MODE: 3;
            __IO uint32_t CORE_XTAL_CTRL1_2t1: 2;
            __I uint32_t RESERVED_0: 18;
        } BITS_260;
    } u_260;

    /* 0x0264       0x4000_0264
        1:0     RW     CORE_XTAL_LPS_CAP_STEP          2'b01
        3:2     RW     CORE_XTAL_LPS_CAP_CYC           2'b00
        6:4     RW     CORE_XTAL_OV_UNIT               3'b000
        8:7     RW     CORE_XTAL_OV_RATIO              2'b01
        11:9    RW     CORE_XTAL_MODE                  3'b100
        13:12   RW     CORE_XTAL_CTRL1_2t1             2'b00
        31:14   R      RSVD                            18'h0
    */
    union
    {
        __IO uint32_t PERION_CORE_SWR_PAR_TAB_6_1;
        struct
        {
            __IO uint32_t CORE_XTAL_LPS_CAP_STEP: 2;
            __IO uint32_t CORE_XTAL_LPS_CAP_CYC: 2;
            __IO uint32_t CORE_XTAL_OV_UNIT: 3;
            __IO uint32_t CORE_XTAL_OV_RATIO: 2;
            __IO uint32_t CORE_XTAL_MODE: 3;
            __IO uint32_t CORE_XTAL_CTRL1_2t1: 2;
            __I uint32_t RESERVED_0: 18;
        } BITS_264;
    } u_264;

    /* 0x0268       0x4000_0268
        1:0     RW     CORE_XTAL_LPS_CAP_STEP          2'b01
        3:2     RW     CORE_XTAL_LPS_CAP_CYC           2'b00
        6:4     RW     CORE_XTAL_OV_UNIT               3'b000
        8:7     RW     CORE_XTAL_OV_RATIO              2'b01
        11:9    RW     CORE_XTAL_MODE                  3'b100
        13:12   RW     CORE_XTAL_CTRL1_2t1             2'b00
        31:14   R      RSVD                            18'h0
    */
    union
    {
        __IO uint32_t PERION_CORE_SWR_PAR_TAB_7_1;
        struct
        {
            __IO uint32_t CORE_XTAL_LPS_CAP_STEP: 2;
            __IO uint32_t CORE_XTAL_LPS_CAP_CYC: 2;
            __IO uint32_t CORE_XTAL_OV_UNIT: 3;
            __IO uint32_t CORE_XTAL_OV_RATIO: 2;
            __IO uint32_t CORE_XTAL_MODE: 3;
            __IO uint32_t CORE_XTAL_CTRL1_2t1: 2;
            __I uint32_t RESERVED_0: 18;
        } BITS_268;
    } u_268;

    /* 0x026C       0x4000_026c
        0       RW     CORE_BUCK_PFM_AOT_EN            1'b0
        1       RW     CORE_BUCK_PFM_COT_EN            1'b1
        2       RW     CORE_BUCK_ENOFFSAWPFM_H         1'b1
        3       RW     CORE_BUCK_POW_EA_Iqx2_H         1'b1
        4       RW     CORE_BUCK_POW_EA_Iqx3_H         1'b1
        5       RW     CORE_BUCK_POW_EA_IqEhance_H     1'b1
        8:6     RW     CORE_BUCK_PFM_STEP_INI          3'b0
        16:9    RW     CORE_AUTO_SET                   8'haa
        22:17   RW     CORE_D300k_REG_MANU_CCOT        6'b010000
        31:23   R      RSVD                            9'b0
    */
    union
    {
        __IO uint32_t PERION_CORE_SWR_PAR_TAB_0_2;
        struct
        {
            __IO uint32_t CORE_BUCK_PFM_AOT_EN: 1;
            __IO uint32_t CORE_BUCK_PFM_COT_EN: 1;
            __IO uint32_t CORE_BUCK_ENOFFSAWPFM_H: 1;
            __IO uint32_t CORE_BUCK_POW_EA_Iqx2_H: 1;
            __IO uint32_t CORE_BUCK_POW_EA_Iqx3_H: 1;
            __IO uint32_t CORE_BUCK_POW_EA_IqEhance_H: 1;
            __IO uint32_t CORE_BUCK_PFM_STEP_INI: 3;
            __IO uint32_t CORE_AUTO_SET: 8;
            __IO uint32_t CORE_D300k_REG_MANU_CCOT: 6;
            __I uint32_t RESERVED_0: 9;
        } BITS_26C;
    } u_26C;

    /* 0x0270       0x4000_0270
        0       RW     CORE_BUCK_PFM_AOT_EN            1'b0
        1       RW     CORE_BUCK_PFM_COT_EN            1'b1
        2       RW     CORE_BUCK_ENOFFSAWPFM_H         1'b1
        3       RW     CORE_BUCK_POW_EA_Iqx2_H         1'b1
        4       RW     CORE_BUCK_POW_EA_Iqx3_H         1'b1
        5       RW     CORE_BUCK_POW_EA_IqEhance_H     1'b1
        8:6     RW     CORE_BUCK_PFM_STEP_INI          3'b0
        16:9    RW     CORE_AUTO_SET                   8'haa
        22:17   RW     CORE_D300k_REG_MANU_CCOT        6'b010000
        31:23   R      RSVD                            9'b0
    */
    union
    {
        __IO uint32_t PERION_CORE_SWR_PAR_TAB_4_2;
        struct
        {
            __IO uint32_t CORE_BUCK_PFM_AOT_EN: 1;
            __IO uint32_t CORE_BUCK_PFM_COT_EN: 1;
            __IO uint32_t CORE_BUCK_ENOFFSAWPFM_H: 1;
            __IO uint32_t CORE_BUCK_POW_EA_Iqx2_H: 1;
            __IO uint32_t CORE_BUCK_POW_EA_Iqx3_H: 1;
            __IO uint32_t CORE_BUCK_POW_EA_IqEhance_H: 1;
            __IO uint32_t CORE_BUCK_PFM_STEP_INI: 3;
            __IO uint32_t CORE_AUTO_SET: 8;
            __IO uint32_t CORE_D300k_REG_MANU_CCOT: 6;
            __I uint32_t RESERVED_0: 9;
        } BITS_270;
    } u_270;

    /* 0x0274       0x4000_0274
        0       RW     CORE_BUCK_PFM_AOT_EN            1'b0
        1       RW     CORE_BUCK_PFM_COT_EN            1'b1
        2       RW     CORE_BUCK_ENOFFSAWPFM_H         1'b1
        3       RW     CORE_BUCK_POW_EA_Iqx2_H         1'b1
        4       RW     CORE_BUCK_POW_EA_Iqx3_H         1'b1
        5       RW     CORE_BUCK_POW_EA_IqEhance_H     1'b1
        8:6     RW     CORE_BUCK_PFM_STEP_INI          3'b0
        16:9    RW     CORE_AUTO_SET                   8'haa
        22:17   RW     CORE_D300k_REG_MANU_CCOT        6'b010000
        31:23   R      RSVD                            9'b0
    */
    union
    {
        __IO uint32_t PERION_CORE_SWR_PAR_TAB_5_2;
        struct
        {
            __IO uint32_t CORE_BUCK_PFM_AOT_EN: 1;
            __IO uint32_t CORE_BUCK_PFM_COT_EN: 1;
            __IO uint32_t CORE_BUCK_ENOFFSAWPFM_H: 1;
            __IO uint32_t CORE_BUCK_POW_EA_Iqx2_H: 1;
            __IO uint32_t CORE_BUCK_POW_EA_Iqx3_H: 1;
            __IO uint32_t CORE_BUCK_POW_EA_IqEhance_H: 1;
            __IO uint32_t CORE_BUCK_PFM_STEP_INI: 3;
            __IO uint32_t CORE_AUTO_SET: 8;
            __IO uint32_t CORE_D300k_REG_MANU_CCOT: 6;
            __I uint32_t RESERVED_0: 9;
        } BITS_274;
    } u_274;

    /* 0x0278       0x4000_0278
        0       RW     CORE_BUCK_PFM_AOT_EN            1'b0
        1       RW     CORE_BUCK_PFM_COT_EN            1'b1
        2       RW     CORE_BUCK_ENOFFSAWPFM_H         1'b1
        3       RW     CORE_BUCK_POW_EA_Iqx2_H         1'b1
        4       RW     CORE_BUCK_POW_EA_Iqx3_H         1'b1
        5       RW     CORE_BUCK_POW_EA_IqEhance_H     1'b1
        8:6     RW     CORE_BUCK_PFM_STEP_INI          3'b0
        16:9    RW     CORE_AUTO_SET                   8'haa
        22:17   RW     CORE_D300k_REG_MANU_CCOT        6'b010000
        31:23   R      RSVD                            9'b0
    */
    union
    {
        __IO uint32_t PERION_CORE_SWR_PAR_TAB_6_2;
        struct
        {
            __IO uint32_t CORE_BUCK_PFM_AOT_EN: 1;
            __IO uint32_t CORE_BUCK_PFM_COT_EN: 1;
            __IO uint32_t CORE_BUCK_ENOFFSAWPFM_H: 1;
            __IO uint32_t CORE_BUCK_POW_EA_Iqx2_H: 1;
            __IO uint32_t CORE_BUCK_POW_EA_Iqx3_H: 1;
            __IO uint32_t CORE_BUCK_POW_EA_IqEhance_H: 1;
            __IO uint32_t CORE_BUCK_PFM_STEP_INI: 3;
            __IO uint32_t CORE_AUTO_SET: 8;
            __IO uint32_t CORE_D300k_REG_MANU_CCOT: 6;
            __I uint32_t RESERVED_0: 9;
        } BITS_278;
    } u_278;

    /* 0x027C       0x4000_027c
        0       RW     CORE_BUCK_PFM_AOT_EN            1'b0
        1       RW     CORE_BUCK_PFM_COT_EN            1'b1
        2       RW     CORE_BUCK_ENOFFSAWPFM_H         1'b1
        3       RW     CORE_BUCK_POW_EA_Iqx2_H         1'b1
        4       RW     CORE_BUCK_POW_EA_Iqx3_H         1'b1
        5       RW     CORE_BUCK_POW_EA_IqEhance_H     1'b1
        8:6     RW     CORE_BUCK_PFM_STEP_INI          3'b0
        16:9    RW     CORE_AUTO_SET                   8'haa
        22:17   RW     CORE_D300k_REG_MANU_CCOT        6'b010000
        31:23   R      RSVD                            9'b0
    */
    union
    {
        __IO uint32_t PERION_CORE_SWR_PAR_TAB_7_2;
        struct
        {
            __IO uint32_t CORE_BUCK_PFM_AOT_EN: 1;
            __IO uint32_t CORE_BUCK_PFM_COT_EN: 1;
            __IO uint32_t CORE_BUCK_ENOFFSAWPFM_H: 1;
            __IO uint32_t CORE_BUCK_POW_EA_Iqx2_H: 1;
            __IO uint32_t CORE_BUCK_POW_EA_Iqx3_H: 1;
            __IO uint32_t CORE_BUCK_POW_EA_IqEhance_H: 1;
            __IO uint32_t CORE_BUCK_PFM_STEP_INI: 3;
            __IO uint32_t CORE_AUTO_SET: 8;
            __IO uint32_t CORE_D300k_REG_MANU_CCOT: 6;
            __I uint32_t RESERVED_0: 9;
        } BITS_27C;
    } u_27C;

    /* 0x0280       0x4000_0280
        6:0     R/W    r_PMUX_GPIO_P0_0                7'h0
        7       R/W    DUMMY                           0
        14:8    R/W    r_PMUX_GPIO_P0_1                7'h0
        15      R/W    DUMMY                           0
        22:16   R/W    r_PMUX_GPIO_P0_2                7'h0
        23      R/W    DUMMY                           0
        30:24   R/W    r_PMUX_GPIO_P0_3                7'h1D
        31      R/W    DUMMY                           0
    */
    union
    {
        __IO uint32_t PERION_r_PMUX_GPIO_P0_0_3;
        struct
        {
            __IO uint32_t r_PMUX_GPIO_P0_0: 7;
            __IO uint32_t RESERVED_3: 1;
            __IO uint32_t r_PMUX_GPIO_P0_1: 7;
            __IO uint32_t RESERVED_2: 1;
            __IO uint32_t r_PMUX_GPIO_P0_2: 7;
            __IO uint32_t RESERVED_1: 1;
            __IO uint32_t r_PMUX_GPIO_P0_3: 7;
            __IO uint32_t RESERVED_0: 1;
        } BITS_280;
    } u_280;

    /* 0x0284       0x4000_0284
        6:0     R/W    r_PMUX_GPIO_P0_4                7'h0
        7       R/W    DUMMY                           0
        14:8    R/W    r_PMUX_GPIO_P0_5                7'h0
        15      R/W    DUMMY                           0
        22:16   R/W    r_PMUX_GPIO_P0_6                7'h0
        23      R/W    DUMMY                           0
        30:24   R/W    r_PMUX_GPIO_P0_7                7'h0
        31      R/W    DUMMY                           0
    */
    union
    {
        __IO uint32_t PERION_r_PMUX_GPIO_P0_4_7;
        struct
        {
            __IO uint32_t r_PMUX_GPIO_P0_4: 7;
            __IO uint32_t RESERVED_3: 1;
            __IO uint32_t r_PMUX_GPIO_P0_5: 7;
            __IO uint32_t RESERVED_2: 1;
            __IO uint32_t r_PMUX_GPIO_P0_6: 7;
            __IO uint32_t RESERVED_1: 1;
            __IO uint32_t r_PMUX_GPIO_P0_7: 7;
            __IO uint32_t RESERVED_0: 1;
        } BITS_284;
    } u_284;

    /* 0x0288       0x4000_0288
        6:0     R/W    r_PMUX_GPIO_P1_0                7'h39
        7       R/W    DUMMY                           0
        14:8    R/W    r_PMUX_GPIO_P1_1                7'h38
        15      R/W    DUMMY                           0
        22:16   R/W    r_PMUX_GPIO_P1_2                7'h0
        23      R/W    DUMMY                           0
        30:24   R/W    r_PMUX_GPIO_P1_3                7'h0
        31      R/W    DUMMY                           0
    */
    union
    {
        __IO uint32_t PERION_r_PMUX_GPIO_P1_0_3;
        struct
        {
            __IO uint32_t r_PMUX_GPIO_P1_0: 7;
            __IO uint32_t RESERVED_3: 1;
            __IO uint32_t r_PMUX_GPIO_P1_1: 7;
            __IO uint32_t RESERVED_2: 1;
            __IO uint32_t r_PMUX_GPIO_P1_2: 7;
            __IO uint32_t RESERVED_1: 1;
            __IO uint32_t r_PMUX_GPIO_P1_3: 7;
            __IO uint32_t RESERVED_0: 1;
        } BITS_288;
    } u_288;

    /* 0x028C       0x4000_028c
        6:0     R/W    r_PMUX_GPIO_P1_4                7'h0
        7       R/W    DUMMY                           0
        14:8    R/W    r_PMUX_GPIO_P1_5                7'h0
        15      R/W    DUMMY                           0
        22:16   R/W    r_PMUX_GPIO_P1_6                7'h0
        23      R/W    DUMMY                           0
        30:24   R/W    r_PMUX_GPIO_P1_7                7'h0
        31      R/W    DUMMY                           0
    */
    union
    {
        __IO uint32_t PERION_r_PMUX_GPIO_P1_4_7;
        struct
        {
            __IO uint32_t r_PMUX_GPIO_P1_4: 7;
            __IO uint32_t RESERVED_3: 1;
            __IO uint32_t r_PMUX_GPIO_P1_5: 7;
            __IO uint32_t RESERVED_2: 1;
            __IO uint32_t r_PMUX_GPIO_P1_6: 7;
            __IO uint32_t RESERVED_1: 1;
            __IO uint32_t r_PMUX_GPIO_P1_7: 7;
            __IO uint32_t RESERVED_0: 1;
        } BITS_28C;
    } u_28C;

    /* 0x0290       0x4000_0290
        6:0     R/W    r_PMUX_GPIO_P2_0                7'h0
        7       R/W    DUMMY                           0
        14:8    R/W    r_PMUX_GPIO_P2_1                7'h0
        15      R/W    DUMMY                           0
        22:16   R/W    r_PMUX_GPIO_P2_2                7'h0
        23      R/W    DUMMY                           0
        30:24   R/W    r_PMUX_GPIO_P2_3                7'h0
        31      R/W    DUMMY                           0
    */
    union
    {
        __IO uint32_t PERION_r_PMUX_GPIO_P2_0_3;
        struct
        {
            __IO uint32_t r_PMUX_GPIO_P2_0: 7;
            __IO uint32_t RESERVED_3: 1;
            __IO uint32_t r_PMUX_GPIO_P2_1: 7;
            __IO uint32_t RESERVED_2: 1;
            __IO uint32_t r_PMUX_GPIO_P2_2: 7;
            __IO uint32_t RESERVED_1: 1;
            __IO uint32_t r_PMUX_GPIO_P2_3: 7;
            __IO uint32_t RESERVED_0: 1;
        } BITS_290;
    } u_290;

    /* 0x0294       0x4000_0294
        6:0     R/W    r_PMUX_GPIO_P2_4                7'h0
        7       R/W    DUMMY                           0
        14:8    R/W    r_PMUX_GPIO_P2_5                7'h0
        15      R/W    DUMMY                           0
        22:16   R/W    r_PMUX_GPIO_P2_6                7'h0
        23      R/W    DUMMY                           0
        30:24   R/W    r_PMUX_GPIO_P2_7                7'h0
        31      R/W    DUMMY                           0
    */
    union
    {
        __IO uint32_t PERION_r_PMUX_GPIO_P2_4_7;
        struct
        {
            __IO uint32_t r_PMUX_GPIO_P2_4: 7;
            __IO uint32_t RESERVED_3: 1;
            __IO uint32_t r_PMUX_GPIO_P2_5: 7;
            __IO uint32_t RESERVED_2: 1;
            __IO uint32_t r_PMUX_GPIO_P2_6: 7;
            __IO uint32_t RESERVED_1: 1;
            __IO uint32_t r_PMUX_GPIO_P2_7: 7;
            __IO uint32_t RESERVED_0: 1;
        } BITS_294;
    } u_294;

    /* 0x0298       0x4000_0298
        6:0     R/W    r_PMUX_GPIO_P3_0                7'h23
        7       R/W    DUMMY                           0
        14:8    R/W    r_PMUX_GPIO_P3_1                7'h24
        15      R/W    DUMMY                           0
        22:16   R/W    r_PMUX_GPIO_P3_2                7'h0
        23      R/W    DUMMY                           0
        30:24   R/W    r_PMUX_GPIO_P3_3                7'h0
        31      R/W    DUMMY                           0
    */
    union
    {
        __IO uint32_t PERION_r_PMUX_GPIO_P3_0_3;
        struct
        {
            __IO uint32_t r_PMUX_GPIO_P3_0: 7;
            __IO uint32_t RESERVED_3: 1;
            __IO uint32_t r_PMUX_GPIO_P3_1: 7;
            __IO uint32_t RESERVED_2: 1;
            __IO uint32_t r_PMUX_GPIO_P3_2: 7;
            __IO uint32_t RESERVED_1: 1;
            __IO uint32_t r_PMUX_GPIO_P3_3: 7;
            __IO uint32_t RESERVED_0: 1;
        } BITS_298;
    } u_298;

    /* 0x029C       0x4000_029c
        6:0     R/W    r_PMUX_GPIO_P3_4                7'h0
        7       R/W    DUMMY                           0
        14:8    R/W    r_PMUX_GPIO_P3_5                7'h0
        15      R/W    DUMMY                           0
        22:16   R/W    r_PMUX_GPIO_P3_6                7'h0
        23      R/W    DUMMY                           0
        30:24   R/W    r_PMUX_GPIO_P3_7                7'h0
        31      R/W    DUMMY                           0
    */
    union
    {
        __IO uint32_t PERION_r_PMUX_GPIO_P3_4_7;
        struct
        {
            __IO uint32_t r_PMUX_GPIO_P3_4: 7;
            __IO uint32_t RESERVED_3: 1;
            __IO uint32_t r_PMUX_GPIO_P3_5: 7;
            __IO uint32_t RESERVED_2: 1;
            __IO uint32_t r_PMUX_GPIO_P3_6: 7;
            __IO uint32_t RESERVED_1: 1;
            __IO uint32_t r_PMUX_GPIO_P3_7: 7;
            __IO uint32_t RESERVED_0: 1;
        } BITS_29C;
    } u_29C;

    /* 0x02A0       0x4000_02a0
        6:0     R/W    r_PMUX_GPIO_P4_0                7'h2e
        7       R/W    DUMMY                           0
        14:8    R/W    r_PMUX_GPIO_P4_1                7'h2f
        15      R/W    DUMMY                           0
        22:16   R/W    r_PMUX_GPIO_P4_2                7'h30
        23      R/W    DUMMY                           0
        30:24   R/W    r_PMUX_GPIO_P4_3                7'h2d
        31      R/W    DUMMY                           0
    */
    union
    {
        __IO uint32_t PERION_r_PMUX_GPIO_P4_0_3;
        struct
        {
            __IO uint32_t r_PMUX_GPIO_P4_0: 7;
            __IO uint32_t RESERVED_3: 1;
            __IO uint32_t r_PMUX_GPIO_P4_1: 7;
            __IO uint32_t RESERVED_2: 1;
            __IO uint32_t r_PMUX_GPIO_P4_2: 7;
            __IO uint32_t RESERVED_1: 1;
            __IO uint32_t r_PMUX_GPIO_P4_3: 7;
            __IO uint32_t RESERVED_0: 1;
        } BITS_2A0;
    } u_2A0;

    /* 0x02A4       0x4000_02a4
        6:0     R/W    r_PMUX_GPIO_H_0                 7'h0
        7       R/W    DUMMY                           0
        14:8    R/W    r_PMUX_GPIO_H_1                 7'h0
        15      R/W    DUMMY                           0
        22:16   R/W    r_PMUX_GPIO_H_2                 7'h0
        31:23   R/W    DUMMY                           9'h0
    */
    union
    {
        __IO uint32_t PERION_r_PMUX_GPIO_H_0_2;
        struct
        {
            __IO uint32_t r_PMUX_GPIO_H_0: 7;
            __IO uint32_t RESERVED_2: 1;
            __IO uint32_t r_PMUX_GPIO_H_1: 7;
            __IO uint32_t RESERVED_1: 1;
            __IO uint32_t r_PMUX_GPIO_H_2: 7;
            __IO uint32_t RESERVED_0: 9;
        } BITS_2A4;
    } u_2A4;

    /* 0x02A8       0x4000_02a8
        3:0     R/W    r_PMUX_TEST_MODE                4'h1
        6:4     R/W    DUMMY                           0
        7       R/W    r_PMUX_TEST_MODE_EN             0
        8       R/W    DUMMY                           0
        9       R/W    DUMMY                           0
        11:10   R/W    r_PMUX_1_WIRE_UART_EN           2'h0
        15:12   R/W    DUMMY                           6'h0
        19:16   R/W    r_PMUX_DBG_MODE_SEL             4'h2
        20      R/W    DUMMY                           0
        21      R/W    DUMMY                           0
        22      R/W    DUMMY                           0
        23      R/W    r_PMUX_DIG_SMUX_EN              0
        24      R/W    r_PON_PMUX_SPIM0_EN             0
        25      R/W    r_PMUX_SPIM_EN_OPT              0
        26      R/W    r_PMUX_FLASH_EXTC_EN            0
        27      R/W    r_PMUX_FLASH_EN                 0
        28      R/W    DUMMY                           0
        29      R/W    DUMMY                           0
        30      R/W    DUMMY                           0
        31      R/W    DUMMY                           0
    */
    union
    {
        __IO uint32_t PERION_r_PMUX_TEST_MODE;
        struct
        {
            __IO uint32_t r_PMUX_TEST_MODE: 4;
            __IO uint32_t RESERVED_10: 3;
            __IO uint32_t r_PMUX_TEST_MODE_EN: 1;
            __IO uint32_t RESERVED_9: 1;
            __IO uint32_t RESERVED_8: 1;
            __IO uint32_t r_PMUX_1_WIRE_UART_EN: 2;
            __IO uint32_t RESERVED_7: 4;
            __IO uint32_t r_PMUX_DBG_MODE_SEL: 4;
            __IO uint32_t RESERVED_6: 1;
            __IO uint32_t RESERVED_5: 1;
            __IO uint32_t RESERVED_4: 1;
            __IO uint32_t r_PMUX_DIG_SMUX_EN: 1;
            __IO uint32_t r_PON_PMUX_SPIM0_EN: 1;
            __IO uint32_t r_PMUX_SPIM_EN_OPT: 1;
            __IO uint32_t r_PMUX_FLASH_EXTC_EN: 1;
            __IO uint32_t r_PMUX_FLASH_EN: 1;
            __IO uint32_t RESERVED_3: 1;
            __IO uint32_t RESERVED_2: 1;
            __IO uint32_t RESERVED_1: 1;
            __IO uint32_t RESERVED_0: 1;
        } BITS_2A8;
    } u_2A8;

    /* 0x02AC       0x4000_02ac
        31:0    R/W    DUMMY                           32'h0
    */
    __IO uint32_t PERION_REG_DUMMY_2AC;

    /* 0x02B0       0x4000_02b0
        31:0    R      RSVD                            32'hEAEAEAEA
    */
    __IO uint32_t PERION_REG_RSVD_2B0;

    /* 0x02B4       0x4000_02b4
        31:0    R/W    DUMMY                           32'h0
    */
    __IO uint32_t PERION_REG_DUMMY_2B4;

    /* 0x02B8       0x4000_02b8
        31:0    R/W    DUMMY                           32'h0
    */
    __IO uint32_t PERION_REG_DUMMY_2B8;

    /* 0x02BC       0x4000_02bc
        31:0    R      RSVD                            32'hEAEAEAEA
    */
    __IO uint32_t PERION_REG_RSVD_2BC;

    /* 0x02C0       0x4000_02c0
        31:0    R      RSVD                            32'hEAEAEAEA
    */
    __IO uint32_t PERION_REG_RSVD_2C0;

    /* 0x02C4       0x4000_02c4
        31:0    R      RSVD                            32'hEAEAEAEA
    */
    __IO uint32_t PERION_REG_RSVD_2C4;

    /* 0x02C8       0x4000_02c8
        31:0    R      RSVD                            32'hEAEAEAEA
    */
    __IO uint32_t PERION_REG_RSVD_2C8;

    /* 0x02CC       0x4000_02cc
        31:0    R      RSVD                            32'hEAEAEAEA
    */
    __IO uint32_t PERION_REG_RSVD_2CC;

    /* 0x02D0       0x4000_02d0
        1:0     R/W    DUMMY                           2'h0
        3:2     R/W    DUMMY                           2'h0
        4       R/W    DUMMY                           0
        5       R/W    DUMMY                           0
        6       R/W    DUMMY                           0
        7       R/W    DUMMY                           0
        8       R/W    DUMMY                           0
        31:9    R/W    DUMMY                           23'h0
    */
    union
    {
        __IO uint32_t PERION_REG_RSVD_2D0;
        struct
        {
            __IO uint32_t RESERVED_7: 2;
            __IO uint32_t RESERVED_6: 2;
            __IO uint32_t RESERVED_5: 1;
            __IO uint32_t RESERVED_4: 1;
            __IO uint32_t RESERVED_3: 1;
            __IO uint32_t RESERVED_2: 1;
            __IO uint32_t RESERVED_1: 1;
            __IO uint32_t RESERVED_0: 23;
        } BITS_2D0;
    } u_2D0;

    /* 0x02D4       0x4000_02d4
        31:0    R      RSVD                            32'hEAEAEAEA
    */
    __IO uint32_t PERION_REG_RSVD_2D4;

    /* 0x02D8       0x4000_02d8
        31:0    R      RSVD                            32'hEAEAEAEA
    */
    __IO uint32_t PERION_REG_RSVD_2D8;

    /* 0x02DC       0x4000_02dc
        31:0    R      RSVD                            32'hEAEAEAEA
    */
    __IO uint32_t PERION_REG_RSVD_2DC;

    /* 0x02E0       0x4000_02e0
        7:0     R/W    DUMMY                           8'hC0
        18:8    R      RSVD                            11'h0
        31:19   R/W    DUMMY                           13'h0
    */
    union
    {
        __IO uint32_t PERION_REG_RSVD_2E0;
        struct
        {
            __IO uint32_t RESERVED_2: 8;
            __I uint32_t RESERVED_1: 11;
            __IO uint32_t RESERVED_0: 13;
        } BITS_2E0;
    } u_2E0;

    /* 0x02E4       0x4000_02e4
        15:0    R/W    DUMMY                           16'h0
        25:16   R      RSVD                            10'h0
        31:26   R/W    DUMMY                           6'h0
    */
    union
    {
        __IO uint32_t PERION_REG_RSVD_2E4;
        struct
        {
            __IO uint32_t RESERVED_2: 16;
            __I uint32_t RESERVED_1: 10;
            __IO uint32_t RESERVED_0: 6;
        } BITS_2E4;
    } u_2E4;

    /* 0x02E8       0x4000_02e8
        0       R/W    CORE_BUCK_POW_EA_Initial        1'b1
        1       R/W    CORE_BUCK_FPWM_H                1'b1
        2       R/W    CORE_BG_ENPC_DIGI               1'b0
        7:3     R/W    CORE_BG_VDIGI_TUNE              5'b10111
        8       R/W    DUMMY                           1'b0
        9       R/W    CORE_BUCK_PMOS_DRIH_H           1'b1
        10      R/W    CORE_BUCK_PMOS_DRIL_H           1'b1
        12:11   R/W    CORE_BUCK_OCPRES_H              2'b01
        17:13   R/W    DUMMY                           0
        20:18   R/W    CORE_BUCK_VREFOCP_H             3'b001
        26:21   R/W    CORE_BUCK_SWRTUNE_H             6'b011101
        30:27   R/W    CORE_BUCK_VREFPFM_H             4'b1011
        31      R/W    DUMMY                           0
    */
    union
    {
        __IO uint32_t PERION_CORE_SWR_PAR_TAB_0_0;
        struct
        {
            __IO uint32_t CORE_BUCK_POW_EA_Initial: 1;
            __IO uint32_t CORE_BUCK_FPWM_H: 1;
            __IO uint32_t CORE_BG_ENPC_DIGI: 1;
            __IO uint32_t CORE_BG_VDIGI_TUNE: 5;
            __IO uint32_t RESERVED_2: 1;
            __IO uint32_t CORE_BUCK_PMOS_DRIH_H: 1;
            __IO uint32_t CORE_BUCK_PMOS_DRIL_H: 1;
            __IO uint32_t CORE_BUCK_OCPRES_H: 2;
            __IO uint32_t RESERVED_1: 5;
            __IO uint32_t CORE_BUCK_VREFOCP_H: 3;
            __IO uint32_t CORE_BUCK_SWRTUNE_H: 6;
            __IO uint32_t CORE_BUCK_VREFPFM_H: 4;
            __IO uint32_t RESERVED_0: 1;
        } BITS_2E8;
    } u_2E8;

    /* 0x02EC       0x4000_02ec
        0       R/W    CORE_BUCK_POW_EA_Initial        1'b1
        1       R/W    CORE_BUCK_FPWM_H                1'b1
        2       R/W    CORE_BG_ENPC_DIGI               1'b0
        7:3     R/W    CORE_BG_VDIGI_TUNE              5'b10111
        8       R/W    DUMMY                           1'b0
        9       R/W    CORE_BUCK_PMOS_DRIH_H           1'b1
        10      R/W    CORE_BUCK_PMOS_DRIL_H           1'b1
        12:11   R/W    CORE_BUCK_OCPRES_H              2'b01
        17:13   R/W    dummy                           0
        20:18   R/W    CORE_BUCK_VREFOCP_H             3'b001
        26:21   R/W    CORE_BUCK_SWRTUNE_H             6'b011101
        30:27   R/W    CORE_BUCK_VREFPFM_H             4'b1011
        31      R/W    DUMMY                           0
    */
    union
    {
        __IO uint32_t PERION_CORE_SWR_PAR_TAB_4_0;
        struct
        {
            __IO uint32_t CORE_BUCK_POW_EA_Initial: 1;
            __IO uint32_t CORE_BUCK_FPWM_H: 1;
            __IO uint32_t CORE_BG_ENPC_DIGI: 1;
            __IO uint32_t CORE_BG_VDIGI_TUNE: 5;
            __IO uint32_t RESERVED_2: 1;
            __IO uint32_t CORE_BUCK_PMOS_DRIH_H: 1;
            __IO uint32_t CORE_BUCK_PMOS_DRIL_H: 1;
            __IO uint32_t CORE_BUCK_OCPRES_H: 2;
            __IO uint32_t RESERVED_1: 5;
            __IO uint32_t CORE_BUCK_VREFOCP_H: 3;
            __IO uint32_t CORE_BUCK_SWRTUNE_H: 6;
            __IO uint32_t CORE_BUCK_VREFPFM_H: 4;
            __IO uint32_t RESERVED_0: 1;
        } BITS_2EC;
    } u_2EC;

    /* 0x02F0       0x4000_02f0
        0       R/W    CORE_BUCK_POW_EA_Initial        1'b1
        1       R/W    CORE_BUCK_FPWM_H                1'b1
        2       R/W    CORE_BG_ENPC_DIGI               1'b0
        7:3     R/W    CORE_BG_VDIGI_TUNE              5'b10111
        8       R/W    DUMMY                           1'b0
        9       R/W    CORE_BUCK_PMOS_DRIH_H           1'b1
        10      R/W    CORE_BUCK_PMOS_DRIL_H           1'b1
        12:11   R/W    CORE_BUCK_OCPRES_H              2'b01
        17:13   R/W    dummy                           0
        20:18   R/W    CORE_BUCK_VREFOCP_H             3'b001
        26:21   R/W    CORE_BUCK_SWRTUNE_H             6'b011101
        30:27   R/W    CORE_BUCK_VREFPFM_H             4'b1011
        31      R/W    DUMMY                           0
    */
    union
    {
        __IO uint32_t PERION_CORE_SWR_PAR_TAB_5_0;
        struct
        {
            __IO uint32_t CORE_BUCK_POW_EA_Initial: 1;
            __IO uint32_t CORE_BUCK_FPWM_H: 1;
            __IO uint32_t CORE_BG_ENPC_DIGI: 1;
            __IO uint32_t CORE_BG_VDIGI_TUNE: 5;
            __IO uint32_t RESERVED_2: 1;
            __IO uint32_t CORE_BUCK_PMOS_DRIH_H: 1;
            __IO uint32_t CORE_BUCK_PMOS_DRIL_H: 1;
            __IO uint32_t CORE_BUCK_OCPRES_H: 2;
            __IO uint32_t RESERVED_1: 5;
            __IO uint32_t CORE_BUCK_VREFOCP_H: 3;
            __IO uint32_t CORE_BUCK_SWRTUNE_H: 6;
            __IO uint32_t CORE_BUCK_VREFPFM_H: 4;
            __IO uint32_t RESERVED_0: 1;
        } BITS_2F0;
    } u_2F0;

    /* 0x02F4       0x4000_02f4
        0       R/W    CORE_BUCK_POW_EA_Initial        1'b1
        1       R/W    CORE_BUCK_FPWM_H                1'b1
        2       R/W    CORE_BG_ENPC_DIGI               1'b0
        7:3     R/W    CORE_BG_VDIGI_TUNE              5'b10111
        8       R/W    DUMMY                           1'b0
        9       R/W    CORE_BUCK_PMOS_DRIH_H           1'b1
        10      R/W    CORE_BUCK_PMOS_DRIL_H           1'b1
        12:11   R/W    CORE_BUCK_OCPRES_H              2'b01
        17:13   R/W    dummy                           0
        20:18   R/W    CORE_BUCK_VREFOCP_H             3'b001
        26:21   R/W    CORE_BUCK_SWRTUNE_H             6'b011101
        30:27   R/W    CORE_BUCK_VREFPFM_H             4'b1011
        31      R/W    DUMMY                           0
    */
    union
    {
        __IO uint32_t PERION_CORE_SWR_PAR_TAB_6_0;
        struct
        {
            __IO uint32_t CORE_BUCK_POW_EA_Initial: 1;
            __IO uint32_t CORE_BUCK_FPWM_H: 1;
            __IO uint32_t CORE_BG_ENPC_DIGI: 1;
            __IO uint32_t CORE_BG_VDIGI_TUNE: 5;
            __IO uint32_t RESERVED_2: 1;
            __IO uint32_t CORE_BUCK_PMOS_DRIH_H: 1;
            __IO uint32_t CORE_BUCK_PMOS_DRIL_H: 1;
            __IO uint32_t CORE_BUCK_OCPRES_H: 2;
            __IO uint32_t RESERVED_1: 5;
            __IO uint32_t CORE_BUCK_VREFOCP_H: 3;
            __IO uint32_t CORE_BUCK_SWRTUNE_H: 6;
            __IO uint32_t CORE_BUCK_VREFPFM_H: 4;
            __IO uint32_t RESERVED_0: 1;
        } BITS_2F4;
    } u_2F4;

    /* 0x02F8       0x4000_02f8
        0       R/W    CORE_BUCK_POW_EA_Initial        1'b1
        1       R/W    CORE_BUCK_FPWM_H                1'b1
        2       R/W    CORE_BG_ENPC_DIGI               1'b0
        7:3     R/W    CORE_BG_VDIGI_TUNE              5'b10111
        8       R/W    DUMMY                           1'b0
        9       R/W    CORE_BUCK_PMOS_DRIH_H           1'b1
        10      R/W    CORE_BUCK_PMOS_DRIL_H           1'b1
        12:11   R/W    CORE_BUCK_OCPRES_H              2'b01
        17:13   R/W    dummy                           5'h0
        20:18   R/W    CORE_BUCK_VREFOCP_H             3'b001
        26:21   R/W    CORE_BUCK_SWRTUNE_H             6'b011101
        30:27   R/W    CORE_BUCK_VREFPFM_H             4'b1011
        31      R/W    DUMMY                           0
    */
    union
    {
        __IO uint32_t PERION_CORE_SWR_PAR_TAB_7_0;
        struct
        {
            __IO uint32_t CORE_BUCK_POW_EA_Initial: 1;
            __IO uint32_t CORE_BUCK_FPWM_H: 1;
            __IO uint32_t CORE_BG_ENPC_DIGI: 1;
            __IO uint32_t CORE_BG_VDIGI_TUNE: 5;
            __IO uint32_t RESERVED_2: 1;
            __IO uint32_t CORE_BUCK_PMOS_DRIH_H: 1;
            __IO uint32_t CORE_BUCK_PMOS_DRIL_H: 1;
            __IO uint32_t CORE_BUCK_OCPRES_H: 2;
            __IO uint32_t RESERVED_1: 5;
            __IO uint32_t CORE_BUCK_VREFOCP_H: 3;
            __IO uint32_t CORE_BUCK_SWRTUNE_H: 6;
            __IO uint32_t CORE_BUCK_VREFPFM_H: 4;
            __IO uint32_t RESERVED_0: 1;
        } BITS_2F8;
    } u_2F8;

    /* 0x02FC       0x4000_02fc
        5:0     R/W    CORE_SWR_PAR_TAB_0[36:31]       6'b1
        11:6    R/W    CORE_SWR_PAR_TAB_4[36:31]       6'b1
        17:12   R/W    CORE_SWR_PAR_TAB_5[36:31]       6'b1
        23:18   R/W    CORE_SWR_PAR_TAB_6[36:31]       6'b1
        29:24   R/W    CORE_SWR_PAR_TAB_7[36:31]       6'b1
        31:30   R/W    DUMMY                           2'h0
    */
    union
    {
        __IO uint32_t PERION_CORE_SWR_PAR_TAB_ext;
        struct
        {
            __IO uint32_t CORE_SWR_PAR_TAB_0_36_31: 6;
            __IO uint32_t CORE_SWR_PAR_TAB_4_36_31: 6;
            __IO uint32_t CORE_SWR_PAR_TAB_5_36_31: 6;
            __IO uint32_t CORE_SWR_PAR_TAB_6_36_31: 6;
            __IO uint32_t CORE_SWR_PAR_TAB_7_36_31: 6;
            __IO uint32_t RESERVED_0: 2;
        } BITS_2FC;
    } u_2FC;

    /* 0x0300       0x4000_0300
        7:0     R/W    PON_PERI_DLYSEL_CTRL            8'h0
        15:8    R/W    DUMMY                           8'h0
        23:16   R/W    DUMMY                           8'h0
        24      R/W    r_flash0_clk_inv_en             0
        25      R/W    DUMMY                           0
        26      R/W    DUMMY                           0
        30:27   R/W    DUMMY                           3'h0
        31      R/W    r_spic2_dma_tx_req_as_single    0
    */
    union
    {
        __IO uint32_t PERION_PON_PERI_DLYSEL_CTRL;
        struct
        {
            __IO uint32_t PON_PERI_DLYSEL_CTRL: 8;
            __IO uint32_t RESERVED_4: 8;
            __IO uint32_t RESERVED_3: 8;
            __IO uint32_t r_flash0_clk_inv_en: 1;
            __IO uint32_t RESERVED_2: 1;
            __IO uint32_t RESERVED_1: 1;
            __IO uint32_t RESERVED_0: 4;
            __IO uint32_t r_spic2_dma_tx_req_as_single: 1;
        } BITS_300;
    } u_300;

    /* 0x0304       0x4000_0304
        31:0    R/W    DUMMY                           32'h1FC00001
    */
    __IO uint32_t PERION_REG_DUMMY_304;

    /* 0x0308       0x4000_0308
        0       R/W    PON_SPIO_MST                    0
        3:1     R/W    DUMMY                           3'h0
        4       R/W    PON_SPI0_BRG_EN                 0
        5       R/W    PON_SPI0_H2S_BRG_EN             0
        6       R/W    PON_SPI1_BRG_EN                 0
        7       R/W    r_spi0_clk_src_sel              0
        8       R/W    r_spi1_clk_src_sel              0
        9       R/W    r_sclk_irrc_src_sel             0
        31:10   R/W    DUMMY                           22'h0
    */
    union
    {
        __IO uint32_t PERION_REG_RSVD_308;
        struct
        {
            __IO uint32_t PON_SPIO_MST: 1;
            __IO uint32_t RESERVED_1: 3;
            __IO uint32_t PON_SPI0_BRG_EN: 1;
            __IO uint32_t PON_SPI0_H2S_BRG_EN: 1;
            __IO uint32_t PON_SPI1_BRG_EN: 1;
            __IO uint32_t r_spi0_clk_src_sel: 1;
            __IO uint32_t r_spi1_clk_src_sel: 1;
            __IO uint32_t r_sclk_irrc_src_sel: 1;
            __IO uint32_t RESERVED_0: 22;
        } BITS_308;
    } u_308;

    __IO uint32_t RSVD_0x30c[3];

    /* 0x0318       0x4000_0318
        23:0    R      RSVD                            24'd0
        31:24   R/W    DUMMY                           8'd0
    */
    union
    {
        __IO uint32_t PERION_REG_DUMMY_318;
        struct
        {
            __I uint32_t RESERVED_1: 24;
            __IO uint32_t RESERVED_0: 8;
        } BITS_318;
    } u_318;

    __IO uint32_t RSVD_0x31c[1];

    /* 0x0320       0x4000_0320
        15:0    R/W    DUMMY                           16'h7E02
        31:16   R/W    DUMMY                           16'h0912
    */
    union
    {
        __IO uint32_t PERION_BT_ANAPAR_PLL;
        struct
        {
            __IO uint32_t RESERVED_1: 16;
            __IO uint32_t RESERVED_0: 16;
        } BITS_320;
    } u_320;

    /* 0x0324       0x4000_0324
        31:0    R      RSVD                            32'hEAEAEAEA
    */
    __IO uint32_t PERION_REG_RSVD_324;

    /* 0x0328       0x4000_0328
        31:0    R      RSVD                            32'hEAEAEAEA
    */
    __IO uint32_t PERION_REG_RSVD_328;

    /* 0x032C       0x4000_032c
        3:0     R      OSC40_FSET                      4'h0
        7:4     R      RSVD                            4'h0
        31:8    R/W    DUMMY                           24'h000109
    */
    union
    {
        __IO uint32_t PERION_REG_DUMMY_32C;
        struct
        {
            __I uint32_t OSC40_FSET: 4;
            __I uint32_t RESERVED_1: 4;
            __IO uint32_t RESERVED_0: 24;
        } BITS_32C;
    } u_32C;

    /* 0x0330       0x4000_0330
        0       R/W    rst_n_aac                       0
        1       R/W    Offset_plus                     0
        7:2     R/W    XAAC_GM_offset                  6'b000000
        8       R/W    GM_STEP                         0
        14:9    R/W    GM_INIT                         6'b111110
        17:15   R/W    XTAL_CLK_SET                    3'b101
        23:18   R/W    GM_STUP                         6'b111111
        29:24   R/W    GM_MANUAL                       6'b111111
        30      R/W    r_EN_XTAL_AAC_DIGI              0
        31      R/W    r_EN_XTAL_AAC_TRIG              0
    */
    union
    {
        __IO uint32_t PERION_REG_AAC_0;
        struct
        {
            __IO uint32_t rst_n_aac: 1;
            __IO uint32_t Offset_plus: 1;
            __IO uint32_t XAAC_GM_offset: 6;
            __IO uint32_t GM_STEP: 1;
            __IO uint32_t GM_INIT: 6;
            __IO uint32_t XTAL_CLK_SET: 3;
            __IO uint32_t GM_STUP: 6;
            __IO uint32_t GM_MANUAL: 6;
            __IO uint32_t r_EN_XTAL_AAC_DIGI: 1;
            __IO uint32_t r_EN_XTAL_AAC_TRIG: 1;
        } BITS_330;
    } u_330;

    /* 0x0334       0x4000_0334
        0       R      XAAC_BUSY                       0
        1       R      XAAC_READY                      0
        7:2     R      XTAL_GM_OUT                     6'h3F
        11:8    R      xaac_curr_state                 4'h0
        12      R/W    EN_XTAL_AAC_GM                  0
        13      R/W    EN_XTAL_AAC_PKDET               0
        14      R      XTAL_PKDET_OUT                  0
        15      R/W    DUMMY                           0
        31:16   R/W    DUMMY                           16'h0129
    */
    union
    {
        __IO uint32_t PERION_REG_AAC_1;
        struct
        {
            __I uint32_t XAAC_BUSY: 1;
            __I uint32_t XAAC_READY: 1;
            __I uint32_t XTAL_GM_OUT: 6;
            __I uint32_t xaac_curr_state: 4;
            __IO uint32_t EN_XTAL_AAC_GM: 1;
            __IO uint32_t EN_XTAL_AAC_PKDET: 1;
            __I uint32_t XTAL_PKDET_OUT: 1;
            __IO uint32_t RESERVED_1: 1;
            __IO uint32_t RESERVED_0: 16;
        } BITS_334;
    } u_334;

    /* 0x0338       0x4000_0338
        0       R/W    disable_pll_pre_gating          0
        15:1    R/W    DUMMY                           15'h0
        17:16   R/W    DUMMY                           2'h3
        23:18   R      RSVD                            6'h0
        24      R      BT_PLL_READY                    0
        25      R      XTAL_OK                         0
        31:26   R      RSVD                            6'h0
    */
    union
    {
        __IO uint32_t PERION_REG_XTAL_PLL;
        struct
        {
            __IO uint32_t disable_pll_pre_gating: 1;
            __IO uint32_t RESERVED_3: 15;
            __IO uint32_t RESERVED_2: 2;
            __I uint32_t RESERVED_1: 6;
            __I uint32_t BT_PLL_READY: 1;
            __I uint32_t XTAL_OK: 1;
            __I uint32_t RESERVED_0: 6;
        } BITS_338;
    } u_338;

    /* 0x033c       0x4000_033c
        0       R/W    resetn                          0
        1       R/W    EN_XTAL_PDCK_DIGI               1
        2       R/W    PDCK_SEARCH_MODE                0
        4:3     R/W    PDCK_WAIT_CYC[1:0]              2'b01
        9:5     R/W    VREF_MANUAL[4:0]                5'b10000
        14:10   R/W    VREF_INIT[4:0]                  5'b11111
        16:15   R/W    XTAL_PDCK_UNIT[1:0]             2'b10
        21:17   R/W    XPDCK_VREF_SEL[4:0]             5'b00000
        22      R/W    PDCK_LPOW                       0
        27:23   R/W    Reserved                        5'h0
        31:28   R      pdck_state[3:0]                 4'h0
    */
    union
    {
        __IO uint32_t PERION_REG_XTAL_PDCK;
        struct
        {
            __IO uint32_t resetn: 1;
            __IO uint32_t EN_XTAL_PDCK_DIGI: 1;
            __IO uint32_t PDCK_SEARCH_MODE: 1;
            __IO uint32_t PDCK_WAIT_CYC_1_0: 2;
            __IO uint32_t VREF_MANUAL_4_0: 5;
            __IO uint32_t VREF_INIT_4_0: 5;
            __IO uint32_t XTAL_PDCK_UNIT_1_0: 2;
            __IO uint32_t XPDCK_VREF_SEL_4_0: 5;
            __IO uint32_t PDCK_LPOW: 1;
            __IO uint32_t RESERVED_0: 5;
            __I uint32_t pdck_state_3_0: 4;
        } BITS_33c;
    } u_33c;

    /* 0x0340       0x4000_0340
        31:0    R/W    DUMMY                           32'hAAAAAAAA
    */
    __IO uint32_t PERION_REG_DUMMY_340;

    /* 0x0344       0x4000_0344
        7:0     R/W    dbnc_cnt_limit                  8'h1
        11:8    R/W    dbnc_div_sel                    4'h1
        12      R/W    dbnc_div_en                     1'b0
        15:13   R/W    DUMMY                           3'h0
        31:16   R/W    DUMMY                           16'h0
    */
    union
    {
        __IO uint32_t PERION_REG_DBNC;
        struct
        {
            __IO uint32_t dbnc_cnt_limit: 8;
            __IO uint32_t dbnc_div_sel: 4;
            __IO uint32_t dbnc_div_en: 1;
            __IO uint32_t RESERVED_1: 3;
            __IO uint32_t RESERVED_0: 16;
        } BITS_344;
    } u_344;

    /* 0x0348       0x4000_0348
        31:0    R      RSVD                            32'h0
    */
    __IO uint32_t PERION_REG_RSVD_348;

    /* 0x034C       0x4000_034c
        31:0    R      RSVD                            32'h0
    */
    __IO uint32_t PERION_REG_RSVD_34C;

    __IO uint32_t RSVD_0x350[3];

    /* 0x035C       0x4000_035c
        0       R/W    BIT_PERI_GT3_CLK_SEL            0
        1       R/W    BIT_PERI_GT5_CLK_SEL            0
        2       R/W    BIT_PERI_GT7_CLK_SEL            0
        7:3     R/W    DUMMY                           5'h0
        8       R/W    DUMMY                           0
        10:9    R/W    BIT_PERI_UART0_CLK_DIV          2'h0
        12:11   R/W    BIT_PERI_UART1_CLK_DIV          2'h0
        14:13   R/W    BIT_PERI_UART2_CLK_DIV          2'h0
        16:15   R/W    BIT_PERI_I2C0_CLK_DIV           2'h0
        18:17   R/W    BIT_PERI_I2C1_CLK_DIV           2'h0
        20:19   R/W    BIT_PERI_SPI0_CLK_DIV           2'h0
        22:21   R/W    BIT_PERI_SPI1_CLK_DIV           2'h0
        23      R/W    DUMMY                           0
        24      R/W    TIMER2_CLK_SRC_SEL              0
        25      R/W    TIMER3_CLK_SRC_SEL              0
        26      R/W    TIMER4_CLK_SRC_SEL              0
        27      R/W    TIMER5_CLK_SRC_SEL              0
        28      R/W    TIMER6_CLK_SRC_SEL              0
        29      R/W    TIMER7_CLK_SRC_SEL              0
        31:30   R/W    DUMMY                           2'h0
    */
    union
    {
        __IO uint32_t PERION_REG_PERI_GTIMER_CLK_SRC1;
        struct
        {
            __IO uint32_t BIT_PERI_GT3_CLK_SEL: 1;
            __IO uint32_t BIT_PERI_GT5_CLK_SEL: 1;
            __IO uint32_t BIT_PERI_GT7_CLK_SEL: 1;
            __IO uint32_t RESERVED_3: 5;
            __IO uint32_t RESERVED_2: 1;
            __IO uint32_t BIT_PERI_UART0_CLK_DIV: 2;
            __IO uint32_t BIT_PERI_UART1_CLK_DIV: 2;
            __IO uint32_t BIT_PERI_UART2_CLK_DIV: 2;
            __IO uint32_t BIT_PERI_I2C0_CLK_DIV: 2;
            __IO uint32_t BIT_PERI_I2C1_CLK_DIV: 2;
            __IO uint32_t BIT_PERI_SPI0_CLK_DIV: 2;
            __IO uint32_t BIT_PERI_SPI1_CLK_DIV: 2;
            __IO uint32_t RESERVED_1: 1;
            __IO uint32_t TIMER2_CLK_SRC_SEL: 1;
            __IO uint32_t TIMER3_CLK_SRC_SEL: 1;
            __IO uint32_t TIMER4_CLK_SRC_SEL: 1;
            __IO uint32_t TIMER5_CLK_SRC_SEL: 1;
            __IO uint32_t TIMER6_CLK_SRC_SEL: 1;
            __IO uint32_t TIMER7_CLK_SRC_SEL: 1;
            __IO uint32_t RESERVED_0: 2;
        } BITS_35C;
    } u_35C;

    /* 0x0360       0x4000_0360
        2:0     R/W    BIT_PERI_GT0_CLK_DIV            3'h0
        5:3     R/W    BIT_PERI_GT1_CLK_DIV            3'h0
        7:6     R/W    DUMMY                           2'h0
        8       R/W    BIT_TIMER_CLK_32K_EN            1
        9       R/W    BIT_TIMER_CLK_f40M_EN           1
        10      R/W    timer_apb_clk_disable           0
        11      R/W    gpio_intr_trig_en               0
        12      R/W    DUMMY                           1'h0
        15:13   R/W    BIT_PERI_GT2_CLK_DIV            3'h0
        18:16   R/W    BIT_PERI_GT3_CLK_DIV            3'h0
        21:19   R/W    BIT_PERI_GT4_CLK_DIV            3'h0
        24:22   R/W    BIT_PERI_GT5_CLK_DIV            3'h0
        27:25   R/W    BIT_PERI_GT6_CLK_DIV            3'h0
        30:28   R/W    BIT_PERI_GT7_CLK_DIV            3'h0
        31      R/W    DUMMY                           0
    */
    union
    {
        __IO uint32_t PERION_REG_PERI_GTIMER_CLK_SRC0;
        struct
        {
            __IO uint32_t BIT_PERI_GT0_CLK_DIV: 3;
            __IO uint32_t BIT_PERI_GT1_CLK_DIV: 3;
            __IO uint32_t RESERVED_2: 2;
            __IO uint32_t BIT_TIMER_CLK_32K_EN: 1;
            __IO uint32_t BIT_TIMER_CLK_f40M_EN: 1;
            __IO uint32_t timer_apb_clk_disable: 1;
            __IO uint32_t gpio_intr_trig_en: 1;
            __IO uint32_t RESERVED_1: 1;
            __IO uint32_t BIT_PERI_GT2_CLK_DIV: 3;
            __IO uint32_t BIT_PERI_GT3_CLK_DIV: 3;
            __IO uint32_t BIT_PERI_GT4_CLK_DIV: 3;
            __IO uint32_t BIT_PERI_GT5_CLK_DIV: 3;
            __IO uint32_t BIT_PERI_GT6_CLK_DIV: 3;
            __IO uint32_t BIT_PERI_GT7_CLK_DIV: 3;
            __IO uint32_t RESERVED_0: 1;
        } BITS_360;
    } u_360;

    /* 0x0364       0x4000_0364
        7:0     R/W    dead_zone_size                  8'h0
        8       R/W    emg_stop                        0
        9       R/W    stop_state[0]                   0
        10      R/W    stop_state[1]                   0
        11      R/W    DUMMY                           0
        12      R/W    dead_zone_en                    0
        13      R/W    pwm_pn_invserse_sel             0
        15:14   R/W    DUMMY                           2'h0
        16      R/W    r_clk_timer_pwm_wrap0_sel       0
        17      R/W    r_FEN_TIMER_PWM_WRAP0           0
        18      R/W    r_CKE_TIMER_PWM_WRAP0           0
        19      R/W    r_clk_timer_pwm_wrap0_sel1      0
        31:20   R/W    DUMMY                           12'h0
    */
    union
    {
        __IO uint32_t PERION_REG_PERI_PWM2_DZONE_CTRL;
        struct
        {
            __IO uint32_t dead_zone_size: 8;
            __IO uint32_t emg_stop: 1;
            __IO uint32_t stop_state_0: 1;
            __IO uint32_t stop_state_1: 1;
            __IO uint32_t RESERVED_2: 1;
            __IO uint32_t dead_zone_en: 1;
            __IO uint32_t pwm_pn_invserse_sel: 1;
            __IO uint32_t RESERVED_1: 2;
            __IO uint32_t r_clk_timer_pwm_wrap0_sel: 1;
            __IO uint32_t r_FEN_TIMER_PWM_WRAP0: 1;
            __IO uint32_t r_CKE_TIMER_PWM_WRAP0: 1;
            __IO uint32_t r_clk_timer_pwm_wrap0_sel1: 1;
            __IO uint32_t RESERVED_0: 12;
        } BITS_364;
    } u_364;

    /* 0x0368       0x4000_0368
        7:0     R/W    dead_zone_size                  8'h0
        8       R/W    emg_stop                        0
        9       R/W    stop_state[0]                   0
        10      R/W    stop_state[1]                   0
        11      R/W    DUMMY                           0
        12      R/W    dead_zone_en                    0
        13      R/W    pwm_pn_invserse_sel             0
        15:14   R/W    DUMMY                           2'h0
        16      R/W    r_clk_timer_pwm_wrap0_sel       0
        17      R/W    r_FEN_TIMER_PWM_WRAP0           0
        18      R/W    r_CKE_TIMER_PWM_WRAP0           0
        19      R/W    r_clk_timer_pwm_wrap0_sel1      0
        31:20   R/W    DUMMY                           12'h0
    */
    union
    {
        __IO uint32_t PERION_REG_PERI_ENHTIMER0_DZONE_CTRL;
        struct
        {
            __IO uint32_t dead_zone_size: 8;
            __IO uint32_t emg_stop: 1;
            __IO uint32_t stop_state_0: 1;
            __IO uint32_t stop_state_1: 1;
            __IO uint32_t RESERVED_2: 1;
            __IO uint32_t dead_zone_en: 1;
            __IO uint32_t pwm_pn_invserse_sel: 1;
            __IO uint32_t RESERVED_1: 2;
            __IO uint32_t r_clk_timer_pwm_wrap0_sel: 1;
            __IO uint32_t r_FEN_TIMER_PWM_WRAP0: 1;
            __IO uint32_t r_CKE_TIMER_PWM_WRAP0: 1;
            __IO uint32_t r_clk_timer_pwm_wrap0_sel1: 1;
            __IO uint32_t RESERVED_0: 12;
        } BITS_368;
    } u_368;

    __IO uint32_t RSVD_0x36c[34];

    /* 0x03F4       0x4000_03f4
        31:0    R      RSVD                            32'h0
    */
    __IO uint32_t PERION_REG_RSVD_3F4;

    /* 0x03F8       0x4000_03f8
        0       R      r_cap_touch_clk_src_sel         1'h0
        31:1    R      RSVD                            31'h0
    */
    union
    {
        __IO uint32_t PERION_REG_CTC_CLK_SRC_SEL;
        struct
        {
            __I uint32_t r_cap_touch_clk_src_sel: 1;
            __I uint32_t RESERVED_0: 31;
        } BITS_3F8;
    } u_3F8;

} SYS_BLKCTRL_TypeDef;

/* ================================================================================ */
/* ================                   Peripheral                   ================ */
/* ================================================================================ */

/**
  * @brief Peripheral. (Peripheral)
  */

typedef struct                                      /*!< Peripheral Structure */
{
    uint32_t RSVD0[134];                                /*!< Reserved */
    __IO uint32_t    PERI_FUNC0_EN;                     /*!< 0x218 */
    __IO uint32_t    PERI_FUNC1_EN;                     /*!< 0x21C */
    __IO uint32_t    PERI_BD_FUNC0_EN;                  /*!< 0x220 */
    uint32_t RSVD1[3];                                  /*!< Reserved */
    __IO uint32_t    PERI_CLK_CTRL;                     /*!< 0x230 */
    __IO uint32_t    PERI_CLK_CTRL0;                    /*!< 0x234 */
    __IO uint32_t    PERI_CLK_CTRL1;                    /*!< 0x238 */
} PHERIPHERIAL_TypeDef;

/* ================================================================================ */
/* ================              Peripheral Interrupt              ================ */
/* ================================================================================ */

/**
  * @brief Peripheral Interrupt. (Peripheral Interrupt)
  */

typedef struct                 /*!< Peripheral Interrupt Structure */
{
    __IO uint32_t RSVD0;       /*!< 0x00 */
    __IO uint32_t STATUS;      /*!< 0x04 */
    __IO uint32_t MODE;        /*!< 0x08, 0: high level active, 1: edge trigger */
    __IO uint32_t EN;          /*!< 0x0C */
    __IO uint32_t RSVD1;       /*!< 0x10 */
    __IO uint32_t RSVD2;       /*!< 0x14 */
    __IO uint32_t EDGE_MODE;   /*!< 0x18, 0: rising edge (HW default), 1: both edge */
} PERI_INT_TypeDef;

/* ================================================================================ */
/* ================                    Security                    ================ */
/* ================================================================================ */

/**
  * @brief Security. (Security)
  */

typedef union                                               /*!< Security Structure */
{
    __IO uint32_t d32;
    struct
    {
        __IO uint32_t reg_pon_debug_port_wp: 1;
        __IO uint32_t reg_pon_debug_port: 1;
        __IO uint32_t reg_pon_hwspi_en_wp: 1;
        __IO uint32_t reg_pon_hwspi_en: 1;
        __IO uint32_t is_efuse_invalid: 1;
        __IO uint32_t reg_efuse_system_general_ctrl_lock: 1;
        __IO uint32_t rsvd0: 2;
        __IO uint32_t is_enable_efuse_read_protect: 1;
        __IO uint32_t is_enable_efuse_write_protect: 1;
        __IO uint32_t is_disable_hci_ram_patch: 1;
        __IO uint32_t is_disable_hci_flash_access: 1;
        __IO uint32_t is_disable_hci_system_access: 1;
        __IO uint32_t is_disable_hci_efuse_access: 1;
        __IO uint32_t is_disable_hci_bt_test: 1;
        __IO uint32_t is_debug_password_invalid: 1;
        __IO uint32_t rsvd1: 4;
        __IO uint32_t reg_pon_clk_flash_dis: 1;
        __IO uint32_t rsvd2: 11;
    };
} SECURITY_TypeDef;

/* ================================================================================ */
/* ================                 Peri clock reg                 ================ */
/* ================================================================================ */

/**
  * @brief PWM
  */
typedef struct
{
    __IO uint32_t CLKSELE;                    /*!< 0x348*/
    uint32_t RSVD[4];
    __IO uint32_t CLK_SRCL;
    __IO uint32_t CLK_SRCH;                     /*!< 0x360*/
    __IO uint32_t PWM0_CTRL_L;
    __IO uint32_t PWM0_CTRL_H;
    __IO uint32_t PWM1_CTRL_L;
    __IO uint32_t PWM1_CTRL_H;                 /*!< 0x370*/
    __IO uint32_t PWM2_CTRL_L;
    __IO uint32_t PWM2_CTRL_H;
    __IO uint32_t PWM3_CTRL_L;
    __IO uint32_t PWM3_CTRL_H;                /*!< 0x380*/
    __IO uint32_t TIM_EVT_CTRL;

} Peri_ClockGate_TypeDef;


/* ================================================================================ */
/* ================                      CACHE                     ================ */
/* ================================================================================ */

/**
  * @brief cache for flash
  */
typedef struct
{
    __IO uint32_t CACHE_ENABLE;      /*!< SPIC cache Enable Register, Address Offset: 0x00*/
    __IO uint32_t FLUSH;             /*!< Cache Flush register, Address Offset: 0x04*/
    __IO uint32_t INTR;              /*!< Cache Interrupt register, Address Offset: 0x08*/
    __O  uint32_t RST_CNT;           /*!< Cache Reset Counter register, Address Offset: 0x0C*/
    __I  uint32_t RD_EVT_CNT;        /*!< Cache Read Event Counter register, Address Offset: 0x10*/
    __I  uint32_t HIT_EVT_CNT;       /*!< Cache HIT Event Counter register, Address Offset: 0x14*/
    __I  uint32_t HIT_LSTW_EVT_CNT;  /*!< Cache Hit lastway event counter register, Offset: 0x18*/
    __I  uint32_t RD_PND_CNT;        /*!< Cache Read pending counter register, Offset: 0x1c*/
    __I  uint32_t RSVD0[4];          /*!< Reserved, Offset: 0x20 ~ 0x2C */
    __IO uint32_t HITWAYBUF_CTRL;    /*!< Cache Hit way buffer control register, Offset: 0x30 */
    __IO uint32_t CACHE_RAM_CTRL;    /*!< Cache Two way function control register, Offset: 0x34 */
    __IO uint32_t COUNTER_ENABLE;    /*!< Cache counter enable register, Offset: 0x38 */
} CACHE_TypeDef;

/* ================================================================================ */
/* ================                      SPIC                      ================ */
/* ================================================================================ */
typedef struct
{
    __IO uint32_t ctrlr0;             /*!< SPIC control register0, Address offset: 0x000 */
    __IO uint32_t
    rx_ndf;             /*!< SPIC receiving data frame number register, Address offset: 0x004 */
    __IO uint32_t ssienr;             /*!< SPIC enable register, Address offset: 0x008 */
    __IO uint32_t mwcr;               /*!< N/A, Address offset: 0x00C */
    __IO uint32_t ser;                /*!< SPIC slave enable register, Address offset: 0x010 */
    __IO uint32_t baudr;              /*!< SPIC baudrate select register, Address offset: 0x014 */
    __IO uint32_t txftlr;             /*!< SPIC transmit FIFO threshold level,  Address offset: 0x018 */
    __IO uint32_t rxftlr;             /*!< SPIC receive FIFO threshold level, Address offset: 0x01C */
    __IO uint32_t txflr;              /*!< SPIC transmit FIFO level register, Address offset: 0x020 */
    __IO uint32_t rxflr;              /*!< SPIC receive FIFO level register,  Address offset: 0x024 */
    __IO uint32_t sr;                 /*!< SPIC status register, Address offset: 0x028 */
    __IO uint32_t imr;                /*!< SPIC interrupt mask register, Address offset: 0x02C */
    __IO uint32_t isr;                /*!< SPIC interrupt status register, Address offset: 0x030 */
    __IO uint32_t risr;               /*!< SPIC raw interrupt status register, Address offset: 0x034 */
    __IO uint32_t
    txoicr;             /*!< SPIC transmit FIFO overflow interrupt clear register,offset: 0x038 */
    __IO uint32_t
    rxoicr;             /*!< SPIC receive FIFO overflow interrupt clear register, offset: 0x03C */
    __IO uint32_t
    rxuicr;             /*!< SPIC receive FIFO underflow interrupt clear register,offset: 0x040 */
    __IO uint32_t
    msticr;             /*!< SPIC master error interrupt clear register,   Address offset: 0x044 */
    __IO uint32_t icr;                /*!< SPIC interrupt clear register, Address offset: 0x048 */
    __IO uint32_t dmacr;              /*!< N/A, Address offset: 0x04C */
    __IO uint32_t dmatdlr;            /*!< N/A, Address offset: 0x050 */
    __IO uint32_t dmardlr;            /*!< N/A, Address offset: 0x054 */
    __IO uint32_t idr;                /*!< SPIC Identiation register,  Address offset: 0x058 */
    __IO uint32_t spi_flash_version;  /*!< SPIC version ID register, Address offset: 0x05C */
    union
    {
        __IO uint8_t  byte;
        __IO uint16_t half;
        __IO uint32_t word;
    } dr[32];                         /*!< SPIC data register, Address offset: 0x060~0x09C */
    __IO uint32_t rd_fast_single;     /*!< Fast read data command of SPI Flash, offset: 0x0E0 */
    __IO uint32_t rd_dual_o;          /*!< Dual output read command of SPI Flash, offset: 0x0E4 */
    __IO uint32_t rd_dual_io;         /*!< Dual I/O read command of SPI Flash, offset: 0x0E8 */
    __IO uint32_t rd_quad_o;          /*!< Quad output read command of SPI Flash, offset: 0x0EC */
    __IO uint32_t rd_quad_io;         /*!< Quad I/O read command of SPI Flash, offset: 0x0F0 */
    __IO uint32_t wr_single;          /*!< Page program command of SPI Flash,  offset: 0x0F4 */
    __IO uint32_t
    wr_dual_i;          /*!< Dual data input program command of SPI Flash, offset: 0x0F8 */
    __IO uint32_t
    wr_dual_ii;         /*!< Dual address&data program command of SPI Flash,offset: 0x0FC */
    __IO uint32_t wr_quad_i;          /*!< Quad data input program command of SPI Flash,offset: 0x100 */
    __IO uint32_t
    wr_quad_ii;         /*!< Quad address&data program command of SPI Flash,offset: 0x104 */
    __IO uint32_t wr_enable;          /*!< Write enabe command of SPI Flash, Address offset: 0x108 */
    __IO uint32_t rd_status;          /*!< Read status command of SPI Flash, offset: 0x10C */
    __IO uint32_t ctrlr2;             /*!< SPIC control register2, Address offset: 0x110 */
    __IO uint32_t fbaudr;             /*!< SPIC fast baudrate select, Address offset: 0x114 */
    __IO uint32_t user_length;        /*!< SPIC address length register, Address offset: 0x118 */
    __IO uint32_t auto_length;        /*!< SPIC auto address length register, Address offset: 0x11C */
    __IO uint32_t valid_cmd;          /*!< SPIC valid command register, Address offset: 0x120 */
    __IO uint32_t flash_size;         /*!< SPIC flash size register, Address offset: 0x124 */
    __IO uint32_t flush_fifo;         /*!< SPIC flush FIFO register, Address offset: 0x128 */
    __IO uint32_t dum_byte;           /*!< SPIC dummy byte register, Address offset: 0x12c */
    __IO uint32_t
    tx_ndf;             /*!< SPIC transmitting data frame number register, Address offset: 0x130 */
    __IO uint32_t device_info;        /*!< SPIC device info register, Address offset: 0x134 */
    __IO uint32_t tpr0;               /*!< SPIC timing parameter register, Address offset: 0x138 */
    __IO uint32_t auto_length2;       /*!< SPIC auto length 2 register, Address offset: 0x13c */
} SPIC_TypeDef;

/** @} */ /* End of group RTL876X_Peripheral_Registers_Structures */

/*============================================================================*
 *                              Macros
 *============================================================================*/

/** @defgroup RTL876X_Exported_Macros RTL876X  Exported Macros
    * @brief
    * @{
    */

/* ================================================================================ */
/* ================              Peripheral memory map             ================ */
/* ================================================================================ */
#define SYSTEM_REG_BASE             0x40000000UL
#define PERIPH_REG_BASE             0x40000000UL
#define RTC_REG_BASE                0x40000100UL
#define RTC_LP_REG_BASE             0x40000180UL
#define LPC_REG_BASE                0x40000180UL
#define AON_WDG_REG_BASE            0x40000190UL
#define SYSBLKCTRL_REG_BASE         0x40000200UL
#define PINMUX_REG_BASE             0x40000280UL
#define SPIC_DLY_CTRL_BASE          0x40000300UL
#define PWM2_REG_BASE               0x40000364UL
#define ENHPWM0_REG_BASE            0x40000368UL
#define RTC_LED_REG_BASE            0x40000400UL

#define GPIO_REG_BASE               0x40001000UL
#define TIM0_REG_BASE               0x40002000UL
#define TIM1_REG_BASE               0x40002014UL
#define TIM2_REG_BASE               0x40002028UL
#define TIM3_REG_BASE               0x4000203CUL
#define TIM4_REG_BASE               0x40002050UL
#define TIM5_REG_BASE               0x40002064UL
#if (IC_TYPE == IC_TYPE_SBEE2)
#define TIM6_REG_BASE               0x40002078UL
#define TIM7_REG_BASE               0x4000208CUL
#endif
#define QDEC_REG_BASE               0x40004000UL
#define SPI2WIRE_REG_BASE           0x40004000UL
#define KEYSCAN_REG_BASE            0x40005000UL
#define VENDOR_REG_BASE             0x40006000UL
#define WDG_REG_BASE                0x40006000UL
#define PERI_INT_REG_BASE           0x40006004UL
#define RANDOM_GEN_REG_BASE         0x40006150UL
#define CACHE_REG_BASE              0x40007000UL
#define ENHANCED_TIM_BASE           0x40008000UL
#define ENHTIM0_REG_BASE            0x40008000UL
#define ENHTIM1_REG_BASE            0x40008024UL
#define ENHTIM_SHARE_REG_BASE       0x40008120UL
#define CAP_TOUCH_BASE              0x40009000UL
#define CTC_Channel0_BASE           0x40009100UL
#define CTC_Channel1_BASE           0x40009110UL
#define CTC_Channel2_BASE           0x40009120UL
#define CTC_Channel3_BASE           0x40009130UL
#define CTC_Channel4_BASE           0x40009140UL
#define CTC_FAST_BASE               0x40009150UL
#define ADC_REG_BASE                0x40010000UL
#define UART1_REG_BASE              0x40011000UL  //log
#define UART0_REG_BASE              0x40012000UL  //data
#define SPI0_REG_BASE               0x40013000UL
#define SPI1_REG_BASE               0x40013400UL
#define HW_AES_REG_BASE             0x40014000UL
#define I2C0_REG_BASE               0x40015000UL
#define I2C1_REG_BASE               0x40015400UL
#if (IC_TYPE == IC_TYPE_BEE3)
#define IR_REG_BASE                 0x40015800UL
#else
#define IR_REG_BASE                 0x40016000UL
#endif
#if (IC_TYPE != IC_TYPE_BEE3)
#define SPORT0_REG_BASE             0x40020000UL
#define I2S0_REG_BASE               0x40020000UL
#define SPORT1_REG_BASE             0x40021000UL
#define I2S1_REG_BASE               0x40021000UL
#endif
#define SPDIF_REG_BASE              0x40021800UL
#define CODEC_ANA_REG_BASE          0x40022000UL
#define CODEC_REG_BASE              0x40022100UL
#define H2D_D2H_REG_BASE            0x40023000UL
#if (IC_TYPE == IC_TYPE_SBEE2)
#define UART2_REG_BASE              0x40024000UL
#define IF8080_REG_BASE             0x40024800UL
#define IF8080_LLI_REG1_BASE        0x40024850UL
#define IF8080_LLI_REG1_GDMA_BASE   0x400248A0UL
#define IF8080_LLI_REG2_BASE        0x40024880UL
#define IF8080_LLI_REG2_GDMA_BASE   0x400248C0UL
#define IF8080_LLI_REG1_OFT_BASE    0x40024870UL
#define IF8080_LLI_REG2_OFT_BASE    0x40024878UL
#define IF8080_LLI_CR_REG_BASE      0x40024894UL
#endif
#define GDMA_CHANNEL_REG_BASE       0x40027000UL // for platform_1_1_1_20160323 later
#define GDMA_Channel0_BASE          (GDMA_CHANNEL_REG_BASE + 0x0000)
#define GDMA_Channel1_BASE          (GDMA_CHANNEL_REG_BASE + 0x0058)
#define GDMA_Channel2_BASE          (GDMA_CHANNEL_REG_BASE + 0x00b0)
#define GDMA_Channel3_BASE          (GDMA_CHANNEL_REG_BASE + 0x0108)
#if (IC_TYPE == IC_TYPE_SBEE2)
#define GDMA_Channel4_BASE          (GDMA_CHANNEL_REG_BASE + 0x0160)
#define GDMA_Channel5_BASE          (GDMA_CHANNEL_REG_BASE + 0x01b8)
#endif
#define GDMA_REG_BASE               (GDMA_CHANNEL_REG_BASE + 0x02c0)
#define I2S0_REG_BASE               0x40029000UL
#define RTL_SPI0_BASE               0x40042000UL
#define RTL_SPI1_BASE               0x40042400UL
#define BT_BB_REG_BASE              0x40050000UL
#define HCI_DMA_REG_BASE            0x40064000UL
#define HCI_UART_REG_BASE           0x40068000UL
#define BT_VENDOR_REG_BASE          0x40058000UL
#define SECURITY_REG_BASE           0x40058f28UL
#define GDMA0_REG_BASE              0x40060000UL
#define SPIC0_REG_BASE              0x40080000UL
#define SPIC1_REG_BASE              0x40090000UL
#define SPIC2_REG_BASE              0x400A0000UL

#define SPI0_MASTER_MODE_REG        *((volatile uint32_t *)0x40000308UL)
#define SPI0_MASTER_MODE_BIT        BIT(8)

/** @brief clock divider for peripheral */
#define SYSTEM_CLK_CTRL  *((volatile uint32_t *)0x4000020CUL)
#define CLK_SOURCE_REG_1 *((volatile uint32_t *)0x4000035CUL)
#define CLK_SOURCE_REG_2 *((volatile uint32_t *)0x40000360UL)

#define REG_PEON_SYS_CLK_SEL        0x0200
#define REG_PEON_SYS_CLK_SEL_2      0x0208
#define REG_PEON_SYS_CLK_SEL_3      0x020C
#define REG_SOC_FUNC_EN             0x0210
#define REG_SOC_HCI_COM_FUNC_EN     0x0214
#define REG_SOC_PERI_FUNC0_EN       0x0218
#define REG_SOC_PERI_FUNC1_EN       0x021C
#define REG_PESOC_CLK_CTRL          0x0230
#define REG_PESOC_PERI_CLK_CTRL0    0x0234
#define REG_PESOC_PERI_CLK_CTRL1    0x0238
#define REG_PESOC_DSP_SHARE_RAM     0x0250
#define REG_TEST_MODE               0x02a8
#define REG_ANAPAR_PLL1_0           0x0320
#define REG_ANAPAR_PLL3_2           0x0324
#define REG_ANAPAR_PLL5_4           0x0328
#define REG_XTAL_PLL_READY          0x0338

#define GPIO_OUTPUT_OFFSET          0x00
#define GPIO_DIRECTION_OFFSET       0x04

/** @brief AON PAD AREA */
#define REG_PAD_WKEN_ADDRESS        0x20
#define REG_PAD_WK_CTRL_ADDRESS     0x12d
#define REG_PAD_WKPOL_ADDRESS       0x25
#define REG_PAD_O_ADDRESS           0x2A
#define REG_AON_PAD_E_ADDRESS       0x2F
#define REG_AON_PAD_S_ADDRESS       0x34
#define REG_AON_PAD_PU_ADDRESS      0x39
#define REG_AON_PAD_PD_ADDRESS      0x3E
#define REG_AON_PAD_PWRON_ADDRESS   0x4C

#define BIT_WK_INTEN                (1U << 7) /**< BIT7 */
#define BIT_WK_FLAG_GPIO            (1U << 6) /**< BIT6 */

#define BIT_SOC_ACTCK_GPIO_EN       (1U << 24) /**< BIT24 */
#define BIT_PERI_GPIO_EN            (1U << 8) /**< BIT8 */

#define BIT_PERI_KEYSCAN_EN         (1U << 19) /**< BIT19 */
#define BIT_SOC_ACTCK_KEYSCAN_EN    (1U << 6) /**< BIT6 */


/* ================================================================================ */
/* ================             Peripheral declaration             ================ */
/* ================================================================================ */
/** @brief System */
#define PERIPH                          ((PHERIPHERIAL_TypeDef  *) PERIPH_REG_BASE)
#define SYSBLKCTRL                      ((SYS_BLKCTRL_TypeDef   *) SYSBLKCTRL_REG_BASE)
#define PERIPHINT                       ((PERI_INT_TypeDef      *) VENDOR_REG_BASE)
#define SECURITY                        ((SECURITY_TypeDef      *) SECURITY_REG_BASE)

/** @brief IO */
#define PINMUX                          ((PINMUX_TypeDef        *) PINMUX_REG_BASE)
#define KEYSCAN                         ((KEYSCAN_TypeDef       *) KEYSCAN_REG_BASE)
#define GPIO                            ((GPIO_TypeDef          *) GPIO_REG_BASE)
#define QDEC                            ((QDEC_TypeDef          *) QDEC_REG_BASE)
#define ADC                             ((ADC_TypeDef           *) ADC_REG_BASE)
#define CODEC_ANA                       ((CODEC_AnalogTypeDef   *) CODEC_ANA_REG_BASE)
#define CODEC                           ((CODEC_TypeDef         *) CODEC_REG_BASE)
#define CTC                             ((CTC_TypeDef           *) CAP_TOUCH_BASE)
#define CTC_Channel0                    ((CTC_ChannelTypeDef    *) CTC_Channel0_BASE)
#define CTC_Channel1                    ((CTC_ChannelTypeDef    *) CTC_Channel1_BASE)
#define CTC_Channel2                    ((CTC_ChannelTypeDef    *) CTC_Channel2_BASE)
#define CTC_Channel3                    ((CTC_ChannelTypeDef    *) CTC_Channel3_BASE)
#define CTC_Channel4                    ((CTC_ChannelTypeDef    *) CTC_Channel4_BASE)
#define CTC_FAST                        ((CTC_FSModeTypeDef     *) CTC_FAST_BASE)
#define ENH_TIM0                        ((ENHTIM_TypeDef        *) ENHTIM0_REG_BASE)
#define ENH_TIM1                        ((ENHTIM_TypeDef        *) ENHTIM1_REG_BASE)
#define ENH_TIM_SHARE                   ((ENHTIM_ShareTypeDef   *) ENHTIM_SHARE_REG_BASE)
#define GDMA_BASE                       ((GDMA_TypeDef          *) GDMA_REG_BASE)
#define GDMA_Channel0                   ((GDMA_ChannelTypeDef   *) GDMA_Channel0_BASE)
#define GDMA_Channel1                   ((GDMA_ChannelTypeDef   *) GDMA_Channel1_BASE)
#define GDMA_Channel2                   ((GDMA_ChannelTypeDef   *) GDMA_Channel2_BASE)
#define GDMA_Channel3                   ((GDMA_ChannelTypeDef   *) GDMA_Channel3_BASE)
#define I2C0                            ((I2C_TypeDef           *) I2C0_REG_BASE)
#define I2C1                            ((I2C_TypeDef           *) I2C1_REG_BASE)
#define I2S0                            ((I2S_TypeDef           *) I2S0_REG_BASE)
#define IR                              ((IR_TypeDef            *) IR_REG_BASE)
#define LPC                             ((LPC_TypeDef           *) LPC_REG_BASE)
#define HWAES                           ((HW_AES_TypeDef        *) HW_AES_REG_BASE)
#define WDG                             ((WDG_TypeDef           *) WDG_REG_BASE)
#define AON_WDG                         ((AON_WDG_TypeDef       *) AON_WDG_REG_BASE)
#define RAN_GEN                         ((RAN_GEN_TypeDef       *) RANDOM_GEN_REG_BASE)
#define ICG                             ((ICG_TypeDef           *) ICG_REG_BASE)
#define CACHE                           ((CACHE_TypeDef         *) CACHE_REG_BASE)
#define RTC                             ((RTC_TypeDef           *) RTC_REG_BASE)
#define RTC_LED                         ((RTC_LED_TypeDef       *) RTC_LED_REG_BASE)
#define RTC_LP                          ((RTC_LP_TypeDef        *) RTC_LP_REG_BASE)
#define SPI0                            ((SPI_TypeDef           *) SPI0_REG_BASE)
#define SPI1                            ((SPI_TypeDef           *) SPI1_REG_BASE)
#define SPI3WIRE                        ((SPI3WIRE_TypeDef      *) SPI2WIRE_REG_BASE)
#define SPIC                            ((SPIC_TypeDef          *) SPIC0_REG_BASE)
#define SPIC0                           ((SPIC_TypeDef          *) SPIC0_REG_BASE)
#define SPIC1                           ((SPIC_TypeDef          *) SPIC1_REG_BASE)
#define SPIC2                           ((SPIC_TypeDef          *) SPIC2_REG_BASE)
#define TIM0                            ((TIM_TypeDef           *) TIM0_REG_BASE)
#define TIM1                            ((TIM_TypeDef           *) TIM1_REG_BASE)
#define TIM2                            ((TIM_TypeDef           *) TIM2_REG_BASE)
#define TIM3                            ((TIM_TypeDef           *) TIM3_REG_BASE)
#define TIM4                            ((TIM_TypeDef           *) TIM4_REG_BASE)
#define TIM5                            ((TIM_TypeDef           *) TIM5_REG_BASE)
#define UART0                           ((UART_TypeDef          *) UART0_REG_BASE)
#define UART1                           ((UART_TypeDef          *) UART1_REG_BASE)

#define CLK_GATE                        ((Peri_ClockGate_TypeDef*) PERI_CLOCKGATE_REG_BASE)

//Add by Vendor
#define LITTLE_ENDIAN                   0
#define BIG_ENDIAN                      1
#define SYSTEM_ENDIAN                   LITTLE_ENDIAN

#define SWAP32(x) ((uint32_t)(\
                              (((uint32_t)(x) & (uint32_t)0x000000ff) << 24) |\
                              (((uint32_t)(x) & (uint32_t)0x0000ff00) <<  8) |\
                              (((uint32_t)(x) & (uint32_t)0x00ff0000) >>  8) |\
                              (((uint32_t)(x) & (uint32_t)0xff000000) >> 24)))

#define WAP16(x) ((uint16_t)(\
                             (((uint16_t)(x) & (uint16_t)0x00ff) <<  8) |\
                             (((uint16_t)(x) & (uint16_t)0xff00) >>  8)))

#if SYSTEM_ENDIAN == LITTLE_ENDIAN
#ifndef rtk_le16_to_cpu
#define rtk_cpu_to_le32(x)      ((uint32_t)(x))
#define rtk_le32_to_cpu(x)      ((uint32_t)(x))
#define rtk_cpu_to_le16(x)      ((uint16_t)(x))
#define rtk_le16_to_cpu(x)      ((uint16_t)(x))
#define rtk_cpu_to_be32(x)      SWAP32((x))
#define rtk_be32_to_cpu(x)      SWAP32((x))
#define rtk_cpu_to_be16(x)      WAP16((x))
#define rtk_be16_to_cpu(x)      WAP16((x))
#endif

#elif SYSTEM_ENDIAN == BIG_ENDIAN
#ifndef rtk_le16_to_cpu
#define rtk_cpu_to_le32(x)      SWAP32((x))
#define rtk_le32_to_cpu(x)      SWAP32((x))
#define rtk_cpu_to_le16(x)      WAP16((x))
#define rtk_le16_to_cpu(x)      WAP16((x))
#define rtk_cpu_to_be32(x)      ((uint32_t)(x))
#define rtk_be32_to_cpu(x)      ((uint32_t)(x))
#define rtk_cpu_to_be16(x)      ((uint16_t)(x))
#define rtk_be16_to_cpu(x)      ((uint16_t)(x))
#endif
#endif

#define HAL_READ32(base, addr)            \
    rtk_le32_to_cpu(*((volatile uint32_t *)(base + addr)))

#define HAL_WRITE32(base, addr, value32)  \
    ((*((volatile uint32_t *)(base + addr))) = rtk_cpu_to_le32(value32))

#define HAL_UPDATE32(addr, mask, value32)  \
    HAL_WRITE32(0, addr, (HAL_READ32(0, addr) & ~mask) | (value32 & mask))

#define HAL_READ16(base, addr)            \
    rtk_le16_to_cpu(*((volatile uint16_t *)(base + addr)))

#define HAL_WRITE16(base, addr, value)  \
    ((*((volatile uint16_t *)(base + addr))) = rtk_cpu_to_le16(value))

#define HAL_UPDATE16(addr, mask, value16)  \
    HAL_WRITE16(0, addr, (HAL_READ16(0, addr) & ~mask) | (value16 & mask))

#define HAL_READ8(base, addr)            \
    (*((volatile uint8_t *)(base + addr)))

#define HAL_WRITE8(base, addr, value)  \
    ((*((volatile uint8_t *)(base + addr))) = value)

#define HAL_UPDATE8(addr, mask, value8)  \
    HAL_WRITE8(0, addr, (HAL_READ8(0, addr) & ~mask) | (value8 & mask))

#define BIT0        0x00000001
#define BIT1        0x00000002
#define BIT2        0x00000004
#define BIT3        0x00000008
#define BIT4        0x00000010
#define BIT5        0x00000020
#define BIT6        0x00000040
#define BIT7        0x00000080
#define BIT8        0x00000100
#define BIT9        0x00000200
#define BIT10       0x00000400
#define BIT11       0x00000800
#define BIT12       0x00001000
#define BIT13       0x00002000
#define BIT14       0x00004000
#define BIT15       0x00008000
#define BIT16       0x00010000
#define BIT17       0x00020000
#define BIT18       0x00040000
#define BIT19       0x00080000
#define BIT20       0x00100000
#define BIT21       0x00200000
#define BIT22       0x00400000
#define BIT23       0x00800000
#define BIT24       0x01000000
#define BIT25       0x02000000
#define BIT26       0x04000000
#define BIT27       0x08000000
#define BIT28       0x10000000
#define BIT29       0x20000000
#define BIT30       0x40000000
#define BIT31       0x80000000

#ifdef BIT
#undef BIT
#endif
#define BIT(_n)                  (uint32_t)(1U << (_n))

#ifdef BIT64
#undef BIT64
#endif
#define BIT64(n)                 (1ULL << (n))

#define BIT_BAND(reg, bit_pos)   (*((volatile uint32_t*)(0x42000000 + ((uint32_t)&reg - 0x40000000) * 32 + bit_pos * 4)))


/* Uncomment the line below to expanse the "assert_param" macro in the
   Standard Peripheral Library drivers code */

//#define USE_FULL_ASSERT

/** @} */ /* End of group RTL876X_Exported_Macros */


/*============================================================================*
  *                                Functions
  *============================================================================*/
/** @defgroup RTL876X_Exported_Functions RTL876X Sets Exported Functions
    * @brief
    * @{
    */
#ifdef  USE_FULL_ASSERT
/**
  * @brief  The assert_param macro is used for function's parameters check.
  * @param  expr: If expr is false, it calls assert_failed function which reports
  *         the name of the source file and the source line number of the call
  *         that failed. If expr is true, it returns no value.
  * @retval None
  */
#define assert_param(expr) ((expr) ? (void)0 : io_assert_failed((uint8_t *)__FILE__, __LINE__))
void io_assert_failed(uint8_t *file, uint32_t line);
#else
#define assert_param(expr) ((void)0)
#endif /* USE_FULL_ASSERT */

/**
    * @brief    Read data from aon register
    * @param    offset: register address
    * @return   data read from register
    */
extern uint16_t btaon_fast_read(uint16_t offset);
extern uint8_t btaon_fast_read_8b(uint16_t offset);

/**
    * @brief    Read data from aon register safely
    * @param    offset: register address
    * @return   data read from register
    */
extern uint16_t btaon_fast_read_safe(uint16_t offset);
extern uint8_t btaon_fast_read_safe_8b(uint16_t offset);

/**
    * @brief    Write data to aon register
    * @param    offset:  register address
    * @param    data:  data to be writen to register
    * @return
    */
extern void btaon_fast_write(uint16_t offset, uint16_t data);
extern void btaon_fast_write_8b(uint16_t offset, uint8_t data);

/**
    * @brief    Write data to aon egister safely
    * @param    offset:  register address
    * @param    data:  data to be writen to register
    * @return
    */
extern void btaon_fast_write_safe(uint16_t offset, uint16_t data);
extern void btaon_fast_write_safe_8b(uint16_t offset, uint8_t data);

/**
    * @brief    Write data to aon egister
    * @param    offset: register address
    * @param    mask:   indicate which bit in data will be updated
    * @param    data:   data to be writen to register
    * @return
    */
extern void btaon_fast_update(uint16_t offset, uint16_t mask, uint16_t data);
extern void btaon_fast_update_8b(uint16_t offset, uint8_t mask, uint8_t data);

/**
    * @brief    Write data to aon egister safely
    * @param    offset: register address
    * @param    mask:   indicate which bit in data will be updated
    * @param    data:   data to be writen to register
    * @return
    */
extern void btaon_fast_update_safe(uint16_t offset, uint16_t mask, uint16_t data);
extern void btaon_fast_update_safe_8b(uint16_t offset, uint8_t mask, uint8_t data);

/** @} */ /* End of RTL876X_Exported_Functions */

/** @} */ /* End of group RTL876X */

#ifdef __cplusplus
}
#endif
#endif  /* RTL876X_H */

