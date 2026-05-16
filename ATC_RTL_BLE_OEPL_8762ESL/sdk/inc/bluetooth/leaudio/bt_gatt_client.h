#ifndef _BT_GATT_CLIENT_H_
#define _BT_GATT_CLIENT_H_

#ifdef  __cplusplus
extern "C" {
#endif      /* __cplusplus */

#include "gap.h"
#include "profile_client.h"
#include "gap_conn_le.h"


typedef enum
{
    GATTC_STORAGE_STATE_IDLE      = 0x00,
    GATTC_STORAGE_STATE_DISCOVERY = 0x01,
    GATTC_STORAGE_STATE_DONE      = 0x02,
    GATTC_STORAGE_STATE_FAILED    = 0x03,
} T_GATTC_STORAGE_STATE;

#define  GATT_CLT_CONFIG_DISABLE     0x00
#define  GATT_CLT_CONFIG_NOTIFICATION     0x01
#define  GATT_CLT_CONFIG_INDICATION       0x02
#define  GATT_CLT_CONFIG_ALL              0x03
#define  ATTR_INSTANCE_NUM_MAX            20

typedef enum
{
    GATT_STORAGE_EVENT_SRV_TBL_GET_IND  = 0x01,
    GATT_STORAGE_EVENT_SRV_TBL_SET_IND  = 0x02,
} T_GATT_STORAGE_EVENT;

typedef enum
{
    GATT_DISCOV_MODE_CLOSED  = 0x00,
    GATT_DISCOV_MODE_REG_SRV = 0x01,
    GATT_DISCOV_MODE_ALL     = 0x02,
} T_GATT_DISCOV_MODE;

//GATT_STORAGE_EVENT_SRV_TBL_GET_IND
typedef struct
{
    uint8_t  addr[6];
    uint8_t  remote_bd_type;
    uint16_t data_len;
    uint8_t  *p_data;
} T_GATT_STORAGE_SRV_TBL_GET_IND;

//GATT_STORAGE_EVENT_SRV_TBL_SET_IND
typedef struct
{
    uint8_t   addr[6];
    uint8_t   remote_bd_type;
    uint16_t  data_len;
    uint8_t  *p_data;
} T_GATT_STORAGE_SRV_TBL_SET_IND;

typedef enum
{
    GATT_CLIENT_EVENT_DIS_DONE      = 0x01,
    GATT_CLIENT_EVENT_READ_RESULT   = 0x02,
    GATT_CLIENT_EVENT_WRITE_RESULT  = 0x03,
    GATT_CLIENT_EVENT_NOTIFY_IND    = 0x04,
    GATT_CLIENT_EVENT_CCCD_CFG      = 0x05,
    GATT_CLIENT_EVENT_DIS_ALL_STATE = 0x06,
    GATT_CLIENT_EVENT_CONN_DEL      = 0x07,
} T_GATT_CLIENT_EVENT;

typedef struct
{
    bool    is_uuid16;
    uint8_t instance_id;
    union
    {
        uint16_t    uuid16;
        uint8_t     uuid128[16];
    } p;
} T_ATTR_UUID;

typedef struct
{
    T_GATTC_STORAGE_STATE state;
    bool load_form_ftl;
} T_GATT_CLIENT_DIS_ALL_DONE;

typedef struct
{
    bool is_found;
    bool load_form_ftl;
    uint8_t srv_instance_num;
} T_GATT_CLIENT_DIS_DONE;

typedef struct
{
    bool is_cccd_desc;
    uint8_t  srv_instance_id;
    T_ATTR_UUID char_uuid;
    uint16_t cause;
    uint16_t handle;
    uint16_t value_size;
    uint8_t *p_value;
} T_GATT_CLIENT_READ_RESULT;

typedef struct
{
    bool is_cccd_desc;
    uint8_t  srv_instance_id;
    T_ATTR_UUID char_uuid;
    uint16_t cause;
    T_GATT_WRITE_TYPE type;
    uint16_t handle;
} T_GATT_CLIENT_WRITE_RESULT;

typedef struct
{
    uint8_t  srv_instance_id;
    T_ATTR_UUID char_uuid;
    bool notify;
    uint16_t handle;
    uint16_t value_size;
    uint8_t *p_value;
} T_GATT_CLIENT_NOTIFY_IND;

typedef struct
{
    bool    srv_cfg;
    uint8_t srv_instance_id;
    uint16_t cccd_data;
    uint16_t cause;
    T_ATTR_UUID uuid;
} T_GATT_CLIENT_CCCD_CFG;

typedef union
{
    T_GATT_CLIENT_DIS_DONE     dis_done;
    T_GATT_CLIENT_READ_RESULT  read_result;
    T_GATT_CLIENT_WRITE_RESULT write_result;
    T_GATT_CLIENT_NOTIFY_IND   notify_ind;
    T_GATT_CLIENT_CCCD_CFG     cccd_cfg;
} T_GATT_CLIENT_DATA;

typedef union
{
    T_GATT_CLIENT_DATA *p_gatt_data;
} T_GATTC_DATA;

typedef struct
{
    uint8_t instance_num;
    uint8_t instance_id[ATTR_INSTANCE_NUM_MAX];
} T_ATTR_INSTANCE;

typedef T_APP_RESULT(*P_FUN_GATT_CLIENT_CB)(uint16_t conn_handle, T_GATT_CLIENT_EVENT type,
                                            void *p_data);
typedef T_APP_RESULT(*P_FUN_GATT_STORAGE_CB)(T_GATT_STORAGE_EVENT type, void *p_data);

T_GAP_CAUSE gatt_client_start_discovery_all(uint16_t conn_handle, P_FUN_GATT_CLIENT_CB p_dis_cb);
T_GAP_CAUSE gatt_client_enable_srv_cccd(uint16_t conn_handle, T_ATTR_UUID *p_srv_uuid,
                                        uint8_t cccd_cfg);
T_GAP_CAUSE gatt_client_enable_char_cccd(uint16_t conn_handle, T_ATTR_UUID *p_srv_uuid,
                                         uint16_t char_uuid16, uint8_t cccd_cfg);
T_GAP_CAUSE gatt_client_enable_uuid128_char_cccd(uint16_t conn_handle, T_ATTR_UUID *p_srv_uuid,
                                                 uint8_t *p_uuid128, uint8_t cccd_cfg);
T_GAP_CAUSE gatt_client_read(uint16_t conn_handle, uint16_t handle, P_FUN_GATT_CLIENT_CB p_req_cb);
T_GAP_CAUSE gatt_client_read_uuid(uint16_t conn_handle, uint16_t start_handle,
                                  uint16_t end_handle, uint16_t uuid16, P_FUN_GATT_CLIENT_CB p_req_cb);
T_GAP_CAUSE gatt_client_write(uint16_t conn_handle, T_GATT_WRITE_TYPE write_type,
                              uint16_t handle, uint16_t length, uint8_t *p_data, P_FUN_GATT_CLIENT_CB p_req_cb);

T_GAP_CAUSE gatt_spec_client_register(T_ATTR_UUID *p_srv_uuid, P_FUN_GATT_CLIENT_CB p_fun_cb);

//find info about service
bool gatt_client_find_char_handle(uint16_t conn_handle, T_ATTR_UUID *p_srv_uuid,
                                  T_ATTR_UUID *p_char_uuid, uint16_t *p_handle);
bool gatt_client_find_primary_srv_by_include(uint16_t conn_handle, T_ATTR_UUID *p_included_srv,
                                             T_ATTR_UUID *p_primary_srv);
bool gatt_client_find_include_srv_by_primary(uint16_t conn_handle, T_ATTR_UUID *p_primary_srv,
                                             T_ATTR_UUID *p_included_srv,
                                             T_ATTR_INSTANCE *p_attr_instance);
uint8_t gatt_client_get_char_num(uint16_t conn_handle, T_ATTR_UUID *p_srv_uuid,
                                 T_ATTR_UUID *p_char_uuid);
bool gatt_client_get_char_prop(uint16_t conn_handle, T_ATTR_UUID *p_srv_uuid,
                               T_ATTR_UUID *p_char_uuid, uint16_t *p_properties);

bool gatt_client_init(T_GATT_DISCOV_MODE mode);
bool gatt_storage_register(P_FUN_GATT_STORAGE_CB p_fun_cb);

#ifdef  __cplusplus
}
#endif      /*  __cplusplus */

#endif
