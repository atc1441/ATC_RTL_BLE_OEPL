/**
************************************************************************************************************
*               Copyright(c) 2014-2015, Realtek Semiconductor Corporation. All rights reserved.
************************************************************************************************************
* @file     ima_adpcm_enc.h
* @brief
* @author   Chenjie Jin
* @date     2018-05-14
* @version  v0.2
*************************************************************************************************************
*/

#ifndef __IMA_ADPCM_ENCODE_H__
#define __IMA_ADPCM_ENCODE_H__

#include "stdint.h"

typedef struct
{
    short valprev;  /* Previous output value */
    char index;  /* Index into stepsize table */
    uint16_t seq_id;  /* sequnce index */
} T_IMA_ADPCM_STATE;

int ima_adpcm_encode(short *indata, unsigned char *outdata, int len, T_IMA_ADPCM_STATE *state);

extern T_IMA_ADPCM_STATE ima_adpcm_global_state;

#endif /* __CVSD_ENCODE_H__ */
