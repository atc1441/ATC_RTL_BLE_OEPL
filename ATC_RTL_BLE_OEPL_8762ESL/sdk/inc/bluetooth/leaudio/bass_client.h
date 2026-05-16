#ifndef _BASS_CLIENT_H_
#define _BASS_CLIENT_H_

#ifdef  __cplusplus
extern "C" {
#endif      /* __cplusplus */

#include "bass_def.h"
#include "ble_audio_sync.h"
//#include "broadcast_source_sm.h"

#if LE_AUDIO_BASS_CLIENT_SUPPORT
//LE_AUDIO_MSG_BASS_CLIENT_DIS_DONE
typedef struct
{
    uint16_t conn_handle;
    bool    is_found;
    bool    load_form_ftl;
    uint8_t brs_char_num;
} T_BASS_CLIENT_DIS_DONE;

//LE_AUDIO_MSG_BASS_CLIENT_BRS_DATA
typedef struct
{
    uint16_t conn_handle;
    bool notify;
    uint16_t read_cause;
    uint8_t instance_id;
    T_BASS_BRS_DATA *p_brs_data;
} T_BASS_CLIENT_BRS_DATA;

//LE_AUDIO_MSG_BASS_CLIENT_SYNC_INFO_REQ
typedef struct
{
    uint16_t conn_handle;
    uint8_t instance_id;
    T_BASS_BRS_DATA *p_brs_data;
} T_BASS_CLIENT_SYNC_INFO_REQ;

//LE_AUDIO_MSG_BASS_CLIENT_CP_RESULT
typedef struct
{
    uint16_t conn_handle;
    uint16_t cause;
} T_BASS_CLIENT_CP_RESULT;

//LE_AUDIO_MSG_BASS_CLIENT_CCCD
typedef struct
{
    uint16_t conn_handle;
    uint16_t cause;
} T_BASS_CLIENT_CCCD;

bool bass_enable_cccd(uint16_t conn_handle);
bool bass_read_brs_value(uint16_t conn_handle, uint8_t instance_id);

bool bass_cp_remote_scan_stop(uint16_t conn_handle, bool is_req);
bool bass_cp_remote_scan_start(uint16_t conn_handle, bool is_req);
bool bass_cp_add_source(uint16_t conn_handle, T_BASS_CP_ADD_SOURCE *p_cp_data, bool is_req);
bool bass_cp_modify_source(uint16_t conn_handle, T_BASS_CP_MODIFY_SOURCE *p_cp_data, bool is_req);
bool bass_cp_set_broadcast_code(uint16_t conn_handle, T_BASS_CP_SET_BROADCAST_CODE *p_cp_data,
                                bool is_req);
bool bass_cp_remove_source(uint16_t conn_handle, T_BASS_CP_REMOVE_SOURCE *p_cp_data, bool is_req);
T_BASS_BRS_DATA *bass_get_brs_data(uint16_t conn_handle, uint8_t instance_id);

bool bass_cp_add_source_by_sync_info(T_BLE_AUDIO_SYNC_HANDLE handle, uint16_t conn_handle,
                                     T_BASS_PA_SYNC pa_sync, uint32_t bis_array, bool is_req);
bool bass_cp_modify_source_by_sync_info(T_BLE_AUDIO_SYNC_HANDLE handle, uint16_t conn_handle,
                                        uint8_t source_id,
                                        T_BASS_PA_SYNC pa_sync, uint32_t bis_array, bool is_req);

bool bass_transfer_syncinfo_of_remote_src(T_BLE_AUDIO_SYNC_HANDLE handle, uint16_t conn_handle,
                                          T_BASS_PAST_SRV_DATA srv_data);

#endif
#ifdef  __cplusplus
}
#endif      /*  __cplusplus */

#endif
