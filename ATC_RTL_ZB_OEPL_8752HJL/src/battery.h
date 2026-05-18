#ifndef _BATTERY_H_
#define _BATTERY_H_

#include <stdint.h>

/* Trigger a single VBAT one-shot measurement and return millivolts.
 * Blocks for ~25 µs (one ADC conversion). Returns 0 on error. */
uint16_t battery_measure_mv(void);

/* Read the on-chip thermal meter and return temperature in °C.
 * Uses the ROM get_thermal_meter_celsius function pointer. */
int8_t temperature_measure_celsius(void);

#endif /* _BATTERY_H_ */
