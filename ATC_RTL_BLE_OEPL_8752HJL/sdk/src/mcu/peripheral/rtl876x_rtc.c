/**
*********************************************************************************************************
*               Copyright(c) 2020, Realtek Semiconductor Corporation. All rights reserved.
**********************************************************************************************************
* @file     rtl876x_rtc.c
* @brief    This file provides all the RTC firmware functions.
* @details
* @author   yuan
* @date     2020-11-11
* @version  v2.1.0
*********************************************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include "rtl876x_rtc.h"

/* Internal defines ------------------------------------------------------------*/
#define REG_FAST_WRITE_BASE_ADDR        (0x40000100UL)
#define REG_RTC_FAST_WDATA              (0x400001F0UL)
#define REG_RTC_FAST_ADDR               (0x400001F4UL)
#define REG_RTC_WR_STROBE               (0x400001F8UL)

/**
  * @brief  Fast write RTC register.
  * @param  reg_address: the register address.
  * @param  data: data which write to register.
  * @return None
  */
void RTC_WriteReg(uint32_t reg_address, uint32_t data)
{
    static bool is_called = false;

    if (is_called == false)
    {
        *((volatile uint32_t *)0x40000014) |= BIT(9);//no need run this every time
        is_called = true;
    }

    /* Write data */
    *((volatile uint32_t *)REG_RTC_FAST_WDATA) = data;
    /* Write RTC register address. Only offset */
    *((volatile uint32_t *)REG_RTC_FAST_ADDR) = reg_address - REG_FAST_WRITE_BASE_ADDR;
    *((volatile uint32_t *)REG_RTC_WR_STROBE) = 1;
}

/**
  * @brief  Reset RTC.
  * @param  None
  * @return None
  */
void RTC_DeInit(void)
{
    /* Stop RTC counter */
    RTC_WriteReg((uint32_t)(&(RTC->CR0)), (RTC->CR0 & RTC_START_CLR));

    /* Disable wakeup signal */
    RTC_WriteReg((uint32_t)(&(RTC->CR0)), (RTC->CR0 & RTC_NV_EN_CLR));
    RTC_WriteReg((uint32_t)(&(RTC->CR0)), (RTC->CR0 & RTC_WAKEUP_EN_CLR));

    /* Clear all RTC interrupt & wakeup */
    RTC_WriteReg((uint32_t)(&(RTC->INT_CLR)), RTC_ALL_INT_CLR_SET | RTC_ALL_WAKEUP_CLR_SET);
    __NOP();
    __NOP();
    RTC_WriteReg((uint32_t)(&(RTC->INT_CLR)), 0);

    /* Clear prescale register */
    RTC_WriteReg((uint32_t)(&(RTC->PRESCALER)), 0);
    /* Clear all comparator register */
    RTC_WriteReg((uint32_t)(&(RTC->COMP0)), 0);
    RTC_WriteReg((uint32_t)(&(RTC->COMP1)), 0);
    RTC_WriteReg((uint32_t)(&(RTC->COMP2)), 0);
    RTC_WriteReg((uint32_t)(&(RTC->COMP3)), 0);
    RTC_WriteReg((uint32_t)(&(RTC->COMP0GT)), 0);
    RTC_WriteReg((uint32_t)(&(RTC->COMP1GT)), 0);
    RTC_WriteReg((uint32_t)(&(RTC->COMP2GT)), 0);
    RTC_WriteReg((uint32_t)(&(RTC->COMP3GT)), 0);
    RTC_WriteReg((uint32_t)(&(RTC->PRE_COMP)), 0);

    /* Reset prescale counter and counter */
    RTC_WriteReg((uint32_t)(&(RTC->CR0)), RTC_PRE_COUNTER_RST_Msk | RTC_COUNTER_RST_Msk);
    __NOP();
    __NOP();
    RTC_WriteReg((uint32_t)(&(RTC->CR0)), 0x0);
}

/**
  * @brief  Set RTC prescaler value.
  * @param  value: the prescaler value to be set.Should be no more than 12 bits!
  * @return None
  */
void RTC_SetPrescaler(uint16_t value)
{
    RTC_WriteReg((uint32_t)(&(RTC->PRESCALER)), value & 0xFFF);
}

/**
  * @brief  Start or stop RTC peripheral.
  * @param  NewState: new state of RTC peripheral.
  *         This parameter can be the following values:
  *         @arg ENABLE: start RTC.
  *         @arg DISABLE: stop RTC.
  * @return None
  */
void RTC_Cmd(FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    if (NewState == ENABLE)
    {
        /* Start RTC */
        RTC_WriteReg((uint32_t)(&(RTC->CR0)), RTC->CR0 | RTC_START_Msk);
    }
    else
    {
        /* Stop RTC */
        RTC_WriteReg((uint32_t)(&(RTC->CR0)), RTC->CR0 & RTC_START_CLR);
    }
}

/**
  * @brief  Enable or disable the specified RTC interrupts.
  * @param  RTC_INT: specifies the RTC interrupt source to be enabled or disabled.
  *         This parameter can be any combination of the following values:
  *         @arg RTC_INT_TICK: tick interrupt
  *         @arg RTC_INT_OVF: counter overflow interrupt
  *         @arg RTC_INT_PRE_CMP: prescale compare interrupt
  *         @arg RTC_INT_PRE_CMP3: prescale & compare 3 interrupt
  *         @arg RTC_INT_CMP0: compare 0 interrupt
  *         @arg RTC_INT_CMP1: compare 1 interrupt
  *         @arg RTC_INT_CMP2: compare 2 interrupt
  *         @arg RTC_INT_CMP3: compare 3 interrupt
  * @param  NewState: new state of the specified RTC interrupt.
  *         This parameter can be: ENABLE or DISABLE.
  * @return None.
  */
void RTC_INTConfig(E_RTC_INT RTC_INT, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_RTC_INT(RTC_INT));
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    if (NewState == ENABLE)
    {
        /* Enable the selected RTC comparator interrupt */
        RTC_WriteReg((uint32_t)(&(RTC->CR0)), (RTC->CR0 | RTC_INT));
    }
    else
    {
        /* Disable the selected RTC comparator interrupt */
        RTC_WriteReg((uint32_t)(&(RTC->CR0)), (RTC->CR0 & (~RTC_INT)));
    }
}

/**
  * @brief  Enable or disable the specified RTC wakeup function.
  * @param  RTC_WK: specifies the RTC wakeup function to be enabled or disabled.
  *         This parameter can be any combination of the following values:
  *         @arg RTC_WK_TICK: tick wakeup function
  *         @arg RTC_WK_OVF: tick wakeup function
  *         @arg RTC_WK_PRE_CMP: prescale compare wakeup function
  *         @arg RTC_WK_PRE_CMP3: prescale & compare 3 wakeup function
  *         @arg RTC_WK_COMP0GT: compare 0 gt wakeup function
  *         @arg RTC_WK_COMP1GT: compare 1 gt wakeup function
  *         @arg RTC_WK_COMP2GT: compare 2 gt wakeup function
  *         @arg RTC_WK_COMP3GT: compare 3 gt wakeup function
  *         @arg RTC_WK_CMP0: compare 0 wakeup function
  *         @arg RTC_WK_CMP1: compare 1 wakeup function
  *         @arg RTC_WK_CMP2: compare 2 wakeup function
  *         @arg RTC_WK_CMP3: compare 3 wakeup function
  * @param  NewState: new state of the specified RTC wakeup function.
  *         This parameter can be: ENABLE or DISABLE.
  * @return None.
  */
void RTC_WKConfig(E_RTC_WK RTC_WK, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_RTC_WK(RTC_WK));
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    if (NewState == ENABLE)
    {
        /* Enable the selected RTC comparator interrupt */
        RTC_WriteReg((uint32_t)(&(RTC->CR0)), (RTC->CR0 | RTC_WK));
    }
    else
    {
        /* Disable the selected RTC comparator interrupt */
        RTC_WriteReg((uint32_t)(&(RTC->CR0)), (RTC->CR0 & (~RTC_WK)));
    }
}

/**
  * @brief  Enable interrupt signal to CPU NVIC.
  * @param  This parameter can be: ENABLE or DISABLE.
  * @return None.
  */
void RTC_NvCmd(FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    if (NewState != DISABLE)
    {
        RTC_WriteReg((uint32_t)(&(RTC->CR0)), (RTC->CR0 | RTC_NV_EN_Msk));
    }
    else
    {
        RTC_WriteReg((uint32_t)(&(RTC->CR0)), (RTC->CR0 & RTC_NV_EN_CLR));
    }
}

/**
 * @brief  Enable or disable system wake up of RTC.
 * @param  NewState: new state of the wake up function.
 *         This parameter can be: ENABLE or DISABLE.
 * @return None
 */
void RTC_SystemWakeupConfig(FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    if (NewState == ENABLE)
    {
        /* Enable system wake up */
        RTC_WriteReg((uint32_t)(&(RTC->CR0)), (RTC->CR0  | RTC_WAKEUP_EN_Msk));
    }
    else
    {
        /* Disable system wake up */
        RTC_WriteReg((uint32_t)(&(RTC->CR0)), (RTC->CR0  & RTC_WAKEUP_EN_CLR));
    }
}

/**
  * @brief  Reset counter value of RTC.
  * @param  None
  * @return None
  */
void RTC_ResetCounter(void)
{
    RTC_WriteReg((uint32_t)(&(RTC->CR0)), (RTC->CR0) | RTC_COUNTER_RST_Msk);
    __NOP();
    __NOP();
    RTC_WriteReg((uint32_t)(&(RTC->CR0)), (RTC->CR0) & RTC_COUNTER_RST_CLR);
}

/**
  * @brief  Reset prescaler counter value of RTC.
  * @param  None
  * @return None
  */
void RTC_ResetPrescalerCounter(void)
{
    RTC_WriteReg((uint32_t)(&(RTC->CR0)), (RTC->CR0) | RTC_PRE_COUNTER_RST_Msk);
    __NOP();
    __NOP();
    RTC_WriteReg((uint32_t)(&(RTC->CR0)), (RTC->CR0) & RTC_PRE_COUNTER_RST_CLR);
}

/**
  * @brief  Checks whether the specified RTC interrupt is set or not.
  * @param  RTC_INT: specifies the RTC interrupt source to be enabled or disabled.
  *         This parameter can be any combination of the following values:
  *         @arg RTC_INT_TICK: RTC tick interrupt source
  *         @arg RTC_INT_PRE_COMP: prescale compare interrupt source
  *         @arg RTC_INT_PRE_COMP3: prescale & compare 3 interrupt source
  *         @arg RTC_INT_COMP0: compare 0 interrupt source
  *         @arg RTC_INT_COMP1: compare 1 interrupt source
  *         @arg RTC_INT_COMP2: compare 2 interrupt source
  *         @arg RTC_INT_COMP3: compare 3 interrupt source
  * @return The new state of RTC_INT (SET or RESET).
  */
ITStatus RTC_GetINTStatus(E_RTC_INT RTC_INT)
{
    /* Check the parameters */
    assert_param(IS_RTC_CONFIG_INT(RTC_INT));

    ITStatus int_status = RESET;

    if ((RTC->INT_SR & ((uint32_t)RTC_INT >> 8)) != (uint32_t)RESET)
    {
        int_status = SET;
    }

    /* Return the RTC_INT status */
    return  int_status;
}

/**
  * @brief  Clear the interrupt pending bits of RTC.
  * @param  RTC_INT: specifies the RTC interrupt flag to clear.
  *   This parameter can be any combination of the following values:
  *     @arg RTC_INT_TICK: RTC tick interrupt source
  *     @arg RTC_INT_OVF: RTC counter overflow interrupt source
  *     @arg RTC_INT_PRE_COMP: prescale compare interrupt source
  *     @arg RTC_INT_PRE_COMP3: prescale & compare 3 interrupt source
  *     @arg RTC_INT_COMP0: compare 0 interrupt source
  *     @arg RTC_INT_COMP1: compare 1 interrupt source
  *     @arg RTC_INT_COMP2: compare 2 interrupt source
  *     @arg RTC_INT_COMP3: compare 3 interrupt source
  * @return None
  */
void RTC_ClearINTPendingBit(E_RTC_INT RTC_INT)
{
    /* Check the parameters */
    assert_param(IS_RTC_INT(RTC_INT));

    RTC_WriteReg((uint32_t)(&(RTC->INT_CLR)), (uint32_t)RTC_INT >> 8);
    __NOP();
    __NOP();
    RTC_WriteReg((uint32_t)(&(RTC->INT_CLR)), 0);
}

/**
  * @brief  Checks whether the specified RTC wakeup state is set or not.
  * @param  RTC_WK: specifies the RTC interrupt source to be enabled or disabled.
  *         This parameter can be any combination of the following values:
  *         @arg RTC_WK_TICK: tick wakeup function
  *         @arg RTC_WK_OVF: tick wakeup function
  *         @arg RTC_WK_PRE_CMP: prescale compare wakeup function
  *         @arg RTC_WK_PRE_CMP3: prescale & compare 3 wakeup function
  *         @arg RTC_WK_COMP0GT: compare 0 gt wakeup function
  *         @arg RTC_WK_COMP1GT: compare 1 gt wakeup function
  *         @arg RTC_WK_COMP2GT: compare 2 gt wakeup function
  *         @arg RTC_WK_COMP3GT: compare 3 gt wakeup function
  *         @arg RTC_WK_CMP0: compare 0 wakeup function
  *         @arg RTC_WK_CMP1: compare 1 wakeup function
  *         @arg RTC_WK_CMP2: compare 2 wakeup function
  *         @arg RTC_WK_CMP3: compare 3 wakeup function
  * @return The new state of RTC_INT (SET or RESET).
  */
ITStatus RTC_GetWakeupStatus(E_RTC_WK RTC_WK)
{
    /* Check the parameters */
    assert_param(IS_RTC_CONFIG_INT(RTC_WK));

    ITStatus wakeup_status = RESET;

    if ((RTC->INT_SR & ((uint32_t)RTC_WK >> 8)) != (uint32_t)RESET)
    {
        wakeup_status = SET;
    }

    /* Return the RTC_INT status */
    return  wakeup_status;
}

/**
  * @brief  Clear the wakeup status bits of RTC.
  * @param  RTC_WK: specifies the RTC wakeup flag to clear.
  *         This parameter can be any combination of the following values:
  *         @arg RTC_WK_TICK: tick wakeup function
  *         @arg RTC_WK_OVF: tick wakeup function
  *         @arg RTC_WK_PRE_CMP: prescale compare wakeup function
  *         @arg RTC_WK_PRE_CMP3: prescale & compare 3 wakeup function
  *         @arg RTC_WK_COMP0GT: compare 0 gt wakeup function
  *         @arg RTC_WK_COMP1GT: compare 1 gt wakeup function
  *         @arg RTC_WK_COMP2GT: compare 2 gt wakeup function
  *         @arg RTC_WK_COMP3GT: compare 3 gt wakeup function
  *         @arg RTC_WK_CMP0: compare 0 wakeup function
  *         @arg RTC_WK_CMP1: compare 1 wakeup function
  *         @arg RTC_WK_CMP2: compare 2 wakeup function
  *         @arg RTC_WK_CMP3: compare 3 wakeup function
  * @return None
  */
void RTC_ClearWakeupStatusBit(E_RTC_WK RTC_WK)
{
    /* Check the parameters */
    assert_param(IS_RTC_WK(RTC_WK));

    RTC_WriteReg((uint32_t)(&(RTC->INT_CLR)), (uint32_t)RTC_WK >> 8);
    __NOP();
    __NOP();
    RTC_WriteReg((uint32_t)(&(RTC->INT_CLR)), 0);
}

/**
  * @brief  Clear the interrupt pending bit of the select comparator of RTC.
  * @param  index: the comparator number 0~3.
  * @return None
  */
void RTC_ClearCompINT(E_RTC_COMP_INDEX index)
{
    RTC_WriteReg((uint32_t)(&(RTC->INT_CLR)), BIT(RTC_COMP0_CLR_Pos + index));
    __NOP();
    __NOP();
    RTC_WriteReg((uint32_t)(&(RTC->INT_CLR)), 0);
}

/**
  * @brief  Clear the overflow interrupt pending bit of RTC.
  * @param  None
  * @return None
  */
void RTC_ClearOverFlowINT(void)
{
    RTC_WriteReg((uint32_t)(&(RTC->INT_CLR)), RTC_OVERFLOW_CLR_SET);
    __NOP();
    __NOP();
    RTC_WriteReg((uint32_t)(&(RTC->INT_CLR)), 0);
}

/**
  * @brief  Clear the tick interrupt pending bit of RTC.
  * @param  None
  * @return None
  */
void RTC_ClearTickINT(void)
{
    RTC_WriteReg((uint32_t)(&(RTC->INT_CLR)), RTC_TICK_CLR_SET);
    __NOP();
    __NOP();
    RTC_WriteReg((uint32_t)(&(RTC->INT_CLR)), 0);
}

/******************* (C) COPYRIGHT 2020 Realtek Semiconductor Corporation *****END OF FILE****/

