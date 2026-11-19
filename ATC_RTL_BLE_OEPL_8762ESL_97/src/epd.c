#include "epd.h"
#include "board.h"
#include "log_uart.h"
#include <rtl876x_gpio.h>
#include <rtl876x_rcc.h>
#include <rtl876x_pinmux.h>
#include <platform_utils.h>
#include <os_sched.h>

/* -------------------------------------------------------------------------
 * EL097R2CRN low-level bus
 * -------------------------------------------------------------------------
 * The stock firmware uses the same board pins as the existing ELO58R2CRN
 * port.  For bring-up we intentionally keep the already proven GPIO
 * bit-banged SPI implementation.  The stock firmware uses the hardware SPI
 * peripheral on P0_4/P0_6, which can be introduced later as an optimisation.
 *
 * Pin mapping:
 *   P0_0 RST
 *   P0_1 D/C
 *   P0_2 CS
 *   P0_4 CLK
 *   P0_5 EPD power enable (HIGH = on)
 *   P0_6 SDIO/MOSI (output is sufficient for normal drawing)
 *   P3_2 BS/interface select (LOW = SPI)
 *   P3_3 BUSY (HIGH = busy)
 * ------------------------------------------------------------------------- */

static void spi_write_byte(uint8_t byte)
{
    for (int i = 7; i >= 0; i--)
    {
        GPIO_ResetBits(GPIO_GetPin(EPD_CLK_PIN));
        if (byte & (1u << i))
            GPIO_SetBits(GPIO_GetPin(EPD_MOSI_PIN));
        else
            GPIO_ResetBits(GPIO_GetPin(EPD_MOSI_PIN));
        GPIO_SetBits(GPIO_GetPin(EPD_CLK_PIN));
    }
    GPIO_ResetBits(GPIO_GetPin(EPD_CLK_PIN));
}

void epd_cmd(uint8_t cmd)
{
    GPIO_ResetBits(GPIO_GetPin(EPD_DC_PIN));
    GPIO_ResetBits(GPIO_GetPin(EPD_CS_PIN));
    spi_write_byte(cmd);
    GPIO_SetBits(GPIO_GetPin(EPD_CS_PIN));
}

void epd_data(uint8_t data)
{
    GPIO_SetBits(GPIO_GetPin(EPD_DC_PIN));
    GPIO_ResetBits(GPIO_GetPin(EPD_CS_PIN));
    spi_write_byte(data);
    GPIO_SetBits(GPIO_GetPin(EPD_CS_PIN));
}

void epd_write(uint8_t cmd, int n_data, ...)
{
    GPIO_ResetBits(GPIO_GetPin(EPD_DC_PIN));
    GPIO_ResetBits(GPIO_GetPin(EPD_CS_PIN));
    spi_write_byte(cmd);

    if (n_data > 0)
    {
        GPIO_SetBits(GPIO_GetPin(EPD_DC_PIN));
        __builtin_va_list ap;
        __builtin_va_start(ap, n_data);
        for (int i = 0; i < n_data; i++)
            spi_write_byte((uint8_t)__builtin_va_arg(ap, int));
        __builtin_va_end(ap);
    }

    GPIO_SetBits(GPIO_GetPin(EPD_CS_PIN));
}

void epd_stream_data(const uint8_t *buf, uint32_t len)
{
    GPIO_SetBits(GPIO_GetPin(EPD_DC_PIN));
    GPIO_ResetBits(GPIO_GetPin(EPD_CS_PIN));
    for (uint32_t i = 0; i < len; i++)
        spi_write_byte(buf[i]);
    GPIO_SetBits(GPIO_GetPin(EPD_CS_PIN));
}

void epd_stream_const(uint8_t value, uint32_t n)
{
    GPIO_SetBits(GPIO_GetPin(EPD_DC_PIN));
    GPIO_ResetBits(GPIO_GetPin(EPD_CS_PIN));
    for (uint32_t i = 0; i < n; i++)
        spi_write_byte(value);
    GPIO_SetBits(GPIO_GetPin(EPD_CS_PIN));
}

/* BUSY is active HIGH on the stock EL097R2CRN firmware. */
void epd_wait_busy(void)
{
    uint32_t waited_ms = 0;
    while (GPIO_ReadInputDataBit(GPIO_GetPin(EPD_BUSY_PIN)) == Bit_SET)
    {
        platform_delay_ms(10);
        waited_ms += 10;
        if (waited_ms >= 30000u)
        {
            uart_printf("EPD BUSY timeout\n");
            break;
        }
    }
}

/* Exact reset shape reconstructed from the stock firmware:
 * RST LOW 2 ms -> RST HIGH 2 ms -> CS HIGH 2 ms -> SWRESET (0x12) -> 10 ms.
 */
static void epd_hw_reset_stock(void)
{
    GPIO_ResetBits(GPIO_GetPin(EPD_RST_PIN));
    platform_delay_ms(2);
    GPIO_SetBits(GPIO_GetPin(EPD_RST_PIN));
    platform_delay_ms(2);
    GPIO_SetBits(GPIO_GetPin(EPD_CS_PIN));
    platform_delay_ms(2);
    epd_cmd(0x12);
    platform_delay_ms(10);
}

/* Start writing the first (black/white) RAM plane.
 * Stock firmware function at ~0x4307A:
 *   4E BF 03
 *   4F 00 00
 *   24
 */
void epd_begin_bw(void)
{
    epd_write(0x4E, 2, 0xBF, 0x03); /* X counter = 959 */
    epd_write(0x4F, 2, 0x00, 0x00); /* Y counter = 0   */
    epd_cmd(0x24);
}

/* Start writing the second/red RAM plane.
 * Stock firmware function at ~0x430A8.
 */
void epd_begin_red(void)
{
    epd_write(0x4E, 2, 0xBF, 0x03);
    epd_write(0x4F, 2, 0x00, 0x00);
    epd_cmd(0x26);
}

/* Full refresh sequence reconstructed from stock firmware (~0x43B00). */
void epd_refresh(void)
{
    epd_write(0x22, 1, 0xF7);
    epd_cmd(0x20);
    platform_delay_ms(10);
}

/* -------------------------------------------------------------------------
 * Controller initialisation
 * -------------------------------------------------------------------------
 * This command stream is taken from the decrypted original EL097R2CRN
 * firmware.  It configures a 960 x 672 RAM/window:
 *   0x03BF = 959
 *   0x029F = 671
 */
void epd_init(void)
{
    GPIO_InitTypeDef g;
    GPIO_StructInit(&g);

    g.GPIO_Pin  = GPIO_GetPin(EPD_PWR_PIN)  | GPIO_GetPin(EPD_BS_PIN)  |
                  GPIO_GetPin(EPD_CS_PIN)   | GPIO_GetPin(EPD_RST_PIN) |
                  GPIO_GetPin(EPD_DC_PIN)   | GPIO_GetPin(EPD_CLK_PIN) |
                  GPIO_GetPin(EPD_MOSI_PIN);
    g.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_Init(&g);

    g.GPIO_Pin  = GPIO_GetPin(EPD_BUSY_PIN);
    g.GPIO_Mode = GPIO_Mode_IN;
    GPIO_Init(&g);

    /* Stock-compatible idle states. */
    GPIO_ResetBits(GPIO_GetPin(EPD_PWR_PIN));
    GPIO_ResetBits(GPIO_GetPin(EPD_BS_PIN));
    GPIO_SetBits  (GPIO_GetPin(EPD_CS_PIN));
    GPIO_SetBits  (GPIO_GetPin(EPD_RST_PIN));
    GPIO_ResetBits(GPIO_GetPin(EPD_DC_PIN));
    GPIO_ResetBits(GPIO_GetPin(EPD_CLK_PIN));
    GPIO_ResetBits(GPIO_GetPin(EPD_MOSI_PIN));

    /* Stock board init enables P0_5 after a 10 ms settling interval. */
    platform_delay_ms(10);
    GPIO_SetBits(GPIO_GetPin(EPD_PWR_PIN));
    platform_delay_ms(2);

    epd_hw_reset_stock();

    epd_write(0x46, 1, 0xF7);
    epd_wait_busy();
    platform_delay_ms(2);

    epd_write(0x47, 1, 0xF7);
    epd_wait_busy();
    platform_delay_ms(2);

    epd_write(0x0C, 5, 0xAE, 0xC7, 0xC3, 0xC0, 0x80);

    /* Gate count = 0x029F = 671 -> 672 lines. */
    epd_write(0x01, 3, 0x9F, 0x02, 0x00);

    epd_write(0x11, 1, 0x02);

    /* Source window = 0x03BF..0 = 960 pixels. */
    epd_write(0x44, 4, 0xBF, 0x03, 0x00, 0x00);
    epd_write(0x45, 4, 0x00, 0x00, 0x9F, 0x02);

    /* Stock firmware derives this byte from display configuration bits.
     * The normal/default path resolves to 0x01. */
    epd_write(0x3C, 1, 0x01);

    epd_write(0x18, 1, 0x80);
    epd_write(0x22, 1, 0xF7);
}

void epd_display_white(void)
{
    epd_begin_bw();
    epd_stream_const(0xFF, EPD_BUF_SIZE);

    epd_begin_red();
    epd_stream_const(0x00, EPD_BUF_SIZE);

    epd_refresh();
    epd_wait_busy();
}

void epd_sleep(void)
{
    /* The EL097R2CRN stock firmware does not issue a 0x10 deep-sleep
     * command on the normal refresh-complete path.  It waits about 200 ms
     * after BUSY clears and then shuts the board pins down, with EPD_PWR
     * handled last.  Keep the first bring-up path equally conservative. */
    platform_delay_ms(200);
}

/* -------------------------------------------------------------------------
 * Board pad/pinmux setup
 * ------------------------------------------------------------------------- */
void epd_board_init(void)
{
    RCC_PeriphClockCmd(APBPeriph_GPIO, APBPeriph_GPIO_CLOCK, ENABLE);

    Pad_Config(EPD_PWR_PIN,  PAD_PINMUX_MODE, PAD_IS_PWRON, PAD_PULL_NONE, PAD_OUT_ENABLE,  PAD_OUT_LOW);
    Pad_Config(EPD_BS_PIN,   PAD_PINMUX_MODE, PAD_IS_PWRON, PAD_PULL_NONE, PAD_OUT_ENABLE,  PAD_OUT_LOW);
    Pad_Config(EPD_CS_PIN,   PAD_PINMUX_MODE, PAD_IS_PWRON, PAD_PULL_NONE, PAD_OUT_ENABLE,  PAD_OUT_HIGH);
    Pad_Config(EPD_RST_PIN,  PAD_PINMUX_MODE, PAD_IS_PWRON, PAD_PULL_NONE, PAD_OUT_ENABLE,  PAD_OUT_HIGH);
    Pad_Config(EPD_DC_PIN,   PAD_PINMUX_MODE, PAD_IS_PWRON, PAD_PULL_NONE, PAD_OUT_ENABLE,  PAD_OUT_LOW);
    Pad_Config(EPD_CLK_PIN,  PAD_PINMUX_MODE, PAD_IS_PWRON, PAD_PULL_NONE, PAD_OUT_ENABLE,  PAD_OUT_LOW);
    Pad_Config(EPD_MOSI_PIN, PAD_PINMUX_MODE, PAD_IS_PWRON, PAD_PULL_NONE, PAD_OUT_ENABLE,  PAD_OUT_LOW);
    Pad_Config(EPD_BUSY_PIN, PAD_PINMUX_MODE, PAD_IS_PWRON, PAD_PULL_NONE, PAD_OUT_DISABLE, PAD_OUT_LOW);

    Pinmux_Config(EPD_PWR_PIN,  DWGPIO);
    Pinmux_Config(EPD_BS_PIN,   DWGPIO);
    Pinmux_Config(EPD_CS_PIN,   DWGPIO);
    Pinmux_Config(EPD_RST_PIN,  DWGPIO);
    Pinmux_Config(EPD_DC_PIN,   DWGPIO);
    Pinmux_Config(EPD_CLK_PIN,  DWGPIO);
    Pinmux_Config(EPD_MOSI_PIN, DWGPIO);
    Pinmux_Config(EPD_BUSY_PIN, DWGPIO);
}

void epd_board_sleep(void)
{
    /* Stock shutdown releases all interface pins first and P0_5/PWR last. */
    Pinmux_Config(EPD_BS_PIN,   IDLE_MODE);
    Pinmux_Config(EPD_BUSY_PIN, IDLE_MODE);
    Pinmux_Config(EPD_RST_PIN,  IDLE_MODE);
    Pinmux_Config(EPD_DC_PIN,   IDLE_MODE);
    Pinmux_Config(EPD_CS_PIN,   IDLE_MODE);
    Pinmux_Config(EPD_CLK_PIN,  IDLE_MODE);
    Pinmux_Config(EPD_MOSI_PIN, IDLE_MODE);

    Pinmux_Deinit(EPD_BS_PIN);
    Pinmux_Deinit(EPD_BUSY_PIN);
    Pinmux_Deinit(EPD_RST_PIN);
    Pinmux_Deinit(EPD_DC_PIN);
    Pinmux_Deinit(EPD_CS_PIN);
    Pinmux_Deinit(EPD_CLK_PIN);
    Pinmux_Deinit(EPD_MOSI_PIN);

    platform_delay_ms(20);
    GPIO_ResetBits(GPIO_GetPin(EPD_PWR_PIN));
    Pinmux_Config(EPD_PWR_PIN, IDLE_MODE);
    Pinmux_Deinit(EPD_PWR_PIN);
}

void epd_draw_full(void)
{
    epd_board_init();
    epd_init();
    epd_display_white();
    epd_sleep();
    epd_board_sleep();
}

/* Compatibility API ------------------------------------------------------- */
void EPD_Display_start(uint8_t color)
{
    epd_board_init();
    epd_init();
    if (color == 0)
        epd_begin_bw();
    else
        epd_begin_red();
}

void EPD_Display_byte(uint8_t data)
{
    epd_data(data);
}

void EPD_Display_color_change(uint8_t color)
{
    if (color == 0)
        epd_begin_bw();
    else
        epd_begin_red();
}

void EPD_Display_end(void)
{
    epd_refresh();
    epd_wait_busy();
    epd_sleep();
    epd_board_sleep();
}
