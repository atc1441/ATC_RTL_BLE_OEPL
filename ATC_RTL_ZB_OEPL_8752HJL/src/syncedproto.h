#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "proto.h"

extern uint8_t  mSelfMac[8];
extern uint8_t  currentChannel;
extern uint8_t  APmac[8];
extern uint8_t  curImgSlot;

void   initializeProto(void);
uint8_t detectAP(uint8_t channel);

struct AvailDataInfo *getAvailDataInfo(void);
struct AvailDataInfo *getShortAvailDataInfo(void);

bool processAvailDataInfo(struct AvailDataInfo *avail);
void drawImageFromEeprom(uint8_t imgSlot);
void write_ota_firmware_to_flash(void);
