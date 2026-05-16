/**
*****************************************************************************************
*     Copyright(c) 2016, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
  * @file    hrs_client.c
  * @brief
  * @details
  * @author  jane
  * @date    2016-02-18
  * @version v1.0
  ******************************************************************************
  */

/** Add Includes here **/
#include <string.h>
#include <hrs_client.h>
#include <trace.h>
#include <os_mem.h>

/********************************************************************************************************
* local static variables defined here, only used in this source file.
********************************************************************************************************/

/**
 * @brief  HRS client Link control block definition.
 */
typedef struct
{
    T_HRS_DISC_STATE disc_state;
    bool             write_notify_value;
    uint16_t         properties;
    uint16_t         hdl_cache[HDL_HRS_CACHE_LEN];
} T_HRS_LINK, *P_HRS_LINK;

static P_HRS_LINK hrs_table;
static uint8_t hrs_link_num;

/**<  HRS client ID. */
static T_CLIENT_ID hrs_client = CLIENT_PROFILE_GENERAL_ID;
/**<  Callback used to send data to app from HRS client layer. */
static P_FUN_GENERAL_APP_CB hrs_client_cb = NULL;

/**
  * @brief  Used by application, to start the discovery procedure of heart rate service.
  * @param[in]  conn_id connection ID.
  * @retval true send request to upper stack success.
  * @retval false send request to upper stack failed.
  *
  * <b>Example usage</b>
  * \code{.c}
    static T_USER_CMD_PARSE_RESULT cmd_hrsdis(T_USER_CMD_PARSED_VALUE *p_parse_value)
    {
        uint8_t conn_id = p_parse_value->dw_param[0];
        bool ret = hrs_start_discovery(conn_id);
        ......
    }
  * \endcode
  */
bool hrs_start_discovery(uint8_t conn_id)
{
    PROFILE_PRINT_INFO0("hrs_start_discovery");
    if (conn_id >= hrs_link_num)
    {
        PROFILE_PRINT_ERROR1("hrs_start_discovery: failed invalid conn_id %d", conn_id);
        return false;
    }
    /* First clear handle cache. */
    memset(&hrs_table[conn_id], 0, sizeof(T_HRS_LINK));
    hrs_table[conn_id].disc_state = DISC_HRS_START;
    if (client_by_uuid_srv_discovery(conn_id, hrs_client,
                                     GATT_UUID_SERVICE_HEART_RATE) == GAP_CAUSE_SUCCESS)
    {
        return true;
    }
    return false;
}

/**
  * @brief  Used by application, to set the notification flag of heart rate measurement.
  * @param[in]  conn_id connection ID.
  * @param[in]  notify value to enable or disable notify.
  * @retval true send request to upper stack success.
  * @retval false send request to upper stack failed.
  *
  * <b>Example usage</b>
  * \code{.c}
    static T_USER_CMD_PARSE_RESULT cmd_hrsmeascccd(T_USER_CMD_PARSED_VALUE *p_parse_value)
    {
        uint8_t conn_id = p_parse_value->dw_param[0];
        bool notify = p_parse_value->dw_param[1];
        bool ret;
        ret = hrs_set_heart_rate_measurement_notify(conn_id, notify);
        ......
    }
  * \endcode
  */
bool hrs_set_heart_rate_measurement_notify(uint8_t conn_id, bool notify)
{
    if (conn_id >= hrs_link_num)
    {
        PROFILE_PRINT_ERROR1("hrs_set_heart_rate_measurement_notify: failed invalid conn_id %d", conn_id);
        return false;
    }
    if (hrs_table[conn_id].hdl_cache[HDL_HRS_HEART_RATE_MEASUREMENT_CCCD])
    {
        uint16_t handle = hrs_table[conn_id].hdl_cache[HDL_HRS_HEART_RATE_MEASUREMENT_CCCD];
        uint16_t length = sizeof(uint16_t);
        uint16_t cccd_bits = notify ? 1 : 0;
        if (client_attr_write(conn_id, hrs_client, GATT_WRITE_TYPE_REQ, handle,
                              length, (uint8_t *)&cccd_bits) == GAP_CAUSE_SUCCESS)
        {
            hrs_table[conn_id].write_notify_value = notify;
            return true;
        }
    }
    PROFILE_PRINT_ERROR0("hrs_set_heart_rate_measurement_notify: false handle = 0");
    return false;
}

/**
  * @brief  Used by application, to get handle cache.
  * @param[in]  conn_id connection ID.
  * @param[in]  p_hdl_cache pointer of the handle cache table
  * @param[in]  len the length of handle cache table
  * @retval true success.
  * @retval false failed.
  *
  * <b>Example usage</b>
  * \code{.c}
    static T_USER_CMD_PARSE_RESULT cmd_hrshdl(T_USER_CMD_PARSED_VALUE *p_parse_value)
    {
        uint8_t conn_id = p_parse_value->dw_param[0];
        uint16_t hdl_cache[HDL_HRS_CACHE_LEN];
        bool ret = hrs_get_hdl_cache(conn_id, hdl_cache,
                                     sizeof(uint16_t) * HDL_HRS_CACHE_LEN);

        ......
    }
  * \endcode
  */
bool hrs_get_hdl_cache(uint8_t conn_id, uint16_t *p_hdl_cache, uint8_t len)
{
    if (conn_id >= hrs_link_num)
    {
        PROFILE_PRINT_ERROR1("hrs_get_hdl_cache: failed invalid conn_id %d", conn_id);
        return false;
    }
    if (hrs_table[conn_id].disc_state != DISC_HRS_DONE)
    {
        PROFILE_PRINT_ERROR1("hrs_get_hdl_cache: failed invalid state %d", hrs_table[conn_id].disc_state);
        return false;
    }
    if (len != sizeof(uint16_t) * HDL_HRS_CACHE_LEN)
    {
        PROFILE_PRINT_ERROR1("hrs_get_hdl_cache: failed invalid len %d", len);
        return false;
    }
    memcpy(p_hdl_cache, hrs_table[conn_id].hdl_cache, len);
    return true;
}

/**
  * @brief  Used by application, to set handle cache.
  * @param[in]  conn_id connection ID.
  * @param[in]  p_hdl_cache pointer of the handle cache table
  * @param[in]  len the length of handle cache table
  * @retval true success.
  * @retval false failed.
  *
  * <b>Example usage</b>
  * \code{.c}
    void app_discov_services(uint8_t conn_id, bool start)
    {
        ......
        if (app_srvs_table.srv_found_flags & APP_DISCOV_HRS_FLAG)
        {
            hrs_set_hdl_cache(conn_id, app_srvs_table.hrs_hdl_cache, sizeof(uint16_t) * HDL_HRS_CACHE_LEN);
        }
        ......
    }
  * \endcode
  */
bool hrs_set_hdl_cache(uint8_t conn_id, uint16_t *p_hdl_cache, uint8_t len)
{
    if (conn_id >= hrs_link_num)
    {
        PROFILE_PRINT_ERROR1("hrs_set_hdl_cache: failed invalid conn_id %d", conn_id);
        return false;
    }
    if (hrs_table[conn_id].disc_state != DISC_HRS_IDLE)
    {
        PROFILE_PRINT_ERROR1("hrs_set_hdl_cache: failed invalid state %d", hrs_table[conn_id].disc_state);
        return false;
    }
    if (len != sizeof(uint16_t) * HDL_HRS_CACHE_LEN)
    {
        PROFILE_PRINT_ERROR1("hrs_set_hdl_cache: failed invalid len %d", len);
        return false;
    }
    memcpy(hrs_table[conn_id].hdl_cache, p_hdl_cache, len);
    hrs_table[conn_id].disc_state = DISC_HRS_DONE;
    return true;
}

static bool hrs_start_char_discovery(uint8_t conn_id)
{
    uint16_t start_handle;
    uint16_t end_handle;

    PROFILE_PRINT_INFO0("hrs_start_char_discovery");
    start_handle = hrs_table[conn_id].hdl_cache[HDL_HRS_SRV_START];
    end_handle = hrs_table[conn_id].hdl_cache[HDL_HRS_SRV_END];
    if (client_all_char_discovery(conn_id, hrs_client, start_handle,
                                  end_handle) == GAP_CAUSE_SUCCESS)
    {
        return true;
    }
    return false;
}

static bool hrs_start_char_descriptor_discovery(uint8_t conn_id)
{
    uint16_t start_handle;
    uint16_t end_handle;

    PROFILE_PRINT_INFO0("hrs_start_char_descriptor_discovery");
    start_handle = hrs_table[conn_id].hdl_cache[HDL_HRS_HEART_RATE_MEASUREMENT];
    end_handle = hrs_table[conn_id].hdl_cache[HDL_HRS_SRV_END];
    if (client_all_char_descriptor_discovery(conn_id, hrs_client, start_handle,
                                             end_handle) == GAP_CAUSE_SUCCESS)
    {
        return true;
    }
    return false;
}

static void hrs_client_discover_state_cb(uint8_t conn_id,  T_DISCOVERY_STATE discovery_state)
{
    bool cb_flag = false;
    T_HRS_CLIENT_CB_DATA cb_data;
    cb_data.cb_type = HRS_CLIENT_CB_TYPE_DISC_STATE;

    PROFILE_PRINT_INFO1("hrs_client_discover_state_cb: discovery_state = %d", discovery_state);
    if (hrs_table[conn_id].disc_state == DISC_HRS_START)
    {
        switch (discovery_state)
        {
        case DISC_STATE_SRV_DONE:
            /* Indicate that service handle found. Start discover characteristic. */
            if ((hrs_table[conn_id].hdl_cache[HDL_HRS_SRV_START] != 0)
                || (hrs_table[conn_id].hdl_cache[HDL_HRS_SRV_END] != 0))
            {
                if (hrs_start_char_discovery(conn_id) == false)
                {
                    hrs_table[conn_id].disc_state = DISC_HRS_FAILED;
                    cb_flag = true;
                }
            }
            /* No HRS handle found. Discover procedure complete. */
            else
            {
                hrs_table[conn_id].disc_state = DISC_HRS_FAILED;
                cb_flag = true;
            }
            break;

        case DISC_STATE_CHAR_DONE:
            if (hrs_table[conn_id].properties & GATT_CHAR_PROP_NOTIFY)
            {
                //discovery cccd
                if (hrs_start_char_descriptor_discovery(conn_id) == false)
                {
                    hrs_table[conn_id].disc_state = DISC_HRS_FAILED;
                    cb_flag = true;
                }
            }
            else
            {
                hrs_table[conn_id].disc_state = DISC_HRS_DONE;
                cb_flag = true;
            }
            break;

        case DISC_STATE_CHAR_DESCRIPTOR_DONE:
            hrs_table[conn_id].disc_state = DISC_HRS_DONE;
            cb_flag = true;
            break;

        case DISC_STATE_FAILED:
            hrs_table[conn_id].disc_state = DISC_HRS_FAILED;
            cb_flag = true;
            break;

        default:
            PROFILE_PRINT_ERROR0("Invalid Discovery State!");
            break;
        }
    }

    /* Send discover state to application if needed. */
    if (cb_flag && hrs_client_cb)
    {
        cb_data.cb_content.disc_state = hrs_table[conn_id].disc_state;
        (*hrs_client_cb)(hrs_client, conn_id, &cb_data);
    }
    return;
}


static void hrs_client_discover_result_cb(uint8_t conn_id,  T_DISCOVERY_RESULT_TYPE result_type,
                                          T_DISCOVERY_RESULT_DATA result_data)
{
    PROFILE_PRINT_INFO1("hrs_client_discover_result_cb: result_type = %d", result_type);
    if (hrs_table[conn_id].disc_state == DISC_HRS_START)
    {
        switch (result_type)
        {
        case DISC_RESULT_SRV_DATA:
            hrs_table[conn_id].hdl_cache[HDL_HRS_SRV_START] =
                result_data.p_srv_disc_data->att_handle;
            hrs_table[conn_id].hdl_cache[HDL_HRS_SRV_END] =
                result_data.p_srv_disc_data->end_group_handle;
            break;

        case DISC_RESULT_CHAR_UUID16:
            {
                uint16_t handle;
                handle = result_data.p_char_uuid16_disc_data->value_handle;
                if (result_data.p_char_uuid16_disc_data->uuid16 == GATT_UUID_CHAR_HRS_HEART_RATE_MEASUREMENT)
                {
                    hrs_table[conn_id].hdl_cache[HDL_HRS_HEART_RATE_MEASUREMENT] = handle;
                    hrs_table[conn_id].properties = result_data.p_char_uuid16_disc_data->properties;
                }
            }
            break;

        case DISC_RESULT_CHAR_DESC_UUID16:
            if (result_data.p_char_desc_uuid16_disc_data->uuid16 == GATT_UUID_CHAR_CLIENT_CONFIG)
            {
                uint16_t handle;
                handle = result_data.p_char_desc_uuid16_disc_data->handle;
                if (handle == hrs_table[conn_id].hdl_cache[HDL_HRS_HEART_RATE_MEASUREMENT] + 1)
                {
                    hrs_table[conn_id].hdl_cache[HDL_HRS_HEART_RATE_MEASUREMENT_CCCD] = handle;
                }
            }
            break;

        default:
            PROFILE_PRINT_ERROR0("Invalid Discovery Result Type!");
            break;
        }
    }

    return;
}

static void hrs_client_read_result_cb(uint8_t conn_id,  uint16_t cause,
                                      uint16_t handle, uint16_t value_size, uint8_t *p_value)
{
    T_HRS_CLIENT_CB_DATA cb_data;
    uint16_t *hdl_cache;
    hdl_cache = hrs_table[conn_id].hdl_cache;
    cb_data.cb_type = HRS_CLIENT_CB_TYPE_READ_RESULT;

    PROFILE_PRINT_INFO2("hrs_client_read_result_cb: handle 0x%x, cause 0x%x", handle, cause);
    if (handle == hdl_cache[HDL_HRS_HEART_RATE_MEASUREMENT])
    {
        cb_data.cb_content.read_result.cause = cause;
    }

    if (hrs_client_cb)
    {
        (*hrs_client_cb)(hrs_client, conn_id, &cb_data);
    }
    return;
}

static void hrs_client_write_result_cb(uint8_t conn_id, T_GATT_WRITE_TYPE type,
                                       uint16_t handle,
                                       uint16_t cause,
                                       uint8_t credits)
{
    T_HRS_CLIENT_CB_DATA cb_data;
    uint16_t *hdl_cache;
    hdl_cache = hrs_table[conn_id].hdl_cache;
    cb_data.cb_type = HRS_CLIENT_CB_TYPE_WRITE_RESULT;

    PROFILE_PRINT_INFO2("hrs_client_write_result_cb: handle 0x%x, cause 0x%x", handle, cause);
    cb_data.cb_content.write_result.cause = cause;

    if (handle == hdl_cache[HDL_HRS_HEART_RATE_MEASUREMENT_CCCD])
    {
        if (hrs_table[conn_id].write_notify_value)
        {
            cb_data.cb_content.write_result.type = HRS_WRITE_HEART_RATE_MEASUREMENT_NOTIFY_ENABLE;
        }
        else
        {
            cb_data.cb_content.write_result.type = HRS_WRITE_HEART_RATE_MEASUREMENT_NOTIFY_DISABLE;
        }
    }
    else
    {
        return;
    }

    if (hrs_client_cb)
    {
        (*hrs_client_cb)(hrs_client, conn_id, &cb_data);
    }
    return;
}

static T_APP_RESULT hrs_client_notify_ind_cb(uint8_t conn_id, bool notify, uint16_t handle,
                                             uint16_t value_size, uint8_t *p_value)
{
    T_APP_RESULT app_result = APP_RESULT_SUCCESS;
    T_HRS_CLIENT_CB_DATA cb_data;
    uint16_t *hdl_cache;

    hdl_cache = hrs_table[conn_id].hdl_cache;
    cb_data.cb_type = HRS_CLIENT_CB_TYPE_NOTIF_IND_RESULT;

    if (handle == hdl_cache[HDL_HRS_HEART_RATE_MEASUREMENT])
    {
        cb_data.cb_content.notify_data.type = HRS_FROM_HEART_RATE_MEASUREMENT;
        cb_data.cb_content.notify_data.value_size = value_size;
        cb_data.cb_content.notify_data.p_value = p_value;
    }
    else
    {
        return APP_RESULT_SUCCESS;
    }

    if (hrs_client_cb)
    {
        app_result = (*hrs_client_cb)(hrs_client, conn_id, &cb_data);
    }

    return app_result;
}

static void hrs_client_disc_cb(uint8_t conn_id)
{
    PROFILE_PRINT_INFO0("hrs_client_disc_cb.");
    if (conn_id >= hrs_link_num)
    {
        PROFILE_PRINT_ERROR1("hrs_client_disc_cb: failed invalid conn_id %d", conn_id);
        return;
    }
    memset(&hrs_table[conn_id], 0, sizeof(T_HRS_LINK));
    return;
}
/**
 * @brief HRS Client Callbacks.
*/
const T_FUN_CLIENT_CBS hrs_client_cbs =
{
    hrs_client_discover_state_cb,   //!< Discovery State callback function pointer
    hrs_client_discover_result_cb,  //!< Discovery result callback function pointer
    hrs_client_read_result_cb,      //!< Read response callback function pointer
    hrs_client_write_result_cb,     //!< Write result callback function pointer
    hrs_client_notify_ind_cb,       //!< Notify Indicate callback function pointer
    hrs_client_disc_cb              //!< Link disconnection callback function pointer
};

/**
  * @brief      Add hrs client to application.
  * @param[in]  app_cb pointer of app callback function to handle specific client module data.
  * @param[in]  link_num initialize link num.
  * @return Client ID of the specific client module.
  * @retval 0xff failed.
  * @retval other success.
  *
  * <b>Example usage</b>
  * \code{.c}
    void app_le_profile_init(void)
    {
        client_init(1);
        hrs_client_id = hrs_add_client(app_client_callback, APP_MAX_LINKS);
    }
  * \endcode
  */
T_CLIENT_ID hrs_add_client(P_FUN_GENERAL_APP_CB app_cb, uint8_t link_num)
{
    uint16_t size;
    if (link_num > HRS_MAX_LINKS)
    {
        PROFILE_PRINT_ERROR1("hrs_add_client: invalid link_num %d", link_num);
        return 0xff;
    }
    if (false == client_register_spec_client_cb(&hrs_client, &hrs_client_cbs))
    {
        hrs_client = CLIENT_PROFILE_GENERAL_ID;
        PROFILE_PRINT_ERROR0("hrs_add_client:register fail");
        return hrs_client;
    }
    PROFILE_PRINT_INFO1("hrs_add_client: client id %d", hrs_client);

    /* register callback for profile to inform application that some events happened. */
    hrs_client_cb = app_cb;
    hrs_link_num = link_num;
    size = hrs_link_num * sizeof(T_HRS_LINK);
    hrs_table = os_mem_zalloc(RAM_TYPE_DATA_ON, size);

    return hrs_client;
}

