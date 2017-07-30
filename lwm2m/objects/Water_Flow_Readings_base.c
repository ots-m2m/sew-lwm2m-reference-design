/*
 * Water_Flow_Readings_base.c
 * 
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * This code was generated using gt-object-gen provided by Operational 
 * Technology Solutions M2M Pty. Ltd. For licensing enquires visit 
 * ot-solutions.com.au.
 */
#include "Water_Flow_Readings_base.h"

#include <string.h>
#include <stdbool.h>

#include "mutex.h"
#define assert(condition) if (!(condition)) { Water_Flow_Readings_unlock(); return COAP_406_NOT_ACCEPTABLE; }
#define assert_void(condition) if (!(condition)) { Water_Flow_Readings_unlock(); return; }

uint8_t __Water_Flow_Readings_create_instance(uint16_t instance_id);
#if CONFIG_Water_Flow_Readings_Interval_Period_OPTION == 1
static uint8_t __Water_Flow_Readings_Interval_Period_get(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data);
  
#endif
#if CONFIG_Water_Flow_Readings_Interval_Start_Offset_OPTION == 1
static uint8_t __Water_Flow_Readings_Interval_Start_Offset_get(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data);
  
#endif
#if CONFIG_Water_Flow_Readings_Interval_UTC_Offset_OPTION == 1
static uint8_t __Water_Flow_Readings_Interval_UTC_Offset_get(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data);
  
#endif
#if CONFIG_Water_Flow_Readings_Interval_Collection_Start_Time_OPTION == 1
static uint8_t __Water_Flow_Readings_Interval_Collection_Start_Time_get(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data);
  
#endif
#if CONFIG_Water_Flow_Readings_Oldest_Recorded_Interval_OPTION == 1
static uint8_t __Water_Flow_Readings_Oldest_Recorded_Interval_get(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data);
  
#endif
#if CONFIG_Water_Flow_Readings_Last_Delivered_Interval_OPTION == 1
static uint8_t __Water_Flow_Readings_Last_Delivered_Interval_get(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data);
  
#endif
#if CONFIG_Water_Flow_Readings_Last_Delivered_Interval_OPTION == 1
static uint8_t __Water_Flow_Readings_Last_Delivered_Interval_set(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data);
#endif
#if CONFIG_Water_Flow_Readings_Latest_Recorded_Interval_OPTION == 1
static uint8_t __Water_Flow_Readings_Latest_Recorded_Interval_get(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data);
  
#endif
#if CONFIG_Water_Flow_Readings_Interval_Delivery_Midnight_Aligned_OPTION == 1
static uint8_t __Water_Flow_Readings_Interval_Delivery_Midnight_Aligned_get(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data);
  
#endif
#if CONFIG_Water_Flow_Readings_Interval_Delivery_Midnight_Aligned_OPTION == 1
static uint8_t __Water_Flow_Readings_Interval_Delivery_Midnight_Aligned_set(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data);
#endif
#if CONFIG_Water_Flow_Readings_Interval_Historical_Read_Payload_OPTION == 1
static uint8_t __Water_Flow_Readings_Interval_Historical_Read_Payload_get(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data);
  
#endif
#if CONFIG_Water_Flow_Readings_Status_OPTION == 1
static uint8_t __Water_Flow_Readings_Status_get(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data);
  
#endif
#if CONFIG_Water_Flow_Readings_Latest_Payload_OPTION == 1
static uint8_t __Water_Flow_Readings_Latest_Payload_get(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data);
  
#endif

static lwm2m_object_t Water_Flow_Readings_lwm2m_object = {
  .objID = 8010
};

lwm2m_object_t *Water_Flow_Readings_get_lwm2m_object(void)
{
  return &Water_Flow_Readings_lwm2m_object;
}

typedef struct Water_Flow_Readings_instance_t
{
  struct Water_Flow_Readings_instance * next;   // matches lwm2m_list_t::next
  uint16_t id;                                // matches lwm2m_list_t::id
  uint16_t index;
  bool registered;
  void *user_data;
#if CONFIG_Water_Flow_Readings_Interval_Period_OPTION == 1
  uint32_t Interval_Period;
#endif
#if CONFIG_Water_Flow_Readings_Interval_Start_Offset_OPTION == 1
  uint32_t Interval_Start_Offset;
#endif
#if CONFIG_Water_Flow_Readings_Interval_UTC_Offset_OPTION == 1
  char Interval_UTC_Offset [MAX_Water_Flow_Readings_Interval_UTC_Offset];
#endif
#if CONFIG_Water_Flow_Readings_Interval_Collection_Start_Time_OPTION == 1
  time_t Interval_Collection_Start_Time;
#endif
#if CONFIG_Water_Flow_Readings_Oldest_Recorded_Interval_OPTION == 1
  time_t Oldest_Recorded_Interval;
#endif
#if CONFIG_Water_Flow_Readings_Last_Delivered_Interval_OPTION == 1
  time_t Last_Delivered_Interval;
#endif
#if CONFIG_Water_Flow_Readings_Latest_Recorded_Interval_OPTION == 1
  time_t Latest_Recorded_Interval;
#endif
#if CONFIG_Water_Flow_Readings_Interval_Delivery_Midnight_Aligned_OPTION == 1
  bool Interval_Delivery_Midnight_Aligned;
#endif
#if CONFIG_Water_Flow_Readings_Interval_Historical_Read_Payload_OPTION == 1
  uint8_t *Interval_Historical_Read_Payload;
  uint16_t Interval_Historical_Read_Payload_length;
#endif
#if CONFIG_Water_Flow_Readings_Status_OPTION == 1
  uint32_t Status;
#endif
#if CONFIG_Water_Flow_Readings_Latest_Payload_OPTION == 1
  uint8_t *Latest_Payload;
  uint16_t Latest_Payload_length;
#endif
}
Water_Flow_Readings_instance;

static Water_Flow_Readings_instance Water_Flow_Readings_instances[MAX_Water_Flow_Readings_INSTANCES];

lwm2m_object_declaration_t Water_Flow_Readings_object_declaration = {
  .id = 8010,
  .delete = Water_Flow_Readings_delete_instance,
  .create = __Water_Flow_Readings_create_instance,
#if CONFIG_Water_Flow_Readings_Interval_Period_OPTION == 1
  .resources[0] = { 
    .supported = true,  
    .id = 9000,
    .operation = LWM2M_OPERATION_R,
    .instances = LWM2M_INSTANCETYPE_single,
    .mandatory = false,
    .type = LWM2M_TYPE_integer,
    .changed = (void *) 0,
    .execute = (void *) 0,
    .get = __Water_Flow_Readings_Interval_Period_get,
    .set = (void *) 0,
  },
#else
  .resources[0] = { .supported = false },
#endif
#if CONFIG_Water_Flow_Readings_Interval_Start_Offset_OPTION == 1
  .resources[1] = { 
    .supported = true,  
    .id = 9001,
    .operation = LWM2M_OPERATION_R,
    .instances = LWM2M_INSTANCETYPE_single,
    .mandatory = false,
    .type = LWM2M_TYPE_integer,
    .changed = (void *) 0,
    .execute = (void *) 0,
    .get = __Water_Flow_Readings_Interval_Start_Offset_get,
    .set = (void *) 0,
  },
#else
  .resources[1] = { .supported = false },
#endif
#if CONFIG_Water_Flow_Readings_Interval_UTC_Offset_OPTION == 1
  .resources[2] = { 
    .supported = true,  
    .id = 9002,
    .operation = LWM2M_OPERATION_R,
    .instances = LWM2M_INSTANCETYPE_single,
    .mandatory = false,
    .type = LWM2M_TYPE_string,
    .changed = (void *) 0,
    .execute = (void *) 0,
    .get = __Water_Flow_Readings_Interval_UTC_Offset_get,
    .set = (void *) 0,
  },
#else
  .resources[2] = { .supported = false },
#endif
#if CONFIG_Water_Flow_Readings_Interval_Collection_Start_Time_OPTION == 1
  .resources[3] = { 
    .supported = true,  
    .id = 9003,
    .operation = LWM2M_OPERATION_R,
    .instances = LWM2M_INSTANCETYPE_single,
    .mandatory = false,
    .type = LWM2M_TYPE_time,
    .changed = (void *) 0,
    .execute = (void *) 0,
    .get = __Water_Flow_Readings_Interval_Collection_Start_Time_get,
    .set = (void *) 0,
  },
#else
  .resources[3] = { .supported = false },
#endif
#if CONFIG_Water_Flow_Readings_Oldest_Recorded_Interval_OPTION == 1
  .resources[4] = { 
    .supported = true,  
    .id = 9004,
    .operation = LWM2M_OPERATION_R,
    .instances = LWM2M_INSTANCETYPE_single,
    .mandatory = false,
    .type = LWM2M_TYPE_time,
    .changed = (void *) 0,
    .execute = (void *) 0,
    .get = __Water_Flow_Readings_Oldest_Recorded_Interval_get,
    .set = (void *) 0,
  },
#else
  .resources[4] = { .supported = false },
#endif
#if CONFIG_Water_Flow_Readings_Last_Delivered_Interval_OPTION == 1
  .resources[5] = { 
    .supported = true,  
    .id = 9005,
    .operation = LWM2M_OPERATION_RW,
    .instances = LWM2M_INSTANCETYPE_single,
    .mandatory = false,
    .type = LWM2M_TYPE_time,
    .changed = Water_Flow_Readings_Last_Delivered_Interval_changed,
    .execute = (void *) 0,
    .get = __Water_Flow_Readings_Last_Delivered_Interval_get,
    .set = __Water_Flow_Readings_Last_Delivered_Interval_set,
  },
#else
  .resources[5] = { .supported = false },
#endif
#if CONFIG_Water_Flow_Readings_Latest_Recorded_Interval_OPTION == 1
  .resources[6] = { 
    .supported = true,  
    .id = 9006,
    .operation = LWM2M_OPERATION_R,
    .instances = LWM2M_INSTANCETYPE_single,
    .mandatory = false,
    .type = LWM2M_TYPE_time,
    .changed = (void *) 0,
    .execute = (void *) 0,
    .get = __Water_Flow_Readings_Latest_Recorded_Interval_get,
    .set = (void *) 0,
  },
#else
  .resources[6] = { .supported = false },
#endif
#if CONFIG_Water_Flow_Readings_Interval_Delivery_Midnight_Aligned_OPTION == 1
  .resources[7] = { 
    .supported = true,  
    .id = 9007,
    .operation = LWM2M_OPERATION_RW,
    .instances = LWM2M_INSTANCETYPE_single,
    .mandatory = false,
    .type = LWM2M_TYPE_boolean,
    .changed = Water_Flow_Readings_Interval_Delivery_Midnight_Aligned_changed,
    .execute = (void *) 0,
    .get = __Water_Flow_Readings_Interval_Delivery_Midnight_Aligned_get,
    .set = __Water_Flow_Readings_Interval_Delivery_Midnight_Aligned_set,
  },
#else
  .resources[7] = { .supported = false },
#endif
#if CONFIG_Water_Flow_Readings_Interval_Historical_Read_OPTION == 1
  .resources[8] = { 
    .supported = true,  
    .id = 9008,
    .operation = LWM2M_OPERATION_E,
    .instances = LWM2M_INSTANCETYPE_single,
    .mandatory = false,
    .type = LWM2M_TYPE_string,
    .changed = (void *) 0,
    .execute = Water_Flow_Readings_Interval_Historical_Read_execute,
    .get = (void *) 0,
    .set = (void *) 0,
  },
#else
  .resources[8] = { .supported = false },
#endif
#if CONFIG_Water_Flow_Readings_Interval_Historical_Read_Payload_OPTION == 1
  .resources[9] = { 
    .supported = true,  
    .id = 9009,
    .operation = LWM2M_OPERATION_R,
    .instances = LWM2M_INSTANCETYPE_single,
    .mandatory = false,
    .type = LWM2M_TYPE_opaque,
    .changed = (void *) 0,
    .execute = (void *) 0,
    .get = __Water_Flow_Readings_Interval_Historical_Read_Payload_get,
    .set = (void *) 0,
  },
#else
  .resources[9] = { .supported = false },
#endif
#if CONFIG_Water_Flow_Readings_Interval_Change_Configuration_OPTION == 1
  .resources[10] = { 
    .supported = true,  
    .id = 9010,
    .operation = LWM2M_OPERATION_E,
    .instances = LWM2M_INSTANCETYPE_single,
    .mandatory = false,
    .type = LWM2M_TYPE_string,
    .changed = (void *) 0,
    .execute = Water_Flow_Readings_Interval_Change_Configuration_execute,
    .get = (void *) 0,
    .set = (void *) 0,
  },
#else
  .resources[10] = { .supported = false },
#endif
#if CONFIG_Water_Flow_Readings_Start_OPTION == 1
  .resources[11] = { 
    .supported = true,  
    .id = 4011,
    .operation = LWM2M_OPERATION_E,
    .instances = LWM2M_INSTANCETYPE_single,
    .mandatory = false,
    .type = LWM2M_TYPE_string,
    .changed = (void *) 0,
    .execute = Water_Flow_Readings_Start_execute,
    .get = (void *) 0,
    .set = (void *) 0,
  },
#else
  .resources[11] = { .supported = false },
#endif
#if CONFIG_Water_Flow_Readings_Stop_OPTION == 1
  .resources[12] = { 
    .supported = true,  
    .id = 4012,
    .operation = LWM2M_OPERATION_E,
    .instances = LWM2M_INSTANCETYPE_single,
    .mandatory = false,
    .type = LWM2M_TYPE_string,
    .changed = (void *) 0,
    .execute = Water_Flow_Readings_Stop_execute,
    .get = (void *) 0,
    .set = (void *) 0,
  },
#else
  .resources[12] = { .supported = false },
#endif
#if CONFIG_Water_Flow_Readings_Status_OPTION == 1
  .resources[13] = { 
    .supported = true,  
    .id = 4013,
    .operation = LWM2M_OPERATION_R,
    .instances = LWM2M_INSTANCETYPE_single,
    .mandatory = false,
    .type = LWM2M_TYPE_integer,
    .changed = (void *) 0,
    .execute = (void *) 0,
    .get = __Water_Flow_Readings_Status_get,
    .set = (void *) 0,
  },
#else
  .resources[13] = { .supported = false },
#endif
#if CONFIG_Water_Flow_Readings_Latest_Payload_OPTION == 1
  .resources[14] = { 
    .supported = true,  
    .id = 4014,
    .operation = LWM2M_OPERATION_R,
    .instances = LWM2M_INSTANCETYPE_single,
    .mandatory = false,
    .type = LWM2M_TYPE_opaque,
    .changed = (void *) 0,
    .execute = (void *) 0,
    .get = __Water_Flow_Readings_Latest_Payload_get,
    .set = (void *) 0,
  },
#else
  .resources[14] = { .supported = false },
#endif
};


void Water_Flow_Readings_lock() 
{ 
  mutex_lock(&Water_Flow_Readings_object_declaration.lock, MUTEXT_WAIT_FOREVER); 
}
void Water_Flow_Readings_unlock() 
{ 
  mutex_unlock(&Water_Flow_Readings_object_declaration.lock);
}


uint16_t Water_Flow_Readings_get_index_for_instance(uint16_t instance)
{
  for (int i = 0; i < MAX_Water_Flow_Readings_INSTANCES; i++)
  {
    if (Water_Flow_Readings_instances[i].id == instance) return i;
  }
  return LWM2M_INVALID_INSTANCE;
}

__attribute__((weak)) lwm2m_object_t *Water_Flow_Readings_initialise(void)
{
  mutex_initialise(&Water_Flow_Readings_object_declaration.lock);
  
  memset(Water_Flow_Readings_instances, 0, sizeof(Water_Flow_Readings_instances));
  for (int i = 0; i < MAX_Water_Flow_Readings_INSTANCES; i++)
  {
    Water_Flow_Readings_instances[i].id = LWM2M_INVALID_INSTANCE;
    Water_Flow_Readings_instances[i].index = i;
  }
  lwm2m_object_base_initialse_object(&Water_Flow_Readings_lwm2m_object);
  Water_Flow_Readings_lwm2m_object.userData = &Water_Flow_Readings_object_declaration; 
  return &Water_Flow_Readings_lwm2m_object;
}
  
__attribute__((weak)) void Water_Flow_Readings_register_object(lwm2m_context_t *context)
{
  Water_Flow_Readings_lock();
  Water_Flow_Readings_object_declaration.context = context;
  Water_Flow_Readings_unlock();
}
  
__attribute__((weak)) void Water_Flow_Readings_register_instance(uint16_t instance_id)
{
  Water_Flow_Readings_lock();
  uint16_t index = Water_Flow_Readings_get_index_for_instance(instance_id);
  assert_void(index != LWM2M_INVALID_INSTANCE);
  
  if ((Water_Flow_Readings_instances[index].id !=  LWM2M_INVALID_INSTANCE) && (!Water_Flow_Readings_instances[index].registered))
  {
    Water_Flow_Readings_lwm2m_object.instanceList = LWM2M_LIST_ADD(Water_Flow_Readings_lwm2m_object.instanceList, (lwm2m_list_t *)&Water_Flow_Readings_instances[index]);
    Water_Flow_Readings_instances[index].registered = true;
  }
  Water_Flow_Readings_unlock();
}

__attribute__((weak)) void Water_Flow_Readings_unregister_instance(uint16_t instance_id)
{
  Water_Flow_Readings_lock();
  uint16_t index = Water_Flow_Readings_get_index_for_instance(instance_id);
  assert_void(index != LWM2M_INVALID_INSTANCE);
  
  if ((Water_Flow_Readings_instances[index].id !=  LWM2M_INVALID_INSTANCE) && (Water_Flow_Readings_instances[index].registered))
  {
    Water_Flow_Readings_instance *instance = 0;
    Water_Flow_Readings_lwm2m_object.instanceList = lwm2m_list_remove(Water_Flow_Readings_lwm2m_object.instanceList, instance_id, (lwm2m_list_t **)&instance);
    Water_Flow_Readings_instances[index].registered = false;
    if (instance != 0)
    { 
      instance->next = 0;
    }
  }
  Water_Flow_Readings_unlock();
}

//override this to perform actions when a new instance is created
__attribute__((weak)) void Water_Flow_Readings_instance_created(uint16_t instance_id)
{
  
}
  

//called when the server requests an object to be created; we need to create AND register the 
//instance
uint8_t __Water_Flow_Readings_create_instance(uint16_t instance_id)
{
  uint8_t res = Water_Flow_Readings_create_instance(instance_id);
  if (res == COAP_201_CREATED)
  {
    Water_Flow_Readings_register_instance(instance_id);
  }
  return res;
}
  
__attribute__((weak)) uint8_t Water_Flow_Readings_create_instance(uint16_t instance_id)
{
  Water_Flow_Readings_lock();
  //find an unallocated instance
  for (int i = 0; i < MAX_Water_Flow_Readings_INSTANCES; i++)
  {
    if (Water_Flow_Readings_instances[i].id == LWM2M_INVALID_INSTANCE)
    {
      Water_Flow_Readings_instances[i].id = instance_id;
      Water_Flow_Readings_instance_created(instance_id);
      Water_Flow_Readings_unlock();
      return COAP_201_CREATED;
    }
  }
  Water_Flow_Readings_unlock();
  return COAP_500_INTERNAL_SERVER_ERROR; //none found
}

void Water_Flow_Readings_foreach(lwm2m_each_instance_func func, void *data)
{
  Water_Flow_Readings_lock();
  
  for (int i = 0; i < MAX_Water_Flow_Readings_INSTANCES; i++)
  {
    if (Water_Flow_Readings_instances[i].id != LWM2M_INVALID_INSTANCE)
    {
      if (!func(Water_Flow_Readings_instances[i].id, data)) break;
    }
  }
  Water_Flow_Readings_unlock();
}
  
//provides access to a user data pointer for storing additional instance
//data.
void *Water_Flow_Readings_get_user_data(uint16_t instance_id)
{
  uint16_t index = Water_Flow_Readings_get_index_for_instance(instance_id);
  if (index == LWM2M_INVALID_INSTANCE) return 0;
  return Water_Flow_Readings_instances[index].user_data;
}
  
bool Water_Flow_Readings_set_user_data(uint16_t instance_id, void *data)
{
  uint16_t index = Water_Flow_Readings_get_index_for_instance(instance_id);
  if (index == LWM2M_INVALID_INSTANCE) return false;
  Water_Flow_Readings_instances[index].user_data = data;
  return true;
}
  
//override this to perform actions when a new instance is created
__attribute__((weak)) void Water_Flow_Readings_instance_deleted(uint16_t instance_id)
{
  
}

__attribute__((weak)) uint8_t Water_Flow_Readings_delete_instance(uint16_t instance_id)
{
  Water_Flow_Readings_lock();
  uint16_t index = Water_Flow_Readings_get_index_for_instance(instance_id);
  assert(index != LWM2M_INVALID_INSTANCE);
  
  if ((instance_id < MAX_Water_Flow_Readings_INSTANCES) && (Water_Flow_Readings_instances[index].id !=  LWM2M_INVALID_INSTANCE))
  {
    Water_Flow_Readings_unregister_instance(instance_id);
    Water_Flow_Readings_instance_deleted(instance_id);
    Water_Flow_Readings_instances[index].id = LWM2M_INVALID_INSTANCE;
  }
  Water_Flow_Readings_unlock();
  return COAP_202_DELETED;
}


__attribute__((weak)) void Water_Flow_Readings_changed(uint16_t instance)
{
  
}

#if CONFIG_Water_Flow_Readings_Interval_Period_OPTION == 1
  
__attribute__((weak)) uint8_t Water_Flow_Readings_Interval_Period_get(uint16_t instance,  uint32_t *val)
{
  Water_Flow_Readings_lock();
  assert(instance >= 0 && instance < MAX_Water_Flow_Readings_INSTANCES);
  uint16_t index = Water_Flow_Readings_get_index_for_instance(instance);
  assert(index != LWM2M_INVALID_INSTANCE);
  
  assert(Water_Flow_Readings_instances[index].id != LWM2M_INVALID_INSTANCE);
  
  
  *val = Water_Flow_Readings_instances[index].Interval_Period;
  Water_Flow_Readings_unlock();
  return COAP_205_CONTENT;
}
static uint8_t __Water_Flow_Readings_Interval_Period_get(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data)
{
  uint32_t val;
  uint8_t res = Water_Flow_Readings_Interval_Period_get(instance,  &val);
  lwm2m_data_encode_int(val, data);
  return res;
}
                                         
#endif
#if CONFIG_Water_Flow_Readings_Interval_Period_OPTION == 1
__attribute__((weak)) uint8_t Water_Flow_Readings_Interval_Period_set(uint16_t instance,  uint32_t val, bool notify)
{
  Water_Flow_Readings_lock();
  assert(instance >= 0 && instance < MAX_Water_Flow_Readings_INSTANCES);
  uint16_t index = Water_Flow_Readings_get_index_for_instance(instance);
  assert(index != LWM2M_INVALID_INSTANCE);
  
  assert(Water_Flow_Readings_instances[index].id != LWM2M_INVALID_INSTANCE);

  Water_Flow_Readings_instances[index].Interval_Period = val;
  
  if (notify && Water_Flow_Readings_instances[index].registered) lwm2m_object_base_resource_changed(&Water_Flow_Readings_lwm2m_object, instance, 9000, false);  
  Water_Flow_Readings_unlock();
  return COAP_204_CHANGED;
}
#endif
#if CONFIG_Water_Flow_Readings_Interval_Start_Offset_OPTION == 1
  
__attribute__((weak)) uint8_t Water_Flow_Readings_Interval_Start_Offset_get(uint16_t instance,  uint32_t *val)
{
  Water_Flow_Readings_lock();
  assert(instance >= 0 && instance < MAX_Water_Flow_Readings_INSTANCES);
  uint16_t index = Water_Flow_Readings_get_index_for_instance(instance);
  assert(index != LWM2M_INVALID_INSTANCE);
  
  assert(Water_Flow_Readings_instances[index].id != LWM2M_INVALID_INSTANCE);
  
  
  *val = Water_Flow_Readings_instances[index].Interval_Start_Offset;
  Water_Flow_Readings_unlock();
  return COAP_205_CONTENT;
}
static uint8_t __Water_Flow_Readings_Interval_Start_Offset_get(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data)
{
  uint32_t val;
  uint8_t res = Water_Flow_Readings_Interval_Start_Offset_get(instance,  &val);
  lwm2m_data_encode_int(val, data);
  return res;
}
                                         
#endif
#if CONFIG_Water_Flow_Readings_Interval_Start_Offset_OPTION == 1
__attribute__((weak)) uint8_t Water_Flow_Readings_Interval_Start_Offset_set(uint16_t instance,  uint32_t val, bool notify)
{
  Water_Flow_Readings_lock();
  assert(instance >= 0 && instance < MAX_Water_Flow_Readings_INSTANCES);
  uint16_t index = Water_Flow_Readings_get_index_for_instance(instance);
  assert(index != LWM2M_INVALID_INSTANCE);
  
  assert(Water_Flow_Readings_instances[index].id != LWM2M_INVALID_INSTANCE);

  Water_Flow_Readings_instances[index].Interval_Start_Offset = val;
  
  if (notify && Water_Flow_Readings_instances[index].registered) lwm2m_object_base_resource_changed(&Water_Flow_Readings_lwm2m_object, instance, 9001, false);  
  Water_Flow_Readings_unlock();
  return COAP_204_CHANGED;
}
#endif
#if CONFIG_Water_Flow_Readings_Interval_UTC_Offset_OPTION == 1
  
__attribute__((weak))  uint8_t Water_Flow_Readings_Interval_UTC_Offset_get(uint16_t instance,  const char **val)
{
  Water_Flow_Readings_lock();
  assert(instance >= 0 && instance < MAX_Water_Flow_Readings_INSTANCES);
  uint16_t index = Water_Flow_Readings_get_index_for_instance(instance);
  assert(index != LWM2M_INVALID_INSTANCE);
  
  assert(Water_Flow_Readings_instances[index].id != LWM2M_INVALID_INSTANCE);
  
  
  *val = Water_Flow_Readings_instances[index].Interval_UTC_Offset;
  Water_Flow_Readings_unlock();
  return COAP_205_CONTENT;
}
static uint8_t __Water_Flow_Readings_Interval_UTC_Offset_get(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data)
{
  const char *val;
  uint8_t res = Water_Flow_Readings_Interval_UTC_Offset_get(instance,  &val);
  lwm2m_data_encode_string(val, data);
  return res;
}
                                         
#endif
#if CONFIG_Water_Flow_Readings_Interval_UTC_Offset_OPTION == 1
__attribute__((weak)) uint8_t Water_Flow_Readings_Interval_UTC_Offset_set(uint16_t instance,  const char *val, size_t length, bool notify)
{
  Water_Flow_Readings_lock();
  assert(instance >= 0 && instance < MAX_Water_Flow_Readings_INSTANCES);
  uint16_t index = Water_Flow_Readings_get_index_for_instance(instance);
  assert(index != LWM2M_INVALID_INSTANCE);
  
  assert(Water_Flow_Readings_instances[index].id != LWM2M_INVALID_INSTANCE);
 
  if (length >= sizeof(Water_Flow_Readings_instances[index].Interval_UTC_Offset )) length = sizeof(Water_Flow_Readings_instances[index].Interval_UTC_Offset) - 1;
  memcpy(Water_Flow_Readings_instances[index].Interval_UTC_Offset, val, length);
  Water_Flow_Readings_instances[index].Interval_UTC_Offset[length] = 0;
  
  
  if (notify && Water_Flow_Readings_instances[index].registered) lwm2m_object_base_resource_changed(&Water_Flow_Readings_lwm2m_object, instance, 9002, false);
  Water_Flow_Readings_unlock();
  return COAP_204_CHANGED;
}
#endif
#if CONFIG_Water_Flow_Readings_Interval_Collection_Start_Time_OPTION == 1
  
__attribute__((weak))  uint8_t Water_Flow_Readings_Interval_Collection_Start_Time_get(uint16_t instance,  time_t *val)
{
  Water_Flow_Readings_lock();
  uint16_t index = Water_Flow_Readings_get_index_for_instance(instance);
  assert(index != LWM2M_INVALID_INSTANCE);
  
  assert(Water_Flow_Readings_instances[index].id != LWM2M_INVALID_INSTANCE);
  
                                                                                           
  *val = Water_Flow_Readings_instances[index].Interval_Collection_Start_Time;
  Water_Flow_Readings_unlock();
  return COAP_205_CONTENT;
}
static uint8_t __Water_Flow_Readings_Interval_Collection_Start_Time_get(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data)
{
 time_t val;
 uint8_t res = Water_Flow_Readings_Interval_Collection_Start_Time_get(instance,  &val);
 lwm2m_data_encode_int(val, data);
 return res;
}
                                         
#endif
#if CONFIG_Water_Flow_Readings_Interval_Collection_Start_Time_OPTION == 1
__attribute__((weak)) uint8_t Water_Flow_Readings_Interval_Collection_Start_Time_set(uint16_t instance, time_t val, bool notify)
{
  Water_Flow_Readings_lock();
  assert(instance >= 0 && instance < MAX_Water_Flow_Readings_INSTANCES);
  uint16_t index = Water_Flow_Readings_get_index_for_instance(instance);
  assert(index != LWM2M_INVALID_INSTANCE);
  
  assert(Water_Flow_Readings_instances[index].id != LWM2M_INVALID_INSTANCE);

  Water_Flow_Readings_instances[index].Interval_Collection_Start_Time = val;   
  
  
  if (notify && Water_Flow_Readings_instances[index].registered) lwm2m_object_base_resource_changed(&Water_Flow_Readings_lwm2m_object, instance, 9003,false);
  Water_Flow_Readings_unlock();
  return COAP_204_CHANGED;
}
#endif
#if CONFIG_Water_Flow_Readings_Oldest_Recorded_Interval_OPTION == 1
  
__attribute__((weak))  uint8_t Water_Flow_Readings_Oldest_Recorded_Interval_get(uint16_t instance,  time_t *val)
{
  Water_Flow_Readings_lock();
  uint16_t index = Water_Flow_Readings_get_index_for_instance(instance);
  assert(index != LWM2M_INVALID_INSTANCE);
  
  assert(Water_Flow_Readings_instances[index].id != LWM2M_INVALID_INSTANCE);
  
                                                                                           
  *val = Water_Flow_Readings_instances[index].Oldest_Recorded_Interval;
  Water_Flow_Readings_unlock();
  return COAP_205_CONTENT;
}
static uint8_t __Water_Flow_Readings_Oldest_Recorded_Interval_get(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data)
{
 time_t val;
 uint8_t res = Water_Flow_Readings_Oldest_Recorded_Interval_get(instance,  &val);
 lwm2m_data_encode_int(val, data);
 return res;
}
                                         
#endif
#if CONFIG_Water_Flow_Readings_Oldest_Recorded_Interval_OPTION == 1
__attribute__((weak)) uint8_t Water_Flow_Readings_Oldest_Recorded_Interval_set(uint16_t instance, time_t val, bool notify)
{
  Water_Flow_Readings_lock();
  assert(instance >= 0 && instance < MAX_Water_Flow_Readings_INSTANCES);
  uint16_t index = Water_Flow_Readings_get_index_for_instance(instance);
  assert(index != LWM2M_INVALID_INSTANCE);
  
  assert(Water_Flow_Readings_instances[index].id != LWM2M_INVALID_INSTANCE);

  Water_Flow_Readings_instances[index].Oldest_Recorded_Interval = val;   
  
  
  if (notify && Water_Flow_Readings_instances[index].registered) lwm2m_object_base_resource_changed(&Water_Flow_Readings_lwm2m_object, instance, 9004,false);
  Water_Flow_Readings_unlock();
  return COAP_204_CHANGED;
}
#endif
#if CONFIG_Water_Flow_Readings_Last_Delivered_Interval_OPTION == 1
  
__attribute__((weak))  uint8_t Water_Flow_Readings_Last_Delivered_Interval_get(uint16_t instance,  time_t *val)
{
  Water_Flow_Readings_lock();
  uint16_t index = Water_Flow_Readings_get_index_for_instance(instance);
  assert(index != LWM2M_INVALID_INSTANCE);
  
  assert(Water_Flow_Readings_instances[index].id != LWM2M_INVALID_INSTANCE);
  
                                                                                           
  *val = Water_Flow_Readings_instances[index].Last_Delivered_Interval;
  Water_Flow_Readings_unlock();
  return COAP_205_CONTENT;
}
static uint8_t __Water_Flow_Readings_Last_Delivered_Interval_get(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data)
{
 time_t val;
 uint8_t res = Water_Flow_Readings_Last_Delivered_Interval_get(instance,  &val);
 lwm2m_data_encode_int(val, data);
 return res;
}
                                         
#endif
#if CONFIG_Water_Flow_Readings_Last_Delivered_Interval_OPTION == 1
__attribute__((weak)) uint8_t Water_Flow_Readings_Last_Delivered_Interval_set(uint16_t instance, time_t val, bool notify)
{
  Water_Flow_Readings_lock();
  assert(instance >= 0 && instance < MAX_Water_Flow_Readings_INSTANCES);
  uint16_t index = Water_Flow_Readings_get_index_for_instance(instance);
  assert(index != LWM2M_INVALID_INSTANCE);
  
  assert(Water_Flow_Readings_instances[index].id != LWM2M_INVALID_INSTANCE);

  Water_Flow_Readings_instances[index].Last_Delivered_Interval = val;   
  
  
  if (notify && Water_Flow_Readings_instances[index].registered) lwm2m_object_base_resource_changed(&Water_Flow_Readings_lwm2m_object, instance, 9005,false);
  Water_Flow_Readings_unlock();
  return COAP_204_CHANGED;
}
static uint8_t __Water_Flow_Readings_Last_Delivered_Interval_set(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data)
{
  time_t val;
  int64_t tmp;
  lwm2m_data_decode_int(data, &tmp);
  val = (time_t)tmp;
  return Water_Flow_Readings_Last_Delivered_Interval_set(instance,  val, false);
}
#endif
#if CONFIG_Water_Flow_Readings_Last_Delivered_Interval_OPTION == 1
__attribute__((weak)) uint8_t Water_Flow_Readings_Last_Delivered_Interval_changed(uint16_t instance)
{
  Water_Flow_Readings_lock();
  assert(instance >= 0 && instance < MAX_Water_Flow_Readings_INSTANCES);
  uint16_t index = Water_Flow_Readings_get_index_for_instance(instance);
  assert(index != LWM2M_INVALID_INSTANCE);
  
  Water_Flow_Readings_changed(instance);
  Water_Flow_Readings_unlock();
  return COAP_NO_ERROR;
}
#endif
#if CONFIG_Water_Flow_Readings_Latest_Recorded_Interval_OPTION == 1
  
__attribute__((weak))  uint8_t Water_Flow_Readings_Latest_Recorded_Interval_get(uint16_t instance,  time_t *val)
{
  Water_Flow_Readings_lock();
  uint16_t index = Water_Flow_Readings_get_index_for_instance(instance);
  assert(index != LWM2M_INVALID_INSTANCE);
  
  assert(Water_Flow_Readings_instances[index].id != LWM2M_INVALID_INSTANCE);
  
                                                                                           
  *val = Water_Flow_Readings_instances[index].Latest_Recorded_Interval;
  Water_Flow_Readings_unlock();
  return COAP_205_CONTENT;
}
static uint8_t __Water_Flow_Readings_Latest_Recorded_Interval_get(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data)
{
 time_t val;
 uint8_t res = Water_Flow_Readings_Latest_Recorded_Interval_get(instance,  &val);
 lwm2m_data_encode_int(val, data);
 return res;
}
                                         
#endif
#if CONFIG_Water_Flow_Readings_Latest_Recorded_Interval_OPTION == 1
__attribute__((weak)) uint8_t Water_Flow_Readings_Latest_Recorded_Interval_set(uint16_t instance, time_t val, bool notify)
{
  Water_Flow_Readings_lock();
  assert(instance >= 0 && instance < MAX_Water_Flow_Readings_INSTANCES);
  uint16_t index = Water_Flow_Readings_get_index_for_instance(instance);
  assert(index != LWM2M_INVALID_INSTANCE);
  
  assert(Water_Flow_Readings_instances[index].id != LWM2M_INVALID_INSTANCE);

  Water_Flow_Readings_instances[index].Latest_Recorded_Interval = val;   
  
  
  if (notify && Water_Flow_Readings_instances[index].registered) lwm2m_object_base_resource_changed(&Water_Flow_Readings_lwm2m_object, instance, 9006,false);
  Water_Flow_Readings_unlock();
  return COAP_204_CHANGED;
}
#endif
#if CONFIG_Water_Flow_Readings_Interval_Delivery_Midnight_Aligned_OPTION == 1
  
                                         
__attribute__((weak)) uint8_t Water_Flow_Readings_Interval_Delivery_Midnight_Aligned_get(uint16_t instance,  bool *val)
{
  Water_Flow_Readings_lock();
  assert(instance >= 0 && instance < MAX_Water_Flow_Readings_INSTANCES);
  uint16_t index = Water_Flow_Readings_get_index_for_instance(instance);
  assert(index != LWM2M_INVALID_INSTANCE);
  
  assert(Water_Flow_Readings_instances[index].id != LWM2M_INVALID_INSTANCE);
  
  
  *val = Water_Flow_Readings_instances[index].Interval_Delivery_Midnight_Aligned;
  Water_Flow_Readings_unlock();
  return COAP_205_CONTENT;
}
static uint8_t __Water_Flow_Readings_Interval_Delivery_Midnight_Aligned_get(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data)
{
  bool val;
  uint8_t res = Water_Flow_Readings_Interval_Delivery_Midnight_Aligned_get(instance,  &val);
  lwm2m_data_encode_bool(val, data);
  return res;
}
#endif
#if CONFIG_Water_Flow_Readings_Interval_Delivery_Midnight_Aligned_OPTION == 1
__attribute__((weak)) uint8_t Water_Flow_Readings_Interval_Delivery_Midnight_Aligned_set(uint16_t instance,  bool val, bool notify)
{
  Water_Flow_Readings_lock();
  assert(instance >= 0 && instance < MAX_Water_Flow_Readings_INSTANCES);
  uint16_t index = Water_Flow_Readings_get_index_for_instance(instance);
  assert(index != LWM2M_INVALID_INSTANCE);
  
  assert(Water_Flow_Readings_instances[index].id != LWM2M_INVALID_INSTANCE);

  Water_Flow_Readings_instances[index].Interval_Delivery_Midnight_Aligned = val;  
  
  
  if (notify && Water_Flow_Readings_instances[index].registered) lwm2m_object_base_resource_changed(&Water_Flow_Readings_lwm2m_object, instance, 9007, false);
  Water_Flow_Readings_unlock();
  return COAP_204_CHANGED;
}
static uint8_t __Water_Flow_Readings_Interval_Delivery_Midnight_Aligned_set(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data)
{
  bool val;
  lwm2m_data_decode_bool(data, &val);
  return Water_Flow_Readings_Interval_Delivery_Midnight_Aligned_set(instance, val, false);
}
#endif
#if CONFIG_Water_Flow_Readings_Interval_Delivery_Midnight_Aligned_OPTION == 1
__attribute__((weak)) uint8_t Water_Flow_Readings_Interval_Delivery_Midnight_Aligned_changed(uint16_t instance)
{
  Water_Flow_Readings_lock();
  assert(instance >= 0 && instance < MAX_Water_Flow_Readings_INSTANCES);
  uint16_t index = Water_Flow_Readings_get_index_for_instance(instance);
  assert(index != LWM2M_INVALID_INSTANCE);
  
  Water_Flow_Readings_changed(instance);
  Water_Flow_Readings_unlock();
  return COAP_NO_ERROR;
}
#endif
#if CONFIG_Water_Flow_Readings_Interval_Historical_Read_OPTION == 1
__attribute__((weak)) uint8_t Water_Flow_Readings_Interval_Historical_Read_execute(lwm2m_object_t *object, uint16_t instance, uint16_t resource_id, uint8_t * buffer, int length)
{
  return COAP_405_METHOD_NOT_ALLOWED;
}
#endif
#if CONFIG_Water_Flow_Readings_Interval_Historical_Read_Payload_OPTION == 1
  
__attribute__((weak))  uint8_t Water_Flow_Readings_Interval_Historical_Read_Payload_get(uint16_t instance,  uint8_t **val, size_t *length)
{
  Water_Flow_Readings_lock();
  assert(instance >= 0 && instance < MAX_Water_Flow_Readings_INSTANCES);
  uint16_t index = Water_Flow_Readings_get_index_for_instance(instance);
  assert(index != LWM2M_INVALID_INSTANCE);
  
  assert(Water_Flow_Readings_instances[index].id != LWM2M_INVALID_INSTANCE);
  
  
  
  *val = Water_Flow_Readings_instances[index].Interval_Historical_Read_Payload;
  *length = Water_Flow_Readings_instances[index].Interval_Historical_Read_Payload_length;
  Water_Flow_Readings_unlock();
  return COAP_205_CONTENT;
}
static uint8_t __Water_Flow_Readings_Interval_Historical_Read_Payload_get(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data)
{
  uint8_t *val;
  size_t length = 0;
  
  uint8_t res = Water_Flow_Readings_Interval_Historical_Read_Payload_get(instance,  &val, &length);
  
  lwm2m_data_encode_opaque(val, length, data);
  return res;
}
                                         
#endif
#if CONFIG_Water_Flow_Readings_Interval_Historical_Read_Payload_OPTION == 1
 __attribute__((weak)) uint8_t Water_Flow_Readings_Interval_Historical_Read_Payload_set(uint16_t instance,  uint8_t* val, size_t length, bool notify)
 {
   Water_Flow_Readings_lock();
   assert(instance >= 0 && instance < MAX_Water_Flow_Readings_INSTANCES);
 
   uint16_t index = Water_Flow_Readings_get_index_for_instance(instance);
   assert(index != LWM2M_INVALID_INSTANCE);
   
   if (Water_Flow_Readings_instances[index].Interval_Historical_Read_Payload != 0) lwm2m_free(Water_Flow_Readings_instances[index].Interval_Historical_Read_Payload);
   Water_Flow_Readings_instances[index].Interval_Historical_Read_Payload = lwm2m_malloc(length);
   if (Water_Flow_Readings_instances[index].Interval_Historical_Read_Payload == 0)
   {
     Water_Flow_Readings_instances[index].Interval_Historical_Read_Payload_length  = 0;   
     Water_Flow_Readings_unlock();
     return COAP_500_INTERNAL_SERVER_ERROR;                                     
   } 
   Water_Flow_Readings_instances[index].Interval_Historical_Read_Payload_length  = length;
   memcpy(Water_Flow_Readings_instances[index].Interval_Historical_Read_Payload, val, length);                                      
                                  
   
  
   if (notify && Water_Flow_Readings_instances[index].registered) lwm2m_object_base_resource_changed(&Water_Flow_Readings_lwm2m_object, instance, 9009, false);
  
  
   Water_Flow_Readings_unlock();
   return COAP_204_CHANGED;
 }
#endif
#if CONFIG_Water_Flow_Readings_Interval_Change_Configuration_OPTION == 1
__attribute__((weak)) uint8_t Water_Flow_Readings_Interval_Change_Configuration_execute(lwm2m_object_t *object, uint16_t instance, uint16_t resource_id, uint8_t * buffer, int length)
{
  return COAP_405_METHOD_NOT_ALLOWED;
}
#endif
#if CONFIG_Water_Flow_Readings_Start_OPTION == 1
__attribute__((weak)) uint8_t Water_Flow_Readings_Start_execute(lwm2m_object_t *object, uint16_t instance, uint16_t resource_id, uint8_t * buffer, int length)
{
  return COAP_405_METHOD_NOT_ALLOWED;
}
#endif
#if CONFIG_Water_Flow_Readings_Stop_OPTION == 1
__attribute__((weak)) uint8_t Water_Flow_Readings_Stop_execute(lwm2m_object_t *object, uint16_t instance, uint16_t resource_id, uint8_t * buffer, int length)
{
  return COAP_405_METHOD_NOT_ALLOWED;
}
#endif
#if CONFIG_Water_Flow_Readings_Status_OPTION == 1
  
__attribute__((weak)) uint8_t Water_Flow_Readings_Status_get(uint16_t instance,  uint32_t *val)
{
  Water_Flow_Readings_lock();
  assert(instance >= 0 && instance < MAX_Water_Flow_Readings_INSTANCES);
  uint16_t index = Water_Flow_Readings_get_index_for_instance(instance);
  assert(index != LWM2M_INVALID_INSTANCE);
  
  assert(Water_Flow_Readings_instances[index].id != LWM2M_INVALID_INSTANCE);
  
  
  *val = Water_Flow_Readings_instances[index].Status;
  Water_Flow_Readings_unlock();
  return COAP_205_CONTENT;
}
static uint8_t __Water_Flow_Readings_Status_get(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data)
{
  uint32_t val;
  uint8_t res = Water_Flow_Readings_Status_get(instance,  &val);
  lwm2m_data_encode_int(val, data);
  return res;
}
                                         
#endif
#if CONFIG_Water_Flow_Readings_Status_OPTION == 1
__attribute__((weak)) uint8_t Water_Flow_Readings_Status_set(uint16_t instance,  uint32_t val, bool notify)
{
  Water_Flow_Readings_lock();
  assert(instance >= 0 && instance < MAX_Water_Flow_Readings_INSTANCES);
  uint16_t index = Water_Flow_Readings_get_index_for_instance(instance);
  assert(index != LWM2M_INVALID_INSTANCE);
  
  assert(Water_Flow_Readings_instances[index].id != LWM2M_INVALID_INSTANCE);

  Water_Flow_Readings_instances[index].Status = val;
  
  if (notify && Water_Flow_Readings_instances[index].registered) lwm2m_object_base_resource_changed(&Water_Flow_Readings_lwm2m_object, instance, 4013, false);  
  Water_Flow_Readings_unlock();
  return COAP_204_CHANGED;
}
#endif
#if CONFIG_Water_Flow_Readings_Latest_Payload_OPTION == 1
  
__attribute__((weak))  uint8_t Water_Flow_Readings_Latest_Payload_get(uint16_t instance,  uint8_t **val, size_t *length)
{
  Water_Flow_Readings_lock();
  assert(instance >= 0 && instance < MAX_Water_Flow_Readings_INSTANCES);
  uint16_t index = Water_Flow_Readings_get_index_for_instance(instance);
  assert(index != LWM2M_INVALID_INSTANCE);
  
  assert(Water_Flow_Readings_instances[index].id != LWM2M_INVALID_INSTANCE);
  
  
  
  *val = Water_Flow_Readings_instances[index].Latest_Payload;
  *length = Water_Flow_Readings_instances[index].Latest_Payload_length;
  Water_Flow_Readings_unlock();
  return COAP_205_CONTENT;
}
static uint8_t __Water_Flow_Readings_Latest_Payload_get(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data)
{
  uint8_t *val;
  size_t length = 0;
  
  uint8_t res = Water_Flow_Readings_Latest_Payload_get(instance,  &val, &length);
  
  lwm2m_data_encode_opaque(val, length, data);
  return res;
}
                                         
#endif
#if CONFIG_Water_Flow_Readings_Latest_Payload_OPTION == 1
 __attribute__((weak)) uint8_t Water_Flow_Readings_Latest_Payload_set(uint16_t instance,  uint8_t* val, size_t length, bool notify)
 {
   Water_Flow_Readings_lock();
   assert(instance >= 0 && instance < MAX_Water_Flow_Readings_INSTANCES);
 
   uint16_t index = Water_Flow_Readings_get_index_for_instance(instance);
   assert(index != LWM2M_INVALID_INSTANCE);
   
   if (Water_Flow_Readings_instances[index].Latest_Payload != 0) lwm2m_free(Water_Flow_Readings_instances[index].Latest_Payload);
   Water_Flow_Readings_instances[index].Latest_Payload = lwm2m_malloc(length);
   if (Water_Flow_Readings_instances[index].Latest_Payload == 0)
   {
     Water_Flow_Readings_instances[index].Latest_Payload_length  = 0;   
     Water_Flow_Readings_unlock();
     return COAP_500_INTERNAL_SERVER_ERROR;                                     
   } 
   Water_Flow_Readings_instances[index].Latest_Payload_length  = length;
   memcpy(Water_Flow_Readings_instances[index].Latest_Payload, val, length);                                      
                                  
   
  
   if (notify && Water_Flow_Readings_instances[index].registered) lwm2m_object_base_resource_changed(&Water_Flow_Readings_lwm2m_object, instance, 4014, false);
  
  
   Water_Flow_Readings_unlock();
   return COAP_204_CHANGED;
 }
#endif
