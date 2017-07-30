/*
 * Device.c
 * 
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 */
#include "Device.h"
#include <sys/time.h>
#include "interval_data_base.h"
#include "event_and_alarm_base.h"
#include "Delivery_Schedule.h"

uint8_t Device_Current_Time_changed(uint16_t instance)
{
  Device_lock();

  //NOTE: Production devices should use NTP to synchronse their time, this
  //is NOT a reliable way to get an accurate time sync.
  time_t time;
  Device_Current_Time_get(instance,  &time);

  struct timeval old_tv;
  gettimeofday(&old_tv, 0);

  struct timeval new_tv;
  new_tv.tv_sec = time;
  settimeofday(&new_tv, 0);

  int delta = old_tv.tv_sec - new_tv.tv_sec;
  //check if the time shift is big enough to effect the recording
  if ((delta > 5) || (delta < -5))
  {
    interval_data_base_time_changed();
    event_and_alarm_base_time_changed();
    Delivery_Schedule_time_changed();
  }

  Device_unlock();
  return COAP_NO_ERROR;
}
