/**
**********************************************************************************************************
*               Copyright(c) 2020, Realtek Semiconductor Corporation. All rights reserved.
**********************************************************************************************************
* @file     rtl876x_enh_tim.c
* @brief    This file provides all the Timer firmware functions.
* @details
* @author   Yuan
* @date     2020.09.29
* @version  v1.0.0
*********************************************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include "rtl876x_rcc.h"
#include "rtl876x_enh_tim.h"

/* ENHTIM Private Defines */
#define TIMER_CLK_SOURCE_REG_35C (*((volatile uint32_t *)0x4000035CUL))
#define TIMER_CLK_SOURCE_REG_360 (*((volatile uint32_t *)0x40000360UL))

/**
  * @brief  Initializes the ENHTIMx peripheral according to
  *         the specified parameters in the ENHTIM_InitStruct.
  * @param  ENHTIMx: where x can be 0 to 1 to select the ENHTIM peripheral.
  * @param  ENHTIM_InitStruct: pointer to a ENHTIM_InitTypeDef structure
  *         that contains the configuration information for the specified ENHTIM peripheral.
  * @retval None
  */
void ENHTIM_Init(ENHTIM_TypeDef *ENHTIMx, ENHTIM_InitTypeDef *ENHTIM_InitStruct)
{
    uint32_t enhtim_id = 0;
    uint32_t temp = 0;

    /* Check the parameters */
    assert_param(IS_ENHTIM_ALL_PERIPH(ENHTIMx));

    enhtim_id = ((uint32_t)ENHTIMx - (uint32_t)ENH_TIM0) / 0x24;

    ENH_TIM_SHARE->CMD &= ~BIT(enhtim_id);
    ENH_TIM_SHARE->INT_CMD &= ~BIT(enhtim_id);

    /* Clock source config */
    if (TIMER_CLK_SOURCE_REG_360 & BIT(10))
    {
        TIMER_CLK_SOURCE_REG_360 &= ~(BIT(10));
    }

    TIMER_CLK_SOURCE_REG_360 |= ((ENHTIM_InitStruct->ENHTIM_ClockDiv & 0x07) << (enhtim_id * 3 + 25));

    if (ENHTIM_InitStruct->ENHTIM_PWMDeadZoneClockSource == ENHTIM_PWM_DZCLKSRCE_32K)
    {
        TIMER_CLK_SOURCE_REG_360 |= BIT8;
    }

    TIMER_CLK_SOURCE_REG_35C |= ((ENHTIM_InitStruct->ENHTIM_ClockSource & 0x01) << (enhtim_id + 1));

    temp = 0;
    for (uint8_t i = 0; i < 3; i++)
    {
        temp |= ((ENHTIM_InitStruct->ENHTIM_LatchCountEn[i] & 0x01) << (i + 10)) \
                | ((ENHTIM_InitStruct->ENHTIM_LatchCountTrigger[i] & 0x03) << (i * 2 + 4));
    }
    ENHTIMx->CR |= temp;

    /*Config latch count2. */
    temp = 0x3F;
    if (ENHTIM_InitStruct->ENHTIM_LatchCountEn[2] == ENHTIM_LATCH_COUNT_ENABLE)
    {
        /* Clear Latch Count2 thd */
        ENHTIMx->CR &= ~(0x1F << 23);
        ENHTIMx->CR |= (ENHTIM_InitStruct->ENHTIM_LatchCount2Thd & 0x1F) << 23;

        /* Config Latch Count2 trigger GPIO pin. */
        if ((ENHTIM_InitStruct->ENHTIM_LatchTriggerPad >= P1_6) &&
            (ENHTIM_InitStruct->ENHTIM_LatchTriggerPad <= P3_6))
        {
            temp = (ENHTIM_InitStruct->ENHTIM_LatchTriggerPad - 4);
        }
        else if ((ENHTIM_InitStruct->ENHTIM_LatchTriggerPad >= P4_0) &&
                 (ENHTIM_InitStruct->ENHTIM_LatchTriggerPad <= P5_2))
        {
            temp = (ENHTIM_InitStruct->ENHTIM_LatchTriggerPad - 5);
        }
        else if (ENHTIM_InitStruct->ENHTIM_LatchTriggerPad <= P1_1)
        {
            temp = (ENHTIM_InitStruct->ENHTIM_LatchTriggerPad);
        }
    }

    ENHTIM_LATCH_COUNT_CR &=  ~(0x1FF);
    ENHTIM_LATCH_COUNT_CR |= (ENHTIM_InitStruct->ENHTIM_TimerGPIOTriggerEn << 8) | \
                             (ENHTIM_InitStruct->ENHTIM_BTGPIOTriggerEn << 7) | \
                             ((enhtim_id & 0x01) << 6) | ((temp) & 0x3F);

    /* Config PWM mode. */
    temp = 0;
    if ((ENHTIM_InitStruct->ENHTIM_Mode == ENHTIM_MODE_PWM_AUTO) ||
        (ENHTIM_InitStruct->ENHTIM_Mode == ENHTIM_MODE_PWM_MANUAL))
    {
        temp = (ENHTIM_InitStruct->ENHTIM_PWMStartPolarity & 0x04) |
               (ENHTIM_InitStruct->ENHTIM_PWMOutputEn & 0x08) |
               ENHTIM_InitStruct->ENHTIM_Mode;
        ENHTIMx->CR |= temp;
        temp = 0;
        ENHTIMx->MAX_CNT = ENHTIM_InitStruct->ENHTIM_MaxCount;
    }

    if (ENHTIM_InitStruct->ENHTIM_Mode == ENHTIM_MODE_PWM_MANUAL)
    {
        ENHTIMx->CCR = ENHTIM_InitStruct->ENHTIM_CCValue;
    }

    /* Set pwm1 deadzone mode, pwm1_pn based on enhance timer0 */
    if (ENHTIM_InitStruct->ENHTIM_PWMDeadZoneEn == ENHTIM_PWM_DEADZONE_ENABLE)
    {
        if (enhtim_id  == 0)
        {
            /* Set pwm deadzone time. */
            ENHTIM_PWM_DEADZONE_CR |= ((ENHTIM_InitStruct->ENHTIM_PWMDeadZoneClockSource & 0x90000) \
                                       | ((ENHTIM_InitStruct->ENHTIM_PWMStopStateP << 10) & (BIT10)) \
                                       | ((ENHTIM_InitStruct->ENHTIM_PWMStopStateN << 9) & (BIT9)) \
                                       | (ENHTIM_InitStruct->ENHTIM_DeadZoneSize & 0xFF) \
                                       | BIT12 | BIT17 | BIT18);
        }
    }
    else
    {
        if (enhtim_id == 0)
        {
            /* Disable pwm1 deadzone mode. */
            ENHTIM_PWM_DEADZONE_CR &= ~(BIT12 | BIT17 | BIT18);
        }
    }

    /* Clear the IT status */
    ENH_TIM_SHARE->FIFO_SR0 |= BIT(16 + enhtim_id);
    ENH_TIM_SHARE->FIFO_SR2 |= BIT(16 + enhtim_id * 2);
    ENH_TIM_SHARE->INT_SR |= BIT(enhtim_id);
}

/**
  * @brief  Fills each ENHTIM_InitStruct member with its default value.
  * @param  ENHTIM_InitStruct: Pointer to a ENHTIM_InitStruct structure which will be initialized.
  * @return None.
  */
void ENHTIM_StructInit(ENHTIM_InitTypeDef *ENHTIM_InitStruct)
{
    ENHTIM_InitStruct->ENHTIM_ClockSource           = ENHTIM_DIVIDER_CLOCK;
    ENHTIM_InitStruct->ENHTIM_ClockDiv              = ENHTIM_CLOCK_DIVIDER_1;
    ENHTIM_InitStruct->ENHTIM_Mode                  = ENHTIM_MODE_FreeRun;
    ENHTIM_InitStruct->ENHTIM_PWMOutputEn           = ENHTIM_PWM_DISABLE;
    ENHTIM_InitStruct->ENHTIM_PWMStartPolarity      = ENHTIM_PWM_START_WITH_LOW;
    ENHTIM_InitStruct->ENHTIM_LatchCountEn[0]       = ENHTIM_LATCH_COUNT_DISABLE;
    ENHTIM_InitStruct->ENHTIM_LatchCountTrigger[0]  = ENHTIM_LATCH_TRIGGER_RISING_EDGE;
    ENHTIM_InitStruct->ENHTIM_LatchCountEn[1]       = ENHTIM_LATCH_COUNT_DISABLE;
    ENHTIM_InitStruct->ENHTIM_LatchCountTrigger[1]  = ENHTIM_LATCH_TRIGGER_RISING_EDGE;
    ENHTIM_InitStruct->ENHTIM_LatchCountEn[2]       = ENHTIM_LATCH_COUNT_DISABLE;
    ENHTIM_InitStruct->ENHTIM_LatchCountTrigger[2]  = ENHTIM_LATCH_TRIGGER_RISING_EDGE;
    ENHTIM_InitStruct->ENHTIM_LatchCount2Thd        = 3;
    ENHTIM_InitStruct->ENHTIM_TimerGPIOTriggerEn    = DISABLE;
    ENHTIM_InitStruct->ENHTIM_BTGPIOTriggerEn       = DISABLE;
    ENHTIM_InitStruct->ENHTIM_MaxCount              = 0xFFFFFFFE;//range 0x1~0xFFFFFFFE
    ENHTIM_InitStruct->ENHTIM_CCValue               = 0x0;

    ENHTIM_InitStruct->ENHTIM_PWMDeadZoneClockSource    = ENHTIM_PWM_DZCLKSRCE_32K;
    ENHTIM_InitStruct->ENHTIM_PWMDeadZoneEn             = ENHTIM_PWM_DEADZONE_DISABLE;
    ENHTIM_InitStruct->ENHTIM_PWMStopStateP         = ENHTIM_PWM_STOP_AT_HIGH;
    ENHTIM_InitStruct->ENHTIM_PWMStopStateN         = ENHTIM_PWM_STOP_AT_LOW;
    ENHTIM_InitStruct->ENHTIM_DeadZoneSize          = 0xFF;/*!< range 0x1 ~ 0xFF */
}

/**
  * @brief  Enables or disables the specified ENHTIM peripheral.
  * @param  ENHTIMx: Where x can be 0 to 1 to select the ENHTIMx peripheral.
  * @param  NewState: New state of the ENHTIMx peripheral.
  *         This parameter can be: ENABLE or DISABLE.
  * @retutn None.
  */
void ENHTIM_Cmd(ENHTIM_TypeDef *ENHTIMx, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_ENHTIM_ALL_PERIPH(ENHTIMx));
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    uint32_t enhtim_id = ((uint32_t)ENHTIMx - (uint32_t)ENH_TIM0) / 0X24;

    if (NewState != DISABLE)
    {
        /* Enable the TIM Counter */
        ENH_TIM_SHARE->CMD |= BIT(enhtim_id);
    }
    else
    {
        /* Disable the TIM Counter */
        ENH_TIM_SHARE->CMD &= ~(BIT(enhtim_id));
    }
}

/**
  * @brief  Mask or unmask the latch count2 fifo interrupt.
  * @param  ENHTIMx: Where x can be 0 to 1 to select the ENHTIMx peripheral.
  * @param  NewState: New state of the specified ENHTIMx peripheral latch count2 fifo interrupt.
  *         This parameter can be: ENABLE or DISABLE.
  * @retval None.
  */
void ENHTIM_LCFIFOMaskConfig(ENHTIM_TypeDef *ENHTIMx, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_ENHTIM_ALL_PERIPH(ENHTIMx));
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    uint32_t enhtim_id = ((uint32_t)ENHTIMx - (uint32_t)ENH_TIM0) / 0X24;

    if (NewState != DISABLE)
    {
        ENH_TIM_SHARE->INT_CMD |= BIT(24) << enhtim_id;
    }
    else
    {
        ENH_TIM_SHARE->INT_CMD &= ~(BIT(24) << enhtim_id);
    }
}

/**
  * @brief  Enables or disables ENHTIMx interrupt.
  * @param  ENHTIMx: Where x can be 0 to 1 to select the ENHTIMx peripheral.
 * \param[in] ENHTIM_INT: Specifies the ENHTIMx interrupt source which to be enabled or disabled.
 *            This parameter can be one of the following values:
 *            \arg ENHTIM_INT_TIM: Enhance Timer interrupt source.
 *            \arg ENHTIM_INT_LATCH_CNT2_FIFO_FULL: Enhance Timer latch count0 interrupt source.
 *            \arg ENHTIM_INT_LATCH_CNT2_FIFO_THD: Enhance Timer latch count2 interrupt source.
  * @param  NewState: New state of the ENHTIMx peripheral.
  *         This parameter can be: ENABLE or DISABLE.
  * @return None.
  */
void ENHTIM_INTConfig(ENHTIM_TypeDef *ENHTIMx, uint8_t ENHTIM_INT, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_ENHTIM_ALL_PERIPH(ENHTIMx));
    assert_param(IS_ENHTIM_INT(ENHTIM_INT));
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    uint32_t enhtim_id = ((uint32_t)ENHTIMx - (uint32_t)ENH_TIM0) / 0X24;

    if (NewState != DISABLE)
    {
        /* Enable the ENHTIM Interrupt */
        if (ENHTIM_INT & 0x40)
        {
            *((volatile uint32_t *)(&(ENH_TIM_SHARE->LC_INT_CMD0) + (ENHTIM_INT & 0xF))) |= BIT(
                    24) << enhtim_id;
        }
        else
        {
            ENH_TIM_SHARE->INT_CMD |= BIT(enhtim_id);
        }
    }
    else
    {
        /* Disable the ENHTIM Interrupt */
        if (ENHTIM_INT & 0x40)
        {
            *((volatile uint32_t *)(&(ENH_TIM_SHARE->LC_INT_CMD0) + (ENHTIM_INT & 0xF))) &= ~(BIT(
                    24) << enhtim_id);;
        }
        else
        {
            ENH_TIM_SHARE->INT_CMD &= ~BIT(enhtim_id);
        }
    }
}

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
void ENHTIM_ReadLatchCount2FIFO(ENHTIM_TypeDef *ENHTIMx, uint32_t *pBuf, uint8_t length)
{
    /* Check the parameters */
    assert_param(IS_ENHTIM_ALL_PERIPH(ENHTIMx));

    if ((pBuf == 0) || (length > 4))
    {
        return;
    }

    for (uint8_t i = 0; i < length; i++)
    {
        pBuf[i] = ENHTIMx->LATCH_CNT2;
    }
}

/**
 * \brief     Check whether the ENHTIM latch count2 fifo has data or not.
 * \param[in] ENHTIMx: Where x can be 0 to 1 to select the ENHTIMx peripheral.
 * \return    The new state of the specified ENHTIMx peripheral
 *            latch count2 fifo (SET or RESET).
 */
ITStatus ENHTIM_GetLCFIFOStatus(ENHTIM_TypeDef *ENHTIMx)
{
    /* Check the parameters */
    assert_param(IS_ENHTIM_ALL_PERIPH(ENHTIMx));

    uint32_t enhtim_id = ((uint32_t)ENHTIMx - (uint32_t)ENH_TIM0) / 0X24;
    ITStatus itstatus = RESET;

    if (ENH_TIM_SHARE->INT_SR & ((BIT(24) << enhtim_id)))
    {
        itstatus = SET;
    }

    return itstatus;
}

/**
 * \brief     Check whether the ENHTIM latch count2 fifo interrupt has occurred or not.
 * \param[in] ENHTIMx: Where x can be 0 to 1 to select the ENHTIMx peripheral.
 * \return    The new state of the specified ENHTIMx peripheral
 *            latch count2 fifo interrupt(SET or RESET).
 */
ITStatus ENHTIM_GetLCFIFOMaskStatus(ENHTIM_TypeDef *ENHTIMx)
{
    /* Check the parameters */
    assert_param(IS_ENHTIM_ALL_PERIPH(ENHTIMx));

    uint32_t enhtim_id = ((uint32_t)ENHTIMx - (uint32_t)ENH_TIM0) / 0X24;
    ITStatus itstatus = RESET;

    if (ENH_TIM_SHARE->MASK_INT_SR & ((BIT(24) << enhtim_id)))
    {
        itstatus = SET;
    }

    return itstatus;
}

/**
 * \brief     Check whether the ENHTIM interrupt has occurred or not.
 * \param[in] ENHTIMx: Where x can be 0 to 1 to select the ENHTIMx peripheral.
 * \param[in] ENHTIM_INT: Specifies the ENHTIMx interrupt source which to be enabled or disabled.
 *            This parameter can be one of the following values:
 *            \arg ENHTIM_INT_TIM: Enhance Timer interrupt source.
 *            \arg ENHTIM_INT_LATCH_CNT2_FIFO_FULL: Enhance Timer latch count2 fifo full interrupt source.
// *            \arg ENHTIM_INT_LATCH_CNT2_FIFO_EMPTY: Enhance Timer latch count2 fifo empty interrupt source.
 *            \arg ENHTIM_INT_LATCH_CNT2_FIFO_THD: Enhance Timer latch count2 fifo threshold interrupt source.
 * \return    The new state of the ENHTIM_INT(SET or RESET).
 */
ITStatus ENHTIM_GetINTStatus(ENHTIM_TypeDef *ENHTIMx, uint8_t ENHTIM_INT)
{
    /* Check the parameters */
    assert_param(IS_ENHTIM_ALL_PERIPH(ENHTIMx));
    assert_param(IS_ENHTIM_INT(INT));

    uint32_t enhtim_id = ((uint32_t)ENHTIMx - (uint32_t)ENH_TIM0) / 0X24;
    ITStatus itstatus = RESET;

    if (ENHTIM_INT == ENHTIM_INT_TIM)
    {
        itstatus = (ITStatus)((ENH_TIM_SHARE->INT_SR) >> (enhtim_id));
    }
    else if (ENHTIM_INT == ENHTIM_INT_LATCH_CNT2_FIFO_FULL)
    {
        itstatus = (ITStatus)(((ENH_TIM_SHARE->FIFO_SR2) >> (16 + enhtim_id * 2)) & 0x01);
    }
    else if (ENHTIM_INT == ENHTIM_INT_LATCH_CNT2_FIFO_EMPTY)
    {
        itstatus = (ITStatus)(((ENH_TIM_SHARE->FIFO_SR2) >> (17 + enhtim_id * 2)) & 0x01);
    }
    else if (ENHTIM_INT == ENHTIM_INT_LATCH_CNT2_FIFO_THD)
    {
        itstatus = (ITStatus)(((ENH_TIM_SHARE->FIFO_SR0) >> (16 + enhtim_id)) & 0x01);
    }

    return itstatus;
}

/**
 * \brief     Clear ENHTIM interrupt.
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
void ENHTIM_ClearINTPendingBit(ENHTIM_TypeDef *ENHTIMx, uint8_t ENHTIM_INT)
{
    /* Check the parameters */
    assert_param(IS_ENHTIM_ALL_PERIPH(ENHTIMx));

    uint32_t enhtim_id = ((uint32_t)ENHTIMx - (uint32_t)ENH_TIM0) / 0X24;

    /* Clear the IT */
    if (ENHTIM_INT == ENHTIM_INT_TIM)
    {
        ENH_TIM_SHARE->INT_SR |= BIT(enhtim_id);
    }
    else if (ENHTIM_INT == ENHTIM_INT_LATCH_CNT2_FIFO_FULL)
    {
        ENH_TIM_SHARE->FIFO_SR2 |= BIT(16 + enhtim_id * 2);
    }
    else if (ENHTIM_INT == ENHTIM_INT_LATCH_CNT2_FIFO_EMPTY)
    {
        ENH_TIM_SHARE->FIFO_SR2 |= BIT(17 + enhtim_id * 2);
    }
    else if (ENHTIM_INT == ENHTIM_INT_LATCH_CNT2_FIFO_THD)
    {
        ENH_TIM_SHARE->FIFO_SR0 |= BIT(16 + enhtim_id);
    }
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
 */
void ENHTIM_PWMDZBypassCmd(ENHTIM_TypeDef *ENHTIMx, FunctionalState NewState)
{
    /* Check the parameters. */
    assert_param(IS_ENHTIM_ALL_PERIPH(ENHTIMx));

    if (NewState != DISABLE)
    {
        /* Enable bypass dead zone function. */
        ENHTIM_PWM_DEADZONE_CR |= BIT13;
    }
    else
    {
        /* Disable bypass dead zone function. */
        ENHTIM_PWM_DEADZONE_CR &= (~BIT13);
    }
}

/******************* (C) COPYRIGHT 2020 Realtek Semiconductor Corporation *****END OF FILE****/

