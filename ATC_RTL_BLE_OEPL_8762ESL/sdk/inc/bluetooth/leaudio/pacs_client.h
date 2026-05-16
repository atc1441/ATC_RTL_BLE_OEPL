#ifndef _PACS_CLIENT_H_
#define _PACS_CLIENT_H_

#ifdef  __cplusplus
extern "C" {
#endif      /* __cplusplus */

typedef enum
{
    PACS_OP_SINK,
    PACS_OP_SOURCE,
    PACS_OP_ALL
} T_PACS_CCCD_OP_TYPE;

typedef enum
{
    PACS_AUDIO_AVAILABLE_CONTEXTS,
    PACS_AUDIO_SUPPORTED_CONTEXTS,
    PACS_SINK_AUDIO_LOC,
    PACS_SINK_PAC,
    PACS_SOURCE_AUDIO_LOC,
    PACS_SOURCE_PAC,
} T_PACS_TYPE;

//LE_AUDIO_MSG_PACS_CLIENT_DIS_DONE
typedef struct
{
    uint16_t conn_handle;
    bool    is_found;
    bool    load_form_ftl;
    uint8_t sink_pac_num;
    uint8_t source_pac_num;
    bool    sink_loc_writable;
    bool    sink_loc_exist;
    bool    source_loc_writable;
    bool    source_loc_exist;
} T_PACS_CLIENT_DIS_DONE;

//LE_AUDIO_MSG_PACS_CLIENT_CCCD
typedef struct
{
    uint16_t conn_handle;
    T_PACS_CCCD_OP_TYPE type;
    uint16_t cause;
} T_PACS_CLIENT_CCCD;

//LE_AUDIO_MSG_PACS_CLIENT_WRITE_SINK_LOC_RESULT
//LE_AUDIO_MSG_PACS_CLIENT_WRITE_SOURCE_LOC_RESULT
typedef struct
{
    uint16_t conn_handle;
    uint16_t cause;
} T_PACS_CLIENT_WRITE_RESULT;

typedef struct
{
    bool     is_complete;
    uint16_t handle;
    uint16_t pac_record_len;
    uint8_t *p_record;
} T_PAC_CHAR_DATA;

typedef struct
{
    uint16_t sink_contexts;
    uint16_t source_contexts;
} T_AUDIO_CONTEXTS_DATA;

typedef union
{
    T_AUDIO_CONTEXTS_DATA contexts_data;
    uint32_t audio_locations;
    T_PAC_CHAR_DATA pac_data;
} T_PACS_DATA;

//LE_AUDIO_MSG_PACS_CLIENT_READ_RESULT
typedef struct
{
    uint16_t conn_handle;
    T_PACS_TYPE type;
    uint16_t cause;
    T_PACS_DATA data;
} T_PACS_CLIENT_READ_RESULT;

//LE_AUDIO_MSG_PACS_CLIENT_NOTIFY
typedef struct
{
    uint16_t conn_handle;
    T_PACS_TYPE type;
    T_PACS_DATA data;
} T_PACS_CLIENT_NOTIFY;

bool pacs_read_char_value(uint16_t conn_handle, T_PACS_TYPE type);
bool pacs_enable_cccd(uint16_t conn_handle, T_PACS_CCCD_OP_TYPE type);
bool pacs_write_sink_audio_locations(uint16_t conn_handle, uint32_t sink_audio_location);
bool pacs_write_source_audio_locations(uint16_t conn_handle, uint32_t source_audio_location);

#ifdef  __cplusplus
}
#endif      /*  __cplusplus */

#endif
