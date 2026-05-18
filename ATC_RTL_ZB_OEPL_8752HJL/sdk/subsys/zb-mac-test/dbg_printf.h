#ifndef __DBG_PRINTF_H__
#define __DBG_PRINTF_H__

#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>

#include "rtl876x.h"
#include "rtl876x_nvic.h"
#include "rtl876x_uart.h"
#include "rtl876x_gdma.h"
#include "rtl876x_rcc.h"

void dbg_init(void);
void dbg_putchar(uint8_t c);
void dbg_flush(void);
uint32_t dbg_flush_busy(void);

int dbg_printf(const char *format, ...);
int dbg_vsprintf(char *buffer, const char *fmt, va_list args);

#endif /* __DBG_PRINTF_H__ */
