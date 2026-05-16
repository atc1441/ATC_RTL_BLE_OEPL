/**
************************************************************************************************************
*               Copyright(c) 2015, Realtek Semiconductor Corporation. All rights reserved.
************************************************************************************************************
* @file     dfu_api.h
* @brief    APIs to implement device firmware update.
* @details  OTA is used to update device firmware via Bluetooth.
* @author   ranhui
* @date     2015-10-29
* @version  v1.0
*************************************************************************************************************
*/

/*============================================================================*
 *               Define to prevent recursive inclusion
 *============================================================================*/
#ifndef _DFU_API_H_
#define  _DFU_API_H_

#ifdef  __cplusplus
extern  "C" {
#endif  // __cplusplus

/*============================================================================*
 *                        Header Files
 *============================================================================*/
#include "patch_header_check.h"
#include "rtl876x_hw_sha256.h"

/** @defgroup DFU_API DFU
  * @brief
  * @{
  */

/*============================================================================*
 *                         Macros
 *============================================================================*/
/** @defgroup DFU_API_Exported_Macros DFU Exported Macros
  * @brief
  * @{
  */

#define MAJOR_IMG_VER(p)        (p & 0xf)
#define MINOR_IMG_VER(p)        ((p & 0xff0) >> 4)
#define REVISION_IMG_VER(p)     ((p & 0x07fff000) >> 12)
#define RESERVE_IMG_VER(p)      ((p & 0xf8000000) >> 27)

/* Convert T_IMAGE_VERSION to little endian uint32_t value, so that compare version value directly. */
#define IMAGE_VERSION_TO_LE_UINT32(ver)                             (((ver & 0xf) << 28 )\
                                                                     + (((ver & 0xff0) >> 4) << 20)\
                                                                     + (((ver & 0x07fff000) >> 12) << 5)\
                                                                     + ((ver & 0xf8000000) >> 27))


/* dfu service uuid */
#define GATT_UUID128_DFU_SERVICE        0x12, 0xA2, 0x4D, 0x2E, 0xFE, 0x14, 0x48, 0x8e, 0x93, 0xD2, 0x17, 0x3C, 0x87, 0x62, 0x00, 0x00
#define GATT_UUID128_DFU_DATA           0x12, 0xA2, 0x4D, 0x2E, 0xFE, 0x14, 0x48, 0x8e, 0x93, 0xD2, 0x17, 0x3C, 0x87, 0x63, 0x00, 0x00
#define GATT_UUID128_DFU_CONTROL_POINT  0x12, 0xA2, 0x4D, 0x2E, 0xFE, 0x14, 0x48, 0x8e, 0x93, 0xD2, 0x17, 0x3C, 0x87, 0x64, 0x00, 0x00

/*length of each control point procedure*/
#define DFU_LENGTH_START_DFU                      (1+12+4) //!< 4 bytes is padding for encrypt.
#define DFU_LENGTH_RECEIVE_FW_IMAGE_INFO          (1+2+4) //!< img_id + cur_offset
#define DFU_LENGTH_VALID_FW                       (1+2)   //!< img_id
#define DFU_LENGTH_ACTIVE_IMAGE_RESET             (1)
#define DFU_LENGTH_ACTIVE_IMAGE_RESET_TO_OTA_MODE (1+1) //!< uint8_t reset_mode, 1: reset to ota mode.
#define DFU_LENGTH_SYSTEM_RESET                   (1)
#define DFU_LENGTH_REPORT_TARGET_INFO             (1+2) //!< img_id
#define DFU_LENGTH_CONN_PARA_TO_UPDATE_REQ        (1+2+2+2+2) //!< conn_interval_min, conn_interval_max, conn_latency, superv_tout
#define DFU_LENGTH_BUFFER_CHECK_EN                (1)
#define DFU_LENGTH_REPORT_BUFFER_CRC              (1+2+2) //!< buf_size, buf_crc
#define DFU_LENGTH_IMAGE_VERSION                  (1+2+4) //!< img_id, dfu_img_version
#define DFU_LENGTH_HANDSHAKE                      (1+16) //!< handshake
#define DFU_LENGTH_MAX                            DFU_LENGTH_START_DFU

/*length of notification*/
#define DFU_NOTIFY_LENGTH_ARV                   3  //!< others opcode notification length
#define DFU_NOTIFY_LENGTH_START_DFU             (DFU_NOTIFY_LENGTH_ARV)
#define DFU_NOTIFY_LENGTH_VALID_FW              (DFU_NOTIFY_LENGTH_ARV)
#define DFU_NOTIFY_LENGTH_CONN_PARA_UPDATE_REQ  (DFU_NOTIFY_LENGTH_ARV)
#define DFU_NOTIFY_LENGTH_REPORT_TARGET_INFO    (DFU_NOTIFY_LENGTH_ARV+4+4)  //!< img_ver, cur_offset
#define DFU_NOTIFY_LENGTH_BUFFER_CHECK_EN       (DFU_NOTIFY_LENGTH_ARV+2+2)  //!< buf_size, mtu_size
#define DFU_NOTIFY_LENGTH_REPORT_BUFFER_CRC     (DFU_NOTIFY_LENGTH_ARV+4)   //!< cur_offset
#define DFU_NOTIFY_LENGTH_RECEIVE_IC_TYPE       (DFU_NOTIFY_LENGTH_ARV+1)   //!< ic_type
#define DFU_NOTIFY_LENGTH_HANDSHAKE             (DFU_NOTIFY_LENGTH_ARV+16)   //!< handshake
/*max length*/
#define DFU_NOTIFY_LENGTH_MAX                   DFU_NOTIFY_LENGTH_HANDSHAKE

/** End of DFU_API_Exported_Macros
  * @}
  */

/*============================================================================*
 *                         Types
 *============================================================================*/
/** @defgroup DFU_API_Exported_Types DFU Exported Types
  * @{
  */

typedef union
{
    uint8_t value;
    struct
    {
        uint8_t buf_check_en: 1; //!< 1:support,  0:don't support.
        uint8_t aesflg: 1;      //!< 1:aes encrypt when ota,  0:not encrypt.
        uint8_t aesmode: 1;     //!< 1:all data is encrypted, 0:only encrypt 16byte.
        uint8_t copy_img: 1;    //!< 1:support, 0:don't support.
uint8_t multi_img:
        1;   //!< 1:support(update multi img at a time), 0:don't support(one img at a time).
        uint8_t handshake: 1;     //!< 1:support,  0:don't support.
uint8_t compress_mode:
        1; //!< 1:support ota image compressed,  0:don't support ota image compressed.
        uint8_t rsvd: 1;
    } mode_flag;
} T_OTA_MODE;


/*each control point procedure, can't modify existing value*/
typedef enum
{
    DFU_OPCODE_MIN                          = 0x00, /*!< control point opcode min*/
    DFU_OPCODE_START_DFU                    = 0x01,
    DFU_OPCODE_RECEIVE_FW_IMAGE_INFO        = 0x02,
    DFU_OPCODE_VALID_FW                     = 0x03,
    DFU_OPCODE_ACTIVE_IMAGE_RESET           = 0x04,
    DFU_OPCODE_SYSTEM_RESET                 = 0x05,
    DFU_OPCODE_REPORT_TARGET_INFO           = 0x06,
    DFU_OPCODE_CONN_PARA_TO_UPDATE_REQ      = 0x07,
    DFU_OPCODE_PKT_RX_NOTIFICATION_VOICE    = 0x08,
    DFU_OPCODE_BUFFER_CHECK_EN              = 0x09,
    DFU_OPCODE_REPORT_BUFFER_CRC            = 0x0a,
    DFU_OPCODE_RECEIVE_IC_TYPE              = 0x0b,
    DFU_OPCODE_COPY_IMG                     = 0x0c,
    DFU_OPCODE_CHECK_IMAGE_VER              = 0x0d,
    DFU_OPCODE_HANDSHAKE                    = 0x0e,
    DFU_OPCODE_MAX                          = 0x0f, /*!< control point opcode max*/
    DFU_OPCODE_NOTIFICATION                 = 0x10, /*!< notification opcode, fixed*/
} T_DFU_CP_OPCODE;

/*dfu error code*/
typedef enum
{
    DFU_ARV_SUCCESS                        = 0x01,
    DFU_ARV_FAIL_INVALID_PARAMETER         = 0x02,
    DFU_ARV_FAIL_OPERATION                 = 0x03,
    DFU_ARV_FAIL_DATA_SIZE_EXCEEDS_LIMIT   = 0x04,
    DFU_ARV_FAIL_CRC_ERROR                 = 0x05,
    DFU_ARV_FAIL_LENGTH_ERROR              = 0x06,
    DFU_ARV_FAIL_PROG_ERROR                = 0x07,
    DFU_ARV_FAIL_ERASE_ERROR               = 0x08,
    DFU_ARV_FAIL_SYS_VERSION_ERROR         = 0x09,
} T_DFU_ARV_ERROR_CODE;

/** End of DFU_API_Exported_Types
  * @}
  */


/*============================================================================*
  *                                Functions
  *============================================================================*/
/** @defgroup DFU_API_Exported_Functions DFU Exported Functions
    * @{
    */
/**
 * @brief  Get image version from the image start address.
 *
 * @param[in]  start_addr    image start address
 * @return image version
 */
static inline uint32_t get_image_version_le_value_from_start_addr(uint32_t start_addr)
{
    uint32_t img_ver = 0;
    T_IMG_HEADER_FORMAT *p_header = (T_IMG_HEADER_FORMAT *)start_addr;
    if (p_header)
    {
        img_ver = (p_header->git_ver.ver_info.sub_version._version_major << 28)
                  + (p_header->git_ver.ver_info.sub_version._version_minor << 20)
                  + (p_header->git_ver.ver_info.sub_version._version_revision << 5)
                  + p_header->git_ver.ver_info.sub_version._version_reserve;
    }
    return img_ver;
}


/** End of DFU_API_Exported_Functions
  * @}
  */

/** End of DFU_API
  * @}
  */

#ifdef __cplusplus
}
#endif // __cplusplus

#endif
