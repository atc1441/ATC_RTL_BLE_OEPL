#ifndef _CSIS_RSI_H_
#define _CSIS_RSI_H_

#ifdef  __cplusplus
extern "C" {
#endif      /* __cplusplus */

#include <stdint.h>
#include <stdbool.h>

#define GAP_ADTYPE_RSI        0x2E

#define CSI_RSI_LEN           6

#if (LE_AUDIO_CSIS_CLIENT_SUPPORT|LE_AUDIO_CSIS_SUPPORT)
bool csis_gen_rsi(const uint8_t *p_sirk, uint8_t *p_rsik);
bool csis_resolve_rsi(const uint8_t *p_sirk, uint8_t *p_rsik);
#endif

#ifdef  __cplusplus
}
#endif      /*  __cplusplus */

#endif
