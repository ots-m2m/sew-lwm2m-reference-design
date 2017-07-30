/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "unit_tests.h"
#include "log.h"
#include "sys_tick_tests.h"
#include "rtc_tests.h"
#include "ring_buffer_tests.h"
#include "event_and_alarm_base_tests.h"
#include "delivery_schedule_tests.h"
#include "utc_offset_parser_tests.h"
#include "parse_lwm2m_exe_arg_tests.h"

void unit_tests_run(void)
{
  ring_buffer_tests_run();
  sys_tick_tests_run();
  rtc_tests_run();
  event_and_alarm_tests_run();
  delivery_schedule_tests_run();
  utc_offset_parse_tests_run();
  parse_lwm2m_exe_arg_test_run();
}

void unity_putchar(int c)
{
  log_write_console_char((unsigned char)c);
  if (c == '\n')
  {
    log_write_console_char('\r');
  }
}
