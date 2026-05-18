#ifndef OTA_H
#define OTA_H

#include <stdint.h>
#include "flash_map.h"

/* OTA firmware staging area in the external SPI flash chip.
 * Placed immediately after 3 image slots (3 × 0x17000 = 0x45000). */
#define OTA_EXT_START       0x00045000UL
#define OTA_EXT_SIZE        BANK0_APP_SIZE   /* 0x25000 = 148 KB */

/* Image slot count must be capped so slots do not overlap OTA_EXT_START. */
#define OTA_IMG_SLOTS_MAX   3u

/* Copy firmware from external flash staging area to internal BANK0_APP and
 * reset.  fw_size is the total firmware byte count (from AvailDataInfo).
 * This function does not return. */
void ota_apply(uint32_t fw_size);

#endif /* OTA_H */
