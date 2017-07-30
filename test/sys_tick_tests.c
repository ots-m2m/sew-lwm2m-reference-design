/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
#include "sys_tick_tests.h"

#include "unity.h"
#include "log.h"
#include "sys_tick.h"

void test_sys_tick_increments(void)
{
  uint32_t start_time_ms = sys_tick_get_ms();
  uint32_t start_time_s = sys_tick_get_s();

  for (volatile long i = 0; i < 1000000; i++) { }
  TEST_ASSERT_TRUE(start_time_ms != sys_tick_get_ms());
  TEST_ASSERT_TRUE(start_time_s != sys_tick_get_s());
}

void test_measure_systick(void)
{
  //use manual timing to test that sys_tick is working correctly, at some point
  //we should toggle a pin and measure the time using an oscilloscope.

  log_info("timing start (waiting 5 seconds)");
  volatile uint32_t start_time = sys_tick_get_s();
  volatile uint32_t now = start_time;
  while ((start_time + 5) > now)
  {
    now = sys_tick_get_s();
  }
  log_info("timing end (should have taken 5 seconds)");
  TEST_ASSERT_EQUAL_INT(5, now - start_time);
}

void test_msleep(void)
{
  log_info("timing start (waiting 5 seconds)");
  uint32_t start_time = sys_tick_get_s();
  msleep(5000);
  log_info("timing end (should have taken 5 seconds)");
  TEST_ASSERT_EQUAL_INT(5, sys_tick_get_s() - start_time);
}

void sys_tick_tests_run(void)
{
  RUN_TEST(test_sys_tick_increments);
  RUN_TEST(test_measure_systick);
  RUN_TEST(test_msleep);
}
