#ifndef _BLE_CMD_HANDLER_H_
#define _BLE_CMD_HANDLER_H_

#include <stdint.h>
#include <stdbool.h>
#include "tag_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ---- BLE command IDs (client → device) --------------------------------- */
#define BLE_CMD_LED                 1
#define BLE_CMD_FAST                2
#define BLE_CMD_SLOW                3
#define BLE_CMD_SET_TYPE            4
#define BLE_CMD_GET_INFOS           5
#define BLE_CMD_OEPL_ENABLE         6
#define BLE_CMD_OEPL_DISABLE        7
#define BLE_CMD_OEPL_BLE_PERIOD     8
#define BLE_CMD_SET_CUSTOM_MAC      9
#define BLE_CMD_RESET_TO_DEFAULT    10
#define BLE_CMD_SET_UNIXTIME        11
#define BLE_CMD_DISABLE_UNIXTIME    12
#define BLE_CMD_READ_LUT            13
#define BLE_CMD_READ_LUT_NEXT       14
#define BLE_CMD_SET_DYNAMIC_TYPE_TEST  15
#define BLE_CMD_SET_DYNAMIC_TYPE_SAVE  16
#define BLE_CMD_SET_DYNAMIC_TYPE_READ  17
#define BLE_CMD_BLE_DISABLE         18
#define BLE_CMD_SET_SPEED           19
#define BLE_CMD_DEEPSLEEP           20
#define BLE_CMD_SET_LED             21
#define BLE_CMD_READ_DEBUG          22
#define BLE_CMD_REBOOT              23

/* ---- BLE response / notification IDs (device → client) ----------------- */
#define BLE_CMD_ACK_CMD             99
#define BLE_CMD_AVAILDATA           100
#define BLE_CMD_BLK_DATA            101
#define BLE_CMD_ERR_BLKPRT          196
#define BLE_CMD_ACK_BLKPRT          197
#define BLE_CMD_REQ                 198
#define BLE_CMD_ACK                 199
#define BLE_CMD_ACK_IS_SHOWN        200
#define BLE_CMD_ACK_FW_UPDATED      201
#define BLE_CMD_ACK_LUT_START       202
#define BLE_CMD_ACK_LUT_DATA        203
#define BLE_CMD_ACK_LUT_DONE        204
#define BLE_CMD_ACK_DYNAMIC_READ    205
#define BLE_CMD_ACK_DYNAMIC_OK      206
#define BLE_CMD_ACK_DYNAMIC_ERR     207
#define BLE_CMD_ERR_FILE_TOO_BIG    208
#define BLE_CMD_ACK_DEBUG_INFO_STATUS 209

#define BLE_CMD_ERR                 0xffff

#define BLOCK_PART_DATA_SIZE_BLE    230     /* bytes of image data per BLE part */
#define BLOCK_DATA_SIZE             4096UL  /* max image data per block */
#define BLOCK_MAX_PARTS             42      /* ceil((4+4096)/230) = 18, but proto allows 42 */
#define BLOCK_REQ_PARTS_BYTES       6       /* 48-bit bitmap of requested parts */
#define BLOCK_XFER_BUFFER_SIZE      (BLOCK_DATA_SIZE + 4)  /* 4 = sizeof(struct blockData) */

/* ---- Wire-format structs — ALL must be packed to match OEPL on-wire ---- */
#pragma pack(push, 1)

/* Sent by client to announce available image data */
struct AvailDataInfo {
    uint8_t  checksum;
    uint64_t dataVer;           /* MD5 / version hash of image */
    uint32_t dataSize;          /* total image size in bytes */
    uint8_t  dataType;          /* DATATYPE_IMG_RAW_1BPP etc. */
    uint8_t  dataTypeArgument;  /* LUT selector for EPD */
    uint16_t nextCheckIn;       /* hint: minutes until next check-in */
};

/* Sent by device to request a block of parts */
struct blockRequest {
    uint8_t  checksum;
    uint64_t ver;               /* copy of AvailDataInfo.dataVer */
    uint8_t  blockId;
    uint8_t  type;              /* copy of AvailDataInfo.dataType */
    uint8_t  requestedParts[BLOCK_REQ_PARTS_BYTES]; /* bitmap: 1 = still needed */
};

/* One 230-byte chunk of image data, sent by client */
struct blockPart {
    uint8_t  checksum;          /* sum of blockId + blockPart + data[0..229] */
    uint8_t  blockId;
    uint8_t  blockPart;         /* index within block [0..BLOCK_MAX_PARTS-1] */
    uint8_t  data[BLOCK_PART_DATA_SIZE_BLE];
};

/* Header at the start of each 4096-byte block buffer */
struct blockData {
    uint16_t size;              /* actual payload bytes in this block */
    uint16_t checksum;          /* sum of data[0..size-1] */
    uint8_t  data[];            /* variable — up to BLOCK_DATA_SIZE bytes */
};

#pragma pack(pop)

/* ---- Public API --------------------------------------------------------- */

/* Main entry point — called with cmdId stripped, payload pointing PAST the
 * 2-byte cmdId prefix, len = payload bytes NOT including the cmdId. */
int  zb_ble_hci_cmd_handler(uint16_t cmdId, uint16_t len, uint8_t *payload);

/* Send a BLE notification with a 2-byte cmdId prefix. */
void ble_send_notify(uint16_t cmd, uint8_t *data, uint8_t len);

#ifdef __cplusplus
}
#endif

#endif /* _BLE_CMD_HANDLER_H_ */
