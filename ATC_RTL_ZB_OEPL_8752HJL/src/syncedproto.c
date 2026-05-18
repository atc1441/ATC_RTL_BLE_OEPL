#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include "zigbee.h"
#include "proto.h"
#include "syncedproto.h"
#include "comms.h"
#include "powermgt.h"
#include "eeprom.h"
#include "drawing.h"
#include "ota.h"
#include "app_flags.h"
#include <os_mem.h>

/* ── Block transfer state ─────────────────────────────────────────────── */
/* blockXferBuffer (4100 bytes) is heap-allocated in initializeProto() so it
 * lands in HEAP_DATA_ON (52 KB) instead of APP_DATA_ON (16 KB). */
static uint8_t           *blockXferBuffer = NULL;
static struct blockRequest curBlock    = {0};
static struct AvailDataInfo curDataInfo = {0};
static bool   requestPartialBlock = false;
#define BLOCK_TRANSFER_ATTEMPTS 5

/* ── Image slot tracking ──────────────────────────────────────────────── */
uint8_t         curImgSlot     = 0xFF;
static uint32_t curHighSlotId  = 0;
static uint8_t  nextImgSlot    = 0;
static uint8_t  imgSlots       = 0;
static uint8_t  drawWithLut    = 0;

/* ── Network state ────────────────────────────────────────────────────── */
uint8_t  APmac[8]      = {0};
uint16_t APsrcPan      = 0;
uint8_t  mSelfMac[8]   = {0};

static uint8_t seq            = 0;
uint8_t        currentChannel = 0;

/* ── Packet buffers ───────────────────────────────────────────────────── */
static uint8_t inBuffer[128]  = {0};
static uint8_t outBuffer[128] = {0};

/* ── EEPROM layout (external SPI flash) ──────────────────────────────── */
#define PROTO_EEPROM_IMG_START  0x00000UL
#define PROTO_EEPROM_IMG_EACH   0x17000UL
#define PROTO_EEPROM_OTA_START  OTA_EXT_START
#define PROTO_EEPROM_OTA_LEN    OTA_EXT_SIZE
#define PROTO_PAGE_SIZE         4096u

/* ── Checksum helpers ─────────────────────────────────────────────────── */
static bool checkCRC(const void *p, uint8_t len)
{
    uint8_t total = 0;
    for (uint8_t c = 1; c < len; c++)
        total += ((const uint8_t *)p)[c];
    return ((const uint8_t *)p)[0] == total;
}

static void addCRC(void *p, uint8_t len)
{
    uint8_t total = 0;
    for (uint8_t c = 1; c < len; c++)
        total += ((uint8_t *)p)[c];
    ((uint8_t *)p)[0] = total;
}

/* ── Frame type helpers ───────────────────────────────────────────────── */
static uint8_t getPacketType(const void *buffer)
{
    const struct MacFcs *fcs = buffer;
    if (fcs->frameType == FRAME_TYPE_DATA && fcs->destAddrType == ADDR_MODE_SHORT
        && fcs->srcAddrType == ADDR_MODE_LONG && fcs->panIdCompressed == 0)
    {
        return ((const uint8_t *)buffer)[sizeof(struct MacFrameBcast)];
    }
    if (fcs->frameType == FRAME_TYPE_DATA && fcs->destAddrType == ADDR_MODE_LONG
        && fcs->srcAddrType == ADDR_MODE_LONG && fcs->panIdCompressed == 1)
    {
        return ((const uint8_t *)buffer)[sizeof(struct MacFrameNormal)];
    }
    return 0;
}

static bool pktIsUnicast(const void *buffer)
{
    const struct MacFcs *fcs = buffer;
    return (fcs->frameType == FRAME_TYPE_DATA && fcs->destAddrType == ADDR_MODE_LONG
            && fcs->srcAddrType == ADDR_MODE_LONG && fcs->panIdCompressed == 1);
}

/* ── AP discovery ─────────────────────────────────────────────────────── */
static void sendPing(void)
{
    struct MacFrameBcast *txframe = (struct MacFrameBcast *)(outBuffer + 1);
    memset(outBuffer, 0, sizeof(struct MacFrameBcast) + 4);
    outBuffer[0] = sizeof(struct MacFrameBcast) + 1 + 2;
    outBuffer[sizeof(struct MacFrameBcast) + 1] = PKT_PING;
    memcpy(txframe->src, mSelfMac, 8);
    txframe->fcs.frameType     = FRAME_TYPE_DATA;
    txframe->fcs.ackReqd       = 1;
    txframe->fcs.destAddrType  = ADDR_MODE_SHORT;
    txframe->fcs.srcAddrType   = ADDR_MODE_LONG;
    txframe->seq               = seq++;
    txframe->dstPan            = PROTO_PAN_ID;
    txframe->dstAddr           = 0xFFFF;
    txframe->srcPan            = PROTO_PAN_ID;
    commsTxNoCpy(outBuffer);
}

uint8_t detectAP(uint8_t channel)
{
    radioRxEnable(false);
    radioSetChannel(channel);
    radioRxFlush();
    radioRxEnable(true);

    for (uint8_t c = 1; c <= MAXIMUM_PING_ATTEMPTS; c++) {
        sendPing();
        uint32_t timeout = clock_time();
        while (!clock_time_exceed(timeout, PING_REPLY_WINDOW * 1000UL)) {
            int8_t ret = commsRxUnencrypted(inBuffer);
            if (ret > 1) {
                if (inBuffer[sizeof(struct MacFrameNormal) + 1] == channel
                    && getPacketType(inBuffer) == PKT_PONG
                    && pktIsUnicast(inBuffer))
                {
                    struct MacFrameNormal *f = (struct MacFrameNormal *)inBuffer;
                    memcpy(APmac, f->src, 8);
                    APsrcPan = f->pan;
                    return c;
                }
            }
            WaitMs(1);
        }
    }
    return 0;
}

/* ── Data request ─────────────────────────────────────────────────────── */
static void sendAvailDataReq(void)
{
    struct MacFrameBcast *txframe = (struct MacFrameBcast *)(outBuffer + 1);
    memset(outBuffer, 0, sizeof(struct MacFrameBcast) + sizeof(struct AvailDataReq) + 4);
    struct AvailDataReq *req = (struct AvailDataReq *)(outBuffer + 2 + sizeof(struct MacFrameBcast));
    outBuffer[0] = sizeof(struct MacFrameBcast) + sizeof(struct AvailDataReq) + 2 + 2;
    outBuffer[sizeof(struct MacFrameBcast) + 1] = PKT_AVAIL_DATA_REQ;
    memcpy(txframe->src, mSelfMac, 8);
    txframe->fcs.frameType    = FRAME_TYPE_DATA;
    txframe->fcs.ackReqd      = 1;
    txframe->fcs.destAddrType = ADDR_MODE_SHORT;
    txframe->fcs.srcAddrType  = ADDR_MODE_LONG;
    txframe->seq              = seq++;
    txframe->dstPan           = PROTO_PAN_ID;
    txframe->dstAddr          = 0xFFFF;
    txframe->srcPan           = PROTO_PAN_ID;
    req->hwType        = HW_TYPE;
    req->wakeupReason  = wakeUpReason;
    req->lastPacketRSSI = mLastRSSI;
    req->lastPacketLQI  = mLastLqi;
    req->temperature   = temperature;
    req->batteryMv     = batteryVoltage;
    req->capabilities  = capabilities;
    req->currentChannel = currentChannel;
    req->tagSoftwareVersion = FIRMWARE_VERSION;
    addCRC(req, sizeof(struct AvailDataReq));
    commsTxNoCpy(outBuffer);
}

static void sendShortAvailDataReq(void)
{
    struct MacFrameBcast *txframe = (struct MacFrameBcast *)(outBuffer + 1);
    outBuffer[0] = sizeof(struct MacFrameBcast) + 1 + 2;
    outBuffer[sizeof(struct MacFrameBcast) + 1] = PKT_AVAIL_DATA_SHORTREQ;
    memcpy(txframe->src, mSelfMac, 8);
    outBuffer[1] = 0x21;
    outBuffer[2] = 0xC8;
    txframe->seq    = seq++;
    txframe->dstPan = PROTO_PAN_ID;
    txframe->dstAddr = 0xFFFF;
    txframe->srcPan  = PROTO_PAN_ID;
    commsTxNoCpy(outBuffer);
}

struct AvailDataInfo *getAvailDataInfo(void)
{
    radioRxEnable(true);
    for (uint8_t c = 0; c < DATA_REQ_MAX_ATTEMPTS; c++) {
        sendAvailDataReq();
        uint32_t timeout = clock_time();
        while (!clock_time_exceed(timeout, DATA_REQ_RX_WINDOW_SIZE * 1000UL)) {
            int8_t ret = commsRxUnencrypted(inBuffer);
            if (ret > 1 && getPacketType(inBuffer) == PKT_AVAIL_DATA_INFO) {
                if (checkCRC(inBuffer + sizeof(struct MacFrameNormal) + 1,
                             sizeof(struct AvailDataInfo)))
                {
                    struct MacFrameNormal *f = (struct MacFrameNormal *)inBuffer;
                    memcpy(APmac, f->src, 8);
                    APsrcPan = f->pan;
                    return (struct AvailDataInfo *)(inBuffer + sizeof(struct MacFrameNormal) + 1);
                }
            }
        }
    }
    return NULL;
}

struct AvailDataInfo *getShortAvailDataInfo(void)
{
    radioRxEnable(true);
    for (uint8_t c = 0; c < DATA_REQ_MAX_ATTEMPTS; c++) {
        sendShortAvailDataReq();
        uint32_t timeout = clock_time();
        while (!clock_time_exceed(timeout, DATA_REQ_RX_WINDOW_SIZE * 1000UL)) {
            int8_t ret = commsRxUnencrypted(inBuffer);
            if (ret > 1 && getPacketType(inBuffer) == PKT_AVAIL_DATA_INFO) {
                if (checkCRC(inBuffer + sizeof(struct MacFrameNormal) + 1,
                             sizeof(struct AvailDataInfo)))
                {
                    struct MacFrameNormal *f = (struct MacFrameNormal *)inBuffer;
                    memcpy(APmac, f->src, 8);
                    APsrcPan = f->pan;
                    return (struct AvailDataInfo *)(inBuffer + sizeof(struct MacFrameNormal) + 1);
                }
            }
        }
    }
    return NULL;
}

/* ── Block transfer ───────────────────────────────────────────────────── */
static bool processBlockPart(const struct blockPart *bp)
{
    uint16_t start = bp->blockPart * BLOCK_PART_DATA_SIZE;
    uint16_t size  = BLOCK_PART_DATA_SIZE;
    if (bp->blockId != curBlock.blockId)   return false;
    if (start >= (BLOCK_XFER_BUFFER_SIZE - 1)) return false;
    if (bp->blockPart > BLOCK_MAX_PARTS)   return false;
    if ((start + size) > BLOCK_XFER_BUFFER_SIZE)
        size = BLOCK_XFER_BUFFER_SIZE - start;
    if (!checkCRC(bp, sizeof(struct blockPart) + BLOCK_PART_DATA_SIZE)) {
        printf("CRC fail part %u\r\n", bp->blockPart);
        return false;
    }
    memcpy(blockXferBuffer + start, bp->data, size);
    curBlock.requestedParts[bp->blockPart / 8] &= ~(1 << (bp->blockPart % 8));
    return true;
}

static bool blockRxLoop(uint32_t timeout_ms)
{
    uint32_t t = clock_time();
    while (!clock_time_exceed(t, timeout_ms * 1000UL)) {
        int8_t ret = commsRxUnencrypted(inBuffer);
        if (ret > 1 && getPacketType(inBuffer) == PKT_BLOCK_PART) {
            struct blockPart *bp = (struct blockPart *)(inBuffer + sizeof(struct MacFrameNormal) + 1);
            processBlockPart(bp);
        }
    }
    return true;
}

static struct blockRequestAck *continueToRX(void)
{
    struct blockRequestAck *ack = (struct blockRequestAck *)(inBuffer + sizeof(struct MacFrameNormal) + 1);
    ack->pleaseWaitMs = 0;
    return ack;
}

static void sendBlockRequest(void)
{
    memset(outBuffer, 0, sizeof(struct MacFrameNormal) + sizeof(struct blockRequest) + 4);
    struct MacFrameNormal *f = (struct MacFrameNormal *)(outBuffer + 1);
    struct blockRequest *req = (struct blockRequest *)(outBuffer + 2 + sizeof(struct MacFrameNormal));
    outBuffer[0] = sizeof(struct MacFrameNormal) + sizeof(struct blockRequest) + 2 + 2;
    outBuffer[sizeof(struct MacFrameNormal) + 1] = requestPartialBlock
        ? PKT_BLOCK_PARTIAL_REQUEST : PKT_BLOCK_REQUEST;
    memcpy(f->src, mSelfMac, 8);
    memcpy(f->dst, APmac, 8);
    f->fcs.frameType      = FRAME_TYPE_DATA;
    f->fcs.panIdCompressed = 1;
    f->fcs.destAddrType   = ADDR_MODE_LONG;
    f->fcs.srcAddrType    = ADDR_MODE_LONG;
    f->seq = seq++;
    f->pan = APsrcPan;
    memcpy(req, &curBlock, sizeof(struct blockRequest));
    addCRC(req, sizeof(struct blockRequest));
    commsTxNoCpy(outBuffer);
}

static struct blockRequestAck *performBlockRequest(void)
{
    printf("performBlockRequest APmac=%02X:%02X pan=%04X\r\n",
           APmac[0], APmac[1], APsrcPan);
    for (uint8_t c = 0; c < 30; c++) {
        sendBlockRequest();
        uint32_t timeout = clock_time();
        do {
            int8_t ret = commsRxUnencrypted(inBuffer);
            if (ret > 1) {
                uint8_t ptype = getPacketType(inBuffer);
                printf("blkRq rx ptype=0x%02X\r\n", ptype);
                switch (ptype) {
                case PKT_BLOCK_REQUEST_ACK:
                    if (checkCRC(inBuffer + sizeof(struct MacFrameNormal) + 1,
                                 sizeof(struct blockRequestAck)))
                        return (struct blockRequestAck *)(inBuffer + sizeof(struct MacFrameNormal) + 1);
                    break;
                case PKT_BLOCK_PART:
                    return continueToRX();
                case PKT_CANCEL_XFER:
                    return NULL;
                }
            }
        } while (!clock_time_exceed(timeout, 50UL * 1000UL));
    }
    printf("performBlockRequest: no ACK after 30 attempts\r\n");
    return continueToRX();
}

static void sendXferCompletePacket(void)
{
    memset(outBuffer, 0, sizeof(struct MacFrameNormal) + 4);
    struct MacFrameNormal *f = (struct MacFrameNormal *)(outBuffer + 1);
    outBuffer[0] = sizeof(struct MacFrameNormal) + 2 + 2;
    outBuffer[sizeof(struct MacFrameNormal) + 1] = PKT_XFER_COMPLETE;
    memcpy(f->src, mSelfMac, 8);
    memcpy(f->dst, APmac, 8);
    f->fcs.frameType       = FRAME_TYPE_DATA;
    f->fcs.panIdCompressed = 1;
    f->fcs.destAddrType    = ADDR_MODE_LONG;
    f->fcs.srcAddrType     = ADDR_MODE_LONG;
    f->pan = APsrcPan;
    f->seq = seq++;
    commsTxNoCpy(outBuffer);
}

static void sendXferComplete(void)
{
    radioRxEnable(true);
    for (uint8_t c = 0; c < 16; c++) {
        sendXferCompletePacket();
        uint32_t timeout = clock_time();
        while (!clock_time_exceed(timeout, 6UL * 1000UL * 1000UL)) {
            int8_t ret = commsRxUnencrypted(inBuffer);
            if (ret > 1 && getPacketType(inBuffer) == PKT_XFER_COMPLETE_ACK) {
                printf("XFC ACK\r\n");
                return;
            }
        }
    }
    printf("XFC no ACK\r\n");
}

static bool validateBlockData(void)
{
    struct blockData *bd = (struct blockData *)blockXferBuffer;
    if (bd->size > BLOCK_XFER_BUFFER_SIZE - sizeof(struct blockData))
        return false;
    uint16_t t = 0;
    for (uint16_t c = 0; c < bd->size; c++)
        t += bd->data[c];
    return bd->checksum == t;
}

/* ── EEPROM slot helpers ──────────────────────────────────────────────── */
static uint32_t getAddressForSlot(uint8_t s)
{
    return PROTO_EEPROM_IMG_START + (PROTO_EEPROM_IMG_EACH * s);
}

static void getNumSlots(void)
{
    uint32_t eeSize = eepromGetSize();
    uint16_t nSlots = (uint16_t)(eeSize / PROTO_EEPROM_IMG_EACH);
    if (eeSize < PROTO_EEPROM_IMG_EACH || !nSlots) {
        printf("EEPROM too small\r\n");
        while (1) { }
    }
    imgSlots = (nSlots > 254) ? 254 : (uint8_t)nSlots;
}

static uint8_t findSlot(const uint8_t *ver)
{
    uint32_t markerValid = EEPROM_IMG_VALID;
    for (uint8_t c = 0; c < imgSlots; c++) {
        struct EepromImageHeader *eih = (struct EepromImageHeader *)blockXferBuffer;
        eepromRead(getAddressForSlot(c), (uint8_t *)eih, sizeof(struct EepromImageHeader));
        if (!memcmp(&eih->validMarker, &markerValid, 4)
            && !memcmp(eih->version, ver, 8))
            return c;
    }
    return 0xFF;
}

static uint32_t getHighSlotId(void)
{
    uint32_t temp = 0;
    uint32_t markerValid = EEPROM_IMG_VALID;
    for (uint8_t c = 0; c < imgSlots; c++) {
        struct EepromImageHeader *eih = (struct EepromImageHeader *)blockXferBuffer;
        eepromRead(getAddressForSlot(c), (uint8_t *)eih, sizeof(struct EepromImageHeader));
        if (!memcmp(&eih->validMarker, &markerValid, 4) && eih->id > temp) {
            temp = eih->id;
            nextImgSlot = c;
        }
    }
    printf("highSlot id=%u slot=%u\r\n", (unsigned)temp, nextImgSlot);
    return temp;
}

static void saveImgBlockData(uint8_t imgSlot, uint8_t blockId)
{
    uint32_t offset = sizeof(struct EepromImageHeader) + blockId * BLOCK_DATA_SIZE;
    uint32_t length = PROTO_EEPROM_IMG_EACH - offset;
    if (length > BLOCK_DATA_SIZE) length = BLOCK_DATA_SIZE;
    eepromWrite(getAddressForSlot(imgSlot) + offset,
                blockXferBuffer + sizeof(struct blockData), (uint32_t)length);
}

static void saveUpdateBlockData(uint8_t blockId)
{
    eepromWrite(PROTO_EEPROM_OTA_START + blockId * BLOCK_DATA_SIZE,
                blockXferBuffer + sizeof(struct blockData), BLOCK_DATA_SIZE);
}

void drawImageFromEeprom(uint8_t imgSlot)
{
    drawImageAtAddress(getAddressForSlot(imgSlot), drawWithLut);
    drawWithLut = 0;
}

/* ── OTA firmware flash write ─────────────────────────────────────────── */
static uint32_t otaFwSize = 0;

void write_ota_firmware_to_flash(void)
{
    printf("OTA: applying fw_size=%lu\r\n", (unsigned long)otaFwSize);
    ota_apply(otaFwSize);
}

/* ── Block download ───────────────────────────────────────────────────── */
static uint8_t  partsThisBlock = 0;
static uint8_t  blockAttempts  = 0;

static bool getDataBlock(uint16_t blockSize)
{
    blockAttempts = BLOCK_TRANSFER_ATTEMPTS;
    if (blockSize == BLOCK_DATA_SIZE) {
        partsThisBlock = BLOCK_MAX_PARTS;
        memset(curBlock.requestedParts, 0xFF, BLOCK_REQ_PARTS_BYTES);
    } else {
        partsThisBlock = (uint8_t)((sizeof(struct blockData) + blockSize) / BLOCK_PART_DATA_SIZE);
        if ((sizeof(struct blockData) + blockSize) % BLOCK_PART_DATA_SIZE)
            partsThisBlock++;
        memset(curBlock.requestedParts, 0x00, BLOCK_REQ_PARTS_BYTES);
        for (uint8_t c = 0; c < partsThisBlock; c++)
            curBlock.requestedParts[c / 8] |= (1 << (c % 8));
    }
    requestPartialBlock = false;

    while (blockAttempts--) {
        wdt10s();
        printf("REQ block %u\r\n", curBlock.blockId);

        struct blockRequestAck *ack = performBlockRequest();
        if (!ack) {
            printf("block request cancelled\r\n");
            return false;
        }
        if (ack->pleaseWaitMs)
            WaitMs(ack->pleaseWaitMs - 10);

        blockRxLoop(300);

        bool complete = true;
        for (uint8_t c = 0; c < partsThisBlock; c++) {
            if (curBlock.requestedParts[c / 8] & (1 << (c % 8))) {
                complete = false;
                break;
            }
        }

        if (complete) {
            if (validateBlockData()) {
                printf("block %u OK\r\n", curBlock.blockId);
                return true;
            }
            /* Validation failed — re-request everything */
            for (uint8_t c = 0; c < partsThisBlock; c++)
                curBlock.requestedParts[c / 8] |= (1 << (c % 8));
            requestPartialBlock = false;
            printf("block validation failed\r\n");
        } else {
            requestPartialBlock = true;
        }
    }
    printf("block download failed\r\n");
    return false;
}

/* ── Image download to EEPROM ─────────────────────────────────────────── */
static uint16_t dataRequestSize = 0;
static uint16_t imageSize       = 0;

static bool downloadImageDataToEEPROM(const struct AvailDataInfo *avail)
{
    if (!memcmp(&avail->dataVer, &curDataInfo.dataVer, 8) && curDataInfo.dataSize) {
        printf("resuming image download\r\n");
        curImgSlot = nextImgSlot;
    } else {
        nextImgSlot++;
        if (nextImgSlot >= imgSlots) nextImgSlot = 0;
        curImgSlot  = nextImgSlot;
        drawWithLut = avail->dataTypeArgument;
        printf("new download to slot %u\r\n", curImgSlot);

        printf("Erasing addr=0x%lX len=0x%lX\r\n",
               (unsigned long)getAddressForSlot(curImgSlot),
               (unsigned long)PROTO_EEPROM_IMG_EACH);
        uint8_t attempt = 5;
        while (attempt--) {
            uint8_t r = eepromErase(getAddressForSlot(curImgSlot), PROTO_EEPROM_IMG_EACH);
            printf("erase attempt result=%u\r\n", r);
            if (r) goto eraseOk;
        }
        printf("erase failed\r\n");
        return false;
    eraseOk:
        printf("erase OK, dataSize=%lu\r\n", (unsigned long)avail->dataSize);
        curBlock.blockId = 0;
        memcpy(&curBlock.ver, &avail->dataVer, 8);
        curBlock.type = avail->dataType;
        memcpy(&curDataInfo, avail, sizeof(struct AvailDataInfo));
        imageSize = (uint16_t)curDataInfo.dataSize;
    }

    printf("download loop: dataSize=%lu\r\n", (unsigned long)curDataInfo.dataSize);
    while (curDataInfo.dataSize) {
        wdt10s();
        dataRequestSize = (curDataInfo.dataSize > BLOCK_DATA_SIZE)
                          ? BLOCK_DATA_SIZE : (uint16_t)curDataInfo.dataSize;
        printf("requesting block %u (%u bytes)\r\n", curBlock.blockId, dataRequestSize);
        if (getDataBlock(dataRequestSize)) {
            saveImgBlockData(curImgSlot, curBlock.blockId);
            curBlock.blockId++;
            curDataInfo.dataSize -= dataRequestSize;
        } else {
            return false;
        }
    }

    /* Write header to mark slot valid */
    struct EepromImageHeader *eih = (struct EepromImageHeader *)blockXferBuffer;
    memcpy(eih->version, &curDataInfo.dataVer, 8);
    eih->validMarker = EEPROM_IMG_VALID;
    eih->id          = ++curHighSlotId;
    eih->size        = imageSize;
    eih->dataType    = curDataInfo.dataType;
    eepromWrite(getAddressForSlot(curImgSlot), (uint8_t *)eih, sizeof(struct EepromImageHeader));
    return true;
}

static bool downloadFWUpdate(const struct AvailDataInfo *avail)
{
    otaFwSize = avail->dataSize;
    if (!memcmp(&avail->dataVer, &curDataInfo.dataVer, 8) && curDataInfo.dataSize) {
        /* resume */
    } else {
        curBlock.blockId = 0;
        memcpy(&curBlock.ver, &avail->dataVer, 8);
        curBlock.type = avail->dataType;
        memcpy(&curDataInfo, avail, sizeof(struct AvailDataInfo));
        eepromErase(PROTO_EEPROM_OTA_START, PROTO_EEPROM_OTA_LEN);
    }
    while (curDataInfo.dataSize) {
        wdt10s();
        dataRequestSize = (curDataInfo.dataSize > BLOCK_DATA_SIZE)
                          ? BLOCK_DATA_SIZE : (uint16_t)curDataInfo.dataSize;
        if (getDataBlock(dataRequestSize)) {
            saveUpdateBlockData(curBlock.blockId);
            curBlock.blockId++;
            curDataInfo.dataSize -= dataRequestSize;
        } else {
            return false;
        }
    }
    return true;
}

/* ── Data processing ──────────────────────────────────────────────────── */
bool processAvailDataInfo(struct AvailDataInfo *avail)
{
    printf("dataType 0x%02X\r\n", avail->dataType);
    switch (avail->dataType) {
    case DATATYPE_NOUPDATE:
        return true;

    case DATATYPE_IMG_RAW_1BPP:
    case DATATYPE_IMG_RAW_2BPP:
    case DATATYPE_IMG_ZLIB:
        if (!curDataInfo.dataSize
            && !memcmp(&avail->dataVer, &curDataInfo.dataVer, 8))
        {
            printf("already shown, sending XFC\r\n");
            sendXferComplete();
            return true;
        }
        curImgSlot = findSlot((const uint8_t *)&avail->dataVer);
        if (curImgSlot != 0xFF) {
            sendXferComplete();
            printf("cached in slot %u\r\n", curImgSlot);
            memcpy(&curDataInfo, avail, sizeof(struct AvailDataInfo));
            curDataInfo.dataSize = 0;
            drawWithLut = avail->dataTypeArgument;
            wdt60s();
            drawImageFromEeprom(curImgSlot);
            return true;
        }
        drawWithLut = avail->dataTypeArgument;
        if (downloadImageDataToEEPROM(avail)) {
            sendXferComplete();
            wdt60s();
            drawImageFromEeprom(curImgSlot);
            return true;
        }
        return false;

    case DATATYPE_FW_UPDATE:
        if (downloadFWUpdate(avail)) {
            sendXferComplete();
            printf("FW update complete, flashing\r\n");
            write_ota_firmware_to_flash();
        }
        return false;

    default:
        printf("unhandled dataType 0x%02X\r\n", avail->dataType);
        return true;
    }
}

/* ── Init ─────────────────────────────────────────────────────────────── */
void initializeProto(void)
{
    /* Allocate the 4100-byte block transfer buffer from the FreeRTOS heap
     * (HEAP_DATA_ON, 52 KB) rather than keeping it in the static BSS region
     * (APP_DATA_ON, 16 KB).  Allocated once and never freed. */
    if (!blockXferBuffer) {
        blockXferBuffer = os_mem_alloc(RAM_TYPE_DATA_ON, BLOCK_XFER_BUFFER_SIZE);
        if (!blockXferBuffer) {
            printf("FATAL: blockXferBuffer alloc failed\r\n");
            while (1) { }
        }
    }
    getNumSlots();
    curHighSlotId = getHighSlotId();
}
