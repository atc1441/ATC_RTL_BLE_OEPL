/*********************************************************************************************************
*               Copyright(c) 2017, Realtek Semiconductor Corporation. All rigbcs reserved.
**********************************************************************************************************
* @file     bcs.c
* @brief    Body Composition Service source file.
* @details  Interfaces to access Body Composition Service.
* @author
* @date     2017-9-21
* @version  v1.0
*********************************************************************************************************
*/
#include "stdint.h"
#include "gatt.h"
#include <string.h>
#include "trace.h"
#include "profile_server.h"
#include "bcs.h"


/********************************************************************************************************
* local static variables defined here, only used in this source file.
********************************************************************************************************/
#define BCS_BODY_COMPOSITION_FEATURE_INDEX                       2
#define BCS_BODY_COMPOSITION_MEASUREMENT_INDEX                   4
#define BCS_BODY_COMPOSITION_MEASUREMENT_CCCD_INDEX              5

T_BODY_COMPOSITION_FEATURE bcs_body_composition_feature = {0};
static uint8_t bcs_measurement_value_for_indicate[BCS_MEASUREMENT_VALUE_MAX_LEN] = {0};
static uint8_t bcs_measurement_remain_value_for_indicate[10] = {0};

static uint8_t bcs_measurement_value_actual_length = 0;
static uint8_t bcs_measurement_value_first_length = 0;
static uint8_t bcs_measurement_value_second_length = 0;
static uint8_t bcs_measurement_value_consecutive_flag = 0;

static uint8_t report_mtu = 20;

/**<  Function pointer used to send event to application from location and navigation profile. */
static P_FUN_SERVER_GENERAL_CB pfn_bcs_cb = NULL;

/**< @brief  profile/service definition.  */
static const T_ATTRIB_APPL bcs_attr_tbl[] =
{
    /*----------------- Body Composition Service -------------------*/
    /* <<Primary Service>>, .. 0,*/
    {
        (ATTRIB_FLAG_VALUE_INCL | ATTRIB_FLAG_LE),   /* wFlags     */
        {                                           /* bTypeValue */
            LO_WORD(GATT_UUID_PRIMARY_SERVICE),
            HI_WORD(GATT_UUID_PRIMARY_SERVICE),
            LO_WORD(GATT_UUID_BODY_COMPOSITION),              /* service UUID */
            HI_WORD(GATT_UUID_BODY_COMPOSITION)
        },
        UUID_16BIT_SIZE,                            /* bValueLen     */
        NULL,                                       /* pValueContext */
        GATT_PERM_READ                              /* wPermissions  */
    },

    /* <<Characteristic>>, .. 1,*/
    {
        ATTRIB_FLAG_VALUE_INCL,                     /* wFlags */
        {                                           /* bTypeValue */
            LO_WORD(GATT_UUID_CHARACTERISTIC),
            HI_WORD(GATT_UUID_CHARACTERISTIC),
            (GATT_CHAR_PROP_READ/* characteristic properties */
            )
            /* characteristic UUID not needed here, is UUID of next attrib. */
        },
        1,                                          /* bValueLen */
        NULL,
        GATT_PERM_READ                              /* wPermissions */
    },
    /* Body Composition Feature 2,*/
    {
        ATTRIB_FLAG_VALUE_APPL,                     /* wFlags */
        {                                           /* bTypeValue */
            LO_WORD(GATT_UUID_CHAR_BODY_COMPOSITION_FEATURE),
            HI_WORD(GATT_UUID_CHAR_BODY_COMPOSITION_FEATURE)
        },
        0,                                          /* bValueLen */
        NULL,
        GATT_PERM_READ/* wPermissions */
    },

    /* <<Characteristic>>, .. 3,*/
    {
        ATTRIB_FLAG_VALUE_INCL,                     /* wFlags */
        {                                           /* bTypeValue */
            LO_WORD(GATT_UUID_CHARACTERISTIC),
            HI_WORD(GATT_UUID_CHARACTERISTIC),
            (GATT_CHAR_PROP_INDICATE/* characteristic properties */
            )
            /* characteristic UUID not needed here, is UUID of next attrib. */
        },
        1,                                          /* bValueLen */
        NULL,
        GATT_PERM_READ                              /* wPermissions */
    },
    /* Body Composition Measurement 4,*/
    {
        ATTRIB_FLAG_VALUE_APPL,                     /* wFlags */
        {                                           /* bTypeValue */
            LO_WORD(GATT_UUID_CHAR_BODY_COMPOSITION_MEASUREMENT),
            HI_WORD(GATT_UUID_CHAR_BODY_COMPOSITION_MEASUREMENT)
        },
        0,                                          /* bValueLen */
        NULL,
        GATT_PERM_NOTIF_IND                              /* wPermissions */
    },
    /* client characteristic configuration 5,*/
    {
        ATTRIB_FLAG_VALUE_INCL | ATTRIB_FLAG_CCCD_APPL,                   /* flags */
        {                                           /* type_value */
            LO_WORD(GATT_UUID_CHAR_CLIENT_CONFIG),
            HI_WORD(GATT_UUID_CHAR_CLIENT_CONFIG),
            /* NOTE: this value has an instantiation for each client, a write to */
            /* this attribute does not modify this default value:                */
            LO_WORD(GATT_CLIENT_CHAR_CONFIG_DEFAULT), /* client char. config. bit field */
            HI_WORD(GATT_CLIENT_CHAR_CONFIG_DEFAULT)
        },
        2,                                          /* bValueLen */
        NULL,
        (GATT_PERM_READ | GATT_PERM_WRITE)          /* permissions */
    }
};
/**< @brief  Body Composition service size definition.  */
const static uint16_t bcs_attr_tbl_size = sizeof(bcs_attr_tbl);

bool bcs_set_parameter(T_BCS_PARAM_TYPE param_type, uint8_t len, void *p_value)
{
    bool ret = true;

    switch (param_type)
    {
    default:
        {
            ret = false;
            PROFILE_PRINT_ERROR0("bcs_set_parameter failed\n");
        }
        break;

    case BCS_PARAM_BODY_COMPOSITION_FEATURE:
        {
            if (len != sizeof(uint32_t))
            {
                ret = false;
            }
            else
            {
                memcpy(&bcs_body_composition_feature, p_value, len);
            }
        }
        break;
    }
    return ret;
}

void bcs_format_measurement_value(T_BCS_BODY_COMPOSITION_MEASUREMENT *p_data)
{
    uint8_t cur_offset = 2;
    uint8_t remain_cur_offset = 2;

    T_BODY_COMPOSITION_MEASUREMENT_FLAG flag = {0};
    T_BODY_COMPOSITION_MEASUREMENT_FLAG remain_flag = {0};

    flag.bcs_measurement_units_bit = p_data->bcs_measurement_flag.bcs_measurement_units_bit;
    remain_flag.bcs_measurement_units_bit = p_data->bcs_measurement_flag.bcs_measurement_units_bit;

    memcpy(&bcs_measurement_value_for_indicate[cur_offset], &p_data->body_fat_percentage, 2);
    cur_offset += 2;
    memcpy(&bcs_measurement_remain_value_for_indicate[remain_cur_offset], &p_data->body_fat_percentage,
           2);
    remain_cur_offset += 2;

    if (p_data->bcs_measurement_flag.bcs_time_stamp_present_bit &
        bcs_body_composition_feature.bcs_feature_time_stamp_support_bit & 1)
    {
        flag.bcs_time_stamp_present_bit = 1;
        memcpy(&bcs_measurement_value_for_indicate[cur_offset], &p_data->time_stamp, 7);
        cur_offset += 7;
    }

    if (p_data->bcs_measurement_flag.bcs_user_id_bit &
        bcs_body_composition_feature.bcs_feature_multiple_users_support_bit & 1)
    {
        flag.bcs_user_id_bit = 1;
        memcpy(&bcs_measurement_value_for_indicate[cur_offset], &p_data->user_id,
               1);
        cur_offset += 1;
    }

    if (p_data->bcs_measurement_flag.bcs_basal_metabolism_present_bit &
        bcs_body_composition_feature.bcs_feature_basal_metabolism_support_bit & 1)
    {
        flag.bcs_basal_metabolism_present_bit = 1;
        memcpy(&bcs_measurement_value_for_indicate[cur_offset], &p_data->basal_metabolism,
               2);
        cur_offset += 2;
    }

    if (p_data->bcs_measurement_flag.bcs_muscle_percentage_present_bit &
        bcs_body_composition_feature.bcs_feature_muscle_percentage_support_bit & 1)
    {
        flag.bcs_muscle_percentage_present_bit = 1;
        memcpy(&bcs_measurement_value_for_indicate[cur_offset], &p_data->muscle_percentage,
               2);
        cur_offset += 2;
    }

    if (p_data->bcs_measurement_flag.bcs_muscle_mass_present_bit &
        bcs_body_composition_feature.bcs_feature_muscle_mass_support_bit & 1)
    {
        flag.bcs_muscle_mass_present_bit = 1;
        memcpy(&bcs_measurement_value_for_indicate[cur_offset], &p_data->muscle_mass,
               2);
        cur_offset += 2;
    }

    if (p_data->bcs_measurement_flag.bcs_fat_free_mass_present_bit &
        bcs_body_composition_feature.bcs_feature_fat_free_mass_support_bit & 1)
    {
        flag.bcs_fat_free_mass_present_bit = 1;
        memcpy(&bcs_measurement_value_for_indicate[cur_offset], &p_data->fat_free_mass,
               2);
        cur_offset += 2;
    }
    bcs_measurement_value_first_length = cur_offset;

    if (p_data->bcs_measurement_flag.bcs_soft_lean_mass_present_bit &
        bcs_body_composition_feature.bcs_feature_soft_lean_mass_support_bit & 1)
    {
        flag.bcs_soft_lean_mass_present_bit = 1;
        memcpy(&bcs_measurement_value_for_indicate[cur_offset], &p_data->soft_lean_mass,
               2);
        cur_offset += 2;
        bcs_measurement_value_first_length += 2;
    }

    if (cur_offset > report_mtu)
    {
        bcs_measurement_value_first_length -= 2;
        flag.bcs_soft_lean_mass_present_bit = 0;
        remain_flag.bcs_soft_lean_mass_present_bit = 1;
        memcpy(&bcs_measurement_remain_value_for_indicate[remain_cur_offset], &p_data->soft_lean_mass,
               2);
        remain_cur_offset += 2;
    }

    if (p_data->bcs_measurement_flag.bcs_body_water_mass_present_bit &
        bcs_body_composition_feature.bcs_feature_body_water_mass_support_bit & 1)
    {
        flag.bcs_body_water_mass_present_bit = 1;
        memcpy(&bcs_measurement_value_for_indicate[cur_offset], &p_data->body_water_mass,
               2);
        cur_offset += 2;
        bcs_measurement_value_first_length += 2;
    }

    if (cur_offset > report_mtu)
    {
        bcs_measurement_value_first_length -= 2;
        flag.bcs_body_water_mass_present_bit = 0;
        remain_flag.bcs_body_water_mass_present_bit = 1;
        memcpy(&bcs_measurement_remain_value_for_indicate[remain_cur_offset], &p_data->body_water_mass,
               2);
        remain_cur_offset += 2;
    }

    if (p_data->bcs_measurement_flag.bcs_impedance_present_bit &
        bcs_body_composition_feature.bcs_feature_impedance_support_bit & 1)
    {
        flag.bcs_impedance_present_bit = 1;
        memcpy(&bcs_measurement_value_for_indicate[cur_offset], &p_data->impedance,
               2);
        cur_offset += 2;
        bcs_measurement_value_first_length += 2;
    }

    if (cur_offset > report_mtu)
    {
        bcs_measurement_value_first_length -= 2;
        flag.bcs_impedance_present_bit = 0;
        remain_flag.bcs_impedance_present_bit = 1;
        memcpy(&bcs_measurement_remain_value_for_indicate[remain_cur_offset], &p_data->impedance,
               2);
        remain_cur_offset += 2;
    }

    if (p_data->bcs_measurement_flag.bcs_weight_present_bit &
        bcs_body_composition_feature.bcs_feature_weight_support_bit & 1)
    {
        flag.bcs_weight_present_bit = 1;
        memcpy(&bcs_measurement_value_for_indicate[cur_offset], &p_data->weight,
               2);
        cur_offset += 2;
        bcs_measurement_value_first_length += 2;
    }

    if (cur_offset > report_mtu)
    {
        bcs_measurement_value_first_length -= 2;
        flag.bcs_weight_present_bit = 0;
        remain_flag.bcs_weight_present_bit = 1;
        memcpy(&bcs_measurement_remain_value_for_indicate[remain_cur_offset], &p_data->weight,
               2);
        remain_cur_offset += 2;
    }

    if (p_data->bcs_measurement_flag.bcs_height_present_bit &
        bcs_body_composition_feature.bcs_feature_height_support_bit & 1)
    {
        flag.bcs_height_present_bit = 1;
        memcpy(&bcs_measurement_value_for_indicate[cur_offset], &p_data->height,
               2);
        cur_offset += 2;
        bcs_measurement_value_first_length += 2;
    }

    if (cur_offset > report_mtu)
    {
        bcs_measurement_value_first_length -= 2;
        flag.bcs_height_present_bit = 0;
        remain_flag.bcs_height_present_bit = 1;
        memcpy(&bcs_measurement_remain_value_for_indicate[remain_cur_offset], &p_data->height,
               2);
        remain_cur_offset += 2;
    }

    if (cur_offset > report_mtu)
    {
        bcs_measurement_value_consecutive_flag = 1;
        flag.bcs_multiple_packet_measurement_bit = 1;
        remain_flag.bcs_multiple_packet_measurement_bit = 1;
    }

    memcpy(&bcs_measurement_value_for_indicate, &flag, 2);
    memcpy(&bcs_measurement_remain_value_for_indicate, &remain_flag, 2);

    bcs_measurement_value_actual_length = cur_offset;
    bcs_measurement_value_second_length = remain_cur_offset;
}

bool bcs_body_composition_measurement_value_indicate(uint8_t conn_id, T_SERVER_ID service_id,
                                                     T_BCS_BODY_COMPOSITION_MEASUREMENT *p_data)
{
    APP_PRINT_INFO0("bcs_body_composition_measurement_value_indicate");
    bcs_format_measurement_value(p_data);

    if (bcs_measurement_value_consecutive_flag)
    {
        return server_send_data(conn_id, service_id, BCS_BODY_COMPOSITION_MEASUREMENT_INDEX,
                                (uint8_t *)&bcs_measurement_value_for_indicate,
                                bcs_measurement_value_first_length, GATT_PDU_TYPE_INDICATION);
    }

    return server_send_data(conn_id, service_id, BCS_BODY_COMPOSITION_MEASUREMENT_INDEX,
                            (uint8_t *)&bcs_measurement_value_for_indicate,
                            bcs_measurement_value_actual_length, GATT_PDU_TYPE_INDICATION);
}

bool bcs_measurement_value_consecutive_indicate(uint8_t conn_id, T_SERVER_ID service_id)
{
    uint8_t ret = false;
    if (bcs_measurement_value_consecutive_flag)
    {
        APP_PRINT_INFO0("bcs_measurement_value_consecutive_indicate: send remain data");

        server_send_data(conn_id, service_id, BCS_BODY_COMPOSITION_MEASUREMENT_INDEX,
                         (uint8_t *)&bcs_measurement_remain_value_for_indicate,
                         bcs_measurement_value_second_length, GATT_PDU_TYPE_INDICATION);

        bcs_measurement_value_consecutive_flag = 0;
        ret = true;
    }
    return ret;
}

/**
 * @brief read characteristic data from service.
 *
 * @param[in] conn_id       Connection id.
 * @param[in] service_id    Service ID.
 * @param[in] attrib_index          Attribute index of getting characteristic data.
 * @param[in] offset                Used for Blob Read.
 * @param[in,out] p_length            length of getting characteristic data.
 * @param[in,out] pp_value            data got from service.
 * @return Profile procedure result
*/
T_APP_RESULT bcs_attr_read_cb(uint8_t conn_id, T_SERVER_ID service_id, uint16_t attrib_index,
                              uint16_t offset, uint16_t *p_length, uint8_t **pp_value)
{
    T_APP_RESULT cause = APP_RESULT_SUCCESS;
    *p_length = 0;
    T_BCS_CALLBACK_DATA callback_data;
    callback_data.msg_type = SERVICE_CALLBACK_TYPE_READ_CHAR_VALUE;

    PROFILE_PRINT_INFO2("bcs_attr_read_cb: attrib_index %d offset %d", attrib_index, offset);

    switch (attrib_index)
    {
    default:
        {
            PROFILE_PRINT_ERROR0("bcs_attr_read_cb: unknown attrib_index");
            cause  = APP_RESULT_ATTR_NOT_FOUND;
        }
        break;

    case BCS_BODY_COMPOSITION_FEATURE_INDEX:
        {
            callback_data.conn_id = conn_id;
            cause = pfn_bcs_cb(service_id, (void *)&callback_data);

            *pp_value = (uint8_t *)&bcs_body_composition_feature;
            *p_length = sizeof(bcs_body_composition_feature);
        }
        break;
    }
    return (cause);
}

/**
 * @brief update CCCD bits from stack.
 *
 * @param[in] conn_id       Connection id.
 * @param[in] service_id          Service ID.
 * @param[in] index          Attribute index of characteristic data.
 * @param[in] ccc_bits         CCCD bits from stack.
 * @return None
*/
void bcs_cccd_update_cb(uint8_t conn_id, T_SERVER_ID service_id, uint16_t index, uint16_t ccc_bits)
{
    T_BCS_CALLBACK_DATA callback_data;
    callback_data.msg_type = SERVICE_CALLBACK_TYPE_INDIFICATION_NOTIFICATION;
    callback_data.conn_id = conn_id;
    bool handle = true;
    PROFILE_PRINT_INFO2("bcs_cccd_update_cb: index %d ccc_bits 0x%04x", index, ccc_bits);

    switch (index)
    {
    case BCS_BODY_COMPOSITION_MEASUREMENT_CCCD_INDEX:
        {
            if (ccc_bits & GATT_CLIENT_CHAR_CONFIG_INDICATE)
            {
                callback_data.msg_data.notification_indification_index =
                    BCS_INDICATE_BODY_COMPOSITIONT_MEASUREMENT_ENABLE;
            }
            else
            {
                callback_data.msg_data.notification_indification_index =
                    BCS_INDICATE_BODY_COMPOSITIONT_MEASUREMENT_DISABLE;
            }
            break;
        }

    default:
        {
            handle = false;
            break;
        }
    }

    if (pfn_bcs_cb && (handle == true))
    {
        pfn_bcs_cb(service_id, (void *)&callback_data);
    }

    return;
}

/**
 * @brief Body Composition Service Callbacks.
*/
const T_FUN_GATT_SERVICE_CBS bcs_cbs =
{
    bcs_attr_read_cb,   // Read callback function pointer
    NULL,               // Write callback function pointer
    bcs_cccd_update_cb  // CCCD update callback function pointer
};

T_SERVER_ID bcs_add_service(void *p_func)
{
    T_SERVER_ID service_id;
    if (false == server_add_service(&service_id,
                                    (uint8_t *)bcs_attr_tbl,
                                    bcs_attr_tbl_size,
                                    bcs_cbs))
    {
        PROFILE_PRINT_ERROR1("bcs_add_service: ServiceId %d", service_id);
        service_id = 0xff;
    }
    pfn_bcs_cb = (P_FUN_SERVER_GENERAL_CB)p_func;

    return service_id;
}
