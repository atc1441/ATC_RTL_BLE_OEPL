#include <string.h>
#include "stdlib.h"
#include "bt_gatt_svc.h"
#include "trace.h"


T_CHAR_UUID gatt_svc_find_char_uuid_by_index(const T_ATTRIB_APPL *p_srv, uint16_t index,
                                             uint16_t attr_num)
{
    T_CHAR_UUID char_uuid;
    memset(&char_uuid, 0, sizeof(T_CHAR_UUID));
    if (p_srv == NULL)
    {
        PROFILE_PRINT_ERROR0("gatt_svc_find_char_uuid_by_index: failed, service table is NULL");
        return char_uuid;
    }

    if (index >= attr_num)
    {
        PROFILE_PRINT_ERROR0("gatt_svc_find_char_uuid_by_index: failed, index error");
        return char_uuid;
    }


    char_uuid.uu.char_uuid16 = (p_srv[index].type_value[0]) | (p_srv[index].type_value[1] << 8);
    char_uuid.uuid_size = UUID_16BIT_SIZE;
    //If it is cccd, we should find charac uuid
    if (char_uuid.uu.char_uuid16 == GATT_UUID_CHAR_CLIENT_CONFIG)
    {
        index--;
        while (index > 0)
        {
            char_uuid.uu.char_uuid16 = (p_srv[index].type_value[0]) | (p_srv[index].type_value[1] << 8);
            if (char_uuid.uu.char_uuid16 == GATT_UUID_CHARACTERISTIC)
            {
                index++;
                char_uuid.uu.char_uuid16 = (p_srv[index].type_value[0]) | (p_srv[index].type_value[1] << 8);
                break;
            }
            index--;
        }
    }

    char_uuid.index = index;
    return char_uuid;
}


/*********************************************************************
***  attr_num: the table total attr_num

***   char_uuid:
      char_uuid.index: the which num-th character to be found

***   return char index in the table
**********************************************************************/
uint16_t gatt_svc_find_char_index_by_uuid16(const T_ATTRIB_APPL *p_srv, uint16_t char_uuid16,
                                            uint16_t attr_num)
{
    T_CHAR_UUID char_uuid;
    uint16_t attrib_idx = 0;
    char_uuid.index = 1;
    char_uuid.uuid_size = UUID_16BIT_SIZE;
    char_uuid.uu.char_uuid16 = char_uuid16;

    uint16_t idx = 0;
    uint8_t char_num = 1;

    if (p_srv != NULL)
    {
        while (idx < attr_num)
        {
            if ((char_uuid.uuid_size == UUID_16BIT_SIZE && !(p_srv[idx].flags & ATTRIB_FLAG_UUID_128BIT)))
            {
                if (memcmp(&char_uuid.uu.char_uuid16, p_srv[idx].type_value, char_uuid.uuid_size) == 0)
                {
                    if (char_uuid.index == char_num)
                    {
                        attrib_idx = idx;
                        return attrib_idx;
                    }
                    char_num++;
                }
            }
            idx++;
        }
    }

    PROFILE_PRINT_ERROR0("gatt_svc_find_char_index_by_uuid16: failed");
    return attrib_idx;
}

