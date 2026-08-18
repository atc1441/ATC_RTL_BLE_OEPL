#ifndef _EEPROM_H_
#define _EEPROM_H_

#include <stdint.h>

#define EEPROM_IMG_START (0x00000UL)
#define EEPROM_IMG_LEN (0x80000UL)

#define EEPROM_WRITE_PAGE_SZ (256)
#define EEPROM_PAGE_SIZEour (0x01000)

#define EEPROM_IMG_VALID (0x494d4721UL)

#pragma pack(push, 1)
struct EepromImageHeader {
    uint8_t  version[8];    /* AvailDataInfo.dataVer — used by findSlot() */
    uint32_t validMarker;   /* EEPROM_IMG_VALID + img_each — used by findSlot() */
    uint32_t size;          /* total image bytes (same order as CH573 eeprom.h) */
    uint8_t  dataType;      /* DATATYPE_IMG_RAW_1BPP etc. */
    uint32_t id;            /* auto-incrementing slot ID */
};
#pragma pack(pop)

/* Full init: GPIO setup + 20 ms power-on delay + JEDEC read.
 * Leaves the flash in deep power-down on return. */
void FLASH_Init(void);

/* Power management: wake from / enter SPI flash deep power-down (0xAB / 0xB9).
 * Calls are idempotent — double-wake or double-sleep are no-ops.
 * All callers must bracket their flash sessions with these two calls. */
void eepromPowerUp(void);
void eepromPowerDown(void);

/* Lightweight GPIO-only reinit — no delays, no SPI transactions.
 * Call from io_dlps_exit_cb after every DLPS wake-up to restore
 * the PAD configuration that may not be preserved by the DLPS framework. */
void SPI_Flash_enable_GPIO(void);

/* Write/read self-test.  Returns 1 on pass, 0 on failure.
 * Controlled by FLASH_SELFTEST_EN in app_flags.h. */
uint8_t flash_selftest(void);

void    eepromRead(uint32_t addr, uint8_t *dst, uint32_t len);
uint8_t eepromWrite(uint32_t addr, uint8_t *src, uint32_t len);
uint8_t eepromErase(uint32_t addr, uint32_t len);
uint32_t eepromGetSize(void);

#endif
