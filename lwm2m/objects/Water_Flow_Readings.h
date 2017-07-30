/*
 * Water_Flow_Readings.h
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 */

#ifndef LWM2M_Water_Flow_Readings_OBJECT_H_
#define LWM2M_Water_Flow_Readings_OBJECT_H_

#include "Water_Flow_Readings_base.h"

void Water_Flow_Readings_object_initialise(void);
void Water_Flow_Readings_set_value(double sensor_value);
void Water_Flow_Readings_register(lwm2m_context_t *context);

#endif
