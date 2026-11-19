/*
 * log_uart.h — redirect RTL APP_PRINT_* macros to UART output
 *
 * Include this header AFTER <trace.h> in any file where you want the RTL
 * log macros (APP_PRINT_INFO, APP_PRINT_ERROR, …) to appear on the UART.
 * The binary RTL trace still fires in parallel — only the UART output is added.
 *
 * Controlled by UART_LOG_EN in app_flags.h:
 *   0 → macros are left as-is (RTL binary trace only)
 *   1 → macros are redirected to uart_printf (UART output)
 *
 * Usage in a source file:
 *   #include <trace.h>       // must come first
 *   #include "log_uart.h"    // overrides APP_PRINT_* when UART_LOG_EN=1
 *
 * Format-specifier compatibility:
 *   %d %u %x %s %c  → fully supported
 *   TRACE_BDADDR()  → returns "XX:XX:XX:XX:XX:XX" string, use with %s
 *   TRACE_STRING()  → returns the string pointer, use with %s
 *   TRACE_BINARY()  → returns a hex string, use with %s
 *   %b              → Realtek-only specifier, NOT supported by uart_printf;
 *                     replace with %s when using TRACE_BINARY()
 */

#ifndef _LOG_UART_H_
#define _LOG_UART_H_

#include "app_flags.h"
#include "retarget.h"

#if UART_LOG_EN

/* ---- Safe replacements for TRACE_* helper macros ----------------------
 *
 * The original trace_bdaddr() / trace_string() / trace_binary() functions
 * in bee3_sdk.a do two things:
 *   1. Send binary-encoded data to the Realtek log UART (LogUartTxChar).
 *   2. Return a const char * for use as a %s argument.
 *
 * Problem: if the log system is not initialised, they may return NULL,
 * causing vsnprintf to dereference a null pointer → HardFault.
 * Additionally, LogUartTxChar may write binary bytes to UART0 while
 * uart_printf is mid-transmission, corrupting the output.
 *
 * These replacements bypass the Realtek log system entirely and produce
 * plain strings that work safely with uart_printf / standard vsnprintf.
 * -------------------------------------------------------------------- */
#undef TRACE_BDADDR
#undef TRACE_STRING
#undef TRACE_BINARY

/* uart_bdaddr_str formats "XX:XX:XX:XX:XX:XX", never returns NULL */
#define TRACE_BDADDR(addr)          uart_bdaddr_str((const uint8_t *)(addr))

/* TRACE_STRING was just the pointer anyway — cast it safely */
#define TRACE_STRING(data)          ((const char *)(data) ? (const char *)(data) : "")

/* TRACE_BINARY is only used with Realtek's %b specifier which uart_printf
 * does not support; replace with a placeholder so %s works safely */
#define TRACE_BINARY(length, data)  ("<bin>")

/* ---- ERROR ------------------------------------------------------------ */
#undef APP_PRINT_ERROR0
#undef APP_PRINT_ERROR1
#undef APP_PRINT_ERROR2
#undef APP_PRINT_ERROR3
#undef APP_PRINT_ERROR4
#undef APP_PRINT_ERROR5
#undef APP_PRINT_ERROR6
#undef APP_PRINT_ERROR7
#undef APP_PRINT_ERROR8

#define APP_PRINT_ERROR0(fmt)                           uart_printf("[E] " fmt "\n")
#define APP_PRINT_ERROR1(fmt,a0)                        uart_printf("[E] " fmt "\n",a0)
#define APP_PRINT_ERROR2(fmt,a0,a1)                     uart_printf("[E] " fmt "\n",a0,a1)
#define APP_PRINT_ERROR3(fmt,a0,a1,a2)                  uart_printf("[E] " fmt "\n",a0,a1,a2)
#define APP_PRINT_ERROR4(fmt,a0,a1,a2,a3)               uart_printf("[E] " fmt "\n",a0,a1,a2,a3)
#define APP_PRINT_ERROR5(fmt,a0,a1,a2,a3,a4)            uart_printf("[E] " fmt "\n",a0,a1,a2,a3,a4)
#define APP_PRINT_ERROR6(fmt,a0,a1,a2,a3,a4,a5)         uart_printf("[E] " fmt "\n",a0,a1,a2,a3,a4,a5)
#define APP_PRINT_ERROR7(fmt,a0,a1,a2,a3,a4,a5,a6)      uart_printf("[E] " fmt "\n",a0,a1,a2,a3,a4,a5,a6)
#define APP_PRINT_ERROR8(fmt,a0,a1,a2,a3,a4,a5,a6,a7)   uart_printf("[E] " fmt "\n",a0,a1,a2,a3,a4,a5,a6,a7)

/* ---- WARN ------------------------------------------------------------- */
#undef APP_PRINT_WARN0
#undef APP_PRINT_WARN1
#undef APP_PRINT_WARN2
#undef APP_PRINT_WARN3
#undef APP_PRINT_WARN4
#undef APP_PRINT_WARN5
#undef APP_PRINT_WARN6
#undef APP_PRINT_WARN7
#undef APP_PRINT_WARN8

#define APP_PRINT_WARN0(fmt)                            uart_printf("[W] " fmt "\n")
#define APP_PRINT_WARN1(fmt,a0)                         uart_printf("[W] " fmt "\n",a0)
#define APP_PRINT_WARN2(fmt,a0,a1)                      uart_printf("[W] " fmt "\n",a0,a1)
#define APP_PRINT_WARN3(fmt,a0,a1,a2)                   uart_printf("[W] " fmt "\n",a0,a1,a2)
#define APP_PRINT_WARN4(fmt,a0,a1,a2,a3)                uart_printf("[W] " fmt "\n",a0,a1,a2,a3)
#define APP_PRINT_WARN5(fmt,a0,a1,a2,a3,a4)             uart_printf("[W] " fmt "\n",a0,a1,a2,a3,a4)
#define APP_PRINT_WARN6(fmt,a0,a1,a2,a3,a4,a5)          uart_printf("[W] " fmt "\n",a0,a1,a2,a3,a4,a5)
#define APP_PRINT_WARN7(fmt,a0,a1,a2,a3,a4,a5,a6)       uart_printf("[W] " fmt "\n",a0,a1,a2,a3,a4,a5,a6)
#define APP_PRINT_WARN8(fmt,a0,a1,a2,a3,a4,a5,a6,a7)    uart_printf("[W] " fmt "\n",a0,a1,a2,a3,a4,a5,a6,a7)

/* ---- INFO ------------------------------------------------------------- */
#undef APP_PRINT_INFO0
#undef APP_PRINT_INFO1
#undef APP_PRINT_INFO2
#undef APP_PRINT_INFO3
#undef APP_PRINT_INFO4
#undef APP_PRINT_INFO5
#undef APP_PRINT_INFO6
#undef APP_PRINT_INFO7
#undef APP_PRINT_INFO8

#define APP_PRINT_INFO0(fmt)                            uart_printf("[I] " fmt "\n")
#define APP_PRINT_INFO1(fmt,a0)                         uart_printf("[I] " fmt "\n",a0)
#define APP_PRINT_INFO2(fmt,a0,a1)                      uart_printf("[I] " fmt "\n",a0,a1)
#define APP_PRINT_INFO3(fmt,a0,a1,a2)                   uart_printf("[I] " fmt "\n",a0,a1,a2)
#define APP_PRINT_INFO4(fmt,a0,a1,a2,a3)                uart_printf("[I] " fmt "\n",a0,a1,a2,a3)
#define APP_PRINT_INFO5(fmt,a0,a1,a2,a3,a4)             uart_printf("[I] " fmt "\n",a0,a1,a2,a3,a4)
#define APP_PRINT_INFO6(fmt,a0,a1,a2,a3,a4,a5)          uart_printf("[I] " fmt "\n",a0,a1,a2,a3,a4,a5)
#define APP_PRINT_INFO7(fmt,a0,a1,a2,a3,a4,a5,a6)       uart_printf("[I] " fmt "\n",a0,a1,a2,a3,a4,a5,a6)
#define APP_PRINT_INFO8(fmt,a0,a1,a2,a3,a4,a5,a6,a7)    uart_printf("[I] " fmt "\n",a0,a1,a2,a3,a4,a5,a6,a7)

/* ---- TRACE (sehr verbose, separat steuerbar) -------------------------- */
#undef APP_PRINT_TRACE0
#undef APP_PRINT_TRACE1
#undef APP_PRINT_TRACE2
#undef APP_PRINT_TRACE3
#undef APP_PRINT_TRACE4
#undef APP_PRINT_TRACE5

#if UART_LOG_TRACE_EN
#define APP_PRINT_TRACE0(fmt)                           uart_printf("[T] " fmt "\n")
#define APP_PRINT_TRACE1(fmt,a0)                        uart_printf("[T] " fmt "\n",a0)
#define APP_PRINT_TRACE2(fmt,a0,a1)                     uart_printf("[T] " fmt "\n",a0,a1)
#define APP_PRINT_TRACE3(fmt,a0,a1,a2)                  uart_printf("[T] " fmt "\n",a0,a1,a2)
#define APP_PRINT_TRACE4(fmt,a0,a1,a2,a3)               uart_printf("[T] " fmt "\n",a0,a1,a2,a3)
#define APP_PRINT_TRACE5(fmt,a0,a1,a2,a3,a4)            uart_printf("[T] " fmt "\n",a0,a1,a2,a3,a4)
#else
#define APP_PRINT_TRACE0(fmt)
#define APP_PRINT_TRACE1(fmt,a0)
#define APP_PRINT_TRACE2(fmt,a0,a1)
#define APP_PRINT_TRACE3(fmt,a0,a1,a2)
#define APP_PRINT_TRACE4(fmt,a0,a1,a2,a3)
#define APP_PRINT_TRACE5(fmt,a0,a1,a2,a3,a4)
#endif

#endif /* UART_LOG_EN */
#endif /* _LOG_UART_H_ */
