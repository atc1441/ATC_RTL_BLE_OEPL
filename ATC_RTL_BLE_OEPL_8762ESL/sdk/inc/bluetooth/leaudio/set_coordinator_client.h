#ifndef _SET_COORDINATOR_TEST_H_
#define _SET_COORDINATOR_TEST_H_

#ifdef  __cplusplus
extern "C" {
#endif      /* __cplusplus */

#if LE_AUDIO_CSIS_CLIENT_SUPPORT
#include "csis_rsi.h"
#include "csis_def.h"
#include "ble_audio_group.h"
#include "os_queue.h"

#define CSIS_SIZE_UNKNOWN     0

#define CSIS_LOCK_FLAG 0x01
#define CSIS_SIRK_FLAG 0x02
#define CSIS_SIZE_FLAG 0x04
#define CSIS_RANK_FLAG 0x08

typedef struct
{
    uint16_t conn_handle;
    bool    is_found;
    bool    load_form_ftl;
    uint8_t srv_num;
} T_CSIS_CLIENT_DIS_DONE;

typedef struct
{
    uint16_t          conn_handle;
    uint16_t          cause;
    T_BLE_AUDIO_GROUP_HANDLE group_handle;
} T_CSIS_MEMBER_LOCK_REQ_DONE;

typedef struct
{
    uint16_t          conn_handle;
    uint16_t          cause;
    T_BLE_AUDIO_GROUP_HANDLE group_handle;
} T_CSIS_MEMBER_UNLOCK_REQ_DONE;

typedef struct
{
    uint16_t          conn_handle;
    uint16_t          srv_uuid;
    uint8_t           srv_instance_id;
    T_BLE_AUDIO_GROUP_HANDLE group_handle;
    uint8_t           lock;
} T_CSIS_MEMBER_LOCK_STATE;

typedef struct
{
    T_BLE_AUDIO_GROUP_HANDLE group_handle;
} T_CSIS_COOR_SET_DEL;

typedef struct
{
    uint8_t         set_size;
    uint8_t         size;
    bool            search_done;
} T_CSIS_SEARCH_RESULT;

typedef struct
{
    T_BLE_AUDIO_GROUP_HANDLE group_handle;
    T_BLE_AUDIO_DEV_HANDLE   dev_handle;
    uint8_t                 bd_addr[6];
    uint8_t                 addr_type;
    uint16_t                srv_uuid;
    uint8_t                 rank;
    uint8_t                 size;
    uint8_t                 sirk[CSI_SIRK_LEN];
} T_CSIS_SET_MEM_FOUND;

typedef struct
{
    uint16_t                cause;
    uint16_t                conn_handle;
    T_BLE_AUDIO_GROUP_HANDLE group_handle;
    T_BLE_AUDIO_DEV_HANDLE   dev_handle;
    uint8_t                 bd_addr[6];
    uint8_t                 addr_type;
    uint16_t                srv_uuid;
    uint8_t                 srv_instance_id;
    uint8_t                 char_exit;
    uint8_t                 rank;
    uint8_t                 size;
    uint8_t                 sirk[CSI_SIRK_LEN];
} T_CSIS_READ_RESULT;

//LE_AUDIO_MSG_CSIS_CLIENT_SIRK_CHANGE
typedef struct
{
    T_BLE_AUDIO_GROUP_HANDLE group_handle;
    uint8_t                  sirk[CSI_SIRK_LEN];
} T_CSIS_SIRK_CHANGE;

typedef struct
{
    uint16_t serv_uuid;
    uint8_t  srv_instance_id;
    uint8_t  dev_num;
    uint8_t  set_mem_size;
    uint8_t  sirk[CSI_SIRK_LEN];
} T_CSIS_GROUP_INFO;

void coordinator_read_csis_chars(uint16_t conn_handle, uint8_t instance_id);
bool csis_client_parse_ext_adv(uint8_t report_data_len, uint8_t *p_report_data,
                               uint8_t *p_bd_addr, uint8_t addr_type);
bool set_discover_members_state(T_BLE_AUDIO_GROUP_HANDLE group_handle);
bool csis_client_get_info(T_BLE_AUDIO_GROUP_HANDLE group_handle, T_CSIS_GROUP_INFO *p_info);
bool set_lock_of_coor_set(T_BLE_AUDIO_GROUP_HANDLE group_handle);
bool set_unlock_of_coor_set(T_BLE_AUDIO_GROUP_HANDLE group_handle);
T_BLE_AUDIO_GROUP_HANDLE coordinator_set_find_by_rsi(uint8_t *p_rsik);
T_BLE_AUDIO_GROUP_HANDLE coordinator_set_find_by_sirk(uint8_t *p_sirk);
T_BLE_AUDIO_GROUP_HANDLE coordinator_set_find_by_addr(uint8_t *bd_addr, uint8_t addr_type,
                                                      uint16_t serv_uuid);
T_BLE_AUDIO_DEV_HANDLE set_member_find_by_conn_handle(T_BLE_AUDIO_GROUP_HANDLE group_handle,
                                                      uint16_t conn_handle);
bool coordinator_or_member_find_by_addr(uint8_t *bd_addr, uint8_t addr_type, uint16_t serv_uuid,
                                        T_BLE_AUDIO_GROUP_HANDLE *p_group_handle, T_BLE_AUDIO_DEV_HANDLE *p_dev_handle);
bool set_member_info_find_by_dev_handle(T_BLE_AUDIO_DEV_HANDLE *p_dev_handle,
                                        T_CSIS_SET_MEM_FOUND *set_mem_info);

#endif

#ifdef  __cplusplus
}
#endif      /*  __cplusplus */

#endif
