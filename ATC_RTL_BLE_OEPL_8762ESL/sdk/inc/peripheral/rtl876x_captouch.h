/**
*********************************************************************************************************
*               Copyright(c) 2020, Realtek Semiconductor Corporation. All rights reserved.
*********************************************************************************************************
* \file     rtl876x_captouch.h
* \brief    The header file of the peripheral CAP TOUCH driver.
* \details  This file provides all CAP TOUCH firmware functions.
* \author   Yuan
* \date     2020-11-11
* \version  v1.0.0
* *********************************************************************************************************
*/

#ifndef _RTL876X_CAPTOUCH_H_
#define _RTL876X_CAPTOUCH_H_

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \addtogroup  IO          Peripheral Drivers
 * \defgroup    CTC         CAP TOUCH CONTROLLER
 * \brief       Manage the CAP TOUCH peripheral functions.
 *
 * \ingroup     IO
 */

/*============================================================================*
 *                         Includes
 *============================================================================*/
#include "rtl876x.h"

/*============================================================================*
 *                         Registers Definitions
 *============================================================================*/

/* Peripheral: CTC */
/* Description: Cap touch register defines */

/* Register: CR */
/* Description: CTC Control Register 0. Offset: 0x00. Address: 0x40007000. */

/* CR[8] :CTC_BASELINE_INI. Baseline initial function enable. 0x1: Enable. 0x0: Disable. */
#define CTC_BASELINE_INI_Pos            (8UL)
#define CTC_BASELINE_INI_Msk            (0x1UL << CTC_BASELINE_INI_Pos)
#define CTC_BASELINE_INI_CLR            (~(CTC_BASELINE_INI_Msk))

/* CR[6:5] :CTC_DEBOUNCE_TIME. De-bounce time setting. 0x1: Enable. 0x0: Disable. */
#define CTC_DEBOUNCE_TIME_Pos           (5UL)
#define CTC_DEBOUNCE_TIME_Msk           (0x03UL << CTC_DEBOUNCE_TIME_Pos)
#define CTC_DEBOUNCE_TIME_CLR           (~(CTC_DEBOUNCE_TIME_Msk))

/* CR[4] :CTC_DEBOUNCE_EN. De-bounce function control. 0x1: Enable. 0x0: Disable. */
#define CTC_DEBOUNCE_EN_Pos             (4UL)
#define CTC_DEBOUNCE_EN_Msk             (0x1UL << CTC_DEBOUNCE_EN_Pos)
#define CTC_DEBOUNCE_EN_CLR             (~(CTC_DEBOUNCE_EN_Msk))

/* CR[0] :CTC_CT_EN. Cap touch function control. 0x1: Enable. 0x0: Disable. */
#define CTC_CT_EN_Pos                   (0UL)
#define CTC_CT_EN_Msk                   (0x1UL << CTC_CT_EN_Pos)
#define CTC_CT_EN_Clr                   (~(CTC_CT_EN_Msk))

/* Register: SCAN_PERIOD */
/* Description: Scan period Register 0. Offset: 0x04. Address: 0x40007004. */

/* SCAN_PERIOD[18:16] :CTC_SAMPLE_AVE. ADC Sampled number for average function. */
#define CTC_SAMPLE_AVE_Pos              (16UL)
#define CTC_SAMPLE_AVE_Msk              (0x7UL << CTC_SAMPLE_AVE_Pos)
#define CTC_SAMPLE_AVE_Clr              (~(CTC_SAMPLE_AVE_Msk))

/* SCAN_PERIOD[11:0] :CTC_SCAN_INTERVAL. Scan interval setting.  */
#define CTC_SCAN_INTERVAL_Pos           (0UL)
#define CTC_SCAN_INTERVAL_Msk           (0xFFFUL << CTC_SCAN_INTERVAL_Pos)
#define CTC_SCAN_INTERVAL_Clr           (~(CTC_SCAN_INTERVAL_Msk))

/* Register: ETC_CR */
/* Description: ETC Register 0. Offset: 0x08. Address: 0x40007008. */

/* ETC_CR[15:12] :CTC_BASELINE_UPDATE_STEP. Baseline update step every time  for all channel. Default: 0001. */
#define CTC_BASELINE_UPDATE_STEP_Pos    (12UL)
#define CTC_BASELINE_UPDATE_STEP_Msk    (0xFUL << CTC_BASELINE_UPDATE_STEP_Pos)
#define CTC_BASELINE_UPDATE_STEP_Clr    (~(CTC_BASELINE_UPDATE_STEP_Msk))

/* ETC_CR[11:8] :CTC_BASELINE_UPD_WT_FACTOR. Baseline update weight factor for all channels. */
#define CTC_BASELINE_UPD_WT_FACTOR_Pos  (8UL)
#define CTC_BASELINE_UPD_WT_FACTOR_Msk  (0xFUL << CTC_BASELINE_UPD_WT_FACTOR_Pos)
#define CTC_BASELINE_UPD_WT_FACTOR_Clr  (~(CTC_BASELINE_UPD_WT_FACTOR_Msk))

/* ETC_CR[7:1] :CTC_ETC_UPDATE_INTERVAL. ETC update interval between 2 scan period(sleep time )for all channel. */
#define CTC_ETC_UPDATE_INTERVAL_Pos     (1UL)
#define CTC_ETC_UPDATE_INTERVAL_Msk     (0x7FUL << CTC_ETC_UPDATE_INTERVAL_Pos)
#define CTC_ETC_UPDATE_INTERVAL_Clr     (~(CTC_ETC_UPDATE_INTERVAL_Msk))

/* ETC_CR[0] :CTC_ETC_EN. Environmental cap tracking calibration function. 0x1: Enable. 0x0: Disable. */
#define CTC_ETC_EN_Pos                  (0UL)
#define CTC_ETC_EN_Msk                  (0x1UL << CTC_ETC_EN_Pos)
#define CTC_ETC_EN_Clr                  (~(CTC_ETC_EN_Msk))

/* Register: SNR_INF */
/* Description: Scan period Register 0. Offset: 0x0C. Address: 0x4000700C. */

/* SNR_INF[31] :CTC_SNR_UPDATE_MODE. SNR update mode. */
#define CTC_SNR_UPDATE_MODE_Pos         (31UL)
#define CTC_SNR_UPDATE_MODE_Msk         (0x1UL << CTC_SNR_UPDATE_MODE_Pos)
#define CTC_SNR_UPDATE_MODE_Clr         (~(CTC_SNR_UPDATE_MODE_Msk))

/* Register: CTC_GUARD_CNT */
/* Description: Guard counter Register. Offset: 0x1C. Address: 0x4000701C. */

/* CTC_GUARD_CNT[20:16] :CTC_DLY_GUARD_INTERVAL. Delay guard interval,1 unit for 32k clock period (31.25us). */
#define CTC_DLY_GUARD_INTERVAL_Pos          (16UL)
#define CTC_DLY_GUARD_INTERVAL_Msk          (0x1FUL << CTC_DLY_GUARD_INTERVAL_Pos)
#define CTC_DLY_GUARD_INTERVAL_Clr          (~(CTC_DLY_GUARD_INTERVAL_Msk))

/* CTC_GUARD_CNT[4:0] :CTC_ADV_GUARD_INTERVAL. Advanced guard interval,1 unit for 32k clock period (31.25us). */
#define CTC_ADV_GUARD_INTERVAL_Pos          (0UL)
#define CTC_ADV_GUARD_INTERVAL_Msk          (0x1FUL << CTC_ADV_GUARD_INTERVAL_Pos)
#define CTC_ADV_GUARD_INTERVAL_Clr          (~(CTC_ADV_GUARD_INTERVAL_Msk))

/*============================================================================*
 *                         Constants
 *============================================================================*/
/**
 * \defgroup    CTC_Exported_Constants Macro Definitions
 * \ingroup     CTC
 */

#define IS_CTC_PERIPH(PERIPH) ((PERIPH) == CTC)
#define IS_CTC_CHANNEL(CH) (((CH) == CTC_Channel0)\
                            ||((CH) == CTC_Channel1))

/* CTC private defines */
#define CTC_ANA_ADC_REG0X_LPAD      *((volatile uint32_t *)0x40007400UL)
#define CTC_ANA_ADC_REG1X_LPAD      *((volatile uint32_t *)0x40007404UL)
#define CTC_ANA_ADC_REG0X_LPSD      *((volatile uint32_t *)0x40007408UL)
#define CTC_ANA_ADC_TIME_SET        *((volatile uint32_t *)0x4000740CUL)
#define CTC_ANA_ADC_POWER_SEQ       *((volatile uint32_t *)0x40007410UL)

#define CTC_CLOCK_SOURCE_1M         1000000
#define CTC_CLOCK_SOURCE_40M        40000000

typedef enum
{
    CTC_SLOW_MODE = 0,
    CTC_FAST_MODE = 1
} E_CTC_MODE;

#define IS_CTC_MODE(mode) (((mode) == CTC_SLOW_MODE) || ((mode) == CTC_FAST_MODE))

typedef enum
{
    CTC_TOUCH_JUDGEMENT_DIFFEENCE = 0,
    CTC_TOUCH_JUDGEMENT_ABSOLUTE = 1,
} E_CTC_TOUCH_JUDGEMENT_MODE;

#define IS_CTC_TOUCH_JUDGEMENT_MODE(mode) (((mode) == CTC_TOUCH_JUDGEMENT_DIFFEENCE) || ((mode) == CTC_TOUCH_JUDGEMENT_ABSOLUTE))

/**
 * \defgroup    CTC_BaselineIni_Enable CTC BaselineIni Enable
 * \{
 * \ingroup     CTC_Exported_Constants
 */
#define CTC_BASELINEINI_DISABLE         0x00
#define CTC_BASELINEINI_ENABLE          0x01
/** \} */

/**
 * \defgroup    CTC_Debounce_Times CTC Debounce Times
 * \{
 * \ingroup     CTC_Exported_Constants
 */
#define CTC_DEBOUNCE_TIMES_2            0x00
#define CTC_DEBOUNCE_TIMES_3            0x01
#define CTC_DEBOUNCE_TIMES_4            0x02
#define CTC_DEBOUNCE_TIMES_5            0x03
/** \} */

/**
 * \defgroup    CTC_Interrupts_Definition CTC Interrupts Definition
 * \{
 * \ingroup     CTC_Exported_Constants
 */
#define CTC_DEBOUNCE_DISABLE            0x00
#define CTC_DEBOUNCE_ENABLE             0x01
/** \} */

/**
 * \defgroup    CTC_Enable CTC Enable
 * \{
 * \ingroup     CTC_Exported_Constants
 */
#define CTC_DISABLE                     0x00
#define CTC_ENABLE                      0x01
/** \} */

/**
 * \defgroup    CTC_Sample_Time CTC Sample Time
 * \{
 * \ingroup     CTC_Exported_Constants
 */
#define CTC_SAMPLE_TIME_AVE_4           0x00
#define CTC_SAMPLE_TIME_AVE_8           0x01
#define CTC_SAMPLE_TIME_AVE_16          0x02
#define CTC_SAMPLE_TIME_AVE_32          0x03
#define CTC_SAMPLE_TIME_AVE_64          0x04
#define CTC_SAMPLE_TIME_AVE_128         0x05
#define CTC_SAMPLE_TIME_AVE_256         0x06
#define CTC_SAMPLE_TIME_AVE_512         0x07
/** \} */

/**
 * \defgroup    CTC_Scan_Interval CTC Scan Interval
 * \{
 * \ingroup     CTC_Exported_Constants
 */
#define CTC_SCAN_INTERVAL_60ms          0x3C
#define CTC_SCAN_INTERVAL_100ms         0x64
#define CTC_SCAN_INTERVAL_140ms         0x8C
#define CTC_SCAN_INTERVAL_180ms         0xB4
#define CTC_SCAN_INTERVAL_220ms         0xDC
#define CTC_SCAN_INTERVAL_260ms         0x104
#define CTC_SCAN_INTERVAL_300ms         0x12C
#define CTC_SCAN_INTERVAL_480ms         0x1E0
/** \} */

/**
 * \defgroup    CTC_Interrupts_Definition CTC Interrupts Definition
 * \{
 * \ingroup     CTC_Exported_Constants
 */
#define CTC_INT_FALSE_TOUCH_CH1     BIT20
#define CTC_INT_FALSE_TOUCH_CH0     BIT19
#define CTC_INT_N_NOISE_THD         BIT18
#define CTC_INT_FIFO_OVERFLOW       BIT17
#define CTC_INT_P_NOISE_THD         BIT16
#define CTC_INT_TOUCH_RELEASE_CH1   BIT9
#define CTC_INT_TOUCH_RELEASE_CH0   BIT8
#define CTC_INT_TOUCH_PRESS_CH1     BIT1
#define CTC_INT_TOUCH_PRESS_CH0     BIT0
/** \} */

#define IS_CTC_INT(INT) (((INT) == CTC_INT_FALSE_TOUCH_CH1) || \
                         ((INT) == CTC_INT_FALSE_TOUCH_CH0) || \
                         ((INT) == CTC_INT_N_NOISE_THD) || \
                         ((INT) == CTC_INT_FIFO_OVERFLOW) || \
                         ((INT) == CTC_INT_P_NOISE_THD) || \
                         ((INT) == CTC_INT_TOUCH_RELEASE_CH1) || \
                         ((INT) == CTC_INT_TOUCH_RELEASE_CH0) || \
                         ((INT) == CTC_INT_TOUCH_PRESS_CH1) || \
                         ((INT) == CTC_INT_TOUCH_PRESS_CH0))

/**
 * \defgroup    CTC_MBIAS_Current CTC MBIAS Current
 * \{
 * \ingroup     CTC_Exported_Constants
 */
#define CTC_MBIAS_0p25uA            (1UL << 0)
#define CTC_MBIAS_0p5uA             (1UL << 1)
#define CTC_MBIAS_1uA               (1UL << 2)
#define CTC_MBIAS_2uA               (1UL << 3)
#define CTC_MBIAS_4uA               (1UL << 4)
#define CTC_MBIAS_8uA               (1UL << 5)
/** \} */

#define IS_CTC_MBIAS(mbias) (((mbias) == CTC_MBIAS_0p25uA) ||\
                             ((mbias) == CTC_MBIAS_0p5uA) ||\
                             ((mbias) == CTC_MBIAS_1uA  ) ||\
                             ((mbias) == CTC_MBIAS_2uA  ) ||\
                             ((mbias) == CTC_MBIAS_4uA  ) ||\
                             ((mbias) == CTC_MBIAS_8uA  ))

/**
 * \defgroup    CTC_FS_Match_Count CTC FS Match Count
 * \{
 * \ingroup     CTC_Exported_Constants
 */
typedef enum
{
    CTC_FS_Match_Cnt_No_Switch = 0x0,
    CTC_FS_Match_Cnt_1DB       = 0x01,
    CTC_FS_Match_Cnt_2DB       = 0x02,
    CTC_FS_Match_Cnt_3DB       = 0x03,
    CTC_FS_Match_Cnt_4DB       = 0x04
} E_CTC_MODE_SWITCH;
/** \} */

#define IS_CTC_FS_Match(cnt) (((cnt) == CTC_FS_Match_Cnt_No_Switch) ||\
                              ((cnt) == CTC_FS_Match_Cnt_1DB) ||\
                              ((cnt) == CTC_FS_Match_Cnt_2DB  ) ||\
                              ((cnt) == CTC_FS_Match_Cnt_3DB  ) ||\
                              ((cnt) == CTC_FS_Match_Cnt_4DB  ))

#define CTC_CH_MAX              2

/**
 * \defgroup    CTC_Channel CTC Channel
 * \{
 * \ingroup     CTC_Exported_Constants
 */
typedef enum
{
    CTC_CH0 = 0,
    CTC_CH1 = 1,
} E_CTC_CH;
/** \} */

/**
 * \cond        private
 * \defgroup    CTC_FS_TOUCH_CNT_TYPE CTC FS Touch Count
 * \{
 */
typedef union _FS_TOUCH_CNT01_TYPE
{
    uint32_t d32;
    struct
    {
        uint32_t TOUCH_ACTIVE_CNT0: 10;
        uint32_t RSVD: 6;
        uint32_t TOUCH_ACTIVE_CNT1: 10;
        uint32_t RSVD2: 6;
    };
} FS_TOUCH_CNT01_TYPE;

typedef union _FS_TOUCH_CNT23_TYPE
{
    uint32_t d32;
    struct
    {
        uint32_t TOUCH_ACTIVE_CNT2: 10;
        uint32_t RSVD: 6;
        uint32_t TOUCH_ACTIVE_CNT3: 10;
        uint32_t RSVD2: 6;
    };
} FS_TOUCH_CNT23_TYPE;

/* combine 0x158 & 0x15C for API usage */
typedef union _FS_TOUCH_CNT_TYPE
{
    uint16_t d16[4];
    struct
    {
        FS_TOUCH_CNT01_TYPE fs_touch_cnt_01;
        FS_TOUCH_CNT23_TYPE fs_touch_cnt_23;
    };
} FS_TOUCH_CNT_TYPE;
/**
 * \}
 * \endcond
 */

/*============================================================================*
 *                         Types
 *============================================================================*/

/**
 * \defgroup    CTC_Exported_Types Init Params Struct
 *
 * \ingroup     CTC
 */

/**
 * \brief       CTC init structure definition.
 *
 * \ingroup     CTC_Exported_Types
 */
typedef struct
{
    uint16_t CTC_BaselineIniEn;             /**< Baseline initial function enable, HW will clear this bit to "0" after baseline initial. */
    uint8_t  CTC_DebounceTime;              /**< Debounce time setting.*/
    uint8_t  CTC_DebounceEn;                /**< Debounce function control: "0":disable; "1": enable.*/
    uint32_t CTC_SampleTime;                /**< ADC sample time, active time. sample_time = 2^(CTC_SampleTime+2)*131072;unit is 131.072KHz;
                                                 sample number=4~512.Default: 110, sample number=256. */
    uint16_t CTC_ScanInterval;              /**< Scan interval :Sleep time between 2 scan burst for all channels, range: 0~4095ms; code: 0~4095(0~0xFFF), unit is 1.024KHz cycle. (1/32 32.768KHz).
                                                 When this register set to 0 or 1, HW will scan continuously and has no sleep time; Recommend value: 60~480ms; Default: 0X3c(60ms).*/
    uint16_t CTC_BaselineUpdateStep;        /**< Baseline update step every time for all channel.
                                                 This parameter can be a value of \ref ENHTIM_Clock_Divider*/
    uint16_t CTC_BaselineWeightFactor;      /**< Baseline update weight factor for all channels; Factor=2 ^ (BASELINE_WT_FACTOR); Default: 0010.
                                                 This parameter can be a value of DISABLE or ENABLE */
    uint8_t CTC_ETCScanInterval;            /**< ETC update interval between 2 scan period(sleep time )for all channel:
                                                 Interval=(ETC_SCAN_INTERVAL+1)*Scan period; (1~128). Default: 0x2; (Update every scan period). */
    uint8_t CTC_ETCEn;                      /**< Environmental cap tracking calibration function: "0":disable; "1": enable.
                                                 This parameter can be a value of DISABLE or ENABLE */
    uint32_t CTC_SNRUpdateMode;             /**< Specifies SNR update mode.
                                                 This parameter default to 0x0, which is update SNR_noise data or touch data based on touch status*/
    uint8_t CTC_ScanChannel;                /**< Specifies scan channel control.
                                                 This parameter default to 0x0000. */
    uint8_t CTC_ScanChannelSwitchMode;      /**< Specifies the scan channel switch control.
                                                 This parameter default to 1, which is auto switch to next channel.*/
    uint32_t CTC_DelayGuardInterval;        /**< Delay guard interval 1 unit for 32k clock period (31.25us).*/
    uint32_t CTC_AdvancedGuardInterval;     /**< Delay guard interval 1 unit for 32k clock period (31.25us).*/
    uint32_t CTC_FastMatchCnt;              /**< Enter Fast mode when the Debounce time matches FS_MATCH_CNT.
                                                 This parameter can be a value of \ref CTC_FS_Match_Coun. */
    uint32_t CTC_FastScanInerval;           /**< Specifies the scan Interval under Fast mode,range: 0~4095ms;
                                                 This parameter can be a range of 0~0xFFF;*/
    uint32_t CTC_FalseTouchCnt;             /**< Specifies the false touch detection after enter Fast mode (1 unit for 1scan).
                                                 This parameter can be a a range of 0~0x3FF. */
    uint32_t CTC_FalseAlarmCnt;             /**< Specifies the false alarm detection after enter Fast mode (1 unit for 1scan).
                                                 This parameter can be a a range of 0~0xF.*/
    uint32_t CTC_ReleaseActiveCnt;          /**< Specifies the determine counter value when leave Fast mode after touch release.
                                                 This parameter can be a a range of 0~0x3FF.*/
} CTC_InitTypeDef;

/**
 * \brief       CTC channel init structure definition.
 *
 * \ingroup     CTC_Exported_Types
 */
typedef struct
{
    uint8_t CTC_ChannelEn;          /**< Cap Sensor activity control of channelx.
                                         This parameter can be a value of ENABLE or DISABLE. */
    uint16_t CTC_BaselineData;      /**< Specifies digital baseline data of channelx.
                                         This parameter can be 0x0 ~ 0xFFF. */
    uint16_t CTC_DifferenceTouchThd;/**< Difference threshold data of touch judgement for channelx.
                                         This parameter can be 0x0 ~ 0xFFF. */
    uint16_t CTC_AbsoluteTouchThd;  /**< Specifies absolute threshold data of touch judgement for channelx.
                                         This parameter can be 0x0 ~ 0xFFF. */
    uint32_t CTC_PNoiseThd;         /**< The environmental pasitive noise threshold: the positive maximum capacitance change of raw data that is still considered an environmental change.
                                         This parameter can be 0x0 ~ 0xFF. */
    uint32_t CTC_NNoiseThd;         /**< The environmental negative noise threshold: the negative maximum capacitance change of raw data that is still considered an environmental change.
                                         This parameter can be 0x0 ~ 0xFF. */
    uint8_t CTC_MBias;              /**< Specifies Channelx mbias current tuning(sensitivity tuning) Touch bias current [5:0]: 8uA/4Ua/2uA/1uA/0.5uA/0.25uA.
                                         This parameter can be a value of \ref CTC_MBIAS_Current. */
} CTC_ChannelInitTypeDef;

/*============================================================================*
 *                         Functions
 *============================================================================*/

/**
 * \defgroup    CTC_Exported_Functions  Peripheral APIs
 * \{
 * \ingroup     CTC
 */

/**
 * \brief   Deinitialize the Cap Touch peripheral registers to their default reset values.
 * \return  None.
 */
void CTC_DeInit(void);

/**
 * \brief  Enable the CTC peripheral IP clock and function.
 * \param  clock_source: CTC peripheral clock source.
 *         This parameter can be one of the following values.
 *         \arg  CTC_CLOCK_SOURCE_1M: 1M clock source
 *         \arg  CTC_CLOCK_SOURCE_40M: 40M clock source
 * \return None.
 */
void CTC_RCCConfig(uint32_t clock_source);

/**
  * \brief      Initializes the CTC peripheral according to
  *             the specified parameters in the CTC_InitStruct.
  * \param[in]  CTCx: CTC peripheral.
  * \param[in]  CTC_InitStruct: Pointer to a CTC_InitTypeDef structure that contains
  *                             the configuration information for the specified CTC peripheral.
  * \return     None.
  */
void CTC_Init(CTC_TypeDef *CTCx, CTC_InitTypeDef *CTC_InitStruct);

/**
  * \brief  Initializes the specified channel.
  * \param  CTC_Channelx: specified channel
  * \param  CTC_ChannelInitStruct: pointer to a CTC_ChannelInitTypeDef structure that
  *         contains the configuration information for the specified CTC channel.
  * \return None
  */
void CTC_ChannelInit(CTC_ChannelTypeDef *CTC_Channelx,
                     CTC_ChannelInitTypeDef *CTC_ChannelInitStruct);

/**
  * \brief  Fills each CTC_InitStruct member with its default value.
  * \param  CTC_InitStruct: Pointer to a CTC_InitStruct structure which will be initialized.
  * \return None.
  */
void CTC_StructInit(CTC_InitTypeDef *CTC_InitStruct);

/**
  * \brief  Fills each CTC_InitStruct member with its default value.
  * \param  CTC_InitStruct: Pointer to a CTC_InitStruct structure which will be initialized.
  * \return None.
  */
void CTC_ChannelStructInit(CTC_ChannelInitTypeDef *CTC_ChannelInitStruct);

/**
 * \brief     Enable or disable the specified Cap Touch interrupt.
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
  * \param  NewState: New state of the Cap Touch interrupt.
  *         This parameter can be: ENABLE or DISABLE.
  * \return None
  */
void CTC_INTConfig(uint32_t CTC_INT, FunctionalState NewState);

/**
  * \brief  Enables or disables the specified Cap Touch peripheral.
  * \param  ENHTIMx: Cap Touch peripheral.
  * \param  NewState: New state of the Cap Touch peripheral.
  *         This parameter can be: ENABLE or DISABLE.
  * \retutn None.
  */
void CTC_Cmd(CTC_TypeDef *CTCx, FunctionalState NewState);

/**
 * \brief  Enable or disable system wake up of CTC.
 * \param  NewState: new state of the wake up function.
 *         This parameter can be: ENABLE or DISABLE.
 * \return None
 */
void CTC_SystemWakeupConfig(E_CTC_CH channel, FunctionalState NewState);

/**
  * \brief  Set CTC Scan interval for slow mode or fast mode.
  * \param  scan_interval: scan interval in units of 1s/1.024KHz (= 0.9765625 ms)
  *         Configurable range: 0x0~0xFFF (0~4095)
  *         Note that slow mode interval should not be lower then fast mode interval,
  *         Fast mode interval should be greater then total guard time for auxADC
  * \param  CTCMode: CTC_SLOW_MODE or CTC_FAST_MODE.
  * \return ture: success, false: interval is out of range.
  */
bool CTC_SetScanInterval(uint16_t scan_interval, E_CTC_MODE CTCMode);

/**
  * \brief  Get touch active count from specified channel under fast mode.
  * \param  channel: specified channel
  * \return touch active count
  */
uint16_t CTC_GetChannelTouchCount(E_CTC_CH channel);

/**
  * \brief  Get Cap Touch baseline init status.
  * \return Cap Touch enable status. 1: Baseline initial, 0: Baseline initial done.
  */
__STATIC_INLINE FlagStatus CTC_GetBaselineInistatus(void)
{
    return (FlagStatus)((CTC->CR & BIT8) >> 8);
}

/**
  * \brief  Get Cap Touch enable status.
  * \return Cap Touch enable status. 1: Running, 0: Non-runnung.
  */
__STATIC_INLINE bool CTC_IsRunning(void)
{
    return (CTC->CR & BIT0);
}

/**
  * \brief  Read Cap Touch mode.
  * \retval Cap Touch mode. 1: Fast mode, 0: Slow mode
  */
__STATIC_INLINE bool CTC_IsFastMode(void)
{
    return (CTC_FAST->TOUCH_SR & BIT8) >> 8;
}

/**
  * \brief  Check legality of specified channel.
  * \param  channel: specified channel.
  * \return 1: Allowed, 0: Not allowed.
  */
__STATIC_INLINE bool CTC_IsChannelAllowed(E_CTC_CH channel)
{
    return (channel <= CTC_CH_MAX);
}

/**
  * \brief  Set relative touch threshold for related channel.
  * \param  channel: specified channel
  * \param  threshold: related threshold value.
  * \return None
  */
__STATIC_INLINE void CTC_SetCHDifferenceThd(CTC_ChannelTypeDef *CTC_Channelx, uint16_t threshold)
{
    /* Check the parameters */
//    configASSERT(CTC_IsChAllowed(channel));
//    configASSERT(threshold <= 4095);
//    configASSERT(!CTC_IsRunning());

    CTC_Channelx->CR |= (uint32_t)threshold << 16;
}

/**
  * \brief  Get absolute threshold of touch judgement for specified channel.
  * \param  channel: specified channel
  * \param  threshold: Threshold value
  * \return None
  */
__STATIC_INLINE void CTC_SetCHAbsoluteThd(CTC_ChannelTypeDef *CTC_Channelx, uint16_t threshold)
{
    /* Check the parameters */
//    configASSERT(CTC_IsChAllowed(channel));
//    configASSERT(threshold <= 4095);
//    configASSERT(!CTC_IsRunning());

    CTC_Channelx->TOUCH_TH |= threshold & 0xFFF;
}

/**
  * \brief  Set P noise threshold for related channel.
  * \param  channel: specified channel
  * \param  p_noise_threshold: Pnoise Threshold value.
  * \retval None
  */
__STATIC_INLINE void CTC_SetCHPNoiseThd(CTC_ChannelTypeDef *CTC_Channelx, uint8_t p_noise_threshold)
{
    /* Check the parameters */
//    configASSERT(CTC_IsChAllowed(channel));
//    configASSERT(p_noise_threshold <= 255);
//    configASSERT(!CTC_IsRunning());

    CTC_Channelx->TOUCH_TH |= (uint32_t)p_noise_threshold << 16;
}

/**
  * \brief  Set N noise threshold for related channel.
  * \param  channel: specified channel
  * \param  n_noise_threshold: Nnoise Threshold value.
  * \return None
  */
__STATIC_INLINE void CTC_SetCHNNoiseThd(CTC_ChannelTypeDef *CTC_Channelx, uint8_t n_noise_threshold)
{
    /* Check the parameters */
//    configASSERT(CTC_IsChAllowed(channel));
//    configASSERT(n_noise_threshold <= 255);
//    configASSERT(!CTC_IsRunning());

    CTC_Channelx->TOUCH_TH |= (uint32_t)n_noise_threshold << 24;
}

/**
 * \brief   Set mbias current for specified channel.
 * \param   channel: specified channel
 * \param   mbias: mbias value, relate current = 0.25*mbias.
 * \return  None.
 */
__STATIC_INLINE void CTC_SetChannelMbias(CTC_ChannelTypeDef *CTC_Channelx, uint8_t mbias)
{
//    /* Check the parameters */
//    configASSERT(CTC_IsChAllowed(channel));
//    configASSERT(mbias < 64);
//    configASSERT(!CTC_IsRunning());

    CTC_Channelx->MBIAS |= mbias & 0x3F;
}

/**
  * \brief  Get status for specified channel.
  * \param  channel: specified channel
  * \return channel enable status.
  */
__STATIC_INLINE FlagStatus CTC_GetChannelStatus(CTC_ChannelTypeDef *CTC_Channelx)
{
    return (FlagStatus)(CTC_Channelx->CR & 0x1);
}

/**
  * \brief  Get Baseline data from specified channel.
  * \param  channel: Specified channel.
  * \return Baseline data.
  */
__STATIC_INLINE uint16_t CTC_GetChannelBaseline(CTC_ChannelTypeDef *CTC_Channelx)
{
    return ((CTC_Channelx->CR & 0xFFF0) >> 4);
}

/**
  * \brief  Get relative threshold of touch judgement for specified channel.
  * \param  channel: specified channel
  * \return Difference threshold of specified channel.
  */
__STATIC_INLINE uint16_t CTC_GetCHDifferenceThd(CTC_ChannelTypeDef *CTC_Channelx)
{
    return ((CTC_Channelx->CR >> 16) & 0xFFF);
}

/**
  * \brief  Get Absolute threshold of touch judgement for specified channel.
  * \param  channel: specified channel
  * \return Difference threshold of specified channel.
  */
__STATIC_INLINE uint16_t CTC_GetCHAbsoluteThd(CTC_ChannelTypeDef *CTC_Channelx)
{
    return (CTC_Channelx->TOUCH_TH & 0xFFF);
}

/**
  * \brief  Get positive noise threshold for specified channel.
  * \param  CTC_Channelx: specified channel
  * \return Noise threshold of specified channel.
  */
__STATIC_INLINE uint8_t CTC_GetCHPNoiseThd(CTC_ChannelTypeDef *CTC_Channelx)
{
    return ((CTC_Channelx->TOUCH_TH >> 16) & 0xFF);
}

/**
  * \brief  Get negative noise threshold for specified channel.
  * \param  CTC_Channelx: specified channel
  * \return Noise threshold of specified channel.
  */
__STATIC_INLINE uint8_t CTC_GetCHNNoiseThd(CTC_ChannelTypeDef *CTC_Channelx)
{
    return ((CTC_Channelx->TOUCH_TH >> 24) & 0xFF);
}

/**
  * \brief  Get ave data from specified channel.
  * \param  channel: specified channel
  * \retval Average data
  */
__STATIC_INLINE uint16_t CTC_GetChannelAveData(CTC_ChannelTypeDef *CTC_Channelx)
{
    return (CTC_Channelx->DATA & 0xFFF);
}

/**
 * \brief     Get Cap Touch status under fast mode.
 * \param[in] channel: Specifies the Cap Touch channel.
 *            This parameter can be one of the following values:
 *            \arg CTC_CH3: Channel 3.
 *            \arg CTC_CH2: Channel 2.
 *            \arg CTC_CH1: Channel 1.
 *            \arg CTC_CH0: Channel 0.
 * \return    The new state of the Cap Touch channel(SET or RESET). bit4~bit0 each bit represent one channel.
 *            \retval SET: tounch.
 *            \retval RESET: un-tounch.
 */
__STATIC_INLINE FlagStatus CTC_GetChannelTouchStatus(E_CTC_CH channel)
{
    if (CTC_IsFastMode())
    {
        return (FlagStatus)((CTC_FAST->TOUCH_SR & 0xF) >> (uint8_t)channel);
    }
    else
    {
        return RESET;
    }
}

/**
 * \brief     Check whether the Cap Touch interrupt has occurred or not.
 * \param[in] CTCx: The Cap Touch peripheral.
 * \param[in] CTC_INT: Specifies the Cap Touch interrupt has occurred or not.
 *            This parameter can be any combination of the following values:
 *            \arg CTC_INT_FALSE_TOUCH_CH1: Channel 1 Touch cnt reach false touch cnt interrupt.
 *            \arg CTC_INT_FALSE_TOUCH_CH0: Channel 0 Touch cnt reach false touch cnt interrupt.
 *            \arg CTC_INT_N_NOISE_THD: Negative noise threshold overflow interrupt.
 *            \arg CTC_INT_FIFO_OVERFLOW: Raw code FIFO over flow interrupt
 *            \arg CTC_INT_P_NOISE_THD: Positive noise threshold overflow interrupt.
 *            \arg CTC_INT_TOUCH_RELEASE_CH1: Channel 1 release interrupt.
 *            \arg CTC_INT_TOUCH_RELEASE_CH0: Channel 0 release interrupt.
 *            \arg CTC_INT_TOUCH_PRESS_CH1: Channel 1 press interrupt.
 *            \arg CTC_INT_TOUCH_PRESS_CH0: Channel 0 press interrupt.
 * \return    The new state of the CTC_INT(SET or RESET).
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void cap_touch_demo(void)
 * {
 *     ITStatus int_status = CTC_GetINTStatus(CTC, CTC_INT_TOUCH_PRESS_CH1);
 * }
 * \endcode
 */
__STATIC_INLINE ITStatus CTC_GetINTStatus(CTC_TypeDef *CTCx, uint32_t CTC_INT)
{
    /* Check the parameters */
    assert_param(IS_CTC_ALL_PERIPH(CTCx));

    ITStatus itstatus = RESET;

    if (CTCx->INT_SR & (CTC_INT))
    {
        itstatus = SET;
    }

    return itstatus;
}

/**
 * \brief     Get CTC Raw Interrupt (without hardware mask) Status.
 * \param[in] CTCx: The Cap Touch peripheral.
 * \param[in] CTC_INT: Specifies the Cap Touch interrupt has occurred or not.
 *            This parameter can be any combination of the following values:
 *            \arg CTC_INT_FALSE_TOUCH_CH1: Channel 1 Touch cnt reach false touch cnt interrupt.
 *            \arg CTC_INT_FALSE_TOUCH_CH0: Channel 0 Touch cnt reach false touch cnt interrupt.
 *            \arg CTC_INT_N_NOISE_THD: Negative noise threshold overflow interrupt.
 *            \arg CTC_INT_FIFO_OVERFLOW: Raw code FIFO over flow interrupt
 *            \arg CTC_INT_P_NOISE_THD: Positive noise threshold overflow interrupt.
 *            \arg CTC_INT_TOUCH_RELEASE_CH1: Channel 1 release interrupt.
 *            \arg CTC_INT_TOUCH_RELEASE_CH0: Channel 0 release interrupt.
 *            \arg CTC_INT_TOUCH_PRESS_CH1: Channel 1 press interrupt.
 *            \arg CTC_INT_TOUCH_PRESS_CH0: Channel 0 press interrupt.
 * \return    Raw interrupt status of the CTC_INT(SET or RESET).
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void cap_touch_demo(void)
 * {
 *     ITStatus int_status = CTC_GetRawINTStatus(CTC, CTC_INT_TOUCH_PRESS_CH1);
 * }
 * \endcode
 */
__STATIC_INLINE ITStatus CTC_GetRawINTStatus(CTC_TypeDef *CTCx, uint32_t CTC_INT)
{
    /* Check the parameters */
    assert_param(IS_CTC_ALL_PERIPH(CTCx));

    ITStatus itstatus = RESET;

    if (CTCx->RAW_INT_SR & (CTC_INT))
    {
        itstatus = SET;
    }

    return itstatus;
}

/**
 * \brief     Clear CTC interrupt.
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
 * \return    None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void cap_touch_demo(void)
 * {
 *     CTC_ClearINTPendingBit(CTC, CTC_INT_TOUCH_PRESS_CH0);
 * }
 * \endcode
 */
__STATIC_INLINE void CTC_ClearINTPendingBit(CTC_TypeDef *CTCx, uint32_t CTC_INT)
{
    /* Check the parameters */
    assert_param(IS_CTC_ALL_PERIPH(CTCx));

    /* Clear the IT */
    CTCx->INT_SCLR |= CTC_INT;
}

/**
 * \brief   Clear ALL CTC interrupt.
 * \param   None.
 * \return  None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void cap_touch_demo(void)
 * {
 *     CTC_ClearAllINT();
 * }
 * \endcode
 */
__STATIC_INLINE void CTC_ClearAllINT(void)
{
    /* Clear the IT */
    CTC->INT_CLR |= BIT0;
}
/** \} */ /* End of group CODEC_Exported_Functions */

#ifdef __cplusplus
}
#endif

#endif /* _RTL876X_CAPTOUCH_H_ */


/******************* (C) COPYRIGHT 2020 Realtek Semiconductor Corporation *****END OF FILE****/
