/**
**********************************************************************************************************
*               Copyright(c) 2020, Realtek Semiconductor Corporation. All rights reserved.
**********************************************************************************************************
* @file     rtl876x_captouch.c
* @brief    This file provides all the Cap Touch functions.
* @details
* @author   Yuan
* @date     2020.09.29
* @version  v1.0.0
*********************************************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include "rtl876x_rcc.h"
#include "rtl876x_captouch.h"
#include "platform_utils.h"

#define CTC_FUNC_EN_REG         0x26
#define CTC_CLK_SRC_REG         (*((volatile uint32_t *)0x4000020CUL))
#define CTC_CLK_EN_REG          (*((volatile uint32_t *)0x40000234UL))
#define CTC_CLK_SRC_SEL_REG     (*((volatile uint32_t *)0x400003F8UL))
#define CTC_WK_EN_REG           (*((volatile uint32_t *)0x40009000UL))
#define CTC_WK_EN_REG_1         (*((volatile uint32_t *)0x40009048UL))

/**
  * @brief  Enable the CTCx peripheral IP clock and function..
  * @param  None.
  * @retval None.
  */
void CTC_RCCConfig(uint32_t clock_source)
{
    /* Enable Cap Touch IP clock and function */
    if (clock_source == CTC_CLOCK_SOURCE_1M)
    {
        CTC_CLK_SRC_REG |= BIT29;
        CTC_CLK_SRC_SEL_REG |= BIT0;
    }
    else if (clock_source == CTC_CLOCK_SOURCE_40M)
    {
        CTC_CLK_SRC_SEL_REG &= ~BIT0;
    }

    CTC_CLK_EN_REG |= BIT12;

    /* Enable CTC function */
    uint16_t reg_value = btaon_fast_read_safe(CTC_FUNC_EN_REG);
    reg_value |= BIT(15);
    btaon_fast_write_safe(CTC_FUNC_EN_REG, reg_value);
}

/**
  * @brief  Deinitializes the CTCx peripheral registers to their default reset values.
  * @param  None.
  * @retval None.
  */
void CTC_DeInit()
{
    /* Enable Cap Touch IP clock and function */
    CTC_CLK_EN_REG &= ~BIT12;

    uint16_t reg_value = btaon_fast_read_safe(CTC_FUNC_EN_REG);
    reg_value &= ~BIT(15);
    btaon_fast_write_safe(CTC_FUNC_EN_REG, reg_value);
}

/**
  * @brief  Initializes the CTCx peripheral according to
  *         the specified parameters in the CTC_InitStruct.
  * @param  CTCx: CTC peripheral.
  * @param  CTC_InitStruct: pointer to a CTC_InitTypeDef structure that
  *         contains the configuration information for the specified CTC peripheral.
  * @retval None.
  */
void CTC_Init(CTC_TypeDef *CTCx, CTC_InitTypeDef *CTC_InitStruct)
{
    uint32_t temp = 0;

    /* Check the parameters */
    assert_param(IS_CTC_ALL_PERIPH(CTCx));

    /* Disable CTC */
    CTCx->CR &= CTC_CT_EN_Clr;

    temp |= ((CTC_InitStruct->CTC_DebounceEn & 0x01) << CTC_DEBOUNCE_EN_Pos) |
            (CTC_InitStruct->CTC_DebounceTime & 0x03) << CTC_DEBOUNCE_TIME_Pos;
    CTCx->CR = temp;

    temp = ((CTC_InitStruct->CTC_SampleTime & 0x07) << 16)
           | (CTC_InitStruct->CTC_ScanInterval & 0xFFF);
    CTCx->SCAN_PERIOD = temp;

    temp = ((CTC_InitStruct->CTC_BaselineUpdateStep & 0x0F) << 12)
           | ((CTC_InitStruct->CTC_BaselineWeightFactor & 0x0F) << 8)
           | ((CTC_InitStruct->CTC_ETCScanInterval & 0x7F) << 1)
           | ((CTC_InitStruct->CTC_ETCEn & 0x01) << 0);
    CTCx->ETC_CR = temp;

    temp = ((CTC_InitStruct->CTC_DelayGuardInterval & 0x1F) << 16)
           | ((CTC_InitStruct->CTC_AdvancedGuardInterval & 0x1F) << 0);
    CTCx->GUARD_CNT = temp;

    temp = ((CTC_InitStruct->CTC_FastMatchCnt & 0x07) << 16)
           | ((CTC_InitStruct->CTC_FastScanInerval & 0xFFF) << 0);
    CTC_FAST->SCAN_INI = temp;

    temp = ((CTC_InitStruct->CTC_FalseTouchCnt & 0x3FF) << 16)
           | ((CTC_InitStruct->CTC_FalseAlarmCnt & 0xF) << 10)
           | ((CTC_InitStruct->CTC_ReleaseActiveCnt & 0x3FF) << 0);
    CTC_FAST->SET_CNT = temp;

    uint16_t reg_value = btaon_fast_read_safe(0x116);
    btaon_fast_write_safe(0x116, reg_value | BIT(10) | BIT(15));

    /* vref_sel = 0.85V */
    reg_value = btaon_fast_read_safe(0x116);
    btaon_fast_write_safe(0x116, reg_value | BIT(5) | BIT(4));
}

/**
  * @brief  Initializes the specified channel.
  * @param  channel: specified channel
  * @retval None
  */
void CTC_ChannelInit(CTC_ChannelTypeDef *CTC_Channelx,
                     CTC_ChannelInitTypeDef *CTC_ChannelInitStruct)
{
    /* Check the parameters */
    assert_param(IS_CTC_CHANNEL(CH));

    CTC_Channelx->TOUCH_TH |= ((CTC_ChannelInitStruct->CTC_NNoiseThd & 0xFF) << 24)
                              | ((CTC_ChannelInitStruct->CTC_PNoiseThd & 0xFF) << 16);
    CTC_Channelx->CR |= ((CTC_ChannelInitStruct->CTC_DifferenceTouchThd & 0xFFF) << 16) | \
                        ((CTC_ChannelInitStruct->CTC_ChannelEn & 0x1) << 0);
    CTC_Channelx->MBIAS |= ((CTC_ChannelInitStruct->CTC_MBias & 0x3F) << 0);
}


/**
  * @brief  Fills each CTC_InitStruct member with its default value.
  * @param  CTC_InitStruct: Pointer to a CTC_InitStruct structure which will be initialized.
  * @return None.
  */
void CTC_StructInit(CTC_InitTypeDef *CTC_InitStruct)
{
    CTC_InitStruct->CTC_BaselineIniEn           = CTC_BASELINEINI_ENABLE;
    CTC_InitStruct->CTC_DebounceEn              = CTC_DEBOUNCE_ENABLE;
    CTC_InitStruct->CTC_DebounceTime            = CTC_DEBOUNCE_TIMES_2;
    CTC_InitStruct->CTC_SampleTime              = CTC_SAMPLE_TIME_AVE_256;
    CTC_InitStruct->CTC_ScanInterval            = CTC_SCAN_INTERVAL_60ms;//60ms
    CTC_InitStruct->CTC_BaselineUpdateStep      = 0x0001;
    CTC_InitStruct->CTC_BaselineWeightFactor    = 0x0002;
    CTC_InitStruct->CTC_ETCScanInterval         = 0x02;
    CTC_InitStruct->CTC_ETCEn                   = ENABLE;
    CTC_InitStruct->CTC_SNRUpdateMode           = 0x0;
    CTC_InitStruct->CTC_ScanChannel             = 0x0;
    CTC_InitStruct->CTC_ScanChannelSwitchMode   = 0x1;
    CTC_InitStruct->CTC_DelayGuardInterval      = 0x0;
    CTC_InitStruct->CTC_AdvancedGuardInterval   = 0x203;

    CTC_InitStruct->CTC_FastMatchCnt            =
        CTC_FS_Match_Cnt_1DB; /*!< FS_MATCH_CNT must smaller than debounce time. */
    CTC_InitStruct->CTC_FastScanInerval         = 0x1D;
    CTC_InitStruct->CTC_FalseTouchCnt           = 0x3FF;
    CTC_InitStruct->CTC_FalseAlarmCnt           = 0x5;
    CTC_InitStruct->CTC_ReleaseActiveCnt        = 0xFF;
}

/**
  * @brief  Fills each CTC_InitStruct member with its default value.
  * @param  CTC_InitStruct: Pointer to a CTC_InitStruct structure which will be initialized.
  * @return None.
  */
void CTC_ChannelStructInit(CTC_ChannelInitTypeDef *CTC_ChannelInitStruct)
{
    CTC_ChannelInitStruct->CTC_ChannelEn            = DISABLE;
    CTC_ChannelInitStruct->CTC_BaselineData         = 0x0;
    CTC_ChannelInitStruct->CTC_DifferenceTouchThd   = 0x64;
    CTC_ChannelInitStruct->CTC_AbsoluteTouchThd     = 0x0;
    CTC_ChannelInitStruct->CTC_PNoiseThd            = 0xFF;
    CTC_ChannelInitStruct->CTC_NNoiseThd            = 0xFF;
    CTC_ChannelInitStruct->CTC_MBias                = CTC_MBIAS_8uA;
}

/**
  * @brief    Enable or disable the specified Cap Touch interrupt.
 * \param[in] CTCx: The Cap Touch peripheral.
 * \param[in] CTC_INT: Specifies the Cap Touch interrupt source which to be enabled or disabled.
 *            This parameter can be any combination of the following values:
 *            \arg CTC_INT_FALSE_TOUCH_CH1: Channel 1 Touch cnt reach false touch cnt interrupt.
 *            \arg CTC_INT_FALSE_TOUCH_CH0: Channel 0 Touch cnt reach false touch cnt interrupt.
 *            \arg CTC_INT_N_NOISE_THD: Negative noise threshold overflow interrupt.
 *            \arg CTC_INT_FIFO_OVERFLOW: Raw code FIFO over flow interrupt
 *            \arg CTC_INT_P_NOISE_THD: Positive noise threshold overflow interrupt.
 *            \arg CTC_INT_TOUCH_RELEASE_CH1: Channel x release interrupt.
 *            \arg CTC_INT_TOUCH_RELEASE_CH0: Channel x release interrupt.
 *            \arg CTC_INT_TOUCH_PRESS_CH1: Channel x press interrupt.
 *            \arg CTC_INT_TOUCH_PRESS_CH0: Channel x press interrupt.
  * @param    NewState: New state of the Cap Touch interrupt.
  *           This parameter can be: ENABLE or DISABLE.
  * @retval   None
  */
void CTC_INTConfig(uint32_t CTC_INT, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_CTC_INT(CTC_INT));
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    if (NewState != DISABLE)
    {
        /* Enable the selected GPIO pin interrupts */
        CTC->INT_EN |= CTC_INT;
    }
    else
    {
        /* Disable the selected GPIO pin interrupts */
        CTC->INT_EN &= ~CTC_INT;
    }
}

/**
  * @brief  Enables or disables the specified Cap Touch peripheral.
  * @param  CTCx: Cap Touch peripheral.
  * @param  NewState: New state of the Cap Touch peripheral.
  *         This parameter can be: ENABLE or DISABLE.
  * @retutn None.
  */
void CTC_Cmd(CTC_TypeDef *CTCx, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_CTC_ALL_PERIPH(CTCx));
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    if (NewState != DISABLE)
    {
        /* Enable the Cap Touch */
        CTCx->CR |= CTC_BASELINE_INI_Msk | CTC_CT_EN_Msk;
    }
    else
    {
        /* Disable the Cap Touch */
        CTCx->CR &= CTC_CT_EN_Clr;
        CTCx->CR &= CTC_BASELINE_INI_CLR;
    }
}

/**
 * @brief  Enable or disable system wake up of CTC.
 * @param  NewState: new state of the wake up function.
 *         This parameter can be: ENABLE or DISABLE.
 * @return None
 */
void CTC_SystemWakeupConfig(E_CTC_CH channel, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    if (NewState == ENABLE)
    {
        /* Enable system wake up */
        if (channel == CTC_CH0)
        {
            CTC_WK_EN_REG |= BIT16;
        }
        else
        {
            CTC_WK_EN_REG_1 |= BIT(channel);
        }
    }
    else
    {
        /* Disable system wake up */
        if (channel == CTC_CH0)
        {
            CTC_WK_EN_REG &= ~BIT16;
        }
        else
        {
            CTC_WK_EN_REG_1 &= ~BIT(channel);
        }
    }
}

/**
  * @brief  Check the fast mode scan interval param to be set is in avalible range.
  * @param  fast_scan_interval: fast scan interval in units of ms.
  * @retval "TRUE" for in avalible range; "FALSE" for out of avalible range.
  */
bool CTC_CheckFastScanIntervalGuardTime(uint8_t fast_scan_interval)
{
    uint8_t adv_guard_interval = CTC->GUARD_CNT & 0x3FF;
    uint8_t dly_guard_interval = (CTC->GUARD_CNT >> 16) & 0x3FF;
    return (fast_scan_interval * 125 > ((adv_guard_interval + dly_guard_interval + 2)) * 4);
}

/**
  * @brief  Set CTC scan interval for slow mode or fast mode.
  * @param  scan_interval: scan interval in units of 1s/1.024KHz (= 0.9765625 ms)
  *         Configurable range: 0x0~0xFFF (0~4095)
  * @param  CTCMode: CTC_SLOW_MODE or CTC_FAST_MODE.
  * @retval ture: success, false: interval is out of range.
  * @note   Note that slow mode interval should not be lower then fast mode interval,
  *         Fast mode interval should be greater then total guard time for AUXADC.
  */
bool CTC_SetScanInterval(uint16_t scan_interval, E_CTC_MODE CTC_Mode)
{
    /* Check the parameters */
    assert_param(interval <= 4095);
    assert_param(IS_CTC_MODE(CTC_Mode));

    if (CTC_Mode == CTC_SLOW_MODE)
    {
        CTC->SCAN_PERIOD |= scan_interval & 0xFFF;
    }
    else
    {
//      if (CTC_CheckFastScanIntervalGuardTime(scan_interval))
//        {
//            return false;
//        }
        CTC_FAST->SCAN_INI |= scan_interval & 0xFFF;
    }

    if (((CTC->SCAN_PERIOD & 0xFFF) < (CTC_FAST->SCAN_INI & 0xFFF)))
    {
        return false;
    }

    return true;
}

/**
  * @brief  Get touch active count from specified channel under fast mode.
  * @param  channel: specified channel
  * @retval touch active count
  */
uint16_t CTC_GetChannelTouchCount(E_CTC_CH channel)
{
    /* Set strobe signal for the Touch active count value. */
    CTC_FAST->SCAN_INI |= BIT(24);
    platform_delay_us(2);

    FS_TOUCH_CNT_TYPE fs_touch_cnt = {0};

    /* Read touch active count */
    fs_touch_cnt.fs_touch_cnt_01.d32 = CTC_FAST->TOUCH_CNT01;

    /* Return touch active count of specified channel(10 bit) */
    return (fs_touch_cnt.d16[channel] & 0x3FF);
}

/******************* (C) COPYRIGHT 2020 Realtek Semiconductor Corporation *****END OF FILE****/
