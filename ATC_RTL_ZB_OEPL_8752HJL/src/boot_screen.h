#ifndef _BOOT_SCREEN_H_
#define _BOOT_SCREEN_H_

#include <stdint.h>

/* Display an arbitrary set of text lines on the EPD and trigger an async refresh.
 * Returns immediately; caller must start the epd_poll_timer to detect completion.
 * lines: array of n_lines C-strings (each up to 99 chars visible). */
void boot_screen_show_error(const char * const *lines, uint8_t n_lines);

/*
 * Show a protocol status screen and block until the EPD refresh is complete.
 *   mac8       — 8-byte Zigbee MAC (mSelfMac)
 *   battery_mv — battery voltage in mV
 *   channel    — current channel (0 = scanning)
 *   status     — one-line status string shown on the last line
 */
void status_screen_show(const uint8_t *mac8, uint16_t battery_mv,
                        uint8_t channel, const char *status);

#endif
