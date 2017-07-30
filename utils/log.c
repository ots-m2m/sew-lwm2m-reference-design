/*
 * log.c
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 */
#include "log.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include "sys_tick.h"
#include "uart.h"
#include <assert.h>
#include <stdio.h>

#define BAUD_RATE 115200
#define LOG_UART UCA0

static uint8_t recv_buffer[8];

static uart_config_t log_uart = {
  .baud = BAUD_RATE,
  .recv_buffer = { .s_elem = 1, .n_elem = sizeof(recv_buffer), .buffer = recv_buffer }
};

void log_initialise()
{
  assert(uart_init(LOG_UART, &log_uart) != -1);
}
int log_write_ccs(const char *str)
{
  while (*str != '\0')
  {
    putchar(*str);
    str++;
  }
  return 0;
}

int log_getchar(char *c, uint32_t timeout_ms)
{
  uint32_t to_count = timeout_ms;
  int res;
  while ((res=uart_getchar(LOG_UART, (uint8_t *) c) != 0) && (timeout_ms > 0) && (--to_count > 0))
  {
    msleep(1);
  }
  return res;
}

int log_write_console(const char *str)
{
  return uart_puts(LOG_UART, str);
}

int log_write_console_char(const char c)
{
  return uart_putchar(LOG_UART, c);
}

static char log_buf[128];

void log_write_entry(bool mirror_to_debugger, const char *tag, const char *format, ...)
{
  int result = 0;
  *log_buf = 0;
  int pos = snprintf(log_buf, sizeof(log_buf), "%08ld.%03ld", sys_tick_get_s(), sys_tick_get_ms());
  strcat(log_buf, tag);
  pos += strlen(tag);

  va_list list;
  va_start(list, format);
  result = vsnprintf(&log_buf[pos], sizeof(log_buf) - pos - 1, format, list);
  log_buf[sizeof(log_buf) - 1] = 0;
  va_end(list);
  if (result < 0)
  {
    return;
  }

  if (mirror_to_debugger)
  {
     log_write_ccs(log_buf);
     log_write_ccs("\r\n");
  }
  log_write_console(log_buf);
  log_write_console("\r\n");
}

void log_write(const char *format, ...)
{
  int result = 0;
  *log_buf = 0;

  va_list list;
  va_start(list, format);
  result = vsnprintf(log_buf, sizeof(log_buf), format, list);
  log_buf[sizeof(log_buf) - 1] = 0;
  va_end(list);
  if (result < 0)
  {
    return;
  }
  log_write_console(log_buf);
}

//
// make libc printf go to the uart
int fputc(int c, register FILE *_fp)
{
  return uart_putchar(LOG_UART, c);
}

int fputs(const char *str, register FILE *_fp)
{
  return uart_puts(LOG_UART, str);
}
