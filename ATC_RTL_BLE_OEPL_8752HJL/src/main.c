#include <os_sched.h>
#include <platform_utils.h>
#include <string.h>
#include <stdio.h>
#include <trace.h>
#include "log_uart.h"
#include <gap.h>
#include <gap_adv.h>
#include <gap_bond_le.h>
#include <profile_server.h>
#include <gap_msg.h>
#include <simple_ble_service.h>
#include <bas.h>
#include <app_task.h>
#include <peripheral_app.h>
#include "custom_service.h"
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

/*============================================================================*
 *                              Constants
 *============================================================================*/
/** @brief  Default minimum advertising interval when device is discoverable (units of 625us, 160=100ms) */
#define DEFAULT_ADVERTISING_INTERVAL_MIN 3200
/** @brief  Default maximum advertising interval */
#define DEFAULT_ADVERTISING_INTERVAL_MAX 3200

/*============================================================================*
 *                              Functions
 *============================================================================*/
/**
 * @brief  Initialize peripheral and gap bond manager related parameters
 * @return void
 */
void app_le_gap_init(void)
{

    /* Device name and device appearance */
    uint16_t appearance = GAP_GATT_APPEARANCE_UNKNOWN;
    uint8_t slave_init_mtu_req = false;

    /* Advertising parameters */
    uint8_t adv_evt_type = GAP_ADTYPE_ADV_IND;
    uint8_t adv_direct_type = GAP_REMOTE_ADDR_LE_PUBLIC;
    uint8_t adv_direct_addr[GAP_BD_ADDR_LEN] = {0};
    uint8_t adv_chann_map = GAP_ADVCHAN_ALL;
    uint8_t adv_filter_policy = GAP_ADV_FILTER_ANY;
    uint16_t adv_int_min = DEFAULT_ADVERTISING_INTERVAL_MIN;
    uint16_t adv_int_max = DEFAULT_ADVERTISING_INTERVAL_MAX;

    /* GAP Bond Manager parameters */
    uint8_t auth_pair_mode = GAP_PAIRING_MODE_PAIRABLE;
    uint16_t auth_flags = GAP_AUTHEN_BIT_BONDING_FLAG;
    uint8_t auth_io_cap = GAP_IO_CAP_NO_INPUT_NO_OUTPUT;
    uint8_t auth_oob = false;
    uint8_t auth_use_fix_passkey = false;
    uint32_t auth_fix_passkey = 0;
    uint16_t auth_sec_req_flags = GAP_AUTHEN_BIT_BONDING_FLAG;

    /* Set device name and device appearance */
    le_set_gap_param(GAP_PARAM_DEVICE_NAME, OEPL_DEVICE_NAME_LEN, device_name);
    le_set_gap_param(GAP_PARAM_APPEARANCE, sizeof(appearance), &appearance);
    le_set_gap_param(GAP_PARAM_SLAVE_INIT_GATT_MTU_REQ, sizeof(slave_init_mtu_req), &slave_init_mtu_req);

    /* Set advertising parameters */
    le_adv_set_param(GAP_PARAM_ADV_EVENT_TYPE, sizeof(adv_evt_type), &adv_evt_type);
    le_adv_set_param(GAP_PARAM_ADV_DIRECT_ADDR_TYPE, sizeof(adv_direct_type), &adv_direct_type);
    le_adv_set_param(GAP_PARAM_ADV_DIRECT_ADDR, sizeof(adv_direct_addr), adv_direct_addr);
    le_adv_set_param(GAP_PARAM_ADV_CHANNEL_MAP, sizeof(adv_chann_map), &adv_chann_map);
    le_adv_set_param(GAP_PARAM_ADV_FILTER_POLICY, sizeof(adv_filter_policy), &adv_filter_policy);
    le_adv_set_param(GAP_PARAM_ADV_INTERVAL_MIN, sizeof(adv_int_min), &adv_int_min);
    le_adv_set_param(GAP_PARAM_ADV_INTERVAL_MAX, sizeof(adv_int_max), &adv_int_max);

    /* Set placeholder battery value here (pre-RTOS, calibration not yet loadable).
     * The stack-ready handler in peripheral_app.c refreshes with a real reading. */
    set_adv_data(0);

    /* Setup the GAP Bond Manager */
    gap_set_param(GAP_PARAM_BOND_PAIRING_MODE, sizeof(auth_pair_mode), &auth_pair_mode);
    gap_set_param(GAP_PARAM_BOND_AUTHEN_REQUIREMENTS_FLAGS, sizeof(auth_flags), &auth_flags);
    gap_set_param(GAP_PARAM_BOND_IO_CAPABILITIES, sizeof(auth_io_cap), &auth_io_cap);
    gap_set_param(GAP_PARAM_BOND_OOB_ENABLED, sizeof(auth_oob), &auth_oob);
    le_bond_set_param(GAP_PARAM_BOND_FIXED_PASSKEY, sizeof(auth_fix_passkey), &auth_fix_passkey);
    le_bond_set_param(GAP_PARAM_BOND_FIXED_PASSKEY_ENABLE, sizeof(auth_use_fix_passkey), &auth_use_fix_passkey);
    le_bond_set_param(GAP_PARAM_BOND_SEC_REQ_REQUIREMENT, sizeof(auth_sec_req_flags), &auth_sec_req_flags);

    /* register gap message callback */
    le_register_app_cb(app_gap_callback);
}

/**
 * @brief  Add GATT services and register callbacks
 * @return void
 */
void app_le_profile_init(void)
{
    server_init(1);
    custom_srv_id = custom_service_add_service(app_profile_callback);
    server_register_app_cb(app_profile_callback);
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
    uart_printf("driver_init: ADC ready\n");
}

/**
 * @brief    Contains the power mode settings
 * @return   void
 */
void io_dlps_enter_cb(void)
{
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

    /* ADC: power down the analog block before sleep (AON reg 0x113 bit2 = 1). */
    uint8_t adc_r = btaon_fast_read_safe(0x113);
    btaon_fast_write(0x113, adc_r | 0x04u);

    /* EPD: if a display refresh is in progress, latch all output pins into
     * PAD_SW_MODE.  During DLPS the GPIO APB clock is off — without SW mode
     * the pads float, causing CS/PWR/RST to fall to wrong levels and corrupt
     * the ongoing refresh.  SW mode latches the current GPIO output value
     * (CS=HIGH, PWR=HIGH, RST=HIGH, CLK/MOSI/DC/BS=LOW) and holds it
     * actively even while the GPIO peripheral is unpowered. */
    if (g_epd_refreshing)
    {
        Pad_ControlSelectValue(EPD_CS_PIN, PAD_SW_MODE);
        Pad_ControlSelectValue(EPD_PWR_PIN, PAD_SW_MODE);
        Pad_ControlSelectValue(EPD_RST_PIN, PAD_SW_MODE);
        Pad_ControlSelectValue(EPD_CLK_PIN, PAD_SW_MODE);
        Pad_ControlSelectValue(EPD_MOSI_PIN, PAD_SW_MODE);
        Pad_ControlSelectValue(EPD_DC_PIN, PAD_SW_MODE);
        Pad_ControlSelectValue(EPD_BS_PIN, PAD_SW_MODE);
    }
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

    /* ADC: re-enable the analog block immediately on wake. */
    uint8_t adc_r = btaon_fast_read_safe(0x113);
    btaon_fast_write(0x113, adc_r & ~0x04u);

    /* EPD: restore PINMUX mode so the GPIO peripheral drives the pins again.
     * USE_GPIO_DLPS=1 has already restored the GPIO output register values,
     * so the pin levels will be correct immediately after the switch. */
    if (g_epd_refreshing)
    {
        Pad_ControlSelectValue(EPD_CS_PIN, PAD_PINMUX_MODE);
        Pad_ControlSelectValue(EPD_PWR_PIN, PAD_PINMUX_MODE);
        Pad_ControlSelectValue(EPD_RST_PIN, PAD_PINMUX_MODE);
        Pad_ControlSelectValue(EPD_CLK_PIN, PAD_PINMUX_MODE);
        Pad_ControlSelectValue(EPD_MOSI_PIN, PAD_PINMUX_MODE);
        Pad_ControlSelectValue(EPD_DC_PIN, PAD_PINMUX_MODE);
        Pad_ControlSelectValue(EPD_BS_PIN, PAD_PINMUX_MODE);
    }
}

void pwr_mgr_init(void)
{
#if F_BT_DLPS_EN
    DLPS_IORegUserDlpsEnterCb(io_dlps_enter_cb);
    DLPS_IORegUserDlpsExitCb(io_dlps_exit_cb);
    DLPS_IORegister();
    lps_mode_set(PLATFORM_DLPS_PFM);
#endif
}

/**
 * @brief    Contains the initialization of all tasks
 * @note     There is only one task in BLE Peripheral APP, thus only one APP task is init here
 * @return   void
 */
void task_init(void)
{
    app_task_init();
}

/* TEMP: hexdump 1 MB starting at 0x00000000 -------------------------------- */
static void dump_memory_hex(void)
{
    const uint8_t *p = (const uint8_t *)0x00000000u;
    uart_printf("=== MEM DUMP 0x00000000 + 1MB ===\r\n");
    for (uint32_t i = 0; i < 0x100000u; i += 16)
    {
        uart_printf( //"%08lx: "
            "%02x %02x %02x %02x %02x %02x %02x %02x  "
            "%02x %02x %02x %02x %02x %02x %02x %02x\r\n",
            // i,
            p[i + 0], p[i + 1], p[i + 2], p[i + 3],
            p[i + 4], p[i + 5], p[i + 6], p[i + 7],
            p[i + 8], p[i + 9], p[i + 10], p[i + 11],
            p[i + 12], p[i + 13], p[i + 14], p[i + 15]);
    }
    uart_printf("=== END MEM DUMP ===\r\n");
}
/* END TEMP ----------------------------------------------------------------- */

/**
 * @brief    Entry of APP code
 * @return   int (To avoid compile warning)
 */
int main(void)
{
    board_init();
    uart_printf("BLE Peripheral booting...\n");
    le_gap_init(APP_MAX_LINKS);
    gap_lib_init();
    app_le_gap_init();
    app_le_profile_init();
    pwr_mgr_init();
    task_init();
    os_sched_start();

    return 0;
}

