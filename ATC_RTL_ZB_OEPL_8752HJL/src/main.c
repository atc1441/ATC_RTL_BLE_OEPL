#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <os_sched.h>
#include <platform_utils.h>
#include <trace.h>
#include "log_uart.h"
#include <gap.h>
#include <gap_adv.h>
#include <gap_bond_le.h>
#include <profile_server.h>
#include <gap_msg.h>
#include <simple_ble_service.h>
#include <bas.h>
#if F_BT_DLPS_EN
#include <dlps.h>
#include <rtl876x_io_dlps.h>
#endif
#include <rtl876x_pinmux.h>
#include <rtl876x_rcc.h>
#include <rtl876x_gpio.h>
#include "board.h"
#include "retarget.h"
#include "epd.h"
#include "eeprom.h"
#include "battery.h"
#include "drawing.h"
#include <rtl876x_aon_wdg.h>
#include "zigbee.h"
#include "syncedproto.h"
#include "powermgt.h"
#include "boot_screen.h"
#include <os_task.h>
#include <os_sched.h>

/**
 * @brief    Contains the power mode settings
 * @return   void
 */
volatile uint32_t g_dlps_enter_count = 0;

void io_dlps_enter_cb(void)
{
    g_dlps_enter_count++;
    /* UART: switch pads to SW mode to avoid current leakage through the
     * UART pull-ups when PINMUX drives them during sleep. */
    Pad_ControlSelectValue(UART_TX_PIN, PAD_SW_MODE);
    Pad_ControlSelectValue(UART_RX_PIN, PAD_SW_MODE);

    /* SPI flash: switch data/clock pins to SW mode.  CS is already HIGH
     * (flash deasserted) from the last SPI transaction; SW mode latches
     * that value so it stays HIGH throughout sleep without the GPIO block
     * having to remain active. */
    Pad_ControlSelectValue(EXT_FLASH_CS_PIN, PAD_SW_MODE);
    Pad_ControlSelectValue(EXT_FLASH_CLK_PIN, PAD_SW_MODE);
    Pad_ControlSelectValue(EXT_FLASH_MOSI_PIN, PAD_SW_MODE);
    Pad_ControlSelectValue(EXT_FLASH_MISO_PIN, PAD_SW_MODE);

    Pad_ControlSelectValue(EPD_CS_PIN, PAD_SW_MODE);
    Pad_ControlSelectValue(EPD_PWR_PIN, PAD_SW_MODE);
    Pad_ControlSelectValue(EPD_RST_PIN, PAD_SW_MODE);
    Pad_ControlSelectValue(EPD_CLK_PIN, PAD_SW_MODE);
    Pad_ControlSelectValue(EPD_MOSI_PIN, PAD_SW_MODE);
    Pad_ControlSelectValue(EPD_DC_PIN, PAD_SW_MODE);
    Pad_ControlSelectValue(EPD_BS_PIN, PAD_SW_MODE);
}

void io_dlps_exit_cb(void)
{
    /* Feed the AON watchdog on every wake-up. */
    AON_WDG_Restart();

    /* UART: restore PINMUX mode and APB clock. */
    Pad_ControlSelectValue(UART_TX_PIN, PAD_PINMUX_MODE);
    Pad_ControlSelectValue(UART_RX_PIN, PAD_PINMUX_MODE);
    RCC_PeriphClockCmd(APBPeriph_UART0, APBPeriph_UART0_CLOCK, ENABLE);

    /* SPI flash: re-init GPIO pads after every wake-up. */
    SPI_Flash_enable_GPIO();

    Pad_ControlSelectValue(EPD_CS_PIN, PAD_PINMUX_MODE);
    Pad_ControlSelectValue(EPD_PWR_PIN, PAD_PINMUX_MODE);
    Pad_ControlSelectValue(EPD_RST_PIN, PAD_PINMUX_MODE);
    Pad_ControlSelectValue(EPD_CLK_PIN, PAD_PINMUX_MODE);
    Pad_ControlSelectValue(EPD_MOSI_PIN, PAD_PINMUX_MODE);
    Pad_ControlSelectValue(EPD_DC_PIN, PAD_PINMUX_MODE);
    Pad_ControlSelectValue(EPD_BS_PIN, PAD_PINMUX_MODE);
}
void pwr_mgr_init(void)
{
#if F_BT_DLPS_EN
    DLPS_IORegUserDlpsEnterCb(io_dlps_enter_cb);
    DLPS_IORegUserDlpsExitCb(io_dlps_exit_cb);
    DLPS_IORegister();
    lps_mode_set(PLATFORM_DLPS_PFM);
    /* Enable platform PM debug — prints blocking reason each time DLPS check fails */
    platform_pm_feature_cfg.platform_check_dbg1 = 1;
#endif
}
/* --------------------------------------------------------------------------
 * nfc_board_init — pad + GPIO setup for the NFC IC (software I2C).
 *
 * SDA and SCL are open-drain.  External pull-ups on the PCB are required.
 * Both lines are configured as inputs here (released / HIGH via pull-up).
 * The software I2C driver will toggle direction to drive LOW.
 *
 * NFC_FIELD_PIN (P5_2, pad 38) is in the AON GPIO domain and requires a
 * separate AON GPIO API — not configured here yet.
 * -------------------------------------------------------------------------- */
void nfc_board_init(void)
{
    /* --- Pad configuration ------------------------------------------------ */
    Pad_Config(NFC_PWR_PIN, PAD_PINMUX_MODE, PAD_IS_PWRON, PAD_PULL_NONE, PAD_OUT_ENABLE, PAD_OUT_LOW);
    Pad_Config(NFC_SDA_PIN, PAD_PINMUX_MODE, PAD_IS_PWRON, PAD_PULL_NONE, PAD_OUT_DISABLE, PAD_OUT_LOW);
    Pad_Config(NFC_SCL_PIN, PAD_PINMUX_MODE, PAD_IS_PWRON, PAD_PULL_NONE, PAD_OUT_DISABLE, PAD_OUT_LOW);

    /* --- Pinmux to GPIO --------------------------------------------------- */
    Pinmux_Config(NFC_PWR_PIN, DWGPIO);
    Pinmux_Config(NFC_SDA_PIN, DWGPIO);
    Pinmux_Config(NFC_SCL_PIN, DWGPIO);

    /* --- GPIO direction --------------------------------------------------- */
    GPIO_InitTypeDef g;
    GPIO_StructInit(&g);

    /* NFC_PWR output */
    g.GPIO_Pin = GPIO_GetPin(NFC_PWR_PIN);
    g.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_Init(&g);
    GPIO_ResetBits(GPIO_GetPin(NFC_PWR_PIN)); /* power off */

    /* SDA + SCL as inputs (open-drain idle state) */
    g.GPIO_Pin = GPIO_GetPin(NFC_SDA_PIN) | GPIO_GetPin(NFC_SCL_PIN);
    g.GPIO_Mode = GPIO_Mode_IN;
    GPIO_Init(&g);

    /* NFC_FIELD_PIN (P5_2 / AON GPIO) — TODO: configure via AON GPIO API */
    Pinmux_Config(NFC_PWR_PIN, IDLE_MODE);
    Pinmux_Config(NFC_SDA_PIN, IDLE_MODE);
    Pinmux_Config(NFC_SCL_PIN, IDLE_MODE);

    Pinmux_Deinit(NFC_PWR_PIN);
    Pinmux_Deinit(NFC_SDA_PIN);
    Pinmux_Deinit(NFC_SCL_PIN);
}

void board_init(void)
{
    // nfc_board_init();
    uart_print_init();
}

/**
 * @brief    Contains the initialization of peripherals
 * @note     Both new architecture driver and legacy driver initialization method can be used
 * @return   void
 */
void driver_init(void)
{
    uart_printf("driver_init: start\n");
    /* AON WDT: 30 s timeout, stop counting during DLPS, reload on wake. */
    AON_WDG_Config(1, 30000u, 1, 1);
    AON_WDG_Enable();
    uart_printf("driver_init: WDT enabled (30s)\n");
    Pad_Config(LED_R, PAD_PINMUX_MODE, PAD_IS_PWRON, PAD_PULL_UP, PAD_OUT_DISABLE, PAD_OUT_LOW);
    Pad_Config(LED_G, PAD_PINMUX_MODE, PAD_IS_PWRON, PAD_PULL_UP, PAD_OUT_DISABLE, PAD_OUT_LOW);
    Pad_Config(LED_B, PAD_PINMUX_MODE, PAD_IS_PWRON, PAD_PULL_UP, PAD_OUT_DISABLE, PAD_OUT_LOW);
    Pad_ControlSelectValue(LED_R, PAD_SW_MODE);
    Pad_ControlSelectValue(LED_G, PAD_SW_MODE);
    Pad_ControlSelectValue(LED_B, PAD_SW_MODE);
    FLASH_Init();
    uart_printf("driver_init: flash ready\n");
#if FLASH_SELFTEST_EN
    if (!flash_selftest())
    {
        uart_printf("*** FLASH SELF-TEST FAILED — check SPI wiring ***\n");
    }
#endif
}

#define PROTO_TASK_STACK (512 * 24) /* 12 KB */
#define PROTO_TASK_PRIO 1

uint32_t scanAttempts = 0;

static void proto_task(void *arg)
{
    uint8_t prev_channel = 0;
    uint8_t screen_stale = 0;
    (void)arg;
    driver_init();
    pwr_mgr_init();
    eepromPowerUp();

    eepromReadUID(mSelfMac);
    printf("Self MAC: %02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X\r\n",
           mSelfMac[7], mSelfMac[6], mSelfMac[5], mSelfMac[4],
           mSelfMac[3], mSelfMac[2], mSelfMac[1], mSelfMac[0]);

    /* Init radio — must be called after mSelfMac is set */
    radioInit();

    batteryVoltage = battery_measure_mv();
    printf("Battery: %u mV\r\n", batteryVoltage);

    initializeProto();

    /* AP scan on all channels */
    currentChannel = 0;
    for (uint8_t i = 0; i < sizeof(channelList) && !currentChannel; i++)
    {
        uint8_t r = detectAP(channelList[i]);
        if (r)
            currentChannel = channelList[i];
    }

    if (currentChannel)
    {
        printf("AP found on ch%u\r\n", currentChannel);
        struct AvailDataInfo *availtemp = getAvailDataInfo(); // Already request data to triger potential update generation
        if (availtemp)
        {
            wakeUpReason = WAKEUP_REASON_TIMED;
        }
        status_screen_show(mSelfMac, batteryVoltage, currentChannel, "AP found");
    }
    else
    {
        printf("No AP found\r\n");
        status_screen_show(mSelfMac, batteryVoltage, 0, "No AP found");
    }

    prev_channel = currentChannel;

    while (1)
    {
        wdt10s();
        batteryVoltage = battery_measure_mv();

        if (currentChannel)
        {
            struct AvailDataInfo *avail = getAvailDataInfo();

            if (!avail)
            {
                printf("No data from AP\r\n");
                scanAttempts++;
            }
            else
            {
                scanAttempts = 0;
                wakeUpReason = WAKEUP_REASON_TIMED;
                if (avail->dataType != DATATYPE_NOUPDATE)
                {
                    printf("Data transfer starting\r\n");
                    if (!processAvailDataInfo(avail))
                    {
                    }
                }
                else
                {
                }
            }

            if (scanAttempts >= 6)
            {
                currentChannel = 0;
                printf("AP lost - rescanning\r\n");
                if (curImgSlot != 0xFF)
                {
                    drawOnOffline(0);
                    drawImageFromEeprom(curImgSlot);
                    drawOnOffline(1);
                }
                else
                {
                    status_screen_show(mSelfMac, batteryVoltage, 0, "AP lost - rescanning");
                }
                screen_stale = false;
            }

            doSleep(SLEEP_DELAY_ONLINE * 1000UL);
        }
        else
        {
            /* Show scanning screen once when we transition to no-AP state */
            if (!screen_stale || prev_channel != currentChannel)
            {
                prev_channel = currentChannel;
            }

            for (uint8_t i = 0; i < sizeof(channelList); i++)
            {
                uint8_t r = detectAP(channelList[i]);
                if (r)
                {
                    currentChannel = channelList[i];
                    break;
                }
            }
            if (currentChannel)
            {
                printf("AP re-found on ch%u\r\n", currentChannel);
                wakeUpReason = WAKEUP_REASON_NETWORK_SCAN;
                if (curImgSlot != 0xFF)
                {
                    drawOnOffline(1);
                    drawImageFromEeprom(curImgSlot);
                }
                else
                {
                    status_screen_show(mSelfMac, batteryVoltage, currentChannel, "AP found");
                }
                screen_stale = false;
                prev_channel = currentChannel;
                doSleep(SLEEP_DELAY_ONLINE * 1000UL);
            }
            else
            {
                doSleep(SLEEP_DELAY_OFFLINE * 60 * 1000UL);
            }
        }
    }
}

int main(void)
{
    board_init();
    static void *proto_task_handle;
    os_task_create(&proto_task_handle, "proto", proto_task, NULL, PROTO_TASK_STACK, PROTO_TASK_PRIO);

    os_sched_start();
    return 0;
}
