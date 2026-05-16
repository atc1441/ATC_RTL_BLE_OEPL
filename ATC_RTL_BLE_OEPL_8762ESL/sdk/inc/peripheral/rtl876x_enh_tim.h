/**
*********************************************************************************************************
*               Copyright(c) 2020, Realtek Semiconductor Corporation. All rights reserved.
*********************************************************************************************************
* \file     rtl876x_enh_tim.h
* \brief    The header file of the peripheral Enhance ENHTIMER driver.
* \details  This file provides all Enhance ENHTIMER firmware functions.
* \author   Yuan
* \date     2020-10-19
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
 * \defgroup    ENHTIM_Exported_Constants Macro Definitions
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
/** \} */
#define IS_ENHTIM_CLOCK_SOURCE(src) (((src) == ENHTIM_TIMER_TOGGLE_OUTPUT) || \
                                     ((src) == ENHTIM_DIVIDER_CLOCK))

/**
 * \defgroup    ENHTIM_Latch_Count_En ENHTIM Latch Count Enable
 * \{
 * \ingroup     ENHTIM_Exported_Constants
 */
#define ENHTIM_LATCH_COUNT_ENABLE          ((uint16_t)0x1)
#define ENHTIM_LATCH_COUNT_DISABLE         ((uint16_t)0x0)
/** \} */
#define IS_ENHTIM_LATCH_COUNT_En(mode) (((mode) == ENHTIM_LATCH_COUNT_ENABLE) || \
                                        ((mode) == ENHTIM_LATCH_COUNT_DISABLE))

/**
 * \defgroup    ENHTIM_LATCH_TRIGGER_Mode Latch Count Trigger Mode
 * \{
 * \ingroup     ENHTIM_Exported_Constants
 */
#define ENHTIM_LATCH_TRIGGER_BOTH_EDGE      ((uint16_t)0x02)
#define ENHTIM_LATCH_TRIGGER_FALLING_EDGE   ((uint16_t)0x01)
#define ENHTIM_LATCH_TRIGGER_RISING_EDGE    ((uint16_t)0x00)
/** \} */
#define IS_ENHTIM_LATCH_TRIGGER_Mode(mode) (((mode) == ENHTIM_LATCH_TRIGGER_BOTH_EDGE) || \
                                            ((mode) == ENHTIM_LATCH_TRIGGER_FALLING_EDGE) || \
                                            ((mode) == ENHTIM_LATCH_TRIGGER_RISING_EDGE))

/**
 * \defgroup    ENHTIM_PWM_En ENHTIM PWM Output Enable
 * \{
 * \ingroup     ENHTIM_Exported_Constants
 */
#define ENHTIM_PWM_ENABLE                   ((uint16_t)0x08)
#define ENHTIM_PWM_DISABLE                  ((uint16_t)0x00)
/** \} */
#define IS_ENHTIM_PWM_En(mode) (((mode) == ENHTIM_PWM_ENABLE) || \
                                ((mode) == ENHTIM_PWM_DISABLE))

/**
 * \defgroup    ENHTIM_PWM_Polarity ENHTIM PWM Polarity
 * \{
 * \ingroup     ENHTIM_Exported_Constants
 */
#define ENHTIM_PWM_START_WITH_HIGH          ((uint16_t)0x04)
#define ENHTIM_PWM_START_WITH_LOW           ((uint16_t)0x00)
/** \} */
#define IS_ENHTIM_PWM_POLARITY(pola) (((pola) == ENHTIM_PWM_START_WITH_HIGH) || \
                                      ((pola) == ENHTIM_PWM_START_WITH_LOW))

/**
 * \defgroup    ENHTIM_Mode ENHTIM Mode
 * \{
 * \ingroup     ENHTIM_Exported_Constants
 */
#define ENHTIM_MODE_PWM_MANUAL              ((uint16_t)0x02) /*!< User define pwm manual mode. */
#define ENHTIM_MODE_PWM_AUTO                ((uint16_t)0x01) /*!< User define pwm auto mode. */
#define ENHTIM_MODE_FreeRun                 ((uint16_t)0x00) /*!< User define freerun mode. */
/** \} */
#define IS_ENHTIM_MODE(mode) (((mode) == ENHTIM_MODE_PWM_MANUAL) || \
                              ((mode) == ENHTIM_MODE_PWM_AUTO) || \
                              ((mode) == ENHTIM_MODE_UserDefine))

/**
 * \defgroup    ENHTIM_Interrupts_Definition ENHTIM Interrupts Definition
 * \{
 * \ingroup     ENHTIM_Exported_Constants
 */
#define ENHTIM_INT_TIM                      (0x00)
#define ENHTIM_INT_LATCH_CNT2_FIFO_FULL     (0x40)
#define ENHTIM_INT_LATCH_CNT2_FIFO_THD      (0x42)
#define ENHTIM_INT_LATCH_CNT2_FIFO_EMPTY    (0x01) /*!< Empty interrupt flag bit but no interrupt */
/** \} */

#define IS_ENHTIM_INT(INT) (((INT) == ENHTIM_INT_TIM) || \
                            ((INT) == ENHTIM_INT_LATCH_CNT2_FIFO_FULL) || \
                            ((INT) == ENHTIM_INT_LATCH_CNT2_FIFO_THD))

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
/** \} */
#define IS_ENHTIM_CCR_FIFO_FLAG(flag) (((flag) == ENHTIM_FLAG_TIM1_LC_FIFO_EMPTY) || \
                                       ((flag) == ENHTIM_FLAG_TIM0_LC_FIFO_EMPTY) || \
                                       ((flag) == ENHTIM_FLAG_TIM1_CCR_FIFO_EMPTY) || \
                                       ((flag) == ENHTIM_FLAG_TIM1_CCR_FIFO_FULL) || \
                                       ((flag) == ENHTIM_FLAG_TIM0_CCR_FIFO_EMPTY) || \
                                       ((flag) == ENHTIM_FLAG_TIM0_CCR_FIFO_FULL) ))

/**
 * \defgroup    PWM_DeadZone_En PWM DeadZone enable
 * \{
 * \ingroup     ENHTIM_Exported_Constants
 */
#define ENHTIM_PWM_DEADZONE_ENABLE          ((uint16_t)0x1)
#define ENHTIM_PWM_DEADZONE_DISABLE         ((uint16_t)0x0)
/** \} */
#define IS_ENHTIM_PWM_DEADZONE_EN(mode) (((mode) == ENHTIM_PWM_DEADZONE_ENABLE) || \
                                         ((mode) == ENHTIM_PWM_DEADZONE_DISABLE))

/**
 * \defgroup    PWM_DeadZone_En PWM DeadZone enable
 * \{
 * \ingroup     ENHTIM_Exported_Constants
 */
#define ENHTIM_PWM_DZCLKSRCE_ENHTIM         ((uint32_t)0x80000)
#define ENHTIM_PWM_DZCLKSRCE_5M             ((uint32_t)0x10000)
#define ENHTIM_PWM_DZCLKSRCE_32K            ((uint32_t)0x0)
/** \} */
#define IS_ENHTIM_PWM_DEADZONE_EN(mode) (((mode) == ENHTIM_PWM_DEADZONE_ENABLE) || \
                                         ((mode) == ENHTIM_PWM_DEADZONE_DISABLE))

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
 * \defgroup    ENHTIM_Latch_Channel_Count ENHTIM Latch Channel Order Number
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

/*============================================================================*
 *                         Types
 *============================================================================*/

/**
 * \defgroup    ENHTIM_Exported_Types Init Params Struct
 *
 * \ingroup     ENHTIM
 */

/**
 * \brief       ENHTIM init structure definition.
 *
 * \ingroup     ENHTIM_Exported_Types
 */
typedef struct
{
    uint16_t ENHTIM_ClockSource;            /*!< Specifies the ENHTIM clock source.*/
    E_ENHTIM_CLKDIV ENHTIM_ClockDiv;        /*!< Specifies the clock source div.
                                                 This parameter can be a value of \ref ENHTIM_Clock_Divider*/
    uint16_t ENHTIM_Mode;                   /*!< Specifies the counter mode.
                                                 This parameter can be a value of \ref ENHTIM_Mode. */
    uint16_t ENHTIM_PWMOutputEn;            /*!< Enbale EnhtimerN PWM oupput.
                                                 This parameter can be a value of \ref ENHTIM_PWM_En */
    uint16_t ENHTIM_PWMStartPolarity;       /*!< EnhtimerN toggle output polarity for user-define PWM mode.
                                                 This parameter can be a value of \ref ENHTIM_PWMStartPolarity. */
    uint16_t ENHTIM_LatchCountEn[3];        /*!< Specifies enbale EnhtimerN Latch_cnt.
                                                 This parameter can be a value of DISABLE or ENABLE */
    uint16_t ENHTIM_LatchCountTrigger[3];   /*!< Specifies EnhtimerN counter latch trigger mode.
                                                 This parameter can be a value of \ref ENHTIM_LatchCountTrigger. */
    uint16_t ENHTIM_LatchCount2Thd;         /*!< Specifies EnhtimerN latched counter fifo threshold.
                                                 This parameter ranges from 1 to 4. */
    uint16_t ENHTIM_LatchTriggerPad;        /*!< User-defined GPIO trigger latch pin.*/
    uint16_t ENHTIM_TimerGPIOTriggerEn;     /*!< Timer GPIO trigger enable.
                                                 This parameter can be a value of DISABLE or ENABLE */
    uint16_t ENHTIM_BTGPIOTriggerEn;        /*!< Bluetooth GPIO trigger enable.
                                                 This parameter can be a value of DISABLE or ENABLE */
    uint32_t ENHTIM_MaxCount;               /*!< Specifies the Enhtimer max counter value for user-define PWM mode.
                                                 This parameter leagel value range is from 0 ~ 2^32-2. */
    uint32_t ENHTIM_CCValue;                /*!< Specifies the Enhtimer capture/compare value for user-define PWM mode.*/
    uint16_t ENHTIM_PWMDeadZoneEn;          /*!< PWM Deadzone enable, pwm0_pn: timer2, pwm1_pn:timer3
                                                 This parameter can be a value of ENABLE or DISABLE. */
    uint32_t ENHTIM_PWMDeadZoneClockSource; /*!< Specifies ENHTIM Source depend.timer3 depend timer2 ,timer5 depend timer4, timer7 depend timer6.
                                                 This parameter can be a value of ENABLE or DISABLE */
    uint16_t ENHTIM_PWMStopStateP;          /*!< Specifies the PWM P stop state.
                                                 This parameter can be a value of \ref PWMDeadZone_Stop_state. */
    uint16_t ENHTIM_PWMStopStateN;          /*!< Specifies the PWM N stop state.
                                                 This parameter can be a value of \ref PWMDeadZone_Stop_state. */
    uint32_t ENHTIM_DeadZoneSize;           /*!< Size of deadzone time, DeadzoneTime=deadzonesize/32000 or 32768.
                                                 This parameter must range from 1 to 0xff. */
} ENHTIM_InitTypeDef;

/*============================================================================*
 *                         Functions
 *============================================================================*/

/**
 * \defgroup    ENHTIM_Exported_Functions  Peripheral APIs
 * \{
 * \ingroup     ENHTIM
 */

/**
 * \brief     Initialize the ENHTIMx unit peripheral according to
 *            the specified parameters in ENHTIM_InitStruct.
 * \param[in] ENHTIMx: Where x can be 0 to 1 to select the ENHTIM peripheral.
 * \param[in] ENHTIM_InitStruct: pointer to a ENHTIM_InitTypeDef structure
  *           that contains the configuration information for the specified ENHTIM peripheral.
 * \return  None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void driver_enhance_timer_init(void)
 * {
 *     RCC_PeriphClockCmd(APBPeriph_TIMER, APBPeriph_TIMER_CLOCK, ENABLE);
 *
 *     ENHTIM_InitTypeDef ENHTIM_InitStruct;
 *     ENHTIM_StructInit(&ENHTIM_InitStruct);
 *
 *     ENHTIM_InitStruct.ENHTIM_PWM_En = PWM_DISABLE;
 *     ENHTIM_InitStruct.ENHTIM_Period = 1000000 - 1 ;
 *     ENHTIM_InitStruct.ENHTIM_Mode = ENHTIM_Mode_UserDefine;
 *     ENHTIM_Init(ENHTIMER_NUM, &ENHTIM_InitStruct);
 * }
 * \endcode
 */
void ENHTIM_Init(ENHTIM_TypeDef *ENHTIMx, ENHTIM_InitTypeDef *ENHTIM_TimeBaseInitStruct);

/**
 * \brief       Fills each ENHTIM_InitStruct member with its default value.
 * \param[in]   ENHTIM_TimeBaseInitStruct: Pointer to a ENHTIM_TimeBaseInitTypeDef structure which will be initialized.
 * \return      None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void driver_enhance_timer_init(void)
 * {
 *     RCC_PeriphClockCmd(APBPeriph_ENHTIMER, APBPeriph_ENHTIMER_CLOCK, ENABLE);
 *
 *     ENHTIM_TimeBaseInitTypeDef ENHTIM_InitStruct;
 *     ENHTIM_StructInit(&ENHTIM_InitStruct);
 *
 *     ENHTIM_InitStruct.ENHTIM_PWM_En = PWM_DISABLE;
 *     ENHTIM_InitStruct.ENHTIM_Period = 1000000 - 1;
 *     ENHTIM_InitStruct.ENHTIM_Mode = ENHTIM_Mode_UserDefine;
 *     ENHTIM_TimeBaseInit(ENH_TIM0, &ENHTIM_InitStruct);
 * }
 * \endcode
 */
void ENHTIM_StructInit(ENHTIM_InitTypeDef *ENHTIM_InitStruct);

/**
 * \brief     Enables or disables the specified ENHTIM peripheral.
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
 *     ENHTIM_InitStruct.ENHTIM_PWM_En = PWM_DISABLE;
 *     ENHTIM_InitStruct.ENHTIM_Period = 1000000 - 1;
 *     ENHTIM_InitStruct.ENHTIM_Mode = ENHTIM_Mode_UserDefine;
 *     ENHTIM_Init(ENH_TIM0, &ENHTIM_InitStruct);
 *     ENHTIM_Cmd(ENH_TIM0, ENABLE);
 * }
 * \endcode
 */
void ENHTIM_Cmd(ENHTIM_TypeDef *ENHTIMx, FunctionalState NewState);

/**
  * \brief      Mask or unmask the latch count2 fifo interrupt.
  * \param[in]  ENHTIMx: Where x can be 0 to 1 to select the ENHTIMx peripheral.
  * \param[in]  NewState: New state of the specified ENHTIMx peripheral latch count2 fifo interrupt.
  *             This parameter can be: ENABLE or DISABLE.
  * \return     None.
  */
void ENHTIM_LCFIFOMaskConfig(ENHTIM_TypeDef *ENHTIMx, FunctionalState NewState);

/**
 * \brief     Enables or disables ENHTIMx interrupt.
 * \param[in] ENHTIMx: Where x can be 0 to 1 to select the ENHTIMx peripheral.
 * \param[in] ENHTIM_INT: Specifies the ENHTIMx interrupt source which to be enabled or disabled.
 *            This parameter can be one of the following values:
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
 *     RCC_PeriphClockCmd(APBPeriph_TIMER, APBPeriph_TIMER_CLOCK, ENABLE);
 *
 *     ENHTIM_InitTypeDef ENHTIM_InitStruct;
 *     ENHTIM_StructInit(&ENHTIM_InitStruct);
 *
 *     ENHTIM_InitStruct.ENHTIM_PWM_En = PWM_DISABLE;
 *     ENHTIM_InitStruct.ENHTIM_Period = 1000000 - 1;
 *     ENHTIM_InitStruct.ENHTIM_Mode = ENHTIM_Mode_UserDefine;
 *     ENHTIM_Init(ENH_TIM0, &ENHTIM_InitStruct);
 *     ENHTIM_ClearINT(ENH_TIM0);
 *     ENHTIM_INTConfig(ENH_TIM0, ENABLE);
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
 * }
 * \endcode
 */
void ENHTIM_ReadLatchCount2FIFO(ENHTIM_TypeDef *ENHTIMx, uint32_t *pBuf, uint8_t length);

/**
 * \brief     Check whether the ENHTIM interrupt has occurred or not.
 * \param[in] ENHTIMx: Where x can be 0 to 1 to select the ENHTIMx peripheral.
 * \return    The new state of the specified ENHTIMx peripheral
 *            latch count2 fifo interrupt(SET or RESET).
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
 * \param[in] ENHTIM_INT: Specifies the ENHTIMx interrupt source which to be enabled or disabled.
 *            This parameter can be one of the following values:
 *            \arg ENHTIM_INT_TIM: Enhance Timer interrupt source.
 *            \arg ENHTIM_INT_LATCH_CNT2_FIFO_FULL: Enhance Timer latch count2 fifo full interrupt source.
 *            \arg ENHTIM_INT_LATCH_CNT2_FIFO_EMPTY: Enhance Timer latch count2 fifo empty interrupt source.
 *            \arg ENHTIM_INT_LATCH_CNT2_FIFO_THD: Enhance Timer latch count2 fifo threshold interrupt source.
 * \return    The new state of the ENHTIM_INT(SET or RESET).
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
 * \param[in] ENHTIM_INT: Specifies the ENHTIMx interrupt source which to be enabled or disabled.
 *            This parameter can be one of the following values:
 *            \arg ENHTIM_INT_TIM: Enhance Timer interrupt source.
 *            \arg ENHTIM_INT_LATCH_CNT2_FIFO_FULL: Enhance Timer latch count2 fifo full interrupt source.
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
 * \param[in] ENHTIMx: where x can be 0 to 1 to select the ENHTIMx peripheral.
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
 * \param[in] count: EnhtimerN max counter value for user-define PWM mode (leagel value range is from 0 ~
2^32-2).
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
 * \brief     Set ENHTIMx capture/compare value for user-define PWM manual mode.
 * \param[in] ENHTIMx: Where x can be 0 to 1 to select the ENHTIMx peripheral.
 * \param[in] value: .
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
 * \brief     Set ENHTIMx capture/compare value for user-define PWM auto mode.
 * \param[in] ENHTIMx: Where x can be 0 to 1 to select the ENHTIMx peripheral.
 * \param[in] value: .
 * \return    None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void enhance_timer_demo(void)
 * {
 *     ENHTIM_WriteCCFIFO(ENH_TIM0,0x10000);
 * }
 * \endcode
 */
__STATIC_INLINE void ENHTIM_WriteCCFIFO(ENHTIM_TypeDef *ENHTIMx, uint32_t value)
{
    /* Check the parameters */
    assert_param(IS_ENHTIM_ALL_PERIPH(ENHTIMx));

    ENHTIMx->CCR_FIFO = value;
}

///**
// * \brief     Check whether the specified ENHTIM CCR FIFO flag is set.
// * \param[in] ENHTIM_FLAG: Specifies the flag to check.
// *            This parameter can be one of the following values:
// *            \arg ENHTIM_FLAG_TIM1_LC_FIFO_EMPTY: FIFO empty or not. If SET, CCR FIFO is empty.
// *            \arg ENHTIM_FLAG_TIM0_LC_FIFO_EMPTY: FIFO full or not. If SET, CCR FIFO is full.
// * \return    The new state of ENHTIM_FLAG (SET or RESET).
// *
// * <b>Example usage</b>
// * \code{.c}
// *
// * void enhance_timer_demo(void)
// * {
// *     FlagStatus flag_status = ENHTIM_GetCCRFIFOFlagStatus(ENHTIM_FLAG_FIFO_EMPTY);
// * }
// * \endcode
// */
//__STATIC_INLINE FlagStatus ENHTIM_GetLCFIFOFlagStatus(uint32_t ENHTIM_FLAG)
//{
//    /* Check the parameters */
//    assert_param(IS_ENHTIM_LC_FIFO_FLAG(ENHTIM_FLAG));

//    FlagStatus bitstatus = RESET;

//  if (ENH_TIM_SHARE->FIFO_SR2 & ENHTIM_FLAG)
//  {
//      bitstatus = SET;
//  }

//  return bitstatus;
//}

///**
// * \brief     Check whether the specified ENHTIM CCR FIFO flag is set.
// * \param[in] ENHTIM_FLAG: Specifies the flag to check.
// *            This parameter can be one of the following values:
// *            \arg ENHTIM_FLAG_TIM1_CCR_FIFO_EMPTY: FIFO empty or not. If SET, CCR FIFO is empty.
// *            \arg ENHTIM_FLAG_TIM1_CCR_FIFO_FULL: FIFO full or not. If SET, CCR FIFO is full.
// *            \arg ENHTIM_FLAG_TIM0_CCR_FIFO_EMPTY: FIFO empty or not. If SET, CCR FIFO is empty.
// *            \arg ENHTIM_FLAG_TIM0_CCR_FIFO_FULL: FIFO full or not. If SET, CCR FIFO is full.
// * \return    The new state of ENHTIM_FLAG (SET or RESET).
// *
// * <b>Example usage</b>
// * \code{.c}
// *
// * void enhance_timer_demo(void)
// * {
// *     FlagStatus flag_status = ENHTIM_GetCCRFIFOFlagStatus(ENHTIM_FLAG_FIFO_EMPTY);
// * }
// * \endcode
// */
//__STATIC_INLINE FlagStatus ENHTIM_GetCCRFIFOFlagStatus(uint32_t ENHTIM_FLAG)
//{
//    /* Check the parameters */
//    assert_param(IS_ENHTIM_CCR_FIFO_FLAG(ENHTIM_FLAG));

//    FlagStatus bitstatus = RESET;

//  if (ENH_TIM_SHARE->FIFO_SR2 & ENHTIM_FLAG)
//  {
//      bitstatus = SET;
//  }

//  return bitstatus;
//}

/**
 * \brief     Check whether the specified ENHTIM CCR FIFO flag is set.
 * \param[in] ENHTIM_FLAG: Specifies the flag to check.
 *            This parameter can be one of the following values:
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
 * \param[in] ENHTIMx: where x can be 0 to 1 to select the ENHTIMx peripheral.
 * \param[in] LatchCntIdx: E_ENHTIM_LATCHCNT enum value.
 *            This parameter can be one of follow.
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
 *            This parameter can be one of follow.
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
 * \brief     Get ENHTIMx latch count value.
 * \param[in] ENHTIMx: Where x can be 0 to 1 to select the ENHTIMx peripheral.
 * \param[in] LatchCntIdx: E_ENHTIM_LATCHCNT enum value.
 *            This parameter can be one of follow.
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
 * \brief     Get ENHTIMx latch counter2 fifo length.
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
 * \param[in] FIFO_CLR: Specifies the FIFO type which to be clear.
 *            This parameter can be one of the following values:
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
 * \param   None.
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
 * \return    ENHTIM PWM output status(SET or RESET).
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

///**
// * \brief     Check whether the ENHTIM interrupt has occurred or not.
// * \param[in] ENHTIMx: Where x can be 0 to 1 to select the ENHTIMx peripheral.
// * \return    The new state of the ENHTIM_IT(SET or RESET).
// *
// * <b>Example usage</b>
// * \code{.c}
// *
// * void enhance_timer_demo(void)
// * {
// *     ITStatus int_status = ENHTIM_GetINTStatus(ENH_TIM0);
// * }
// * \endcode
// */
//__STATIC_INLINE uint32_t ENHTIM_GetAllINTStatus(void)
//{
//    return ENH_TIM_SHARE->MASK_INT_SR;
//}

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

/** \} */ /* End of group ENHTIM_Exported_Functions */

#ifdef __cplusplus
}
#endif

#endif /*_RTL876X_ENH_TIM_H_*/


/******************* (C) COPYRIGHT 2020 Realtek Semiconductor Corporation *****END OF FILE****/
