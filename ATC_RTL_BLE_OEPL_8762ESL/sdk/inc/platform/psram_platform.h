#ifndef _PSRAM_PLATFORM_H_
#define _PSRAM_PLATFORM_H_

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

void psram_init(void);

void psram_config(uint8_t bit_mode);

void psram_try_high_speed(void);

bool psram_read(uint32_t addr, uint32_t len, uint8_t *data);

bool psram_write(uint32_t addr, uint32_t len, uint8_t *data);

void psram_deinit(void);

#ifdef __cplusplus
}
#endif

#endif /* _PSRAM_PLATFORM_H_ */
