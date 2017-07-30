/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
#include "rtc_tests.h"

#include "unity.h"
#include "sys_tick.h"
#include <sys/time.h>

void test_set_get_time(void)
{
  struct timeval tv;
  tv.tv_sec = 0;
  tv.tv_usec = 0;
  TEST_ASSERT_EQUAL_INT(-1, settimeofday(&tv, 0));
  tv.tv_sec = 100000;
  TEST_ASSERT_EQUAL_INT(0, settimeofday(&tv, 0));

  //wait a few seconds to ensure the time is ticking
  msleep(3000);
  gettimeofday(&tv, 0);
  TEST_ASSERT_EQUAL_INT(100003, tv.tv_sec);
}

void rtc_tests_run(void)
{
  RUN_TEST(test_set_get_time);
}
