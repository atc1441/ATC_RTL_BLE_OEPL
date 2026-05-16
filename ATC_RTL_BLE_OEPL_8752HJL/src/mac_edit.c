#include "mac_edit.h"
#include "flash_map.h"
#include "patch_header_check.h"
#include "flash_device.h"
#include "rtl876x_hw_sha256.h"
#include "rtl876x_wdg.h"
#include "gap.h"
#include "os_mem.h"
#include <string.h>
#include <stdio.h>

bool mac_edit_update(uint8_t *new_mac)
{
    uint8_t current_mac[6];
    if (gap_get_param(GAP_PARAM_BD_ADDR, current_mac) != GAP_CAUSE_SUCCESS) {
        printf("MAC EDIT: Failed to get current MAC\n");
        return false;
    }

    printf("MAC EDIT: Current MAC %02X:%02X:%02X:%02X:%02X:%02X\n",
           current_mac[5], current_mac[4], current_mac[3],
           current_mac[2], current_mac[1], current_mac[0]);

    uint8_t *buffer = (uint8_t *)os_mem_alloc(RAM_TYPE_DATA_ON, 0x1000);
    if (!buffer) {
        printf("MAC EDIT: Memory allocation failed\n");
        return false;
    }

    // Read 0x1000 bytes from OEM_CFG_ADDR
    if (!flash_read_locked(OEM_CFG_ADDR, 0x1000, buffer)) {
        printf("MAC EDIT: Flash read failed\n");
        os_mem_free(buffer);
        return false;
    }

    T_IMG_HEADER_FORMAT *p_hdr = (T_IMG_HEADER_FORMAT *)buffer;

    // Validate header
    if (p_hdr->ctrl_header.image_id != IMAGE_FIRST) {
        printf("MAC EDIT: Invalid image ID 0x%04X (expected 0x%04X)\n", 
               p_hdr->ctrl_header.image_id, IMAGE_FIRST);
        os_mem_free(buffer);
        return false;
    }

    // Find current MAC in data section (offset 0x400 to 0x1000)
    bool found = false;
    for (uint32_t i = 0x400; i <= 0x1000 - 6; i++) {
        if (memcmp(&buffer[i], current_mac, 6) == 0) {
            printf("MAC EDIT: Found MAC at offset 0x%03lX\n", (unsigned long)i);
            memcpy(&buffer[i], new_mac, 6);
            found = true;
            break;
        }
    }

    if (!found) {
        printf("MAC EDIT: Current MAC not found in config data\n");
        os_mem_free(buffer);
        return false;
    }

    // Update SHA256
    uint32_t payload_len = p_hdr->ctrl_header.payload_len;
    printf("MAC EDIT: Payload len %lu\n", (unsigned long)payload_len);
    
    // Payload starts at offset 0x400
    // Check if payload_len fits in our 4K buffer after the 1K header
    if (payload_len > 0xC00) {
        printf("MAC EDIT: Payload len %lu exceeds buffer limit (0xC00), capping for hash\n", 
               (unsigned long)payload_len);
        payload_len = 0xC00;
    }

    uint32_t hash[8];
    hw_sha256_init();
    // Mode 0 is HW_SHA256_CPU_MODE
    if (hw_sha256(&buffer[0x1B0], payload_len + 0x250, hash, 0)) {
        memcpy(p_hdr->auth.image_hash, hash, 32);
        printf("MAC EDIT: SHA256 updated\n");
    } else {
        printf("MAC EDIT: SHA256 calculation failed\n");
        os_mem_free(buffer);
        return false;
    }

    // Write back to flash
    printf("MAC EDIT: Erasing and writing flash...\n");
    if (!flash_erase_locked(FLASH_ERASE_SECTOR, OEM_CFG_ADDR)) {
        printf("MAC EDIT: Flash erase failed\n");
        os_mem_free(buffer);
        return false;
    }
    
    for (uint32_t i = 0; i < 0x1000; i += 256) {
        if (!flash_write_locked(OEM_CFG_ADDR + i, 256, &buffer[i])) {
            printf("MAC EDIT: Flash write failed at offset 0x%lx\n", (unsigned long)i);
            os_mem_free(buffer);
            return false;
        }
    }

    printf("MAC EDIT: Success! Rebooting to apply new config...\n");
    
    // Give some time for UART to flush
    for(volatile int i=0; i<10000; i++);

    os_mem_free(buffer);

    // Reboot the chip using WDG_SystemReset for a clean and immediate restart
    WDG_SystemReset(RESET_ALL, UPPER_CMD_RESET);
    while (1);

    return true;
}
