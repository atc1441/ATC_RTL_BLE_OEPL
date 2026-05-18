#include "powermgt.h"
#include "zigbee.h"
#include "battery.h"
#include <rtl876x_aon_wdg.h>
#include <stdio.h>
#include "eeprom.h"
#include <rtl876x_pinmux.h>
#include <rtl876x_rcc.h>
#include <rtl876x_gpio.h>
#include "board.h"
#include "retarget.h"
#include "epd.h"
#include "drawing.h"
#include "syncedproto.h"
#include "boot_screen.h"
#include <os_task.h>
#include <os_sched.h>
#include <dlps.h>
#include <stdlib.h>

extern volatile uint32_t g_dlps_enter_count;

/* ── Global state ─────────────────────────────────────────────────────── */
uint8_t wakeUpReason = WAKEUP_REASON_FIRSTBOOT;
uint8_t capabilities = 0;
int8_t temperature = 0;
uint16_t batteryVoltage = 0;
bool lowBattery = false;
uint16_t longDataReqCounter = 0;
uint16_t voltageCheckCounter = 0;

/* ── Watchdog ─────────────────────────────────────────────────────────── */
void wdt10s(void) { AON_WDG_Restart(); }
void wdt60s(void) { AON_WDG_Restart(); }

/* ── Timing ───────────────────────────────────────────────────────────── */
uint32_t getMillis(void)
{
    return clock_time() / 1000u;
}

/* ── Sleep ────────────────────────────────────────────────────────────── */
void doSleep(uint32_t t)
{
    t += (uint32_t)(rand() % 61);
    uart_flush();
    eepromPowerDown();
    radioSleep(t);
    /* ADC: power down the analog block before sleep (AON reg 0x113 bit2 = 1). */
    //uint8_t adc_r = btaon_fast_read_safe(0x113);
    //btaon_fast_write(0x113, adc_r | 0x04u);
    AON_WDG_Disable();
    WaitMs(t); /* blocks task → DLPS framework calls enter/exit callbacks automatically */
    AON_WDG_Enable();
    /* ADC: re-enable the analog block immediately on wake. */
    //adc_r = btaon_fast_read_safe(0x113);
    //btaon_fast_write(0x113, adc_r & ~0x04u);
    uint32_t wakeup_cnt, last_wakeup, last_sleep;
    platform_pm_get_statistics(&wakeup_cnt, &last_wakeup, &last_sleep);
    printf("Sleep end: dlps_cb=%u dlps_total=%u pm_err=%u zb_err=%u\r\n",
           (unsigned)g_dlps_enter_count,
           (unsigned)wakeup_cnt,
           (unsigned)platform_pm_get_error_code(),
           (unsigned)radioGetZbpmError());
    radioWake();
    eepromPowerUp();
    uart_flush();
}
