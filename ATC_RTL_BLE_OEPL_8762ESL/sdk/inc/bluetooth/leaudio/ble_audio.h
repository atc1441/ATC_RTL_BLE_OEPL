#ifndef _BLE_AUDIO_H_
#define _BLE_AUDIO_H_

#ifdef  __cplusplus
extern "C" {
#endif      /* __cplusplus */

#include "gap_msg.h"
#include "gap.h"
#include "app_msg.h"
//#include "profile_server_def.h"

typedef enum
{
    LE_AUDIO_MSG_GROUP_BASS         = 0x0000,
    LE_AUDIO_MSG_GROUP_BASS_CLIENT  = 0x0100,
    LE_AUDIO_MSG_GROUP_PACS         = 0x0200,
    LE_AUDIO_MSG_GROUP_PACS_CLIENT  = 0x0300,
    LE_AUDIO_MSG_GROUP_CSIS         = 0x0400,
    LE_AUDIO_MSG_GROUP_CSIS_CLIENT  = 0x0500,

    LE_AUDIO_MSG_GROUP_CAP          = 0x2000,
    LE_AUDIO_MSG_GROUP_BAP          = 0x2100,
} T_LE_AUDIO_MSG_GROUP;

typedef enum
{
    //bass_client.h
    LE_AUDIO_MSG_BASS_CLIENT_DIS_DONE         = LE_AUDIO_MSG_GROUP_BASS_CLIENT | 0x00,
    LE_AUDIO_MSG_BASS_CLIENT_CCCD             = LE_AUDIO_MSG_GROUP_BASS_CLIENT | 0x01,
    LE_AUDIO_MSG_BASS_CLIENT_CP_RESULT        = LE_AUDIO_MSG_GROUP_BASS_CLIENT | 0x02,
    LE_AUDIO_MSG_BASS_CLIENT_BRS_DATA         = LE_AUDIO_MSG_GROUP_BASS_CLIENT | 0x03,
    LE_AUDIO_MSG_BASS_CLIENT_SYNC_INFO_REQ    = LE_AUDIO_MSG_GROUP_BASS_CLIENT | 0x04,

    //pacs_client.h
    LE_AUDIO_MSG_PACS_CLIENT_DIS_DONE                = LE_AUDIO_MSG_GROUP_PACS_CLIENT | 0x00,
    LE_AUDIO_MSG_PACS_CLIENT_CCCD                    = LE_AUDIO_MSG_GROUP_PACS_CLIENT | 0x01,
    LE_AUDIO_MSG_PACS_CLIENT_WRITE_SINK_LOC_RESULT   = LE_AUDIO_MSG_GROUP_PACS_CLIENT | 0x02,
    LE_AUDIO_MSG_PACS_CLIENT_WRITE_SOURCE_LOC_RESULT = LE_AUDIO_MSG_GROUP_PACS_CLIENT | 0x03,
    LE_AUDIO_MSG_PACS_CLIENT_READ_RESULT             = LE_AUDIO_MSG_GROUP_PACS_CLIENT | 0x04,
    LE_AUDIO_MSG_PACS_CLIENT_NOTIFY                  = LE_AUDIO_MSG_GROUP_PACS_CLIENT | 0x05,

    //csis_client.h
    LE_AUDIO_MSG_CSIS_CLIENT_DIS_DONE           = LE_AUDIO_MSG_GROUP_CSIS_CLIENT | 0x00,
    LE_AUDIO_MSG_CSIS_CLIENT_SEARCH_TIMEOUT     = LE_AUDIO_MSG_GROUP_CSIS_CLIENT | 0x01,
    LE_AUDIO_MSG_CSIS_CLIENT_SEARCH_DONE        = LE_AUDIO_MSG_GROUP_CSIS_CLIENT | 0x02,
    LE_AUDIO_MSG_CSIS_CLIENT_SET_MEM_FOUND      = LE_AUDIO_MSG_GROUP_CSIS_CLIENT | 0x04,
    LE_AUDIO_MSG_CSIS_CLIENT_COOR_SET_DEL       = LE_AUDIO_MSG_GROUP_CSIS_CLIENT | 0x05,
    LE_AUDIO_MSG_CSIS_CLIENT_LOCK_REQ_DONE      = LE_AUDIO_MSG_GROUP_CSIS_CLIENT | 0x06,
    LE_AUDIO_MSG_CSIS_CLIENT_UNLOCK_REQ_DONE    = LE_AUDIO_MSG_GROUP_CSIS_CLIENT | 0x07,
    LE_AUDIO_MSG_CSIS_CLIENT_LOCK_STATE_CHANGE  = LE_AUDIO_MSG_GROUP_CSIS_CLIENT | 0x08,
    LE_AUDIO_MSG_CSIS_CLIENT_READ_RESULT        = LE_AUDIO_MSG_GROUP_CSIS_CLIENT | 0x09,
    LE_AUDIO_MSG_CSIS_CLIENT_SIRK_CHANGE        = LE_AUDIO_MSG_GROUP_CSIS_CLIENT | 0x0a,

    //bap.h
    LE_AUDIO_MSG_BAP_PACS_DIS_DONE                    = LE_AUDIO_MSG_GROUP_BAP | 0x00,
    LE_AUDIO_MSG_BAP_PAC_NOTIFY                       = LE_AUDIO_MSG_GROUP_BAP | 0x01,
    LE_AUDIO_MSG_BAP_ASCS_DIS_DONE                    = LE_AUDIO_MSG_GROUP_BAP | 0x02,

    //cap.h
    LE_AUDIO_MSG_CAP_DIS_DONE                         = LE_AUDIO_MSG_GROUP_CAP | 0x00,
} T_LE_AUDIO_MSG;

typedef T_APP_RESULT(*P_FUN_BLE_AUDIO_CB)(T_LE_AUDIO_MSG msg, void *buf);

typedef struct
{
    P_FUN_BLE_AUDIO_CB p_fun_cb;
    void *evt_queue_handle;
    void *io_queue_handle;
} T_BLE_AUDIO_PARAMS;

bool ble_audio_init(T_BLE_AUDIO_PARAMS *p_param);
bool ble_audio_check_remote_features(uint16_t conn_handle, uint8_t array_index,
                                     uint8_t feature_mask);

void ble_audio_handle_msg(T_IO_MSG *p_io_msg);
void ble_audio_handle_gap_msg(uint16_t subtype, T_LE_GAP_MSG gap_msg);
void ble_audio_handle_gap_cb(uint8_t cb_type, void *p_cb_data);

#ifdef  __cplusplus
}
#endif      /*  __cplusplus */

#endif
