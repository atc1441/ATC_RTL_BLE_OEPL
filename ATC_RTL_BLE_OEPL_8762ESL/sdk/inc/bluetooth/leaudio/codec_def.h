#ifndef _CODEC_DEF_H_
#define _CODEC_DEF_H_

#ifdef  __cplusplus
extern "C" {
#endif      /* __cplusplus */

#define TRANSPARENT_CODEC_ID               0x03
#define LC3_CODEC_ID                       0x06
#define VENDOR_CODEC_ID                    0xFF
#define CODEC_ID_LEN                       5

/* Codec_Specific_Capabilities parameters*/
//Codec Specific Capability Types
#define CODEC_CAP_TYPE_SUPPORTED_SAMPLING_FREQUENCIES   0x01
#define CODEC_CAP_TYPE_SUPPORTED_FRAME_DURATIONS        0x02
#define CODEC_CAP_TYPE_AUDIO_CHANNEL_COUNTS             0x03
/*4 octets.
Octet 0-1: Minimum number of octets supported per codec frame
Octet 2-3: Maximum number of octets supported per codec frame*/
#define CODEC_CAP_TYPE_SUPPORTED_OCTETS_PER_CODEC_FRAME 0x04
#define CODEC_CAP_TYPE_MAX_SUPPORTED_FRAMES_PER_SDU     0x05

#define SAMPLING_FREQUENCY_8K              0x0001
#define SAMPLING_FREQUENCY_11K             0x0002
#define SAMPLING_FREQUENCY_16K             0x0004
#define SAMPLING_FREQUENCY_22K             0x0008
#define SAMPLING_FREQUENCY_24K             0x0010
#define SAMPLING_FREQUENCY_32K             0x0020
#define SAMPLING_FREQUENCY_44_1K           0x0040
#define SAMPLING_FREQUENCY_48K             0x0080
#define SAMPLING_FREQUENCY_88K             0x0100
#define SAMPLING_FREQUENCY_96K             0x0200
#define SAMPLING_FREQUENCY_176K            0x0400
#define SAMPLING_FREQUENCY_192K            0x0800
#define SAMPLING_FREQUENCY_384K            0x1000
#define SAMPLING_FREQUENCY_MASK            0x1FFF

#define FRAME_DURATION_7_5_MS_BIT           0x01
#define FRAME_DURATION_10_MS_BIT            0x02
#define FRAME_DURATION_PREFER_7_5_MS_BIT    0x10
#define FRAME_DURATION_PREFER_10_MS_BIT     0x20

#define AUDIO_CHANNEL_COUNTS_1             0x01
#define AUDIO_CHANNEL_COUNTS_2             0x02
#define AUDIO_CHANNEL_COUNTS_3             0x04
#define AUDIO_CHANNEL_COUNTS_4             0x08
#define AUDIO_CHANNEL_COUNTS_5             0x10
#define AUDIO_CHANNEL_COUNTS_6             0x20
#define AUDIO_CHANNEL_COUNTS_7             0x40
#define AUDIO_CHANNEL_COUNTS_8             0x80

//Config Codec operation
#define CODEC_CFG_TYPE_SAMPLING_FREQUENCY       0x01
#define CODEC_CFG_TYPE_FRAME_DURATION           0x02
#define CODEC_CFG_TYPE_AUDIO_CHANNEL_ALLOCATION 0x03
#define CODEC_CFG_TYPE_OCTET_PER_CODEC_FRAME    0x04
#define CODEC_CFG_TYPE_BLOCKS_PER_SDU           0x05

#define SAMPLING_FREQUENCY_CFG_8K              0x01
#define SAMPLING_FREQUENCY_CFG_11K             0x02
#define SAMPLING_FREQUENCY_CFG_16K             0x03
#define SAMPLING_FREQUENCY_CFG_22K             0x04
#define SAMPLING_FREQUENCY_CFG_24K             0x05
#define SAMPLING_FREQUENCY_CFG_32K             0x06
#define SAMPLING_FREQUENCY_CFG_44_1K           0x07
#define SAMPLING_FREQUENCY_CFG_48K             0x08
#define SAMPLING_FREQUENCY_CFG_88K             0x09
#define SAMPLING_FREQUENCY_CFG_96K             0x0A
#define SAMPLING_FREQUENCY_CFG_176K            0x0B
#define SAMPLING_FREQUENCY_CFG_192K            0x0C
#define SAMPLING_FREQUENCY_CFG_384K            0x0D

#define FRAME_DURATION_CFG_7_5_MS              0x00
#define FRAME_DURATION_CFG_10_MS               0x01



#define CODEC_CAP_SUPPORTED_SAMPLING_FREQUENCIES_EXIST   0x0001
#define CODEC_CAP_SUPPORTED_FRAME_DURATIONS_EXIST        0x0002
#define CODEC_CAP_AUDIO_CHANNEL_COUNTS_EXIST             0x0004
#define CODEC_CAP_SUPPORTED_OCTETS_PER_CODEC_FRAME_EXIST 0x0008
#define CODEC_CAP_MAX_SUPPORTED_FRAMES_PER_SDU_EXIST     0x0010

#define CODEC_CFG_SAMPLING_FREQUENCY_EXIST       0x0001
#define CODEC_CFG_FRAME_DURATION_EXIST           0x0002
#define CODEC_CFG_AUDIO_CHANNEL_ALLOCATION_EXIST 0x0004
#define CODEC_CFG_OCTET_PER_CODEC_FRAME_EXIST    0x0008
#define CODEC_CFG_TYPE_BLOCKS_PER_SDU_EXIST      0x0010

typedef struct
{
    uint16_t type_exist;
    uint16_t supported_sampling_frequencies;
    uint8_t  supported_frame_durations;
    uint8_t  audio_channel_counts;
    uint8_t  max_supported_codec_frames_per_sdu;
    uint16_t min_octets_per_codec_frame;
    uint16_t max_octets_per_codec_frame;
} T_CODEC_CAP;

typedef struct
{
    uint16_t type_exist;
    uint8_t  frame_duration;
    uint8_t  sample_frequency;
    uint8_t  codec_frame_blocks_per_sdu;
    uint16_t octets_per_codec_frame;
    uint32_t audio_channel_allocation;
    uint32_t presentation_delay;
} T_CODEC_CFG;

uint8_t count_bits_1(uint32_t value);
bool codec_cap_parse(uint8_t len, uint8_t *p_data, T_CODEC_CAP *p_cap);
bool codec_cfg_parse(uint8_t len, uint8_t *p_data, T_CODEC_CFG *p_cfg);
bool codec_cfg_gen(uint8_t *p_len, uint8_t *p_data, T_CODEC_CFG *p_cfg);

#ifdef  __cplusplus
}
#endif      /*  __cplusplus */

#endif
