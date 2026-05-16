#ifndef _EPD_H_
#define _EPD_H_

#include <stdint.h>

#define EPD_WIDTH        400
#define EPD_HEIGHT       272
#define EPD_BUF_SIZE     (EPD_WIDTH / 8 * EPD_HEIGHT)   /* 13600 bytes */

/*
 * epd_init  — full hardware + software init sequence from sniff.
 *             Call from driver_init() (inside RTOS task, RTOS must be running
 *             because the busy-wait uses platform_delay_ms).
 */
void epd_board_init(void);
void epd_board_sleep(void);
void epd_init(void);

/*
 * epd_display_white — write all-white frame to both BW and Red RAM,
 *                     trigger full refresh and wait until done (~22 s).
 *                     Uses os_delay() internally — call from RTOS task.
 */
void epd_display_white(void);

/*
 * epd_sleep  — put the controller into deep sleep (1 µA standby).
 *              Call after refresh is complete.  Requires full re-init
 *              (epd_init) before next use.
 */
void epd_sleep(void);

void epd_draw_full(void);

/* Low-level functions for drawing.c */
void epd_cmd(uint8_t cmd);
void epd_data(uint8_t data);
void epd_write(uint8_t cmd, int n_data, ...);

/* Stream a pre-loaded buffer to the EPD (DC=high, CS held low for the entire
 * buffer).  Use after epd_cmd() to write RAM data efficiently. */
void epd_stream_data(const uint8_t *buf, uint32_t len);

/* Stream n bytes of constant value as data (DC=high). */
void epd_stream_const(uint8_t value, uint32_t n);

/* Compatibility functions for drawing.c */
void EPD_Display_start(uint8_t color);
void EPD_Display_byte(uint8_t data);
void EPD_Display_color_change(uint8_t color);
void EPD_Display_end(void);

#endif /* _EPD_H_ */
