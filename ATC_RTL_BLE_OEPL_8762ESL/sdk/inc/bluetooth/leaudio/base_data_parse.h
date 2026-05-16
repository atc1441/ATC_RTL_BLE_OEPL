#ifndef _BASE_DATA_PARSE_H_
#define _BASE_DATA_PARSE_H_

#ifdef  __cplusplus
extern "C" {
#endif      /* __cplusplus */

#include <stdint.h>
#include "codec_def.h"

#if LE_AUDIO_BASE_DATA_PARSE

typedef struct
{
    uint8_t         subgroup_idx;
    uint8_t         bis_index;
    uint8_t         codec_id[CODEC_ID_LEN];
    T_CODEC_CFG     bis_codec_cfg;
} T_BASE_DATA_BIS_PARAM;

typedef struct
{
    uint8_t                 subgroup_idx;
    uint8_t                 num_bis;
    uint8_t                 metadata_len;
    uint32_t                bis_array;
    uint8_t                 *p_metadata;
    T_BASE_DATA_BIS_PARAM   *p_bis_param;
} T_BASE_DATA_SUBGROUP_PARAM;

typedef struct
{
    uint8_t num_bis;
    uint8_t num_subgroups;
    uint32_t presentation_delay;
    //Level2 param
    T_BASE_DATA_SUBGROUP_PARAM *p_subgroup;
} T_BASE_DATA_MAPPING;

void base_data_print(T_BASE_DATA_MAPPING *p_mapping);
T_BASE_DATA_MAPPING *base_data_parse_data(uint16_t pa_data_len, uint8_t *p_pa_data);
bool base_data_cmp(T_BASE_DATA_MAPPING *p_mapping_a, T_BASE_DATA_MAPPING *p_mapping_b);
bool base_data_get_bis_codec_cfg(T_BASE_DATA_MAPPING *p_mapping, uint8_t bis_idx,
                                 T_CODEC_CFG *p_cfg);
void base_data_free(T_BASE_DATA_MAPPING *p_mapping);

#endif

#ifdef  __cplusplus
}
#endif      /*  __cplusplus */

#endif
