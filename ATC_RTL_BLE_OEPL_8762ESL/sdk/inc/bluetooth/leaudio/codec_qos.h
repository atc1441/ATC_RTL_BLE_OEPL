#ifndef _CODEC_QOS_H_
#define _CODEC_QOS_H_

#ifdef  __cplusplus
extern "C" {
#endif      /* __cplusplus */

#include "ble_audio_def.h"
#include "codec_def.h"

typedef enum
{
    CODEC_CFG_ITEM_8_1 = 0,
    CODEC_CFG_ITEM_8_2 = 1,
    CODEC_CFG_ITEM_16_1 = 2,
    CODEC_CFG_ITEM_16_2 = 3,
    CODEC_CFG_ITEM_24_1 = 4,
    CODEC_CFG_ITEM_24_2 = 5,
    CODEC_CFG_ITEM_32_1 = 6,
    CODEC_CFG_ITEM_32_2 = 7,
    CODEC_CFG_ITEM_441_1 = 8,
    CODEC_CFG_ITEM_441_2 = 9,
    CODEC_CFG_ITEM_48_1 = 10,
    CODEC_CFG_ITEM_48_2 = 11,
    CODEC_CFG_ITEM_48_3 = 12,
    CODEC_CFG_ITEM_48_4 = 13,
    CODEC_CFG_ITEM_48_5 = 14,
    CODEC_CFG_ITEM_48_6 = 15,
    CODEC_CFG_ITEM_LC3_MAX,

    CODEC_CFG_ITEM_VENDOR = 0xff,
} T_CODEC_CFG_ITEM;


#define CODEC_CFG_ITEM_8_1_BIT   (1 << CODEC_CFG_ITEM_8_1)
#define CODEC_CFG_ITEM_8_2_BIT   (1 << CODEC_CFG_ITEM_8_2)
#define CODEC_CFG_ITEM_16_1_BIT  (1 << CODEC_CFG_ITEM_16_1)
#define CODEC_CFG_ITEM_16_2_BIT  (1 << CODEC_CFG_ITEM_16_2)
#define CODEC_CFG_ITEM_24_1_BIT  (1 << CODEC_CFG_ITEM_24_1)
#define CODEC_CFG_ITEM_24_2_BIT  (1 << CODEC_CFG_ITEM_24_2)
#define CODEC_CFG_ITEM_32_1_BIT  (1 << CODEC_CFG_ITEM_32_1)
#define CODEC_CFG_ITEM_32_2_BIT  (1 << CODEC_CFG_ITEM_32_2)
#define CODEC_CFG_ITEM_441_1_BIT (1 << CODEC_CFG_ITEM_441_1)
#define CODEC_CFG_ITEM_441_2_BIT (1 << CODEC_CFG_ITEM_441_2)
#define CODEC_CFG_ITEM_48_1_BIT  (1 << CODEC_CFG_ITEM_48_1)
#define CODEC_CFG_ITEM_48_2_BIT  (1 << CODEC_CFG_ITEM_48_2)
#define CODEC_CFG_ITEM_48_3_BIT  (1 << CODEC_CFG_ITEM_48_3)
#define CODEC_CFG_ITEM_48_4_BIT  (1 << CODEC_CFG_ITEM_48_4)
#define CODEC_CFG_ITEM_48_5_BIT  (1 << CODEC_CFG_ITEM_48_5)
#define CODEC_CFG_ITEM_48_6_BIT  (1 << CODEC_CFG_ITEM_48_6)

#define SAMPLE_FREQ_8K_TABLE_MASK       (CODEC_CFG_ITEM_8_1_BIT|CODEC_CFG_ITEM_8_2_BIT)
#define SAMPLE_FREQ_16K_TABLE_MASK      (CODEC_CFG_ITEM_16_1_BIT|CODEC_CFG_ITEM_16_2_BIT)
#define SAMPLE_FREQ_24K_TABLE_MASK      (CODEC_CFG_ITEM_24_1_BIT|CODEC_CFG_ITEM_24_2_BIT)
#define SAMPLE_FREQ_32K_TABLE_MASK      (CODEC_CFG_ITEM_32_1_BIT|CODEC_CFG_ITEM_32_2_BIT)
#define SAMPLE_FREQ_441K_TABLE_MASK     (CODEC_CFG_ITEM_441_1_BIT|CODEC_CFG_ITEM_441_2_BIT)
#define SAMPLE_FREQ_48K_TABLE_MASK      (CODEC_CFG_ITEM_48_1_BIT|CODEC_CFG_ITEM_48_2_BIT| \
                                         CODEC_CFG_ITEM_48_3_BIT|CODEC_CFG_ITEM_48_4_BIT| \
                                         CODEC_CFG_ITEM_48_5_BIT|CODEC_CFG_ITEM_48_6_BIT)

#define FREAM_DUIATION_7_5M_TALBLE_MASK  (CODEC_CFG_ITEM_8_1_BIT|CODEC_CFG_ITEM_16_1_BIT| \
                                          CODEC_CFG_ITEM_24_1_BIT|CODEC_CFG_ITEM_32_1_BIT| \
                                          CODEC_CFG_ITEM_441_1_BIT|CODEC_CFG_ITEM_48_1_BIT| \
                                          CODEC_CFG_ITEM_48_3_BIT|CODEC_CFG_ITEM_48_5_BIT)
#define FREAM_DUIATION_10M_TALBLE_MASK   (CODEC_CFG_ITEM_8_2_BIT|CODEC_CFG_ITEM_16_2_BIT| \
                                          CODEC_CFG_ITEM_24_2_BIT|CODEC_CFG_ITEM_32_2_BIT| \
                                          CODEC_CFG_ITEM_441_2_BIT|CODEC_CFG_ITEM_48_2_BIT| \
                                          CODEC_CFG_ITEM_48_4_BIT|CODEC_CFG_ITEM_48_6_BIT)

typedef enum
{
    QOS_CFG_CIS_LOW_LATENCY,
    QOS_CFG_CIS_HIG_RELIABILITY,
    QOS_CFG_BIS_LOW_LATENCY,
    QOS_CFG_BIS_HIG_RELIABILITY,
} T_QOS_CFG_TYPE;

typedef struct
{
    uint32_t sdu_interval;
    uint8_t  framing;
    uint16_t max_sdu;
    uint8_t  retransmission_number;
    uint16_t max_transport_latency;
    uint32_t presentation_delay;
} T_QOS_CFG_PREFERRED;

bool codec_preferred_cfg_get(T_CODEC_CFG_ITEM item, T_CODEC_CFG *p_cfg);
bool qos_preferred_cfg_get(T_CODEC_CFG_ITEM item, T_QOS_CFG_TYPE type, T_QOS_CFG_PREFERRED *p_qos);
bool qos_cfg_find_by_codec_cfg(T_CODEC_CFG *p_cfg, uint8_t target_latency,
                               T_QOS_CFG_PREFERRED *p_qos);
bool get_max_sdu_len_by_codec_cfg(T_CODEC_CFG *p_cfg, uint16_t *p_max_len);
bool get_sdu_interval_by_codec_cfg(T_CODEC_CFG *p_cfg, uint32_t *p_sdu_int);
bool codec_cap_get_cfg_bits(uint32_t *p_cfg_bits, T_CODEC_CAP *p_cap);


#ifdef  __cplusplus
}
#endif      /*  __cplusplus */

#endif
