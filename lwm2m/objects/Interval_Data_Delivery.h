/*
 * Interval_Data_Delivery.h
 * 
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * This code was generated using gt-object-gen provided by Operational 
 * Technology Solutions M2M Pty. Ltd. For licensing enquires visit 
 * ot-solutions.com.au.
 *
 */

#ifndef LWM2M_Interval_Data_Delivery_OBJECT_H_
#define LWM2M_Interval_Data_Delivery_OBJECT_H_

#include "Interval_Data_Delivery_base.h"


// This defines the structure of an object's instance internal variable
typedef struct Interval_Data_Delivery_instance_variables_t
{
  int16_t instance_id;
}
Interval_Data_Delivery_instance_variables_t;

/******************************************************************************/

void Interval_Data_Delivery_object_initialise(void);
void Interval_Data_Delivery_register(lwm2m_context_t *context);

//FIXME: this should be declared by the code generator in the
//_base.h file.
uint16_t Interval_Data_Delivery_get_index_for_instance(uint16_t instance);




#endif
