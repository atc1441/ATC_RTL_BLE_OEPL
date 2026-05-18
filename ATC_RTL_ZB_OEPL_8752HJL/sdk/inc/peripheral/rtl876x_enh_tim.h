/**
*********************************************************************************************************
*               Copyright(c) 2024, Realtek Semiconductor Corporation. All rights reserved.
*********************************************************************************************************
* \file     rtl876x_enh_tim.h
* \brief    The header file of the peripheral Enhance ENHTIMER driver.
* \details  This file provides all Enhance ENHTIMER firmware functions.
* \author   Yuan
* \date     2024-01-17
* \version  v1.0.0
* *********************************************************************************************************
*/

#ifndef _RTL876X_ENH_TIM_H_
#define _RTL876X_ENH_TIM_H_

#ifdef __cplusplus
extern "C" {
#endif


/**
 * \addtogroup  IO          Peripheral Drivers
 * \defgroup    ENHTIM      ENHTIM
 * \brief       Manage the ENHTIM peripheral functions.
 *
 * \ingroup     IO
 */

/*============================================================================*
 *                         Includes
 *============================================================================*/
#include "rtl876x.h"

/*============================================================================*
 *                         Constants
 *============================================================================*/

/* ENHTIM private defines */
#define ENHTIM_PWM_DEADZONE_CR  *((volatile uint32_t *)0x40000368UL)
#define ENHTIM_LATCH_COUNT_CR   *((volatile uint32_t *)0x40006028UL)

/**
 * \defgroup    ENHTIM_Exported_Constants  ENHTIM Exported Constants
 *
 * \ingroup     ENHTIM
 */
#define IS_ENHTIM_ALL_PERIPH(PERIPH) (((PERIPH) == ENH_TIM0) || \
                                      ((PERIPH) == ENH_TIM1))

/**
 * \defgroup    ENHTIM_Clock_Source ENHTIM Clock Source
 * \{
 * \ingroup     ENHTIM_Exported_Constants
 */
#define ENHTIM_TIMER_TOGGLE_OUTPUT          ((uint16_t)0x1)
#define ENHTIM_DIVIDER_CLOCK                ((uint16_t)0x0)

#define IS_ENHTIM_CLOCK_SOURCE(src) (((src) == ENHTIM_TIMER_TOGGLE_OUTPUT) || \
                                     ((src) == ENHTIM_DIVIDER_CLOCK))
/** \} */

/**
 * \defgroup    ENHTIM_Latch_En ENHTIM Latch Count Enable
 * \{
 * \ingroup     ENHTIM_Exported_Constants
 */
#define ENHTIM_LATCH_COUNT_ENABLE          ((uint16_t)0x1)
#define ENHTIM_LATCH_COUNT_DISABLE         ((uint16_t)0x0)

#define IS_ENHTIM_LATCH_COUNT_En(mode) (((mode) == ENHTIM_LATCH_COUNT_ENABLE) || \
                                        ((mode) == ENHTIM_LATCH_COUNT_DISABLE))
/** \} */

/**
 * \defgroup    ENHTIM_Latch_Trigger_Mode ENHTIM Latch Count Trigger Mode
 * \{
 * \ingroup     ENHTIM_Exported_Constants
 */
#define ENHTIM_LATCH_TRIGGER_BOTH_EDGE      ((uint16_t)0x02)
#define ENHTIM_LATCH_TRIGGER_FALLING_EDGE   ((uint16_t)0x01)
#define ENHTIM_LATCH_TRIGGER_RISING_EDGE    ((uint16_t)0x00)

#define IS_ENHTIM_Latch_Trigger_Mode(mode) (((mode) == ENHTIM_LATCH_TRIGGER_BOTH_EDGE) || \
                                            ((mode) == ENHTIM_LATCH_TRIGGER_FALLING_EDGE) || \
                                            ((mode) == ENHTIM_LATCH_TRIGGER_RISING_EDGE))
/** \} */

/**
 * \defgroup    ENHTIM_PWM_En ENHTIM PWM Output Enable
 * \{
 * \ingroup     ENHTIM_Exported_Constants
 */
#define ENHTIM_PWM_ENABLE                   ((uint16_t)0x08)
#define ENHTIM_PWM_DISABLE                  ((uint16_t)0x00)

#define IS_ENHTIM_PWM_En(mode) (((mode) == ENHTIM_PWM_ENABLE) || \
                                ((mode) == ENHTIM_PWM_DISABLE))
/** \} */

/**
 * \defgroup    ENHTIM_PWM_Polarity ENHTIM PWM Polarity
 * \{
 * \ingroup     ENHTIM_Exported_Constants
 */
#define ENHTIM_PWM_START_WITH_HIGH          ((uint16_t)0x04)
#define ENHTIM_PWM_START_WITH_LOW           ((uint16_t)0x00)

#define IS_ENHTIM_PWM_POLARITY(pola) (((pola) == ENHTIM_PWM_START_WITH_HIGH) || \
                                      ((pola) == ENHTIM_PWM_START_WITH_LOW))
/** \} */

/**
 * \defgroup    ENHTIM_Mode ENHTIM Mode
 * \{
 * \ingroup     ENHTIM_Exported_Constants
 */
#define ENHTIM_MODE_PWM_MANUAL              ((uint16_t)0x02) /*!< User define pwm manual mode. */
#define ENHTIM_MODE_PWM_AUTO                ((uint16_t)0x01) /*!< User define pwm auto mode. */
#define ENHTIM_MODE_FreeRun                 ((uint16_t)0x00) /*!< Freerun mode. */

#define IS_ENHTIM_MODE(mode) (((mode) == ENHTIM_MODE_PWM_MANUAL) || \
                              ((mode) == ENHTIM_MODE_PWM_AUTO) || \
                              ((mode) == ENHTIM_MODE_FreeRun))
/** \} */

/**
 * \defgroup    ENHTIM_Interrupts_Definition ENHTIM Interrupts Definition
 * \{
 * \ingroup     ENHTIM_Exported_Constants
 */
#define ENHTIM_INT_TIM                      (0x00)
#define ENHTIM_INT_LATCH_CNT2_FIFO_EMPTY    (0x01)
#define ENHTIM_INT_LATCH_CNT2_FIFO_FULL     (0x40)
#define ENHTIM_INT_LATCH_CNT2_FIFO_THD      (0x42)

#define IS_ENHTIM_INT(INT) (((INT) == ENHTIM_INT_TIM) || \
                            ((INT) == ENHTIM_INT_LATCH_CNT2_FIFO_FULL) || \
                            ((INT) == ENHTIM_INT_LATCH_CNT2_FIFO_THD))
/** \} */

/**
 * \defgroup    ENHTIM_FIFO_Flag ENHTIM FIFO Flag
 * \{
 * \ingroup     ENHTIM_Exported_Constants
 */
#define ENHTIM_FLAG_TIM1_LC_FIFO_EMPTY          BIT(19)
#define ENHTIM_FLAG_TIM0_LC_FIFO_EMPTY          BIT(17)
#define ENHTIM_FLAG_TIM1_CCR_FIFO_EMPTY         BIT(3)
#define ENHTIM_FLAG_TIM1_CCR_FIFO_FULL          BIT(2)
#define ENHTIM_FLAG_TIM0_CCR_FIFO_EMPTY         BIT(1)
#define ENHTIM_FLAG_TIM0_CCR_FIFO_FULL          BIT(0)

#define IS_ENHTIM_CCR_FIFO_FLAG(flag) (((flag) == ENHTIM_FLAG_TIM1_LC_FIFO_EMPTY) || \
                                       ((flag) == ENHTIM_FLAG_TIM0_LC_FIFO_EMPTY) || \
                                       ((flag) == ENHTIM_FLAG_TIM1_CCR_FIFO_EMPTY) || \
                                       ((flag) == ENHTIM_FLAG_TIM1_CCR_FIFO_FULL) || \
                                       ((flag) == ENHTIM_FLAG_TIM0_CCR_FIFO_EMPTY) || \
                                       ((flag) == ENHTIM_FLAG_TIM0_CCR_FIFO_FULL))
/** \} */

/**
 * \defgroup    ENHTIM_PWM_DeadZone_En ENHTIM PWM DeadZone Enable
 * \{
 * \ingroup     ENHTIM_Exported_Constants
 */
#define ENHTIM_PWM_DEADZONE_ENABLE          ((uint16_t)0x1)
#define ENHTIM_PWM_DEADZONE_DISABLE         ((uint16_t)0x0)

#define IS_ENHTIM_PWM_DEADZONE_EN(mode) (((mode) == ENHTIM_PWM_DEADZONE_ENABLE) || \
                                         ((mode) == ENHTIM_PWM_DEADZONE_DISABLE))
/** \} */

/**
 * \defgroup    ENHTIM_PWM_DeadZone_Clock ENHTIM PWM DeadZone Clock
 * \{
 * \ingroup     ENHTIM_Exported_Constants
 */
#define ENHTIM_PWM_DZCLKSRCE_ENHTIM         ((uint32_t)0x80000)
#define ENHTIM_PWM_DZCLKSRCE_5M             ((uint32_t)0x10000)
#define ENHTIM_PWM_DZCLKSRCE_32K            ((uint32_t)0x0)

#define IS_ENHTIM_PWM_DEADZONE_Clock(mode) (((mode) == ENHTIM_PWM_DEADZONE_ENABLE) || \
                                            ((mode) == ENHTIM_PWM_DEADZONE_DISABLE))
/** \} */

/**
 * \defgroup    ENHTIM_PWM_DeadZone_Stop_State ENHTIM PWM DeadZone Stop State
 * \{
 * \ingroup     ENHTIM_Exported_Constants
 */
#define ENHTIM_PWM_STOP_AT_HIGH             ((uint16_t)0x1)
#define ENHTIM_PWM_STOP_AT_LOW              ((uint16_t)0x0)
/** \} */

/**
 * \defgroup    ENHTIM_FIFO_Clear_Flag ENHTIM FIFO Clear Flag
 * \{
 * \ingroup     ENHTIM_Exported_Constants
 */
#define ENHTIM_FIFO_CLR_CCR                 (0)
#define ENHTIM_FIFO_CLR_CNT2                (24)
/** \} */

/**
 * \defgroup    ENHTIM_Clock_Divider ENHTIM Clock Divider
 * \{
 * \ingroup     ENHTIM_Exported_Constants
 */
typedef enum
{
    ENHTIM_CLOCK_DIVIDER_1   = 0x00,
    ENHTIM_CLOCK_DIVIDER_125 = 0x03,
    ENHTIM_CLOCK_DIVIDER_2   = 0x04,
    ENHTIM_CLOCK_DIVIDER_4   = 0x05,
    ENHTIM_CLOCK_DIVIDER_8   = 0x06,
    ENHTIM_CLOCK_DIVIDER_40  = 0x07,
} E_ENHTIM_CLKDIV;
/** \} */

/**
 * \defgroup    ENHTIM_Latch_Counter ENHTIM Latch Counter
 * \{
 * \ingroup     ENHTIM_Exported_Constants
 */
typedef enum
{
    LATCH_CNT_0 = 0,
    LATCH_CNT_1 = 1,
    LATCH_CNT_2 = 2,
} E_ENHTIM_LATCHCNT;
/** \} */

/** End of ENHTIM_Exported_Constants
  * \}
  */

/*============================================================================*
 *                         Types
 *============================================================================*/
/**
 * \defgroup    ENHTIM_Exported_Types ENHTIM Exported Types
 * \{
 * \ingroup     ENHTIM
 */

/**
 * \brief       ENHTIM init structure definition
 *
 * \ingroup     ENHTIM_Exported_Types
 */
typedef struct
{
    uint16_t ENHTIM_ClockSource;            /*!< <b>Deprecated</b> use RCC instead.*/
    E_ENHTIM_CLKDIV ENHTIM_ClockDiv;        /*!< Specify the clock source div.
                                                 This parameter can be a value of \ref ENHTIM_Clock_Divider.*/
    uint16_t ENHTIM_Mode;                   /*!< Specify the counter mode.
                                                 This parameter can be a value of \ref ENHTIM_Mode. */
    uint16_t ENHTIM_PWMOutputEn;            /*!< Enable or disable the PWM mode.
                                                 This parameter can be a value of \ref ENHTIM_PWM_En. */
    uint16_t ENHTIM_PWMStartPolarity;       /*!< Specify the PWM start polarity.
                                                 This parameter can be a value of \ref ENHTIM_PWM_Polarity. */
    uint16_t ENHTIM_LatchCountEn[3];        /*!< Enable or disable Enhtimer Latch_cnt.
                                                 This parameter can be a value of DISABLE or ENABLE. */
    uint16_t ENHTIM_LatchCountTrigger[3];   /*!< Specify Enhtimer counter latch trigger mode.
                                                 This parameter can be a value of \ref ENHTIM_Latch_Trigger_Mode. */
    uint16_t ENHTIM_LatchCount2Thd;         /*!< Specify Enhtimer latched counter fifo threshold.
                                                 This parameter can be a value of 0~4. */
    uint16_t ENHTIM_LatchTriggerPad;        /*!< Specify the Enhtimer latch trigger Pad.
                                                 This parameter can be a value of P0_0 to P5_2. */
    uint16_t ENHTIM_TimerGPIOTriggerEn;     /*!< Enable or disable acc latch.
                                                 This parameter can be a value of DISABLE or ENABLE. */
    uint16_t ENHTIM_BTGPIOTriggerEn;        /*!< Enable or disable BT latch.
                                                 This parameter can be a value of DISABLE or ENABLE. */
    uint32_t ENHTIM_MaxCount;               /*!< Specify the Enhtimer max counter value for user-defined PWM mode.
                                                 This parameter legal value range is from 0 ~ 2^32-2. */
    uint32_t ENHTIM_CCValue;                /*!< Specify the Enhtimer capture/compare value for user-defined PWM manual mode.
                                                 This parameter legal value range is from 0 ~ 2^32-2.*/
    uint16_t ENHTIM_PWMDeadZoneEn;          /*!< Enable or disable PWM Deadzone, pwm0_pn: timer2, pwm1_pn:timer3.
                                                 This parameter can be a value of ENABLE or DISABLE. */
    uint32_t ENHTIM_PWMDeadZoneClockSource; /*!< Specify the pwm deadzone clock source.
                                                 This parameter can be a value of \ref ENHTIM_PWM_DeadZone_Clock. */
    uint16_t ENHTIM_PWMStopStateP;          /*!< Specify the PWM P stop state.
                                                 This parameter can be a value of \ref PWMDeadZone_Stop_State. */
    uint16_t ENHTIM_PWMStopStateN;          /*!< Specify the PWM N stop state.
                                                 This parameter can be a value of \ref PWMDeadZone_Stop_State. */
    uint32_t ENHTIM_DeadZoneSize;           /*!< Specify the size of deadzone time, deadzone Time = ENHTIM_DeadZoneSize/32000(internal clock src) or 32768(external clock src).
                                                 This parameter must range 0x1 ~ 0xFF. */
} ENHTIM_InitTypeDef;

/** End of ENHTIM_Exported_Types
  * \}
  */

/*============================================================================*
 *                         Functions
 *============================================================================*/
/**
 * \defgroup    ENHTIM_Exported_Functions  ENHTIM Exported Functions
 * \{
 * \ingroup     ENHTIM
 */

/**
 * \brief     Initialize the ENHTIMx unit peripheral according to
 *            the specified parameters in ENHTIM_TimeBaseInitStruct.
 * \param[in] ENHTIMx: Where x can be 0 to 1 to select the ENHTIM peripheral.
 * \param[in] ENHTIM_TimeBaseInitStruct: pointer to a ENHTIM_InitTypeDef structure
  *           that contains the configuration information for the specified ENHTIM peripheral.
 * \return  None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void driver_enhance_timer_init(void)
 * {
 *     RCC_PeriphClockCmd(APBPeriph_ENHTIMER, APBPeriph_ENHTIMER_CLOCK, ENABLE);
 *
 *     ENHTIM_InitTypeDef ENHTIM_InitStruct;
 *     ENHTIM_StructInit(&ENHTIM_InitStruct);
 *
 *     ENHTIM_InitStruct.ENHTIM_PWMOutputEn = ENHTIM_PWM_DISABLE;
 *     ENHTIM_InitStruct.ENHTIM_MaxCount = 4000;
 *     ENHTIM_InitStruct.ENHTIM_CCValue  = 2000;
 *     ENHTIM_InitStruct.ENHTIM_Mode = ENHTIM_MODE_PWM_MANUAL;
 *     ENHTIM_Init(ENHTIMER_NUM, &ENHTIM_InitStruct);
 * }
 * \endcode
 */
void ENHTIM_Init(ENHTIM_TypeDef *ENHTIMx, ENHTIM_InitTypeDef *ENHTIM_TimeBaseInitStruct);

/**
 * \brief   Fill each ENHTIM_InitStruct member with its default value.
 * \param[in] ENHTIM_InitStruct: Pointer to a ENHTIM_InitTypeDef structure which will be initialized.
 * \return  None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void driver_enhance_timer_init(void)
 * {
 *     RCC_PeriphClockCmd(APBPeriph_ENHTIMER, APBPeriph_ENHTIMER_CLOCK, ENABLE);
 *
 *     ENHTIM_InitTypeDef ENHTIM_InitStruct;
 *     ENHTIM_StructInit(&ENHTIM_InitStruct);
 *
 *     ENHTIM_InitStruct.ENHTIM_PWMOutputEn = ENHTIM_PWM_ENABLE;
 *     ENHTIM_InitStruct.ENHTIM_MaxCount = 4000;
 *     ENHTIM_InitStruct.ENHTIM_CCValue  = 2000;
 *     ENHTIM_InitStruct.ENHTIM_Mode = ENHTIM_MODE_PWM_MANUAL;
 *     ENHTIM_Init(ENHTIMER_NUM, &ENHTIM_InitStruct);
 * }
 * \endcode
 */
void ENHTIM_StructInit(ENHTIM_InitTypeDef *ENHTIM_InitStruct);

/**
 * \brief     Enable or disable the specified ENHTIM peripheral.
 * \param[in] ENHTIMx: Where x can be 0 to 1 to select the ENHTIMx peripheral.
 * \param[in] NewState: New state of the ENHTIMx peripheral.
 *            This parameter can be: ENABLE or DISABLE.
 * \return    None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void driver_enhance_timer_init(void)
 * {
 *     RCC_PeriphClockCmd(APBPeriph_ENHTIMER, APBPeriph_ENHTIMER_CLOCK, ENABLE);
 *
 *     ENHTIM_InitTypeDef ENHTIM_InitStruct;
 *     ENHTIM_StructInit(&ENHTIM_InitStruct);
 *
 *     ENHTIM_InitStruct.ENHTIM_PWMOutputEn = ENHTIM_PWM_ENABLE;
 *     ENHTIM_InitStruct.ENHTIM_MaxCount = 4000;
 *     ENHTIM_InitStruct.ENHTIM_CCValue  = 2000;
 *     ENHTIM_InitStruct.ENHTIM_Mode = ENHTIM_MODE_PWM_MANUAL;
 *     ENHTIM_Init(ENHTIMER_NUM, &ENHTIM_InitStruct);
 *     ENHTIM_Cmd(ENHTIMER_NUM, ENABLE);
 * }
 * \endcode
 */
void ENHTIM_Cmd(ENHTIM_TypeDef *ENHTIMx, FunctionalState NewState);

/**
  * \brief     Mask or unmask the latch count2 fifo interrupt.
  * \param[in] ENHTIMx: Where x can be 0 to 1 to select the ENHTIMx peripheral.
  * \param[in] NewState: New state of the specified ENHTIMx peripheral latch count2 fifo interrupt.
  *            This parameter can be: ENABLE or DISABLE.
  * \return None.
  */
void ENHTIM_LCFIFOMaskConfig(ENHTIM_TypeDef *ENHTIMx, FunctionalState NewState);

/**
 * \brief     Enable or disable ENHTIMx interrupt.
 * \param[in] ENHTIMx: Where x can be 0 to 1 to select the ENHTIMx peripheral.
 * \param[in] ENHTIM_INT: Specify the ENHTIMx interrupt source which is to be enabled or disabled.
 *            This parameter can be one of the following values, which refer to \ref ENHTIM_Interrupts_Definition.
 *            \arg ENHTIM_INT_TIM: Enhance Timer interrupt source.
 *            \arg ENHTIM_INT_LATCH_CNT2_FIFO_FULL: Enhance Timer latch count2 fifo full interrupt source.
 *            \arg ENHTIM_INT_LATCH_CNT2_FIFO_EMPTY: Enhance Timer latch count2 fifo empty interrupt source.
 *            \arg ENHTIM_INT_LATCH_CNT2_FIFO_THD: Enhance Timer latch count2 fifo threshold interrupt source.
 * \param[in] NewState: New state of the ENHTIMx peripheral.
 *            This parameter can be: ENABLE or DISABLE.
 * \return    None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void driver_enhance_timer_init(void)
 * {
 *     RCC_PeriphClockCmd(APBPeriph_ENHTIMER, APBPeriph_ENHTIMER_CLOCK, ENABLE);
 *
 *     ENHTIM_InitTypeDef ENHTIM_InitStruct;
 *     ENHTIM_StructInit(&ENHTIM_InitStruct);
 *
 *     ENHTIM_InitStruct.ENHTIM_ClockDiv               = ENHTIM_CLOCK_DIVIDER_1;
 *     ENHTIM_InitStruct.ENHTIM_Mode                   = ENHTIM_MODE_FreeRun;
 *     ENHTIM_InitStruct.ENHTIM_LatchCountEn[2]        = ENHTIM_LATCH_COUNT_ENABLE;
 *     ENHTIM_InitStruct.ENHTIM_LatchCountTrigger[2]   = ENHTIM_LATCH_TRIGGER_RISING_EDGE;
 *     ENHTIM_InitStruct.ENHTIM_LatchCount2Thd         = 3;
 *     ENHTIM_InitStruct.ENHTIM_LatchTriggerPad        = GPIO_INPUT_PIN_0;
 *     ENHTIM_Init(ENHTIMER_NUM, &ENHTIM_InitStruct);
 *     ENHTIM_ClearINTPendingBit(ENHTIMER_NUM, ENHTIM_INT_LATCH_CNT2_FIFO_THD);
 *     ENHTIM_INTConfig(ENHTIMER_NUM, ENHTIM_INT_LATCH_CNT2_FIFO_THD, ENABLE);
 * }
 * \endcode
 */
void ENHTIM_INTConfig(ENHTIM_TypeDef *ENHTIMx, uint8_t ENHTIM_INT, FunctionalState NewState);

/**
 * \brief     Read ENHTIMx latch counter2 fifo data.
 * \param[in] ENHTIMx: Where x can be 0 to 1 to select the ENHTIMx peripheral.
 * \param[in] length: Latch count2 fifo length, max 4.
 * \pBuf[out] pBuf: FIFO data out buffer.
 * \return    None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void enhance_timer_demo(void)
 * {
 *     uint8_t length = ENHTIM_GetLatchCount2FIFOLength(ENH_TIM0);
 *     uint32_t data[4] = {0};
 *     ENHTIM_ReadLatchCount2FIFO(ENH_TIM0, data, length);
 * }
 * \endcode
 */
void ENHTIM_ReadLatchCount2FIFO(ENHTIM_TypeDef *ENHTIMx, uint32_t *pBuf, uint8_t length);

/**
 * \brief     Check whether the ENHTIM latch count2 fifo interrupt has occurred or not.
 * \param[in] ENHTIMx: Where x can be 0 to 1 to select the ENHTIMx peripheral.
 * \return    The new state of the specified ENHTIMx peripheral
 *            latch count2 fifo interrupt (SET or RESET).
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void enhance_timer_demo(void)
 * {
 *     ITStatus int_status = ENHTIM_GetLCMaskINTStatus(ENH_TIM0);
 * }
 * \endcode
 */
ITStatus ENHTIM_GetLCFIFOMaskStatus(ENHTIM_TypeDef *ENHTIMx);

/**
 * \brief     Check whether the ENHTIM interrupt has occurred or not.
 * \param[in] ENHTIMx: Where x can be 0 to 1 to select the ENHTIMx peripheral.
 * \param[in] ENHTIM_INT: Specify the ENHTIMx interrupt source which is to be enabled or disabled.
 *            This parameter can be one of the following values, which refer to \ref ENHTIM_Interrupts_Definition.
 *            \arg ENHTIM_INT_TIM: Enhance Timer interrupt source.
 *            \arg ENHTIM_INT_LATCH_CNT2_FIFO_FULL: Enhance Timer latch count2 fifo full interrupt source.
 *            \arg ENHTIM_INT_LATCH_CNT2_FIFO_EMPTY: Enhance Timer latch count2 fifo empty interrupt source.
 *            \arg ENHTIM_INT_LATCH_CNT2_FIFO_THD: Enhance Timer latch count2 fifo threshold interrupt source.
 * \return    The new state of the ENHTIM_INT (SET or RESET).
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void enhance_timer_demo(void)
 * {
 *     ENHTIM_GetINTStatus(ENH_TIM0, ENHTIM_INT_TIM);
 * }
 * \endcode
 */
ITStatus ENHTIM_GetINTStatus(ENHTIM_TypeDef *ENHTIMx, uint8_t ENHTIM_INT);

/**
 * \brief     Clear ENHTIMx interrupt.
 * \param[in] ENHTIMx: Where x can be 0 to 1 to select the ENHTIMx peripheral.
 * \param[in] ENHTIM_INT: Specify the ENHTIMx interrupt source which is to be enabled or disabled.
 *            This parameter can be one of the following values, which refer to \ref ENHTIM_Interrupts_Definition.
 *            \arg ENHTIM_INT_TIM: Enhance Timer interrupt source.
 *            \arg ENHTIM_INT_LATCH_CNT2_FIFO_FULL: Enhance Timer latch count2 fifo full interrupt source.
 *            \arg ENHTIM_INT_LATCH_CNT2_FIFO_EMPTY: Enhance Timer latch count2 fifo empty interrupt source.
 *            \arg ENHTIM_INT_LATCH_CNT2_FIFO_THD: Enhance Timer latch count2 fifo threshold interrupt source.
 * \return    None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void enhance_timer_demo(void)
 * {
 *     ENHTIM_ClearINTPendingBit(ENH_TIM0, ENHTIM_INT_TIM);
 * }
 * \endcode
 */
void ENHTIM_ClearINTPendingBit(ENHTIM_TypeDef *ENHTIMx, uint8_t ENHTIM_INT);

/**
 * \brief     Get ENHTIMx current value when timer is running.
 * \param[in] ENHTIMx: Where x can be 0 to 1 to select the ENHTIMx peripheral.
 * \return    The counter value.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void enhance_timer_demo(void)
 * {
 *     uint32_t cur_value = ENHTIM_GetCurrentValue(ENH_TIM0);
 * }
 * \endcode
 */
__STATIC_INLINE uint32_t ENHTIM_GetCurrentCount(ENHTIM_TypeDef *ENHTIMx)
{
    /* Check the parameters */
    assert_param(IS_ENHTIM_ALL_PERIPH(ENHTIMx));

    return ENHTIMx->CUR_CNT;
}

/**
 * \brief     Set Max Count value.
 * \param[in] ENHTIMx: Where x can be 0 to 1 to select the ENHTIMx peripheral.
 * \param[in] count: This parameter legal value range is from 0 ~ 2^32-2.
 * \return    None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void enhance_timer_demo(void)
 * {
 *     ENHTIM_SetMaxCount(ENH_TIM0, 0x10000);
 * }
 * \endcode
 */
__STATIC_INLINE void ENHTIM_SetMaxCount(ENHTIM_TypeDef *ENHTIMx, uint32_t count)
{
    /* Check the parameters */
    assert_param(IS_ENHTIM_ALL_PERIPH(ENHTIMx));

    ENHTIMx->MAX_CNT = count & 0xFFFFFFFE;
}

/**
 * \brief     Set ENHTIMx capture/compare value for user-defined manual mode.
 * \param[in] ENHTIMx: Where x can be 0 to 1 to select the ENHTIMx peripheral.
 * \param[in] value: This parameter legal value range is from 0 ~ 2^32-2.
 * \return    None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void enhance_timer_demo(void)
 * {
 *     ENHTIM_SetCCValue(ENH_TIM0, 0x1000);
 * }
 * \endcode
 */
__STATIC_INLINE void ENHTIM_SetCCValue(ENHTIM_TypeDef *ENHTIMx, uint32_t value)
{
    /* Check the parameters */
    assert_param(IS_ENHTIM_ALL_PERIPH(ENHTIMx));

    ENHTIMx->CCR = value;
}

/**
 * \brief     Write value to ENHTIM CCR FIFO.
 * \param[in] ENHTIMx: Where x can be 0 to 1 to select the ENHTIMx peripheral.
 * \param[in] value: This parameter legal value range is from 0 ~ 2^32-2.
 * \return    None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void enhance_timer_demo(void)
 * {
 *     ENHTIM_WriteCCFIFO(ENH_TIM0, 0x10000);
 * }
 * \endcode
 */
__STATIC_INLINE void ENHTIM_WriteCCFIFO(ENHTIM_TypeDef *ENHTIMx, uint32_t value)
{
    /* Check the parameters */
    assert_param(IS_ENHTIM_ALL_PERIPH(ENHTIMx));

    ENHTIMx->CCR_FIFO = value;
}

/**
 * \brief     Check whether the specified ENHTIM CCR FIFO flag is set.
 * \param[in] ENHTIM_FLAG: Specify the flag to check.
 *            This parameter can be one of the following values, which refer to \ref ENHTIM_FIFO_Flag.
 *            \arg ENHTIM_FLAG_TIM1_LC_FIFO_EMPTY: FIFO empty or not. If SET, CCR FIFO is empty.
 *            \arg ENHTIM_FLAG_TIM0_LC_FIFO_EMPTY: FIFO full or not. If SET, CCR FIFO is full.
 *            \arg ENHTIM_FLAG_TIM1_CCR_FIFO_EMPTY: FIFO empty or not. If SET, CCR FIFO is empty.
 *            \arg ENHTIM_FLAG_TIM1_CCR_FIFO_FULL: FIFO full or not. If SET, CCR FIFO is full.
 *            \arg ENHTIM_FLAG_TIM0_CCR_FIFO_EMPTY: FIFO empty or not. If SET, CCR FIFO is empty.
 *            \arg ENHTIM_FLAG_TIM0_CCR_FIFO_FULL: FIFO full or not. If SET, CCR FIFO is full.
 * \return    The new state of ENHTIM_FLAG (SET or RESET).
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void enhance_timer_demo(void)
 * {
 *     FlagStatus flag_status = ENHTIM_GetCCRFIFOFlagStatus(ENHTIM_FLAG_FIFO_EMPTY);
 * }
 * \endcode
 */
__STATIC_INLINE FlagStatus ENHTIM_GetFIFOFlagStatus(uint32_t ENHTIM_FLAG)
{
    /* Check the parameters */
    assert_param(IS_ENHTIM_FIFO_FLAG(ENHTIM_FLAG));

    FlagStatus bitstatus = RESET;

    if (ENH_TIM_SHARE->FIFO_SR2 & ENHTIM_FLAG)
    {
        bitstatus = SET;
    }

    return bitstatus;
}

/**
 * \brief     Enable ENHTIMx latch counter.
 * \param[in] ENHTIMx: Where x can be 0 to 1 to select the ENHTIMx peripheral.
 * \param[in] LatchCntIdx: E_ENHTIM_LATCHCNT enum value.
 *            This parameter can be one of the following.
 *            \arg LATCH_COUNT_0: Enhance timer latch count 0.
 *            \arg LATCH_COUNT_1: Enhance timer latch count 1.
 *            \arg LATCH_COUNT_2: Enhance timer latch count 2.
 * \return    None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void enhance_timer_demo(void)
 * {
 *     uint32_t cur_value = ENHTIM_LatchCountEnable(ENH_TIM0, LATCH_COUNT_2);
 * }
 * \endcode
 */
__STATIC_INLINE void ENHTIM_LatchCountEnable(ENHTIM_TypeDef *ENHTIMx, E_ENHTIM_LATCHCNT LatchCntIdx)
{
    /* Check the parameters */
    assert_param(IS_ENHTIM_ALL_PERIPH(ENHTIMx));

    ENHTIMx->CR |= BIT(LatchCntIdx + 10);
}

/**
 * \brief     Disable ENHTIMx latch counter.
 * \param[in] ENHTIMx: where x can be 0 to 1 to select the ENHTIMx peripheral.
 * \param[in] LatchCntIdx: E_ENHTIM_LATCHCNT enum value.
 *            This parameter can be one of the following.
 *            \arg LATCH_COUNT_0: Enhance timer latch count 0.
 *            \arg LATCH_COUNT_1: Enhance timer latch count 1.
 *            \arg LATCH_COUNT_2: Enhance timer latch count 2.
 * \return    None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void enhance_timer_demo(void)
 * {
 *     uint32_t cur_value = ENHTIM_LatchCountDisable(ENH_TIM0, LATCH_COUNT_2);
 * }
 * \endcode
 */
__STATIC_INLINE void ENHTIM_LatchCountDisable(ENHTIM_TypeDef *ENHTIMx,
                                              E_ENHTIM_LATCHCNT LatchCntIdx)
{
    /* Check the parameters */
    assert_param(IS_ENHTIM_ALL_PERIPH(ENHTIMx));

    ENHTIMx->CR &= ~BIT(LatchCntIdx + 10);
}

/**
 * \brief     Get the value of ENHTIMx latch counter.
 * \param[in] ENHTIMx: Where x can be 0 to 1 to select the ENHTIMx peripheral.
 * \param[in] LatchCntIdx: E_ENHTIM_LATCHCNT enum value.
 *            This parameter can be one of the following.
 *            \arg LATCH_COUNT_0: Enhance timer latch count 0.
 *            \arg LATCH_COUNT_1: Enhance timer latch count 1.
 *            \arg LATCH_COUNT_2: Enhance timer latch count 2.
 * \return    The latch counter value.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void enhance_timer_demo(void)
 * {
 *     uint32_t count_value = ENHTIM_GetLatchCountValue(ENH_TIM0, LATCH_COUNT_0);
 * }
 * \endcode
 */
__STATIC_INLINE uint32_t ENHTIM_GetLatchCount(ENHTIM_TypeDef *ENHTIMx,
                                              E_ENHTIM_LATCHCNT LatchCntIdx)
{
    /* Check the parameters */
    assert_param(IS_ENHTIM_ALL_PERIPH(ENHTIMx));

    uint32_t count = 0;
    count = *(volatile uint32_t *)(&(ENHTIMx->LATCH_CNT0) + LatchCntIdx);
    return count;
}

/**
 * \brief     Get the fifo length of ENHTIMx latch counter.
 * \param[in] ENHTIMx: where x can be 0 to 1 to select the ENHTIMx peripheral.
 * \return    FIFO data length.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void enhance_timer_demo(void)
 * {
 *     uint8_t length = ENHTIM_GetLatchCount2FIFOLength(ENH_TIM0);
 * }
 * \endcode
 */
__STATIC_INLINE uint8_t ENHTIM_GetLatchCount2FIFOLength(ENHTIM_TypeDef *ENHTIMx)
{
    /* Check the parameters */
    assert_param(IS_ENHTIM_ALL_PERIPH(ENHTIMx));

    uint32_t enhtim_id = ((uint32_t)ENHTIMx - (uint32_t)ENH_TIM0) / 0X24;

    return (uint8_t)(((*((volatile uint32_t *)(&(ENH_TIM_SHARE->LC_FIFO_LEVEL0) + enhtim_id))) >> 16) &
                     0x1F);
}

/**
 * \brief     Clear capture/compare or latch count2 fifo.
 * \param[in] ENHTIMx: Where x can be 0 to 1 to select the ENHTIMx peripheral.
 * \param[in] FIFO_CLR: Specify the FIFO type which to be cleared.
 *            This parameter can be one of the following values, which refer to \ref ENHTIM_FIFO_Clear_Flag.
 *            \arg ENHTIM_FIFO_CLR_CCR: Enhance Timer CCR FIFO clear flag.
 *            \arg ENHTIM_FIFO_CLR_CNT2: Enhance Timer latch count2 FIFO clear flag.
 * \return    None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void enhance_timer_demo(void)
 * {
 *     ENHTIM_ClearFIFO(ENH_TIM0, ENHTIM_FIFO_CLR_CCR);
 * }
 * \endcode
 */
__STATIC_INLINE void ENHTIM_ClearFIFO(ENHTIM_TypeDef *ENHTIMx, uint8_t FIFO_CLR)
{
    /* Check the parameters */
    assert_param(IS_ENHTIM_ALL_PERIPH(ENHTIMx));

    uint32_t enhtim_id = ((uint32_t)ENHTIMx - (uint32_t)ENH_TIM0) / 0X24;

    ENH_TIM_SHARE->FIFO_CLR |= (BIT(FIFO_CLR + enhtim_id));
}

/**
 * \brief   ENHTIM PWM complementary output emergency stop.
 * \return  None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void enhance_timer_demo(void)
 * {
 *     ENHTIM_PWMDeadZoneEMStop();
 * }
 * \endcode
 */
__STATIC_INLINE void ENHTIM_PWMDeadZoneEMStop(void)
{
    ENHTIM_PWM_DEADZONE_CR |= BIT(8);
}

/**
 * \brief     Get ENHTIM PWM output status.
 * \param[in] ENHTIMx: Where x can be 0 to 1 to select the ENHTIMx peripheral.
 * \return    ENHTIM PWM output status (SET or RESET).
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void enhance_timer_demo(void)
 * {
 *     FlagStatus status = ENHTIM_GetPWMOutputState(ENH_TIM0);
 * }
 * \endcode
 */
__STATIC_INLINE FlagStatus ENHTIM_GetPWMOutputState(ENHTIM_TypeDef *ENHTIMx)
{
    /* Check the parameters */
    assert_param(IS_ENHTIM_ALL_PERIPH(ENHTIMx));

    uint32_t enhtim_id = ((uint32_t)ENHTIMx - (uint32_t)ENH_TIM0) / 0X24;
    uint32_t status = ENHTIM_LATCH_COUNT_CR & (0x1 << (enhtim_id + 6));
    return (FlagStatus)(status >> (enhtim_id + 6));
}

/**
 * \brief   Enable or disable bypass dead zone function of PWM complementary output.
 *          After enabling, PWM_P = ~PWM_N.
 * \param[in] ENHTIMx: ENHTIM0.
 * \param[in] NewState: New state of the ENHTIMx peripheral.
 *      \ref DISABLE: Disable bypass dead zone function.
 *      \ref ENABLE: Enable bypass dead zone function.
 * \note    To use this function, need to configure the corresponding enhtimer.
 *          ENHTIMx can be only ENHTIM0.
 * \return  None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void driver_pwm_init(void)
 * {
 *     ENHTIM_PWMDZBypassCmd(ENHTIM0, ENABLE);
 * }
 * \endcode
 */
void ENHTIM_PWMDZBypassCmd(ENHTIM_TypeDef *ENHTIMx, FunctionalState NewState);

/** End of ENHTIM_Exported_Functions
  * \}
  */

/** End of ENHTIM
  * \}
  */

#ifdef __cplusplus
}
#endif

#endif /*_RTL876X_ENH_TIM_H_*/


/******************* (C) COPYRIGHT 2020 Realtek Semiconductor Corporation *****END OF FILE****/
