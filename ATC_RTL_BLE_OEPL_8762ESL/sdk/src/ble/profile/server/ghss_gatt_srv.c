/****************************************************************************************
*     Copyright(c) 2017, Realtek Semiconductor Corporation. All rigghss reserved.
*****************************************************************************************

  * @file     ghss.c
  * @brief    Generic Health Sensor Service source file.
  * @details  Interface to access the Generic Health Sensor Service.
  * @author
  * @date
  * @version  v1.0
  * *************************************************************************************
  */
#include "trace.h"
#include <string.h>
#include "profile_server.h"
#include "ghss_gatt_srv.h"
#include "bt_types.h"

/********************************************************************************************************
* local static variables defined here, only used in this source file.
********************************************************************************************************/

typedef struct
{
    uint8_t ghss_live_obs_notify_indicate_enable: 1;
    uint8_t ghss_stored_obs_notify_indicate_enable: 1;
    uint8_t ghss_racp_indicate_enable: 1;
    uint8_t ghss_ghs_cp_indicate_enable: 1;
    uint8_t rfu: 4;
} T_GHSS_NOTIFY_INDICATE_FLAG;

/**<  Function pointer used to send event to application from GHSS profile. */
static P_FUN_GHSS_SERVER_APP_CB pfn_ghss_cb = NULL;

T_GHSS_NOTIFY_INDICATE_FLAG ghss_notify_indicate_flag = {0};

/** @brief  profile/service definition.  */
const T_ATTRIB_APPL ghss_attr_tbl[] =
{
    /* <<Primary Service>> */
    {
        (ATTRIB_FLAG_VALUE_INCL | ATTRIB_FLAG_LE),  /* wFlags     */
        {                                           /* bTypeValue */
            LO_WORD(GATT_UUID_PRIMARY_SERVICE),
            HI_WORD(GATT_UUID_PRIMARY_SERVICE),
            LO_WORD(GATT_UUID_GENERIC_HEALTH_SENSOR),      /* service UUID */
            HI_WORD(GATT_UUID_GENERIC_HEALTH_SENSOR)
        },
        UUID_16BIT_SIZE,                            /* bValueLen     */
        NULL,                                       /* pValueContext */
        GATT_PERM_READ                              /* wPermissions  */
    },
    /* <<Characteristic>> */
    {
        ATTRIB_FLAG_VALUE_INCL,                     /* wFlags */
        {                                           /* bTypeValue */
            LO_WORD(GATT_UUID_CHARACTERISTIC),
            HI_WORD(GATT_UUID_CHARACTERISTIC),
#if (GHSS_CHAR_LIVE_GHSS_HEALTH_OBS_PROP_NOTIFY_SUPPORT && GHSS_CHAR_LIVE_GHSS_HEALTH_OBS_PROP_INDICATE_SUPPORT)
            GATT_CHAR_PROP_NOTIFY | GATT_CHAR_PROP_INDICATE
#elif GHSS_CHAR_LIVE_GHSS_HEALTH_OBS_PROP_NOTIFY_SUPPORT
            GATT_CHAR_PROP_NOTIFY
#elif GHSS_CHAR_LIVE_GHSS_HEALTH_OBS_PROP_INDICATE_SUPPORT
            GATT_CHAR_PROP_INDICATE
#endif
            /* characteristic properties */
            /* characteristic UUID not needed here, is UUID of next attrib. */
        },
        1,                                          /* bValueLen */
        NULL,
        GATT_PERM_READ                              /* wPermissions */
    },
    /*--- Live Health Observations characteristic value */
    {
        ATTRIB_FLAG_VALUE_APPL,                     /* wFlags */
        {                                           /* bTypeValue */
            LO_WORD(GATT_UUID_CHAR_LIVE_HEALTH_OBSERVATIONS),
            HI_WORD(GATT_UUID_CHAR_LIVE_HEALTH_OBSERVATIONS)
        },
        0,                                          /* bValueLen */
        NULL,
        GATT_PERM_NONE                              /* wPermissions */
    },
    /* client characteristic configuration */
    {
        (ATTRIB_FLAG_VALUE_INCL |                   /* wFlags */
         ATTRIB_FLAG_CCCD_APPL),
        {                                           /* bTypeValue */
            LO_WORD(GATT_UUID_CHAR_CLIENT_CONFIG),
            HI_WORD(GATT_UUID_CHAR_CLIENT_CONFIG),
            /* NOTE: this value has an instantiation for each client, a write to */
            /* this attribute does not modify this default value:                */
            LO_WORD(GATT_CLIENT_CHAR_CONFIG_DEFAULT), /* client char. config. bit field */
            HI_WORD(GATT_CLIENT_CHAR_CONFIG_DEFAULT)
        },
        2,                                          /* bValueLen */
        NULL,
        (GATT_PERM_READ | GATT_PERM_WRITE)          /* wPermissions */
    },
    /* <<Characteristic>> */
    {
        ATTRIB_FLAG_VALUE_INCL,                     /* wFlags */
        {                                           /* bTypeValue */
            LO_WORD(GATT_UUID_CHARACTERISTIC),
            HI_WORD(GATT_UUID_CHARACTERISTIC),
#if (GHSS_CHAR_STORED_GHSS_HEALTH_OBS_PROP_NOTIFY_SUPPORT && GHSS_CHAR_STORED_GHSS_HEALTH_OBS_PROP_INDICATE_SUPPORT)
            GATT_CHAR_PROP_NOTIFY | GATT_CHAR_PROP_INDICATE
#elif GHSS_CHAR_STORED_GHSS_HEALTH_OBS_PROP_NOTIFY_SUPPORT
            GATT_CHAR_PROP_NOTIFY
#elif GHSS_CHAR_STORED_GHSS_HEALTH_OBS_PROP_INDICATE_SUPPORT
            GATT_CHAR_PROP_INDICATE
#endif
            /* characteristic properties */
            /* characteristic UUID not needed here, is UUID of next attrib. */
        },
        1,                                          /* bValueLen */
        NULL,
        GATT_PERM_READ                              /* wPermissions */
    },
    /*--- Stored Health Observations characteristic value */
    {
        ATTRIB_FLAG_VALUE_APPL,                     /* wFlags */
        {                                           /* bTypeValue */
            LO_WORD(GATT_UUID_CHAR_STORED_HEALTH_OBSERVATIONS),
            HI_WORD(GATT_UUID_CHAR_STORED_HEALTH_OBSERVATIONS)
        },
        0,                                          /* bValueLen */
        NULL,
        GATT_PERM_NONE                              /* wPermissions */
    },
    /* client characteristic configuration */
    {
        (ATTRIB_FLAG_VALUE_INCL |                   /* wFlags */
         ATTRIB_FLAG_CCCD_APPL),
        {                                           /* bTypeValue */
            LO_WORD(GATT_UUID_CHAR_CLIENT_CONFIG),
            HI_WORD(GATT_UUID_CHAR_CLIENT_CONFIG),
            /* NOTE: this value has an instantiation for each client, a write to */
            /* this attribute does not modify this default value:                */
            LO_WORD(GATT_CLIENT_CHAR_CONFIG_DEFAULT), /* client char. config. bit field */
            HI_WORD(GATT_CLIENT_CHAR_CONFIG_DEFAULT)
        },
        2,                                          /* bValueLen */
        NULL,
        (GATT_PERM_READ | GATT_PERM_WRITE)          /* wPermissions */
    },
    /* <<Characteristic>> */
    {
        ATTRIB_FLAG_VALUE_INCL,                     /* wFlags */
        {                                           /* bTypeValue */
            LO_WORD(GATT_UUID_CHARACTERISTIC),
            HI_WORD(GATT_UUID_CHARACTERISTIC),
            GATT_CHAR_PROP_WRITE | GATT_CHAR_PROP_INDICATE  /* characteristic properties */
            /* characteristic UUID not needed here, is UUID of next attrib. */
        },
        1,                                          /* bValueLen */
        NULL,
        GATT_PERM_READ                              /* wPermissions */
    },
    /*--- Record Access Control Point characteristic value */
    {
        ATTRIB_FLAG_VALUE_APPL,                     /* wFlags */
        {                                           /* bTypeValue */
            LO_WORD(GATT_UUID_CHAR_RECORD_ACCESS_CONTROL_POINT),
            HI_WORD(GATT_UUID_CHAR_RECORD_ACCESS_CONTROL_POINT)
        },
        0,                                          /* bValueLen */
        NULL,
        (GATT_PERM_READ | GATT_PERM_WRITE)         /* wPermissions */
    },
    /* client characteristic configuration */
    {
        (ATTRIB_FLAG_VALUE_INCL |                   /* wFlags */
         ATTRIB_FLAG_CCCD_APPL),
        {                                           /* bTypeValue */
            LO_WORD(GATT_UUID_CHAR_CLIENT_CONFIG),
            HI_WORD(GATT_UUID_CHAR_CLIENT_CONFIG),
            /* NOTE: this value has an instantiation for each client, a write to */
            /* this attribute does not modify this default value:                */
            LO_WORD(GATT_CLIENT_CHAR_CONFIG_DEFAULT), /* client char. config. bit field */
            HI_WORD(GATT_CLIENT_CHAR_CONFIG_DEFAULT)
        },
        2,                                          /* bValueLen */
        NULL,
        (GATT_PERM_READ | GATT_PERM_WRITE)          /* wPermissions */
    },
    /* <<Characteristic>> */
    {
        ATTRIB_FLAG_VALUE_INCL,                     /* wFlags */
        {                                           /* bTypeValue */
            LO_WORD(GATT_UUID_CHARACTERISTIC),
            HI_WORD(GATT_UUID_CHARACTERISTIC),
            GATT_CHAR_PROP_WRITE | GATT_CHAR_PROP_INDICATE  /* characteristic properties */
            /* characteristic UUID not needed here, is UUID of next attrib. */
        },
        1,                                          /* bValueLen */
        NULL,
        GATT_PERM_READ                              /* wPermissions */
    },
    /*--- Health Sensor Features characteristic value */
    {
        ATTRIB_FLAG_VALUE_APPL,                     /* wFlags */
        {                                           /* bTypeValue */
            LO_WORD(GATT_UUID_CHAR_GHS_CONTROL_POINT),
            HI_WORD(GATT_UUID_CHAR_GHS_CONTROL_POINT)
        },
        0,                                          /* bValueLen */
        NULL,
        (GATT_PERM_READ | GATT_PERM_WRITE)          /* wPermissions */
    },
    /* client characteristic configuration */
    {
        (ATTRIB_FLAG_VALUE_INCL |                   /* wFlags */
         ATTRIB_FLAG_CCCD_APPL),
        {                                           /* bTypeValue */
            LO_WORD(GATT_UUID_CHAR_CLIENT_CONFIG),
            HI_WORD(GATT_UUID_CHAR_CLIENT_CONFIG),
            /* NOTE: this value has an instantiation for each client, a write to */
            /* this attribute does not modify this default value:                */
            LO_WORD(GATT_CLIENT_CHAR_CONFIG_DEFAULT), /* client char. config. bit field */
            HI_WORD(GATT_CLIENT_CHAR_CONFIG_DEFAULT)
        },
        2,                                          /* bValueLen */
        NULL,
        (GATT_PERM_READ | GATT_PERM_WRITE)          /* wPermissions */
    }
};

/**< @brief  Generic Health Sensor Service size definition.  */
const uint16_t ghss_char_num = sizeof(ghss_attr_tbl) / sizeof(T_ATTRIB_APPL);

uint16_t ghss_format_health_obs_body_optional_value_length(T_GHSS_HEALTH_OBS_BODY *p_value)
{
    uint16_t val_len = 0;
    T_GHSS_HOB_FLAGS flags = p_value->flags;

    // Mandatory for all single observations but not for observation bundles.
    if (flags.observation_type_present)
    {
        val_len += 4;
    }

    // Stored observations shall have a time stamp. Live observations optional.
    if (flags.time_stamp_present)
    {
        val_len += (1 + 6 + 1 + 1);
    }

    if (flags.measurement_duration_present)
    {
        val_len += 4;
    }

    if (flags.measurement_status_present)
    {
        val_len += 2;
    }

    if (flags.observation_id_present)
    {
        val_len += 4;
    }

    if (flags.patient_present)
    {
        val_len += 1;
    }

    if (flags.supplemental_information_present)
    {
        uint8_t count = p_value->suppl_info.count;
        val_len += 1;

        if (count)
        {
            val_len += (4 * count);
        }
        else
        {
            APP_PRINT_ERROR1("ghss_format_health_obs_body_optional_value_length: Invalid supplemental information count %d",
                             count);
            return 0;
        }
    }

    if (flags.derived_from_present)
    {
        uint8_t count = p_value->derived_from.count;
        val_len += 1;

        if (count)
        {
            val_len += (4 * count);
        }
        else
        {
            APP_PRINT_ERROR1("ghss_format_health_obs_body_optional_value_length: Invalid derived from count %d",
                             count);
            return 0;
        }
    }

    if (flags.is_member_of_present)
    {
        uint8_t count = p_value->is_mem_of.count;
        val_len += 1;

        if (count)
        {
            val_len += (4 * count);
        }
        else
        {
            APP_PRINT_ERROR1("ghss_format_health_obs_body_optional_value_length: Invalid is member of count %d",
                             count);
            return 0;
        }
    }

    if (flags.tlvs_present)
    {
        uint8_t num_of_tlv_attr = p_value->tlvs.num_of_tlv_attr;
        val_len += 1;

        for (uint8_t i = 0; i < num_of_tlv_attr; i++)
        {
            val_len += (4 + 2 + 1);
            uint16_t length = p_value->tlvs.p_tlvs_value[i].length;
            val_len += length;
        }
    }

    return val_len;
}

uint16_t ghss_format_health_obs_body_obs_value_length(T_GHSS_HOB_CLASS_TYPE type,
                                                      T_GHSS_HOB_VALUE *p_value)
{
    uint16_t val_len = 0;

    // Mandatory part
    switch (type)
    {
    case GHSS_HOB_CLASS_TYPE_NUMERIC_OBSERVATION:
        {
            val_len += (2 + 4);
        }
        break;

    case GHSS_HOB_CLASS_TYPE_SIMPLE_DISCRETE_OBSERVATION:
        {
            val_len += 4;
        }
        break;

    case GHSS_HOB_CLASS_TYPE_STRING_OBSERVATION:
        {
            T_GHSS_HOBV_STRING_OBS *p_obs_value = &p_value->string_obs;
            uint16_t length = p_obs_value->length;
            val_len += (2 + length);
        }
        break;

    case GHSS_HOB_CLASS_TYPE_SAMPLE_ARRAY_OBSERVATION:
        {
            T_GHSS_HOBV_SAMPLE_ARRAY_OBS *p_obs_value = &p_value->sample_array_obs;
            val_len += (2 + 4 + 4 + 4 + 1 + 1 + 4);
            uint8_t bytes = p_obs_value->bytes_per_period;
            uint32_t number = p_obs_value->number_of_samples;
            val_len += (bytes * number);
        }
        break;

    case GHSS_HOB_CLASS_TYPE_COMPOUND_DISCRETE_EVENT_OBSERVATION:
        {
            T_GHSS_HOBV_COMPOUND_DISCRETE_EVENT_OBS *p_obs_value = &p_value->compound_discrete_event_obs;
            uint8_t number = p_obs_value->number_of_components;
            val_len += 1;
            val_len += (4 * number);
        }
        break;

    case GHSS_HOB_CLASS_TYPE_COMPOUND_STATE_EVENT_OBSERVATION:
        {
            T_GHSS_HOBV_COMPOUND_STATE_EVENT_OBS *p_obs_value = &p_value->compound_state_event_obs;
            uint8_t size = p_obs_value->size;
            val_len += 1;
            val_len += (3 * size);
        }
        break;

    case GHSS_HOB_CLASS_TYPE_TLV_ENCODED_OBSERVATION:   //only for obs_bdl_value
        {
            T_GHSS_HOB_TLVS *p_obs_value = &p_value->tlv_encoded_obs;
            uint8_t number =
                p_obs_value->num_of_tlv_attr;
            val_len += 1;
            for (uint8_t i = 0; i < number; i++)
            {
                val_len += (4 + 2 + 1);
                uint16_t length =
                    p_obs_value->p_tlvs_value[i].length;
                val_len += length;
            }
        }
        break;

    default:
        APP_PRINT_ERROR1("ghss_format_health_obs_body_obs_value_length: Invalid Observation Class Type %d",
                         type);
        return 0;
    }

    return val_len;
}

uint16_t ghss_format_health_obs_body_length(T_GHSS_HEALTH_OBS_BODY *p_value)
{
    uint16_t total_len = 0;
    uint16_t err_idx = 0;

    APP_PRINT_INFO1("ghss_format_health_obs_body_length: Observation Class Type %d",
                    p_value->obs_class_type);

    // Mandatory part: obs_class_type & flags
    total_len += (1 + 2);

    uint16_t value_len = ghss_format_health_obs_body_optional_value_length(p_value);
    if (value_len == 0)
    {
        err_idx = 1;
        goto error;
    }
    total_len += value_len;

    // Mandatory part
    switch (p_value->obs_class_type)
    {
    case GHSS_HOB_CLASS_TYPE_NUMERIC_OBSERVATION:
    case GHSS_HOB_CLASS_TYPE_SIMPLE_DISCRETE_OBSERVATION:
    case GHSS_HOB_CLASS_TYPE_STRING_OBSERVATION:
    case GHSS_HOB_CLASS_TYPE_SAMPLE_ARRAY_OBSERVATION:
    case GHSS_HOB_CLASS_TYPE_COMPOUND_DISCRETE_EVENT_OBSERVATION:
    case GHSS_HOB_CLASS_TYPE_COMPOUND_STATE_EVENT_OBSERVATION:
    case GHSS_HOB_CLASS_TYPE_TLV_ENCODED_OBSERVATION:   //only for obs_bdl_value
        {
            value_len = ghss_format_health_obs_body_obs_value_length(p_value->obs_class_type,
                                                                     &p_value->obs_value);
            if (value_len == 0)
            {
                err_idx = 2;
                goto error;
            }
            total_len += value_len;
        }
        break;

    case GHSS_HOB_CLASS_TYPE_COMPOUND_OBSERVATION:
        {
            uint8_t number =
                p_value->obs_value.compound_obs.number_of_components;
            total_len += 1;

            for (uint8_t i = 0; i < number; i++)
            {
                total_len += (4 + 1);
                T_GHSS_HOB_VALUE *p_obs_value = (T_GHSS_HOB_VALUE *)
                                                p_value->obs_value.compound_obs.p_component_value[i].p_value;
                T_GHSS_HOB_CLASS_TYPE obs_type =
                    p_value->obs_value.compound_obs.p_component_value[i].component_value_type;
                value_len = ghss_format_health_obs_body_obs_value_length(obs_type, p_obs_value);
                if (value_len == 0)
                {
                    err_idx = 3;
                    goto error;
                }
                total_len += value_len;
            }
        }
        break;

    case GHSS_HOB_CLASS_TYPE_OBSERVATION_BUNDLE:
        {
            uint8_t number = p_value->obs_value.obs_bundle.number_of_obs;
            total_len += 1;

            for (uint8_t n = 0; n < number; n++)
            {
                T_GHSS_HEALTH_OBS_BODY *p_obs_bdl_value = (T_GHSS_HEALTH_OBS_BODY *)
                                                          p_value->obs_value.obs_bundle.p_obs_bdl_value + n;
                // Mandatory part: obs_class_type & flags
                total_len += (1 + 2);

                value_len = ghss_format_health_obs_body_optional_value_length(p_obs_bdl_value);

                if (value_len == 0)
                {
                    err_idx = 4;
                    goto error;
                }
                total_len += value_len;

                T_GHSS_HOB_VALUE *p_obs_value = (T_GHSS_HOB_VALUE *)&p_obs_bdl_value->obs_value;

                if (p_obs_bdl_value->obs_class_type == GHSS_HOB_CLASS_TYPE_COMPOUND_OBSERVATION)
                {
                    uint8_t number =
                        p_obs_value->compound_obs.number_of_components;
                    total_len += 1;

                    for (uint8_t i = 0; i < number; i++)
                    {
                        total_len += (4 + 1);
                        T_GHSS_HOB_VALUE *p_obs_components_value = (T_GHSS_HOB_VALUE *)
                                                                   p_obs_value->compound_obs.p_component_value[i].p_value;
                        T_GHSS_HOB_CLASS_TYPE obs_components_type =
                            p_obs_value->compound_obs.p_component_value[i].component_value_type;
                        value_len = ghss_format_health_obs_body_obs_value_length(obs_components_type,
                                                                                 p_obs_components_value);
                        if (value_len == 0)
                        {
                            err_idx = 5;
                            goto error;
                        }
                        total_len += value_len;
                    }
                }
                else
                {
                    value_len = ghss_format_health_obs_body_obs_value_length(p_obs_bdl_value->obs_class_type,
                                                                             p_obs_value);
                    if (value_len == 0)
                    {
                        err_idx = 6;
                        goto error;
                    }
                    total_len += value_len;
                }
                total_len += 2; //Add uint16_t length
            }
        }
        break;

    default:
        err_idx = 7;
        goto error;
    }

    total_len += 2; //Add uint16_t length

    return total_len;

error:
    APP_PRINT_ERROR2("ghss_format_health_obs_body_obs_value_length: Observation Class Type %d, ERR IDX %d",
                     p_value->obs_class_type, err_idx);
    return 0;
}

uint16_t ghss_format_health_obs_body_optional_sending_value(T_GHSS_HEALTH_OBS_BODY *p_value,
                                                            uint16_t offset,
                                                            uint8_t **pp_temp_value)
{
    T_GHSS_HOB_FLAGS flags = p_value->flags;
    uint16_t current_offset = offset;

    // Mandatory for all single observations but not for observation bundles.
    if (flags.observation_type_present)
    {
        memcpy(&(*pp_temp_value)[current_offset],
               &p_value->obs_type, 4);
        current_offset += 4;
    }
    // Stored observations shall have a time stamp. Live observations optional.
    if (flags.time_stamp_present)
    {
        memcpy(&(*pp_temp_value)[current_offset],
               &p_value->time_stamp.flags, 1);
        current_offset += 1;

        current_offset += 6;
        for (uint8_t i = 6; i > 0; i--)
        {
            memcpy(&(*pp_temp_value)[current_offset - i],
                   &p_value->time_stamp.time_value[i - 1], 1);
        }

        memcpy(&(*pp_temp_value)[current_offset],
               &p_value->time_stamp.time_sync_source_type, 1);
        current_offset += 1;
        memcpy(&(*pp_temp_value)[current_offset],
               &p_value->time_stamp.tz_dst_offset, 1);
        current_offset += 1;
    }

    if (flags.measurement_duration_present)
    {
        memcpy(&(*pp_temp_value)[current_offset],
               &p_value->meas_duration, 4);
        current_offset += 4;
    }

    if (flags.measurement_status_present)
    {
        memcpy(&(*pp_temp_value)[current_offset],
               &p_value->meas_status, 2);
        current_offset += 2;
    }

    if (flags.observation_id_present)
    {
        memcpy(&(*pp_temp_value)[current_offset],
               &p_value->obs_id, 4);
        current_offset += 4;
    }

    if (flags.patient_present)
    {
        memcpy(&(*pp_temp_value)[current_offset],
               &p_value->patient, 1);
        current_offset += 1;
    }

    if (flags.supplemental_information_present)
    {
        uint8_t count = p_value->suppl_info.count;
        memcpy(&(*pp_temp_value)[current_offset],
               &p_value->suppl_info.count, 1);
        current_offset += 1;

        if (count)
        {
            memcpy(&(*pp_temp_value)[current_offset],
                   p_value->suppl_info.p_codes,
                   4 * count);
            current_offset += (4 * count);
        }
        else
        {
            APP_PRINT_ERROR1("ghss_format_health_obs_body_optional_sending_value: Invalid supplemental information count %d",
                             count);
            return 0;
        }
    }

    if (flags.derived_from_present)
    {
        uint8_t count = p_value->derived_from.count;
        memcpy(&(*pp_temp_value)[current_offset],
               &p_value->derived_from.count, 1);
        current_offset += 1;

        if (count)
        {
            memcpy(&(*pp_temp_value)[current_offset],
                   p_value->derived_from.p_obs_ids,
                   4 * count);
            current_offset += (4 * count);
        }
        else
        {
            APP_PRINT_ERROR1("ghss_format_health_obs_body_optional_sending_value: Invalid derived from count %d",
                             count);
            return 0;
        }
    }

    if (flags.is_member_of_present)
    {
        uint8_t count = p_value->is_mem_of.count;
        memcpy(&(*pp_temp_value)[current_offset],
               &p_value->is_mem_of.count, 1);
        current_offset += 1;

        if (count)
        {
            memcpy(&(*pp_temp_value)[current_offset],
                   p_value->is_mem_of.p_mem_ids,
                   4 * count);
            current_offset += (4 * count);
        }
        else
        {
            APP_PRINT_ERROR1("ghss_format_health_obs_body_optional_sending_value: Invalid is member of count %d",
                             count);
            return 0;
        }
    }

    if (flags.tlvs_present)
    {
        uint8_t num_of_tlv_attr = p_value->tlvs.num_of_tlv_attr;
        memcpy(&(*pp_temp_value)[current_offset],
               &p_value->tlvs.num_of_tlv_attr, 1);
        current_offset += 1;

        for (uint8_t i = 0; i < num_of_tlv_attr; i++)
        {
            memcpy(&(*pp_temp_value)[current_offset],
                   &p_value->tlvs.p_tlvs_value[i].type, 4);
            current_offset += 4;
            uint16_t length = p_value->tlvs.p_tlvs_value[i].length;
            memcpy(&(*pp_temp_value)[current_offset],
                   &length, 2);
            current_offset += 2;
            memcpy(&(*pp_temp_value)[current_offset],
                   &p_value->tlvs.p_tlvs_value[i].format_type, 1);
            current_offset += 1;
            memcpy(&(*pp_temp_value)[current_offset],
                   p_value->tlvs.p_tlvs_value[i].p_encoded_value,
                   length);
            current_offset += length;
        }
    }

    return current_offset;
}

uint16_t ghss_format_health_obs_body_obs_sending_value(T_GHSS_HOB_CLASS_TYPE type,
                                                       T_GHSS_HOB_VALUE *p_value, uint16_t offset, uint8_t **pp_temp_value)
{
    uint16_t current_offset = offset;

    // Mandatory part
    switch (type)
    {
    case GHSS_HOB_CLASS_TYPE_NUMERIC_OBSERVATION:
        {
            T_GHSS_HOBV_NUMERIC_OBS *p_obs_value = &p_value->numeric_obs;
            memcpy(&(*pp_temp_value)[current_offset],
                   &p_obs_value->unit_code, 2);
            current_offset += 2;
            memcpy(&(*pp_temp_value)[current_offset],
                   &p_obs_value->value, 4);
            current_offset += 4;

        }
        break;

    case GHSS_HOB_CLASS_TYPE_SIMPLE_DISCRETE_OBSERVATION:
        {
            T_GHSS_HOBV_SIMPLE_DISCRETE_OBS *p_obs_value = &p_value->simple_discrete_obs;
            memcpy(&(*pp_temp_value)[current_offset],
                   &p_obs_value->value, 4);
            current_offset += 4;
        }
        break;

    case GHSS_HOB_CLASS_TYPE_STRING_OBSERVATION:
        {
            T_GHSS_HOBV_STRING_OBS *p_obs_value = &p_value->string_obs;
            uint16_t length = p_obs_value->length;
            memcpy(&(*pp_temp_value)[current_offset], &length, 2);
            current_offset += 2;
            memcpy(&(*pp_temp_value)[current_offset],
                   p_obs_value->p_value, length);
            current_offset += length;
        }
        break;

    case GHSS_HOB_CLASS_TYPE_SAMPLE_ARRAY_OBSERVATION:
        {
            T_GHSS_HOBV_SAMPLE_ARRAY_OBS *p_obs_value = &p_value->sample_array_obs;
            memcpy(&(*pp_temp_value)[current_offset],
                   &p_obs_value->unit_code, 2);
            current_offset += 2;
            memcpy(&(*pp_temp_value)[current_offset],
                   &p_obs_value->scale_factor, 4);
            current_offset += 4;
            memcpy(&(*pp_temp_value)[current_offset],
                   &p_obs_value->offset, 4);
            current_offset += 4;
            memcpy(&(*pp_temp_value)[current_offset],
                   &p_obs_value->sample_period, 4);
            current_offset += 4;
            memcpy(&(*pp_temp_value)[current_offset],
                   &p_obs_value->number_of_samples_per_period,
                   1);
            current_offset += 1;
            uint8_t bytes = p_obs_value->bytes_per_period;
            memcpy(&(*pp_temp_value)[current_offset], &bytes, 1);
            current_offset += 1;
            uint32_t number = p_obs_value->number_of_samples;
            memcpy(&(*pp_temp_value)[current_offset], &number, 4);
            current_offset += 4;
            memcpy(&(*pp_temp_value)[current_offset],
                   p_obs_value->p_sample, bytes * number);
            current_offset += (bytes * number);
        }
        break;

    case GHSS_HOB_CLASS_TYPE_COMPOUND_DISCRETE_EVENT_OBSERVATION:
        {
            T_GHSS_HOBV_COMPOUND_DISCRETE_EVENT_OBS *p_obs_value = &p_value->compound_discrete_event_obs;
            uint8_t number = p_obs_value->number_of_components;
            memcpy(&(*pp_temp_value)[current_offset], &number, 1);
            current_offset += 1;
            memcpy(&(*pp_temp_value)[current_offset],
                   p_obs_value->p_value, 4 * number);
            current_offset += (4 * number);
        }
        break;

    case GHSS_HOB_CLASS_TYPE_COMPOUND_STATE_EVENT_OBSERVATION:
        {
            T_GHSS_HOBV_COMPOUND_STATE_EVENT_OBS *p_obs_value = &p_value->compound_state_event_obs;
            uint8_t size = p_obs_value->size;
            memcpy(&(*pp_temp_value)[current_offset], &size, 1);
            current_offset += 1;
            memcpy(&(*pp_temp_value)[current_offset],
                   p_obs_value->p_mask_bits, size);
            current_offset += size;
            memcpy(&(*pp_temp_value)[current_offset],
                   p_obs_value->p_state_event_mask_bits,
                   size);
            current_offset += size;
            memcpy(&(*pp_temp_value)[current_offset],
                   p_obs_value->p_value, size);
            current_offset += size;
        }
        break;

    case GHSS_HOB_CLASS_TYPE_TLV_ENCODED_OBSERVATION:   //only for obs_bdl_value
        {
            T_GHSS_HOB_TLVS *p_obs_value = &p_value->tlv_encoded_obs;
            uint8_t number =
                p_obs_value->num_of_tlv_attr;
            memcpy(&(*pp_temp_value)[current_offset], &number, 1);
            current_offset += 1;
            for (uint8_t i = 0; i < number; i++)
            {
                memcpy(&(*pp_temp_value)[current_offset],
                       &p_obs_value->p_tlvs_value[i].type, 4);
                current_offset += 4;
                uint16_t length =
                    p_obs_value->p_tlvs_value[i].length;
                memcpy(&(*pp_temp_value)[current_offset], &length, 2);
                current_offset += 2;
                memcpy(&(*pp_temp_value)[current_offset],
                       &p_obs_value->p_tlvs_value[i].format_type,
                       1);
                current_offset += 1;
                memcpy(&(*pp_temp_value)[current_offset],
                       p_obs_value->p_tlvs_value[i].p_encoded_value,
                       length);
                current_offset += length;
            }
        }
        break;

    default:
        APP_PRINT_ERROR1("ghss_format_health_obs_body_obs_sending_value: Invalid Observation Class Type %d",
                         type);
        return 0;
    }

    return current_offset;
}

uint16_t ghss_format_health_obs_sending_value(T_GHSS_OBS_REPORT_TYPE type, uint32_t idx,
                                              T_GHSS_HEALTH_OBS_BODY *p_value,
                                              uint8_t **pp_temp_value)
{
    T_GHSS_HOB_FLAGS flags = p_value->flags;
    uint16_t current_offset = 0;
    uint8_t  err_idx = 0;

    APP_PRINT_INFO2("ghss_format_health_obs_sending_value: Report type %d, Observation Class Type %d",
                    type, p_value->obs_class_type);

    uint16_t hob_length = ghss_format_health_obs_body_length(p_value);

    if (hob_length == 0)
    {
        APP_PRINT_ERROR0("ghss_format_health_obs_sending_value: Invalid Length");
        return 0;
    }

    uint16_t total_len = hob_length;

    if (type == GHSS_OBS_REPORT_TYPE_STORED_HEALTH_OBS)
    {
        total_len += 4;
    }

    *pp_temp_value = os_mem_alloc(RAM_TYPE_DATA_ON, total_len);
    if (*pp_temp_value == NULL)
    {
        APP_PRINT_ERROR0("ghss_format_health_obs_sending_value: Allocate memory fail");
        return 0;
    }

    if (type == GHSS_OBS_REPORT_TYPE_STORED_HEALTH_OBS)
    {
        memcpy(&(*pp_temp_value)[current_offset], &idx, 4);
        current_offset += 4;
    }

    // Mandatory part
    memcpy(&(*pp_temp_value)[current_offset], &p_value->obs_class_type, 1);
    current_offset += 1;
    memcpy(&(*pp_temp_value)[current_offset],
           &hob_length, 2);
    current_offset += 2;
    memcpy(&(*pp_temp_value)[current_offset], &flags, 2);
    current_offset += 2;

    uint16_t offset = ghss_format_health_obs_body_optional_sending_value(p_value, current_offset,
                                                                         pp_temp_value);
    if (offset == 0)
    {
        err_idx = 1;
        goto error;
    }
    current_offset = offset;

    // Mandatory part
    switch (p_value->obs_class_type)
    {
    case GHSS_HOB_CLASS_TYPE_NUMERIC_OBSERVATION:
    case GHSS_HOB_CLASS_TYPE_SIMPLE_DISCRETE_OBSERVATION:
    case GHSS_HOB_CLASS_TYPE_STRING_OBSERVATION:
    case GHSS_HOB_CLASS_TYPE_SAMPLE_ARRAY_OBSERVATION:
    case GHSS_HOB_CLASS_TYPE_COMPOUND_DISCRETE_EVENT_OBSERVATION:
    case GHSS_HOB_CLASS_TYPE_COMPOUND_STATE_EVENT_OBSERVATION:
    case GHSS_HOB_CLASS_TYPE_TLV_ENCODED_OBSERVATION:   //only for obs_bdl_value
        {
            uint16_t offset = ghss_format_health_obs_body_obs_sending_value(p_value->obs_class_type,
                                                                            &p_value->obs_value, current_offset, pp_temp_value);
            if (offset == 0)
            {
                err_idx = 2;
                goto error;
            }
            current_offset = offset;
        }
        break;

    case GHSS_HOB_CLASS_TYPE_COMPOUND_OBSERVATION:
        {
            uint8_t number =
                p_value->obs_value.compound_obs.number_of_components;
            memcpy(&(*pp_temp_value)[current_offset], &number, 1);
            current_offset += 1;

            for (uint8_t i = 0; i < number; i++)
            {
                memcpy(&(*pp_temp_value)[current_offset],
                       &p_value->obs_value.compound_obs.p_component_value[i].component_type, 4);
                current_offset += 4;
                memcpy(&(*pp_temp_value)[current_offset],
                       &p_value->obs_value.compound_obs.p_component_value[i].component_value_type, 1);
                current_offset += 1;

                T_GHSS_HOB_VALUE *p_obs_value = (T_GHSS_HOB_VALUE *)
                                                p_value->obs_value.compound_obs.p_component_value[i].p_value;
                T_GHSS_HOB_CLASS_TYPE obs_type =
                    p_value->obs_value.compound_obs.p_component_value[i].component_value_type;

                uint16_t offset = ghss_format_health_obs_body_obs_sending_value(obs_type, p_obs_value,
                                                                                current_offset,
                                                                                pp_temp_value);
                if (offset == 0)
                {
                    err_idx = 3;
                    goto error;
                }
                current_offset = offset;
            }
        }
        break;

    case GHSS_HOB_CLASS_TYPE_OBSERVATION_BUNDLE:
        {
            uint8_t number = p_value->obs_value.obs_bundle.number_of_obs;
            memcpy(&(*pp_temp_value)[current_offset], &number, 1);
            current_offset += 1;

            for (uint8_t n = 0; n < number; n++)
            {
                T_GHSS_HEALTH_OBS_BODY *p_obs_bdl_value = (T_GHSS_HEALTH_OBS_BODY *)
                                                          p_value->obs_value.obs_bundle.p_obs_bdl_value + n;

                uint16_t obs_bdl_len = ghss_format_health_obs_body_length(p_value);

                // Mandatory part
                memcpy(&(*pp_temp_value)[current_offset], &p_obs_bdl_value->obs_class_type, 1);
                current_offset += 1;
                memcpy(&(*pp_temp_value)[current_offset],
                       &obs_bdl_len, 2);
                current_offset += 2;
                memcpy(&(*pp_temp_value)[current_offset], &p_obs_bdl_value->flags, 2);
                current_offset += 2;

                uint16_t offset = ghss_format_health_obs_body_optional_sending_value(p_obs_bdl_value,
                                                                                     current_offset, pp_temp_value);
                if (offset == 0)
                {
                    err_idx = 4;
                    goto error;
                }
                current_offset = offset;

                T_GHSS_HOB_VALUE *p_obs_value = (T_GHSS_HOB_VALUE *)&p_obs_bdl_value->obs_value;

                if (p_obs_bdl_value->obs_class_type == GHSS_HOB_CLASS_TYPE_COMPOUND_OBSERVATION)
                {
                    uint8_t number =
                        p_obs_value->compound_obs.number_of_components;
                    memcpy(&(*pp_temp_value)[current_offset], &number, 1);
                    current_offset += 1;

                    for (uint8_t i = 0; i < number; i++)
                    {
                        memcpy(&(*pp_temp_value)[current_offset],
                               &p_obs_value->compound_obs.p_component_value[i].component_type, 4);
                        current_offset += 4;
                        memcpy(&(*pp_temp_value)[current_offset],
                               &p_obs_value->compound_obs.p_component_value[i].component_value_type, 1);
                        current_offset += 1;

                        T_GHSS_HOB_VALUE *p_obs_components_value = (T_GHSS_HOB_VALUE *)
                                                                   p_obs_value->compound_obs.p_component_value[i].p_value;
                        T_GHSS_HOB_CLASS_TYPE obs_components_type =
                            p_obs_value->compound_obs.p_component_value[i].component_value_type;
                        uint16_t offset = ghss_format_health_obs_body_obs_sending_value(obs_components_type,
                                                                                        p_obs_components_value,
                                                                                        current_offset, pp_temp_value);
                        if (offset == 0)
                        {
                            err_idx = 5;
                            goto error;
                        }
                        current_offset = offset;
                    }
                }
                else
                {
                    uint16_t offset = ghss_format_health_obs_body_obs_sending_value(p_obs_bdl_value->obs_class_type,
                                                                                    p_obs_value,
                                                                                    current_offset, pp_temp_value);
                    if (offset == 0)
                    {
                        err_idx = 6;
                        goto error;
                    }
                    current_offset = offset;
                }
            }
        }
        break;

    default:
        err_idx = 7;
        goto error;
    }

    APP_PRINT_INFO2("ghss_format_health_obs_sending_value:total_len %d, current_offset %d",
                    total_len, current_offset);
    return total_len;

error:
    APP_PRINT_ERROR2("ghss_format_health_obs_sending_value: Observation Class Type %d ERR IDX %d",
                     p_value->obs_class_type, err_idx);
    os_mem_free(*pp_temp_value);
    *pp_temp_value = NULL;
    return 0;
}

uint16_t ghss_format_health_obs_segment(uint8_t seg_count, uint8_t seg_num, uint16_t report_mtu,
                                        uint8_t rolling_seg_count, uint16_t value_len, uint8_t *p_value, uint8_t **pp_seg)
{
    T_GHSS_HEALTH_OBS_SEG_HEADER segmentation_header = {0};
    uint16_t length = 0;

    APP_PRINT_INFO2("ghss_format_health_obs_segment: seg_count %d, seg_num %d",
                    seg_count, seg_num);

    if (rolling_seg_count > GHSS_MAX_ROLLING_SEGMENT_COUNTER)
    {
        APP_PRINT_ERROR1("ghss_format_health_obs_segment: Invalid rolling_seg_count %d",
                         rolling_seg_count);
        return 0;
    }
    else
    {
        segmentation_header.rolling_segment_counter = rolling_seg_count;
    }

    if (seg_num == 1)
    {
        segmentation_header.first_segment = 1;
        segmentation_header.last_segment = 1;
        length = value_len;
    }
    else if (seg_num > 1)
    {
        if (seg_count == 0)
        {
            segmentation_header.first_segment = 1;
            segmentation_header.last_segment = 0;
            length = report_mtu;
        }
        else if ((seg_count > 0) && (seg_count < (seg_num - 1)))
        {
            segmentation_header.first_segment = 0;
            segmentation_header.last_segment = 0;
            length = report_mtu;
        }
        else if (seg_count == seg_num - 1)
        {
            segmentation_header.first_segment = 0;
            segmentation_header.last_segment = 1;
            length = (value_len + seg_num) - seg_count * report_mtu;
        }
    }

    if (length < 2)
    {
        APP_PRINT_ERROR0("ghss_format_health_obs_segment: Invalid seg length");
        return 0;
    }

    *pp_seg = os_mem_alloc(RAM_TYPE_DATA_ON, length);

    if (*pp_seg == NULL)
    {
        APP_PRINT_ERROR0("ghss_format_health_obs_segment: Allocate memory fail");
        return 0;
    }

    memcpy(*pp_seg, &segmentation_header, 1);
    memcpy(&(*pp_seg)[1],
           &p_value[seg_count * 19],
           length - 1);

    return length;
}

bool ghss_check_cccd(uint16_t uuid)
{
    if (uuid == GATT_UUID_CHAR_RECORD_ACCESS_CONTROL_POINT)
    {
        if ((ghss_notify_indicate_flag.ghss_racp_indicate_enable == 1) &&
            (ghss_notify_indicate_flag.ghss_stored_obs_notify_indicate_enable == 1))
        {
            return true;
        }
    }
    else if (uuid == GATT_UUID_CHAR_GHS_CONTROL_POINT)
    {
        if (ghss_notify_indicate_flag.ghss_ghs_cp_indicate_enable == 1)
        {
            return true;
        }
    }

    return false;
}

bool ghss_get_obs_report_attr_idx(T_GHSS_OBS_REPORT_TYPE report_type, uint16_t *p_attr_idx)
{
    bool ret = true;

    switch (report_type)
    {
    case GHSS_OBS_REPORT_TYPE_LIVE_HEALTH_OBS:
        {
            *p_attr_idx = gatt_svc_find_char_index_by_uuid16(ghss_attr_tbl,
                                                             GATT_UUID_CHAR_LIVE_HEALTH_OBSERVATIONS,
                                                             ghss_char_num);
            break;
        }

    case GHSS_OBS_REPORT_TYPE_STORED_HEALTH_OBS:
        {
            *p_attr_idx = gatt_svc_find_char_index_by_uuid16(ghss_attr_tbl,
                                                             GATT_UUID_CHAR_STORED_HEALTH_OBSERVATIONS,
                                                             ghss_char_num);
            break;
        }

    default:
        ret = false;
        break;
    }

    return ret;
}

bool ghss_get_cp_report_attr_idx(T_GHSS_CP_REPORT_TYPE report_type, uint16_t *p_attr_idx)
{
    bool ret = true;

    switch (report_type)
    {
    case GHSS_CP_REPORT_TYPE_RACP:
        {
            *p_attr_idx = gatt_svc_find_char_index_by_uuid16(ghss_attr_tbl,
                                                             GATT_UUID_CHAR_RECORD_ACCESS_CONTROL_POINT,
                                                             ghss_char_num);
            break;
        }

    case GHSS_CP_REPORT_TYPE_GHS_CP:
        {
            *p_attr_idx = gatt_svc_find_char_index_by_uuid16(ghss_attr_tbl,
                                                             GATT_UUID_CHAR_GHS_CONTROL_POINT,
                                                             ghss_char_num);
            break;
        }

    default:
        ret = false;
        break;
    }

    return ret;
}

bool ghss_send_health_obs_report(uint8_t conn_id, uint8_t service_id,
                                 T_GHSS_CHAR_OBS_REPORT report_data)
{
    uint16_t attr_idx = 0;
    T_GATT_PDU_TYPE pdu_type = GATT_PDU_TYPE_INDICATION;

    if (!ghss_get_obs_report_attr_idx(report_data.report_type, &attr_idx))
    {
        APP_PRINT_ERROR0("ghss_send_cp_report: Get invalid attr idx");
        return false;
    }

    if (report_data.is_notify)
    {
        pdu_type = GATT_PDU_TYPE_NOTIFICATION;
    }

    return server_send_data(conn_id, service_id, attr_idx, report_data.obs_segment.p_obs_segment,
                            report_data.obs_segment.obs_segment_len, pdu_type);
}

bool ghss_send_cp_report(uint8_t conn_id, uint8_t service_id, T_GHSS_CHAR_CP_REPORT report_data)
{
    uint16_t attr_idx = 0;

    if (!ghss_get_cp_report_attr_idx(report_data.report_type, &attr_idx))
    {
        APP_PRINT_ERROR0("ghss_send_cp_report: Get invalid attr idx");
        return false;
    }

    void *p_value;
    uint16_t length;

    switch (report_data.report_type)
    {
    case GHSS_CP_REPORT_TYPE_RACP:
        {
            p_value = report_data.value.report_racp.p_racp;
            length = report_data.value.report_racp.racp_len;
            break;
        }

    case GHSS_CP_REPORT_TYPE_GHS_CP:
        {
            p_value = report_data.value.p_ghs_cp;
            length = sizeof(T_GHSS_GHS_CP_OPCODE);
            break;
        }

    default:
        return false;
    }

    return server_send_data(conn_id, service_id, attr_idx, p_value,
                            length, GATT_PDU_TYPE_INDICATION);
}

T_APP_RESULT ghss_attr_write_cb(uint8_t conn_id, T_SERVER_ID service_id, uint16_t attrib_index,
                                T_WRITE_TYPE write_type, uint16_t length, uint8_t *p_value,
                                P_FUN_WRITE_IND_POST_PROC *p_write_post_proc)
{
    T_APP_RESULT cause = APP_RESULT_SUCCESS;
    T_GHSS_SERVER_WRITE_IND write_ind = {0};
    T_CHAR_UUID char_uuid = gatt_svc_find_char_uuid_by_index(ghss_attr_tbl, attrib_index,
                                                             ghss_char_num);

    write_ind.char_uuid = char_uuid.uu.char_uuid16;
    write_ind.service_id = service_id;
    write_ind.write_type = write_type;

    APP_PRINT_INFO4("ghss_attr_write_cb: conn_id 0x%x, service_id 0x%x, char_uuid 0x%x, length %d",
                    conn_id, service_id, write_ind.char_uuid, length);

    if (!p_value)
    {
        cause = APP_RESULT_INVALID_PDU;
        return cause;
    }

    switch (write_ind.char_uuid)
    {
    default:
        cause = APP_RESULT_ATTR_NOT_FOUND;
        break;

    case GATT_UUID_CHAR_RECORD_ACCESS_CONTROL_POINT:
        {
            /* Attribute value has variable size, make sure written value size is valid. */
            if ((length > sizeof(T_GHSS_RACP)) || (p_value == NULL))
            {
                cause = APP_RESULT_INVALID_VALUE_SIZE;
            }
            else if (!ghss_check_cccd(write_ind.char_uuid))
            {
                cause = APP_RESULT_CCCD_IMPROPERLY_CONFIGURED;
            }
            else
            {
                write_ind.data.ghss_racp.opcode = (T_GHSS_RACP_OPCODE)p_value[0];
                write_ind.data.ghss_racp.operator = (T_GHSS_RACP_OPERATOR)p_value[1];
                memcpy(write_ind.data.ghss_racp.operand, &p_value[2], length - 2);
            }
        }
        break;

    case GATT_UUID_CHAR_GHS_CONTROL_POINT:
        {
            if (!ghss_check_cccd(write_ind.char_uuid))
            {
                cause = APP_RESULT_CCCD_IMPROPERLY_CONFIGURED;
            }
            else if ((p_value[0] != GHSS_GHS_CP_OPCODE_START_SEND_LIVE_OBS) &&
                     (p_value[0] != GHSS_GHS_CP_OPCODE_STOP_SEND_LIVE_OBS) &&
                     (p_value[0] != GHSS_GHS_CP_OPCODE_SUCCESS))
            {
                cause = (T_APP_RESULT)ATT_ERR_GHS_COMMAND_NOT_SUPPORTED;
            }
            else
            {
                write_ind.data.ghss_ghs_cp = (T_GHSS_GHS_CP_OPCODE)p_value[0];
            }
        }
        break;
    }

    if (pfn_ghss_cb && (cause == APP_RESULT_SUCCESS))
    {
        cause = pfn_ghss_cb(conn_id, GATT_MSG_GHSS_SERVER_WRITE_IND, (void *)&write_ind);
    }

    return cause;
}

void ghss_cccd_update_cb(uint8_t conn_id, T_SERVER_ID service_id, uint16_t index, uint16_t ccc_bits)
{
    bool cause = true;
    T_CHAR_UUID char_uuid = gatt_svc_find_char_uuid_by_index(ghss_attr_tbl, index, ghss_char_num);
    T_GHSS_SERVER_CCCD_UPDATE cccd_update = {0};

    cccd_update.service_id = service_id;
    cccd_update.char_uuid = char_uuid.uu.char_uuid16;
    cccd_update.cccd_cfg = ccc_bits;

    APP_PRINT_INFO2("ghss_cccd_update_cb index = %d ccc_bits 0x%x", index, ccc_bits);

    switch (cccd_update.char_uuid)
    {
    case GATT_UUID_CHAR_LIVE_HEALTH_OBSERVATIONS:
        {
            if (ccc_bits & (GATT_CLIENT_CHAR_CONFIG_INDICATE | GATT_CLIENT_CHAR_CONFIG_NOTIFY |
                            GATT_CLIENT_CHAR_CONFIG_NOTIFY_INDICATE))
            {
                ghss_notify_indicate_flag.ghss_live_obs_notify_indicate_enable = 1;
            }
            else
            {
                ghss_notify_indicate_flag.ghss_live_obs_notify_indicate_enable = 0;
            }
        }
        break;

    case GATT_UUID_CHAR_STORED_HEALTH_OBSERVATIONS:
        {
            if (ccc_bits & (GATT_CLIENT_CHAR_CONFIG_INDICATE | GATT_CLIENT_CHAR_CONFIG_NOTIFY |
                            GATT_CLIENT_CHAR_CONFIG_NOTIFY_INDICATE))
            {
                ghss_notify_indicate_flag.ghss_stored_obs_notify_indicate_enable = 1;
            }
            else
            {
                ghss_notify_indicate_flag.ghss_stored_obs_notify_indicate_enable = 0;
            }
        }
        break;

    case GATT_UUID_CHAR_RECORD_ACCESS_CONTROL_POINT:
        {
            if (ccc_bits & GATT_CLIENT_CHAR_CONFIG_INDICATE)
            {
                ghss_notify_indicate_flag.ghss_racp_indicate_enable = 1;
            }
            else
            {
                ghss_notify_indicate_flag.ghss_racp_indicate_enable = 0;
            }
        }
        break;

    case GATT_UUID_CHAR_GHS_CONTROL_POINT:
        {
            if (ccc_bits & GATT_CLIENT_CHAR_CONFIG_INDICATE)
            {
                ghss_notify_indicate_flag.ghss_ghs_cp_indicate_enable = 1;
            }
            else
            {
                ghss_notify_indicate_flag.ghss_ghs_cp_indicate_enable = 0;
            }
        }
        break;

    default:
        break;
    }

    if (pfn_ghss_cb && (cause == true))
    {
        pfn_ghss_cb(conn_id, GATT_MSG_GHSS_SERVER_CCCD_UPDATE, (void *)&cccd_update);
    }

    return;
}

/**
 * @brief GHSS Service Callbacks.
 */
const T_FUN_GATT_SERVICE_CBS ghss_cbs =
{
    NULL,                // Read callback function pointer
    ghss_attr_write_cb,  // Write callback function pointer
    ghss_cccd_update_cb  // CCCD update callback function pointer
};

/**
 * @brief Add Generic Health Sensor Service to the Host database.
 */
T_SERVER_ID ghss_reg_srv(P_FUN_GHSS_SERVER_APP_CB app_cb)
{
    T_SERVER_ID service_id;
    if (false == server_add_service(&service_id,
                                    (uint8_t *)ghss_attr_tbl,
                                    sizeof(ghss_attr_tbl),
                                    ghss_cbs))
    {
        APP_PRINT_ERROR1("ghss_reg_srv: service_id %d", service_id);
        service_id = 0xff;
    }

    pfn_ghss_cb = app_cb;
    return service_id;
}

