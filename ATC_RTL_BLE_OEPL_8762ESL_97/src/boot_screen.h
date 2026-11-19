#ifndef _BOOT_SCREEN_H_
#define _BOOT_SCREEN_H_

#include <stdint.h>

/* Draw the info screen at boot and trigger an asynchronous EPD refresh.
 * Returns immediately; caller must start the epd_poll_timer to detect completion. */
void boot_screen_draw(const uint8_t *ble_addr_6, uint16_t battery_mv);

/* Display an arbitrary set of text lines on the EPD and trigger an async refresh.
 * Returns immediately; caller must start the epd_poll_timer to detect completion.
 * lines: array of n_lines C-strings (each up to 99 chars visible). */
void boot_screen_show_error(const char * const *lines, uint8_t n_lines);

#endif
