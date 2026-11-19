#ifndef _TAG_TYPES_H_
#define _TAG_TYPES_H_

#define DATATYPE_NOUPDATE           0
#define DATATYPE_IMG_BMP            2
#define DATATYPE_FW_UPDATE          3
#define DATATYPE_IMG_DIFF           0x10
#define DATATYPE_IMG_RAW_1BPP       0x20
#define DATATYPE_IMG_RAW_2BPP       0x21
#define DATATYPE_IMG_RAW_3BPP       0x22
#define DATATYPE_IMG_RAW_1BPP_DIRECT 0x3F
#define DATATYPE_IMG_ZLIB           0x30
#define DATATYPE_NFC_RAW_CONTENT    0xA0
#define DATATYPE_NFC_URL_DIRECT     0xA1
#define DATATYPE_TAG_CONFIG_DATA    0xA8
#define DATATYPE_COMMAND_DATA       0xAF
#define DATATYPE_CUSTOM_LUT_OTA     0xB0

#define CMD_DO_REBOOT               0
#define CMD_DO_SCAN                 1
#define CMD_DO_RESET_SETTINGS       2
#define CMD_DO_BLE_ON               150
#define CMD_DO_BLE_OFF              151

#define CAPABILITY_HAS_LED          0x01
#define CAPABILITY_SUPPORTS_COMPRESSION 0x02
#define CAPABILITY_SUPPORTS_CUSTOM_LUTS  0x04
#define CAPABILITY_HAS_EXT_POWER    0x10
#define CAPABILITY_HAS_WAKE_BUTTON  0x20
#define CAPABILITY_HAS_NFC          0x40
#define CAPABILITY_NFC_WAKE         0x80
#define CAPABILITY_IS_BLE           0x0100

#endif /* _TAG_TYPES_H_ */
