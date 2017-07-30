/*
 * Water_Meter_High_Pressure_Alarm.h
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 */

#ifndef LWM2M_Water_Meter_High_Pressure_Alarm_OBJECT_H_
#define LWM2M_Water_Meter_High_Pressure_Alarm_OBJECT_H_



#include "Water_Meter_High_Pressure_Alarm_base.h"

void Water_Meter_High_Pressure_Alarm_object_initialise(void);
void Water_Meter_High_Pressure_Alarm_set_value(uint16_t instance_id, double sensor_value);
void Water_Meter_High_Pressure_Alarm_register(lwm2m_context_t *context);

#endif
