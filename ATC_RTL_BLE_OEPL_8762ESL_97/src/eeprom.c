#include "eeprom.h"
#include "board.h"
#include "rtl876x_gpio.h"
#include "rtl876x_pinmux.h"
#include "rtl876x_rcc.h"
#include "platform_utils.h"
#include <stdio.h>
#include <string.h>

void SPI_Flash_CS_set(int a1) {
    if (a1)
        GPIO_SetBits(GPIO_GetPin(EXT_FLASH_CS_PIN));
    else
        GPIO_ResetBits(GPIO_GetPin(EXT_FLASH_CS_PIN));
}

void SPI_Flash_CLK_set(int a1) {
    if (a1)
        GPIO_SetBits(GPIO_GetPin(EXT_FLASH_CLK_PIN));
    else
        GPIO_ResetBits(GPIO_GetPin(EXT_FLASH_CLK_PIN));
}

void SPI_Flash_MOSI_set(int a1) {
    if (a1)
        GPIO_SetBits(GPIO_GetPin(EXT_FLASH_MOSI_PIN));
    else
        GPIO_ResetBits(GPIO_GetPin(EXT_FLASH_MOSI_PIN));
}

uint8_t SPI_Flash_MISO_get() {
    return GPIO_ReadInputDataBit(GPIO_GetPin(EXT_FLASH_MISO_PIN)) != 0;
}

uint8_t SPI_Flash_transceive(int a1); /* forward decl — defined below */

/* ---- Flash deep power-down management ------------------------------------ */
static uint8_t g_flash_awake = 0;

void eepromPowerUp(void) {
    if (g_flash_awake) return;
    SPI_Flash_CS_set(0);
    SPI_Flash_transceive(0xAB);   /* Release from Deep Power-Down */
    SPI_Flash_CS_set(1);
    platform_delay_ms(1);         /* tRES1: typ. 3 µs, 1 ms is safe */
    g_flash_awake = 1;
}

void eepromPowerDown(void) {
    if (!g_flash_awake) return;
    SPI_Flash_CS_set(0);
    SPI_Flash_transceive(0xB9);   /* Deep Power-Down */
    SPI_Flash_CS_set(1);
    g_flash_awake = 0;
}

/* -------------------------------------------------------------------------- */

void SPI_Flash_enable_GPIO() {
    RCC_PeriphClockCmd(APBPeriph_GPIO, APBPeriph_GPIO_CLOCK, ENABLE);

    Pad_Config(EXT_FLASH_CS_PIN, PAD_PINMUX_MODE, PAD_IS_PWRON, PAD_PULL_NONE, PAD_OUT_ENABLE, PAD_OUT_HIGH);
    Pad_Config(EXT_FLASH_CLK_PIN, PAD_PINMUX_MODE, PAD_IS_PWRON, PAD_PULL_NONE, PAD_OUT_ENABLE, PAD_OUT_LOW);
    Pad_Config(EXT_FLASH_MOSI_PIN, PAD_PINMUX_MODE, PAD_IS_PWRON, PAD_PULL_NONE, PAD_OUT_ENABLE, PAD_OUT_LOW);
    Pad_Config(EXT_FLASH_MISO_PIN, PAD_PINMUX_MODE, PAD_IS_PWRON, PAD_PULL_UP, PAD_OUT_DISABLE, PAD_OUT_LOW);

    Pinmux_Config(EXT_FLASH_CS_PIN, DWGPIO);
    Pinmux_Config(EXT_FLASH_CLK_PIN, DWGPIO);
    Pinmux_Config(EXT_FLASH_MOSI_PIN, DWGPIO);
    Pinmux_Config(EXT_FLASH_MISO_PIN, DWGPIO);

    GPIO_InitTypeDef g;
    GPIO_StructInit(&g);
    g.GPIO_Pin = GPIO_GetPin(EXT_FLASH_CS_PIN) | GPIO_GetPin(EXT_FLASH_CLK_PIN) | GPIO_GetPin(EXT_FLASH_MOSI_PIN);
    g.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_Init(&g);

    g.GPIO_Pin = GPIO_GetPin(EXT_FLASH_MISO_PIN);
    g.GPIO_Mode = GPIO_Mode_IN;
    GPIO_Init(&g);

    SPI_Flash_CS_set(1);
    SPI_Flash_CLK_set(0);
}

uint8_t SPI_Flash_transceive(int a1) {
    uint8_t v2 = 0;
    uint8_t v3 = 0;
    do {
        SPI_Flash_MOSI_set(((0x80u >> v3) & a1) != 0);
        SPI_Flash_CLK_set(1);
        if (SPI_Flash_MISO_get() == 1)
            v2 |= 0x80u >> v3;
        SPI_Flash_CLK_set(0);
        v3++;
    } while (v3 < 8);
    return v2;
}

void Flash_wait_busy() {
    int v0;
    SPI_Flash_CS_set(0);
    SPI_Flash_transceive(0x05);
    v0 = 1000000;
    while (v0 && (SPI_Flash_transceive(0xff) & 1)) {
        v0--;
    }
    SPI_Flash_CS_set(1);
}

void Flash_CMD_WriteEN() {
    SPI_Flash_CS_set(0);
    SPI_Flash_transceive(0x06);
    SPI_Flash_CS_set(1);
}

void FLASH_Init() {
    SPI_Flash_enable_GPIO();
    platform_delay_ms(20);

    /* Release from power-down (0xAB). */
    SPI_Flash_CS_set(0);
    SPI_Flash_transceive(0xab);
    SPI_Flash_CS_set(1);
    platform_delay_ms(1);   /* tRES1 wake-up time (typ. 3 µs, 1 ms is safe) */

    /* Read JEDEC ID.
     * IMPORTANT: read all 3 bytes before printing — a printf() between SPI
     * bytes leaves CS low for ~1 ms while UART transmits; flash chips have
     * a CS deselect timeout and will abort the command, returning garbage. */
    uint8_t j[3];
    SPI_Flash_CS_set(0);
    SPI_Flash_transceive(0x9F);
    j[0] = SPI_Flash_transceive(0xFF);
    j[1] = SPI_Flash_transceive(0xFF);
    j[2] = SPI_Flash_transceive(0xFF);
    SPI_Flash_CS_set(1);
    printf("Flash JEDEC: %02X %02X %02X\n", j[0], j[1], j[2]);
    g_flash_awake = 1;   /* chip is awake after 0xAB above */
    eepromPowerDown();   /* immediately back to deep power-down */
}

void eepromRead(uint32_t addr, uint8_t *dst, uint32_t len) {
    SPI_Flash_CS_set(0);
    SPI_Flash_transceive(0x03);
    SPI_Flash_transceive((addr >> 16) & 0xff);
    SPI_Flash_transceive((addr >> 8) & 0xff);
    SPI_Flash_transceive(addr & 0xff);
    for (uint32_t i = 0; i < len; ++i)
        dst[i] = SPI_Flash_transceive(0xff);
    SPI_Flash_CS_set(1);
}

void Flash_write_page(uint32_t addr, uint8_t *src, uint32_t len) {
    Flash_CMD_WriteEN();
    SPI_Flash_CS_set(0);
    SPI_Flash_transceive(0x02);
    SPI_Flash_transceive((addr >> 16) & 0xff);
    SPI_Flash_transceive((addr >> 8) & 0xff);
    SPI_Flash_transceive(addr & 0xff);
    for (uint32_t i = 0; i < len; i++)
        SPI_Flash_transceive(src[i]);
    SPI_Flash_CS_set(1);
    Flash_wait_busy();
}

uint8_t eepromWrite(uint32_t addr, uint8_t *src, uint32_t len) {
    while (len) {
        uint32_t lenNow = EEPROM_WRITE_PAGE_SZ - (addr & (EEPROM_WRITE_PAGE_SZ - 1));
        if (lenNow > len)
            lenNow = len;
        Flash_write_page(addr, src, lenNow);
        addr += lenNow;
        src += lenNow;
        len -= lenNow;
    }
    return 1;
}

void Flash_BlkErase_4k(uint32_t addr) {
    Flash_CMD_WriteEN();
    SPI_Flash_CS_set(0);
    SPI_Flash_transceive(0x20);
    SPI_Flash_transceive((addr >> 16) & 0xff);
    SPI_Flash_transceive((addr >> 8) & 0xff);
    SPI_Flash_transceive(addr & 0xff);
    SPI_Flash_CS_set(1);
    Flash_wait_busy();
}

uint8_t eepromErase(uint32_t addr, uint32_t len) {
    if (addr % EEPROM_PAGE_SIZEour) {
        len += addr % EEPROM_PAGE_SIZEour;
        addr = addr / EEPROM_PAGE_SIZEour * EEPROM_PAGE_SIZEour;
    }
    len = (len + EEPROM_PAGE_SIZEour - 1) / EEPROM_PAGE_SIZEour * EEPROM_PAGE_SIZEour;

    while (len) {
        Flash_BlkErase_4k(addr);
        addr += 0x1000;
        len -= 0x1000;
    }
    return 1;
}

uint32_t eepromGetSize(void) {
    return EEPROM_IMG_LEN;
}

/* -------------------------------------------------------------------------
 * flash_selftest — boot-time SPI flash driver verification.
 *
 * Checks in order:
 *  1. JEDEC ID (manufacturer byte must be 0x01..0xFE)
 *  2. Status register: verify WEL bit is set after WREN command
 *  3. Sector erase at FLASH_TEST_ADDR
 *  4. Blank verify (all 0xFF)
 *  5. Write 16-byte pattern; read back; compare byte by byte
 *  6. Erase test sector (cleanup)
 *
 * Test address: last 4 KB sector of the flash (0x7F000 for a 512 KB chip).
 * The EL097R2CRN layout uses one 0x28000 image slot; this test sector
 * remains at 0x7F000 and is outside both image and OTA staging areas.
 *
 * FLASH_SELFTEST_EN in app_flags.h controls whether driver_init() calls this.
 * Set to 0 once the flash driver is confirmed working.
 * ------------------------------------------------------------------------- */
#define FLASH_TEST_ADDR  (EEPROM_IMG_LEN - EEPROM_PAGE_SIZEour)  /* 0x7F000 */

/* Read the flash status register (SR1). */
static uint8_t flash_read_sr(void) {
    SPI_Flash_CS_set(0);
    SPI_Flash_transceive(0x05);           /* RDSR command */
    uint8_t sr = SPI_Flash_transceive(0xFF);
    SPI_Flash_CS_set(1);
    return sr;
}

uint8_t flash_selftest(void) {
    uint8_t jedec[3];
    uint8_t wbuf[16];
    uint8_t rbuf[16];
    uint8_t sr, pass = 1;

    eepromPowerUp();
    printf("FLASH selftest ---------------------------\n");

    /* ---- 1. JEDEC ID (read all 3 bytes before any printf) --------------- */
    SPI_Flash_CS_set(0);
    SPI_Flash_transceive(0x9F);
    jedec[0] = SPI_Flash_transceive(0xFF);   /* manufacturer */
    jedec[1] = SPI_Flash_transceive(0xFF);   /* memory type  */
    jedec[2] = SPI_Flash_transceive(0xFF);   /* capacity     */
    SPI_Flash_CS_set(1);

    printf("  JEDEC: %02X %02X %02X", jedec[0], jedec[1], jedec[2]);
    if (jedec[0] == 0xFF || jedec[0] == 0x00) {
        printf("  FAIL: no chip response (check SPI wiring)\n");
        printf("FLASH selftest FAILED\n");
        return 0;
    }
    printf("  OK\n");

    /* ---- 2. Status register + Write Enable ------------------------------ */
    sr = flash_read_sr();
    printf("  SR before WREN: 0x%02X  WIP=%d WEL=%d BP=%d\n",
           sr, sr & 1, (sr >> 1) & 1, (sr >> 2) & 7);

    Flash_CMD_WriteEN();   /* send 0x06 */
    sr = flash_read_sr();
    printf("  SR after WREN:  0x%02X  WEL=%d", sr, (sr >> 1) & 1);
    if (!((sr >> 1) & 1)) {
        printf("  FAIL: WEL not set — chip may be write-protected\n");
        printf("FLASH selftest FAILED\n");
        return 0;
    }
    printf("  OK\n");

    /* ---- 3. Sector erase ------------------------------------------------- */
    printf("  Erase 0x%05lX ...", (unsigned long)FLASH_TEST_ADDR);
    Flash_BlkErase_4k(FLASH_TEST_ADDR);   /* includes Flash_wait_busy */
    sr = flash_read_sr();
    printf(" done  SR=0x%02X WIP=%d\n", sr, sr & 1);
    if (sr & 1) {
        printf("  FAIL: WIP still set after erase\n");
        printf("FLASH selftest FAILED\n");
        return 0;
    }

    /* ---- 4. Blank check -------------------------------------------------- */
    eepromRead(FLASH_TEST_ADDR, rbuf, 16);
    printf("  Blank:");
    for (uint8_t i = 0; i < 16; i++) {
        printf(" %02X", rbuf[i]);
        if (rbuf[i] != 0xFF) pass = 0;
    }
    if (!pass) {
        printf("\n  FAIL: not blank after erase — erase cmd not accepted?\n");
        printf("FLASH selftest FAILED\n");
        return 0;
    }
    printf("  OK\n");

    /* ---- 5. Write + readback --------------------------------------------- */
    for (uint8_t i = 0; i < 16; i++) wbuf[i] = (uint8_t)(0xA5 ^ i);

    eepromWrite(FLASH_TEST_ADDR, wbuf, 16);
    eepromRead (FLASH_TEST_ADDR, rbuf, 16);

    printf("  Write:");
    for (uint8_t i = 0; i < 16; i++) printf(" %02X", wbuf[i]);
    printf("\n  Read: ");
    pass = 1;
    for (uint8_t i = 0; i < 16; i++) {
        printf(" %02X", rbuf[i]);
        if (rbuf[i] != wbuf[i]) { pass = 0; printf("(!)"); }
    }
    printf("\n  Verify: %s\n", pass ? "OK" : "FAIL");

    /* ---- 6. Cleanup ------------------------------------------------------- */
    eepromErase(FLASH_TEST_ADDR, EEPROM_PAGE_SIZEour);
    printf("  Sector erased (clean)\n");
    printf("FLASH selftest %s\n", pass ? "PASSED" : "FAILED");
    printf("------------------------------------------\n");
    eepromPowerDown();
    return pass;
}
