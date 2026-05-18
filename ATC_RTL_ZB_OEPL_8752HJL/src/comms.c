#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include "proto.h"
#include "zigbee.h"
#include "comms.h"

extern uint8_t mSelfMac[8];

uint8_t mLastLqi  = 0;
int8_t  mLastRSSI = 0;

uint8_t commsGetLastPacketLQI(void)  { return mLastLqi; }
int8_t  commsGetLastPacketRSSI(void) { return mLastRSSI; }

int8_t commsRxUnencrypted(uint8_t *data)
{
    memset(data, 0, 128);
    int8_t rxedLen = (int8_t)radioRxDequeuePkt(data, 128, &mLastRSSI, &mLastLqi);
    if (rxedLen < 0)
        return COMMS_RX_ERR_NO_PACKETS;
    /* Software filter: destination MAC (bytes 5-12 for MacFrameNormal) must match ours */
    if (memcmp((void *)&data[5], mSelfMac, 8))
        return COMMS_RX_ERR_INVALID_PACKET;
    return rxedLen;
}

bool commsTxNoCpy(const void *packetp)
{
    return radioTxLL((uint8_t *)packetp);
}
