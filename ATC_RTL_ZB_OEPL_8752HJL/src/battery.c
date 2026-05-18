#include "battery.h"
#include <rtl876x_adc.h>
#include <rtl876x_rcc.h>
#include <stdio.h>

uint16_t battery_measure_mv(void)
{
    /* Load Efuse calibration coefficients each call.  Must run from RTOS
     * task context — calling this pre-RTOS causes RAM_DATA_ERROR because
     * the SDK Efuse path is not fully initialised before os_sched_start(). */
    if (!ADC_CalibrationInit())
        printf("BATT: calibration load failed\n");

    /* Full ADC re-init on every call so registers are correct after DLPS.
     * Use default ADC_POWER_ON_AUTO: hardware applies 60 µs power-up
     * stabilisation delays before each conversion.  ADC_POWER_ALWAYS_ON_ENABLE
     * skips those delays — causes rail-stuck (1023) readings after DLPS. */
    RCC_PeriphClockCmd(APBPeriph_ADC, APBPeriph_ADC_CLOCK, ENABLE);

    ADC_InitTypeDef adc;
    ADC_StructInit(&adc);
    adc.ADC_SchIndex[0]     = INTERNAL_VBAT_MODE;
    adc.ADC_Bitmap          = 0x0001;
    adc.ADC_SampleTime      = 255;
    /* ADC_POWER_ALWAYS_ON_ENABLE skips the hardware power-up stabilisation
     * delays and causes rail-stuck (raw=1023) readings after DLPS wake.
     * Force AUTO mode so the 60 µs delay is applied before each conversion. */
    adc.ADC_PowerOnMode     = ADC_POWER_ON_AUTO;
    adc.ADC_PowerAlwaysOnEn = ADC_POWER_ALWAYS_ON_DISABLE;
    ADC_Init(ADC, &adc);
    ADC_INTConfig(ADC, ADC_INT_ONE_SHOT_DONE, ENABLE);

    /* Wait for the internal VBAT reference to stabilise after re-init.
     * The DLPS exit callback already re-enables the analog supply at wake,
     * but a short extra delay here guards against any residual settling time. */
    //platform_delay_ms(85);

    ADC_ClearINTPendingBit(ADC, ADC_INT_ONE_SHOT_DONE);
    ADC_Cmd(ADC, ADC_ONE_SHOT_MODE, ENABLE);

    uint32_t timeout = 200000;
    while (ADC_GetINTStatus(ADC, ADC_INT_ONE_SHOT_DONE) == RESET) {
        if (--timeout == 0) {
            printf("BATT: ADC timeout\n");
            return 0;
        }
    }
    ADC_ClearINTPendingBit(ADC, ADC_INT_ONE_SHOT_DONE);

    uint16_t raw = ADC_ReadRawData(ADC, 0);
    ADC_ErrorStatus err = NO_ERROR;
    float mv = ADC_GetVoltage(DIVIDE_SINGLE_MODE, (int32_t)raw, &err);
    if (err < NO_ERROR) {
        printf("BATT: voltage err=%d raw=%u\n", (int)err, raw);
        return 0;
    }
    uint16_t result = (uint16_t)mv;
    printf("BATT: raw=%u  %u mV\n", raw, result);
    return result;
}
