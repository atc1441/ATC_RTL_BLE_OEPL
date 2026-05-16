#ifndef _CAP_H_
#define _CAP_H_

#ifdef  __cplusplus
extern "C" {
#endif      /* __cplusplus */

#include "ble_audio_group.h"
#include "csis_def.h"

#if LE_AUDIO_CAP_SUPPORT
#define CAP_ACCEPTOR_ROLE    0x01
#define CAP_INITIATOR_ROLE   0x02
#define CAP_CAMMANDER_ROLE   0x04

#define GATT_UUID_CAS             0x1853

typedef struct
{
    uint8_t default_volume_settings;
    uint8_t default_mute;
    uint8_t default_mic_mute;
} T_CAP_DEFAULT_PARAM;

typedef struct
{
    uint8_t cap_role;
    bool cas_client;
    bool csip_set_coordinator;
    uint8_t csis_num;
    struct
    {
        bool enable;
        T_CSIS_SIRK_TYPE csis_sirk_type;
        uint8_t csis_size;
        uint8_t csis_rank;
        uint8_t csis_feature;
        uint8_t *csis_sirk;
    } cas;
    struct
    {
        bool vcp_volume_controller;
        bool micp_mic_controller;
        uint8_t default_volume_settings;
        uint8_t default_mute;
        uint8_t default_mic_mute;
    } vcp_micp;

    bool ccp_call_control_client;
    struct
    {
        bool ccp_call_control_server;
        uint8_t tbs_num;
    } tbs;
    bool mcp_media_control_client;
    struct
    {
        bool mcp_media_control_server;
        uint8_t mcs_num;
        uint8_t ots_num;
    } mcs;
} T_CAP_INIT_PARAMS;

typedef struct
{
    uint16_t conn_handle;
    bool    is_found;
    bool    load_form_ftl;
    uint8_t srv_num;
} T_CAP_DIS_DONE;

bool cap_change_volume_by_address(uint8_t *bd_addr, uint8_t addr_type, uint8_t volume_setting);
bool cap_change_volume(T_BLE_AUDIO_GROUP_HANDLE group_handle, uint8_t volume_setting);
bool cap_change_mute_by_address(uint8_t *bd_addr, uint8_t addr_type, uint8_t mute);
bool cap_change_mute(T_BLE_AUDIO_GROUP_HANDLE group_handle, uint8_t mute);
bool cap_change_mic_mute_by_address(uint8_t *bd_addr, uint8_t addr_type, uint8_t mute);
bool cap_change_input_gain_by_address(uint8_t *bd_addr, uint8_t addr_type, int8_t gain);
bool cap_init(T_CAP_INIT_PARAMS *p_param);

#endif

#ifdef  __cplusplus
}
#endif      /*  __cplusplus */

#endif
