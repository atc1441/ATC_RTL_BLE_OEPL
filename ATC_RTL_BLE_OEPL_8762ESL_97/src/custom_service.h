#ifndef _CUSTOM_SERVICE_H_
#define _CUSTOM_SERVICE_H_

#include <profile_server.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Custom Service UUID: 0x1337 */
#define GATT_UUID_CUSTOM_SERVICE             0x1337
/* Custom Characteristic UUID: 0x1337 */
#define GATT_UUID_CHAR_CUSTOM                0x1337

/* Characteristic Index */
#define CUSTOM_SERVICE_CHAR_NOTIFY_WRITE_INDEX    0x02
#define CUSTOM_SERVICE_CHAR_CCCD_INDEX            0x03

/* Callback Message Types */
#define CUSTOM_SERVICE_WRITE_MSG                  1
#define CUSTOM_SERVICE_NOTIFY_ENABLE              2
#define CUSTOM_SERVICE_NOTIFY_DISABLE             3

typedef struct
{
    uint8_t conn_id;
    uint8_t type;
    uint16_t len;
    uint8_t *p_value;
} T_CUSTOM_CALLBACK_DATA;

/**
 * @brief Add custom BLE service (0x1337) to the BLE stack.
 * @param p_func Callback for service events.
 * @return Service ID.
 */
T_SERVER_ID custom_service_add_service(void *p_func);

/**
 * @brief Send notification to client.
 * @param conn_id Connection ID.
 * @param service_id Service ID.
 * @param p_value Data to notify.
 * @param length Data length.
 * @return true on success.
 */
bool custom_service_send_notification(uint8_t conn_id, T_SERVER_ID service_id, void *p_value, uint16_t length);

#ifdef __cplusplus
}
#endif

#endif
