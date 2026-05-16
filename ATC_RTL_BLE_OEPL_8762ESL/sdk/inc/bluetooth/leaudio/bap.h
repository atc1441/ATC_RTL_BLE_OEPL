#ifndef _BAP_H_
#define _BAP_H_

#ifdef  __cplusplus
extern "C" {
#endif      /* __cplusplus */

#include "ble_audio_def.h"
#include "pacs_client.h"
#include "codec_qos.h"
//#include "ascs_def.h"

#define BAP_BROADCAST_SOURCE_ROLE    0x01
#define BAP_BROADCAST_SINK_ROLE      0x02
#define BAP_BROADCAST_ASSISTANT_ROLE 0x04
#define BAP_SCAN_DELEGATOR_ROLE      0x08
#define BAP_UNICAST_CLT_SRC_ROLE     0x10
#define BAP_UNICAST_CLT_SNK_ROLE     0x20
#define BAP_UNICAST_SRV_SRC_ROLE     0x40
#define BAP_UNICAST_SRV_SNK_ROLE     0x80

typedef struct
{
    uint8_t role_mask;
    uint8_t brs_num; //use isoc_big_receiver_num

    uint8_t snk_ase_num;
    uint8_t src_ase_num;

    bool    init_gap;
//for BAP_UNICAST_CLT_SRC_ROLE, BAP_UNICAST_CLT_SNK_ROLE
    uint8_t isoc_cig_max_num;

//for BAP_UNICAST_CLT_SRC_ROLE, BAP_UNICAST_CLT_SNK_ROLE, BAP_UNICAST_SRV_SRC_ROLE, BAP_UNICAST_SRV_SNK_ROLE
    uint8_t isoc_cis_max_num;

//for BAP_BROADCAST_SOURCE_ROLE
    uint8_t pa_adv_num;
    uint8_t isoc_big_broadcaster_num;
    uint8_t isoc_bis_broadcaster_num;

//for BAP_BROADCAST_SINK_ROLE and BAP_BROADCAST_ASSISTANT_ROLE
    uint8_t pa_sync_num;
    uint8_t isoc_big_receiver_num;
    uint8_t isoc_bis_receiver_num;
} T_BAP_ROLE_INFO;

#if LE_AUDIO_PACS_CLIENT_SUPPORT
#define PACS_AUDIO_AVAILABLE_CONTEXTS_EXIST 0x01
#define PACS_AUDIO_SUPPORTED_CONTEXTS_EXIST 0x02
#define PACS_SINK_AUDIO_LOC_EXIST           0x04
#define PACS_SINK_PAC_EXIST                 0x08
#define PACS_SOURCE_AUDIO_LOC_EXIST         0x10
#define PACS_SOURCE_PAC_EXIST               0x20

typedef struct
{
    uint16_t            value_exist;
    uint8_t             sink_pac_num;
    uint8_t             source_pac_num;
    uint32_t            snk_audio_loc;
    uint32_t            src_audio_loc;
    uint16_t            snk_sup_context;
    uint16_t            src_sup_context;
    uint16_t            snk_avail_context;
    uint16_t            src_avail_context;
} T_BAP_PACS_INFO;

typedef struct
{
    uint8_t                    codec_id[CODEC_ID_LEN];
    uint16_t                   handle;
    uint16_t                   pref_audio_contexts;
    T_CODEC_CAP                codec_cap;
    uint32_t                   lc3_sup_cfg_bits;
} T_BAP_PAC_RECORD;

//LE_AUDIO_MSG_BAP_PACS_DIS_DONE
typedef struct
{
    uint16_t conn_handle;
    bool    is_found;
    uint8_t sink_pac_num;
    uint8_t source_pac_num;
} T_BAP_PACS_DIS_DONE;

//LE_AUDIO_MSG_BAP_PAC_NOTIFY
typedef struct
{
    uint16_t conn_handle;
    uint16_t handle;
} T_BAP_PAC_NOTIFY;

bool bap_pacs_get_info(uint16_t conn_handle, T_BAP_PACS_INFO *p_pacs_info);
bool bap_pacs_get_pac_record(uint16_t conn_handle, T_AUDIO_DIRECTION direction, uint8_t *p_pac_num,
                             T_BAP_PAC_RECORD *p_pac_tbl);
bool bap_pacs_get_pac_record_by_handle(uint16_t conn_handle, uint16_t handle, uint8_t *p_pac_num,
                                       T_BAP_PAC_RECORD *p_pac_tbl);
uint32_t bap_pacs_get_lc3_snk_table_msk(uint16_t conn_handle, uint16_t prefer_context,
                                        uint8_t chl_cnt,
                                        uint8_t block_num);
uint32_t bap_pacs_get_lc3_src_table_msk(uint16_t conn_handle, uint16_t prefer_context,
                                        uint8_t chl_cnt,
                                        uint8_t block_num);
#endif

#if LE_AUDIO_ASCS_CLIENT_SUPPORT
#define ASE_ID_MAX_NUM 8
//LE_AUDIO_MSG_BAP_ASCS_DIS_DONE
typedef struct
{
    uint16_t conn_handle;
    bool    is_found;
    uint8_t sink_ase_num;
    uint8_t sink_ase_id[ASE_ID_MAX_NUM];
    uint8_t source_ase_num;
    uint8_t source_ase_id[ASE_ID_MAX_NUM];
} T_BAP_ASCS_DIS_DONE;
#endif

bool bap_role_init(T_BAP_ROLE_INFO *p_role_info);

#ifdef  __cplusplus
}
#endif      /*  __cplusplus */

#endif
