#ifndef _BATTERY_H_
#define _BATTERY_H_

#include <stdint.h>

/* Trigger a single VBAT one-shot measurement and return millivolts.
 * Blocks for ~25 µs (one ADC conversion). Returns 0 on error. */
uint16_t battery_measure_mv(void);

#endif /* _BATTERY_H_ */
