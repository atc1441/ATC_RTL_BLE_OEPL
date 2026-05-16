#include <string.h>
#include <stdio.h>
#include "ble_cmd_handler.h"
#include "peripheral_app.h"
#include <app_task.h>
#include "gap_conn_le.h"
#include "eeprom.h"
#include "drawing.h"
#include "epd.h"
#include "platform_utils.h"
#include "ota.h"
#include "mac_edit.h"

/* ---- Configuration ------------------------------------------------------ */
#define MAX_BLE_PACKET_SIZE     512
/* imgSlots: floor(EEPROM_IMG_LEN / default_img_each) = 524288 / 94208 = 5 */
#define DEFAULT_IMG_EACH        0x17000UL

/* ---- Module globals ----------------------------------------------------- */
static uint8_t  bleNotifyBuff[MAX_BLE_PACKET_SIZE];

static uint8_t  blockXferBuffer[BLOCK_XFER_BUFFER_SIZE];
static uint16_t dataRequestSizeBLE  = 0;
static uint8_t  partsThisBlockBLE   = 0;

struct blockRequest curBlock;
struct AvailDataInfo curDataInfo;
uint8_t  curImgSlot    = 0xFF;
uint32_t curHighSlotId = 0;
uint8_t  nextImgSlot   = 0;
uint8_t  imgSlots      = 0;   /* computed from EEPROM_IMG_LEN / img_each() */
uint8_t  drawWithLut   = 0;
uint32_t imageSize     = 0;

uint32_t doRebootOrFirmwareUpdate = 0;

/* ---- Helpers ------------------------------------------------------------ */

static inline uint32_t img_each(void) {
    return settings.saved_EEPROM_IMG_EACH ? settings.saved_EEPROM_IMG_EACH : DEFAULT_IMG_EACH;
}

uint32_t getAddressForSlot(const uint8_t s) {
    return EEPROM_IMG_START + img_each() * s;
}

void drawImageFromEeprom(const uint8_t imgSlot)
{
    /* Post a draw request to the app task so the BLE ACK (already sent by
     * the caller) can be transmitted before the EPD work starts. */
    T_IO_MSG draw_msg;
    draw_msg.type    = IO_MSG_TYPE_DRAW;
    draw_msg.subtype = drawWithLut;
    draw_msg.u.param = getAddressForSlot(imgSlot);
    if (!app_send_msg_to_apptask(&draw_msg))
        printf("ERR: draw msg failed slot %u\n", imgSlot);
    drawWithLut = 0;
}

/* Save the data portion of blockXferBuffer (bytes after the blockData header)
 * to the correct EEPROM offset for this block. */
static void saveImgBlockData(const uint8_t imgSlot, const uint8_t blockId) {
    uint32_t slot_base = getAddressForSlot(imgSlot);
    uint32_t offset    = sizeof(struct EepromImageHeader) + (uint32_t)blockId * BLOCK_DATA_SIZE;
    uint32_t avail     = img_each() - offset;
    uint32_t length    = (avail > BLOCK_DATA_SIZE) ? BLOCK_DATA_SIZE : avail;
    printf("SAVE blk %d slot %d addr=0x%lX len=%lu\n",
           blockId, imgSlot, slot_base + offset, length);
    eepromPowerUp();
    if (!eepromWrite(slot_base + offset,
                     blockXferBuffer + sizeof(struct blockData),
                     length))
        printf("ERR: EEPROM write failed\n");
    eepromPowerDown();
}

/* Scan all slots for a stored image with the given 8-byte version hash.
 * Returns the slot index, or 0xFF if not found. */
static uint8_t findSlot(const uint8_t *ver) {
    uint32_t markerValid = EEPROM_IMG_VALID + img_each();
    eepromPowerUp();
    for (uint8_t c = 0; c < imgSlots; c++) {
        struct EepromImageHeader *eih = (struct EepromImageHeader *)blockXferBuffer;
        eepromRead(getAddressForSlot(c), (uint8_t *)eih, sizeof(struct EepromImageHeader));
        if (!memcmp(&eih->validMarker, &markerValid, 4) &&
            !memcmp(&eih->version,     ver, 8)) {
            eepromPowerDown();
            return c;
        }
    }
    eepromPowerDown();
    return 0xFF;
}

/* CRC: byte[0] must equal the sum of bytes[1..len-1].*/
static uint8_t checkCRC(const void *p, const uint8_t len) {
    uint8_t total = 0;
    for (uint8_t c = 1; c < len; c++)
        total += ((const uint8_t *)p)[c];
    return ((const uint8_t *)p)[0] == total;
}

/* Validate the assembled block in blockXferBuffer.
 * Header = struct blockData (4 bytes): size + checksum over data[]. */
static uint8_t validateBlockData(void) {
    const struct blockData *bd = (const struct blockData *)blockXferBuffer;
    printf("BLK validate: size=%u chk=0x%04X\n", bd->size, bd->checksum);
    if (bd->size > BLOCK_XFER_BUFFER_SIZE - sizeof(struct blockData)) {
        printf("ERR: impossible block size %u\n", bd->size);
        return 0;
    }
    uint16_t t = 0;
    for (uint16_t c = 0; c < bd->size; c++)
        t += bd->data[c];
    printf("BLK calc chk=0x%04X %s\n", t, (t == bd->checksum) ? "OK" : "FAIL");
    return bd->checksum == t;
}

/* ---- Notification send -------------------------------------------------- */

void ble_send_notify(uint16_t cmd, uint8_t *data, uint8_t len) {
    if (len > sizeof(bleNotifyBuff) - 2) {
        printf("WARN: notify payload too long (%u), truncating\n", len);
        len = (uint8_t)(sizeof(bleNotifyBuff) - 2);
    }
    bleNotifyBuff[0] = (uint8_t)(cmd >> 8);
    bleNotifyBuff[1] = (uint8_t)(cmd & 0xff);
    if (data && len > 0)
        memcpy(&bleNotifyBuff[2], data, len);
    printf("NOTIFY cmd=0x%04X len=%u\n", cmd, (unsigned)len);
    app_send_custom_notification(0, bleNotifyBuff, (uint16_t)len + 2);
}

/* ---- Block-part receive ------------------------------------------------- */

static uint8_t processBlockPartBLE(const struct blockPart *bp) {
    uint16_t start = (uint16_t)bp->blockPart * BLOCK_PART_DATA_SIZE_BLE;
    uint16_t size  = BLOCK_PART_DATA_SIZE_BLE;

    if (bp->blockId != curBlock.blockId) {
        printf("PART wrong blkId rx=%u exp=%u\n", bp->blockId, curBlock.blockId);
        return 0;
    }
    if (start >= BLOCK_XFER_BUFFER_SIZE - 1) {
        printf("PART start out of range: %u\n", start);
        return 0;
    }
    if (bp->blockPart >= BLOCK_MAX_PARTS) {
        printf("PART index too high: %u\n", bp->blockPart);
        return 0;
    }
    if (start + size > BLOCK_XFER_BUFFER_SIZE)
        size = (uint16_t)(BLOCK_XFER_BUFFER_SIZE - start);

    /* CRC covers: checksum(1) + blockId(1) + blockPart(1) + data(230) = 233 bytes.
     * Always use the full 230-byte part size for CRC regardless of size truncation
     * because the sender always sends 230 bytes. */
    if (!checkCRC(bp, (uint8_t)(3 + BLOCK_PART_DATA_SIZE_BLE))) {
        printf("PART CRC fail (blk=%u part=%u)\n", bp->blockId, bp->blockPart);
        return 0;
    }

    memcpy(blockXferBuffer + start, bp->data, size);
    curBlock.requestedParts[bp->blockPart / 8] &= (uint8_t)~(1u << (bp->blockPart % 8));
    return 1;
}

/* ---- Image-data availability -------------------------------------------- */

static void recalcParts(void) {
    dataRequestSizeBLE = (curDataInfo.dataSize > BLOCK_DATA_SIZE)
                         ? (uint16_t)BLOCK_DATA_SIZE
                         : (uint16_t)curDataInfo.dataSize;
    partsThisBlockBLE = (uint8_t)(
        (sizeof(struct blockData) + dataRequestSizeBLE + BLOCK_PART_DATA_SIZE_BLE - 1)
        / BLOCK_PART_DATA_SIZE_BLE);
}

bool processAvailDataInfoBLE(struct AvailDataInfo *avail) {
    /* Recompute slot count from actual EEPROM size and current img_each setting. */
    uint32_t each = img_each();
    imgSlots = (uint8_t)(OTA_EXT_START / each); /* cap: don't overlap OTA area */
    if (imgSlots > OTA_IMG_SLOTS_MAX) imgSlots = OTA_IMG_SLOTS_MAX;
    if (imgSlots == 0) imgSlots = 1;

    printf("AVAIL dataType=0x%02X size=%lu  slots=%u each=0x%lX\n",
           avail->dataType, (unsigned long)avail->dataSize,
           imgSlots, (unsigned long)each);

    switch (avail->dataType) {
    case DATATYPE_IMG_BMP:
    case DATATYPE_IMG_DIFF:
    case DATATYPE_IMG_ZLIB:
        break;

    case DATATYPE_IMG_RAW_1BPP:
    case DATATYPE_IMG_RAW_2BPP:
        break;

    case DATATYPE_FW_UPDATE:
        if (avail->dataSize == 0 || avail->dataSize > OTA_EXT_SIZE) {
            printf("OTA: size %lu rejected (max %lu)\n",
                   (unsigned long)avail->dataSize, (unsigned long)OTA_EXT_SIZE);
            ble_send_notify(BLE_CMD_ERR, NULL, 0);
            return true;
        }
        /* Resume an in-progress transfer if version matches. */
        if (curDataInfo.dataSize != 0 &&
            memcmp(&avail->dataVer, &curDataInfo.dataVer, 8) == 0) {
            printf("OTA: resume at blk %u remaining=%lu\n",
                   curBlock.blockId, (unsigned long)curDataInfo.dataSize);
        } else {
            /* Fresh download — erase the staging area first. */
            ota_erase_ext();
            curBlock.blockId = 0;
            memcpy(&curBlock.ver, &avail->dataVer, 8);
            curBlock.type = avail->dataType;
            memcpy(&curDataInfo, avail, sizeof(struct AvailDataInfo));
            imageSize = avail->dataSize;
        }
        memset(curBlock.requestedParts, 0xFF, BLOCK_REQ_PARTS_BYTES);
        recalcParts();
        printf("OTA: req blk %u  parts=%u size=%lu\n",
               curBlock.blockId, partsThisBlockBLE,
               (unsigned long)curDataInfo.dataSize);
        ble_send_notify(BLE_CMD_REQ, (uint8_t *)&curBlock, sizeof(struct blockRequest));
        return true;

    case DATATYPE_COMMAND_DATA:
        printf("AVAIL: CMD data arg=0x%02X\n", avail->dataTypeArgument);
        return true;

    default:
        printf("AVAIL: unknown dataType 0x%02X\n", avail->dataType);
        ble_send_notify(BLE_CMD_ERR, NULL, 0);
        return true;
    }

    if (avail->dataSize + sizeof(struct EepromImageHeader) >= img_each()) {
        printf("AVAIL: image too big! %lu > %lu\n",
               (unsigned long)(avail->dataSize + sizeof(struct EepromImageHeader)),
               (unsigned long)img_each());
        ble_send_notify(BLE_CMD_ERR, NULL, 0);
        return true;
    }

    /* If this version is the one currently displayed (transfer finished,
     * dataSize == 0), tell the host it's already shown. */
    if (curDataInfo.dataSize == 0 &&
        memcmp(&avail->dataVer, &curDataInfo.dataVer, 8) == 0) {
        printf("AVAIL: already shown\n");
        ble_send_notify(BLE_CMD_ACK_IS_SHOWN, NULL, 0);
        return true;
    }

    /* Check if we already have this image cached in flash. */
    curImgSlot = findSlot((const uint8_t *)&avail->dataVer);
    if (curImgSlot != 0xFF) {
        printf("AVAIL: cached in slot %u, drawing\n", curImgSlot);
        ble_send_notify(BLE_CMD_ACK, NULL, 0);
        memcpy(&curDataInfo, avail, sizeof(struct AvailDataInfo));
        curDataInfo.dataSize = 0;   /* mark transfer done */
        drawWithLut = avail->dataTypeArgument;
        drawImageFromEeprom(curImgSlot);
        return true;
    }

    /* New image — pick next round-robin slot and erase it. */
    nextImgSlot++;
    if (nextImgSlot >= imgSlots) nextImgSlot = 0;
    curImgSlot = nextImgSlot;

    printf("AVAIL: new image → slot %u addr=0x%lX size=0x%lX\n",
           curImgSlot,
           (unsigned long)getAddressForSlot(curImgSlot),
           (unsigned long)img_each());

    drawWithLut = avail->dataTypeArgument;

    /* Erase with retry (blocks BLE for ~1 s — connection may drop). */
    uint8_t attempt = 3;
    while (attempt--) {
        eepromPowerUp();
        uint8_t erased = eepromErase(getAddressForSlot(curImgSlot), img_each());
        eepromPowerDown();
        if (erased) goto erase_ok;
    }
    printf("ERR: EEPROM erase failed for slot %u\n", curImgSlot);
    ble_send_notify(BLE_CMD_ERR, NULL, 0);
    return true;

erase_ok:
    printf("AVAIL: erase done, starting block 0\n");
    curBlock.blockId = 0;
    memcpy(&curBlock.ver, &avail->dataVer, 8);
    curBlock.type = avail->dataType;
    memcpy(&curDataInfo, avail, sizeof(struct AvailDataInfo));
    imageSize = curDataInfo.dataSize;
    memset(curBlock.requestedParts, 0xFF, BLOCK_REQ_PARTS_BYTES);

    recalcParts();
    printf("AVAIL: req blk 0  parts=%u reqSize=%u\n",
           partsThisBlockBLE, dataRequestSizeBLE);
    ble_send_notify(BLE_CMD_REQ, (uint8_t *)&curBlock, sizeof(struct blockRequest));
    return true;
}

/* ---- Block data receive ------------------------------------------------- */

static void handleBlkBLE(uint8_t *payload) {
    const struct blockPart *bp = (const struct blockPart *)payload;

    if (!processBlockPartBLE(bp)) {
        ble_send_notify(BLE_CMD_ERR_BLKPRT, NULL, 0);
        /* Dump the first bytes for debugging. */
        printf("PART_ERR blk=%u part=%u  bytes:", bp->blockId, bp->blockPart);
        for (int i = 0; i < 8; i++) printf(" %02X", payload[i]);
        printf("\n");
        return;
    }

    /* Print reception progress: '.' = still needed, 'R' = received. */
    printf("RX  blk=%u [", curBlock.blockId);
    for (uint8_t c = 0; c < partsThisBlockBLE; c++) {
        if (c && (c % 8 == 0)) printf("][");
        printf("%c", (curBlock.requestedParts[c / 8] & (1u << (c % 8))) ? '.' : 'R');
    }
    printf("]\n");

    /* Check if all parts have been received. */
    uint8_t blockComplete = 1;
    for (uint8_t c = 0; c < partsThisBlockBLE; c++) {
        if (curBlock.requestedParts[c / 8] & (1u << (c % 8))) {
            blockComplete = 0;
            break;
        }
    }

    if (!blockComplete) {
        ble_send_notify(BLE_CMD_ACK_BLKPRT, NULL, 0);
        return;
    }

    printf("BLK %u complete\n", curBlock.blockId);

    if (!validateBlockData()) {
        printf("BLK %u validation FAILED, re-requesting\n", curBlock.blockId);
        memset(curBlock.requestedParts, 0xFF, BLOCK_REQ_PARTS_BYTES);
        recalcParts();
        ble_send_notify(BLE_CMD_REQ, (uint8_t *)&curBlock, sizeof(struct blockRequest));
        return;
    }

    /* Save the validated block payload to the appropriate destination. */
    const struct blockData *bd = (const struct blockData *)blockXferBuffer;

    if (curBlock.type == DATATYPE_FW_UPDATE) {
        printf("OTA blk %u validated, saving to ext flash\n", curBlock.blockId);
        ota_save_ext_block(curBlock.blockId,
                           blockXferBuffer + sizeof(struct blockData),
                           bd->size);
    } else {
        printf("BLK %u validated, saving to img slot %u\n",
               curBlock.blockId, curImgSlot);
        saveImgBlockData(curImgSlot, curBlock.blockId);
    }

    curBlock.blockId++;
    curDataInfo.dataSize -= dataRequestSizeBLE;

    if (curDataInfo.dataSize == 0) {
        /* ---- Transfer complete ---- */
        if (curBlock.type == DATATYPE_FW_UPDATE) {
            printf("OTA: download done (%lu bytes), posting apply\n",
                   (unsigned long)imageSize);
            ble_send_notify(BLE_CMD_ACK_FW_UPDATED, NULL, 0);
            T_IO_MSG ota_msg;
            ota_msg.type    = IO_MSG_TYPE_OTA_APPLY;
            ota_msg.subtype = 0;
            ota_msg.u.param = imageSize;
            app_send_msg_to_apptask(&ota_msg);
        } else {
            /* Write image header to finalise the slot. */
            struct EepromImageHeader *eih = (struct EepromImageHeader *)blockXferBuffer;
            memcpy(&eih->version, &curDataInfo.dataVer, 8);
            eih->validMarker = EEPROM_IMG_VALID + img_each();
            eih->id          = ++curHighSlotId;
            eih->size        = imageSize;
            eih->dataType    = curDataInfo.dataType;
            printf("HDR write slot=%u type=0x%02X size=%lu\n",
                   curImgSlot, curDataInfo.dataType, (unsigned long)imageSize);
            eepromPowerUp();
            eepromWrite(getAddressForSlot(curImgSlot),
                        (uint8_t *)eih, sizeof(struct EepromImageHeader));
            eepromPowerDown();
            curDataInfo.dataSize = 0;
            printf("XFER complete! drawing from slot %u\n", curImgSlot);
            ble_send_notify(BLE_CMD_ACK, NULL, 0);
            drawImageFromEeprom(curImgSlot);
        }
    } else {
        /* More blocks to go. */
        memset(curBlock.requestedParts, 0xFF, BLOCK_REQ_PARTS_BYTES);
        recalcParts();
        printf("REQ blk %u  parts=%u remaining=%lu\n",
               curBlock.blockId, partsThisBlockBLE,
               (unsigned long)curDataInfo.dataSize);
        ble_send_notify(BLE_CMD_REQ, (uint8_t *)&curBlock, sizeof(struct blockRequest));
    }
}

/* ---- Main command dispatcher -------------------------------------------- */

int zb_ble_hci_cmd_handler(uint16_t cmdId, uint16_t len, uint8_t *payload) {
    /* Caller has already stripped the 2-byte cmdId prefix from payload. */
    printf("CMD 0x%04X len=%u\n", cmdId, len);

    /* Log first bytes of every payload for easy debugging. */
    if (len > 0) {
        uint8_t dump = (len < 16) ? (uint8_t)len : 16;
        printf("  payload[0..%u]:", dump - 1);
        for (uint8_t i = 0; i < dump; i++) printf(" %02X", payload[i]);
        printf("%s\n", (len > 16) ? " ..." : "");
    }

    switch (cmdId) {
    default:
        printf("CMD unknown 0x%04X\n", cmdId);
        ble_send_notify(BLE_CMD_ERR, NULL, 0);
        break;

    case BLE_CMD_FAST:
        printf("CMD FAST\n");
        /* TODO: le_update_conn_param(0, 8, 8, 0, 500) */
        ble_send_notify(BLE_CMD_ACK_CMD, NULL, 0);
        break;

    case BLE_CMD_SLOW:
        printf("CMD SLOW\n");
        /* TODO: le_update_conn_param(0, 80, 80, 0, 500) */
        ble_send_notify(BLE_CMD_ACK_CMD, NULL, 0);
        break;

    case BLE_CMD_LED:
        printf("CMD LED arg=0x%02X\n", len > 0 ? payload[0] : 0);
        ble_send_notify(BLE_CMD_ACK_CMD, NULL, 0);
        break;

    case BLE_CMD_GET_INFOS: {
        printf("CMD GET_INFOS\n");
        uint8_t send_len = (uint8_t)(sizeof(settings) > 247 ? 247 : sizeof(settings));
        ble_send_notify(BLE_CMD_GET_INFOS, (uint8_t *)&settings, send_len);
        break;
    }

    case BLE_CMD_SET_CUSTOM_MAC:
        if (len >= 6) {
            printf("CMD EDIT_OEM_MAC to %02X:%02X:%02X:%02X:%02X:%02X\n",
                   payload[5], payload[4], payload[3], payload[2], payload[1], payload[0]);
            ble_send_notify(BLE_CMD_ACK_CMD, NULL, 0);
            platform_delay_ms(100);
            mac_edit_update(payload);
        } else {
            ble_send_notify(BLE_CMD_ERR, NULL, 0);
        }
        break;

    case BLE_CMD_RESET_TO_DEFAULT:
        printf("CMD RESET_TO_DEFAULT\n");
        /* TODO: reset settings to factory defaults */
        ble_send_notify(BLE_CMD_ACK_CMD, NULL, 0);
        break;

    case BLE_CMD_OEPL_ENABLE:
        printf("CMD OEPL_ENABLE\n");
        ble_send_notify(BLE_CMD_ACK_CMD, NULL, 0);
        break;

    case BLE_CMD_OEPL_DISABLE:
        printf("CMD OEPL_DISABLE\n");
        ble_send_notify(BLE_CMD_ACK_CMD, NULL, 0);
        break;

    case BLE_CMD_AVAILDATA:
        if (len < sizeof(struct AvailDataInfo)) {
            printf("CMD AVAILDATA too short (%u < %u)\n",
                   len, (unsigned)sizeof(struct AvailDataInfo));
            ble_send_notify(BLE_CMD_ERR, NULL, 0);
            break;
        }
        processAvailDataInfoBLE((struct AvailDataInfo *)payload);
        break;

    case BLE_CMD_BLK_DATA:
        if (len < sizeof(struct blockPart)) {
            printf("CMD BLK_DATA too short (%u < %u)\n",
                   len, (unsigned)sizeof(struct blockPart));
            ble_send_notify(BLE_CMD_ERR_BLKPRT, NULL, 0);
            break;
        }
        handleBlkBLE(payload);
        break;

    case BLE_CMD_REBOOT:
        printf("CMD REBOOT\n");
        ble_send_notify(BLE_CMD_ACK_CMD, NULL, 0);
        platform_delay_ms(50);
        doRebootOrFirmwareUpdate = 0x84722338UL;
        break;

    case BLE_CMD_SET_DYNAMIC_TYPE_READ:
        printf("CMD SET_DYNAMIC_TYPE_READ (stub)\n");
        uint8_t send_len = (uint8_t)(sizeof(settings) > 247 ? 247 : sizeof(settings));
        ble_send_notify(BLE_CMD_GET_INFOS, (uint8_t *)&settings, send_len);
        break;

    case BLE_CMD_SET_DYNAMIC_TYPE_TEST:
        printf("CMD SET_DYNAMIC_TYPE_TEST (stub)\n");
        ble_send_notify(BLE_CMD_ACK_CMD, NULL, 0);
        break;

    case BLE_CMD_SET_DYNAMIC_TYPE_SAVE:
        printf("CMD SET_DYNAMIC_TYPE_SAVE (stub)\n");
        ble_send_notify(BLE_CMD_ACK_CMD, NULL, 0);
        break;

    case BLE_CMD_READ_DEBUG:
        printf("CMD READ_DEBUG\n");
        ble_send_notify(BLE_CMD_ACK_DEBUG_INFO_STATUS, NULL, 0);
        break;

    case BLE_CMD_DEEPSLEEP:
        printf("CMD DEEPSLEEP\n");
        ble_send_notify(BLE_CMD_ACK_CMD, NULL, 0);
        break;
    }

    return 0;
}
