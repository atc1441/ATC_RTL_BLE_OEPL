#ifndef _BASS_DEF_H_
#define _BASS_DEF_H_

#ifdef  __cplusplus
extern "C" {
#endif      /* __cplusplus */

#include "gap.h"
#include "metadata_def.h"
#include "ble_audio_def.h"

#define ATT_ERR_BASS_OPCODE_NOT_SUPPORT 0x80
#define ATT_ERR_BASS_INVALID_SRC_ID     0x81

#define BASS_FAILED_TO_SYNC_TO_BIG 0xFFFFFFFF
#define BASS_CP_BIS_SYNC_NO_PREFER 0xFFFFFFFF
#define BASS_PA_INTERVAL_UNKNOWN   0xFFFF

#define BASS_BRS_DATA_MIN_LEN 15
// BASS CP minimum possible total length
#define BASS_CP_OP_MIN_LEN                1
#define BASS_CP_OP_REMOTE_SCAN_STOP_LEN   1
#define BASS_CP_OP_REMOTE_SCAN_START_LEN  1
#define BASS_CP_OP_ADD_SOURCE_MIN_LEN     16
#define BASS_CP_OP_MODIFY_SOURCE_MIN_LEN  6
#define BASS_CP_OP_SET_BROADCAST_CODE_LEN 18
#define BASS_CP_OP_REMOVE_SOURCE_LEN      2

#define BASS_BRA_INC_BST_CODE_LEN         31

#define BASS_BRS_CHAR_MAX_NUM                    4
#define GATT_UUID_BASS                           0x184F
#define BASS_UUID_CHAR_BROADCAST_AUDIO_SCAN_CP   0x2BC7
#define BASS_UUID_CHAR_BROADCAST_RECEIVE_STATE   0x2BC8

typedef enum
{
    PA_SYNC_STATE_NOT_SYNC     = 0x00,
    PA_SYNC_STATE_SYNCINFO_REQ = 0x01,
    PA_SYNC_STATE_SYNC         = 0x02,
    PA_SYNC_STATE_FAILED       = 0x03,
    PA_SYNC_STATE_NO_PAST      = 0x04,
} T_PA_SYNC_STATE;

typedef enum
{
    BIG_NOT_ENCRYPTED           = 0x00,
    BIG_BROADCAST_CODE_REQUIRED = 0x01,
    BIG_DECRYPTING              = 0x02,
    BIG_BAD_CODE                = 0x03,
} T_BIG_ENCRYPTION_STATE;

typedef enum
{
    BASS_CP_OP_REMOTE_SCAN_STOP   = 0x00,
    BASS_CP_OP_REMOTE_SCAN_START  = 0x01,
    BASS_CP_OP_ADD_SOURCE         = 0x02,
    BASS_CP_OP_MODIFY_SOURCE      = 0x03,
    BASS_CP_OP_SET_BROADCAST_CODE = 0x04,
    BASS_CP_OP_REMOVE_SOURCE      = 0x05,
} T_BASS_CP_OP;

typedef enum
{
    BASS_PA_NOT_SYNC     = 0x00,
    BASS_PA_SYNC_PAST    = 0x01,
    BASS_PA_SYNC_NO_PAST = 0x02,
} T_BASS_PA_SYNC;

typedef struct
{
    uint8_t adv_a_match_ext_adv; //AdvA in PAST matches AdvA in ADB_EXT_IND, 0b0=Yes, 0b1=No/Do't know
    uint8_t adv_a_match_src;//AdvA in PAST matches Source_Address, 0b0=Yes, 0b1=No/Do't know
    uint8_t source_id;
} T_BASS_PAST_SRV_DATA;

typedef struct
{
    uint32_t bis_sync;
    uint8_t metadata_len;
    uint8_t *p_metadata;
} T_BASS_CP_BIS_INFO;

typedef struct
{
    bool brs_is_used;
    //Broadcast Receive State Field
    uint8_t source_id;
    uint8_t source_address_type;
    uint8_t source_address[GAP_BD_ADDR_LEN];
    uint8_t source_adv_sid;
    uint8_t broadcast_id[BROADCAST_ID_LEN];
    T_PA_SYNC_STATE pa_sync_state;
    uint32_t bis_sync_state;
    T_BIG_ENCRYPTION_STATE big_encryption;
    uint8_t bad_code[BROADCAST_CODE_LEN];
    uint8_t num_subgroups;
    uint16_t bis_info_size;
    T_BASS_CP_BIS_INFO  *p_cp_bis_info;
} T_BASS_BRS_DATA;

typedef struct
{
    uint8_t advertiser_address_type;
    uint8_t advertiser_address[GAP_BD_ADDR_LEN];
    uint8_t advertiser_sid;
    uint8_t broadcast_id[BROADCAST_ID_LEN];
    T_BASS_PA_SYNC pa_sync;
    uint16_t pa_interval;
    uint8_t num_subgroups;
    uint16_t bis_info_size;
    T_BASS_CP_BIS_INFO  *p_cp_bis_info;
} T_BASS_CP_ADD_SOURCE;

typedef struct
{
    uint8_t source_id;
    T_BASS_PA_SYNC pa_sync;
    uint16_t pa_interval;
    uint8_t num_subgroups;
    uint16_t bis_info_size;
    T_BASS_CP_BIS_INFO  *p_cp_bis_info;
} T_BASS_CP_MODIFY_SOURCE;

typedef struct
{
    uint8_t source_id;
    uint8_t broadcast_code[BROADCAST_CODE_LEN];
} T_BASS_CP_SET_BROADCAST_CODE;

typedef struct
{
    uint8_t source_id;
} T_BASS_CP_REMOVE_SOURCE;

typedef union
{
    T_BASS_CP_ADD_SOURCE         add_source;
    T_BASS_CP_MODIFY_SOURCE      modify_source;
    T_BASS_CP_SET_BROADCAST_CODE set_broadcast_code;
    T_BASS_CP_REMOVE_SOURCE      remove_source;
} T_BASS_CP_PARAM;

typedef struct
{
    T_BASS_CP_OP opcode;
    T_BASS_CP_PARAM param;
} T_BASS_CP_DATA;

#ifdef  __cplusplus
}
#endif      /*  __cplusplus */

#endif
