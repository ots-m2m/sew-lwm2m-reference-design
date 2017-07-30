/*
 * Delivery_Schedule_base.c
 * 
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * This code was generated using gt-object-gen provided by Operational 
 * Technology Solutions M2M Pty. Ltd. For licensing enquires visit 
 * ot-solutions.com.au.
 */
#include "Delivery_Schedule_base.h"

#include <string.h>
#include <stdbool.h>

#include "mutex.h"
#define assert(condition) if (!(condition)) { Delivery_Schedule_unlock(); return COAP_406_NOT_ACCEPTABLE; }
#define assert_void(condition) if (!(condition)) { Delivery_Schedule_unlock(); return; }

uint8_t __Delivery_Schedule_create_instance(uint16_t instance_id);
#if CONFIG_Delivery_Schedule_Schedule_Start_Time_OPTION == 1
static uint8_t __Delivery_Schedule_Schedule_Start_Time_get(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data);
  
#endif
#if CONFIG_Delivery_Schedule_Schedule_Start_Time_OPTION == 1
static uint8_t __Delivery_Schedule_Schedule_Start_Time_set(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data);
#endif
#if CONFIG_Delivery_Schedule_Schedule_UTC_Offset_OPTION == 1
static uint8_t __Delivery_Schedule_Schedule_UTC_Offset_get(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data);
  
#endif
#if CONFIG_Delivery_Schedule_Schedule_UTC_Offset_OPTION == 1
static uint8_t __Delivery_Schedule_Schedule_UTC_Offset_set(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data);
#endif
#if CONFIG_Delivery_Schedule_Delivery_Frequency_OPTION == 1
static uint8_t __Delivery_Schedule_Delivery_Frequency_get(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data);
  
#endif
#if CONFIG_Delivery_Schedule_Delivery_Frequency_OPTION == 1
static uint8_t __Delivery_Schedule_Delivery_Frequency_set(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data);
#endif
#if CONFIG_Delivery_Schedule_Randomised_Delivery_Window_OPTION == 1
static uint8_t __Delivery_Schedule_Randomised_Delivery_Window_get(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data);
  
#endif
#if CONFIG_Delivery_Schedule_Randomised_Delivery_Window_OPTION == 1
static uint8_t __Delivery_Schedule_Randomised_Delivery_Window_set(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data);
#endif
#if CONFIG_Delivery_Schedule_Number_of_Retries_OPTION == 1
static uint8_t __Delivery_Schedule_Number_of_Retries_get(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data);
  
#endif
#if CONFIG_Delivery_Schedule_Number_of_Retries_OPTION == 1
static uint8_t __Delivery_Schedule_Number_of_Retries_set(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data);
#endif
#if CONFIG_Delivery_Schedule_Retry_Period_OPTION == 1
static uint8_t __Delivery_Schedule_Retry_Period_get(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data);
  
#endif
#if CONFIG_Delivery_Schedule_Retry_Period_OPTION == 1
static uint8_t __Delivery_Schedule_Retry_Period_set(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data);
#endif

static lwm2m_object_t Delivery_Schedule_lwm2m_object = {
  .objID = 8004
};

lwm2m_object_t *Delivery_Schedule_get_lwm2m_object(void)
{
  return &Delivery_Schedule_lwm2m_object;
}

typedef struct Delivery_Schedule_instance_t
{
  struct Delivery_Schedule_instance * next;   // matches lwm2m_list_t::next
  uint16_t id;                                // matches lwm2m_list_t::id
  uint16_t index;
  bool registered;
  void *user_data;
#if CONFIG_Delivery_Schedule_Schedule_Start_Time_OPTION == 1
  uint32_t Schedule_Start_Time;
#endif
#if CONFIG_Delivery_Schedule_Schedule_UTC_Offset_OPTION == 1
  char Schedule_UTC_Offset [MAX_Delivery_Schedule_Schedule_UTC_Offset];
#endif
#if CONFIG_Delivery_Schedule_Delivery_Frequency_OPTION == 1
  uint32_t Delivery_Frequency;
#endif
#if CONFIG_Delivery_Schedule_Randomised_Delivery_Window_OPTION == 1
  uint32_t Randomised_Delivery_Window;
#endif
#if CONFIG_Delivery_Schedule_Number_of_Retries_OPTION == 1
  uint32_t Number_of_Retries;
#endif
#if CONFIG_Delivery_Schedule_Retry_Period_OPTION == 1
  uint32_t Retry_Period;
#endif
}
Delivery_Schedule_instance;

static Delivery_Schedule_instance Delivery_Schedule_instances[MAX_Delivery_Schedule_INSTANCES];

lwm2m_object_declaration_t Delivery_Schedule_object_declaration = {
  .id = 8004,
  .delete = Delivery_Schedule_delete_instance,
  .create = __Delivery_Schedule_create_instance,
#if CONFIG_Delivery_Schedule_Schedule_Start_Time_OPTION == 1
  .resources[0] = { 
    .supported = true,  
    .id = 0,
    .operation = LWM2M_OPERATION_RW,
    .instances = LWM2M_INSTANCETYPE_single,
    .mandatory = false,
    .type = LWM2M_TYPE_integer,
    .changed = Delivery_Schedule_Schedule_Start_Time_changed,
    .execute = (void *) 0,
    .get = __Delivery_Schedule_Schedule_Start_Time_get,
    .set = __Delivery_Schedule_Schedule_Start_Time_set,
  },
#else
  .resources[0] = { .supported = false },
#endif
#if CONFIG_Delivery_Schedule_Schedule_UTC_Offset_OPTION == 1
  .resources[1] = { 
    .supported = true,  
    .id = 1,
    .operation = LWM2M_OPERATION_RW,
    .instances = LWM2M_INSTANCETYPE_single,
    .mandatory = false,
    .type = LWM2M_TYPE_string,
    .changed = Delivery_Schedule_Schedule_UTC_Offset_changed,
    .execute = (void *) 0,
    .get = __Delivery_Schedule_Schedule_UTC_Offset_get,
    .set = __Delivery_Schedule_Schedule_UTC_Offset_set,
  },
#else
  .resources[1] = { .supported = false },
#endif
#if CONFIG_Delivery_Schedule_Delivery_Frequency_OPTION == 1
  .resources[2] = { 
    .supported = true,  
    .id = 2,
    .operation = LWM2M_OPERATION_RW,
    .instances = LWM2M_INSTANCETYPE_single,
    .mandatory = false,
    .type = LWM2M_TYPE_integer,
    .changed = Delivery_Schedule_Delivery_Frequency_changed,
    .execute = (void *) 0,
    .get = __Delivery_Schedule_Delivery_Frequency_get,
    .set = __Delivery_Schedule_Delivery_Frequency_set,
  },
#else
  .resources[2] = { .supported = false },
#endif
#if CONFIG_Delivery_Schedule_Randomised_Delivery_Window_OPTION == 1
  .resources[3] = { 
    .supported = true,  
    .id = 3,
    .operation = LWM2M_OPERATION_RW,
    .instances = LWM2M_INSTANCETYPE_single,
    .mandatory = false,
    .type = LWM2M_TYPE_integer,
    .changed = Delivery_Schedule_Randomised_Delivery_Window_changed,
    .execute = (void *) 0,
    .get = __Delivery_Schedule_Randomised_Delivery_Window_get,
    .set = __Delivery_Schedule_Randomised_Delivery_Window_set,
  },
#else
  .resources[3] = { .supported = false },
#endif
#if CONFIG_Delivery_Schedule_Number_of_Retries_OPTION == 1
  .resources[4] = { 
    .supported = true,  
    .id = 4,
    .operation = LWM2M_OPERATION_RW,
    .instances = LWM2M_INSTANCETYPE_single,
    .mandatory = false,
    .type = LWM2M_TYPE_integer,
    .changed = Delivery_Schedule_Number_of_Retries_changed,
    .execute = (void *) 0,
    .get = __Delivery_Schedule_Number_of_Retries_get,
    .set = __Delivery_Schedule_Number_of_Retries_set,
  },
#else
  .resources[4] = { .supported = false },
#endif
#if CONFIG_Delivery_Schedule_Retry_Period_OPTION == 1
  .resources[5] = { 
    .supported = true,  
    .id = 5,
    .operation = LWM2M_OPERATION_RW,
    .instances = LWM2M_INSTANCETYPE_single,
    .mandatory = false,
    .type = LWM2M_TYPE_integer,
    .changed = Delivery_Schedule_Retry_Period_changed,
    .execute = (void *) 0,
    .get = __Delivery_Schedule_Retry_Period_get,
    .set = __Delivery_Schedule_Retry_Period_set,
  },
#else
  .resources[5] = { .supported = false },
#endif
};


void Delivery_Schedule_lock() 
{ 
  mutex_lock(&Delivery_Schedule_object_declaration.lock, MUTEXT_WAIT_FOREVER); 
}
void Delivery_Schedule_unlock() 
{ 
  mutex_unlock(&Delivery_Schedule_object_declaration.lock);
}


uint16_t Delivery_Schedule_get_index_for_instance(uint16_t instance)
{
  for (int i = 0; i < MAX_Delivery_Schedule_INSTANCES; i++)
  {
    if (Delivery_Schedule_instances[i].id == instance) return i;
  }
  return LWM2M_INVALID_INSTANCE;
}

__attribute__((weak)) lwm2m_object_t *Delivery_Schedule_initialise(void)
{
  mutex_initialise(&Delivery_Schedule_object_declaration.lock);
  
  memset(Delivery_Schedule_instances, 0, sizeof(Delivery_Schedule_instances));
  for (int i = 0; i < MAX_Delivery_Schedule_INSTANCES; i++)
  {
    Delivery_Schedule_instances[i].id = LWM2M_INVALID_INSTANCE;
    Delivery_Schedule_instances[i].index = i;
  }
  lwm2m_object_base_initialse_object(&Delivery_Schedule_lwm2m_object);
  Delivery_Schedule_lwm2m_object.userData = &Delivery_Schedule_object_declaration; 
  return &Delivery_Schedule_lwm2m_object;
}
  
__attribute__((weak)) void Delivery_Schedule_register_object(lwm2m_context_t *context)
{
  Delivery_Schedule_lock();
  Delivery_Schedule_object_declaration.context = context;
  Delivery_Schedule_unlock();
}
  
__attribute__((weak)) void Delivery_Schedule_register_instance(uint16_t instance_id)
{
  Delivery_Schedule_lock();
  uint16_t index = Delivery_Schedule_get_index_for_instance(instance_id);
  assert_void(index != LWM2M_INVALID_INSTANCE);
  
  if ((Delivery_Schedule_instances[index].id !=  LWM2M_INVALID_INSTANCE) && (!Delivery_Schedule_instances[index].registered))
  {
    Delivery_Schedule_lwm2m_object.instanceList = LWM2M_LIST_ADD(Delivery_Schedule_lwm2m_object.instanceList, (lwm2m_list_t *)&Delivery_Schedule_instances[index]);
    Delivery_Schedule_instances[index].registered = true;
  }
  Delivery_Schedule_unlock();
}

__attribute__((weak)) void Delivery_Schedule_unregister_instance(uint16_t instance_id)
{
  Delivery_Schedule_lock();
  uint16_t index = Delivery_Schedule_get_index_for_instance(instance_id);
  assert_void(index != LWM2M_INVALID_INSTANCE);
  
  if ((Delivery_Schedule_instances[index].id !=  LWM2M_INVALID_INSTANCE) && (Delivery_Schedule_instances[index].registered))
  {
    Delivery_Schedule_instance *instance = 0;
    Delivery_Schedule_lwm2m_object.instanceList = lwm2m_list_remove(Delivery_Schedule_lwm2m_object.instanceList, instance_id, (lwm2m_list_t **)&instance);
    Delivery_Schedule_instances[index].registered = false;
    if (instance != 0)
    { 
      instance->next = 0;
    }
  }
  Delivery_Schedule_unlock();
}

//override this to perform actions when a new instance is created
__attribute__((weak)) void Delivery_Schedule_instance_created(uint16_t instance_id)
{
  
}
  

//called when the server requests an object to be created; we need to create AND register the 
//instance
uint8_t __Delivery_Schedule_create_instance(uint16_t instance_id)
{
  uint8_t res = Delivery_Schedule_create_instance(instance_id);
  if (res == COAP_201_CREATED)
  {
    Delivery_Schedule_register_instance(instance_id);
  }
  return res;
}
  
__attribute__((weak)) uint8_t Delivery_Schedule_create_instance(uint16_t instance_id)
{
  Delivery_Schedule_lock();
  //find an unallocated instance
  for (int i = 0; i < MAX_Delivery_Schedule_INSTANCES; i++)
  {
    if (Delivery_Schedule_instances[i].id == LWM2M_INVALID_INSTANCE)
    {
      Delivery_Schedule_instances[i].id = instance_id;
      Delivery_Schedule_instance_created(instance_id);
      Delivery_Schedule_unlock();
      return COAP_201_CREATED;
    }
  }
  Delivery_Schedule_unlock();
  return COAP_500_INTERNAL_SERVER_ERROR; //none found
}

void Delivery_Schedule_foreach(lwm2m_each_instance_func func, void *data)
{
  Delivery_Schedule_lock();
  
  for (int i = 0; i < MAX_Delivery_Schedule_INSTANCES; i++)
  {
    if (Delivery_Schedule_instances[i].id != LWM2M_INVALID_INSTANCE)
    {
      if (!func(Delivery_Schedule_instances[i].id, data)) break;
    }
  }
  Delivery_Schedule_unlock();
}
  
//provides access to a user data pointer for storing additional instance
//data.
void *Delivery_Schedule_get_user_data(uint16_t instance_id)
{
  uint16_t index = Delivery_Schedule_get_index_for_instance(instance_id);
  if (index == LWM2M_INVALID_INSTANCE) return 0;
  return Delivery_Schedule_instances[index].user_data;
}
  
bool Delivery_Schedule_set_user_data(uint16_t instance_id, void *data)
{
  uint16_t index = Delivery_Schedule_get_index_for_instance(instance_id);
  if (index == LWM2M_INVALID_INSTANCE) return false;
  Delivery_Schedule_instances[index].user_data = data;
  return true;
}
  
//override this to perform actions when a new instance is created
__attribute__((weak)) void Delivery_Schedule_instance_deleted(uint16_t instance_id)
{
  
}

__attribute__((weak)) uint8_t Delivery_Schedule_delete_instance(uint16_t instance_id)
{
  Delivery_Schedule_lock();
  uint16_t index = Delivery_Schedule_get_index_for_instance(instance_id);
  assert(index != LWM2M_INVALID_INSTANCE);
  
  if ((instance_id < MAX_Delivery_Schedule_INSTANCES) && (Delivery_Schedule_instances[index].id !=  LWM2M_INVALID_INSTANCE))
  {
    Delivery_Schedule_unregister_instance(instance_id);
    Delivery_Schedule_instance_deleted(instance_id);
    Delivery_Schedule_instances[index].id = LWM2M_INVALID_INSTANCE;
  }
  Delivery_Schedule_unlock();
  return COAP_202_DELETED;
}


__attribute__((weak)) void Delivery_Schedule_changed(uint16_t instance)
{
  
}

#if CONFIG_Delivery_Schedule_Schedule_Start_Time_OPTION == 1
  
__attribute__((weak)) uint8_t Delivery_Schedule_Schedule_Start_Time_get(uint16_t instance,  uint32_t *val)
{
  Delivery_Schedule_lock();
  assert(instance >= 0 && instance < MAX_Delivery_Schedule_INSTANCES);
  uint16_t index = Delivery_Schedule_get_index_for_instance(instance);
  assert(index != LWM2M_INVALID_INSTANCE);
  
  assert(Delivery_Schedule_instances[index].id != LWM2M_INVALID_INSTANCE);
  
  
  *val = Delivery_Schedule_instances[index].Schedule_Start_Time;
  Delivery_Schedule_unlock();
  return COAP_205_CONTENT;
}
static uint8_t __Delivery_Schedule_Schedule_Start_Time_get(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data)
{
  uint32_t val;
  uint8_t res = Delivery_Schedule_Schedule_Start_Time_get(instance,  &val);
  lwm2m_data_encode_int(val, data);
  return res;
}
                                         
#endif
#if CONFIG_Delivery_Schedule_Schedule_Start_Time_OPTION == 1
__attribute__((weak)) uint8_t Delivery_Schedule_Schedule_Start_Time_set(uint16_t instance,  uint32_t val, bool notify)
{
  Delivery_Schedule_lock();
  assert(instance >= 0 && instance < MAX_Delivery_Schedule_INSTANCES);
  uint16_t index = Delivery_Schedule_get_index_for_instance(instance);
  assert(index != LWM2M_INVALID_INSTANCE);
  
  assert(Delivery_Schedule_instances[index].id != LWM2M_INVALID_INSTANCE);

  Delivery_Schedule_instances[index].Schedule_Start_Time = val;
  
  if (notify && Delivery_Schedule_instances[index].registered) lwm2m_object_base_resource_changed(&Delivery_Schedule_lwm2m_object, instance, 0, false);  
  Delivery_Schedule_unlock();
  return COAP_204_CHANGED;
}
static uint8_t __Delivery_Schedule_Schedule_Start_Time_set(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data)
{
  int64_t val;
  lwm2m_data_decode_int(data, &val);
  return Delivery_Schedule_Schedule_Start_Time_set(instance,  val, false);
}
#endif
#if CONFIG_Delivery_Schedule_Schedule_Start_Time_OPTION == 1
__attribute__((weak)) uint8_t Delivery_Schedule_Schedule_Start_Time_changed(uint16_t instance)
{
  Delivery_Schedule_lock();
  assert(instance >= 0 && instance < MAX_Delivery_Schedule_INSTANCES);
  uint16_t index = Delivery_Schedule_get_index_for_instance(instance);
  assert(index != LWM2M_INVALID_INSTANCE);
  
  Delivery_Schedule_changed(instance);
  Delivery_Schedule_unlock();
  return COAP_NO_ERROR;
}
#endif
#if CONFIG_Delivery_Schedule_Schedule_UTC_Offset_OPTION == 1
  
__attribute__((weak))  uint8_t Delivery_Schedule_Schedule_UTC_Offset_get(uint16_t instance,  const char **val)
{
  Delivery_Schedule_lock();
  assert(instance >= 0 && instance < MAX_Delivery_Schedule_INSTANCES);
  uint16_t index = Delivery_Schedule_get_index_for_instance(instance);
  assert(index != LWM2M_INVALID_INSTANCE);
  
  assert(Delivery_Schedule_instances[index].id != LWM2M_INVALID_INSTANCE);
  
  
  *val = Delivery_Schedule_instances[index].Schedule_UTC_Offset;
  Delivery_Schedule_unlock();
  return COAP_205_CONTENT;
}
static uint8_t __Delivery_Schedule_Schedule_UTC_Offset_get(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data)
{
  const char *val;
  uint8_t res = Delivery_Schedule_Schedule_UTC_Offset_get(instance,  &val);
  lwm2m_data_encode_string(val, data);
  return res;
}
                                         
#endif
#if CONFIG_Delivery_Schedule_Schedule_UTC_Offset_OPTION == 1
__attribute__((weak)) uint8_t Delivery_Schedule_Schedule_UTC_Offset_set(uint16_t instance,  const char *val, size_t length, bool notify)
{
  Delivery_Schedule_lock();
  assert(instance >= 0 && instance < MAX_Delivery_Schedule_INSTANCES);
  uint16_t index = Delivery_Schedule_get_index_for_instance(instance);
  assert(index != LWM2M_INVALID_INSTANCE);
  
  assert(Delivery_Schedule_instances[index].id != LWM2M_INVALID_INSTANCE);
 
  if (length >= sizeof(Delivery_Schedule_instances[index].Schedule_UTC_Offset )) length = sizeof(Delivery_Schedule_instances[index].Schedule_UTC_Offset) - 1;
  memcpy(Delivery_Schedule_instances[index].Schedule_UTC_Offset, val, length);
  Delivery_Schedule_instances[index].Schedule_UTC_Offset[length] = 0;
  
  
  if (notify && Delivery_Schedule_instances[index].registered) lwm2m_object_base_resource_changed(&Delivery_Schedule_lwm2m_object, instance, 1, false);
  Delivery_Schedule_unlock();
  return COAP_204_CHANGED;
}
static uint8_t __Delivery_Schedule_Schedule_UTC_Offset_set(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data)
{
  return Delivery_Schedule_Schedule_UTC_Offset_set(instance,  (char*)data->value.asBuffer.buffer, data->value.asBuffer.length, false);
}
#endif
#if CONFIG_Delivery_Schedule_Schedule_UTC_Offset_OPTION == 1
__attribute__((weak)) uint8_t Delivery_Schedule_Schedule_UTC_Offset_changed(uint16_t instance)
{
  Delivery_Schedule_lock();
  assert(instance >= 0 && instance < MAX_Delivery_Schedule_INSTANCES);
  uint16_t index = Delivery_Schedule_get_index_for_instance(instance);
  assert(index != LWM2M_INVALID_INSTANCE);
  
  Delivery_Schedule_changed(instance);
  Delivery_Schedule_unlock();
  return COAP_NO_ERROR;
}
#endif
#if CONFIG_Delivery_Schedule_Delivery_Frequency_OPTION == 1
  
__attribute__((weak)) uint8_t Delivery_Schedule_Delivery_Frequency_get(uint16_t instance,  uint32_t *val)
{
  Delivery_Schedule_lock();
  assert(instance >= 0 && instance < MAX_Delivery_Schedule_INSTANCES);
  uint16_t index = Delivery_Schedule_get_index_for_instance(instance);
  assert(index != LWM2M_INVALID_INSTANCE);
  
  assert(Delivery_Schedule_instances[index].id != LWM2M_INVALID_INSTANCE);
  
  
  *val = Delivery_Schedule_instances[index].Delivery_Frequency;
  Delivery_Schedule_unlock();
  return COAP_205_CONTENT;
}
static uint8_t __Delivery_Schedule_Delivery_Frequency_get(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data)
{
  uint32_t val;
  uint8_t res = Delivery_Schedule_Delivery_Frequency_get(instance,  &val);
  lwm2m_data_encode_int(val, data);
  return res;
}
                                         
#endif
#if CONFIG_Delivery_Schedule_Delivery_Frequency_OPTION == 1
__attribute__((weak)) uint8_t Delivery_Schedule_Delivery_Frequency_set(uint16_t instance,  uint32_t val, bool notify)
{
  Delivery_Schedule_lock();
  assert(instance >= 0 && instance < MAX_Delivery_Schedule_INSTANCES);
  uint16_t index = Delivery_Schedule_get_index_for_instance(instance);
  assert(index != LWM2M_INVALID_INSTANCE);
  
  assert(Delivery_Schedule_instances[index].id != LWM2M_INVALID_INSTANCE);

  Delivery_Schedule_instances[index].Delivery_Frequency = val;
  
  if (notify && Delivery_Schedule_instances[index].registered) lwm2m_object_base_resource_changed(&Delivery_Schedule_lwm2m_object, instance, 2, false);  
  Delivery_Schedule_unlock();
  return COAP_204_CHANGED;
}
static uint8_t __Delivery_Schedule_Delivery_Frequency_set(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data)
{
  int64_t val;
  lwm2m_data_decode_int(data, &val);
  return Delivery_Schedule_Delivery_Frequency_set(instance,  val, false);
}
#endif
#if CONFIG_Delivery_Schedule_Delivery_Frequency_OPTION == 1
__attribute__((weak)) uint8_t Delivery_Schedule_Delivery_Frequency_changed(uint16_t instance)
{
  Delivery_Schedule_lock();
  assert(instance >= 0 && instance < MAX_Delivery_Schedule_INSTANCES);
  uint16_t index = Delivery_Schedule_get_index_for_instance(instance);
  assert(index != LWM2M_INVALID_INSTANCE);
  
  Delivery_Schedule_changed(instance);
  Delivery_Schedule_unlock();
  return COAP_NO_ERROR;
}
#endif
#if CONFIG_Delivery_Schedule_Randomised_Delivery_Window_OPTION == 1
  
__attribute__((weak)) uint8_t Delivery_Schedule_Randomised_Delivery_Window_get(uint16_t instance,  uint32_t *val)
{
  Delivery_Schedule_lock();
  assert(instance >= 0 && instance < MAX_Delivery_Schedule_INSTANCES);
  uint16_t index = Delivery_Schedule_get_index_for_instance(instance);
  assert(index != LWM2M_INVALID_INSTANCE);
  
  assert(Delivery_Schedule_instances[index].id != LWM2M_INVALID_INSTANCE);
  
  
  *val = Delivery_Schedule_instances[index].Randomised_Delivery_Window;
  Delivery_Schedule_unlock();
  return COAP_205_CONTENT;
}
static uint8_t __Delivery_Schedule_Randomised_Delivery_Window_get(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data)
{
  uint32_t val;
  uint8_t res = Delivery_Schedule_Randomised_Delivery_Window_get(instance,  &val);
  lwm2m_data_encode_int(val, data);
  return res;
}
                                         
#endif
#if CONFIG_Delivery_Schedule_Randomised_Delivery_Window_OPTION == 1
__attribute__((weak)) uint8_t Delivery_Schedule_Randomised_Delivery_Window_set(uint16_t instance,  uint32_t val, bool notify)
{
  Delivery_Schedule_lock();
  assert(instance >= 0 && instance < MAX_Delivery_Schedule_INSTANCES);
  uint16_t index = Delivery_Schedule_get_index_for_instance(instance);
  assert(index != LWM2M_INVALID_INSTANCE);
  
  assert(Delivery_Schedule_instances[index].id != LWM2M_INVALID_INSTANCE);

  Delivery_Schedule_instances[index].Randomised_Delivery_Window = val;
  
  if (notify && Delivery_Schedule_instances[index].registered) lwm2m_object_base_resource_changed(&Delivery_Schedule_lwm2m_object, instance, 3, false);  
  Delivery_Schedule_unlock();
  return COAP_204_CHANGED;
}
static uint8_t __Delivery_Schedule_Randomised_Delivery_Window_set(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data)
{
  int64_t val;
  lwm2m_data_decode_int(data, &val);
  return Delivery_Schedule_Randomised_Delivery_Window_set(instance,  val, false);
}
#endif
#if CONFIG_Delivery_Schedule_Randomised_Delivery_Window_OPTION == 1
__attribute__((weak)) uint8_t Delivery_Schedule_Randomised_Delivery_Window_changed(uint16_t instance)
{
  Delivery_Schedule_lock();
  assert(instance >= 0 && instance < MAX_Delivery_Schedule_INSTANCES);
  uint16_t index = Delivery_Schedule_get_index_for_instance(instance);
  assert(index != LWM2M_INVALID_INSTANCE);
  
  Delivery_Schedule_changed(instance);
  Delivery_Schedule_unlock();
  return COAP_NO_ERROR;
}
#endif
#if CONFIG_Delivery_Schedule_Number_of_Retries_OPTION == 1
  
__attribute__((weak)) uint8_t Delivery_Schedule_Number_of_Retries_get(uint16_t instance,  uint32_t *val)
{
  Delivery_Schedule_lock();
  assert(instance >= 0 && instance < MAX_Delivery_Schedule_INSTANCES);
  uint16_t index = Delivery_Schedule_get_index_for_instance(instance);
  assert(index != LWM2M_INVALID_INSTANCE);
  
  assert(Delivery_Schedule_instances[index].id != LWM2M_INVALID_INSTANCE);
  
  
  *val = Delivery_Schedule_instances[index].Number_of_Retries;
  Delivery_Schedule_unlock();
  return COAP_205_CONTENT;
}
static uint8_t __Delivery_Schedule_Number_of_Retries_get(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data)
{
  uint32_t val;
  uint8_t res = Delivery_Schedule_Number_of_Retries_get(instance,  &val);
  lwm2m_data_encode_int(val, data);
  return res;
}
                                         
#endif
#if CONFIG_Delivery_Schedule_Number_of_Retries_OPTION == 1
__attribute__((weak)) uint8_t Delivery_Schedule_Number_of_Retries_set(uint16_t instance,  uint32_t val, bool notify)
{
  Delivery_Schedule_lock();
  assert(instance >= 0 && instance < MAX_Delivery_Schedule_INSTANCES);
  uint16_t index = Delivery_Schedule_get_index_for_instance(instance);
  assert(index != LWM2M_INVALID_INSTANCE);
  
  assert(Delivery_Schedule_instances[index].id != LWM2M_INVALID_INSTANCE);

  Delivery_Schedule_instances[index].Number_of_Retries = val;
  
  if (notify && Delivery_Schedule_instances[index].registered) lwm2m_object_base_resource_changed(&Delivery_Schedule_lwm2m_object, instance, 4, false);  
  Delivery_Schedule_unlock();
  return COAP_204_CHANGED;
}
static uint8_t __Delivery_Schedule_Number_of_Retries_set(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data)
{
  int64_t val;
  lwm2m_data_decode_int(data, &val);
  return Delivery_Schedule_Number_of_Retries_set(instance,  val, false);
}
#endif
#if CONFIG_Delivery_Schedule_Number_of_Retries_OPTION == 1
__attribute__((weak)) uint8_t Delivery_Schedule_Number_of_Retries_changed(uint16_t instance)
{
  Delivery_Schedule_lock();
  assert(instance >= 0 && instance < MAX_Delivery_Schedule_INSTANCES);
  uint16_t index = Delivery_Schedule_get_index_for_instance(instance);
  assert(index != LWM2M_INVALID_INSTANCE);
  
  Delivery_Schedule_changed(instance);
  Delivery_Schedule_unlock();
  return COAP_NO_ERROR;
}
#endif
#if CONFIG_Delivery_Schedule_Retry_Period_OPTION == 1
  
__attribute__((weak)) uint8_t Delivery_Schedule_Retry_Period_get(uint16_t instance,  uint32_t *val)
{
  Delivery_Schedule_lock();
  assert(instance >= 0 && instance < MAX_Delivery_Schedule_INSTANCES);
  uint16_t index = Delivery_Schedule_get_index_for_instance(instance);
  assert(index != LWM2M_INVALID_INSTANCE);
  
  assert(Delivery_Schedule_instances[index].id != LWM2M_INVALID_INSTANCE);
  
  
  *val = Delivery_Schedule_instances[index].Retry_Period;
  Delivery_Schedule_unlock();
  return COAP_205_CONTENT;
}
static uint8_t __Delivery_Schedule_Retry_Period_get(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data)
{
  uint32_t val;
  uint8_t res = Delivery_Schedule_Retry_Period_get(instance,  &val);
  lwm2m_data_encode_int(val, data);
  return res;
}
                                         
#endif
#if CONFIG_Delivery_Schedule_Retry_Period_OPTION == 1
__attribute__((weak)) uint8_t Delivery_Schedule_Retry_Period_set(uint16_t instance,  uint32_t val, bool notify)
{
  Delivery_Schedule_lock();
  assert(instance >= 0 && instance < MAX_Delivery_Schedule_INSTANCES);
  uint16_t index = Delivery_Schedule_get_index_for_instance(instance);
  assert(index != LWM2M_INVALID_INSTANCE);
  
  assert(Delivery_Schedule_instances[index].id != LWM2M_INVALID_INSTANCE);

  Delivery_Schedule_instances[index].Retry_Period = val;
  
  if (notify && Delivery_Schedule_instances[index].registered) lwm2m_object_base_resource_changed(&Delivery_Schedule_lwm2m_object, instance, 5, false);  
  Delivery_Schedule_unlock();
  return COAP_204_CHANGED;
}
static uint8_t __Delivery_Schedule_Retry_Period_set(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data)
{
  int64_t val;
  lwm2m_data_decode_int(data, &val);
  return Delivery_Schedule_Retry_Period_set(instance,  val, false);
}
#endif
#if CONFIG_Delivery_Schedule_Retry_Period_OPTION == 1
__attribute__((weak)) uint8_t Delivery_Schedule_Retry_Period_changed(uint16_t instance)
{
  Delivery_Schedule_lock();
  assert(instance >= 0 && instance < MAX_Delivery_Schedule_INSTANCES);
  uint16_t index = Delivery_Schedule_get_index_for_instance(instance);
  assert(index != LWM2M_INVALID_INSTANCE);
  
  Delivery_Schedule_changed(instance);
  Delivery_Schedule_unlock();
  return COAP_NO_ERROR;
}
#endif
