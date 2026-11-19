#ifndef MAC_EDIT_H
#define MAC_EDIT_H

#include <stdint.h>
#include <stdbool.h>

/**
 * @brief  Update the OEM MAC address in flash.
 *
 * This function reads the OEM config from flash (0x801000), finds the current
 * MAC address, replaces it with the new one, updates the SHA256 hash in the
 * header, writes the config back to flash, and reboots the chip.
 *
 * @param[in]  new_mac  Pointer to the new 6-byte MAC address.
 * @return true if success, false otherwise (e.g. current MAC not found).
 */
bool mac_edit_update(uint8_t *new_mac);

#endif /* MAC_EDIT_H */
