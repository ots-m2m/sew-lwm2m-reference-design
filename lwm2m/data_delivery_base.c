/*
 * data_delivery_base.c
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
#include "lwm2m_object_options.h"


#include "data_delivery_base.h"
#include "log.h"

#ifndef DEBUG_DATA_DELIVERY_BASE
#undef log_debug
#define log_debug(...)
#endif

#ifndef DATA_DELIVERY_BASE_MAX_DELVIERY_HANDLERS
#define MAX_DELVIERY_HANDLERS DATA_DELIVERY_BASE_MAX_DELVIERY_HANDLERS
#else
#define MAX_DELVIERY_HANDLERS 16
#endif

static data_delivery_handlers_t data_delivery_handlers[MAX_DELVIERY_HANDLERS] = { 0 };

//
// Adds a handler for the specified scheduler events
void data_delivery_base_add_handler(lwm2m_object_link_t delivery_object, lwm2m_object_link_t schedule, delivery_handler_func_t handler, void *user_data)
{
  for (int i = 0; i < MAX_DELVIERY_HANDLERS; i++)
  {
    if (data_delivery_handlers[i].handler == 0)
    {
      data_delivery_handlers[i].handler = handler;
      data_delivery_handlers[i].user_data = user_data;
      data_delivery_handlers[i].schedule.instance_id = schedule.instance_id;
      data_delivery_handlers[i].schedule.object_id = schedule.object_id;
      data_delivery_handlers[i].delivery_object.instance_id = delivery_object.instance_id;
      data_delivery_handlers[i].delivery_object.object_id = delivery_object.object_id;
      return;
    }
  }
  log_err("too many delivery handlers are registered.");
}

//
// Removes the handler for the specified object instance
void data_delivery_base_remove_handlers(lwm2m_object_link_t delivery_object)
{
  for (int i = 0; i < MAX_DELVIERY_HANDLERS; i++)
  {
    if ((data_delivery_handlers[i].delivery_object.instance_id == delivery_object.instance_id) &&
       (data_delivery_handlers[i].delivery_object.object_id == delivery_object.object_id))
    {
      data_delivery_handlers[i].handler = 0;
      data_delivery_handlers[i].user_data = 0;
      data_delivery_handlers[i].delivery_object.instance_id = 0;
      data_delivery_handlers[i].delivery_object.object_id = 0;
      data_delivery_handlers[i].schedule.instance_id = 0;
      data_delivery_handlers[i].schedule.object_id = 0;
    }
  }
}


//
// This function searches all registered delivery objects and triggers
// the deliver callback on any that are registered against the supplied
// schedule object and instance.
void data_delivery_base_process_schedule(lwm2m_object_link_t schedule)
{
  for (int i = 0; i < MAX_DELVIERY_HANDLERS; i++)
  {
    if ((data_delivery_handlers[i].schedule.instance_id == schedule.instance_id) &&
       (data_delivery_handlers[i].schedule.object_id == schedule.object_id) &&
       (data_delivery_handlers[i].handler != 0))
    {
      data_delivery_handlers[i].handler(data_delivery_handlers[i].delivery_object, data_delivery_handlers[i].schedule, data_delivery_handlers[i].user_data);
    }
  }
}
