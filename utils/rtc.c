/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "sys_tick.h"
#include <stdint.h>

#include <sys/time.h>
#include "log.h"
//
// Epoch is UNIX epoch, 1/1/1970 00:00
static uint32_t sys_tick_epoch_offset = 0;

int gettimeofday(struct timeval *tv, void *tz)
{
  tv->tv_sec = sys_tick_epoch_offset + sys_tick_get_s();
  tv->tv_usec = sys_tick_get_ms() * 1000;
  return 0;
}

int settimeofday(const struct timeval *tv, const struct timezone *tz)
{
  //time cannot set to be before epoch + "time the system has been running".
  //this should not be a problem in practice since the system never needs to
  //run in the past.
  if ((uint32_t)tv->tv_sec < sys_tick_get_s()) return -1;

  sys_tick_epoch_offset = tv->tv_sec - sys_tick_get_s();

  return 0;
}


