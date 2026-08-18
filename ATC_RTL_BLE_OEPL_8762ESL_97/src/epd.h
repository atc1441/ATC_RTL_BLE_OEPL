#ifndef _EPD_H_
#define _EPD_H_

#include <stdint.h>

/* EL097R2CRN / 9.7\" BWR panel.
 * Controller RAM is addressed as 960 source pixels x 672 gate lines.
 * The physical label is normally used in portrait orientation (672x960),
 * but the firmware/OEPL raster is kept in controller-native 960x672 order.
 */
#define EPD_WIDTH        960u
#define EPD_HEIGHT       672u
#define EPD_ROW_BYTES    (EPD_WIDTH / 8u)              /* 120 bytes */
#define EPD_BUF_SIZE     (EPD_ROW_BYTES * EPD_HEIGHT)  /* 80640 bytes */

void epd_board_init(void);
void epd_board_sleep(void);
void epd_init(void);
void epd_sleep(void);
void epd_draw_full(void);

/* Low-level command/data helpers. */
void epd_cmd(uint8_t cmd);
void epd_data(uint8_t data);
void epd_write(uint8_t cmd, int n_data, ...);
void epd_stream_data(const uint8_t *buf, uint32_t len);
void epd_stream_const(uint8_t value, uint32_t n);
void epd_wait_busy(void);

/* EL097R2CRN RAM helpers reconstructed from the stock firmware. */
void epd_begin_bw(void);
void epd_begin_red(void);
void epd_refresh(void);
void epd_display_white(void);

/* Compatibility API used by drawing.c / older code. */
void EPD_Display_start(uint8_t color);
void EPD_Display_byte(uint8_t data);
void EPD_Display_color_change(uint8_t color);
void EPD_Display_end(void);

#endif /* _EPD_H_ */
