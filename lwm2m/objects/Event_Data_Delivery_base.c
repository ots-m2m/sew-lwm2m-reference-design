/*
 * Event_Data_Delivery_base.c
 * 
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * This code was generated using gt-object-gen provided by Operational 
 * Technology Solutions M2M Pty. Ltd. For licensing enquires visit 
 * ot-solutions.com.au.
 */
#include "Event_Data_Delivery_base.h"

#include <string.h>
#include <stdbool.h>

#include "mutex.h"
#define assert(condition) if (!(condition)) { Event_Data_Delivery_unlock(); return COAP_406_NOT_ACCEPTABLE; }
#define assert_void(condition) if (!(condition)) { Event_Data_Delivery_unlock(); return; }

uint8_t __Event_Data_Delivery_create_instance(uint16_t instance_id);
#if CONFIG_Event_Data_Delivery_Name_OPTION == 1
static uint8_t __Event_Data_Delivery_Name_get(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data);
  
#endif
#if CONFIG_Event_Data_Delivery_Name_OPTION == 1
static uint8_t __Event_Data_Delivery_Name_set(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data);
#endif
#if CONFIG_Event_Data_Delivery_Event_Data_Links_OPTION == 1
static uint8_t __Event_Data_Delivery_Event_Data_Links_get(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data);
  
#endif
#if CONFIG_Event_Data_Delivery_Event_Data_Links_OPTION == 1
static uint8_t __Event_Data_Delivery_Event_Data_Links_set(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data);
#endif
#if CONFIG_Event_Data_Delivery_Latest_Eventlog_OPTION == 1
static uint8_t __Event_Data_Delivery_Latest_Eventlog_get(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data);
  
#endif
#if CONFIG_Event_Data_Delivery_Schedule_OPTION == 1
static uint8_t __Event_Data_Delivery_Schedule_get(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data);
  
#endif
#if CONFIG_Event_Data_Delivery_Schedule_OPTION == 1
static uint8_t __Event_Data_Delivery_Schedule_set(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data);
#endif

static lwm2m_object_t Event_Data_Delivery_lwm2m_object = {
  .objID = 8003
};

lwm2m_object_t *Event_Data_Delivery_get_lwm2m_object(void)
{
  return &Event_Data_Delivery_lwm2m_object;
}

typedef struct Event_Data_Delivery_instance_t
{
  struct Event_Data_Delivery_instance * next;   // matches lwm2m_list_t::next
  uint16_t id;                                // matches lwm2m_list_t::id
  uint16_t index;
  bool registered;
  void *user_data;
#if CONFIG_Event_Data_Delivery_Name_OPTION == 1
  char Name [MAX_Event_Data_Delivery_Name];
#endif
#if CONFIG_Event_Data_Delivery_Event_Data_Links_OPTION == 1
  bool Event_Data_Links_is_set[MAX_MULTIPLE_RESOURCES];
  lwm2m_object_link_t Event_Data_Links[MAX_MULTIPLE_RESOURCES];
#endif
#if CONFIG_Event_Data_Delivery_Latest_Eventlog_OPTION == 1
  bool Latest_Eventlog_is_set[MAX_MULTIPLE_RESOURCES];
  uint8_t *Latest_Eventlog[MAX_MULTIPLE_RESOURCES];
  uint16_t Latest_Eventlog_length[MAX_MULTIPLE_RESOURCES];
#endif
#if CONFIG_Event_Data_Delivery_Schedule_OPTION == 1
  lwm2m_object_link_t Schedule;
#endif
}
Event_Data_Delivery_instance;

static Event_Data_Delivery_instance Event_Data_Delivery_instances[MAX_Event_Data_Delivery_INSTANCES];

lwm2m_object_declaration_t Event_Data_Delivery_object_declaration = {
  .id = 8003,
  .delete = Event_Data_Delivery_delete_instance,
  .create = __Event_Data_Delivery_create_instance,
#if CONFIG_Event_Data_Delivery_Name_OPTION == 1
  .resources[0] = { 
    .supported = true,  
    .id = 0,
    .operation = LWM2M_OPERATION_RW,
    .instances = LWM2M_INSTANCETYPE_single,
    .mandatory = false,
    .type = LWM2M_TYPE_string,
    .changed = Event_Data_Delivery_Name_changed,
    .execute = (void *) 0,
    .get = __Event_Data_Delivery_Name_get,
    .set = __Event_Data_Delivery_Name_set,
  },
#else
  .resources[0] = { .supported = false },
#endif
#if CONFIG_Event_Data_Delivery_Event_Data_Links_OPTION == 1
  .resources[1] = { 
    .supported = true,  
    .id = 1,
    .operation = LWM2M_OPERATION_RW,
    .instances = LWM2M_INSTANCETYPE_multiple,
    .mandatory = false,
    .type = LWM2M_TYPE_objlnk,
    .changed = Event_Data_Delivery_Event_Data_Links_changed,
    .execute = (void *) 0,
    .is_set = Event_Data_Delivery_Event_Data_Links_is_set,
    .get = __Event_Data_Delivery_Event_Data_Links_get,
    .set = __Event_Data_Delivery_Event_Data_Links_set,
  },
#else
  .resources[1] = { .supported = false },
#endif
#if CONFIG_Event_Data_Delivery_Latest_Eventlog_OPTION == 1
  .resources[2] = { 
    .supported = true,  
    .id = 2,
    .operation = LWM2M_OPERATION_R,
    .instances = LWM2M_INSTANCETYPE_multiple,
    .mandatory = false,
    .type = LWM2M_TYPE_opaque,
    .changed = (void *) 0,
    .execute = (void *) 0,
    .is_set = Event_Data_Delivery_Latest_Eventlog_is_set,
    .get = __Event_Data_Delivery_Latest_Eventlog_get,
    .set = (void *) 0,
  },
#else
  .resources[2] = { .supported = false },
#endif
#if CONFIG_Event_Data_Delivery_Schedule_OPTION == 1
  .resources[3] = { 
    .supported = true,  
    .id = 3,
    .operation = LWM2M_OPERATION_RW,
    .instances = LWM2M_INSTANCETYPE_single,
    .mandatory = false,
    .type = LWM2M_TYPE_objlnk,
    .changed = Event_Data_Delivery_Schedule_changed,
    .execute = (void *) 0,
    .get = __Event_Data_Delivery_Schedule_get,
    .set = __Event_Data_Delivery_Schedule_set,
  },
#else
  .resources[3] = { .supported = false },
#endif
};


void Event_Data_Delivery_lock() 
{ 
  mutex_lock(&Event_Data_Delivery_object_declaration.lock, MUTEXT_WAIT_FOREVER); 
}
void Event_Data_Delivery_unlock() 
{ 
  mutex_unlock(&Event_Data_Delivery_object_declaration.lock);
}


uint16_t Event_Data_Delivery_get_index_for_instance(uint16_t instance)
{
  for (int i = 0; i < MAX_Event_Data_Delivery_INSTANCES; i++)
  {
    if (Event_Data_Delivery_instances[i].id == instance) return i;
  }
  return LWM2M_INVALID_INSTANCE;
}

__attribute__((weak)) lwm2m_object_t *Event_Data_Delivery_initialise(void)
{
  mutex_initialise(&Event_Data_Delivery_object_declaration.lock);
  
  memset(Event_Data_Delivery_instances, 0, sizeof(Event_Data_Delivery_instances));
  for (int i = 0; i < MAX_Event_Data_Delivery_INSTANCES; i++)
  {
    Event_Data_Delivery_instances[i].id = LWM2M_INVALID_INSTANCE;
    Event_Data_Delivery_instances[i].index = i;
  }
  lwm2m_object_base_initialse_object(&Event_Data_Delivery_lwm2m_object);
  Event_Data_Delivery_lwm2m_object.userData = &Event_Data_Delivery_object_declaration; 
  return &Event_Data_Delivery_lwm2m_object;
}
  
__attribute__((weak)) void Event_Data_Delivery_register_object(lwm2m_context_t *context)
{
  Event_Data_Delivery_lock();
  Event_Data_Delivery_object_declaration.context = context;
  Event_Data_Delivery_unlock();
}
  
__attribute__((weak)) void Event_Data_Delivery_register_instance(uint16_t instance_id)
{
  Event_Data_Delivery_lock();
  uint16_t index = Event_Data_Delivery_get_index_for_instance(instance_id);
  assert_void(index != LWM2M_INVALID_INSTANCE);
  
  if ((Event_Data_Delivery_instances[index].id !=  LWM2M_INVALID_INSTANCE) && (!Event_Data_Delivery_instances[index].registered))
  {
    Event_Data_Delivery_lwm2m_object.instanceList = LWM2M_LIST_ADD(Event_Data_Delivery_lwm2m_object.instanceList, (lwm2m_list_t *)&Event_Data_Delivery_instances[index]);
    Event_Data_Delivery_instances[index].registered = true;
  }
  Event_Data_Delivery_unlock();
}

__attribute__((weak)) void Event_Data_Delivery_unregister_instance(uint16_t instance_id)
{
  Event_Data_Delivery_lock();
  uint16_t index = Event_Data_Delivery_get_index_for_instance(instance_id);
  assert_void(index != LWM2M_INVALID_INSTANCE);
  
  if ((Event_Data_Delivery_instances[index].id !=  LWM2M_INVALID_INSTANCE) && (Event_Data_Delivery_instances[index].registered))
  {
    Event_Data_Delivery_instance *instance = 0;
    Event_Data_Delivery_lwm2m_object.instanceList = lwm2m_list_remove(Event_Data_Delivery_lwm2m_object.instanceList, instance_id, (lwm2m_list_t **)&instance);
    Event_Data_Delivery_instances[index].registered = false;
    if (instance != 0)
    { 
      instance->next = 0;
    }
  }
  Event_Data_Delivery_unlock();
}

//override this to perform actions when a new instance is created
__attribute__((weak)) void Event_Data_Delivery_instance_created(uint16_t instance_id)
{
  
}
  

//called when the server requests an object to be created; we need to create AND register the 
//instance
uint8_t __Event_Data_Delivery_create_instance(uint16_t instance_id)
{
  uint8_t res = Event_Data_Delivery_create_instance(instance_id);
  if (res == COAP_201_CREATED)
  {
    Event_Data_Delivery_register_instance(instance_id);
  }
  return res;
}
  
__attribute__((weak)) uint8_t Event_Data_Delivery_create_instance(uint16_t instance_id)
{
  Event_Data_Delivery_lock();
  //find an unallocated instance
  for (int i = 0; i < MAX_Event_Data_Delivery_INSTANCES; i++)
  {
    if (Event_Data_Delivery_instances[i].id == LWM2M_INVALID_INSTANCE)
    {
      Event_Data_Delivery_instances[i].id = instance_id;
      Event_Data_Delivery_instance_created(instance_id);
      Event_Data_Delivery_unlock();
      return COAP_201_CREATED;
    }
  }
  Event_Data_Delivery_unlock();
  return COAP_500_INTERNAL_SERVER_ERROR; //none found
}

void Event_Data_Delivery_foreach(lwm2m_each_instance_func func, void *data)
{
  Event_Data_Delivery_lock();
  
  for (int i = 0; i < MAX_Event_Data_Delivery_INSTANCES; i++)
  {
    if (Event_Data_Delivery_instances[i].id != LWM2M_INVALID_INSTANCE)
    {
      if (!func(Event_Data_Delivery_instances[i].id, data)) break;
    }
  }
  Event_Data_Delivery_unlock();
}
  
//provides access to a user data pointer for storing additional instance
//data.
void *Event_Data_Delivery_get_user_data(uint16_t instance_id)
{
  uint16_t index = Event_Data_Delivery_get_index_for_instance(instance_id);
  if (index == LWM2M_INVALID_INSTANCE) return 0;
  return Event_Data_Delivery_instances[index].user_data;
}
  
bool Event_Data_Delivery_set_user_data(uint16_t instance_id, void *data)
{
  uint16_t index = Event_Data_Delivery_get_index_for_instance(instance_id);
  if (index == LWM2M_INVALID_INSTANCE) return false;
  Event_Data_Delivery_instances[index].user_data = data;
  return true;
}
  
//override this to perform actions when a new instance is created
__attribute__((weak)) void Event_Data_Delivery_instance_deleted(uint16_t instance_id)
{
  
}

__attribute__((weak)) uint8_t Event_Data_Delivery_delete_instance(uint16_t instance_id)
{
  Event_Data_Delivery_lock();
  uint16_t index = Event_Data_Delivery_get_index_for_instance(instance_id);
  assert(index != LWM2M_INVALID_INSTANCE);
  
  if ((instance_id < MAX_Event_Data_Delivery_INSTANCES) && (Event_Data_Delivery_instances[index].id !=  LWM2M_INVALID_INSTANCE))
  {
    Event_Data_Delivery_unregister_instance(instance_id);
    Event_Data_Delivery_instance_deleted(instance_id);
    Event_Data_Delivery_instances[index].id = LWM2M_INVALID_INSTANCE;
  }
  Event_Data_Delivery_unlock();
  return COAP_202_DELETED;
}


__attribute__((weak)) void Event_Data_Delivery_changed(uint16_t instance)
{
  
}

#if CONFIG_Event_Data_Delivery_Name_OPTION == 1
  
__attribute__((weak))  uint8_t Event_Data_Delivery_Name_get(uint16_t instance,  const char **val)
{
  Event_Data_Delivery_lock();
  assert(instance >= 0 && instance < MAX_Event_Data_Delivery_INSTANCES);
  uint16_t index = Event_Data_Delivery_get_index_for_instance(instance);
  assert(index != LWM2M_INVALID_INSTANCE);
  
  assert(Event_Data_Delivery_instances[index].id != LWM2M_INVALID_INSTANCE);
  
  
  *val = Event_Data_Delivery_instances[index].Name;
  Event_Data_Delivery_unlock();
  return COAP_205_CONTENT;
}
static uint8_t __Event_Data_Delivery_Name_get(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data)
{
  const char *val;
  uint8_t res = Event_Data_Delivery_Name_get(instance,  &val);
  lwm2m_data_encode_string(val, data);
  return res;
}
                                         
#endif
#if CONFIG_Event_Data_Delivery_Name_OPTION == 1
__attribute__((weak)) uint8_t Event_Data_Delivery_Name_set(uint16_t instance,  const char *val, size_t length, bool notify)
{
  Event_Data_Delivery_lock();
  assert(instance >= 0 && instance < MAX_Event_Data_Delivery_INSTANCES);
  uint16_t index = Event_Data_Delivery_get_index_for_instance(instance);
  assert(index != LWM2M_INVALID_INSTANCE);
  
  assert(Event_Data_Delivery_instances[index].id != LWM2M_INVALID_INSTANCE);
 
  if (length >= sizeof(Event_Data_Delivery_instances[index].Name )) length = sizeof(Event_Data_Delivery_instances[index].Name) - 1;
  memcpy(Event_Data_Delivery_instances[index].Name, val, length);
  Event_Data_Delivery_instances[index].Name[length] = 0;
  
  
  if (notify && Event_Data_Delivery_instances[index].registered) lwm2m_object_base_resource_changed(&Event_Data_Delivery_lwm2m_object, instance, 0, false);
  Event_Data_Delivery_unlock();
  return COAP_204_CHANGED;
}
static uint8_t __Event_Data_Delivery_Name_set(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data)
{
  return Event_Data_Delivery_Name_set(instance,  (char*)data->value.asBuffer.buffer, data->value.asBuffer.length, false);
}
#endif
#if CONFIG_Event_Data_Delivery_Name_OPTION == 1
__attribute__((weak)) uint8_t Event_Data_Delivery_Name_changed(uint16_t instance)
{
  Event_Data_Delivery_lock();
  assert(instance >= 0 && instance < MAX_Event_Data_Delivery_INSTANCES);
  uint16_t index = Event_Data_Delivery_get_index_for_instance(instance);
  assert(index != LWM2M_INVALID_INSTANCE);
  
  Event_Data_Delivery_changed(instance);
  Event_Data_Delivery_unlock();
  return COAP_NO_ERROR;
}
#endif
#if CONFIG_Event_Data_Delivery_Event_Data_Links_OPTION == 1
bool Event_Data_Delivery_Event_Data_Links_is_set(uint16_t instance_id, uint16_t resource_instance)
{
  uint16_t index = Event_Data_Delivery_get_index_for_instance(instance_id);
  if ((index == LWM2M_INVALID_INSTANCE) || (resource_instance >= MAX_MULTIPLE_RESOURCES)) return false;
  return Event_Data_Delivery_instances[index].Event_Data_Links_is_set[resource_instance];
}
  
__attribute__((weak)) uint8_t Event_Data_Delivery_Event_Data_Links_get(uint16_t instance, uint16_t resource_instance, lwm2m_object_link_t *val)
{
  Event_Data_Delivery_lock();
  assert(instance >= 0 && instance < MAX_Event_Data_Delivery_INSTANCES);
  uint16_t index = Event_Data_Delivery_get_index_for_instance(instance);
  assert(index != LWM2M_INVALID_INSTANCE);
  assert(resource_instance < MAX_MULTIPLE_RESOURCES);
  assert(Event_Data_Delivery_instances[index].id != LWM2M_INVALID_INSTANCE);
  assert(Event_Data_Delivery_instances[index].Event_Data_Links_is_set[resource_instance]);
  
  memcpy(val, &Event_Data_Delivery_instances[index].Event_Data_Links[resource_instance], sizeof(lwm2m_object_link_t));

  Event_Data_Delivery_unlock();
  return COAP_205_CONTENT;
}
static uint8_t __Event_Data_Delivery_Event_Data_Links_get(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data)
{
  lwm2m_object_link_t val;
  uint8_t res = Event_Data_Delivery_Event_Data_Links_get(instance, resource_instance, &val);
  lwm2m_data_encode_objlink(val.object_id, val.instance_id, data);
  return res;
}
                                         
#endif
#if CONFIG_Event_Data_Delivery_Event_Data_Links_OPTION == 1
__attribute__((weak)) uint8_t Event_Data_Delivery_Event_Data_Links_set(uint16_t instance, uint16_t resource_instance, lwm2m_object_link_t val, bool notify)
{
  Event_Data_Delivery_lock();
  assert(instance >= 0 && instance < MAX_Event_Data_Delivery_INSTANCES);
  uint16_t index = Event_Data_Delivery_get_index_for_instance(instance);
  assert(index != LWM2M_INVALID_INSTANCE);
  assert(resource_instance < MAX_MULTIPLE_RESOURCES);
  assert(Event_Data_Delivery_instances[index].id != LWM2M_INVALID_INSTANCE);

  Event_Data_Delivery_instances[index].Event_Data_Links[resource_instance].object_id = val.object_id;
  Event_Data_Delivery_instances[index].Event_Data_Links[resource_instance].instance_id = val.instance_id;
  Event_Data_Delivery_instances[index].Event_Data_Links_is_set[resource_instance] = true;   
  
  if (notify && Event_Data_Delivery_instances[index].registered) lwm2m_object_base_resource_changed(&Event_Data_Delivery_lwm2m_object, instance, 1, false);  
  Event_Data_Delivery_unlock();
  return COAP_204_CHANGED;
}
static uint8_t __Event_Data_Delivery_Event_Data_Links_set(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data)
{
  lwm2m_object_link_t val;
  if (lwm2m_data_decode_objlink(data, &val.object_id, &val.instance_id) != 0)
  {
    return Event_Data_Delivery_Event_Data_Links_set(instance,resource_instance, val, false);
  }
  return COAP_406_NOT_ACCEPTABLE;
}
#endif
#if CONFIG_Event_Data_Delivery_Event_Data_Links_OPTION == 1
__attribute__((weak)) uint8_t Event_Data_Delivery_Event_Data_Links_changed(uint16_t instance)
{
  Event_Data_Delivery_lock();
  assert(instance >= 0 && instance < MAX_Event_Data_Delivery_INSTANCES);
  uint16_t index = Event_Data_Delivery_get_index_for_instance(instance);
  assert(index != LWM2M_INVALID_INSTANCE);
  
  Event_Data_Delivery_changed(instance);
  Event_Data_Delivery_unlock();
  return COAP_NO_ERROR;
}
#endif
#if CONFIG_Event_Data_Delivery_Latest_Eventlog_OPTION == 1
bool Event_Data_Delivery_Latest_Eventlog_is_set(uint16_t instance_id, uint16_t resource_instance)
{
  uint16_t index = Event_Data_Delivery_get_index_for_instance(instance_id);
  if ((index == LWM2M_INVALID_INSTANCE) || (resource_instance >= MAX_MULTIPLE_RESOURCES)) return false;
  return Event_Data_Delivery_instances[index].Latest_Eventlog_is_set[resource_instance];
}
  
__attribute__((weak))  uint8_t Event_Data_Delivery_Latest_Eventlog_get(uint16_t instance, uint16_t resource_instance, uint8_t **val, size_t *length)
{
  Event_Data_Delivery_lock();
  assert(instance >= 0 && instance < MAX_Event_Data_Delivery_INSTANCES);
  uint16_t index = Event_Data_Delivery_get_index_for_instance(instance);
  assert(index != LWM2M_INVALID_INSTANCE);
  assert(resource_instance < MAX_MULTIPLE_RESOURCES);
  assert(Event_Data_Delivery_instances[index].id != LWM2M_INVALID_INSTANCE);
  
  assert(Event_Data_Delivery_instances[index].Latest_Eventlog_is_set[resource_instance]);
  
  *val = Event_Data_Delivery_instances[index].Latest_Eventlog[resource_instance];
  *length = Event_Data_Delivery_instances[index].Latest_Eventlog_length[resource_instance];
  Event_Data_Delivery_unlock();
  return COAP_205_CONTENT;
}
static uint8_t __Event_Data_Delivery_Latest_Eventlog_get(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data)
{
  uint8_t *val;
  size_t length = 0;
  
  uint8_t res = Event_Data_Delivery_Latest_Eventlog_get(instance, resource_instance, &val, &length);
  
  lwm2m_data_encode_opaque(val, length, data);
  return res;
}
                                         
#endif
#if CONFIG_Event_Data_Delivery_Latest_Eventlog_OPTION == 1
 __attribute__((weak)) uint8_t Event_Data_Delivery_Latest_Eventlog_set(uint16_t instance, uint16_t resource_instance, uint8_t* val, size_t length, bool notify)
 {
   Event_Data_Delivery_lock();
   assert(instance >= 0 && instance < MAX_Event_Data_Delivery_INSTANCES);
 
   uint16_t index = Event_Data_Delivery_get_index_for_instance(instance);
   assert(index != LWM2M_INVALID_INSTANCE);
   assert(resource_instance < MAX_MULTIPLE_RESOURCES);
   if (Event_Data_Delivery_instances[index].Latest_Eventlog[resource_instance] != 0) lwm2m_free(Event_Data_Delivery_instances[index].Latest_Eventlog[resource_instance]);
   Event_Data_Delivery_instances[index].Latest_Eventlog[resource_instance] = lwm2m_malloc(length);
   if (Event_Data_Delivery_instances[index].Latest_Eventlog[resource_instance] == 0)
   {
     Event_Data_Delivery_instances[index].Latest_Eventlog_length[resource_instance]  = 0;   
     Event_Data_Delivery_unlock();
     return COAP_500_INTERNAL_SERVER_ERROR;                                     
   } 
   Event_Data_Delivery_instances[index].Latest_Eventlog_length[resource_instance]  = length;
   memcpy(Event_Data_Delivery_instances[index].Latest_Eventlog[resource_instance], val, length);                                      
                                  
   Event_Data_Delivery_instances[index].Latest_Eventlog_is_set[resource_instance] = true;
  
   if (notify && Event_Data_Delivery_instances[index].registered) lwm2m_object_base_resource_changed(&Event_Data_Delivery_lwm2m_object, instance, 2, false);
  
  
   Event_Data_Delivery_unlock();
   return COAP_204_CHANGED;
 }
#endif
#if CONFIG_Event_Data_Delivery_Schedule_OPTION == 1
  
__attribute__((weak)) uint8_t Event_Data_Delivery_Schedule_get(uint16_t instance,  lwm2m_object_link_t *val)
{
  Event_Data_Delivery_lock();
  assert(instance >= 0 && instance < MAX_Event_Data_Delivery_INSTANCES);
  uint16_t index = Event_Data_Delivery_get_index_for_instance(instance);
  assert(index != LWM2M_INVALID_INSTANCE);
  
  assert(Event_Data_Delivery_instances[index].id != LWM2M_INVALID_INSTANCE);
  
  
  memcpy(val, &Event_Data_Delivery_instances[index].Schedule, sizeof(lwm2m_object_link_t));

  Event_Data_Delivery_unlock();
  return COAP_205_CONTENT;
}
static uint8_t __Event_Data_Delivery_Schedule_get(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data)
{
  lwm2m_object_link_t val;
  uint8_t res = Event_Data_Delivery_Schedule_get(instance,  &val);
  lwm2m_data_encode_objlink(val.object_id, val.instance_id, data);
  return res;
}
                                         
#endif
#if CONFIG_Event_Data_Delivery_Schedule_OPTION == 1
__attribute__((weak)) uint8_t Event_Data_Delivery_Schedule_set(uint16_t instance,  lwm2m_object_link_t val, bool notify)
{
  Event_Data_Delivery_lock();
  assert(instance >= 0 && instance < MAX_Event_Data_Delivery_INSTANCES);
  uint16_t index = Event_Data_Delivery_get_index_for_instance(instance);
  assert(index != LWM2M_INVALID_INSTANCE);
  
  assert(Event_Data_Delivery_instances[index].id != LWM2M_INVALID_INSTANCE);

  Event_Data_Delivery_instances[index].Schedule.object_id = val.object_id;
  Event_Data_Delivery_instances[index].Schedule.instance_id = val.instance_id;
     
  
  if (notify && Event_Data_Delivery_instances[index].registered) lwm2m_object_base_resource_changed(&Event_Data_Delivery_lwm2m_object, instance, 3, false);  
  Event_Data_Delivery_unlock();
  return COAP_204_CHANGED;
}
static uint8_t __Event_Data_Delivery_Schedule_set(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data)
{
  lwm2m_object_link_t val;
  if (lwm2m_data_decode_objlink(data, &val.object_id, &val.instance_id) != 0)
  {
    return Event_Data_Delivery_Schedule_set(instance, val, false);
  }
  return COAP_406_NOT_ACCEPTABLE;
}
#endif
#if CONFIG_Event_Data_Delivery_Schedule_OPTION == 1
__attribute__((weak)) uint8_t Event_Data_Delivery_Schedule_changed(uint16_t instance)
{
  Event_Data_Delivery_lock();
  assert(instance >= 0 && instance < MAX_Event_Data_Delivery_INSTANCES);
  uint16_t index = Event_Data_Delivery_get_index_for_instance(instance);
  assert(index != LWM2M_INVALID_INSTANCE);
  
  Event_Data_Delivery_changed(instance);
  Event_Data_Delivery_unlock();
  return COAP_NO_ERROR;
}
#endif
