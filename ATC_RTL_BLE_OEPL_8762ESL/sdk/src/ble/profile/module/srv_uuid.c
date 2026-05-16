/**
*********************************************************************************************************
*               Copyright(c) 2016, Realtek Semiconductor Corporation. All rights reserved.
**********************************************************************************************************
* @file     srv_uuid.c
* @brief    the general implementation for GATT server.
* @author
* @date     2020-09-10
* @version  v1.0
*********************************************************************************************************
*/

#include <string.h>
#include "trace.h"
#include "os_mem.h"
#include "srv_uuid.h"

T_SRV_CHAR_TBL *srv_uuid_create_tbl(const T_ATTRIB_APPL *p_att_tbl, uint16_t att_tbl_size)
{
    T_SRV_CHAR_TBL *p_tbl;
    uint16_t p_num = att_tbl_size / sizeof(T_ATTRIB_APPL);
    uint16_t mem_size;
    uint8_t char_cur_num = 0;

    APP_PRINT_ERROR1("srv_uuid_create_tbl: p_num %d", p_num);
    if ((p_att_tbl == NULL) || (att_tbl_size == 0))
    {
        return NULL;
    }

    uint16_t char_type = 0;
    for (uint8_t i = 0; i < p_num; i++)
    {
        memcpy(&char_type, &p_att_tbl->type_value[0], 2);
        if (char_type == GATT_UUID_CHARACTERISTIC)
        {
            char_cur_num++;
        }
        p_att_tbl++;
    }

    mem_size = sizeof(T_SRV_CHAR_TBL) + (char_cur_num - 1) * sizeof(T_SRV_UUID_TBL);
    APP_PRINT_INFO2("srv_uuid_create_tbl: char_num %d, mem_size %d", char_cur_num, mem_size);
    p_tbl = os_mem_zalloc(RAM_TYPE_DATA_ON, mem_size);

    p_tbl->char_num = char_cur_num;
    p_att_tbl = p_att_tbl - p_num;

    uint8_t num = 0;
    for (uint8_t i = 0; i < p_num; i++)
    {
        memcpy(&char_type, &p_att_tbl->type_value[0], 2);
        if (char_type == GATT_UUID_CHARACTERISTIC)
        {
            p_att_tbl ++;
            memcpy(&p_tbl->p_char_tbl[num].char_uuid16, &p_att_tbl->type_value[0], 2);
            p_tbl->p_char_tbl[num].char_index = i + 1;
            num++;
        }
        else
        {
            p_att_tbl++;
        }
    }
    return p_tbl;
}

T_SRV_UUID_TBL *srv_find_uuid_by_index(T_SRV_CHAR_TBL *p_srv, uint8_t index)
{
    if (p_srv == NULL)
    {
        return NULL;
    }

    if (p_srv->char_num != 0)
    {
        uint8_t i = 0;
        for (i = 0; i < p_srv->char_num; i++)
        {
            if (p_srv->p_char_tbl[i].char_index == index)
            {
                APP_PRINT_INFO1("srv_find_uuid_by_index: char_num %d", i);
                return &p_srv->p_char_tbl[i];
            }
        }
    }
    APP_PRINT_ERROR2("srv_find_uuid_by_index: failed, p_srv->char_num %d, index %d",
                     p_srv->char_num, index);
    return NULL;
}

T_SRV_UUID_TBL *srv_find_index_by_uuid(T_SRV_CHAR_TBL *p_srv, uint16_t uuid16)
{
    if (p_srv == NULL)
    {
        return NULL;
    }

    if (p_srv->char_num != 0)
    {
        uint8_t i = 0;
        for (i = 0; i < p_srv->char_num; i++)
        {
            if (p_srv->p_char_tbl[i].char_uuid16 == uuid16)
            {
                return &p_srv->p_char_tbl[i];
            }
        }
    }
    APP_PRINT_ERROR2("srv_find_index_by_uuid: failed, p_srv->char_num %d, uuid16 %d",
                     p_srv->char_num, uuid16);
    return NULL;
}

