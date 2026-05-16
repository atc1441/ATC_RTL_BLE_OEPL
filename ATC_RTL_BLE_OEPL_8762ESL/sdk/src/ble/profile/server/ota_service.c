/**
*********************************************************************************************************
*               Copyright(c) 2015, Realtek Semiconductor Corporation. All rights reserved.
**********************************************************************************************************
* @file     ota_service.c
* @brief
* @details
* @author   Ken_mei
* @date     14-May-2018
* @version  v1.0.0
******************************************************************************
* @attention
* <h2><center>&copy; COPYRIGHT 2015 Realtek Semiconductor Corporation</center></h2>
******************************************************************************
*/

#include <string.h>
#include "gatt.h"
#include "patch_header_check.h"
#include "ota_service.h"
#include "otp.h"
#include "trace.h"
#include "board.h"

/*============================================================================*
 *                              Micro
 *============================================================================*/
#define SOCV_CFG    0
#define SYS_CFG     1
#define OTA_HEADER  2
#define SECURE_BOOT 3
#define ROM_PATCH   4
#define APP_IMG     5
#define APP_DATA1   6
#define APP_DATA2   7
#define APP_DATA3   8
#define APP_DATA4   9
#define APP_DATA5   10
#define APP_DATA6   11

#define IMAGE_NOEXIST         0
#define IMAGE_LOCATION_BANK0  1
#define IMAGE_LOCATION_BANK1  2
#define IMAGE_FIX_BANK_EXIST  3

typedef struct
{
    uint8_t image_num;
    uint16_t image_exist;
    uint32_t image_indicator;
} T_OTA_ACTIVE_BANK_IMG_INFO;

/*============================================================================*
 *                              External Variables
 *============================================================================*/

/*============================================================================*
 *                              Local Variables
 *============================================================================*/
/**<  Function pointer used to send event to application from BWPS extended profile. */
P_FUN_SERVER_GENERAL_CB pfn_ota_service_cb = NULL;
uint8_t mac_addr[6];
uint32_t patch_version = 0;
uint32_t patch_ext_version = 0;
uint32_t app_version = 0;
#if (SUPPORT_OTA_PROTOCOL_TYPE_CHARACTERISTIC == 1)
uint16_t protocol_type = 0x0;
#endif
static uint32_t image_version[IMAGE_MAX - OTA] = {0};
T_OTA_ACTIVE_BANK_IMG_INFO ota_active_bank_info;

const uint8_t GATT_UUID_OTA_SERVICE[16] = { 0x12, 0xA2, 0x4D, 0x2E, 0xFE, 0x14, 0x48, 0x8e, 0x93, 0xD2, 0x17, 0x3C, 0xFF, 0xD0, 0x00, 0x00};

/**< @brief  profile/service definition.
*   here is an example of OTA service table
*   including Write
*/
const T_ATTRIB_APPL gatt_Ota_service_table[] =
{
    /*--------------------------OTA Service ---------------------------*/
    /* <<Primary Service>>, .. 0*/
    {
        (ATTRIB_FLAG_VOID | ATTRIB_FLAG_LE),  /* wFlags     */
        {
            LO_WORD(GATT_UUID_PRIMARY_SERVICE),
            HI_WORD(GATT_UUID_PRIMARY_SERVICE),     /* bTypeValue */
        },
        UUID_128BIT_SIZE,                     /* bValueLen     */
        (void *)GATT_UUID_OTA_SERVICE,        /* pValueContext */
        GATT_PERM_READ                              /* wPermissions  */
    },

    /* <<Characteristic1>>, .. 1*/                         //----------------------OTA CMD        1
    {
        ATTRIB_FLAG_VALUE_INCL,                     /* wFlags */
        {   /* bTypeValue */
            LO_WORD(GATT_UUID_CHARACTERISTIC),
            HI_WORD(GATT_UUID_CHARACTERISTIC),
            GATT_CHAR_PROP_WRITE_NO_RSP,                    /* characteristic properties */
            //XXXXMJMJ GATT_CHAR_PROP_INDICATE,                  /* characteristic properties */
            /* characteristic UUID not needed here, is UUID of next attrib. */
        },
        1,                                          /* bValueLen */
        NULL,
        GATT_PERM_READ                              /* wPermissions */
    },
    /*  OTA characteristic value 2*/
    {
        ATTRIB_FLAG_VALUE_APPL,                     /* wFlags */
        {   /* bTypeValue */
            LO_WORD(GATT_UUID_CHAR_OTA),
            HI_WORD(GATT_UUID_CHAR_OTA),
        },
        2,                                          /* variable size */
        (void *)NULL,
        GATT_PERM_READ | GATT_PERM_WRITE            /* wPermissions */
    },

    /* <<Characteristic2>>, .. 3, MAC Address*/              //------------------------MAC Address  2
    {
        ATTRIB_FLAG_VALUE_INCL,                     /* wFlags */
        {   /* bTypeValue */
            LO_WORD(GATT_UUID_CHARACTERISTIC),
            HI_WORD(GATT_UUID_CHARACTERISTIC),
            GATT_CHAR_PROP_READ,                    /* characteristic properties */
            //XXXXMJMJ GATT_CHAR_PROP_INDICATE,                  /* characteristic properties */
            /* characteristic UUID not needed here, is UUID of next attrib. */
        },
        1,                                          /* bValueLen */
        NULL,
        GATT_PERM_READ                              /* wPermissions */
    },
    /*  OTA characteristic value 4*/
    {
        ATTRIB_FLAG_VALUE_APPL,                     /* wFlags */
        {   /* bTypeValue */
            LO_WORD(GATT_UUID_CHAR_MAC),
            HI_WORD(GATT_UUID_CHAR_MAC),
        },
        1,                                          /* variable size */
        (void *)NULL,
        GATT_PERM_READ            /* wPermissions */
    },

    /* <<Characteristic3>>, .. 5, Patch version*/            //-------------------------Patch Version  3
    {
        ATTRIB_FLAG_VALUE_INCL,                     /* wFlags */
        {   /* bTypeValue */
            LO_WORD(GATT_UUID_CHARACTERISTIC),
            HI_WORD(GATT_UUID_CHARACTERISTIC),
            GATT_CHAR_PROP_READ,                    /* characteristic properties */
            //XXXXMJMJ GATT_CHAR_PROP_INDICATE,                  /* characteristic properties */
            /* characteristic UUID not needed here, is UUID of next attrib. */
        },
        1,                                          /* bValueLen */
        NULL,
        GATT_PERM_READ                              /* wPermissions */
    },
    /*  OTA characteristic value 6*/
    {
        ATTRIB_FLAG_VALUE_APPL,                     /* wFlags */
        {   /* bTypeValue */
            LO_WORD(GATT_UUID_CHAR_PATCH),
            HI_WORD(GATT_UUID_CHAR_PATCH),
        },
        1,                                          /* variable size */
        (void *)NULL,
        GATT_PERM_READ            /* wPermissions */
    },
    /* <<Characteristic4>>, .. 7 App version*/            //-----------------------------APP Version   4
    {
        ATTRIB_FLAG_VALUE_INCL,                     /* wFlags */
        {   /* bTypeValue */
            LO_WORD(GATT_UUID_CHARACTERISTIC),
            HI_WORD(GATT_UUID_CHARACTERISTIC),
            GATT_CHAR_PROP_READ,                    /* characteristic properties */
            //XXXXMJMJ GATT_CHAR_PROP_INDICATE,                  /* characteristic properties */
            /* characteristic UUID not needed here, is UUID of next attrib. */
        },
        1,                                          /* bValueLen */
        NULL,
        GATT_PERM_READ                              /* wPermissions */
    },
    /*  OTA characteristic value 8*/
    {
        ATTRIB_FLAG_VALUE_APPL,                     /* wFlags */
        {   /* bTypeValue */
            LO_WORD(GATT_UUID_CHAR_APP_VERSION),
            HI_WORD(GATT_UUID_CHAR_APP_VERSION),
        },
        1,                                          /* variable size */
        (void *)NULL,
        GATT_PERM_READ            /* wPermissions */
    },
    /* <<Characteristic5>>, .. 9 Patch extension version*/  //--------------------Patch Ext Version    5
    {
        ATTRIB_FLAG_VALUE_INCL,                     /* wFlags */
        {   /* bTypeValue */
            LO_WORD(GATT_UUID_CHARACTERISTIC),
            HI_WORD(GATT_UUID_CHARACTERISTIC),
            GATT_CHAR_PROP_READ,                    /* characteristic properties */
            //XXXXMJMJ GATT_CHAR_PROP_INDICATE,                  /* characteristic properties */
            /* characteristic UUID not needed here, is UUID of next attrib. */
        },
        1,                                          /* bValueLen */
        NULL,
        GATT_PERM_READ                              /* wPermissions */
    },
    /*  OTA characteristic value 0x0A*/
    {
        ATTRIB_FLAG_VALUE_APPL,                     /* wFlags */
        {   /* bTypeValue */
            LO_WORD(GATT_UUID_CHAR_PATCH_EXTENSION),
            HI_WORD(GATT_UUID_CHAR_PATCH_EXTENSION),
        },
        1,                                          /* variable size */
        (void *)NULL,
        GATT_PERM_READ            /* wPermissions */
    },

    /* <<Characteristic6>>, .. 0xB TEST MODE*/                 //------------------------MP TEST MODE     6
    {
        ATTRIB_FLAG_VALUE_INCL,                     /* wFlags */
        {   /* bTypeValue */
            LO_WORD(GATT_UUID_CHARACTERISTIC),
            HI_WORD(GATT_UUID_CHARACTERISTIC),
            GATT_CHAR_PROP_WRITE_NO_RSP,                    /* characteristic properties */
            //XXXXMJMJ GATT_CHAR_PROP_INDICATE,                  /* characteristic properties */
            /* characteristic UUID not needed here, is UUID of next attrib. */
        },
        1,                                          /* bValueLen */
        NULL,
        GATT_PERM_READ                              /* wPermissions */
    },
    /*  OTA characteristic value 0x0C*/
    {
        ATTRIB_FLAG_VALUE_APPL,                     /* wFlags */
        {   /* bTypeValue */
            LO_WORD(GATT_UUID_CHAR_TEST_MODE),
            HI_WORD(GATT_UUID_CHAR_TEST_MODE),
        },
        0,                                          /* variable size */
        (void *)NULL,
        GATT_PERM_WRITE            /* wPermissions */
    },

    /* <<Characteristic7>>, .. 0x0D OTA Device info*/    //---------------------------Device info    7
    {
        ATTRIB_FLAG_VALUE_INCL,
        {
            LO_WORD(GATT_UUID_CHARACTERISTIC),
            HI_WORD(GATT_UUID_CHARACTERISTIC),
            GATT_CHAR_PROP_READ,
        },
        1,
        NULL,
        GATT_PERM_READ
    },
    /*  OTA Device info characteristic value 0x0E*/
    {
        ATTRIB_FLAG_VALUE_APPL,
        {
            LO_WORD(GATT_UUID_CHAR_DEVICE_INFO),
            HI_WORD(GATT_UUID_CHAR_DEVICE_INFO),
        },
        1,
        (void *)NULL,
        GATT_PERM_READ
    },
    /* <<Characteristic8>>, .. 0x0F OTA IMAGE COUNT TO UPDATE*/ //----------------------NUM OF IMG TO UPDATA  8
    {
        ATTRIB_FLAG_VALUE_INCL,
        {
            LO_WORD(GATT_UUID_CHARACTERISTIC),
            HI_WORD(GATT_UUID_CHARACTERISTIC),
            GATT_CHAR_PROP_WRITE,
        },
        1,
        NULL,
        GATT_PERM_READ
    },
    /*  OTA IMAGE COUNT TO UPDATE characteristic value 0x10*/
    {
        ATTRIB_FLAG_VALUE_APPL,
        {
            LO_WORD(GATT_UUID_CHAR_IMAGE_COUNT_TO_UPDATE),
            HI_WORD(GATT_UUID_CHAR_IMAGE_COUNT_TO_UPDATE),
        },
        5,
        (void *)NULL,
        GATT_PERM_WRITE
    },

    /* <<Characteristic9>>, .. 0x11,ota pack img version*/            //-------------------------ota pack image version  9
    {
        ATTRIB_FLAG_VALUE_INCL,                     /* wFlags */
        {   /* bTypeValue */
            LO_WORD(GATT_UUID_CHARACTERISTIC),
            HI_WORD(GATT_UUID_CHARACTERISTIC),
            GATT_CHAR_PROP_READ,                    /* characteristic properties */
            //XXXXMJMJ GATT_CHAR_PROP_INDICATE,                  /* characteristic properties */
            /* characteristic UUID not needed here, is UUID of next attrib. */
        },
        1,                                          /* bValueLen */
        NULL,
        GATT_PERM_READ                              /* wPermissions */
    },
    /*  OTA characteristic value 0x12*/
    {
        ATTRIB_FLAG_VALUE_APPL,                     /* wFlags */
        {   /* bTypeValue */
            LO_WORD(GATT_UUID_CHAR_IMAGE_VERSION0),
            HI_WORD(GATT_UUID_CHAR_IMAGE_VERSION0),
        },
        1,                                          /* variable size */
        (void *)NULL,
        GATT_PERM_READ            /* wPermissions */
    },

    /* <<Characteristic10>>, .. 0x13,ota pack img version*/            //-------------------------ota pack image version  9
    {
        ATTRIB_FLAG_VALUE_INCL,                     /* wFlags */
        {   /* bTypeValue */
            LO_WORD(GATT_UUID_CHARACTERISTIC),
            HI_WORD(GATT_UUID_CHARACTERISTIC),
            GATT_CHAR_PROP_READ,                    /* characteristic properties */
            //XXXXMJMJ GATT_CHAR_PROP_INDICATE,                  /* characteristic properties */
            /* characteristic UUID not needed here, is UUID of next attrib. */
        },
        1,                                          /* bValueLen */
        NULL,
        GATT_PERM_READ                              /* wPermissions */
    },
    /*  OTA characteristic value 0x14*/
    {
        ATTRIB_FLAG_VALUE_APPL,                     /* wFlags */
        {   /* bTypeValue */
            LO_WORD(GATT_UUID_CHAR_IMAGE_VERSION1),
            HI_WORD(GATT_UUID_CHAR_IMAGE_VERSION1),
        },
        1,                                          /* variable size */
        (void *)NULL,
        GATT_PERM_READ            /* wPermissions */
    },

    /* <<Characteristic11>>, .. 0x15,ota pack img version*/            //-------------------------ota pack image version  9
    {
        ATTRIB_FLAG_VALUE_INCL,                     /* wFlags */
        {   /* bTypeValue */
            LO_WORD(GATT_UUID_CHARACTERISTIC),
            HI_WORD(GATT_UUID_CHARACTERISTIC),
            GATT_CHAR_PROP_READ,                    /* characteristic properties */
            //XXXXMJMJ GATT_CHAR_PROP_INDICATE,                  /* characteristic properties */
            /* characteristic UUID not needed here, is UUID of next attrib. */
        },
        1,                                          /* bValueLen */
        NULL,
        GATT_PERM_READ                              /* wPermissions */
    },
    /*  OTA characteristic value 0x16*/
    {
        ATTRIB_FLAG_VALUE_APPL,                     /* wFlags */
        {   /* bTypeValue */
            LO_WORD(GATT_UUID_CHAR_IMAGE_VERSION2),
            HI_WORD(GATT_UUID_CHAR_IMAGE_VERSION2),
        },
        1,                                          /* variable size */
        (void *)NULL,
        GATT_PERM_READ            /* wPermissions */
    },

#if (SUPPORT_OTA_PROTOCOL_TYPE_CHARACTERISTIC == 1)
    /* <<Characteristic10>>, .. 0x17,ota protocol type*/            //-------------------------ota protocol type  10
    {
        ATTRIB_FLAG_VALUE_INCL,                     /* wFlags */
        {   /* bTypeValue */
            LO_WORD(GATT_UUID_CHARACTERISTIC),
            HI_WORD(GATT_UUID_CHARACTERISTIC),
            GATT_CHAR_PROP_READ,                    /* characteristic properties */
            //XXXXMJMJ GATT_CHAR_PROP_INDICATE,                  /* characteristic properties */
            /* characteristic UUID not needed here, is UUID of next attrib. */
        },
        1,                                          /* bValueLen */
        NULL,
        GATT_PERM_READ                              /* wPermissions */
    },
    /*  OTA characteristic value 0x14*/
    {
        ATTRIB_FLAG_VALUE_APPL,                     /* wFlags */
        {   /* bTypeValue */
            LO_WORD(GATT_UUID_CHAR_PROTOCOL_TYPE),
            HI_WORD(GATT_UUID_CHAR_PROTOCOL_TYPE),
        },
        1,                                          /* variable size */
        (void *)NULL,
        GATT_PERM_READ            /* wPermissions */
    },
#endif
};


/**
 * @brief write characteristic data from service.
 *
 * @param ServiceID          ServiceID to be written.
 * @param iAttribIndex       Attribute index of characteristic.
 * @param wLength            length of value to be written.
 * @param pValue             value to be written.
 * @return Profile procedure result
*/

T_APP_RESULT ota_attr_write_cb(uint8_t conn_id, uint8_t service_id, uint16_t attrib_index,
                               T_WRITE_TYPE write_type,
                               uint16_t length, uint8_t *p_value, P_FUN_WRITE_IND_POST_PROC *p_write_ind_post_proc)
{
    T_OTA_CALLBACK_DATA callback_data;
    T_APP_RESULT  wCause = APP_RESULT_SUCCESS;

    if (BLE_SERVICE_CHAR_OTA_INDEX == attrib_index)
    {
        /* Make sure written value size is valid. */
        if ((length != sizeof(uint8_t)) || (p_value == NULL))
        {
            wCause  = APP_RESULT_INVALID_VALUE_SIZE;
        }
        else
        {
            /* Notify Application. */
            callback_data.msg_type = SERVICE_CALLBACK_TYPE_WRITE_CHAR_VALUE;
            callback_data.msg_data.write.opcode = OTA_WRITE_CHAR_VAL;
            callback_data.msg_data.write.u.value = p_value[0];

            if (pfn_ota_service_cb)
            {
                pfn_ota_service_cb(service_id, (void *)&callback_data);
            }
        }
    }
    else if (BLE_SERVICE_CHAR_IMAGE_COUNT_INDEX == attrib_index)
    {
        /* Make sure written value size is valid. */
        if ((length != OTA_CHAR_IMAGE_COUNT_LEN) || (p_value == NULL))
        {
            wCause  = APP_RESULT_INVALID_VALUE_SIZE;
        }
        else
        {
            /* Notify Application. */
            callback_data.msg_type = SERVICE_CALLBACK_TYPE_WRITE_CHAR_VALUE;
            callback_data.msg_data.write.opcode = OTA_WRITE_IMAGE_COUNT_VAL;
            callback_data.msg_data.write.u.update_image_info.image_count = p_value[0];
            callback_data.msg_data.write.u.update_image_info.update_patch_version = CHAR2SHORT(&p_value[1]);
            callback_data.msg_data.write.u.update_image_info.update_app_version = CHAR2SHORT(&p_value[3]);

            if (pfn_ota_service_cb)
            {
                pfn_ota_service_cb(service_id, (void *)&callback_data);
            }
        }
    }
    else if (BLE_SERVICE_CHAR_TEST_MODE_INDEX == attrib_index)
    {
        /* Make sure written value size is valid. */
        if ((length != sizeof(uint8_t)) || (p_value == NULL))
        {
            wCause  = APP_RESULT_INVALID_VALUE_SIZE;
        }
        else
        {
            /* Notify Application. */
            callback_data.msg_type = SERVICE_CALLBACK_TYPE_WRITE_CHAR_VALUE;
            callback_data.msg_data.write.opcode = OTA_WRITE_TEST_MODE_CHAR_VAL;
            callback_data.msg_data.write.u.value = p_value[0];

            if (pfn_ota_service_cb)
            {
                pfn_ota_service_cb(service_id, (void *)&callback_data);
            }
        }
    }
    else
    {
        DFU_PRINT_INFO2("==>[OTA]ota_attr_write_cb Error! attrib_index=0x%x, length=%d",
                        attrib_index,
                        length);
        wCause = APP_RESULT_ATTR_NOT_FOUND;
    }
    return wCause;

}

/**
 * @brief  get OTA active bank image info
 * @param[out]  p_ota_image_info
 * image_exist
 * bit0: ota header, bit: fsbl, bit2: patch ..., bit9: appdata6, bit10: upperstack
 * img_indicator
 * Indications for each image version.each indication use 2bit.
 * 00: image is not existed.
 * 01: image is existed in bank0,ota should update image for bank1.
 * 10: image is existed in bank1,ota should update image for bank0.
 * 11: image is standalone. ota should update image for standalone.
 *
 * bit[1:0]: Image 0
 * bit[2N+1:2N]:Image N

 * Image indicator for bee3 is as below:
 * Image 0   OTA Header File
 * Image 1   Secure Boot Loader Image
 * Image 2   ROM Patch Image
 * Image 3   APP Image
 * Image 4   APP Data1 File
 * Image 5   APP Data2 File
 * Image 6   APP Data3 File
 * Image 7   APP Data4 File
 * Image 8   APP Data5 File
 * Image 9   APP Data6 File
 * Image 10  Upperstack File
*/
static void get_ota_active_bank_image_info(T_OTA_ACTIVE_BANK_IMG_INFO *p_ota_image_info)
{
    uint32_t image_location = IMAGE_NOEXIST;
    bool enable_bank_switch = is_ota_support_bank_switch();
    p_ota_image_info->image_num = 0;

    if (enable_bank_switch)
    {
        uint32_t ota_bank0_addr = flash_get_bank_addr(FLASH_OTA_BANK_0);
        if (ota_bank0_addr == get_active_ota_bank_addr())
        {
            image_location = IMAGE_LOCATION_BANK0;
        }
        else
        {
            image_location = IMAGE_LOCATION_BANK1;
        }
    }
    else
    {
        image_location = IMAGE_FIX_BANK_EXIST;
    }

    for (T_IMG_ID image_id = OTA; image_id < IMAGE_MAX; image_id++)
    {
        if (0 != get_active_bank_image_size_by_img_id(image_id))
        {
            p_ota_image_info->image_num++;
            p_ota_image_info->image_exist |= BIT(image_id - OTA);
            p_ota_image_info->image_indicator |= image_location << ((OTA_HEADER + image_id - OTA) * 2);
        }
    }
}
/**
 * @brief read characteristic data from service.
 *
 * @param ServiceId          ServiceID of characteristic data.
 * @param iAttribIndex       Attribute index of getting characteristic data.
 * @param iOffset            Used for Blob Read.
 * @param piLength           length of getting characteristic data.
 * @param ppValue            data got from service.
 * @return Profile procedure result
*/
T_APP_RESULT ota_attr_read_cb(uint8_t conn_id, T_SERVER_ID service_id, uint16_t attrib_index,
                              uint16_t offset, uint16_t *p_length, uint8_t **pp_value)
{
    T_APP_RESULT  wCause = APP_RESULT_SUCCESS;
    T_OTA_CALLBACK_DATA callback_data;
    bool defer_handle = false;

    switch (attrib_index)
    {
    default:
        DFU_PRINT_INFO1("==>ota_attr_read_cb: attrib_index=%d not found!", attrib_index);
        wCause  = APP_RESULT_ATTR_NOT_FOUND;
        break;
    case BLE_SERVICE_CHAR_MAC_ADDRESS_INDEX:
        {
            gap_get_param(GAP_PARAM_BD_ADDR, mac_addr);
            uint8_t addr[6];
            for (int i = 0; i < 6; i++)
            {
                addr[i] = mac_addr[5 - i];
            }
            memcpy(mac_addr, addr, 6);
            *pp_value  = (uint8_t *)mac_addr;
            *p_length = sizeof(mac_addr);
        }
        break;
    case BLE_SERVICE_CHAR_PATCH_INDEX:
        {
            T_IMG_HEADER_FORMAT *p_header;
            uint32_t addr = get_header_addr_by_img_id(RomPatch);
            p_header = (T_IMG_HEADER_FORMAT *)addr;
            patch_version = p_header->git_ver.ver_info.version;
            DFU_PRINT_INFO2("ota_attr_read_cb: Patch addr=0x%x, version=0x%x", addr, patch_version);

            *pp_value  = (uint8_t *)&patch_version;
            *p_length = sizeof(patch_version);
        }
        break;

    case BLE_SERVICE_CHAR_APP_VERSION_INDEX:
        {
            T_IMG_HEADER_FORMAT *p_header;
            uint32_t addr = get_header_addr_by_img_id(AppPatch);
            p_header = (T_IMG_HEADER_FORMAT *)addr;
            app_version = p_header->git_ver.ver_info.version;
            DFU_PRINT_INFO2("ota_attr_read_cb: APP addr=0x%x, version=0x%x", addr, app_version);

            *pp_value  = (uint8_t *)&app_version;
            *p_length = sizeof(app_version);
        }
        break;

    case BLE_SERVICE_CHAR_PATCH_EXTENSION_INDEX:        //not used in bee2
        {

        }
        break;
    case BLE_SERVICE_CHAR_DEVICE_INFO_INDEX:
        {
            static T_DFU_DEVICE_INFO DeviceInfo;
            T_IMG_HEADER_FORMAT *p_ota_header;
            uint32_t ota_header_addr = get_header_addr_by_img_id(OTA);
            p_ota_header = (T_IMG_HEADER_FORMAT *)ota_header_addr;
            DeviceInfo.ic_type = DEFINED_IC_TYPE;
            DeviceInfo.ota_version = 0x1;
            DeviceInfo.secure_version = p_ota_header->ctrl_header.secure_version;
            DFU_PRINT_INFO3("<==ota_attr_read_cb: ic_type=0x%x, ota_version=0x%x, OTA Header secure_version=0x%x",
                            DeviceInfo.ic_type, DeviceInfo.ota_version, DeviceInfo.secure_version);

            DeviceInfo.ota_mode.mode_flag.buf_check_en = DFU_BUFFER_CHECK_ENABLE;
            DeviceInfo.max_buffer_size = DFU_TEMP_BUFFER_SIZE;
            DeviceInfo.ota_mode.mode_flag.aesflg = OTP->ota_with_encryption_data;
            DeviceInfo.ota_mode.mode_flag.aesmode = 1;
            DFU_PRINT_INFO4("<==ota_attr_read_cb: bufChkEn=%d, bufsize=0x%x, aesflg=%d, aesmode=%d",
                            DeviceInfo.ota_mode.mode_flag.buf_check_en, DeviceInfo.max_buffer_size,
                            DeviceInfo.ota_mode.mode_flag.aesflg, DeviceInfo.ota_mode.mode_flag.aesmode);

            /* default disable copy app data img when bank switch */
            DeviceInfo.ota_mode.mode_flag.copy_img = 0;

            /* prepare img_indicator, if modify here need to sync with vaule
                of BLE_SERVICE_CHAR_IMAGE_VERSION_INDEX */
            if (is_ota_support_bank_switch())
            {
                //dual bank must support multiple image update
                DeviceInfo.ota_mode.mode_flag.multi_img = 1;
                uint32_t ota_bank0_addr = flash_get_bank_addr(FLASH_OTA_BANK_0);
                if (ota_bank0_addr == get_active_ota_bank_addr())
                {
                    /* meaningful only when multi_img = 1 */
                    DeviceInfo.temp_bank_size = flash_get_bank_size(FLASH_OTA_BANK_1) / FMC_SEC_SECTION_LEN;
                }
                else
                {
                    /* meaningful only when multi_img = 1 */
                    DeviceInfo.temp_bank_size = flash_get_bank_size(FLASH_OTA_BANK_0) / FMC_SEC_SECTION_LEN;
                }
            }
            else
            {
#if (SUPPORT_TEMP_COMBINED_OTA == 1)
                DeviceInfo.ota_mode.mode_flag.multi_img = 1;
#else
                DeviceInfo.ota_mode.mode_flag.multi_img = 0;
#endif
                /* meaningful only when multi_img = 1 */
                DeviceInfo.temp_bank_size = flash_get_bank_size(FLASH_OTA_TMP) / FMC_SEC_SECTION_LEN;
            }
            /*read flash map info to set image indicator*/
            get_ota_active_bank_image_info(&ota_active_bank_info);
            DeviceInfo.img_indicator = ota_active_bank_info.image_indicator;

            DFU_PRINT_INFO4("<==ota_attr_read_cb: copy_img=0x%x, multi_img=0x%x, temp_bank_size=0x%x, img_indicator=0x%x",
                            DeviceInfo.ota_mode.mode_flag.copy_img, DeviceInfo.ota_mode.mode_flag.multi_img,
                            DeviceInfo.temp_bank_size, DeviceInfo.img_indicator);


            *pp_value  = (uint8_t *)&DeviceInfo;
            *p_length = sizeof(T_DFU_DEVICE_INFO);
        }
        break;
    case BLE_SERVICE_CHAR_IMAGE_VERSION0_INDEX:
        {
            uint8_t index = 0;
            T_IMAGE_VERSION image_ver;

            if (0 == ota_active_bank_info.image_num)
            {
                get_ota_active_bank_image_info(&ota_active_bank_info);
            }
            DFU_PRINT_INFO2("image_exist=0x%x(BIT0:OTA, BIT1:patch, ...), image_num=%d",
                            ota_active_bank_info.image_exist, ota_active_bank_info.image_num);

            for (T_IMG_ID image_id = OTA; image_id < IMAGE_MAX; image_id++)
            {
                if (ota_active_bank_info.image_exist & BIT(image_id - OTA))
                {
                    get_active_bank_image_version(image_id, &image_ver);
                    image_version[index] = image_ver.ver_info.version;
                    index ++;
                    DFU_PRINT_INFO2("image:0x%x exist!, version=0x%x",
                                    image_id, image_ver.ver_info.version);
                }
            }

            *pp_value  = (uint8_t *)image_version;
            if (ota_active_bank_info.image_num > 5)
            {
                *p_length = 20;
            }
            else
            {
                *p_length = 4 * ota_active_bank_info.image_num;
            }
            DFU_PRINT_INFO5("Image Version[0-4]: 0x%x, 0x%x, 0x%x, 0x%x, 0x%x",
                            image_version[0], image_version[1], image_version[2], image_version[3], image_version[4]);

            /* Notify Application. */
            callback_data.msg_type = SERVICE_CALLBACK_TYPE_READ_CHAR_VALUE;
            callback_data.msg_data.read_value_index = OTA_READ_CHAR_IMAGE_VERSION_INDEX;
            /*defer processing before ota start*/
            defer_handle = true;
        }
        break;
    case BLE_SERVICE_CHAR_IMAGE_VERSION1_INDEX:
        {
            if (ota_active_bank_info.image_num > 5)
            {
                *pp_value = (uint8_t *)&image_version[5];
                if (ota_active_bank_info.image_num > 10)
                {
                    *p_length = 20;
                }
                else
                {
                    *p_length = 4 * (ota_active_bank_info.image_num - 5);
                }
                DFU_PRINT_INFO5("Image Version[5-9]: 0x%x, 0x%x, 0x%x, 0x%x, 0x%x",
                                image_version[5], image_version[6], image_version[7], image_version[8], image_version[9]);
            }
            else
            {
                *pp_value = NULL;
                *p_length = 0;
            }

            /* Notify Application. */
            callback_data.msg_type = SERVICE_CALLBACK_TYPE_READ_CHAR_VALUE;
            callback_data.msg_data.read_value_index = OTA_READ_CHAR_IMAGE_VERSION_INDEX;
            /*defer processing before ota start*/
            defer_handle = true;
        }
        break;
    case BLE_SERVICE_CHAR_IMAGE_VERSION2_INDEX:
        {
            if (ota_active_bank_info.image_num > 10)
            {
                *pp_value = (uint8_t *)&image_version[10];
                *p_length = 4 * (ota_active_bank_info.image_num - 10);
                //ota 11 images at most now
                DFU_PRINT_INFO1("Image Version[10]: 0x%x", image_version[10]);
            }
            else
            {
                *pp_value = NULL;
                *p_length = 0;
            }

            /* Notify Application. */
            callback_data.msg_type = SERVICE_CALLBACK_TYPE_READ_CHAR_VALUE;
            callback_data.msg_data.read_value_index = OTA_READ_CHAR_IMAGE_VERSION_INDEX;
            /*defer processing before ota start*/
            defer_handle = true;
        }
        break;
#if (SUPPORT_OTA_PROTOCOL_TYPE_CHARACTERISTIC == 1)
    case BLE_SERVICE_CHAR_PROTOCOL_TYPE_INDEX:
        {
            protocol_type = 0x0012;
            *pp_value  = (uint8_t *)&protocol_type;
            *p_length = sizeof(protocol_type);
        }
        break;
#endif
    }

    /* Notify Application. */
    if (pfn_ota_service_cb && (defer_handle == true))
    {
        pfn_ota_service_cb(service_id, (void *)&callback_data);
    }
    return (wCause);
}


/**
 * @brief OTA ble Service Callbacks.
*/
const T_FUN_GATT_SERVICE_CBS ota_service_cbs =
{
    ota_attr_read_cb,       // Read callback function pointer
    ota_attr_write_cb,      // Write callback function pointer
    NULL                    // CCCD update callback function pointer
};

/**
 * @brief  add OTA ble service to application.
 *
 * @param  p_func          pointer of app callback function called by profile.
 * @return service ID auto generated by profile layer.
 * @retval ServiceId
*/
uint8_t ota_add_service(void *p_func)
{
    uint8_t service_id;
    if (false == server_add_service(&service_id,
                                    (uint8_t *)gatt_Ota_service_table,
                                    sizeof(gatt_Ota_service_table),
                                    ota_service_cbs))
    {
        DFU_PRINT_ERROR1("<==ota_add_service: service_id=%d", service_id);
        service_id = 0xff;
        return service_id;
    }
    pfn_ota_service_cb = (P_FUN_SERVER_GENERAL_CB)p_func;
    return service_id;
}
