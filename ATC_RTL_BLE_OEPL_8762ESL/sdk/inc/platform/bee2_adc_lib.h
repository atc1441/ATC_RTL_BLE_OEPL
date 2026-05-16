#ifndef __ADC_K_LIB_H
#define __ADC_K_LIB_H

#include <stdbool.h>
#include <stdint.h>

typedef enum
{
    DIVIDE_SINGLE_MODE = 1,
    BYPASS_SINGLE_MODE = 2,
    DIVIDE_DIFFERENTIAL_MODE = 3,
    BYPASS_DIFFERENTIAL_MODE = 4,
} ADC_SampleMode;

typedef enum
{
    NO_ERROR = 0,
    PARAMETER_ERROR = -1,
    RAM_DATA_ERROR = -2,
    NO_CALIBRATION = -3,
    VERSION_ERROR = -4,
} ADC_ErrorStatus;

extern int16_t ADC_K_Version;

bool ADC_CalibrationInit(void);
float ADC_GetVoltage(const ADC_SampleMode vSampleMode, int32_t vSampleData,
                     ADC_ErrorStatus *pErrorStatus);
uint16_t ADC_GetResistance(void);



#endif
