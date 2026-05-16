/**
*********************************************************************************************************
*               Copyright(c) 2024, Realtek Semiconductor Corporation. All rights reserved.
*********************************************************************************************************
* \file     adc_lib.h
* \brief    The header file of the peripheral ADC driver.
* \details  This file provides all ADC firmware functions.
* \author   yuan
* \date     2024-01-25
* \version  v1.0.0
* *********************************************************************************************************
*/

#ifndef __ADC_K_LIB_H
#define __ADC_K_LIB_H

/*============================================================================*
 *                         Includes
 *============================================================================*/
#include <stdbool.h>
#include <stdint.h>

/*============================================================================*
 *                         Defines
 *============================================================================*/
#define SOC_VERSION_CHECK               1

#define ADC_K_VERSION_N2                1
//#define ADC_K_VERSION_N1                1
//#define ADC_K_VERSION_0                 1
#define ADC_K_VERSION_P1                1

#define USE_ADC_DIVIDE_SINGLE_MODE      1
#define USE_ADC_BYPASS_SINGLE_MODE      1
#define USE_ADC_DIVIDE_DIFF_MODE        1
#define USE_ADC_BYPASS_DIFF_MODE        1

/**
 * \addtogroup  IO          Peripheral Drivers
 * \defgroup    ADC         ADC
 *
 * \brief       Manage the ADC peripheral functions.
 *
 * \ingroup     IO
 */
/*============================================================================*
 *                         Constants
 *============================================================================*/
/**
 * \defgroup    ADC_Exported_Constants  ADC Exported Constants
 *
 * \ingroup     ADC
 */

/**
 * \defgroup    ADC_Sample_Mode  ADC Sample Mode
 * \{
 * \ingroup     ADC_Exported_Constants
 */
typedef enum
{
    DIVIDE_SINGLE_MODE = 1,
    BYPASS_SINGLE_MODE = 2,
    DIVIDE_DIFFERENTIAL_MODE = 3,
    BYPASS_DIFFERENTIAL_MODE = 4,
} ADC_SampleMode;
/** \} */

/**
 * \defgroup    ADC_Error_Status  ADC Error Status
 * \{
 * \ingroup     ADC_Exported_Constants
 */
typedef enum
{
    NO_ERROR = 0,
    PARAMETER_ERROR = -1,
    RAM_DATA_ERROR = -2,
    NO_CALIBRATION = -3,
    VERSION_ERROR = -4,
} ADC_ErrorStatus;
/** \} */

extern int8_t ADC_K_Version;
extern uint16_t ADC_Resistance_Value;

/** End of ADC_Exported_Constants
  * \}
  */

/*============================================================================*
 *                         Functions
 *============================================================================*/
/** \defgroup    ADC_Exported_Functions ADC Exported Functions
  * \{
  * \ingroup     ADC
  */

/**
 * adc_lib.h
 *
 * \brief       Initialize the voltage conversion.
 * \param[in]   None.
 * \return      The status of ADC calibration.
 * \retval      true: ADC voltage calibration correct.
 * \retval      false:. ADC voltage calibration incorrect.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * bool adc_k_status = false;
 * adc_k_status = ADC_CalibrationInit();
 * \endcode
 */
bool ADC_CalibrationInit(void);

/**
 * adc_lib.h
 *
 * \brief       Obtain the actual voltage value.
 * \param[in]   vSampleMode: ADC operation mode.
 * \param[in]   vSampleData: ADC sample data.
 * \param[in]   pErrorStatus: This parameter can be a value of ADC_ErrorStatus.
 * \return      actual voltage value.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * float sample_voltage = 0;
 * sample_voltage = ADC_GetVoltage(BYPASS_SINGLE_MODE, (int32_t)sample_data, &error_status);
 * \endcode
 */
float ADC_GetVoltage(const ADC_SampleMode vSampleMode, int32_t vSampleData,
                     ADC_ErrorStatus *pErrorStatus);

/**
 * adc_lib.h
 *
 * \brief       Obtain the actual resistance value.
 * \param[in]   None.
 * \return      resistance value.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * uint32_t value = ADC_GetResistance();
 * \endcode
 */
uint16_t ADC_GetResistance(void);

/** End of ADC_Exported_Functions
  * \}
  */

/** End of ADC
  * \}
  */

#endif /* __ADC_K_LIB_H */


/******************* (C) COPYRIGHT 2024 Realtek Semiconductor *****END OF FILE****/
