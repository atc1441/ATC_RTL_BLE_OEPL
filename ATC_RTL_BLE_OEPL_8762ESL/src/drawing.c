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

uint32_t byteCounter = 0;
volatile bool g_epd_refreshing = false; /* true while EPD BUSY pin is HIGH after 0x20 */

/* ---- zlib decompression --------------------------------------------------- */
#define ZLIB_CACHE_SIZE  512
#define MAX_WINDOW_SIZE  4096

static uint8_t  z_comp_buf[ZLIB_CACHE_SIZE];
static uint8_t  z_row_buf[100];       /* one full-width row: 800px / 8 = 100 bytes */
static uint8_t  z_dict[MAX_WINDOW_SIZE];

static uint32_t z_comp_size;          /* total compressed bytes (after the 4-byte size prefix) */
static uint32_t z_comp_pos;           /* read cursor into the compressed stream */
static uint32_t z_eeprom_base;        /* EEPROM address of the 4-byte size prefix */

static int zlib_read_cb(struct uzlib_uncomp *data)
{
    uint32_t left = z_comp_size - z_comp_pos;
    if (left == 0) return -1;
    if (left > ZLIB_CACHE_SIZE) left = ZLIB_CACHE_SIZE;
    eepromRead(z_eeprom_base + 4 + z_comp_pos, z_comp_buf, left);
    data->source       = z_comp_buf + 1;
    data->source_limit = z_comp_buf + left;
    z_comp_pos        += left;
    return z_comp_buf[0];
}

/* Decompress one colour plane row-by-row and dispatch to both EPD panels. */
static bool decompress_plane_rows(struct uzlib_uncomp *d,
                                   uint8_t cmd_p1, uint8_t cmd_p2,
                                   uint32_t n_rows, uint32_t bpr, uint8_t invert)
{
    for (uint32_t r = 0; r < n_rows; r++) {
        d->dest       = z_row_buf;
        d->dest_start = z_row_buf;
        d->dest_limit = z_row_buf + bpr;
        int res = uzlib_uncompress(d);
        if (res < TINF_OK) {
            printf("ZLIB plane err %d row %lu\n", res, (unsigned long)r);
            return false;
        }
        if (invert) {
            for (uint32_t i = 0; i < bpr; i++) z_row_buf[i] = ~z_row_buf[i];
        }
        epd_cmd(cmd_p1); epd_stream_data(z_row_buf,      50);
        epd_cmd(cmd_p2); epd_stream_data(z_row_buf + 49, 50);
        byteCounter += 100;
    }
    return true;
}

/* Populate both panel RAMs from a DATATYPE_IMG_ZLIB slot. Returns false on error. */
static bool draw_zlib(uint32_t addr, const struct EepromImageHeader *eih)
{
    const uint32_t data_base = addr + sizeof(struct EepromImageHeader);
    const uint32_t H   = settings.screen_h;       /* 272 rows  */
    const uint32_t bpr = settings.screen_w / 8;   /* 100 bytes */

    /* 4-byte decompressed-size prefix before the actual zlib stream */
    uint32_t decomp_size;
    eepromRead(data_base, (uint8_t *)&decomp_size, 4);

    z_comp_size   = eih->size - 4;
    z_comp_pos    = ZLIB_CACHE_SIZE;   /* first callback reads from offset ZLIB_CACHE_SIZE */
    z_eeprom_base = data_base;
    eepromRead(data_base + 4, z_comp_buf, ZLIB_CACHE_SIZE); /* prime the buffer */

    struct uzlib_uncomp d;
    d.source         = z_comp_buf;
    d.source_limit   = z_comp_buf + ZLIB_CACHE_SIZE;
    d.source_read_cb = zlib_read_cb;

    int wbits = uzlib_zlib_parse_header(&d);
    if (wbits < 0) { printf("ZLIB header err %d\n", wbits); return false; }
    uint16_t window = (uint16_t)(0x100u << wbits);
    if (window > MAX_WINDOW_SIZE) { printf("ZLIB window %u > max\n", window); return false; }
    uzlib_uncompress_init(&d, z_dict, window);

    /* Custom image header: first byte = total header length */
    uint8_t hdr_len_byte;
    d.dest = &hdr_len_byte; d.dest_start = &hdr_len_byte; d.dest_limit = &hdr_len_byte + 1;
    if (uzlib_uncompress(&d) < TINF_OK) { printf("ZLIB hdr len err\n"); return false; }

    uint8_t hdr_buf[32];
    uint8_t hdr_data_len = hdr_len_byte - 1;
    if (hdr_data_len > (uint8_t)sizeof(hdr_buf)) hdr_data_len = (uint8_t)sizeof(hdr_buf);
    d.dest = hdr_buf; d.dest_start = hdr_buf; d.dest_limit = hdr_buf + hdr_data_len;
    if (uzlib_uncompress(&d) < TINF_OK) { printf("ZLIB hdr err\n"); return false; }

    uint8_t bpp = hdr_buf[4]; /* 1 = BW only, 2 = BW + Red */
    printf("ZLIB %ux%u bpp=%u decomp=%lu\n",
           (unsigned)((hdr_buf[1] << 8) | hdr_buf[0]),
           (unsigned)((hdr_buf[3] << 8) | hdr_buf[2]),
           (unsigned)bpp, (unsigned long)decomp_size);

    /* BW plane — same panel/command assignment as draw_rows_deinterleave */
    if (!decompress_plane_rows(&d, 0xA4, 0x24, H, bpr, /*invert=*/1)) return false;

    /* Red/second-colour plane or constant fill */
    if (bpp >= 2) {
        if (!decompress_plane_rows(&d, 0xA6, 0x26, H, bpr, /*invert=*/0)) return false;
    } else {
        epd_cmd(0x26); epd_stream_const(0x00, EPD_BUF_SIZE);
        epd_cmd(0xA6); epd_stream_const(0x00, EPD_BUF_SIZE);
        byteCounter += 100;
    }
    return true;
}

/* --------------------------------------------------------------------------- */

void drawOnOffline(uint8_t state) { (void)state; }

/* ---- Row-by-row dual-panel deinterleave ---------------------------------- */
static void draw_rows_deinterleave(uint32_t data_base, uint32_t n_rows,
                                   uint32_t bytes_per_row,
                                   uint8_t cmd_p1, uint8_t cmd_p2,
                                   uint8_t invert)
{
    static uint8_t row_buf[128]; 
    const uint32_t bytes_p1 = 50; // Panel 1: 400px
    const uint32_t bytes_p2 = 50; // Panel 2: 400px

    for (uint32_t r = 0; r < n_rows; r++)
    {
        eepromRead(data_base + r * bytes_per_row, row_buf, bytes_per_row);
        
        if (invert)
        {
            for (uint32_t i = 0; i < bytes_per_row; i++)
                row_buf[i] = ~row_buf[i];
        }

        epd_cmd(cmd_p1);
        epd_stream_data(row_buf, bytes_p1);

        epd_cmd(cmd_p2);
        epd_stream_data(row_buf + 49, bytes_p2);

        byteCounter += (bytes_p1 + bytes_p2);
    }
}

/* ---- Main draw entry point ---------------------------------------------- */

void drawImageAtAddress(uint32_t addr, uint8_t lut)
{
    (void)lut;
    byteCounter = 0;

    eepromPowerUp();

    /* Read header from EEPROM. */
    struct EepromImageHeader eih;
    eepromRead(addr, (uint8_t *)&eih, sizeof(eih));

    const uint32_t data_base = addr + sizeof(struct EepromImageHeader);
    const uint32_t bytes_per_row = settings.screen_w / 8; /* 100 for 800 px */
    const uint32_t H = settings.screen_h;                 /* 272 rows       */
    const uint8_t inv_bw = 1;
    const uint8_t inv_red = 0;

    printf("DRAW addr=0x%lX type=0x%02X size=%lu inv_bw=%d inv_red=%d\n",
           (unsigned long)addr, eih.dataType, (unsigned long)eih.size,
           inv_bw, inv_red);

    switch (eih.dataType)
    {

    /* ------------------------------------------------------------------ */
    case DATATYPE_IMG_RAW_1BPP:
        printf("DRAW 1bpp %ux%u (dual panel row deinterleave)\n",
               settings.screen_w, settings.screen_h);
        epd_board_init();
        epd_init();
        /* BW plane: row-by-row deinterleave into both panels */
        draw_rows_deinterleave(data_base, H, bytes_per_row, 0xA4, 0x24, inv_bw);
        /* Red planes: no colour data in 1bpp — constant fill */
        epd_cmd(0x26);
        epd_stream_const(inv_red ? 0xFF : 0x00, EPD_BUF_SIZE);
        epd_cmd(0xA6);
        epd_stream_const(inv_red ? 0xFF : 0x00, EPD_BUF_SIZE);
        break;

    /* ------------------------------------------------------------------ */
    case DATATYPE_IMG_RAW_2BPP:
        printf("DRAW 2bpp %ux%u (dual panel row deinterleave)\n",
               settings.screen_w, settings.screen_h);
        epd_board_init();
        epd_init();
        /* BW plane */
        draw_rows_deinterleave(data_base, H, bytes_per_row, 0xA4, 0x24, inv_bw);
        /* Red plane: immediately follows the BW plane */
        draw_rows_deinterleave(data_base + H * bytes_per_row, H, bytes_per_row, 0xA6, 0x26, inv_red);
        break;

    /* ------------------------------------------------------------------ */
    case DATATYPE_IMG_ZLIB:
        printf("DRAW ZLIB %ux%u\n", settings.screen_w, settings.screen_h);
        epd_board_init();
        epd_init();
        if (!draw_zlib(addr, &eih)) {
            eepromPowerDown();
            return;
        }
        break;

    /* ------------------------------------------------------------------ */
    case DATATYPE_IMG_BMP:
        printf("DRAW BMP not implemented\n");
        eepromPowerDown();
        return;

    default:
        printf("DRAW unknown type 0x%02X\n", eih.dataType);
        eepromPowerDown();
        return;
    }

    /* Flash not needed during the ~25 s EPD refresh — sleep it now. */
    eepromPowerDown();

    /* Trigger refresh — set flag BEFORE epd_cmd so DLPS callbacks see it. */
    g_epd_refreshing = true;
    epd_cmd(0x20);
    printf("DRAW refresh triggered (%lu bytes sent)\n", (unsigned long)byteCounter);
}

/* Check whether the EPD has finished refreshing (BUSY pin LOW).
 * When done: calls epd_sleep() + epd_board_sleep() and returns true.
 * Returns false while still busy — call again later. */
bool epd_refresh_done(void)
{
    if (GPIO_ReadInputDataBit(GPIO_GetPin(EPD_BUSY_PIN)) != Bit_RESET)
        return false;
    g_epd_refreshing = false;   /* clear before board_sleep so next DLPS skips EPD handling */
    epd_sleep();
    epd_board_sleep();
    printf("DRAW done\n");
    return true;
}
