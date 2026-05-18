#include "epd.h"
#include "board.h"
#include <rtl876x_gpio.h>
#include <rtl876x_rcc.h>
#include <rtl876x_pinmux.h>
#include <platform_utils.h>
#include <os_sched.h>

// 224 * 480 * 2 / 8 = 26880 Bytes
#define NEW_EPD_BUF_SIZE 26880

void spi_write_byte(uint8_t byte)
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

/* Send one command byte (DC LOW, inside CS pulse) */
void epd_cmd(uint8_t cmd)
{
    GPIO_ResetBits(GPIO_GetPin(EPD_DC_PIN));
    GPIO_ResetBits(GPIO_GetPin(EPD_CS_PIN));
    spi_write_byte(cmd);
    GPIO_SetBits(GPIO_GetPin(EPD_CS_PIN));
}

/* Send one data byte (DC HIGH, inside CS pulse) */
void epd_data(uint8_t data)
{
    GPIO_SetBits(GPIO_GetPin(EPD_DC_PIN));
    GPIO_ResetBits(GPIO_GetPin(EPD_CS_PIN));
    spi_write_byte(data);
    GPIO_SetBits(GPIO_GetPin(EPD_CS_PIN));
}

/*
 * Send command + data bytes in one CS-asserted transaction.
 * First byte = command (DC LOW), remaining = data (DC HIGH).
 */
void epd_write(uint8_t cmd, int n_data, ...)
{
    /* command phase */
    GPIO_ResetBits(GPIO_GetPin(EPD_DC_PIN));
    GPIO_ResetBits(GPIO_GetPin(EPD_CS_PIN));
    spi_write_byte(cmd);

    /* data phase */
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

/* Stream a buffer as data bytes (DC HIGH, CS held for entire buffer). */
void epd_stream_data(const uint8_t *buf, uint32_t len)
{
    GPIO_SetBits(GPIO_GetPin(EPD_DC_PIN));
    GPIO_ResetBits(GPIO_GetPin(EPD_CS_PIN));
    for (uint32_t i = 0; i < len; i++)
        spi_write_byte(buf[i]);
    GPIO_SetBits(GPIO_GetPin(EPD_CS_PIN));
}

/*
 * Stream n bytes of constant value as data (DC HIGH) for image RAM writes.
 * CS stays asserted for the entire stream.
 */
void epd_stream_const(uint8_t value, uint32_t n)
{
    GPIO_SetBits(GPIO_GetPin(EPD_DC_PIN));
    GPIO_ResetBits(GPIO_GetPin(EPD_CS_PIN));
    for (uint32_t i = 0; i < n; i++)
        spi_write_byte(value);
    GPIO_SetBits(GPIO_GetPin(EPD_CS_PIN));
}

/* Wait until BUSY pin goes HIGH (Controller ready / not low anymore) */
void epd_wait_busy(void)
{
    uart_printf("epd_wait_busy\n");
    /* Laut Sniff: Warten bis Busy NICHT mehr LOW ist (also HIGH wird) */
    while (GPIO_ReadInputDataBit(GPIO_GetPin(EPD_BUSY_PIN)) == Bit_RESET)
        platform_delay_ms(1);
}

void epd_wait_busy_sleep(void)
{
    uart_printf("epd_wait_busy_sleep\n");
    /* Laut Sniff: Warten bis Busy NICHT mehr LOW ist (also HIGH wird) */
    while (GPIO_ReadInputDataBit(GPIO_GetPin(EPD_BUSY_PIN)) == Bit_RESET)
        os_delay(100);
}

/* Hardware reset: RST LOW for 10 ms, then HIGH, then 10 ms settle */
void epd_hw_reset(void)
{
    platform_delay_ms(30);
    GPIO_SetBits(GPIO_GetPin(EPD_RST_PIN));
    platform_delay_ms(22);
    GPIO_ResetBits(GPIO_GetPin(EPD_RST_PIN));
    platform_delay_ms(10);
    GPIO_SetBits(GPIO_GetPin(EPD_RST_PIN));
    platform_delay_ms(15);
}

void epd_init(void)
{
    GPIO_InitTypeDef g;
    GPIO_StructInit(&g);

    g.GPIO_Pin = GPIO_GetPin(EPD_PWR_PIN) | GPIO_GetPin(EPD_BS_PIN) |
                 GPIO_GetPin(EPD_CS_PIN) | GPIO_GetPin(EPD_RST_PIN) |
                 GPIO_GetPin(EPD_DC_PIN) | GPIO_GetPin(EPD_CLK_PIN) |
                 GPIO_GetPin(EPD_MOSI_PIN);
    g.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_Init(&g);

    g.GPIO_Pin = GPIO_GetPin(EPD_BUSY_PIN);
    g.GPIO_Mode = GPIO_Mode_IN;
    GPIO_Init(&g);

    /* Safe initial state */
    GPIO_ResetBits(GPIO_GetPin(EPD_PWR_PIN));
    GPIO_ResetBits(GPIO_GetPin(EPD_BS_PIN));
    GPIO_SetBits(GPIO_GetPin(EPD_CS_PIN));
    GPIO_SetBits(GPIO_GetPin(EPD_RST_PIN));
    GPIO_ResetBits(GPIO_GetPin(EPD_DC_PIN));
    GPIO_ResetBits(GPIO_GetPin(EPD_CLK_PIN));
    GPIO_ResetBits(GPIO_GetPin(EPD_MOSI_PIN));

    /* Power on the display */
    GPIO_SetBits(GPIO_GetPin(EPD_PWR_PIN));
    platform_delay_ms(20);

    /* Hardware Reset */
    epd_hw_reset();
    epd_wait_busy();

    epd_write(0x00, 2, 0b00000011, 0x29);                         /* PSR */
    epd_write(0x01, 6, 0x07, 0x00, 0x22, 0x78, 0x0A, 0x22); /* PWR */
    epd_write(0x03, 3, 0x10, 0x54, 0x44);                   /* PFS */
    epd_write(0xE7, 1, 0x1C);                               /* Flash Control */
    epd_write(0x06, 4, 0xC7, 0xD7, 0x1D, 0x1E);             /* BTST */
    epd_write(0x41, 1, 0x00);                               /* TSE */
    epd_write(0x50, 1, 0x37);                               /* VCOM and Data Interval */
    epd_write(0x60, 2, 0x02, 0x02);                         /* TCON */
    epd_write(0x61, 4, 0x00, 0xE0, 0x01, 0xE0);             /* Resolution 224x480 */
    epd_write(0x65, 4, 0x00, 0x08, 0x00, 0x00);             /* Flash Control */
    epd_write(0xE3, 1, 0x22);                               /* PWS */
    epd_write(0xE9, 1, 0x01);
    epd_write(0x30, 1, 0x08); /* PLL */
}

/* -------------------------------------------------------------------------
 * Write all-white frame + trigger refresh cycle
 * ------------------------------------------------------------------------- */
void epd_display_white(void)
{
    epd_cmd(0x10);
    epd_stream_const(0x55, NEW_EPD_BUF_SIZE);

    /* Power On */
    epd_cmd(0x04);
    epd_wait_busy();
    /* Display Refresh */
    epd_write(0x12, 1, 0x00);
    for (int i = 0; i < 300; i++)
    {
        platform_delay_ms(100);
        if (GPIO_ReadInputDataBit(GPIO_GetPin(EPD_BUSY_PIN)) == Bit_SET)
            break;
    }
    /* Power Off */
    epd_write(0x02, 1, 0x00);
    epd_wait_busy();
}

/* -------------------------------------------------------------------------
 * Deep sleep
 * ------------------------------------------------------------------------- */
void epd_sleep(void)
{
    epd_write(0x07, 1, 0xA5);
    platform_delay_ms(100);
    GPIO_ResetBits(GPIO_GetPin(EPD_PWR_PIN));
}

/**
 * @brief    Contains the initialization of pinmux settings and pad settings
 */
void epd_board_init(void)
{
    RCC_PeriphClockCmd(APBPeriph_GPIO, APBPeriph_GPIO_CLOCK, ENABLE);

    Pad_Config(EPD_PWR_PIN, PAD_PINMUX_MODE, PAD_IS_PWRON, PAD_PULL_NONE, PAD_OUT_ENABLE, PAD_OUT_LOW);
    platform_delay_ms(10);
    Pad_Config(EPD_BS_PIN, PAD_PINMUX_MODE, PAD_IS_PWRON, PAD_PULL_NONE, PAD_OUT_ENABLE, PAD_OUT_LOW);
    Pad_Config(EPD_CS_PIN, PAD_PINMUX_MODE, PAD_IS_PWRON, PAD_PULL_NONE, PAD_OUT_ENABLE, PAD_OUT_HIGH);
    Pad_Config(EPD_RST_PIN, PAD_PINMUX_MODE, PAD_IS_PWRON, PAD_PULL_NONE, PAD_OUT_ENABLE, PAD_OUT_HIGH);
    Pad_Config(EPD_DC_PIN, PAD_PINMUX_MODE, PAD_IS_PWRON, PAD_PULL_NONE, PAD_OUT_ENABLE, PAD_OUT_LOW);
    Pad_Config(EPD_CLK_PIN, PAD_PINMUX_MODE, PAD_IS_PWRON, PAD_PULL_NONE, PAD_OUT_ENABLE, PAD_OUT_LOW);
    Pad_Config(EPD_MOSI_PIN, PAD_PINMUX_MODE, PAD_IS_PWRON, PAD_PULL_NONE, PAD_OUT_ENABLE, PAD_OUT_LOW);
    Pad_Config(EPD_BUSY_PIN, PAD_PINMUX_MODE, PAD_IS_PWRON, PAD_PULL_NONE, PAD_OUT_DISABLE, PAD_OUT_LOW);

    Pinmux_Config(EPD_PWR_PIN, DWGPIO);
    Pinmux_Config(EPD_BS_PIN, DWGPIO);
    Pinmux_Config(EPD_CS_PIN, DWGPIO);
    Pinmux_Config(EPD_RST_PIN, DWGPIO);
    Pinmux_Config(EPD_DC_PIN, DWGPIO);
    Pinmux_Config(EPD_CLK_PIN, DWGPIO);
    Pinmux_Config(EPD_MOSI_PIN, DWGPIO);
    Pinmux_Config(EPD_BUSY_PIN, DWGPIO);
}

void epd_board_sleep(void)
{
    Pinmux_Config(EPD_PWR_PIN, IDLE_MODE);
    Pinmux_Config(EPD_BS_PIN, IDLE_MODE);
    Pinmux_Config(EPD_CS_PIN, IDLE_MODE);
    Pinmux_Config(EPD_RST_PIN, IDLE_MODE);
    Pinmux_Config(EPD_DC_PIN, IDLE_MODE);
    Pinmux_Config(EPD_CLK_PIN, IDLE_MODE);
    Pinmux_Config(EPD_MOSI_PIN, IDLE_MODE);
    Pinmux_Config(EPD_BUSY_PIN, IDLE_MODE);

    Pinmux_Deinit(EPD_PWR_PIN);
    Pinmux_Deinit(EPD_BS_PIN);
    Pinmux_Deinit(EPD_CS_PIN);
    Pinmux_Deinit(EPD_RST_PIN);
    Pinmux_Deinit(EPD_DC_PIN);
    Pinmux_Deinit(EPD_CLK_PIN);
    Pinmux_Deinit(EPD_MOSI_PIN);
    Pinmux_Deinit(EPD_BUSY_PIN);
}

void epd_draw_full(void)
{
    epd_board_init();
    uart_printf("EPD board init done\n");
    epd_init();
    uart_printf("EPD init done\n");

    epd_display_white();
    uart_printf("EPD white refresh done\n");

    epd_sleep();
    epd_board_sleep();
    uart_printf("EPD sleep\n");
}

void EPD_Display_start(uint8_t color)
{
    (void)color;
    epd_board_init();
    epd_init();
    epd_cmd(0x10);
}

void EPD_Display_byte(uint8_t data)
{
    epd_data(data);
}

void EPD_Display_color_change(uint8_t color)
{
    (void)color;
}
