/*
 * retarget.c — UART0 debug output
 *
 * Public API:
 *   uart_print_init()   — call once from driver_init() (inside RTOS task, after BLE stack)
 *   uart_printf(fmt,..) — use instead of printf(); bypasses newlib FILE/locking entirely
 *   uart_puts(s)        — send a plain string
 *
 * WHY NOT printf():
 *   Calling setvbuf(stdout,...) or printf() before newlib's __sinit() has run
 *   (i.e. before the first printf in a running task) corrupts the FILE structure
 *   and causes subsequent printf calls to hang inside __sfp_r()/__fp_lock().
 *   uart_printf() avoids this entirely by using only vsnprintf() + direct UART
 *   writes — no FILE pointer, no locks, no buffering state.
 *
 * Pins and baud rate are set in config/board.h.
 */

#include <stdio.h>
#include <stdarg.h>
#include <rtl876x_uart.h>
#include <rtl876x_rcc.h>
#include <rtl876x_pinmux.h>
#include "board.h"

/* --------------------------------------------------------------------------
 * Baud-rate table (40 MHz clock source, 8N1)
 *   115200  div=0x30  ovsr=2  ovsr_adj=0x003
 *   230400  div=0x18  ovsr=2  ovsr_adj=0x003
 *   460800  div=0x0C  ovsr=2  ovsr_adj=0x003
 *   921600  div=0x06  ovsr=2  ovsr_adj=0x003
 * -------------------------------------------------------------------------- */
#if UART_BAUDRATE == 921600
#define _UART_DIV 0x06
#define _UART_OVSR 0x02
#define _UART_ADJ 0x003
#elif UART_BAUDRATE == 460800
#define _UART_DIV 0x0C
#define _UART_OVSR 0x02
#define _UART_ADJ 0x003
#elif UART_BAUDRATE == 230400
#define _UART_DIV 0x18
#define _UART_OVSR 0x02
#define _UART_ADJ 0x003
#else /* default 115200 */
#define _UART_DIV 0x30
#define _UART_OVSR 0x02
#define _UART_ADJ 0x003
#endif

/* --------------------------------------------------------------------------
 * uart_print_init — hardware init only, no newlib calls.
 * Call from driver_init() inside the RTOS task after gap_start_bt_stack().
 * -------------------------------------------------------------------------- */
void uart_print_init(void)
{
    RCC_PeriphClockCmd(APBPeriph_UART0, APBPeriph_UART0_CLOCK, ENABLE);

    Pad_Config(UART_TX_PIN, PAD_PINMUX_MODE, PAD_IS_PWRON, PAD_PULL_NONE, PAD_OUT_ENABLE, PAD_OUT_HIGH);
    Pad_Config(UART_RX_PIN, PAD_PINMUX_MODE, PAD_IS_PWRON, PAD_PULL_UP, PAD_OUT_DISABLE, PAD_OUT_LOW);

    Pinmux_Config(UART_TX_PIN, UART0_TX);
    Pinmux_Config(UART_RX_PIN, UART0_RX);

    UART_InitTypeDef init;
    UART_StructInit(&init);
    init.UART_Div = _UART_DIV;
    init.UART_Ovsr = _UART_OVSR;
    init.UART_OvsrAdj = _UART_ADJ;
    init.UART_WordLen = UART_WORD_LENGTH_8BIT;
    init.UART_StopBits = UART_STOP_BITS_1;
    init.UART_Parity = UART_PARITY_NO_PARTY;
    UART_Init(UART0, &init);
    /* No setvbuf here — calling it before __sinit() runs corrupts stdout. */
}

/* --------------------------------------------------------------------------
 * uart_puts — send a null-terminated string, \n -> \r\n.
 *
 * RCC_PeriphClockCmd is called ONCE per string, not per byte.
 * Calling it on every byte causes repeated writes to the clock-control
 * register which interferes with BLE stack timing during connection setup.
 *
 * The UART0 APB clock is restored after each DLPS wakeup via
 * io_dlps_exit_cb() in main.c, so we only need to ensure it is on here
 * for the first call (before DLPS has ever fired) and after any reset.
 *
 * Timeout: ~10000 iterations ≈ 1 ms at 40 MHz.
 * At 115200 baud one byte takes ~87 µs (~3500 cycles), well within limit.
 * If the clock is still gated (should not happen after exit_cb), the
 * function simply skips that byte rather than blocking for 10 ms.
 * -------------------------------------------------------------------------- */
void uart_puts(const char *s)
{
    RCC_PeriphClockCmd(APBPeriph_UART0, APBPeriph_UART0_CLOCK, ENABLE);
    while (*s)
    {
        if (*s == '\n')
        {
            volatile uint32_t t = 10000;
            while ((UART0->LSR & (1u << 5)) == 0 && --t)
                ;
            UART0->RB_THR = '\r';
        }
        volatile uint32_t t = 10000;
        while ((UART0->LSR & (1u << 5)) == 0 && --t)
            ;
        UART0->RB_THR = (uint8_t)*s++;
    }
}

/* --------------------------------------------------------------------------
 * uart_bdaddr_str — format a 6-byte BD address to "XX:XX:XX:XX:XX:XX".
 * Used by the TRACE_BDADDR override in log_uart.h.
 * Static buffer — not re-entrant, but fine for debug logging.
 * -------------------------------------------------------------------------- */
const char *uart_bdaddr_str(const uint8_t *addr)
{
    static char buf[18];
    if (!addr)
        return "??:??:??:??:??:??";
    /* BLE BD addresses are transmitted LSB first; display MSB first */
    snprintf(buf, sizeof(buf), "%02X:%02X:%02X:%02X:%02X:%02X",
             addr[5], addr[4], addr[3], addr[2], addr[1], addr[0]);
    return buf;
}

/* --------------------------------------------------------------------------
 * uart_printf — printf-style output via UART, bypasses newlib FILE/locking.
 * Returns the number of characters written (printf-compatible signature).
 * Buffer is 256 bytes; longer messages are silently truncated.
 * -------------------------------------------------------------------------- */
int uart_printf(const char *fmt, ...)
{
    char buf[256];
    va_list ap;
    va_start(ap, fmt);
    int n = vsnprintf(buf, sizeof(buf), fmt, ap);
    va_end(ap);
    if (n > 0)
        uart_puts(buf);
    return n;
}

/* --------------------------------------------------------------------------
 * HardFault_Handler — overrides the weak Default_Handler from startup_rtl876x_gcc.s.
 *
 * Also catches IRQ[5] "Platform interrupt" which Realtek maps to HardFault_Handler
 * for internal SDK assertion failures.
 *
 * The processor stacks {R0-R3, R12, LR, PC, xPSR} on the active stack before
 * entering the handler.  In RTOS task mode the active stack is PSP; we read it
 * with MRS and dump it via UART so the fault address is visible on the terminal.
 *
 * CFSR / HFSR are Cortex-M3/M4 only — they read as 0 on Cortex-M0/M0+.
 * -------------------------------------------------------------------------- */
void HardFault_Handler_C(uint32_t *sp) __attribute__((used));
void HardFault_Handler_C(uint32_t *sp)
{
    RCC_PeriphClockCmd(APBPeriph_UART0, APBPeriph_UART0_CLOCK, ENABLE);
    uart_puts("\r\n*** HARDFAULT / PLATFORM ERROR ***\r\n");
    uart_printf("R0 =0x%08X  R1 =0x%08X\r\n", (unsigned)sp[0], (unsigned)sp[1]);
    uart_printf("R2 =0x%08X  R3 =0x%08X\r\n", (unsigned)sp[2], (unsigned)sp[3]);
    uart_printf("R12=0x%08X  LR =0x%08X\r\n", (unsigned)sp[4], (unsigned)sp[5]);
    uart_printf("PC =0x%08X  xPSR=0x%08X\r\n", (unsigned)sp[6], (unsigned)sp[7]);
    uart_printf("HFSR=0x%08X CFSR=0x%08X\r\n",
                (unsigned)*(volatile uint32_t *)0xE000ED2CUL,
                (unsigned)*(volatile uint32_t *)0xE000ED28UL);
    uart_puts("--- halted ---\r\n");
    while (1)
        ;
}

/* Naked trampoline: selects PSP (task context) or MSP (handler context) and
 * passes the stack pointer as the first argument to HardFault_Handler_C.
 * Uses only Thumb-16 instructions so it assembles for both Cortex-M0+ and M4. */
__attribute__((naked)) void HardFault_Handler(void)
{
    __asm volatile(
        "MOV  R0, LR          \n" /* EXC_RETURN value */
        "MOV  R1, #4          \n"
        "TST  R0, R1          \n" /* bit2=0 -> MSP, bit2=1 -> PSP */
        "BEQ  1f              \n"
        "MRS  R0, PSP         \n" /* task was using PSP */
        "LDR  R1, =HardFault_Handler_C \n"
        "BX   R1              \n"
        "1:                   \n"
        "MRS  R0, MSP         \n" /* handler / bare-metal was using MSP */
        "LDR  R1, =HardFault_Handler_C \n"
        "BX   R1              \n"
    );
}

/* --------------------------------------------------------------------------
 * _isatty / _write — newlib hooks for compatibility with direct printf().
 * Call fflush(stdout) after each printf() to force buffered output out.
 * -------------------------------------------------------------------------- */
int _isatty(int fd)
{
    return (fd == 1 || fd == 2) ? 1 : 0;
}

int _write(int fd, char *buf, int len)
{
    (void)fd;
    RCC_PeriphClockCmd(APBPeriph_UART0, APBPeriph_UART0_CLOCK, ENABLE);
    for (int i = 0; i < len; i++)
    {
        if (buf[i] == '\n')
        {
            volatile uint32_t t = 10000;
            while ((UART0->LSR & (1u << 5)) == 0 && --t)
                ;
            UART0->RB_THR = '\r';
        }
        volatile uint32_t t = 10000;
        while ((UART0->LSR & (1u << 5)) == 0 && --t)
            ;
        UART0->RB_THR = (uint8_t)buf[i];
    }
    return len;
}
