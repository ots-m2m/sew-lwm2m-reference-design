/*
 * Test_Water_Meter.c
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 */
#include "Test_Water_Meter.h"

#include "Water_Meter_High_Pressure_Alarm.h"


void Test_Water_Meter_object_initialise(void)
{
  Test_Water_Meter_initialise();
  Test_Water_Meter_create_instance(0);
  Test_Water_Meter_create_instance(1);
  Test_Water_Meter_create_instance(2);
  Test_Water_Meter_create_instance(3);
}


// This function is called when the water pressure has changed
// value from the server. It updates the value that is used by the water meter
// high pressure alarm to test if an alarm has occurred.
uint8_t Test_Water_Meter_Water_Pressure_changed(uint16_t instance_id)
{
  double val;
  Test_Water_Meter_Water_Pressure_get(instance_id,  &val);
  Water_Meter_High_Pressure_Alarm_set_value(instance_id, val);
  return 0;
}
