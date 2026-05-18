#pragma once

#include <stdint.h>
#include <stdbool.h>

/*
 * Radio channel list — same six channels as the reference implementation.
 * detectAP() scans these in order.
 */
extern uint8_t channelList[6];

/*
 * ── Radio HAL ────────────────────────────────────────────────────────────
 * Interface is intentionally identical to the Example/src/zigbee.h so that
 * comms.c and syncedproto.c compile unchanged (except the tl_common.h
 * platform includes which are replaced by including this header).
 */

/* One-time hardware initialization. Call before using any other function.
 * mSelfMac must be set by the caller before radioInit(); it is written to
 * the MAC hardware filter so only frames addressed to us (or broadcasts) are
 * passed to the RX ring buffer. */
bool radioInit(void);

/* Switch to a new 802.15.4 channel (11-26).
 * Temporarily disables the radio, changes the channel, then restores state. */
bool radioSetChannel(uint_fast8_t channel);

/* Enable or disable the radio receiver. */
bool radioRxEnable(bool on);

/* Discard all frames currently waiting in the software RX ring buffer. */
void radioRxFlush(void);

/*
 * Non-blocking RX dequeue.
 * Returns the frame length on success, -1 if no frame is available.
 * dstBuf receives the raw 802.15.4 frame bytes (MAC header + payload,
 * no FCS). maxLen is the size of dstBuf.
 */
int32_t radioRxDequeuePkt(uint8_t *dstBuf, uint32_t maxLen,
                           int8_t *rssiP, uint8_t *lqiP);

/*
 * Blocking TX.
 * pkt[0]  = total frame byte count including 2-byte FCS placeholder
 *           (i.e. actual MAC bytes to send = pkt[0] - 2)
 * pkt[1+] = raw 802.15.4 frame (MAC header + payload)
 * The hardware appends the real FCS; the caller's +2 is just a length
 * convention inherited from the reference implementation.
 *
 * Disables RX, transmits, spins until TX-done IRQ (max TX_TIMEOUT_US),
 * then restores RX if it was enabled.
 */
bool radioTxLL(uint8_t *pkt);

/*
 * ── Platform timing ───────────────────────────────────────────────────────
 * These replace the TELINK clock_time() / WaitMs() used in syncedproto.c.
 * Include this header instead of tl_common.h in ported files.
 */

/* Returns a free-running microsecond counter (BT-clock derived, 32-bit). */
uint32_t clock_time(void);

/* Returns true when at least 'us' microseconds have elapsed since 'ref'. */
bool clock_time_exceed(uint32_t ref, uint32_t us);

/* Suspend the calling task for at least 'ms' milliseconds (scheduler-safe). */
void WaitMs(uint32_t ms);

/* Busy-wait for at least 'us' microseconds (does not yield). */
void WaitUs(uint32_t us);

/*
 * radioSleep / radioWake — power the RF modem down and back up.
 * sleep_ms must be the same duration passed to the subsequent WaitMs() call
 * so the ZBMAC PM framework can set the correct wakeup timestamp before
 * allowing DLPS entry.  DLPS may engage during the WaitMs wait; the channel
 * register is preserved and radioWake() re-acquires the PHY grant.
 */
void radioSleep(uint32_t sleep_ms);
/* Call immediately before WaitMs(sleep_ms) to set the ZBMAC PM wakeup timestamp
 * as close as possible to the FreeRTOS deadline, minimising timing jitter. */
void radioArmWakeup(uint32_t sleep_ms);
void radioWake(void);
/* Returns s_zbpm_adap.error_code — ZBMAC_PM_ERROR_* from power_manager_unit_zbmac.h */
uint8_t radioGetZbpmError(void);
