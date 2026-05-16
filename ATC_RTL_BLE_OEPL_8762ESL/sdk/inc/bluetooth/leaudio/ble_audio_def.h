#ifndef _BLE_AUDIO_DEF_H_
#define _BLE_AUDIO_DEF_H_

#ifdef  __cplusplus
extern "C" {
#endif      /* __cplusplus */

#include <stdint.h>
#include <stdbool.h>

#define BROADCAST_ID_LEN                   3
#define BROADCAST_CODE_LEN                 16

#define BROADCAST_AUDIO_ANNOUNCEMENT_SRV_UUID 0x1852
#define BASIC_AUDIO_ANNOUNCEMENT_SRV_UUID     0x1851

#define AUDIO_DEFAULT_PRESENTATION_DELAY      40000

#define ADV_TARGETED_ANNOUNCEMENT           0x01
#define ADV_GENERAL_ANNOUNCEMENT            0x00

#define AUDIO_LOCATION_MONO                0x00000000
#define AUDIO_LOCATION_FL                  0x00000001
#define AUDIO_LOCATION_FR                  0x00000002
#define AUDIO_LOCATION_FC                  0x00000004
#define AUDIO_LOCATION_LFE1                0x00000008
#define AUDIO_LOCATION_BL                  0x00000010
#define AUDIO_LOCATION_BR                  0x00000020
#define AUDIO_LOCATION_FLC                 0x00000040
#define AUDIO_LOCATION_FRC                 0x00000080
#define AUDIO_LOCATION_BC                  0x00000100
#define AUDIO_LOCATION_LFE2                0x00000200
#define AUDIO_LOCATION_SIL                 0x00000400
#define AUDIO_LOCATION_SIR                 0x00000800
#define AUDIO_LOCATION_TPFL                0x00001000
#define AUDIO_LOCATION_TPFR                0x00002000
#define AUDIO_LOCATION_TPFC                0x00004000
#define AUDIO_LOCATION_TPC                 0x00008000
#define AUDIO_LOCATION_TPBL                0x00010000
#define AUDIO_LOCATION_TPBR                0x00020000
#define AUDIO_LOCATION_TPSIL               0x00040000
#define AUDIO_LOCATION_TPSIR               0x00080000
#define AUDIO_LOCATION_TPBC                0x00100000
#define AUDIO_LOCATION_BTFC                0x00200000
#define AUDIO_LOCATION_BTFL                0x00400000
#define AUDIO_LOCATION_BTFR                0x00800000
#define AUDIO_LOCATION_FLW                 0x01000000
#define AUDIO_LOCATION_FRW                 0x02000000
#define AUDIO_LOCATION_LS                  0x04000000
#define AUDIO_LOCATION_RS                  0x08000000
#define AUDIO_LOCATION_RFU                 0xF0000000
#define AUDIO_LOCATION_MASK                0x0FFFFFFF

typedef enum
{
    AUDIO_UNFRAMED = 0x00,
    AUDIO_FRAMED   = 0x01,
} T_AUDIO_FRAMING;

typedef enum
{
    SERVER_AUDIO_SINK = 0x01,
    SERVER_AUDIO_SOURCE = 0x02,
} T_AUDIO_DIRECTION;

typedef enum
{
    UNSPEC_AUDIO_INPUT            = 0x00,
    BLUETOOTH_AUDIO_INPUT         = 0x01,
    MICROPHONE_AUDIO_INTPUT       = 0x02,
    ANLOG_AUDIO_INTPUT            = 0x03,
    DIGITAL_AUDIO_INTPUT          = 0x04,
    RADIO_AUDIO_INTPUT            = 0x05,
    STREAMING_AUDIO_INTPUT        = 0x06,
} T_AUDIO_INPUT_TYPE;
#ifdef  __cplusplus
}
#endif      /*  __cplusplus */

#endif
