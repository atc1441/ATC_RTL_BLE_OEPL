#ifndef PANEL_CONFIG_H
#define PANEL_CONFIG_H

/*
 * Panel selection, set by `make PANEL=...` (see gcc/Makefile).
 *
 * PANEL_ID           name shown on the boot screen
 * EPD_OEPL_HW        OEPL hardware type advertised over BLE
 * EPD_NATIVE_W/H     controller RAM size in pixels
 * EPD_SCREEN_W/H     size announced in settings (BLE upload is H x W,
 *                    because screen_w_h_inversed_ble = 1)
 * EPD_BOOT_CHAR_H    boot screen text line height
 * EPD_BOOT_DIRECT    1: boot text is drawn in controller RAM order,
 *                    0: boot text is drawn 90 degrees rotated
 * EPD_BOOT_FLIP      1: rotated boot text is turned by 180 degrees
 * EPD_ROTATE_IMG     1: uploaded image is rotated 90 degrees compared to
 *                    the controller RAM and must be buffered and rotated
 */

#if defined(PANEL_ELM35)

#define PANEL_ID "ELM35R2C4P"
#define EPD_OEPL_HW 0x53
#define EPD_NATIVE_W 224
#define EPD_NATIVE_H 480
#define EPD_SCREEN_W 480
#define EPD_SCREEN_H 224
#define EPD_BOOT_CHAR_H 24
#define EPD_BOOT_DIRECT 0
#define EPD_BOOT_FLIP 0
#define EPD_ROTATE_IMG 0

#elif defined(PANEL_FPC7909)

#define PANEL_ID "FPC7909"
#define EPD_OEPL_HW 0x4C
#define EPD_NATIVE_W 800
#define EPD_NATIVE_H 480
#define EPD_SCREEN_W 480
#define EPD_SCREEN_H 800
#define EPD_BOOT_CHAR_H 16
#define EPD_BOOT_DIRECT 1
#define EPD_BOOT_FLIP 0
#define EPD_ROTATE_IMG 0

#elif defined(PANEL_HINK_E0276A01)

#define PANEL_ID "HINK-E0276A01"
#define EPD_OEPL_HW 0x6E
#define EPD_NATIVE_W 128
#define EPD_NATIVE_H 296
#define EPD_SCREEN_W 128
#define EPD_SCREEN_H 296
#define EPD_BOOT_CHAR_H 16
#define EPD_BOOT_DIRECT 0
#define EPD_BOOT_FLIP 1
#define EPD_ROTATE_IMG 1

#else
#error "Define PANEL_ELM35, PANEL_FPC7909 or PANEL_HINK_E0276A01"
#endif

#endif
