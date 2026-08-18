/* ota.c — OTA firmware update over BLE (external flash staging)
 *
 * Flow:
 *   1. BLE block transfer → validated blocks written to OTA_EXT_START in
 *      external SPI flash (ota_erase_ext / ota_save_ext_block).
 *   2. ota_apply() copies ext flash → internal OTA_TMP (0x85B000) using
 *      normal code; this is safe because OTA_TMP != APP area (0x832000).
 *   3. ota_do_apply() (DATA_RAM_FUNCTION) copies OTA_TMP → BANK0_APP.
 *      Per sector:
 *        a) Read first 256-byte page from OTA_TMP into stack buf (flash idle).
 *        b) Erase 4 KB APP sector (flash busy).
 *        c) Write that page from buf (flash busy).
 *        d) For each remaining page: read from OTA_TMP (flash idle again after
 *           each write_locked), write to APP.
 *      Ends with a Cortex-M AIRCR system reset.
 *
 * Use bin/app_MP.bin (with prepended Realtek header) as the firmware image.
 */

#include "ota.h"
#include "eeprom.h"
#include "app_section.h"
#include "flash_device.h"
#include "patch_header_check.h"
#include "boot_screen.h"
#include "rtl876x_wdg.h"
#include <string.h>
#include <stdio.h>

/* ---- Stage 1: external flash --------------------------------------------- */

void ota_erase_ext(void)
{
    printf("OTA: erase ext 0x%05lX len=0x%05lX\n",
           (unsigned long)OTA_EXT_START, (unsigned long)OTA_EXT_SIZE);
    eepromPowerUp();
    eepromErase(OTA_EXT_START, OTA_EXT_SIZE);
    eepromPowerDown();
}

void ota_save_ext_block(uint8_t block_id, const uint8_t *data, uint32_t len)
{
    uint32_t addr = OTA_EXT_START + (uint32_t)block_id * 4096UL;
    if (addr + len > OTA_EXT_START + OTA_EXT_SIZE) {
        printf("OTA: ERR blk %u out of range\n", block_id);
        return;
    }
    printf("OTA: save blk %u → ext 0x%05lX len=%lu\n",
           block_id, (unsigned long)addr, (unsigned long)len);
    eepromPowerUp();
    eepromWrite(addr, (uint8_t *)data, len);
    eepromPowerDown();
}

/* ---- Stage 2: external flash → OTA_TMP ----------------------------------- */

static void copy_ext_to_tmp(uint32_t fw_size)
{
    printf("OTA: ext→OTA_TMP 0x%08lX  %lu bytes\n",
           (unsigned long)OTA_TMP_ADDR, (unsigned long)fw_size);

    /* Erase the required sectors in OTA_TMP. */
    uint32_t sectors = (fw_size + 0xFFFu) >> 12;
    for (uint32_t s = 0; s < sectors; s++)
        flash_erase_locked(FLASH_ERASE_SECTOR, OTA_TMP_ADDR + s * 0x1000u);

    /* Copy 256 bytes at a time from external flash to OTA_TMP. */
    uint8_t buf[256];
    eepromPowerUp();
    for (uint32_t off = 0; off < fw_size; off += 256u) {
        uint32_t n = (fw_size - off < 256u) ? (fw_size - off) : 256u;
        eepromRead(OTA_EXT_START + off, buf, n);
        if (n < 256u) memset(buf + n, 0xFFu, 256u - n);
        flash_write_locked(OTA_TMP_ADDR + off, 256u, buf);
    }
    eepromPowerDown();

    printf("OTA: ext→OTA_TMP done\n");
}

/* ---- Stage 3: OTA_TMP → BANK0_APP (runs entirely from RAM) --------------- */

DATA_RAM_FUNCTION static void ota_do_apply(uint32_t sectors)
{
    /* Disable interrupts — no ISR must try to fetch code from flash
     * while the APP region is being erased or written.
     * Use raw Cortex-M CPSID instruction; __disable_irq() is not always
     * inlined and would produce a flash-resident external call. */
    __asm volatile ("cpsid i" ::: "memory");

    uint8_t page_buf[256]; /* 256 bytes on stack = always in RAM */
    uint32_t s, pg, i;

    for (s = 0; s < sectors; s++) {
        uint32_t sector_off = s * 0x1000u;

        /* (a) Read the first 256-byte page from OTA_TMP while flash is idle.
         *     OTA_TMP is memory-mapped; pointer deref works when no flash
         *     operation is in progress. */
        volatile uint8_t *src =
            (volatile uint8_t *)(OTA_TMP_ADDR + sector_off);
        for (i = 0u; i < 256u; i++) page_buf[i] = src[i];

        /* (b) Erase 4 KB sector of BANK0_APP (flash busy). */
        flash_erase_locked(FLASH_ERASE_SECTOR, BANK0_APP_ADDR + sector_off);

        /* (c) Write first page from RAM buf (flash busy). */
        flash_write_locked(BANK0_APP_ADDR + sector_off, 256u, page_buf);

        /* (d) Remaining pages: flash is idle after each write_locked returns,
         *     so OTA_TMP is readable again via pointer before the next write. */
        for (pg = 256u; pg < 0x1000u; pg += 256u) {
            src = (volatile uint8_t *)(OTA_TMP_ADDR + sector_off + pg);
            for (i = 0u; i < 256u; i++) page_buf[i] = src[i];
            flash_write_locked(BANK0_APP_ADDR + sector_off + pg, 256u, page_buf);
        }
    }

    /* Cortex-M AIRCR system reset — pure register write, no flash access. */
    *((volatile uint32_t *)0xE000ED0CUL) = 0x05FA0004UL;
    while (1) {}
}

/* ---- Public entry point --------------------------------------------------- */

void ota_apply(uint32_t fw_size)
{
    if (fw_size == 0u || fw_size > OTA_EXT_SIZE) {
        printf("OTA: ERR invalid fw_size=%lu (max=%lu)\n",
               (unsigned long)fw_size, (unsigned long)OTA_EXT_SIZE);
        return;
    }

    /* Stage 2: copy ext flash → OTA_TMP (normal code, safe region). */
    copy_ext_to_tmp(fw_size);

    /* Validate the image in OTA_TMP before touching BANK0_APP.
     *
     * T_IMG_HEADER_FORMAT sits at OTA_TMP_ADDR (1024-byte header prepended by
     * prepend_header.exe).  T_IMG_CTRL_HEADER_FORMAT is its first 12 bytes and
     * holds image_id, payload_len, crc16, and the integrity-check-enable flag.
     * check_image_chksum() selects CRC16 or SHA-256 automatically and returns
     * true only when the computed digest matches the stored value. */
    T_IMG_CTRL_HEADER_FORMAT *p_hdr =
        (T_IMG_CTRL_HEADER_FORMAT *)OTA_TMP_ADDR;

    printf("OTA: validate id=0x%04X payload_len=%lu crc16=0x%04X\n",
           p_hdr->image_id,
           (unsigned long)p_hdr->payload_len,
           (unsigned)p_hdr->crc16);

    /* Sanity-check image ID — must be AppPatch (0x2793). */
    if (p_hdr->image_id != AppPatch) {
        printf("OTA: ERR wrong image_id 0x%04X (expected 0x%04X AppPatch) — abort\n",
               p_hdr->image_id, (unsigned)AppPatch);
        static const char * const e[] = {
            " OTA FAILED",
            " Wrong image type",
            " Expected: AppPatch",
            " Flash unchanged"
        };
        boot_screen_show_error(e, 4);
        return;
    }

    /* Sanity-check size: 1 KB header + payload must fit in APP slot. */
    if ((uint32_t)p_hdr->payload_len + IMG_HEADER_SIZE > BANK0_APP_SIZE) {
        printf("OTA: ERR image too large (%lu B) — abort\n",
               (unsigned long)((uint32_t)p_hdr->payload_len + IMG_HEADER_SIZE));
        static const char * const e[] = {
            " OTA FAILED",
            " Image too large",
            " Flash unchanged"
        };
        boot_screen_show_error(e, 3);
        return;
    }

    /* CRC16 / SHA-256 integrity check. */
    if (!check_image_chksum(p_hdr)) {
        printf("OTA: ERR checksum mismatch — abort, BANK0_APP unchanged\n");
        static const char * const e[] = {
            " OTA FAILED",
            " Checksum mismatch",
            " Please retry",
            " Flash unchanged"
        };
        boot_screen_show_error(e, 4);
        return;
    }

    printf("OTA: image OK — writing %lu sectors to BANK0_APP 0x%08lX\n",
           (unsigned long)((fw_size + 0xFFFu) >> 12),
           (unsigned long)BANK0_APP_ADDR);

    uint32_t sectors = (fw_size + 0xFFFu) >> 12;

    /* Stage 3: overwrite APP area from OTA_TMP — does not return. */
    ota_do_apply(sectors);
}
