/**
*********************************************************************************************************
*               Copyright(c) 2024, Realtek Semiconductor Corporation. All rights reserved.
*********************************************************************************************************
* \file     rtl876x_codec.h
* \brief    The header file of the peripheral CODEC driver.
* \details  This file provides all CODEC firmware functions.
* \author   Yuan
* \date     2024-01-22
* \version  v2.1.1
* *********************************************************************************************************
*/

/*============================================================================*
 *               Define to prevent recursive inclusion
 *============================================================================*/
#ifndef _RTL876X_CODEC_H_
#define _RTL876X_CODEC_H_

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \addtogroup  IO          Peripheral Drivers
 * \defgroup    CODEC       CODEC
 *
 * \brief       Manage the CODEC peripheral functions.
 *
 * \ingroup     IO
 */

/*============================================================================*
 *                         Includes
 *============================================================================*/
#include "rtl876x.h"

/*============================================================================*
 *                         Types
 *============================================================================*/
/**
 * \defgroup    CODEC_Exported_Types    CODEC Exported Types
 *
 * \ingroup     CODEC
 */

/**
 * \brief       CODEC initialize parameters.
 *
 * \ingroup     CODEC_Exported_Types
 */
typedef struct
{
    /* Basic parameters section */
    uint32_t CODEC_SampleRate;          /*!< Specify the sample rate.
                                                This parameter can be a value of \ref CODEC_Sample_Rate_Definition. */
    uint32_t CODEC_DmicClock;           /*!< Specify the dmic clock.
                                                This parameter can be a value of \ref CODEC_DMIC_Clock_Type. */
    uint32_t CODEC_I2SFormat;           /*!< Specify the I2S Tx/Rx format of codec port.
                                                This parameter can be a value of \ref CODEC_I2S_Data_Format. */
    uint32_t CODEC_I2SDataWidth;        /*!< Specify the I2S Tx/Rx data width of codec port.
                                                This parameter can be a value of \ref CODEC_I2S_Data_Width. */
    uint32_t CODEC_I2SChSequence;       /*!< Specify the I2S Tx/Rx channel sequence.
                                                This parameter can be a value of \ref CODEC_I2S_CH. */
    uint32_t CODEC_MicBIAS;             /*!< Specify the MICBIAS voltage.
                                                This parameter can be a value of \ref CODEC_MIC_BIAS_Config. */
    uint32_t CODEC_MicBstGain;          /*!< Specify the MICBST gain.
                                                This parameter can be a value of \ref CODEC_MICBST_Gain. */
    uint32_t CODEC_MicBstMode;          /*!< Specify the MICBST mode.
                                                This parameter can be a value of \ref CODEC_MICBST_Mode. */

    /* MIC channel 0 initialization parameters section */
    uint32_t CODEC_Ch0Mute;             /*!< Specify the channel 0 dmic mute status.
                                                This parameter can be a value of \ref CODEC_Ch0_Mute. */
    uint32_t CODEC_Ch0MicType;          /*!< Specify the channel 0 mic type, which can be dmic or amic.
                                                This parameter can be a value of \ref CODEC_Ch0_Mic_Type. */
    uint32_t CODEC_Ch0DmicDataLatch;    /*!< Specify the channel 0 dmic data latch type.
                                                This parameter can be a value of \ref CODEC_DMIC_Data_Latch_Edge. */
    uint32_t CODEC_Ch0AdGain;           /*!< Specify the channel 0 ADC digital volume. */
    uint32_t CODEC_Ch0BoostGain;        /*!< Specify the channel 0 boost gain.
                                                This parameter can be a value of \ref CODEC_Ch0_Boost_Gain. */
    uint32_t CODEC_Ch0ZeroDetTimeout;   /*!< Specify the channel 0 zero detection timeout mode control.
                                                This parameter can be a value of \ref CODEC_Ch0_ADC_Zero_Detection_Timeout. */

} CODEC_InitTypeDef;

/** End of CODEC_Exported_Types
  * \}
  */

/*============================================================================*
 *                         Register Defines
 *============================================================================*/

/* Peripheral: CODEC */
/* Description: CODEC register defines */

/* Register: ANA_CR0 ----------------------------------------------------------*/
/* Description: Control register 0. Offset: 0x00. Address: 0x40022000UL. */

/* ANA_CR0[10]: ADC_ANA_POW. 0x1: Powen on. 0x0: Power down. */
#define CODEC_ADC_ANA_POW_POS               (10)
#define CODEC_ADC_ANA_POW_MSK               (0x1 << CODEC_ADC_ANA_POW_POS)
#define CODEC_ADC_ANA_POW_CLR               (~CODEC_ADC_ANA_POW_MSK)
/* ANA_CR0[9]: DTSDM_CLK_EN. 0x1: Powen on. 0x0: Power down. */
#define CODEC_DTSDM_CLK_EN_POS              (9)
#define CODEC_DTSDM_CLK_EN_MSK              (0x1 << CODEC_DTSDM_CLK_EN_POS)
#define CODEC_DTSDM_CLK_EN_CLR              (~CODEC_DTSDM_CLK_EN_MSK)
/* ANA_CR0[0]: DAC_ADDACK_POW. 0x1: Powen on. 0x0: Power down. */
#define CODEC_DAC_ADDACK_POW_POS            (0)
#define CODEC_DAC_ADDACK_POW_MSK            (0x1 << CODEC_DAC_ADDACK_POW_POS)
#define CODEC_DAC_ADDACK_POW_CLR            (~CODEC_DAC_ADDACK_POW_MSK)

/* Register: ANA_CR1 ----------------------------------------------------------*/
/* Description: Control register 1. Offset: 0x04. Address: 0x40022004UL. */

/* ANA_CR1[27:26]: VREF_SEL. */
#define CODEC_VREF_SEL_POS                  (26)
#define CODEC_VREF_SEL_MSK                  (0x3 << CODEC_VREF_SEL_POS)
#define CODEC_VREF_SEL_CLR                  (~CODEC_VREF_SEL_MSK)
#define CODEC_VREF_SEL_DEFALUT_MSK          (0x2 << CODEC_VREF_SEL_POS)
/* ANA_CR1[25]: MICBST_POW. */
#define CODEC_MICBST_POW_POS                (25)
#define CODEC_MICBST_POW_MSK                (0x1 << CODEC_MICBST_POW_POS)
#define CODEC_MICBST_POW_CLR                (~CODEC_MICBST_POW_MSK)
/* ANA_CR1[21:20]: MICBST_MUTE. */
#define CODEC_MICBST_MUTE_POS               (20)
#define CODEC_MICBST_MUTE_MSK               (0x3 << CODEC_MICBST_MUTE_POS)
#define CODEC_MICBST_MUTE_CLR               (~CODEC_MICBST_MUTE_MSK)
#define CODEC_MICBST_MUTE_MIC_MSK           (0x2 << CODEC_MICBST_MUTE_POS)
/* ANA_CR1[17:16]: MICBST_GAIN. */
#define CODEC_MICBST_GAIN_POS               (16)
#define CODEC_MICBST_GAIN_MSK               (0x3 << CODEC_MICBST_GAIN_POS)
#define CODEC_MICBST_GAIN_CLR               (~CODEC_MICBST_GAIN_MSK)
/* ANA_CR1[15]: MICBST_VREF_POW. */
#define CODEC_MICBST_VREF_POW_POS           (15)
#define CODEC_MICBST_VREF_POW_MSK           (0x1 << CODEC_MICBST_VREF_POW_POS)
#define CODEC_MICBST_VREF_POW_CLR           (~CODEC_MICBST_VREF_POW_MSK)
/* ANA_CR1[14]: MICBST_ENDFL. */
#define CODEC_MICBST_ENDFL_POS              (14)
#define CODEC_MICBST_ENDFL_MSK              (0x1 << CODEC_MICBST_ENDFL_POS)
#define CODEC_MICBST_ENDFL_CLR              (~CODEC_MICBST_ENDFL_MSK)
/* ANA_CR1[13:11]: MICBIAS_VSET. */
#define CODEC_MICBIAS_VSET_POS              (11)
#define CODEC_MICBIAS_VSET_MSK              (0x7 << CODEC_MICBIAS_VSET_POS)
#define CODEC_MICBIAS_VSET_CLR              (~CODEC_MICBIAS_VSET_MSK)
/* ANA_CR1[10]: MICBIAS_POW. 0x1: Powen on. 0x0: Power down. */
#define CODEC_MICBIAS_POW_POS               (10)
#define CODEC_MICBIAS_POW_MSK               (0x1 << CODEC_MICBIAS_POW_POS)
#define CODEC_MICBIAS_POW_CLR               (~CODEC_MICBIAS_POW_MSK)
/* ANA_CR1[9]: MICBIAS_ENCHX. */
#define CODEC_MICBIAS_ENCHX_POS             (9)
#define CODEC_MICBIAS_ENCHX_MSK             (0x1 << CODEC_MICBIAS_ENCHX_POS)
#define CODEC_MICBIAS_ENCHX_CLR             (~CODEC_MICBIAS_ENCHX_MSK)

/* Register: ANA_CR2 ----------------------------------------------------------*/
/* Description: Control register 2. Offset: 0x08. Address: 0x40022008UL. */

/* ANA_CR2[15]: MICBIAS_LIMIT. */
#define CODEC_MICBIAS_LIMIT_POS             (15)
#define CODEC_MICBIAS_LIMIT_MSK             (0x1 << CODEC_MICBIAS_LIMIT_POS)
#define CODEC_MICBIAS_LIMIT_CLR             (~CODEC_MICBIAS_LIMIT_MSK)
#define CODEC_MICBIAS_LIMIT_DEFALUT_MSK     (0)

/* ANA_CR2[14]: MICBIAS_POWSHDT. */
#define CODEC_MICBIAS_POWSHDT_POS           (14)
#define CODEC_MICBIAS_POWSHDT_MSK           (0x1 << CODEC_MICBIAS_POWSHDT_POS)
#define CODEC_MICBIAS_POWSHDT_CLR           (~CODEC_MICBIAS_POWSHDT_MSK)
#define CODEC_MICBIAS_POWSHDT_DEFALUT_MSK   (0)

/* ANA_CR2[13:12]: MICBIAS_OCSEL. */
#define CODEC_MICBIAS_OCSEL_POS             (12)
#define CODEC_MICBIAS_OCSEL_MSK             (0x3 << CODEC_MICBIAS_OCSEL_POS)
#define CODEC_MICBIAS_OCSEL_CLR             (~CODEC_MICBIAS_OCSEL_MSK)
#define CODEC_MICBIAS_OCSEL_DEFALUT_MSK     (0x1 << CODEC_MICBIAS_OCSEL_POS)

/* ANA_CR2[11:8]: MICBIAS_COUNT. */
#define CODEC_MICBIAS_COUNT_POS             (8)
#define CODEC_MICBIAS_COUNT_MSK             (0xF << CODEC_MICBIAS_COUNT_POS)
#define CODEC_MICBIAS_COUNT_CLR             (~CODEC_MICBIAS_COUNT_MSK)
#define CODEC_MICBIAS_COUNT_DEFALUT_MSK     (0x1 << CODEC_MICBIAS_COUNT_POS)

/* Register: CR0 --------------------------------------------------------------*/
/* Description: audio Control register. Offset: 0x100. Address: 0x4002_2100UL. */

/* CR0[0]: AUDIO_IP_EN. */
#define CODEC_AUDIO_IP_EN_POS               (0)
#define CODEC_AUDIO_IP_EN_MSK               (0x1 << CODEC_AUDIO_IP_EN_POS)
#define CODEC_AUDIO_IP_EN_CLR               (~CODEC_AUDIO_IP_EN_MSK)

/* Register: ANA_CR1 -----------------------------------------------------------*/
/* Description: analog Control register. Offset: 0x104. Address: 0x4002_2104UL. */

/* ANA_CR1[3]: CKX_MICBIAS_EN. */
#define CODEC_CKX_MICBIAS_EN_POS           (3)
#define CODEC_CKX_MICBIAS_EN_MSK           (0x1 << CODEC_CKX_MICBIAS_EN_POS)
#define CODEC_CKX_MICBIAS_EN_CLR           (~CODEC_CKX_MICBIAS_EN_MSK)

/* Register: CLK_CR1 ----------------------------------------------------------*/
/* Description: clock Control register. Offset: 0x108. Address: 0x4002_2108UL. */

/* CLK_CR1[12]: AD_FILTER_CH0_CLK. 0x1: Enable clock. 0x0: Disable clock. */
#define CODEC_AD_FILTER_CH0_CLK_POS         (12)
#define CODEC_AD_FILTER_CH0_CLK_MSK         (0x1 << CODEC_AD_FILTER_CH0_CLK_POS)
#define CODEC_AD_FILTER_CH0_CLK_CLR         (~CODEC_AD_FILTER_CH0_CLK_MSK)
/* CLK_CR1[11]: AD_ANA_CLK_EN. 0x1: Enable ADC analog clock. 0x0: Disable ADC analog clock. */
#define CODEC_AD_ANA_CLK_EN_POS             (11)
#define CODEC_AD_ANA_CLK_EN_MSK             (0x1 << CODEC_AD_ANA_CLK_EN_POS)
#define CODEC_AD_ANA_CLK_EN_CLR             (~CODEC_AD_ANA_CLK_EN_MSK)
/* CLK_CR1[10]: AD_FIFO_CLK_EN. 0x1: Enable ADC FIFO clock. 0x0: Disable ADC FIFO clock. */
#define CODEC_AD_FIFO_CLK_EN_POS            (10)
#define CODEC_AD_FIFO_CLK_EN_MSK            (0x1 << CODEC_AD_FIFO_CLK_EN_POS)
#define CODEC_AD_FIFO_CLK_EN_CLR            (~CODEC_AD_FIFO_CLK_EN_MSK)
/* CLK_CR1[6]: AD_CH0_CLK. 0x1: Enable clock. 0x0: Disable clock. */
#define CODEC_AD_CH0_CLK_POS                (6)
#define CODEC_AD_CH0_CLK_MSK                (0x1 << CODEC_AD_CH0_CLK_POS)
#define CODEC_AD_CH0_CLK_CLR                (~CODEC_AD_CH0_CLK_MSK)

/* Register: CLK_CR2 ----------------------------------------------------------*/
/* Description: clock Control register. Offset: 0x10C. Address: 0x4002_210CUL. */

/* CLK_CR2[4]: DMIC1_CLK_SRC. 0x1: 8M. 0x0: 10M. */
#define CODEC_DMIC1_CLK_SRC_POS             (4)
#define CODEC_DMIC1_CLK_SRC_MSK             (0x1 << CODEC_DMIC1_CLK_SRC_POS)
#define CODEC_DMIC1_CLK_SRC_CLR             (~CODEC_DMIC1_CLK_SRC_MSK)
/* CLK_CR2[3]: DMIC1_CLK_EN. 0x1: Enable clock. 0x0: Disable clock. */
#define CODEC_DMIC1_CLK_EN_POS              (3)
#define CODEC_DMIC1_CLK_EN_MSK              (0x1 << CODEC_DMIC1_CLK_EN_POS)
#define CODEC_DMIC1_CLK_EN_CLR              (~CODEC_DMIC1_CLK_EN_MSK)
/* CLK_CR2[2:0]: DMIC1_CLK_SEL. */
#define CODEC_DMIC1_CLK_SEL_POS             (0)
#define CODEC_DMIC1_CLK_SEL_MSK             (0x7 << CODEC_DMIC1_CLK_SEL_POS)
#define CODEC_DMIC1_CLK_SEL_CLR             (~CODEC_DMIC1_CLK_SEL_MSK)

/* Register: CLK_CR3 ----------------------------------------------------------*/
/* Description: clock Control register. Offset: 0x110. Address: 0x4002_2110UL. */

/* CLK_CR3[13:12]: AD_LPF_CLK_SEL. */
#define CODEC_AD_LPF_CLK_SEL_POS            (12)
#define CODEC_AD_LPF_CLK_SEL_MSK            (0x3 << CODEC_AD_LPF_CLK_SEL_POS)
#define CODEC_AD_LPF_CLK_SEL_CLR            (~CODEC_AD_LPF_CLK_SEL_MSK)
//#define CODEC_AD_LPF_CLK_SEL_DEFAULT        (~CODEC_AD_LPF_CLK_SEL_MSK)

/* CLK_CR3[10]: ADC_LATCH_PHASE. */
#define CODEC_ADC_LATCH_PHASE_POS           (10)
#define CODEC_ADC_LATCH_PHASE_MSK           (0x1 << CODEC_ADC_LATCH_PHASE_POS)
#define CODEC_ADC_LATCH_PHASE_CLR           (~CODEC_ADC_LATCH_PHASE_MSK)

/* CLK_CR3[9]: AD_ANA_CLK_SEL. */
#define CODEC_AD_ANA_CLK_SEL_POS            (9)
#define CODEC_AD_ANA_CLK_SEL_MSK            (0x1 << CODEC_AD_ANA_CLK_SEL_POS)
#define CODEC_AD_ANA_CLK_SEL_CLR            (~CODEC_AD_ANA_CLK_SEL_MSK)

/* CLK_CR3[8]: ANA_CLK_RATE. */
#define CODEC_ANA_CLK_RATE_POS              (8)
#define CODEC_ANA_CLK_RATE_MSK              (0x1 << CODEC_ANA_CLK_RATE_POS)
#define CODEC_ANA_CLK_RATE_CLR              (~CODEC_ANA_CLK_RATE_MSK)

/* CLK_CR3[3:0]: SAMPLE_RATE. */
#define CODEC_SAMPLE_RATE_POS               (0)
#define CODEC_SAMPLE_RATE_MSK               (0xF << CODEC_SAMPLE_RATE_POS)
#define CODEC_SAMPLE_RATE_CLR               (~CODEC_SAMPLE_RATE_MSK)

/* Register: I2S_CTRL -------------------------------------------------------*/
/* Description: I2S Control register. Offset: 0x11C. Address: 0x4002_211CUL. */

/* I2S_CTRL[19:18]: I2S_RX_CH. */
#define CODEC_I2S_RX_CH_POS                 (18)
#define CODEC_I2S_RX_CH_MSK                 (0x3 << CODEC_I2S_RX_CH_POS)
#define CODEC_I2S_RX_CH_CLR                 (~CODEC_I2S_RX_CH_MSK)
/* I2S_CTRL[15:14]: I2S_RX_DATA_LEN. */
#define CODEC_I2S_RX_DATA_LEN_POS           (14)
#define CODEC_I2S_RX_DATA_LEN_MSK           (0x3 << CODEC_I2S_RX_DATA_LEN_POS)
#define CODEC_I2S_RX_DATA_LEN_CLR           (~CODEC_I2S_RX_DATA_LEN_MSK)
/* I2S_CTRL[11:10]: I2S_RX_DATA_FORMAT. */
#define CODEC_I2S_RX_DATA_FORMAT_POS        (10)
#define CODEC_I2S_RX_DATA_FORMAT_MSK        (0x3 << CODEC_I2S_RX_DATA_FORMAT_POS)
#define CODEC_I2S_RX_DATA_FORMAT_CLR        (~CODEC_I2S_RX_DATA_FORMAT_MSK)
/* I2S_CTRL[2]: I2S_SELF_LPBK. */
#define CODEC_I2S_SELF_LPBK_POS             (2)
#define CODEC_I2S_SELF_LPBK_MSK             (0x1 << CODEC_I2S_SELF_LPBK_POS)
#define CODEC_I2S_SELF_LPBK_CLR             (~CODEC_I2S_SELF_LPBK_MSK)
/* I2S_CTRL[1]: I2S_INV_SCLK. */
#define CODEC_I2S_INV_SCLK_POS              (1)
#define CODEC_I2S_INV_SCLK_MSK              (0x1 << CODEC_I2S_INV_SCLK_POS)
#define CODEC_I2S_INV_SCLK_CLR              (~CODEC_I2S_INV_SCLK_MSK)
/* I2S_CTRL[0]: AUDIO_RST_N. 0x1: Power on. 0x0: Power down. */
#define CODEC_AUDIO_RST_N_POS               (0)
#define CODEC_AUDIO_RST_N_MSK               (0x1 << CODEC_AUDIO_RST_N_POS)
#define CODEC_AUDIO_RST_N_CLR               (~CODEC_AUDIO_RST_N_MSK)

/* Register: ADC0_CTRL0 -------------------------------------------------------*/
/* Description: ADC0 Control register. Offset: 0x120. Address: 0x4002_2120UL. */

/* ADC0_CTRL0[19:17]: ADC0_DCHPF_FC_SEL. */
#define CODEC_ADC0_DCHPF_FC_SEL_POS         (17)
#define CODEC_ADC0_DCHPF_FC_SEL_MSK         (0x7 << CODEC_ADC0_DCHPF_FC_SEL_POS)
#define CODEC_ADC0_DCHPF_FC_SEL_CLR         (~CODEC_ADC0_DCHPF_FC_SEL_MSK)

/* ADC0_CTRL0[16]: ADC0_DCHPF_EN. 0x1: enable. 0x0: disable. */
#define CODEC_ADC0_DCHPF_EN_POS             (16)
#define CODEC_ADC0_DCHPF_EN_MSK             (0x1 << CODEC_ADC0_DCHPF_EN_POS)
#define CODEC_ADC0_DCHPF_EN_CLR             (~CODEC_ADC0_DCHPF_EN_MSK)

/* ADC0_CTRL0[15]: AMIC_DMIC_CH0_SEL. 0x1: ADC path. 0x0: DMIC path. */
#define CODEC_AMIC_DMIC_CH0_SEL_POS         (15)
#define CODEC_AMIC_DMIC_CH0_SEL_MSK         (0x1 << CODEC_AMIC_DMIC_CH0_SEL_POS)
#define CODEC_AMIC_DMIC_CH0_SEL_CLR         (~CODEC_AMIC_DMIC_CH0_SEL_MSK)

/* ADC0_CTRL0[14:13]: ADC_CH0_BOOST_GAIN. */
#define CODEC_ADC_CH0_BOOST_GAIN_POS        (13)
#define CODEC_ADC_CH0_BOOST_GAIN_MSK        (0x3 << CODEC_ADC_CH0_BOOST_GAIN_POS)
#define CODEC_ADC_CH0_BOOST_GAIN_CLR        (~CODEC_ADC_CH0_BOOST_GAIN_MSK)

/* ADC0_CTRL0[12]: ADC_CH0_AD_MUTE. */
#define CODEC_ADC_CH0_AD_MUTE_POS           (12)
#define CODEC_ADC_CH0_AD_MUTE_MSK           (0x1 << CODEC_ADC_CH0_AD_MUTE_POS)
#define CODEC_ADC_CH0_AD_MUTE_CLR           (~CODEC_ADC_CH0_AD_MUTE_MSK)

/* ADC0_CTRL0[11:10]: ADC_CH0_ZDET_TOUT. */
#define CODEC_ADC_CH0_ZDET_TOUT_POS         (10)
#define CODEC_ADC_CH0_ZDET_TOUT_MSK         (0x3 << CODEC_ADC_CH0_ZDET_TOUT_POS)
#define CODEC_ADC_CH0_ZDET_TOUT_CLR         (~CODEC_ADC_CH0_ZDET_TOUT_MSK)

/* ADC0_CTRL0[9:8]: ADC_CH0_ZDET_FUN. 0x1: ADC path. 0x0: DMIC path. */
#define CODEC_ADC_CH0_ZDET_FUN_POS          (8)
#define CODEC_ADC_CH0_ZDET_FUN_MSK          (0x3 << CODEC_ADC_CH0_ZDET_FUN_POS)
#define CODEC_ADC_CH0_ZDET_FUN_CLR          (~CODEC_ADC_CH0_ZDET_FUN_MSK)
#define CODEC_ADC_CH0_ZDET_FUN_DEFAULT_MSK  (0x1 << CODEC_ADC_CH0_ZDET_FUN_POS)

/* ADC0_CTRL0[7:6]: ADC0_AD_LPF1ST_FC_SEL. */
#define CODEC_ADC0_AD_LPF1ST_FC_SEL_POS     (6)
#define CODEC_ADC0_AD_LPF1ST_FC_SEL_MSK     (0x3 << CODEC_ADC0_AD_LPF1ST_FC_SEL_POS)
#define CODEC_ADC0_AD_LPF1ST_FC_SEL_CLR     (~CODEC_ADC0_AD_LPF1ST_FC_SEL_MSK)

/* ADC0_CTRL0[5]: ADC0_AD_LPF1ST_EN. 0x1: enable. 0x0: disable. */
#define CODEC_ADC0_AD_LPF1ST_EN_POS         (5)
#define CODEC_ADC0_AD_LPF1ST_EN_MSK         (0x1 << CODEC_ADC0_AD_LPF1ST_EN_POS)
#define CODEC_ADC0_AD_LPF1ST_EN_CLR         (~CODEC_ADC0_AD_LPF1ST_EN_MSK)

/* ADC0_CTRL0[4]: ADC0_AD_LPF2ND_EN. 0x1: enable. 0x0: disable. */
#define CODEC_ADC0_AD_LPF2ND_EN_POS         (4)
#define CODEC_ADC0_AD_LPF2ND_EN_MSK         (0x1 << CODEC_ADC0_AD_LPF2ND_EN_POS)
#define CODEC_ADC0_AD_LPF2ND_EN_CLR         (~CODEC_ADC0_AD_LPF2ND_EN_MSK)

/* ADC0_CTRL0[1:0]: ADC0_DMIC_SRC_SEL. */
#define CODEC_ADC0_DMIC_SRC_SEL_POS         (0)
#define CODEC_ADC0_DMIC_SRC_SEL_MSK         (0x3 << CODEC_ADC0_DMIC_SRC_SEL_POS)
#define CODEC_ADC0_DMIC_SRC_SEL_CLR         (~CODEC_ADC0_DMIC_SRC_SEL_MSK)

#define CODEC_ADC0_CTRL0_REG_DEFAULT_MSK    ((0x2 << 17) | (0x01 << 16) | (0x01 << 10) | (0x02 << 8) | (0x03 <<4))

/*============================================================================*
 *                         Constants
 *============================================================================*/
/**
 * \defgroup    CODEC_Exported_Constants    CODEC Exported Constants
 *
 * \ingroup     CODEC
 */

#define IS_CODEC_PERIPH(PERIPH) ((PERIPH) == CODEC)

/**
 * \defgroup    CODEC_MICBST_Gain CODEC MICBST Gain
 * \{
 * \ingroup     CODEC_Exported_Constants
 */
#define MICBST_Gain_0dB                     ((uint32_t)(0x0))
#define MICBST_Gain_20dB                    ((uint32_t)(0x1 << CODEC_MICBST_GAIN_POS))
#define MICBST_Gain_30dB                    ((uint32_t)(0x2 << CODEC_MICBST_GAIN_POS))
#define MICBST_Gain_40dB                    ((uint32_t)(0x3 << CODEC_MICBST_GAIN_POS))

#define IS_MICBST_GAIN(GAIN) (((GAIN) == MICBST_Gain_0dB) || \
                              ((GAIN) == MICBST_Gain_20dB) || \
                              ((GAIN) == MICBST_Gain_30dB) || \
                              ((GAIN) == MICBST_Gain_40dB))
/** \} */

/**
 * \defgroup    CODEC_MICBST_Mode CODEC MICBST Mode
 * \{
 * \ingroup     CODEC_Exported_Constants
 */
#define MICBST_Mode_Single                  ((uint32_t)(0x0))
#define MICBST_Mode_Differential            ((uint32_t)CODEC_MICBST_ENDFL_MSK)

#define IS_MICBST_MODE(MODE) (((MODE) == MICBST_Mode_Single) || \
                              ((MODE) == MICBST_Mode_Differential))
/** \} */

/**
 * \defgroup    CODEC_MIC_BIAS_Config CODEC MIC BIAS Config
 * \{
 * \ingroup     CODEC_Exported_Constants
 */
#define MICBIAS_VOLTAGE_1_507               ((uint32_t)(0x00))
#define MICBIAS_VOLTAGE_1_62                ((uint32_t)(0x01 << CODEC_MICBIAS_VSET_POS))
#define MICBIAS_VOLTAGE_1_705               ((uint32_t)(0x02 << CODEC_MICBIAS_VSET_POS))
#define MICBIAS_VOLTAGE_1_8                 ((uint32_t)(0x03 << CODEC_MICBIAS_VSET_POS))
#define MICBIAS_VOLTAGE_1_906               ((uint32_t)(0x04 << CODEC_MICBIAS_VSET_POS))
#define MICBIAS_VOLTAGE_2_025               ((uint32_t)(0x05 << CODEC_MICBIAS_VSET_POS))
#define MICBIAS_VOLTAGE_2_16                ((uint32_t)(0x06 << CODEC_MICBIAS_VSET_POS))
#define MICBIAS_VOLTAGE_2_314               ((uint32_t)(0x07 << CODEC_MICBIAS_VSET_POS))

#define IS_MICBIAS_CONFIG(CONFIG) (((CONFIG) == MICBIAS_VOLTAGE_1_507) || ((CONFIG) == MICBIAS_VOLTAGE_1_62) || \
                                   ((CONFIG) == MICBIAS_VOLTAGE_1_705) || ((CONFIG) == MICBIAS_VOLTAGE_1_8) || \
                                   ((CONFIG) == MICBIAS_VOLTAGE_1_906) || ((CONFIG) == MICBIAS_VOLTAGE_2_025) || \
                                   ((CONFIG) == MICBIAS_VOLTAGE_2_16) || ((CONFIG) == MICBIAS_VOLTAGE_2_314))
/** \} */

/**
 * \defgroup    CODEC_Sample_Rate_Definition  CODEC Sample Rate Definition
 * \{
 * \ingroup     CODEC_Exported_Constants
 */
#define SAMPLE_RATE_16KHz                   ((uint32_t)(0x05 << CODEC_SAMPLE_RATE_POS))
#define SAMPLE_RATE_8KHz                    ((uint32_t)(0x07 << CODEC_SAMPLE_RATE_POS))

#define IS_SAMPLE_RATE(RATE) (((RATE) == SAMPLE_RATE_16KHz) || ((RATE) == SAMPLE_RATE_8KHz))
/** \} */

/**
 * \defgroup    CODEC_I2S_CH CODEC I2S CH
 * \{
 * \ingroup     CODEC_Exported_Constants
 */
#define CODEC_I2S_CH_L_R                    ((uint32_t)(0x00))
#define CODEC_I2S_CH_R_L                    ((uint32_t)(0x01 << CODEC_I2S_RX_CH_POS))
#define CODEC_I2S_CH_L_L                    ((uint32_t)(0x02 << CODEC_I2S_RX_CH_POS))
#define CODEC_I2S_CH_R_R                    ((uint32_t)(0x03 << CODEC_I2S_RX_CH_POS))

#define IS_CODEC_I2S_CH(CH) (((CH) == CODEC_I2S_CH_L_R) || \
                             ((CH) == CODEC_I2S_CH_R_L) || \
                             ((CH) == CODEC_I2S_CH_L_L) || \
                             ((CH) == CODEC_I2S_CH_R_R))
/** \} */

/**
 * \defgroup    CODEC_I2S_Data_Width CODEC I2S Data Width
 * \{
 * \ingroup     CODEC_Exported_Constants
 */
#define CODEC_I2S_DataWidth_16Bits          ((uint32_t)(0x00))
#define CODEC_I2S_DataWidth_24Bits          ((uint32_t)(0x02 << CODEC_I2S_RX_DATA_LEN_POS))
#define CODEC_I2S_DataWidth_8Bits           ((uint32_t)(0x03 << CODEC_I2S_RX_DATA_LEN_POS))

#define IS_CODEC_I2S_DATA_WIDTH(WIDTH) (((WIDTH) == CODEC_I2S_DataWidth_16Bits) || \
                                        ((WIDTH) == CODEC_I2S_DataWidth_24Bits) || \
                                        ((WIDTH) == CODEC_I2S_DataWidth_8Bits))
/** \} */

/**
 * \defgroup    CODEC_I2S_Data_Format CODEC I2S Data Format
 * \{
 * \ingroup     CODEC_Exported_Constants
 */
#define CODEC_I2S_DataFormat_I2S            ((uint32_t)(0x00))
#define CODEC_I2S_DataFormat_LeftJustified  ((uint32_t)(0x01 << CODEC_I2S_RX_DATA_FORMAT_POS))
#define CODEC_I2S_DataFormat_PCM_A          ((uint32_t)(0x02 << CODEC_I2S_RX_DATA_FORMAT_POS))
#define CODEC_I2S_DataFormat_PCM_B          ((uint32_t)(0x03 << CODEC_I2S_RX_DATA_FORMAT_POS))

#define IS_CODEC_I2S_DATA_FORMAT(FORMAT) (((FORMAT) == CODEC_I2S_DataFormat_I2S) || \
                                          ((FORMAT) == CODEC_I2S_DataFormat_LeftJustified) || \
                                          ((FORMAT) == CODEC_I2S_DataFormat_PCM_A) || \
                                          ((FORMAT) == CODEC_I2S_DataFormat_PCM_B))
/** \} */

/**
 * \defgroup    CODEC_DMIC_Clock_Type CODEC DMIC Clock Type
 * \{
 * \ingroup     CODEC_Exported_Constants
 */
#define DMIC_Clock_5MHz                     ((uint32_t)(0x0))
#define DMIC_Clock_2500KHz                  ((uint32_t)(0x01 << CODEC_DMIC1_CLK_SEL_POS))
#define DMIC_Clock_1250KHz                  ((uint32_t)(0x02 << CODEC_DMIC1_CLK_SEL_POS))
#define DMIC_Clock_625KHz                   ((uint32_t)(0x03 << CODEC_DMIC1_CLK_SEL_POS))
#define DMIC_Clock_312500Hz                 ((uint32_t)(0x04 << CODEC_DMIC1_CLK_SEL_POS))

#define IS_CODEC_DMIC_CLOCK(CLOCK) (((CLOCK) == DMIC_Clock_5MHz) || ((CLOCK) == DMIC_Clock_2500KHz) || \
                                    ((CLOCK) == DMIC_Clock_1250KHz) || ((CLOCK) == DMIC_Clock_625KHz) || \
                                    ((CLOCK) == DMIC_Clock_312500Hz))
/** \} */

/**
 * \defgroup    CODEC_Ch0_Mic_Type CODEC Ch0 Mic Type
 * \{
 * \ingroup     CODEC_Exported_Constants
 */
#define CODEC_CH0_AMIC                      (CODEC_AMIC_DMIC_CH0_SEL_MSK)
#define CODEC_CH0_DMIC                      (0)

#define IS_CODEC_CH0_MIC_TYPE(TYPE) (((TYPE) == CODEC_CH0_AMIC) || ((TYPE) == CODEC_CH0_DMIC))
/** \} */

/**
 * \defgroup    CODEC_Ch0_Boost_Gain CODEC Ch0 Boost Gain
 * \{
 * \ingroup     CODEC_Exported_Constants
 */
#define Ch0_Boost_Gain_0dB                  ((uint32_t)(0x0))
#define Ch0_Boost_Gain_12dB                 ((uint32_t)(0x1 << CODEC_ADC_CH0_BOOST_GAIN_POS))
#define Ch0_Boost_Gain_24dB                 ((uint32_t)(0x2 << CODEC_ADC_CH0_BOOST_GAIN_POS))
#define Ch0_Boost_Gain_36dB                 ((uint32_t)(0x3 << CODEC_ADC_CH0_BOOST_GAIN_POS))

#define IS_CH0_BOOST_GAIN(GAIN) (((GAIN) == Ch0_Boost_Gain_0dB) || \
                                 ((GAIN) == Ch0_Boost_Gain_12dB) || \
                                 ((GAIN) == Ch0_Boost_Gain_24dB) || \
                                 ((GAIN) == Ch0_Boost_Gain_36dB))
/** \} */

/**
 * \defgroup    CODEC_Ch0_Mute CODEC Ch0 Mute
 * \{
 * \ingroup     CODEC_Exported_Constants
 */
#define CODEC_CH0_MUTE                      (CODEC_ADC_CH0_AD_MUTE_MSK)
#define CODEC_CH0_UNMUTE                    (0)

#define IS_CODEC_CH0_MIC_MUTE(MUTE) (((MUTE) == CODEC_CH0_MUTE) || ((MUTE) == CODEC_CH0_UNMUTE))
/** \} */

/**
 * \defgroup    CODEC_Ch0_ADC_Zero_Detection_Timeout CODEC Ch0 ADC Zero Detection Timeout
 * \{
 * \ingroup     CODEC_Exported_Constants
 */
#define Ch0_ADC_Zero_DetTimeout_1024_16_Sample       ((uint32_t)(0x00))
#define Ch0_ADC_Zero_DetTimeout_1024_32_Sample       ((uint32_t)(0x01 << CODEC_ADC_CH0_ZDET_TOUT_POS))
#define Ch0_ADC_Zero_DetTimeout_1024_64_Sample       ((uint32_t)(0x02 << CODEC_ADC_CH0_ZDET_TOUT_POS))
#define Ch0_ADC_Zero_DetTimeout_64_Sample            ((uint32_t)(0x03 << CODEC_ADC_CH0_ZDET_TOUT_POS))

#define IS_CH0_ADC_ZERO_DET_TIMEOUT(TIMEOUT) (((TIMEOUT) == Ch0_ADC_Zero_DetTimeout_1024_16_Sample) || \
                                              ((TIMEOUT) == Ch0_ADC_Zero_DetTimeout_1024_32_Sample) || \
                                              ((TIMEOUT) == Ch0_ADC_Zero_DetTimeout_1024_64_Sample) || \
                                              ((TIMEOUT) == Ch0_ADC_Zero_DetTimeout_64_Sample))
/** \} */

/**
 * \defgroup    CODEC_DMIC_Data_Latch_Edge CODEC DMIC Data Latch Edge
 * \{
 * \ingroup     CODEC_Exported_Constants
 */
#define DMIC_Ch0_Falling_Latch              ((uint32_t)(0x01 << CODEC_ADC0_DMIC_SRC_SEL_MSK))
#define DMIC_Ch0_Rising_Latch               ((uint32_t)(0x00))

#define IS_DMIC_CH0_LATCH_EDGE(EDGE) (((EDGE) == DMIC_Ch0_Rising_Latch) || ((EDGE) == DMIC_Ch0_Falling_Latch))
/** \} */

/**
 * \defgroup    CODEC_Ad_Gain Codec Ad Gain
 * \{
 * \ingroup     CODEC_Exported_Constants
 */
#define IS_AD_GAIN(GAIN) (((GAIN)<= 0x7F))
/** \} */

/**
 * \defgroup    CODEC_Da_Gain Codec Da Gain
 * \{
 * \ingroup     CODEC_Exported_Constants
 */
#define IS_DA_GAIN(GAIN) (((GAIN)<= 0xFF))
/** \} */

/** End of CODEC_Exported_Constants
  * \}
  */

/*============================================================================*
 *                         Functions
 *============================================================================*/
/**
 * \defgroup    CODEC_Exported_Functions CODEC Exported Functions
 * \{
 * \ingroup     CODEC
 */

/**
 * \brief     Initialize the CODEC analog circuit.
 * \return    None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * CODEC_AnalogCircuitInit();
 * \endcode
 *
 */
void CODEC_AnalogCircuitInit(void);

/**
 * \brief   Deinitialize the CODEC peripheral registers to their default reset values (turn off CODEC clock).
 * \param[in] CODECx: CODEC peripheral selected.
 * \return None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void driver_codec_init(void)
 * {
 *     CODEC_DeInit(CODEC);
 * }
 * \endcode
 *
 */
void CODEC_DeInit(CODEC_TypeDef *CODECx);

/**
 * \brief   Initialize the CODEC peripheral according to the specified
 *          parameters in the CODEC_InitStruct.
 * \param[in] CODECx: CODEC peripheral selected.
 * \param[in] CODEC_InitStruct: Pointer to a CODEC_InitTypeDef structure that
 *            contains the configuration information for the specified CODEC peripheral.
 * \return None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void driver_codec_init(void)
 * {
 *     RCC_PeriphClockCmd(APBPeriph_CODEC, APBPeriph_CODEC_CLOCK, ENABLE);
 *
 *     CODEC_InitTypeDef CODEC_InitStruct;

 *     CODEC_StructInit(&CODEC_InitStruct);
 *     CODEC_InitStruct.CODEC_Ch0MicType       = CODEC_CH0_AMIC;
 *     CODEC_InitStruct.CODEC_MicBstMode       = MICBST_Mode_Differential;
 *     CODEC_InitStruct.CODEC_SampleRate       = SAMPLE_RATE_16KHz;
 *     CODEC_InitStruct.CODEC_I2SFormat        = CODEC_I2S_DataFormat_I2S;
 *     CODEC_InitStruct.CODEC_I2SDataWidth     = CODEC_I2S_DataWidth_16Bits;
 *     CODEC_Init(CODEC, &CODEC_InitStruct);
 * }
 * \endcode
 *
 */
void CODEC_Init(CODEC_TypeDef *CODECx, CODEC_InitTypeDef *CODEC_InitStruct);

/**
 * \brief   Fill each CODEC_InitStruct member with its default value.
 * \param[in] CODEC_InitStruct: Pointer to a CODEC_InitTypeDef structure which will be initialized.
 * \return None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void driver_codec_init(void)
 * {
 *     RCC_PeriphClockCmd(APBPeriph_CODEC, APBPeriph_CODEC_CLOCK, ENABLE);
 *
 *     CODEC_InitTypeDef CODEC_InitStruct;

 *     CODEC_StructInit(&CODEC_InitStruct);
 *     CODEC_InitStruct.CODEC_Ch0MicType       = CODEC_CH0_AMIC;
 *     CODEC_InitStruct.CODEC_MicBstMode       = MICBST_Mode_Differential;
 *     CODEC_InitStruct.CODEC_SampleRate       = SAMPLE_RATE_16KHz;
 *     CODEC_InitStruct.CODEC_I2SFormat        = CODEC_I2S_DataFormat_I2S;
 *     CODEC_InitStruct.CODEC_I2SDataWidth     = CODEC_I2S_DataWidth_16Bits;
 *     CODEC_Init(CODEC, &CODEC_InitStruct);
 * }
 * \endcode
 *
 */
void CODEC_StructInit(CODEC_InitTypeDef *CODEC_InitStruct);

/**
 * \brief  Enable or disable mic_bias output.
 * \param[in] CODECx: CODEC peripheral selected.
 * \param[in] NewState: New state of MICBIAS.
 *      This parameter can be: ENABLE or DISABLE.
 * \return None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void codec_demo(void)
 * {
 *     CODEC_MICBIASCmd(CODEC, ENABLE);
 * }
 * \endcode
 *
 */
void CODEC_MICBIASCmd(CODEC_TypeDef *CODECx, FunctionalState NewState);

/**
 * \brief  Reset CODEC.
 * \param[in] CODECx: CODEC peripheral selected.
 * \return None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void codec_demo(void)
 * {
 *     CODEC_Reset(CODEC);
 * }
 * \endcode
 *
 */
__STATIC_INLINE void CODEC_Reset(CODEC_TypeDef *CODECx)
{
    /* Check the parameters */
    assert_param(IS_CODEC_PERIPH(CODECx));

    /* CODEC IP reset */
    CODECx->CR0 &= CODEC_AUDIO_IP_EN_CLR;
    CODECx->CR0 = CODEC_AUDIO_IP_EN_MSK;
}

/**
 * \brief   Config MIC BIAS Vref voltage.
 * \param[in] CODECx: CODEC peripheral selected.
 * \param[in] data: New value of MIC BIAS.
 *   This parameter can be one of the following values, which refer to \ref CODEC_MIC_BIAS_Config.
 *     \arg MICBIAS_VOLTAGE_1_507: Vref voltage is 1.507V.
 *     \arg MICBIAS_VOLTAGE_1_62:  Vref voltage is 1.62V.
 *     \arg MICBIAS_VOLTAGE_1_705: Vref voltage is 1.705V.
 *     \arg MICBIAS_VOLTAGE_1_8: Vref voltage is 1.8V.
 *     \arg MICBIAS_VOLTAGE_1_906: Vref voltage is 1.906V.
 *     \arg MICBIAS_VOLTAGE_2_025:  Vref voltage is 2.025V.
 *     \arg MICBIAS_VOLTAGE_2_16: Vref voltage is 2.16V.
 *     \arg MICBIAS_VOLTAGE_2_314: Vref voltage is 2.314V.
 * \return None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void codec_demo(void)
 * {
 *     CODEC_SetMICBIAS(CODEC, MICBIAS_VOLTAGE_1_507);
 * }
 * \endcode
 *
 */
__STATIC_INLINE void CODEC_SetMICBIAS(CODEC_TypeDef *CODECx,  uint16_t data)

{
    /* Check the parameters */
    assert_param(IS_CODEC_PERIPH(CODECx));
    assert_param(IS_MICBIAS_CONFIG(data));

    CODEC_ANA->ANA_CR1 &= CODEC_MICBIAS_POW_CLR;
    CODEC_ANA->ANA_CR1 &= CODEC_MICBIAS_VSET_CLR;
    CODEC_ANA->ANA_CR1 |= data;
}

/** End of CODEC_Exported_Functions
  * \}
  */

/** End of CODEC
  * \}
  */

#ifdef __cplusplus
}
#endif

#endif /* _RTL876X_CODEC_H_ */


/******************* (C) COPYRIGHT 2024 Realtek Semiconductor *****END OF FILE****/

