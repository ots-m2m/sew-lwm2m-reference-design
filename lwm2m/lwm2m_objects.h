/*
 * lwm2m_objects.h
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef LWM2M_LWM2M_OBJECTS_H_
#define LWM2M_LWM2M_OBJECTS_H_

#include "liblwm2m.h"
#include <stdint.h>

void lwm2m_objects_initialise(void);
uint16_t lwm2m_objects_register(lwm2m_context_t *context, lwm2m_object_t *objects[], uint16_t max_objects);

#endif /* LWM2M_LWM2M_OBJECTS_H_ */
