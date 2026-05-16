/**
*********************************************************************************************************
*               Copyright(c) 2016, Realtek Semiconductor Corporation. All rights reserved.
**********************************************************************************************************
* @file     gap_fix_chann_conn.h
* @brief    header file of LE Fixed Channel Connection message handle.
* @details  none.
* @author   Tifnan
* @date     2016-03-16
* @version  v0.1
*********************************************************************************************************
*/

#ifndef _LE_FIX_CHANN_CONN_H_
#define _LE_FIX_CHANN_CONN_H_

#include <gap_le.h>
#include <gap.h>

T_GAP_CAUSE le_fixed_chann_reg(uint16_t cid);

T_GAP_CAUSE le_fixed_chann_data_send(uint8_t conn_id, uint16_t cid, uint8_t *p_data,
                                     uint16_t data_len);

#endif /* _LE_FIX_CHANN_CONN_H_ */
