#ifndef _RETARGET_H_
#define _RETARGET_H_

#include <stdint.h>

void        uart_print_init(void);
void        uart_puts(const char *s);
int         uart_printf(const char *fmt, ...);
const char *uart_bdaddr_str(const uint8_t *addr);

/* Redirect printf() to uart_printf() when UART_LOG_EN is enabled.
 * uart_printf() uses vsnprintf internally — no newlib FILE, no lock,
 * safe to call from any RTOS task at any time. */
#if UART_LOG_EN
  #define printf uart_printf
#endif

#endif
