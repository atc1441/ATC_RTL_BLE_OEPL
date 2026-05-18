/**
  ******************************************************************************
  * @file    patch_header_check.h
  * @author
  * @version V0.0.1
  * @date    2017-09-01
  * @brief   This file contains all the functions regarding patch header check.
  ******************************************************************************
  * @attention
  *
  * This module is a confidential and proprietary property of RealTek and
  * possession or use of this module requires written permission of RealTek.
  *
  * Copyright(c) 2017, Realtek Semiconductor Corporation. All rights reserved.
  ******************************************************************************
  */

#ifndef _PATCH_HEADER_CHECK_H_
#define _PATCH_HEADER_CHECK_H_
#include <stdbool.h>
#include <stdint.h>
#include "rtl876x.h"
#include "flash_nor_device.h"

/** @addtogroup  FLASH_DEVICE    Flash Device
    * @{
    */

/*============================================================================*
  *                                   Constants
  *============================================================================*/
/** @defgroup FLASH_DEVICE_Exported_Constants Flash Device Exported Constants
  * @brief
  * @{
  */

#define DEFAULT_HEADER_SIZE           (0x400)
#define CMAC_BYTE_SIZE                (16)
#define RSA_2048_PUBLIC_KEY_BYTE_SIZE (256)
#define RSA_2048_SIGNATURE_BYTE_SIZE  (RSA_2048_PUBLIC_KEY_BYTE_SIZE)
#define RSA_3072_PUBLIC_KEY_BYTE_SIZE (384)
#define RSA_3072_SIGNATURE_BYTE_SIZE  (RSA_3072_PUBLIC_KEY_BYTE_SIZE)
#define SHA256_BYTE_SIZE              (32)
#define SIGNATURE_APP_CB              0x0e85d101 /**< patch callback app signature definition*/
#define UUID_SIZE                     16

/*IC Type refer to WIKI: https://wiki.realtek.com/display/Bee1/BT+SOC+IC+Type*/
#define IMG_IC_TYPE          0x10

#define DFU_HEADER_SIZE          12  /*currently, first 12 byte only will be treated as image header*/
#define IMG_HEADER_SIZE          1024

#define FSBL_EXT_PATTERN         0x736c

#define UP_ALIGN(size, align)    (((size) + (align) - 1) & ~(align - 1))

#define SYS_CFG_SIGNATURE           0x12345bb3
#define FLASH_TABLE_MAGIC_PATTERN   (0x5A5A12A5)

/** End of FLASH_DEVICE_Exported_Constants
  * @}
  */

/*============================================================================*
  *                                   Types
  *============================================================================*/
/** @defgroup FLASH_DEVICE_Exported_Types Flash Device Exported Types
  * @brief
  * @{
  */

typedef enum
{
    IMAGE_FIRST = 0x278D,
    SCCD        = 0x278D,
    OCCD        = 0x278E,
    FactoryCode = 0x278F,
    OTA         = 0x2790, /**< OTA header */
    SecureBoot  = 0x2791,
    RomPatch    = 0x2792,
    AppPatch    = 0x2793,
    AppData1    = 0x2794,
    AppData2    = 0x2795,
    AppData3    = 0x2796,
    AppData4    = 0x2797,
    AppData5    = 0x2798,
    AppConfigFile = 0x2799,
    UpperStack  = 0x279a,
    IMG_BT_STACKPATCH = 0x279b,
    IMAGE_MAX,

    IMAGE_USER_DATA2 = 0xFFFD,  /**< the image only supports unsafe single bank OTA */
    IMAGE_USER_DATA = 0xFFFE,  /**< the image only supports unsafe single bank OTA */
} T_IMG_ID;

typedef enum
{
    ENC_KEY_SCEK = 0,
    ENC_KEY_SCEK_WITH_RTKCONST,
    ENC_KEY_OCEK,
    ENC_KEY_OCEK_WITH_OEMCONST,
    ENC_KEY_ON_FLASH,
    ENC_KEY_MAX,
} T_ENC_KEY_SELECT;

typedef struct
{
    union
    {
        struct
        {
            uint8_t N[RSA_2048_PUBLIC_KEY_BYTE_SIZE];
            uint8_t E[4];
        } rsa_2048_pub_key;

        struct
        {
            uint8_t N[RSA_3072_PUBLIC_KEY_BYTE_SIZE];
            uint8_t E[4];
        } rsa_3072_pub_key;
    };

} T_RSA_PUBLIC_KEY;

typedef enum _APP_IMAGE_TYPE
{
    IMAGE_NORMAL              = 0,
    IMAGE_COMPRESSED          = 1,
    //2-7 are reserved
} T_IMAGE_TYPE;

typedef struct _IMG_CTRL_HEADER_FORMAT
{
    uint16_t crc16;
    uint8_t ic_type;
    uint8_t secure_version;
    union
    {
        uint16_t value;
        struct
        {
            uint16_t xip: 1; // payload is executed on flash
            uint16_t enc: 1; // all the payload is encrypted
            uint16_t load_when_boot: 1; // load image when boot
            uint16_t enc_load: 1; // encrypt load part or not
            uint16_t enc_key_select: 3; // referenced to ENC_KEY_SELECT
            uint16_t not_ready: 1; // for copy image in OTA
            uint16_t not_obsolete: 1; // for copy image in OTA
            uint16_t integrity_check_en_in_boot: 1; // enable image integrity check in boot flow
//            uint16_t header_size: 4; // unit: KB, set for tool
//            uint16_t rsvd: 2;
            uint16_t compressed_not_ready: 1;
            uint16_t compressed_not_obsolete: 1;
            uint16_t rsvd: 1;
            uint16_t image_type: 3; /* for app 000b: normal image, 001b:compressed image, other for more types
            for patch in temp bank consist of 001b: patch+fsbl, 010b: patch+app, 011b: patch+fsbl+app*/
        } flag_value;
    } ctrl_flag;
    uint16_t image_id;
    uint32_t payload_len;
} T_IMG_CTRL_HEADER_FORMAT;

typedef struct _COMPRESS_IMG_HEADER_FORMAT //96 bytes
{
    T_IMG_CTRL_HEADER_FORMAT ctrl_header; //12 bytes
    uint8_t uuid[16];
    uint32_t version;
    uint32_t compress_algo;
    uint8_t sha256[32];
    uint8_t reserved[28];
} T_COMPRESS_IMG_HEADER_FORMAT;

typedef struct
{
    union
    {
        uint32_t version;
        struct
        {
            uint32_t _version_reserve: 8;   //!< reserved
            uint32_t _version_revision: 8; //!< revision version
            uint32_t _version_minor: 8;     //!< minor version
            uint32_t _version_major: 8;     //!< major version
        } header_sub_version; //!< ota header sub version
        struct
        {
            uint32_t _version_major: 4;     //!< major version
            uint32_t _version_minor: 8;     //!< minor version
            uint32_t _version_revision: 15; //!< revision version
            uint32_t _version_reserve: 5;   //!< reserved
        } img_sub_version; //!< other image sub version including patch, app, app data 1-6

    } ver_info;
} T_IMAGE_VERSION;

typedef struct
{
    union
    {
        uint32_t version;
        struct
        {
            uint32_t _version_major: 4;     //!< major version
            uint32_t _version_minor: 8;     //!< minor version
            uint32_t _version_revision: 15; //!< revision version
            uint32_t _version_reserve: 5;   //!< reserved
        } sub_version;
    } ver_info;
    uint32_t _version_commitid;     //!< git commit id
    uint8_t _customer_name[8];      //!< branch name for customer patch
} T_VERSION_FORMAT;

typedef struct _AUTH_HEADER_FORMAT
{
    uint8_t image_mac[CMAC_BYTE_SIZE];
    union
    {
        uint8_t rsa_image_2048_signature[RSA_2048_SIGNATURE_BYTE_SIZE];
        uint8_t rsa_image_3072_signature[RSA_3072_SIGNATURE_BYTE_SIZE];
    } sig;
    uint8_t image_hash[SHA256_BYTE_SIZE];
} T_AUTH_HEADER_FORMAT;

typedef union
{
    uint8_t bytes[24];
    struct
    {
        uint32_t itcm_heap_addr;
        uint32_t app_ram_data_size;
        uint32_t app_heap_data_on_size;
    } app_ram_info;
} T_EXTRA_INFO_FORMAT;

typedef struct _FLASH_SEC_FORMAT
{
    union
    {
        uint32_t value;
        struct
        {
            uint32_t enable: 1;
            uint32_t mode: 2; // gcm mode:0b00, ctr mode:0b01, mix mode:0b10
            uint32_t key_select: 3;
            uint32_t rsvd: 26;
        };
    } ctrl_flag;
    uint32_t base_addr;
    uint32_t region_size;
    uint8_t iv_high[4];
    uint8_t iv_low[4];
} T_FLASH_SEC_FORMAT;

typedef union _IMG_HEADER_FORMAT
{
    uint8_t bytes[DEFAULT_HEADER_SIZE];
    struct
    {
        T_AUTH_HEADER_FORMAT auth;   //16 +384 +32
        T_IMG_CTRL_HEADER_FORMAT ctrl_header;
        uint8_t uuid[16];
        uint32_t exe_base;
        uint32_t load_src;
        uint32_t load_len;
        uint32_t image_base;
        uint16_t dev_id; // for AMZN
        uint16_t flash_layout_size_4k; // ex. 1MB flash used size => flash_layout_size_4k = 1MB/4KB = 256
        uint32_t magic_pattern;
        uint8_t dec_key[16];
        uint32_t load_dst;
        T_EXTRA_INFO_FORMAT ex_info;
        T_VERSION_FORMAT git_ver;
        T_RSA_PUBLIC_KEY PubKey;
        union
        {
            uint8_t reserved_data[92];
            struct
            {
                uint32_t ver_val;
                uint32_t image_info[(IMAGE_MAX - SecureBoot) * 2];
            };
            struct __attribute__((packed))
            {
                uint16_t tool_version;
                uint32_t timestamp;
                T_FLASH_SEC_FORMAT flash_sec_cfg;
                uint16_t fsbl_ext_img_id;
                uint16_t fsbl_ext_load_pattern;
                uint8_t reserved1[62];
            };
        };
    };
} T_IMG_HEADER_FORMAT;

typedef struct _ROM_HEADER_FORMAT
{
    uint8_t uuid[UUID_SIZE];
    void *init_ptr;
    void *entry_ptr;
} T_ROM_HEADER_FORMAT;

typedef enum
{
    IMG_CHECK_PASS,
    IMG_CHECK_ERR_HEADER_ADDR,
    IMG_CHECK_ERR_NOT_READY,
    IMG_CHECK_ERR_ROM_UUID,
    IMG_CHECK_ERR_MAGIC_PATTERN,
    IMG_CHECK_ERR_SIZE,
    IMG_CHECK_ERR_ID,
    IMG_CHECK_ERR_ENTRY_RETURN,
    IMG_CHECK_ERR_VERIFY,
    IMG_CHECK_ERR_EXE_BASE,
    IMG_CHECK_ERR_IC_TYPE,
    IMG_CHECK_ERR_MAX,
} IMG_CHECK_ERR_TYPE;


/** End of FLASH_DEVICE_Exported_Types
  * @}
  */

/*============================================================================*
  *                                   Functions
 *============================================================================*/
/** @defgroup FLASH_DEVICE_Exported_Functions Flash Device Exported Functions
    * @brief
    * @{
    */

/**
 * @brief  Get image address in OTA header
 *
 * @param[in]  ota_addr: valid OTA header address
 * @param[in]  image_id: image ID
 *
 * @return Image address in OTA header
*/
extern uint32_t *get_image_addr_in_bank(uint32_t ota_addr, T_IMG_ID image_id);

/**
 * @brief  Get image size in OTA header
 *
 * @param[in]  ota_addr: valid OTA header address
 * @param[in]  image_id: image ID
 *
 * @return Image size in OTA header
*/
extern uint32_t *get_image_size_in_bank(uint32_t ota_addr, T_IMG_ID image_id);

/**
 * @brief  Get start address of active OTA bank
 *
 * @return Start address of active OTA bank
*/
extern uint32_t get_active_ota_bank_addr(void);

/**
 * @brief  Check configured flash layout support bank switch
 *
 * @return Check result
 * @retval true Support bank switch
 * @retval false Otherwise
*/
extern bool is_ota_support_bank_switch(void);

/**
 * @brief  Calculate checksum (CRC16 or SHA256 determined by image) over the image, and compare
 *         with given checksum value.
 *
 * @param  p_header Image header info of the given image.
 *
 * @return Check result
 * @retval true If image integrity check passes via checksum compare
 * @retval false Otherwise
*/
extern bool check_image_chksum(T_IMG_HEADER_FORMAT *p_header);

/**
 * @brief  Check the validity of the specified image
 *
 * @param[in]  image_id Specify the image
 * @param[in]  header_addr Specify image header address
 *
 * @return Check result
 * @retval IMG_CHECK_PASS If image check passes
 * @retval fail Otherwise
*/
extern IMG_CHECK_ERR_TYPE check_header_valid(uint32_t header_addr, T_IMG_ID image_id);

/**
 * @brief  Get specified image header address in active bank
 *
 * @param[in]  image_id Specify the image
 *
 * @return Specified image header address
*/
extern uint32_t get_header_addr_by_img_id(T_IMG_ID image_id);

/**
 * @brief  Get image size of specified image which is located in OTA active bank
 *
 * @param[in]  image_id Specify the image which is located in OTA active bank
 * @return Image size of specified image which is located in OTA active bank
*/
extern uint32_t get_active_bank_image_size_by_img_id(T_IMG_ID image_id);
/**
 * @brief  Get start address of specified image which is located in OTA temp bank
 *
 * @param[in]  image_id Specify the image
 * @return Start address of specified image which is located in OTA temp bank
*/
static uint32_t get_temp_ota_bank_addr_by_img_id(T_IMG_ID image_id);

/**
 * @brief  Get the size of the specified image located in the OTA temp bank.
 *
 * @param[in]  image_id Specify the image.
 * @return The size of the specified image located in the OTA temp bank.
*/
extern uint32_t get_temp_ota_bank_size_by_img_id(T_IMG_ID image_id);

/**
 * @brief  Get version info of the specified image located in the active bank.
 *
 * @param[in]  image_id Specify the image.
 * @return Version info of the specified image located in the active bank.
*/
extern bool get_active_bank_image_version(T_IMG_ID image_id, T_IMAGE_VERSION *p_image_version);

extern IMG_CHECK_ERR_TYPE image_entry_check(T_ROM_HEADER_FORMAT *rom_header,
                                            T_ROM_HEADER_FORMAT *patch_header);

static inline uint32_t get_temp_ota_bank_addr_by_img_id(T_IMG_ID image_id)
{
    uint32_t image_addr = 0;
    if (image_id < OTA || ((image_id >= IMAGE_MAX)))
    {
        return image_addr;
    }
    if (!is_ota_support_bank_switch())
    {
        if (image_id == OTA)
        {
            return 0;
        }
        image_addr = flash_nor_get_bank_addr(NOR_FLASH_OTA_TMP);
    }
    else
    {
        uint32_t ota_bank0_addr = flash_nor_get_bank_addr(NOR_FLASH_OTA_BANK_0);
        uint32_t temp_bank_addr;
        if (ota_bank0_addr == get_active_ota_bank_addr())
        {
            temp_bank_addr = flash_nor_get_bank_addr(NOR_FLASH_OTA_BANK_1);
        }
        else
        {
            temp_bank_addr = ota_bank0_addr;
        }
        if (image_id == OTA)
        {
            image_addr = temp_bank_addr;
        }
        else if (image_id >= SecureBoot && image_id < IMAGE_MAX)
        {
            if (IMG_CHECK_PASS != check_header_valid(temp_bank_addr, OTA))
            {
                return 0;
            }
            image_addr = *get_image_addr_in_bank(temp_bank_addr, image_id);
        }
    }
    if (image_addr == 0xffffffff)
    {
        return 0;
    }
    return image_addr;
}

/** @} */ /* End of group FLASH_DEVICE_Exported_Functions */
/** @} */ /* End of group FLASH_DEVICE */

#endif // _PATCH_HEADER_CHECK_H_
