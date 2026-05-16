#ifndef _BLE_AUDIO_SYNC_H_
#define _BLE_AUDIO_SYNC_H_

#ifdef  __cplusplus
extern "C" {
#endif      /* __cplusplus */

#if (LE_AUDIO_BROADCAST_SINK_ROLE | LE_AUDIO_BROADCAST_ASSISTANT_ROLE)
#include "gap.h"
#include "gap_pa_sync.h"
#include "gap_past_recipient.h"
#include "ble_audio_def.h"
#include "base_data_parse.h"

typedef void *T_BLE_AUDIO_SYNC_HANDLE;

#define MSG_BLE_AUDIO_PA_SYNC_STATE         0x01
#define MSG_BLE_AUDIO_PA_REPORT_INFO        0x02
#define MSG_BLE_AUDIO_BASE_DATA_MODIFY_INFO 0x03
#define MSG_BLE_AUDIO_PA_BIGINFO            0x04

#define MSG_BLE_AUDIO_SYNC_HANDLE_RELEASED  0x20
#define MSG_BLE_AUDIO_ADDR_UPDATE           0x21

typedef enum
{
    BLE_AUDIO_ACTION_ROLE_IDLE      = 0x00,
    BLE_AUDIO_ACTION_ROLE_LOCAL_API = 0x01,
#if LE_AUDIO_BASS_SUPPORT
    BLE_AUDIO_ACTION_ROLE_BASS      = 0x02,
#endif
} T_BLE_AUDIO_ACTION_ROLE;

typedef struct
{
    T_BASE_DATA_MAPPING *p_base_mapping;
} T_BLE_AUDIO_BASE_DATA_MODIFY_INFO;

typedef enum
{
    BLE_AUDIO_PA_IDLE         = 0x00,
    BLE_AUDIO_PA_SYNC         = 0x01,
    BLE_AUDIO_PA_TERMINATE    = 0x02,
    BLE_AUDIO_PA_LOST         = 0x03,
} T_BLE_AUDIO_PA_ACTION;

typedef struct
{
    T_GAP_PA_SYNC_STATE   sync_state;
    T_BLE_AUDIO_PA_ACTION action;
    T_BLE_AUDIO_ACTION_ROLE action_role;
    uint16_t              cause;
} T_BLE_AUDIO_PA_SYNC_STATE;

typedef struct
{
    T_BLE_AUDIO_ACTION_ROLE action_role;
} T_BLE_AUDIO_SYNC_HANDLE_RELEASED;

typedef struct
{
    uint8_t *advertiser_address;
} T_BLE_AUDIO_ADDR_UPDATE;

typedef union
{
    T_BLE_AUDIO_PA_SYNC_STATE         *p_pa_sync_state;
    T_LE_PERIODIC_ADV_REPORT_INFO     *p_le_periodic_adv_report_info;
    T_BLE_AUDIO_BASE_DATA_MODIFY_INFO *p_base_data_modify_info;

    T_LE_BIGINFO_ADV_REPORT_INFO      *p_le_biginfo_adv_report_info;

    T_BLE_AUDIO_SYNC_HANDLE_RELEASED  *p_sync_handle_released;
    T_BLE_AUDIO_ADDR_UPDATE           *p_addr_update;
} T_BLE_AUDIO_SYNC_CB_DATA;

typedef struct
{
    uint8_t advertiser_address_type;
    uint8_t advertiser_address[GAP_BD_ADDR_LEN];
    uint8_t adv_sid;
    uint8_t broadcast_id[BROADCAST_ID_LEN];
    //PA info
    uint8_t sync_id;
    T_GAP_PA_SYNC_STATE pa_state;
    uint16_t pa_interval;
    T_BASE_DATA_MAPPING *p_base_mapping;
    bool big_info_received;
    T_LE_BIGINFO_ADV_REPORT_INFO big_info;
} T_BLE_AUDIO_SYNC_INFO;

typedef void(*P_FUN_BLE_AUDIO_SYNC_CB)(T_BLE_AUDIO_SYNC_HANDLE handle, uint8_t cb_type,
                                       void *p_cb_data);

T_BLE_AUDIO_SYNC_HANDLE ble_audio_sync_create(P_FUN_BLE_AUDIO_SYNC_CB cb_pfn,
                                              uint8_t advertiser_address_type,
                                              uint8_t *advertiser_address, uint8_t adv_sid,
                                              uint8_t broadcast_id[BROADCAST_ID_LEN]);
bool ble_audio_sync_update_cb(T_BLE_AUDIO_SYNC_HANDLE handle,
                              P_FUN_BLE_AUDIO_SYNC_CB cb_pfn);
bool ble_audio_sync_update_addr(T_BLE_AUDIO_SYNC_HANDLE handle,
                                uint8_t *advertiser_address);

T_BLE_AUDIO_SYNC_HANDLE ble_audio_sync_find(uint8_t advertiser_address_type,
                                            uint8_t adv_sid, uint8_t broadcast_id[BROADCAST_ID_LEN]);
bool ble_audio_sync_get_info(T_BLE_AUDIO_SYNC_HANDLE handle, T_BLE_AUDIO_SYNC_INFO *p_info);
bool ble_audio_sync_realese(T_BLE_AUDIO_SYNC_HANDLE handle);

bool ble_audio_pa_sync_establish(T_BLE_AUDIO_SYNC_HANDLE handle, uint8_t options,
                                 uint8_t sync_cte_type,
                                 uint16_t skip, uint16_t sync_timeout);
bool ble_audio_pa_terminate(T_BLE_AUDIO_SYNC_HANDLE handle);

bool ble_audio_set_default_past_recipient_param(
    T_GAP_PAST_RECIPIENT_PERIODIC_ADV_SYNC_TRANSFER_PARAM *p_param);
bool ble_audio_set_past_recipient_param(uint16_t conn_handle,
                                        T_GAP_PAST_RECIPIENT_PERIODIC_ADV_SYNC_TRANSFER_PARAM *p_param);

#endif


#ifdef  __cplusplus
}
#endif      /*  __cplusplus */

#endif
