#ifndef _BLE_AUDIO_GROUP_H_
#define _BLE_AUDIO_GROUP_H_

#ifdef  __cplusplus
extern "C" {
#endif      /* __cplusplus */

#include "gap.h"
#include "gap_msg.h"

typedef void *T_BLE_AUDIO_GROUP_HANDLE;
typedef void *T_BLE_AUDIO_DEV_HANDLE;

typedef enum
{
    AUDIO_GROUP_MSG_BAP_STATE                         = 0x01,
    AUDIO_GROUP_MSG_BAP_SESSION_REMOVE                = 0x02,
    AUDIO_GROUP_MSG_BAP_START_QOS_CFG                 = 0x04,
    AUDIO_GROUP_MSG_BAP_CREATE_CIS                    = 0x05,
    AUDIO_GROUP_MSG_BAP_START_METADATA_CFG            = 0x06,
    AUDIO_GROUP_MSG_BAP_SETUP_DATA_PATH               = 0x07,
    AUDIO_GROUP_MSG_BAP_REMOVE_DATA_PATH              = 0x08,
    AUDIO_GROUP_MSG_BAP_METADATA_UPDATE               = 0x09,
    AUDIO_GROUP_MSG_BAP_CIS_DISCONN                   = 0x0A,

    AUDIO_GROUP_MSG_DEV_CONN                          = 0x20,
    AUDIO_GROUP_MSG_DEV_DISCONN                       = 0x21,
} T_AUDIO_GROUP_MSG;

//AUDIO_GROUP_MSG_DEV_CONN
typedef struct
{
    T_BLE_AUDIO_DEV_HANDLE dev_handle;
} T_AUDIO_GROUP_MSG_DEV_CONN;

//AUDIO_GROUP_MSG_DEV_DISCONN
typedef struct
{
    T_BLE_AUDIO_DEV_HANDLE dev_handle;
    uint16_t               cause;
} T_AUDIO_GROUP_MSG_DEV_DISCONN;

#if 0
typedef enum
{
    AUDIO_GROUP_RESULT_SUCCESS,
    AUDIO_GROUP_RESULT_FAILED,
    AUDIO_GROUP_RESULT_TIMEOUT_FAILED,
    AUDIO_GROUP_RESULT_TIMEOUT_PARTIAL_SUCCESS,
} T_AUDIO_GROUP_RESULT;
#endif

typedef struct
{
    bool                   is_used;
    T_BLE_AUDIO_DEV_HANDLE dev_handle;
    T_GAP_CONN_STATE       conn_state;
    T_GAP_REMOTE_ADDR_TYPE addr_type;
    uint8_t                bd_addr[6];
} T_AUDIO_DEV_INFO;

typedef T_APP_RESULT(*P_FUN_AUDIO_GROUP_CB)(T_AUDIO_GROUP_MSG msg, T_BLE_AUDIO_GROUP_HANDLE handle,
                                            void *buf);

T_BLE_AUDIO_GROUP_HANDLE ble_audio_group_allocate(void);
bool ble_audio_group_reg_cb(T_BLE_AUDIO_GROUP_HANDLE group_handle, P_FUN_AUDIO_GROUP_CB p_fun_cb);
bool ble_audio_group_release(T_BLE_AUDIO_GROUP_HANDLE group_handle);
T_BLE_AUDIO_DEV_HANDLE ble_audio_group_add_dev(T_BLE_AUDIO_GROUP_HANDLE group_handle,
                                               uint8_t *p_bd_addr, uint8_t addr_type);
bool ble_audio_group_remove_dev(T_BLE_AUDIO_GROUP_HANDLE group_handle,
                                T_BLE_AUDIO_DEV_HANDLE dev_handle);
T_BLE_AUDIO_DEV_HANDLE ble_audio_group_find_dev(T_BLE_AUDIO_GROUP_HANDLE group_handle,
                                                uint8_t *bd_addr, uint8_t addr_type);
T_BLE_AUDIO_DEV_HANDLE ble_audio_group_find_dev_by_conn_handle(T_BLE_AUDIO_GROUP_HANDLE
                                                               group_handle,
                                                               uint16_t conn_handle);
bool ble_audio_group_get_dev_info(T_BLE_AUDIO_GROUP_HANDLE group_handle,
                                  T_BLE_AUDIO_DEV_HANDLE dev_handle,
                                  T_AUDIO_DEV_INFO *p_info);
uint8_t ble_audio_group_get_used_dev_num(T_BLE_AUDIO_GROUP_HANDLE group_handle);
uint8_t ble_audio_group_get_dev_num(T_BLE_AUDIO_GROUP_HANDLE group_handle);
bool ble_audio_group_get_info(T_BLE_AUDIO_GROUP_HANDLE group_handle, uint8_t *p_dev_num,
                              T_AUDIO_DEV_INFO *p_dev_tbl);

#ifdef  __cplusplus
}
#endif      /*  __cplusplus */

#endif
