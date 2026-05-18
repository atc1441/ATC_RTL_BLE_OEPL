#pragma once

#include <stdint.h>
#include <stdbool.h>

/* Wakeup reason codes */
#define WAKEUP_REASON_TIMED         0
#define WAKEUP_REASON_GPIO          2
#define WAKEUP_REASON_NFC           3
#define WAKEUP_REASON_FIRSTBOOT     0xFC
#define WAKEUP_REASON_NETWORK_SCAN  0xFD
#define WAKEUP_REASON_WDT_RESET     0xFE

/* Power-saving algorithm (must match proto.h values) */
#define INTERVAL_BASE               40
#define INTERVAL_AT_MAX_ATTEMPTS    600
#define DATA_REQ_RX_WINDOW_SIZE     5UL
#define DATA_REQ_MAX_ATTEMPTS       14
#define POWER_SAVING_SMOOTHING      8
#define MINIMUM_INTERVAL            45
#define MAXIMUM_PING_ATTEMPTS       20
#define PING_REPLY_WINDOW           5UL

#define LONG_DATAREQ_INTERVAL       300
#define VOLTAGE_CHECK_INTERVAL      288
#define BATTERY_VOLTAGE_MINIMUM     2450

#define INTERVAL_1_TIME             3600UL
#define INTERVAL_1_ATTEMPTS         24
#define INTERVAL_2_TIME             7200UL
#define INTERVAL_2_ATTEMPTS         12
#define INTERVAL_3_TIME             86400UL

/* Watchdog helpers */
void wdt10s(void);
void wdt60s(void);

/* Millisecond counter (free-running, wraps at ~49 days) */
uint32_t getMillis(void);

/* Sleep for t milliseconds — yields to FreeRTOS, DLPS engages if idle */
void doSleep(uint32_t t);

/* Global state used by main loop and protocol */
extern uint8_t  wakeUpReason;
extern uint8_t  capabilities;
extern uint8_t  dataReqLastAttempt;
extern int8_t   temperature;
extern uint16_t batteryVoltage;
extern bool     lowBattery;
extern uint16_t longDataReqCounter;
extern uint16_t voltageCheckCounter;
