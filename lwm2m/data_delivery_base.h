/*
 * data_delivery_base.h
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef LWM2M_DATA_DELIVERY_BASE_H_
#define LWM2M_DATA_DELIVERY_BASE_H_

#include <stdint.h>
#include "lwm2m_object_base.h"

typedef void (*delivery_handler_func_t)(lwm2m_object_link_t delivery_object, lwm2m_object_link_t schedule, void *user_data);

typedef struct data_delivery_handlers_t
{
  lwm2m_object_link_t schedule;
  lwm2m_object_link_t delivery_object;
  delivery_handler_func_t handler;
  void *user_data;
}
data_delivery_handlers_t;

//
// Adds a handler for the specified scheduler events
void data_delivery_base_add_handler(lwm2m_object_link_t delivery_object, lwm2m_object_link_t schedule, delivery_handler_func_t handler, void *user_data);

//
// Removes all handlers for the specified delivery object instance
void data_delivery_base_remove_handlers(lwm2m_object_link_t delivery_object);

//
// This function searches all registered delivery objects and triggers
// the deliver callback on any that are registered against the supplied
// schedule object and instance.
void data_delivery_base_process_schedule(lwm2m_object_link_t schedule);

#endif /* LWM2M_DATA_DELIVERY_BASE_H_ */
