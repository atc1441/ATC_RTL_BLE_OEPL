#include <trace.h>
#include "log_uart.h"
#include <string.h>
#include <gap.h>
#include <gap_adv.h>
#include <gap_le.h>
#include <gap_bond_le.h>
#include <profile_server.h>
#include <gap_msg.h>
#include <simple_ble_service.h>
#include <bas.h>
#include <app_msg.h>
#include <peripheral_app.h>
#include <gap_conn_le.h>
#include <app_section.h>
#include <stdio.h>
#include <ftl.h>
#include <os_timer.h>
#include <app_task.h>
#include <rtl876x_gpio.h>
#include "board.h"
#include "battery.h"
#include "drawing.h"
#include "boot_screen.h"
#include "custom_service.h"
#include "ble_cmd_handler.h"
#include "ota.h"
#include "mac_edit.h"
#include <platform_utils.h>

// Forward declarations
void ble_notify_clear_busy(void);

/* ---- Async EPD refresh --------------------------------------------------- */
/* 200 ms timer that polls BUSY pin after drawImageAtAddress() triggers refresh. */
static void *epd_poll_timer = NULL;
static uint32_t epd_timer_cb_count = 0;
static uint32_t epd_poll_msg_count = 0;

static void epd_poll_timer_cb(void *arg)
{
    (void)arg;
    epd_timer_cb_count++;

    /* 200 ms timer -> print every 25 callbacks = about every 5 seconds. */
    if ((epd_timer_cb_count % 25u) == 0u)
    {
        printf("EPD TIMER cb #%lu\n", (unsigned long)epd_timer_cb_count);
    }

    T_IO_MSG msg;
    msg.type = IO_MSG_TYPE_EPD_POLL;
    msg.subtype = 0;
    msg.u.param = 0;

    if (!app_send_msg_to_apptask(&msg))
    {
        printf("EPD TIMER: app_send failed at cb #%lu\n",
               (unsigned long)epd_timer_cb_count);
    }
}
/* -------------------------------------------------------------------------- */

/* The 6-byte Static Random Address we actually advertise with.*/
static uint8_t g_ble_rand_addr[6];

void ble_addr_early_init(void)
{
    gap_get_param(GAP_PARAM_BD_ADDR, &g_ble_rand_addr);
    printf("Our MAC: %02X:%02X:%02X:%02X:%02X:%02X\n",
           g_ble_rand_addr[5], g_ble_rand_addr[4], g_ble_rand_addr[3],
           g_ble_rand_addr[2], g_ble_rand_addr[1], g_ble_rand_addr[0]);

    // Check if MAC is the stock default: 25:63:87:4C:E0:00
    // In g_ble_rand_addr: [0]=0x00, [1]=0xE0, [2]=0x4C, [3]=0x87, [4]=0x63, [5]=0x25
    if (g_ble_rand_addr[0] == 0x00 && g_ble_rand_addr[1] == 0xE0 &&
        g_ble_rand_addr[2] == 0x4C && g_ble_rand_addr[3] == 0x87 &&
        g_ble_rand_addr[4] == 0x63 && g_ble_rand_addr[5] == 0x25) {

        printf("MAC EDIT: Default stock MAC detected. Generating random MAC...\n");

        uint8_t new_mac[6];
        uint32_t r1 = platform_random(0xFFFFFF);
        uint32_t r2 = platform_random(0xFFFFFF);

        new_mac[0] = (uint8_t)(r1 & 0xFF);
        new_mac[1] = (uint8_t)((r1 >> 8) & 0xFF);
        new_mac[2] = (uint8_t)((r1 >> 16) & 0xFF);
        new_mac[3] = (uint8_t)(r2 & 0xFF);
        new_mac[4] = (uint8_t)((r2 >> 8) & 0xFF);
        new_mac[5] = (uint8_t)((r2 >> 16) & 0xFF);

        // Let's ensure it's not the same default again by some miracle
        if (new_mac[0] == 0x00 && new_mac[1] == 0xE0) new_mac[0] = 0x01;

        mac_edit_update(new_mac);
        // mac_edit_update reboots, so we never get here
    }
}

T_SERVER_ID custom_srv_id;

T_GAP_DEV_STATE gap_dev_state = {0, 0, 0, 0};                  /**< GAP device state */
T_GAP_CONN_STATE gap_conn_state = GAP_CONN_STATE_DISCONNECTED; /**< GAP connection state */

struct BleAdvDataStruct ble_adv_data;
settings_struct settings = {
    .oepl_hw_type = 0x2E, /* OEPL SOLUM_M3_BWR_97: 9.7" BWR */
    .screen_available = 1,
    .screen_type = 1,
    .screen_functions = 0,
    .screen_w_h_inversed_ble = 0,
    .screen_w_h_inversed = 0,
    .screen_h = 672,
    .screen_w = 960,
    .screen_h_offset = 0,
    .screen_w_offset = 0,
    .screen_colors = 2,
    .screen_color_black_invert = 1,
    .screen_color_second_invert = 0,

};
uint8_t adc_temperature = 21;
uint8_t device_name[OEPL_DEVICE_NAME_LEN] = "RTL_000000";

/*============================================================================*
 *                              Functions
 *============================================================================*/

#define CAPABILITY_HAS_LED 0x01
#define CAPABILITY_SUPPORTS_COMPRESSION 0x02
#define CAPABILITY_SUPPORTS_CUSTOM_LUTS 0x04
#define CAPABILITY_ALT_LUT_SIZE 0x08
#define CAPABILITY_HAS_EXT_POWER 0x10
#define CAPABILITY_HAS_WAKE_BUTTON 0x20
#define CAPABILITY_HAS_NFC 0x40
#define CAPABILITY_NFC_WAKE 0x80
#define CAPABILITY_IS_BLE 0x0100

uint16_t get_capabilities(void)
{
    uint16_t temp_capa = CAPABILITY_IS_BLE | CAPABILITY_SUPPORTS_COMPRESSION;
    if (settings.led_pinout != NULL)
        temp_capa |= CAPABILITY_HAS_LED;
    if (settings.nfc_pinout != NULL)
        temp_capa |= CAPABILITY_HAS_NFC | CAPABILITY_NFC_WAKE;
    return temp_capa;
}

bool app_send_custom_notification(uint8_t conn_id, uint8_t *p_data, uint16_t len)
{
    return custom_service_send_notification(conn_id, custom_srv_id, p_data, len);
}

void set_adv_data(uint16_t battery_mv)
{
    ble_adv_data.len_cap = 2;
    ble_adv_data.type_cap = 1;
    ble_adv_data.capabilities_cap = 5;
    /* 1. Prepare OEPL Manufacturer Specific Data Struct */
    ble_adv_data.len = sizeof(struct BleAdvDataStruct) - 4;
    ble_adv_data.type = 0xFF;
    ble_adv_data.manu_id = 0x1337;
    ble_adv_data.version = 2;
    ble_adv_data.hw_type = settings.oepl_hw_type;
    ble_adv_data.fw_version = FIRMWARE_VERSION;
    ble_adv_data.capabilities = get_capabilities();
    ble_adv_data.battery_mv = battery_mv;
    ble_adv_data.temperature = adc_temperature;
    ble_adv_data.counter++;

    /* 2. Set Advertising Data: Flags + Local Name Complete
     * Use g_ble_rand_addr (our persistent random address) for the device name. */
    sprintf((char *)device_name, "RTL_%02X%02X%02X",
            g_ble_rand_addr[2], g_ble_rand_addr[1], g_ble_rand_addr[0]);

    uint8_t adv_data_buf[31];
    uint8_t adv_len = 0;

    /* Flags */
    /*adv_data_buf[adv_len++] = 0x02;
    adv_data_buf[adv_len++] = 0x01; // GAP_ADTYPE_FLAGS
    adv_data_buf[adv_len++] = 0x05; // General Discoverable + BR/EDR Not Supported*/

    /* Local Name Complete */
    adv_data_buf[adv_len++] = 12;
    adv_data_buf[adv_len++] = 0x09; // GAP_ADTYPE_LOCAL_NAME_COMPLETE
    memcpy(&adv_data_buf[adv_len], device_name, 10);
    adv_data_buf[adv_data_buf[0]] = 0;
    adv_len += 11;

    T_GAP_CAUSE adv_data_cause = le_adv_set_param(
        GAP_PARAM_ADV_DATA,
        sizeof(struct BleAdvDataStruct),
        (void *)&ble_adv_data);

    printf("BLE: set ADV data len=%u -> 0x%04X\n",
           (unsigned)sizeof(struct BleAdvDataStruct),
           (unsigned)adv_data_cause);

    /* 3. Set Scan Response Data: local name */
    T_GAP_CAUSE scan_rsp_cause = le_adv_set_param(
        GAP_PARAM_SCAN_RSP_DATA,
        adv_len,
        adv_data_buf);

    printf("BLE: set SCAN_RSP len=%u -> 0x%04X\n",
           (unsigned)adv_len,
           (unsigned)scan_rsp_cause);

    /* 4. Update if advertising */
    if (gap_dev_state.gap_adv_state == GAP_ADV_STATE_ADVERTISING)
    {
        T_GAP_CAUSE update_cause = le_adv_update_param();
        printf("BLE: le_adv_update_param() -> 0x%04X\n",
               (unsigned)update_cause);
    }
}

/**
 * @brief    Update advertising data at runtime
 * @param[in] p_data  Pointer to the new advertising data
 * @param[in] len     Length of the new advertising data
 * @return   Indicates the function call is successful or not
 */
bool app_update_adv_data(uint8_t *p_data, uint8_t len)
{
    if (len > GAP_MAX_ADV_LEN)
    {
        printf("app_update_adv_data: len %d too long\n", len);
        return false;
    }

    T_GAP_CAUSE cause = le_adv_set_param(GAP_PARAM_ADV_DATA, len, p_data);
    if (cause == GAP_CAUSE_SUCCESS)
    {
        /* Only call update if we are already advertising.
           If not advertising, the new data will be used when le_adv_start() is called. */
        if (gap_dev_state.gap_adv_state == GAP_ADV_STATE_ADVERTISING)
        {
            cause = le_adv_update_param();
            if (cause == GAP_CAUSE_SUCCESS)
            {
                return true;
            }
            else
            {
                printf("le_adv_update_param failed: 0x%x\n", cause);
            }
        }
        else
        {
            return true; /* Success, data set for next start */
        }
    }
    else
    {
        printf("le_adv_set_param failed: 0x%x\n", cause);
    }
    return false;
}

void app_handle_gap_msg(T_IO_MSG *p_gap_msg);
/**
 * @brief    All the application messages are pre-handled in this function
 * @note     All the IO MSGs are sent to this function, then the event handling
 *           function shall be called according to the MSG type.
 * @param[in] io_msg  IO message data
 * @return   void
 */
void app_handle_io_msg(T_IO_MSG io_msg)
{
    uint16_t msg_type = io_msg.type;

    switch (msg_type)
    {
    case IO_MSG_TYPE_BT_STATUS:
        app_handle_gap_msg(&io_msg);
        break;

    case IO_MSG_TYPE_DRAW:
        /* BLE ACK was already sent before this message was posted.
         * Now do the data transfer + trigger refresh (non-blocking). */
        drawImageAtAddress(io_msg.u.param, (uint8_t)io_msg.subtype);
        epd_timer_cb_count = 0;
        epd_poll_msg_count = 0;
        os_timer_start(&epd_poll_timer);
        break;

    case IO_MSG_TYPE_EPD_POLL:
    {
        epd_poll_msg_count++;

        /* Print the app-task side every ~5 seconds.  This tells us whether
         * the timer callback message really arrives at the app task, and
         * what BUSY is doing at that instant. */
        if ((epd_poll_msg_count % 25u) == 0u)
        {
            uint8_t busy =
                (GPIO_ReadInputDataBit(GPIO_GetPin(EPD_BUSY_PIN)) == Bit_SET) ? 1u : 0u;

            printf("EPD POLL #%lu busy=%u refreshing=%u\n",
                   (unsigned long)epd_poll_msg_count,
                   (unsigned)busy,
                   g_epd_refreshing ? 1u : 0u);
        }

        if (g_epd_refreshing && epd_refresh_done())
        {
            printf("EPD POLL complete at #%lu\n",
                   (unsigned long)epd_poll_msg_count);
            os_timer_stop(&epd_poll_timer);
        }
        break;
    }

    case IO_MSG_TYPE_OTA_APPLY:
        /* Stop advertising and EPD poll timer before overwriting flash. */
        os_timer_stop(&epd_poll_timer);
        le_adv_stop();
        printf("OTA: starting apply fw_size=%lu\n",
               (unsigned long)io_msg.u.param);
        ota_apply(io_msg.u.param); /* does not return on success */
        /* Error path: ota_apply returned after showing error screen. */
        os_timer_start(&epd_poll_timer);
        break;

    default:
        break;
    }
}

/**
 * @brief    Handle msg GAP_MSG_LE_DEV_STATE_CHANGE
 * @note     All the gap device state events are pre-handled in this function.
 *           Then the event handling function shall be called according to the new_state
 * @param[in] new_state  New gap device state
 * @param[in] cause GAP device state change cause
 * @return   void
 */
void app_handle_dev_state_evt(T_GAP_DEV_STATE new_state, uint16_t cause)
{
    APP_PRINT_INFO3("app_handle_dev_state_evt: init state %d, adv state %d, cause 0x%x",
                    new_state.gap_init_state, new_state.gap_adv_state, cause);
    if (gap_dev_state.gap_init_state != new_state.gap_init_state)
    {
        if (new_state.gap_init_state == GAP_INIT_STATE_STACK_READY)
        {
            APP_PRINT_INFO0("GAP stack ready");
            /* Create the EPD busy-poll timer (200 ms, repeating). */
            os_timer_create(&epd_poll_timer, "epd_poll", 0, 200, true, epd_poll_timer_cb);
            /* Measure battery once here so we can pass it to both the boot screen
             * and the first advertising packet without measuring twice. */
            uint16_t vbat = battery_measure_mv();
            /* Show the boot screen and start polling for refresh completion. */
            boot_screen_draw(g_ble_rand_addr, vbat);
            epd_timer_cb_count = 0;
            epd_poll_msg_count = 0;
            os_timer_start(&epd_poll_timer);

            set_adv_data(vbat);
            T_GAP_CAUSE adv_start_cause = le_adv_start();
            printf("BLE: le_adv_start() -> 0x%04X\n",
                   (unsigned)adv_start_cause);
        }
    }

    if (gap_dev_state.gap_adv_state != new_state.gap_adv_state)
    {
        if (new_state.gap_adv_state == GAP_ADV_STATE_IDLE)
        {
            if (new_state.gap_adv_sub_state == GAP_ADV_TO_IDLE_CAUSE_CONN)
            {
                APP_PRINT_INFO0("GAP adv stoped: because connection created");
            }
            else
            {
                APP_PRINT_INFO0("GAP adv stoped");
            }
        }
        else if (new_state.gap_adv_state == GAP_ADV_STATE_ADVERTISING)
        {
            APP_PRINT_INFO0("GAP adv start");
        }
    }

    gap_dev_state = new_state;
}

/**
 * @brief    Handle msg GAP_MSG_LE_CONN_STATE_CHANGE
 * @note     All the gap conn state events are pre-handled in this function.
 *           Then the event handling function shall be called according to the new_state
 * @param[in] conn_id Connection ID
 * @param[in] new_state  New gap connection state
 * @param[in] disc_cause Use this cause when new_state is GAP_CONN_STATE_DISCONNECTED
 * @return   void
 */
void app_handle_conn_state_evt(uint8_t conn_id, T_GAP_CONN_STATE new_state, uint16_t disc_cause)
{
    APP_PRINT_INFO4("app_handle_conn_state_evt: conn_id %d old_state %d new_state %d, disc_cause 0x%x",
                    conn_id, gap_conn_state, new_state, disc_cause);
    switch (new_state)
    {
    case GAP_CONN_STATE_DISCONNECTED:
    {
        if ((disc_cause != (HCI_ERR | HCI_ERR_REMOTE_USER_TERMINATE)) && (disc_cause != (HCI_ERR | HCI_ERR_LOCAL_HOST_TERMINATE)))
        {
            APP_PRINT_ERROR1("app_handle_conn_state_evt: connection lost cause 0x%x", disc_cause);
        }

        T_GAP_CAUSE adv_restart_cause = le_adv_start();
        printf("BLE: reconnect le_adv_start() -> 0x%04X\n",
               (unsigned)adv_restart_cause);
    }
    break;

    case GAP_CONN_STATE_CONNECTED:
    {
        uint16_t conn_interval;
        uint16_t conn_latency;
        uint16_t conn_supervision_timeout;
        uint8_t remote_bd[6];
        T_GAP_REMOTE_ADDR_TYPE remote_bd_type;

        le_get_conn_param(GAP_PARAM_CONN_INTERVAL, &conn_interval, conn_id);
        le_get_conn_param(GAP_PARAM_CONN_LATENCY, &conn_latency, conn_id);
        le_get_conn_param(GAP_PARAM_CONN_TIMEOUT, &conn_supervision_timeout, conn_id);
        le_get_conn_addr(conn_id, remote_bd, &remote_bd_type);
        APP_PRINT_INFO5("GAP_CONN_STATE_CONNECTED:remote_bd %s, remote_addr_type %d, conn_interval 0x%x, conn_latency 0x%x, conn_supervision_timeout 0x%x",
                        TRACE_BDADDR(remote_bd), remote_bd_type,
                        conn_interval, conn_latency, conn_supervision_timeout);
    }
    break;

    default:
        break;
    }
    gap_conn_state = new_state;
}

/**
 * @brief    Handle msg GAP_MSG_LE_AUTHEN_STATE_CHANGE
 * @note     All the gap authentication state events are pre-handled in this function.
 *           Then the event handling function shall be called according to the new_state
 * @param[in] conn_id Connection ID
 * @param[in] new_state  New authentication state
 * @param[in] cause Use this cause when new_state is GAP_AUTHEN_STATE_COMPLETE
 * @return   void
 */
void app_handle_authen_state_evt(uint8_t conn_id, uint8_t new_state, uint16_t cause)
{
    APP_PRINT_INFO2("app_handle_authen_state_evt:conn_id %d, cause 0x%x", conn_id, cause);

    switch (new_state)
    {
    case GAP_AUTHEN_STATE_STARTED:
    {
        APP_PRINT_INFO0("app_handle_authen_state_evt: GAP_AUTHEN_STATE_STARTED");
    }
    break;

    case GAP_AUTHEN_STATE_COMPLETE:
    {
        if (cause == GAP_SUCCESS)
        {
            APP_PRINT_INFO0("app_handle_authen_state_evt: GAP_AUTHEN_STATE_COMPLETE pair success");
        }
        else
        {
            APP_PRINT_INFO0("app_handle_authen_state_evt: GAP_AUTHEN_STATE_COMPLETE pair failed");
        }
    }
    break;

    default:
    {
        APP_PRINT_ERROR1("app_handle_authen_state_evt: unknown newstate %d", new_state);
    }
    break;
    }
}

/**
 * @brief    Handle msg GAP_MSG_LE_CONN_MTU_INFO
 * @note     This msg is used to inform APP that exchange mtu procedure is completed.
 * @param[in] conn_id Connection ID
 * @param[in] mtu_size  New mtu size
 * @return   void
 */
void app_handle_conn_mtu_info_evt(uint8_t conn_id, uint16_t mtu_size)
{
    APP_PRINT_INFO2("app_handle_conn_mtu_info_evt: conn_id %d, mtu_size %d", conn_id, mtu_size);
}

/**
 * @brief    Handle msg GAP_MSG_LE_CONN_PARAM_UPDATE
 * @note     All the connection parameter update change  events are pre-handled in this function.
 * @param[in] conn_id Connection ID
 * @param[in] status  New update state
 * @param[in] cause Use this cause when status is GAP_CONN_PARAM_UPDATE_STATUS_FAIL
 * @return   void
 */
void app_handle_conn_param_update_evt(uint8_t conn_id, uint8_t status, uint16_t cause)
{
    switch (status)
    {
    case GAP_CONN_PARAM_UPDATE_STATUS_SUCCESS:
    {
        uint16_t conn_interval;
        uint16_t conn_slave_latency;
        uint16_t conn_supervision_timeout;

        le_get_conn_param(GAP_PARAM_CONN_INTERVAL, &conn_interval, conn_id);
        le_get_conn_param(GAP_PARAM_CONN_LATENCY, &conn_slave_latency, conn_id);
        le_get_conn_param(GAP_PARAM_CONN_TIMEOUT, &conn_supervision_timeout, conn_id);
        APP_PRINT_INFO3("app_handle_conn_param_update_evt update success:conn_interval 0x%x, conn_slave_latency 0x%x, conn_supervision_timeout 0x%x",
                        conn_interval, conn_slave_latency, conn_supervision_timeout);
    }
    break;

    case GAP_CONN_PARAM_UPDATE_STATUS_FAIL:
    {
        APP_PRINT_ERROR1("app_handle_conn_param_update_evt update failed: cause 0x%x", cause);
    }
    break;

    case GAP_CONN_PARAM_UPDATE_STATUS_PENDING:
    {
        APP_PRINT_INFO0("app_handle_conn_param_update_evt update pending.");
    }
    break;

    default:
        break;
    }
}

/**
 * @brief    All the BT GAP MSG are pre-handled in this function.
 * @note     Then the event handling function shall be called according to the
 *           subtype of T_IO_MSG
 * @param[in] p_gap_msg Pointer to GAP msg
 * @return   void
 */
void app_handle_gap_msg(T_IO_MSG *p_gap_msg)
{
    T_LE_GAP_MSG gap_msg;
    uint8_t conn_id;
    memcpy(&gap_msg, &p_gap_msg->u.param, sizeof(p_gap_msg->u.param));

    APP_PRINT_TRACE1("app_handle_gap_msg: subtype %d", p_gap_msg->subtype);
    switch (p_gap_msg->subtype)
    {
    case GAP_MSG_LE_DEV_STATE_CHANGE:
    {
        app_handle_dev_state_evt(gap_msg.msg_data.gap_dev_state_change.new_state,
                                 gap_msg.msg_data.gap_dev_state_change.cause);
    }
    break;

    case GAP_MSG_LE_CONN_STATE_CHANGE:
    {
        app_handle_conn_state_evt(gap_msg.msg_data.gap_conn_state_change.conn_id,
                                  (T_GAP_CONN_STATE)gap_msg.msg_data.gap_conn_state_change.new_state,
                                  gap_msg.msg_data.gap_conn_state_change.disc_cause);
    }
    break;

    case GAP_MSG_LE_CONN_MTU_INFO:
    {
        app_handle_conn_mtu_info_evt(gap_msg.msg_data.gap_conn_mtu_info.conn_id,
                                     gap_msg.msg_data.gap_conn_mtu_info.mtu_size);
    }
    break;

    case GAP_MSG_LE_CONN_PARAM_UPDATE:
    {
        app_handle_conn_param_update_evt(gap_msg.msg_data.gap_conn_param_update.conn_id,
                                         gap_msg.msg_data.gap_conn_param_update.status,
                                         gap_msg.msg_data.gap_conn_param_update.cause);
    }
    break;

    case GAP_MSG_LE_AUTHEN_STATE_CHANGE:
    {
        app_handle_authen_state_evt(gap_msg.msg_data.gap_authen_state.conn_id,
                                    gap_msg.msg_data.gap_authen_state.new_state,
                                    gap_msg.msg_data.gap_authen_state.status);
    }
    break;

    case GAP_MSG_LE_BOND_JUST_WORK:
    {
        conn_id = gap_msg.msg_data.gap_bond_just_work_conf.conn_id;
        le_bond_just_work_confirm(conn_id, GAP_CFM_CAUSE_ACCEPT);
        APP_PRINT_INFO0("GAP_MSG_LE_BOND_JUST_WORK");
    }
    break;

    case GAP_MSG_LE_BOND_PASSKEY_DISPLAY:
    {
        uint32_t display_value = 0;
        conn_id = gap_msg.msg_data.gap_bond_passkey_display.conn_id;
        le_bond_get_display_key(conn_id, &display_value);
        APP_PRINT_INFO1("GAP_MSG_LE_BOND_PASSKEY_DISPLAY:passkey %lu", display_value);
        le_bond_passkey_display_confirm(conn_id, GAP_CFM_CAUSE_ACCEPT);
    }
    break;

    case GAP_MSG_LE_BOND_USER_CONFIRMATION:
    {
        uint32_t display_value = 0;
        conn_id = gap_msg.msg_data.gap_bond_user_conf.conn_id;
        le_bond_get_display_key(conn_id, &display_value);
        APP_PRINT_INFO1("GAP_MSG_LE_BOND_USER_CONFIRMATION: passkey %lu", display_value);
        le_bond_user_confirm(conn_id, GAP_CFM_CAUSE_ACCEPT);
    }
    break;

    case GAP_MSG_LE_BOND_PASSKEY_INPUT:
    {
        uint32_t passkey = 888888;
        conn_id = gap_msg.msg_data.gap_bond_passkey_input.conn_id;
        APP_PRINT_INFO1("GAP_MSG_LE_BOND_PASSKEY_INPUT: conn_id %d", conn_id);
        le_bond_passkey_input_confirm(conn_id, passkey, GAP_CFM_CAUSE_ACCEPT);
    }
    break;

    case GAP_MSG_LE_BOND_OOB_INPUT:
    {
        uint8_t oob_data[GAP_OOB_LEN] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
        conn_id = gap_msg.msg_data.gap_bond_oob_input.conn_id;
        APP_PRINT_INFO0("GAP_MSG_LE_BOND_OOB_INPUT");
        le_bond_set_param(GAP_PARAM_BOND_OOB_DATA, GAP_OOB_LEN, oob_data);
        le_bond_oob_input_confirm(conn_id, GAP_CFM_CAUSE_ACCEPT);
    }
    break;

    default:
        APP_PRINT_ERROR1("app_handle_gap_msg: unknown subtype %d", p_gap_msg->subtype);
        break;
    }
}
/** @} */ /* End of group PERIPH_GAP_MSG */

/** @defgroup  PERIPH_GAP_CALLBACK GAP Callback Event Handler
 * @brief Handle GAP callback event
 * @{
 */
/**
 * @brief Callback for gap le to notify app
 * @param[in] cb_type callback msy type @ref GAP_LE_MSG_Types.
 * @param[in] p_cb_data point to callback data @ref T_LE_CB_DATA.
 * @retval result @ref T_APP_RESULT
 */
T_APP_RESULT app_gap_callback(uint8_t cb_type, void *p_cb_data)
{
    T_APP_RESULT result = APP_RESULT_SUCCESS;
    T_LE_CB_DATA *p_data = (T_LE_CB_DATA *)p_cb_data;

    switch (cb_type)
    {
    case GAP_MSG_LE_DATA_LEN_CHANGE_INFO:
        APP_PRINT_INFO3("GAP_MSG_LE_DATA_LEN_CHANGE_INFO: conn_id %d, tx octets 0x%x, max_tx_time 0x%x",
                        p_data->p_le_data_len_change_info->conn_id,
                        p_data->p_le_data_len_change_info->max_tx_octets,
                        p_data->p_le_data_len_change_info->max_tx_time);
        break;

    case GAP_MSG_LE_SET_RAND_ADDR:
        printf("GAP_MSG_LE_SET_RAND_ADDR: cause 0x%x\n",
               p_data->p_le_set_rand_addr_rsp->cause);
        break;

    case GAP_MSG_LE_MODIFY_WHITE_LIST:
        APP_PRINT_INFO2("GAP_MSG_LE_MODIFY_WHITE_LIST: operation %d, cause 0x%x",
                        p_data->p_le_modify_white_list_rsp->operation,
                        p_data->p_le_modify_white_list_rsp->cause);
        break;

    case GAP_MSG_LE_ADV_UPDATE_PARAM:
        APP_PRINT_INFO0("app_gap_callback GAP_MSG_LE_ADV_UPDATE_PARAM");
        break;

    default:
        APP_PRINT_ERROR1("app_gap_callback: unhandled cb_type 0x%x", cb_type);
        break;
    }
    return result;
}
/** @} */ /* End of group PERIPH_GAP_CALLBACK */

/** @defgroup  PERIPH_SEVER_CALLBACK Profile Server Callback Event Handler
 * @brief Handle profile server callback event
 * @{
 */
/**
 * @brief    All the BT Profile service callback events are handled in this function
 * @note     Then the event handling function shall be called according to the
 *           service_id
 * @param    service_id  Profile service ID
 * @param    p_data      Pointer to callback data
 * @return   T_APP_RESULT, which indicates the function call is successful or not
 * @retval   APP_RESULT_SUCCESS  Function run successfully
 * @retval   others              Function run failed, and return number indicates the reason
 */
T_APP_RESULT app_profile_callback(T_SERVER_ID service_id, void *p_data)
{
    T_APP_RESULT app_result = APP_RESULT_SUCCESS;
    if (service_id == SERVICE_PROFILE_GENERAL_ID)
    {
        T_SERVER_APP_CB_DATA *p_param = (T_SERVER_APP_CB_DATA *)p_data;
        switch (p_param->eventId)
        {
        case PROFILE_EVT_SRV_REG_COMPLETE: // srv register result event.
            APP_PRINT_INFO1("PROFILE_EVT_SRV_REG_COMPLETE: result %d",
                            p_param->event_data.service_reg_result);
            break;

        case PROFILE_EVT_SEND_DATA_COMPLETE:
        {
            if (p_param->event_data.send_data_result.cause == GAP_SUCCESS)
            {
                APP_PRINT_INFO0("PROFILE_EVT_SEND_DATA_COMPLETE success");
            }
            else
            {
                APP_PRINT_ERROR0("PROFILE_EVT_SEND_DATA_COMPLETE failed");
            }
        }
        break;

        default:
            break;
        }
    }
    else if (service_id == custom_srv_id)
    {
        T_CUSTOM_CALLBACK_DATA *p_custom_data = (T_CUSTOM_CALLBACK_DATA *)p_data;
        switch (p_custom_data->type)
        {
        case CUSTOM_SERVICE_WRITE_MSG:
        {
            if (p_custom_data->len >= 2)
            {
                uint16_t cmdId = (p_custom_data->p_value[0] << 8) | p_custom_data->p_value[1];
                /* Strip the 2-byte cmdId prefix before passing to handler */
                zb_ble_hci_cmd_handler(cmdId,
                                       p_custom_data->len - 2,
                                       p_custom_data->p_value + 2);
            }
            else
            {
                printf("WARN: RX write too short (%d)\n", p_custom_data->len);
            }
        }
        break;
        case CUSTOM_SERVICE_NOTIFY_ENABLE:
            APP_PRINT_INFO0("Custom Service Notify Enabled");
            break;
        case CUSTOM_SERVICE_NOTIFY_DISABLE:
            APP_PRINT_INFO0("Custom Service Notify Disabled");
            break;
        default:
            break;
        }
    }

    return app_result;
}
