/**
*****************************************************************************************
*     Copyright(c) 2016, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
  * @file     srv_uuid.h
  * @brief    Head file for server structure.
  * @details  Common data struct definition.
  * @author
  * @date     2020-09-10
  * @version  v1.0
  * *************************************************************************************
  */

/* Define to prevent recursive inclusion */
#ifndef SRV_UUID_H
#define SRV_UUID_H

#ifdef  __cplusplus
extern "C" {
#endif      /* __cplusplus */

/*============================================================================*
 *                        Header Files
 *============================================================================*/
#include "profile_server.h"
#include "gatt.h"

typedef struct
{
    uint16_t char_uuid16;
    uint8_t char_index;
} T_SRV_UUID_TBL;

typedef struct
{
    uint8_t char_num;
    T_SRV_UUID_TBL p_char_tbl[1];
} T_SRV_CHAR_TBL;

T_SRV_CHAR_TBL *srv_uuid_create_tbl(const T_ATTRIB_APPL *p_att_tbl, uint16_t att_tbl_size);
T_SRV_UUID_TBL *srv_find_uuid_by_index(T_SRV_CHAR_TBL *p_srv, uint8_t index);
T_SRV_UUID_TBL *srv_find_index_by_uuid(T_SRV_CHAR_TBL *p_srv, uint16_t uuid16);

#ifdef  __cplusplus
}
#endif      /*  __cplusplus */

#endif /* SRV_UUID_H */
