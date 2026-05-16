#include <string.h>
#include <trace.h>
#include <gap.h>
#include "custom_service.h"

static P_FUN_SERVER_GENERAL_CB pfn_custom_service_cb = NULL;
T_SERVER_ID custom_service_id;

/**< @brief  Custom Service Definition. */
const T_ATTRIB_APPL custom_service_tbl[] =
{
    /* Primary Service: 0x1337 */
    {
        (ATTRIB_FLAG_VALUE_INCL | ATTRIB_FLAG_LE),
        {
            LO_WORD(GATT_UUID_PRIMARY_SERVICE),
            HI_WORD(GATT_UUID_PRIMARY_SERVICE),
            LO_WORD(GATT_UUID_CUSTOM_SERVICE),
            HI_WORD(GATT_UUID_CUSTOM_SERVICE)
        },
        UUID_16BIT_SIZE,
        NULL,
        GATT_PERM_READ
    },
    /* Characteristic: 0x1337, Notify | Write | Write Without Response */
    {
        ATTRIB_FLAG_VALUE_INCL,
        {
            LO_WORD(GATT_UUID_CHARACTERISTIC),
            HI_WORD(GATT_UUID_CHARACTERISTIC),
            (GATT_CHAR_PROP_NOTIFY | GATT_CHAR_PROP_WRITE | GATT_CHAR_PROP_WRITE_NO_RSP)
        },
        1,
        NULL,
        GATT_PERM_READ
    },
    {
        ATTRIB_FLAG_VALUE_APPL,
        {
            LO_WORD(GATT_UUID_CHAR_CUSTOM),
            HI_WORD(GATT_UUID_CHAR_CUSTOM)
        },
        0,
        NULL,
        GATT_PERM_WRITE
    },
    /* CCCD: 0x2902 */
    {
        ATTRIB_FLAG_VALUE_INCL | ATTRIB_FLAG_CCCD_APPL,
        {
            LO_WORD(GATT_UUID_CHAR_CLIENT_CONFIG),
            HI_WORD(GATT_UUID_CHAR_CLIENT_CONFIG),
            LO_WORD(GATT_CLIENT_CHAR_CONFIG_DEFAULT),
            HI_WORD(GATT_CLIENT_CHAR_CONFIG_DEFAULT)
        },
        2,
        NULL,
        (GATT_PERM_READ | GATT_PERM_WRITE)
    }
};

T_APP_RESULT custom_service_attr_read_cb(uint8_t conn_id, T_SERVER_ID service_id,
                                         uint16_t attrib_index, uint16_t offset, uint16_t *p_length, uint8_t **pp_value)
{
    /* Nothing special to read in this service besides descriptors handled by stack */
    return APP_RESULT_SUCCESS;
}

T_APP_RESULT custom_service_attr_write_cb(uint8_t conn_id, T_SERVER_ID service_id,
                                          uint16_t attrib_index, T_WRITE_TYPE write_type, uint16_t length, uint8_t *p_value,
                                          P_FUN_WRITE_IND_POST_PROC *p_write_ind_post_proc)
{
    T_APP_RESULT cause = APP_RESULT_SUCCESS;
    if (CUSTOM_SERVICE_CHAR_NOTIFY_WRITE_INDEX == attrib_index)
    {
        T_CUSTOM_CALLBACK_DATA callback_data;
        callback_data.conn_id = conn_id;
        callback_data.type = CUSTOM_SERVICE_WRITE_MSG;
        callback_data.len = length;
        callback_data.p_value = p_value;

        if (pfn_custom_service_cb)
        {
            pfn_custom_service_cb(service_id, (void *)&callback_data);
        }
    }
    else
    {
        cause = APP_RESULT_ATTR_NOT_FOUND;
    }
    return cause;
}

void custom_service_cccd_update_cb(uint8_t conn_id, T_SERVER_ID service_id, uint16_t index,
                                   uint16_t cccbits)
{
    if (index == CUSTOM_SERVICE_CHAR_CCCD_INDEX)
    {
        T_CUSTOM_CALLBACK_DATA callback_data;
        callback_data.conn_id = conn_id;
        callback_data.p_value = NULL;
        callback_data.len = 0;

        if (cccbits & GATT_CLIENT_CHAR_CONFIG_NOTIFY)
        {
            callback_data.type = CUSTOM_SERVICE_NOTIFY_ENABLE;
        }
        else
        {
            callback_data.type = CUSTOM_SERVICE_NOTIFY_DISABLE;
        }

        if (pfn_custom_service_cb)
        {
            pfn_custom_service_cb(service_id, (void *)&callback_data);
        }
    }
}

const T_FUN_GATT_SERVICE_CBS custom_service_cbs =
{
    custom_service_attr_read_cb,
    custom_service_attr_write_cb,
    custom_service_cccd_update_cb
};

T_SERVER_ID custom_service_add_service(void *p_func)
{
    if (false == server_add_service(&custom_service_id,
                                    (uint8_t *)custom_service_tbl,
                                    sizeof(custom_service_tbl),
                                    custom_service_cbs))
    {
        custom_service_id = 0xff;
        return custom_service_id;
    }

    pfn_custom_service_cb = (P_FUN_SERVER_GENERAL_CB)p_func;
    return custom_service_id;
}

bool custom_service_send_notification(uint8_t conn_id, T_SERVER_ID service_id, void *p_value, uint16_t length)
{
    return server_send_data(conn_id, service_id, CUSTOM_SERVICE_CHAR_NOTIFY_WRITE_INDEX, p_value, length, GATT_PDU_TYPE_ANY);
}
