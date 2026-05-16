#include "drawing.h"
#include "epd.h"
#include "board.h"
#include "eeprom.h"
#include "peripheral_app.h"
#include "tag_types.h"
#include "zlib/uzlib.h"
#include <string.h>
#include <rtl876x_gpio.h>
#include <rtl876x_rcc.h>
#include <rtl876x_pinmux.h>
#include <platform_utils.h>
#include <os_sched.h>

#define EPD_PIXEL_WHITE 0x00
#define EPD_PIXEL_BLACK 0x01
#define EPD_PIXEL_RED 0x02
#define EPD_PIXEL_YELLOW 0x03

uint32_t byteCounter = 0;
volatile bool g_epd_refreshing = false;

#define ZLIB_CACHE_SIZE 512
#define MAX_WINDOW_SIZE 4096

static uint8_t z_comp_buf[ZLIB_CACHE_SIZE];
static uint8_t z_comp_buf1[ZLIB_CACHE_SIZE];
static uint8_t z_dict[MAX_WINDOW_SIZE];
static uint8_t z_dict1[MAX_WINDOW_SIZE];

static uint32_t z_comp_size;
static uint32_t z_eeprom_base;
static uint32_t z_pos0, z_pos1;

static void push_bwry_pixels(uint8_t bw_byte, uint8_t ry_byte)
{
    uint8_t outH = 0, outL = 0;
    for (int i = 7; i >= 0; i--)
    {
        uint8_t bitBW = (bw_byte >> i) & 0x01;
        uint8_t bitRY = (ry_byte >> i) & 0x01;
        uint8_t pixel = (bitRY == 0) ? (bitBW ? EPD_PIXEL_BLACK : EPD_PIXEL_WHITE)
                                     : (bitBW ? EPD_PIXEL_YELLOW : EPD_PIXEL_RED);
        if (i >= 4)
            outH = (outH << 2) | pixel;
        else
            outL = (outL << 2) | pixel;
    }
    epd_data(outH);
    epd_data(outL);
    byteCounter += 2;
}

static int zlib_read_cb0(struct uzlib_uncomp *data)
{
    uint32_t left = z_comp_size - z_pos0;
    if (left == 0)
        return -1;
    uint32_t r = (left > ZLIB_CACHE_SIZE) ? ZLIB_CACHE_SIZE : left;
    eepromRead(z_eeprom_base + 4 + z_pos0, z_comp_buf, r);
    data->source = z_comp_buf + 1;
    data->source_limit = z_comp_buf + r;
    z_pos0 += r;
    return z_comp_buf[0];
}

static int zlib_read_cb1(struct uzlib_uncomp *data)
{
    uint32_t left = z_comp_size - z_pos1;
    if (left == 0)
        return -1;
    uint32_t r = (left > ZLIB_CACHE_SIZE) ? ZLIB_CACHE_SIZE : left;
    eepromRead(z_eeprom_base + 4 + z_pos1, z_comp_buf1, r);
    data->source = z_comp_buf1 + 1;
    data->source_limit = z_comp_buf1 + r;
    z_pos1 += r;
    return z_comp_buf1[0];
}

static bool draw_zlib(uint32_t addr, const struct EepromImageHeader *eih)
{
    z_eeprom_base = addr + sizeof(struct EepromImageHeader);
    z_comp_size = eih->size - 4;
    uint32_t decomp_size_from_flash;
    eepromRead(z_eeprom_base, (uint8_t *)&decomp_size_from_flash, 4);

    printf("ZLIB: Base=0x%08lX, CompSize=%lu, DecompSizeHeader=%lu\n", z_eeprom_base, z_comp_size, decomp_size_from_flash);

    struct uzlib_uncomp d0;
    z_pos0 = ZLIB_CACHE_SIZE;
    eepromRead(z_eeprom_base + 4, z_comp_buf, ZLIB_CACHE_SIZE);

    printf("ZLIB: Header Bytes: %02X %02X %02X %02X\n", z_comp_buf[0], z_comp_buf[1], z_comp_buf[2], z_comp_buf[3]);

    d0.source = z_comp_buf;
    d0.source_limit = z_comp_buf + ZLIB_CACHE_SIZE;
    d0.source_read_cb = zlib_read_cb0;

    int res = uzlib_zlib_parse_header(&d0);
    if (res < 0)
    {
        printf("ZLIB Error: parse_header failed (%d)\n", res);
        return false;
    }
    uzlib_uncompress_init(&d0, z_dict, (0x100 << res));

    uint8_t img_hdr[6];
    d0.dest = img_hdr;
    d0.dest_start = img_hdr;
    d0.dest_limit = img_hdr + 6;
    if (uzlib_uncompress(&d0) < TINF_OK)
    {
        printf("ZLIB Error: Failed to read image info header\n");
        return false;
    }
    uint8_t bpp = img_hdr[5];
    printf("ZLIB: Info -> %dbpp\n", bpp);

    uint32_t plane_bytes = (settings.screen_w * settings.screen_h) / 8;
    epd_cmd(0x10);

    if (bpp == 1)
    {
        for (uint32_t i = 0; i < plane_bytes; i++)
        {
            uint8_t b;
            d0.dest = &b;
            d0.dest_start = &b;
            d0.dest_limit = &b + 1;
            uzlib_uncompress(&d0);
            if (settings.screen_color_black_invert)
                b = ~b;
            push_bwry_pixels(b, 0x00);
        }
    }
    else
    {
        struct uzlib_uncomp d1;
        z_pos1 = ZLIB_CACHE_SIZE;
        eepromRead(z_eeprom_base + 4, z_comp_buf1, ZLIB_CACHE_SIZE);
        d1.source = z_comp_buf1;
        d1.source_limit = z_comp_buf1 + ZLIB_CACHE_SIZE;
        d1.source_read_cb = zlib_read_cb1;
        int res1 = uzlib_zlib_parse_header(&d1);
        uzlib_uncompress_init(&d1, z_dict1, (0x100 << res1));

        uint8_t skip;
        uint32_t to_skip = plane_bytes + 6;
        printf("ZLIB: Skipping %lu bytes on Stream 1...\n", to_skip);
        while (to_skip--)
        {
            d1.dest = &skip;
            d1.dest_start = &skip;
            d1.dest_limit = &skip + 1;
            if (uzlib_uncompress(&d1) < TINF_OK)
            {
                printf("Skip Error!\n");
                return false;
            }
        }

        for (uint32_t i = 0; i < plane_bytes; i++)
        {
            uint8_t b0, b1;
            d0.dest = &b0;
            d0.dest_start = &b0;
            d0.dest_limit = &b0 + 1;
            d1.dest = &b1;
            d1.dest_start = &b1;
            d1.dest_limit = &b1 + 1;
            uzlib_uncompress(&d0);
            uzlib_uncompress(&d1);
            if (settings.screen_color_black_invert)
                b0 = ~b0;
            if (settings.screen_color_second_invert)
                b1 = ~b1;
            push_bwry_pixels(b0, b1);
        }
    }
    printf("ZLIB Done. Sent %lu bytes\n", byteCounter);
    return true;
}

void drawImageAtAddress(uint32_t addr, uint8_t lut)
{
    (void)lut;
    printf("\nDRAW START: 0x%08lX\n", addr);
    byteCounter = 0;
    eepromPowerUp();

    struct EepromImageHeader eih;
    eepromRead(addr, (uint8_t *)&eih, sizeof(eih));
    const uint32_t plane_bytes = (settings.screen_w * settings.screen_h) / 8;
    const uint32_t data_base = addr + sizeof(struct EepromImageHeader);

    epd_board_init();
    epd_init();

    if (eih.dataType == DATATYPE_IMG_RAW_1BPP)
    {
        printf("Mode: RAW 1BPP\n");
        epd_cmd(0x10);
        for (uint32_t i = 0; i < plane_bytes; i++)
        {
            uint8_t b;
            eepromRead(data_base + i, &b, 1);
            if (settings.screen_color_black_invert)
                b = ~b;
            push_bwry_pixels(b, 0x00);
        }
    }
    else if (eih.dataType == DATATYPE_IMG_RAW_2BPP)
    {
        printf("Mode: RAW 2BPP\n");
        epd_cmd(0x10);
        for (uint32_t i = 0; i < plane_bytes; i++)
        {
            uint8_t b0, b1;
            eepromRead(data_base + i, &b0, 1);
            eepromRead(data_base + plane_bytes + i, &b1, 1);
            if (settings.screen_color_black_invert)
                b0 = ~b0;
            if (settings.screen_color_second_invert)
                b1 = ~b1;
            push_bwry_pixels(b0, b1);
        }
    }
    else if (eih.dataType == DATATYPE_IMG_ZLIB)
    {
        if (!draw_zlib(addr, &eih))
        {
            printf("ZLIB Failed!\n");
            eepromPowerDown();
            epd_sleep();
            epd_board_sleep();
            return;
        }
    }

    eepromPowerDown();
    g_epd_refreshing = true;
    epd_cmd(0x04);
    epd_wait_busy();
    epd_write(0x12, 1, 0x00);
    printf("Refresh triggered...\n");
}

bool epd_refresh_done(void)
{
    if (GPIO_ReadInputDataBit(GPIO_GetPin(EPD_BUSY_PIN)) == Bit_RESET)
        return false;
    if (g_epd_refreshing)
    {
        g_epd_refreshing = false;
        epd_sleep();
        epd_board_sleep();
        printf("Refresh finished.\n");
    }
    return true;
}