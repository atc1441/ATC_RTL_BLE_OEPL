#ifndef _PERIPHERAL_APP__
#define _PERIPHERAL_APP__

#ifdef __cplusplus
extern "C" {
#endif
/*============================================================================*
 *                              Header Files
 *============================================================================*/
#include <app_msg.h>
#include <gap_le.h>
#include <profile_server.h>
#include <stdint.h>

/** @defgroup PERIPH_APP Peripheral Application
  * @brief Peripheral Application
  * @{
  */

/*============================================================================*
 *                              Structures
 *============================================================================*/
#pragma pack(push, 1)
struct BleAdvDataStruct
{
	uint8_t len_cap;// needs to be 2
	uint8_t type_cap;// needs to be 1
	uint8_t capabilities_cap;// needs to be 5
    uint8_t len;                // Len of manufacturer data block (sizeof(struct BleAdvDataStruct) - 1)
    uint8_t type;               // Always 0xff (Manufacturer Specific)
    uint16_t manu_id;           // 0x1337 for us
    uint8_t version;
    uint16_t hw_type;
    uint16_t fw_version;
    uint16_t capabilities;
    uint16_t battery_mv;
    int8_t temperature;
    uint8_t counter;
};
#pragma pack(pop)

typedef enum
{
	EPD_CTRL_NONE = 0,
	EPD_CTRL_UC,
	EPD_CTRL_SSD,
	EPD_CTRL_ST,
	EPD_CTRL_TI,
	EPD_CTRL_UC_PRO,
	EPD_CTRL_SSD_NEW,
} CMD_CTRL_MODEL;

typedef struct Flash_pinout_struct
{
	uint16_t CS;
	uint16_t CLK;
	uint16_t MISO;
	uint16_t MOSI;
} flash_pinout_struct;

typedef struct Screen_pinout_struct
{
	uint16_t RESET;
	uint16_t DC;
	uint16_t BUSY;
	uint16_t BUSYs;
	uint16_t CS;
	uint16_t CSs;
	uint16_t CLK;
	uint16_t MOSI;
	uint16_t ENABLE;
	uint16_t ENABLE1;
	uint8_t  ENABLE_INVERT;
	uint16_t FLASH_CS;
	uint8_t  PIN_CONFIG_SLEEP;
	uint8_t  PIN_ENABLE;
	uint8_t  PIN_ENABLE_SLEEP;
} screen_pinout_struct;

typedef struct Led_pinout_struct
{
	uint16_t R;
	uint16_t G;
	uint16_t B;
	uint8_t inverted;
} led_pinout_struct;

typedef struct Nfc_pinout_struct
{
	uint16_t SDA;
	uint16_t SCL;
	uint16_t CS;
	uint16_t IRQ;
} nfc_pinout_struct;

#pragma pack(push, 1)
typedef struct
{
    uint32_t magic;
    uint32_t version;
    uint32_t len;

    uint16_t oepl_hw_type; 

    uint8_t screen_available;
    uint16_t screen_type; 
    uint16_t screen_functions; 
    uint8_t screen_w_h_inversed_ble;
    uint16_t screen_w_h_inversed;
    uint16_t screen_h;
    uint16_t screen_w;
    uint16_t screen_h_offset;
    uint16_t screen_w_offset;
    uint8_t screen_colors;
    uint8_t screen_color_black_invert;
    uint8_t screen_color_second_invert;
    uint8_t oepl_enabled;
    uint32_t oepl_wakeup_time; 
    uint8_t ble_enabled;
    uint32_t ble_wakeup_time; 

    uint8_t nfc_available; 
    uint8_t button_available; 
    uint8_t led_available; 

    uint8_t mac_address[8];
    uint8_t ble_mac_address[6];
    uint8_t ble_device_name[40];
    uint8_t ble_device_name_len;

    uint32_t saved_EEPROM_IMG_EACH;

    screen_pinout_struct *epd_pinout;
	led_pinout_struct *led_pinout;
	nfc_pinout_struct *nfc_pinout;
	flash_pinout_struct *flash_pinout;
	uint16_t ADC_pinout;
	uint16_t UART_pinout;
	screen_pinout_struct dynamic_epd_pinout;
	led_pinout_struct dynamic_led_pinout;
	nfc_pinout_struct dynamic_nfc_pinout;
	flash_pinout_struct dynamic_flash_pinout;

    uint8_t crc;
} settings_struct;
#pragma pack(pop)

/* For backward compatibility with existing code in ble_cmd_handler.c */
#define Settings settings_struct 

/*============================================================================*
 *                              Constants
 *============================================================================*/
#define FIRMWARE_VERSION 0x0047
#define OEPL_DEVICE_NAME_LEN 11 // "RTL_" + 6 hex chars + null

/*============================================================================*
 *                              Variables
 *============================================================================*/
extern T_SERVER_ID simp_srv_id; /**< Simple ble service id*/
extern T_SERVER_ID bas_srv_id;  /**< Battery service id */
extern T_SERVER_ID custom_srv_id; /**< Custom service id */
extern settings_struct settings;
extern uint8_t adc_temperature;
extern uint8_t epd_read_temperature;
extern uint8_t device_name[OEPL_DEVICE_NAME_LEN];

/*============================================================================*
 *                              Functions
 *============================================================================*/

/**
 * @brief    Send a notification via the custom characteristic (0x1337)
 * @param[in] conn_id  Connection ID
 * @param[in] p_data   Data to send
 * @param[in] len      Data length
 * @return   true on success
 */
bool app_send_custom_notification(uint8_t conn_id, uint8_t *p_data, uint16_t len);

/**
 * @brief    Get device capabilities
 * @return   Capabilities bitmask
 */
uint16_t get_capabilities(void);

/**
 * @brief    Update advertising and scan response data
 * @param[in] battery_mv  Battery voltage in millivolts
 * @return   void
 */
void set_adv_data(uint16_t battery_mv);

/* Generate tentative random BLE address and set advertising to random type.
 * Call from app_le_gap_init() before os_sched_start(). */
void ble_addr_early_init(void);

/* Custom IO message types (user range 0x20+).
 * Sent from ble_cmd_handler to the app task so ACK and drawing are decoupled. */
#define IO_MSG_TYPE_DRAW      0x20u  /* u.param = EEPROM address, subtype = lut */
#define IO_MSG_TYPE_EPD_POLL  0x21u  /* periodic BUSY-pin check after refresh    */
#define IO_MSG_TYPE_OTA_APPLY 0x22u  /* apply staged firmware; u.param = fw_size */

/**
 * @brief    All the application messages are pre-handled in this function
 * @note     All the IO MSGs are sent to this function, then the event handling
 *           function shall be called according to the MSG type.
 * @param[in] io_msg  IO message data
 * @return   void
 */
void app_handle_io_msg(T_IO_MSG io_msg);

/**
 * @brief    All the BT Profile service callback events are handled in this function
 * @note     Then the event handling function shall be called according to the
 *           service_id.
 * @param[in] service_id  Profile service ID
 * @param[in] p_data      Pointer to callback data
 * @return   Indicates the function call is successful or not
 * @retval   result @ref T_APP_RESULT
 */
T_APP_RESULT app_profile_callback(T_SERVER_ID service_id, void *p_data);

/**
  * @brief Callback for gap le to notify app
  * @param[in] cb_type callback msy type @ref GAP_LE_MSG_Types.
  * @param[in] p_cb_data point to callback data @ref T_LE_CB_DATA.
  * @retval result @ref T_APP_RESULT
  */
T_APP_RESULT app_gap_callback(uint8_t cb_type, void *p_cb_data);

/**
 * @brief    Update advertising data at runtime
 * @param[in] p_data  Pointer to the new advertising data
 * @param[in] len     Length of the new advertising data
 * @return   Indicates the function call is successful or not
 */
bool app_update_adv_data(uint8_t *p_data, uint8_t len);


/** End of PERIPH_APP
* @}
*/


#ifdef __cplusplus
}
#endif

#endif
