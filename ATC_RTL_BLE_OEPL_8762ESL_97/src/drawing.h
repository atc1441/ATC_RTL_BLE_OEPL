#ifndef _DRAWING_H_
#define _DRAWING_H_

#include <stdint.h>
#include <stdbool.h>

void drawOnOffline(uint8_t state);
void drawImageAtAddress(uint32_t addr, uint8_t lut);

/* Poll the EPD BUSY pin.  Returns true once the refresh is complete and
 * epd_sleep() has been called.  Call from a periodic timer (every ~200 ms). */
bool epd_refresh_done(void);

/* True while the EPD is in the middle of a display refresh (BUSY pin HIGH).
 * Read by the DLPS callbacks to decide whether to latch EPD pin states. */
extern volatile bool g_epd_refreshing;

#endif
