#include "epd.h"
#include "board.h"
#include <rtl876x_gpio.h>
#include <rtl876x_rcc.h>
#include <rtl876x_pinmux.h>
#include <platform_utils.h> 
#include <os_sched.h>

void spi_write_byte(uint8_t byte)
{
    for (int i = 7; i >= 0; i--) {
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
    if (n_data > 0) {
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

/* Wait until BUSY pin goes LOW (controller is idle) */
void epd_wait_busy(void)
{
    uart_printf("epd_wait_busy\n");
    /* BUSY = HIGH means the controller is busy */
    while (GPIO_ReadInputDataBit(GPIO_GetPin(EPD_BUSY_PIN)) == Bit_SET)
        platform_delay_ms(1);
}

/* Hardware reset: RST LOW for 10 ms, then HIGH, then 10 ms settle */
void epd_hw_reset(void)
{
    platform_delay_ms(30);
    GPIO_SetBits  (GPIO_GetPin(EPD_RST_PIN));
    platform_delay_ms(22);
    GPIO_ResetBits(GPIO_GetPin(EPD_RST_PIN));
    platform_delay_ms(10);
    GPIO_SetBits  (GPIO_GetPin(EPD_RST_PIN));
    platform_delay_ms(15);
}

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

    /* Safe initial state */
    GPIO_ResetBits(GPIO_GetPin(EPD_PWR_PIN));
    GPIO_ResetBits(GPIO_GetPin(EPD_BS_PIN));
    GPIO_SetBits  (GPIO_GetPin(EPD_CS_PIN));
    GPIO_SetBits  (GPIO_GetPin(EPD_RST_PIN));
    GPIO_ResetBits(GPIO_GetPin(EPD_DC_PIN));
    GPIO_ResetBits(GPIO_GetPin(EPD_CLK_PIN));
    GPIO_ResetBits(GPIO_GetPin(EPD_MOSI_PIN));

    /* Power on the display */
    GPIO_SetBits(GPIO_GetPin(EPD_PWR_PIN));
    platform_delay_ms(20);

    /* --- Phase 1: pre-reset config (temperature + LUT load) -------------- */
    epd_write(0x18, 1, 0x80);       /* Temperature sensor: internal          */
    epd_write(0x22, 1, 0xB1);       /* Display Update Control 2: load temp+LUT */
    epd_cmd (0x20);                  /* Master Activation: execute sequence   */
    epd_wait_busy();                 /* ~5 ms                                 */
    platform_delay_ms(1);

    epd_write(0x1B, 1, 0x1A);       /* (proprietary config)                  */
    epd_write(0x10, 1, 0x01);       /* (proprietary config)                  */
    platform_delay_ms(130);
    epd_hw_reset();

    epd_cmd(0x12);                   /* Software Reset                        */
    epd_wait_busy();                 /* ~5 ms                                 */
    platform_delay_ms(1);

    /* --- Phase 3: address window + data entry mode ------------------------ */
    /*
     * Data Entry Mode 0x00: X-decrement, Y-decrement
     *   → RAM filled right-to-left, bottom-to-top
     */
    epd_write(0x11, 1, 0x02);

    epd_write(0x91, 1, 0x03);       /* Partial/Full mode control             */

    /* BW RAM address window: X = 49..0 (bytes), Y = 0..271 */
    epd_write(0x44, 2, 0x31, 0x00); /* RAM-X Start=49, End=0                 */
    epd_write(0x45, 4, 0x00, 0x00, 0x0F, 0x01); /* RAM-Y Start=0, End=271   */

    /* BW RAM counters: start at (49, 0) — top-right, Y increments downward  */
    epd_write(0x4E, 1, 0x31);
    epd_write(0x4F, 2, 0x00, 0x00);

    /* Red RAM address window: X = 0..49 (bytes), Y = 271..0 */
    epd_write(0xC4, 2, 0x00, 0x31); /* Red-RAM-X Start=0, End=49            */
    epd_write(0xC5, 4, 0x00, 0x00, 0x0F, 0x01);

    /* Red RAM counters */
    epd_write(0xCE, 1, 0x00);
    epd_write(0xCF, 2, 0x00, 0x00);

    epd_write(0x3C, 1, 0x01);       /* Border Waveform Control               */
}

/* -------------------------------------------------------------------------
 * Write all-white frame + trigger full refresh
 * ------------------------------------------------------------------------- */
void epd_display_white(void)
{
    /*
     * BW RAM  (0x24): 0xFF = white pixel
     * Red RAM (0x26): 0x00 = no red  (colour determined by BW RAM)
     */

    /* Write BW RAM */
    epd_cmd(0x24);
    epd_stream_const(0xFF, EPD_BUF_SIZE);   /* all white */

    /* Write Red RAM */
    epd_cmd(0x26);
    epd_stream_const(0x00, EPD_BUF_SIZE);   /* no red    */

    epd_cmd(0xA4);
    epd_stream_const(0xFF, EPD_BUF_SIZE);   /* all white */

    /* Write Red RAM */
    epd_cmd(0xA6);
    epd_stream_const(0x00, EPD_BUF_SIZE);   /* no red    */

    /* Trigger full refresh and wait ~22 s — yield to RTOS while waiting */
    epd_cmd(0x20);
    /* Poll busy in 100 ms steps so BLE events keep processing */
    for (int i = 0; i < 300; i++) {        /* up to 30 s timeout            */
        //os_delay(100);
        platform_delay_ms(100);
        if (GPIO_ReadInputDataBit(GPIO_GetPin(EPD_BUSY_PIN)) == Bit_RESET)
            break;
    }
}

/* -------------------------------------------------------------------------
 * Deep sleep
 * ------------------------------------------------------------------------- */
void epd_sleep(void)
{
    epd_write(0x10, 1, 0x01);       /* Deep Sleep Mode 1 (~1 µA)             */
    platform_delay_ms(300);          /* allow controller to settle            */
    GPIO_ResetBits(GPIO_GetPin(EPD_PWR_PIN)); /* cut display power rail       */
    platform_delay_ms(300);          /* allow controller to settle            */
}

/**
 * @brief    Contains the initialization of pinmux settings and pad settings
 * @note     All the pinmux settings and pad settings shall be initiated in this function,
 *           but if legacy driver is used, the initialization of pinmux setting and pad setting
 *           should be peformed with the IO initializing.
 * @return   void
 */
/* --------------------------------------------------------------------------
 * epd_board_init — pad + GPIO setup for the EPD (e-paper) software-SPI bus.
 *
 * Pin initial states:
 *   EPD_PWR  LOW  — display power rail off until needed
 *   EPD_BS   LOW  — SPI mode select, must stay LOW permanently
 *   EPD_CS   HIGH — deselected
 *   EPD_RST  HIGH — not in reset
 *   EPD_DC   LOW  — command mode default
 *   EPD_CLK  LOW  — idle clock
 *   EPD_MOSI LOW  — idle data
 *   EPD_BUSY INPUT — driven by display
 * -------------------------------------------------------------------------- */
void epd_board_init(void)
{
    /* Pad and pinmux registers are in the AON domain — they survive the
     * BLE stack init that resets APB peripheral clocks.
     * GPIO_Init and RCC_PeriphClockCmd(GPIO) are done inside epd_init()
     * which runs after the BLE stack is fully up. */
    RCC_PeriphClockCmd(APBPeriph_GPIO, APBPeriph_GPIO_CLOCK, ENABLE);

    Pad_Config(EPD_PWR_PIN,  PAD_PINMUX_MODE, PAD_IS_PWRON, PAD_PULL_NONE, PAD_OUT_ENABLE,  PAD_OUT_LOW);
    platform_delay_ms(10);
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
    Pinmux_Config(EPD_PWR_PIN,  IDLE_MODE);
    Pinmux_Config(EPD_BS_PIN,   IDLE_MODE);
    Pinmux_Config(EPD_CS_PIN,   IDLE_MODE);
    Pinmux_Config(EPD_RST_PIN,  IDLE_MODE);
    Pinmux_Config(EPD_DC_PIN,   IDLE_MODE);
    Pinmux_Config(EPD_CLK_PIN,  IDLE_MODE);
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

/* Compatibility functions for drawing.c */
void EPD_Display_start(uint8_t color)
{
    (void)color;
    epd_board_init();
    epd_init();
    epd_cmd(0x24); // BW RAM
}

void EPD_Display_byte(uint8_t data)
{
    epd_data(data);
}

void EPD_Display_color_change(uint8_t color)
{
    (void)color;
    epd_cmd(0x26); // Red RAM
}

void EPD_Display_end(void)
{
    epd_cmd(0x20); // Master Activation
    epd_wait_busy();
    epd_sleep();
    epd_board_sleep();
}

