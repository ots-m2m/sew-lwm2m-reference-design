/*
 * Test_Water_Meter_base.c
 * 
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * This code was generated using gt-object-gen provided by Operational 
 * Technology Solutions M2M Pty. Ltd. For licensing enquires visit 
 * ot-solutions.com.au.
 */
#include "Test_Water_Meter_base.h"

#include <string.h>
#include <stdbool.h>

#include "mutex.h"
#define assert(condition) if (!(condition)) { Test_Water_Meter_unlock(); return COAP_406_NOT_ACCEPTABLE; }
#define assert_void(condition) if (!(condition)) { Test_Water_Meter_unlock(); return; }

uint8_t __Test_Water_Meter_create_instance(uint16_t instance_id);
static uint8_t __Test_Water_Meter_Water_Pressure_get(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data);
  
static uint8_t __Test_Water_Meter_Water_Pressure_set(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data);

static lwm2m_object_t Test_Water_Meter_lwm2m_object = {
  .objID = 9024
};

lwm2m_object_t *Test_Water_Meter_get_lwm2m_object(void)
{
  return &Test_Water_Meter_lwm2m_object;
}

typedef struct Test_Water_Meter_instance_t
{
  struct Test_Water_Meter_instance * next;   // matches lwm2m_list_t::next
  uint16_t id;                                // matches lwm2m_list_t::id
  uint16_t index;
  bool registered;
  void *user_data;
  float Water_Pressure;
}
Test_Water_Meter_instance;

static Test_Water_Meter_instance Test_Water_Meter_instances[MAX_Test_Water_Meter_INSTANCES];

lwm2m_object_declaration_t Test_Water_Meter_object_declaration = {
  .id = 9024,
  .delete = Test_Water_Meter_delete_instance,
  .create = __Test_Water_Meter_create_instance,
  .resources[0] = { 
    .supported = true,  
    .id = 0,
    .operation = LWM2M_OPERATION_RW,
    .instances = LWM2M_INSTANCETYPE_single,
    .mandatory = true,
    .type = LWM2M_TYPE_float,
    .changed = Test_Water_Meter_Water_Pressure_changed,
    .execute = (void *) 0,
    .get = __Test_Water_Meter_Water_Pressure_get,
    .set = __Test_Water_Meter_Water_Pressure_set,
  },
};


void Test_Water_Meter_lock() 
{ 
  mutex_lock(&Test_Water_Meter_object_declaration.lock, MUTEXT_WAIT_FOREVER); 
}
void Test_Water_Meter_unlock() 
{ 
  mutex_unlock(&Test_Water_Meter_object_declaration.lock);
}


uint16_t Test_Water_Meter_get_index_for_instance(uint16_t instance)
{
  for (int i = 0; i < MAX_Test_Water_Meter_INSTANCES; i++)
  {
    if (Test_Water_Meter_instances[i].id == instance) return i;
  }
  return LWM2M_INVALID_INSTANCE;
}

__attribute__((weak)) lwm2m_object_t *Test_Water_Meter_initialise(void)
{
  mutex_initialise(&Test_Water_Meter_object_declaration.lock);
  
  memset(Test_Water_Meter_instances, 0, sizeof(Test_Water_Meter_instances));
  for (int i = 0; i < MAX_Test_Water_Meter_INSTANCES; i++)
  {
    Test_Water_Meter_instances[i].id = LWM2M_INVALID_INSTANCE;
    Test_Water_Meter_instances[i].index = i;
  }
  lwm2m_object_base_initialse_object(&Test_Water_Meter_lwm2m_object);
  Test_Water_Meter_lwm2m_object.userData = &Test_Water_Meter_object_declaration; 
  return &Test_Water_Meter_lwm2m_object;
}
  
__attribute__((weak)) void Test_Water_Meter_register_object(lwm2m_context_t *context)
{
  Test_Water_Meter_lock();
  Test_Water_Meter_object_declaration.context = context;
  Test_Water_Meter_unlock();
}
  
__attribute__((weak)) void Test_Water_Meter_register_instance(uint16_t instance_id)
{
  Test_Water_Meter_lock();
  uint16_t index = Test_Water_Meter_get_index_for_instance(instance_id);
  assert_void(index != LWM2M_INVALID_INSTANCE);
  
  if ((Test_Water_Meter_instances[index].id !=  LWM2M_INVALID_INSTANCE) && (!Test_Water_Meter_instances[index].registered))
  {
    Test_Water_Meter_lwm2m_object.instanceList = LWM2M_LIST_ADD(Test_Water_Meter_lwm2m_object.instanceList, (lwm2m_list_t *)&Test_Water_Meter_instances[index]);
    Test_Water_Meter_instances[index].registered = true;
  }
  Test_Water_Meter_unlock();
}

__attribute__((weak)) void Test_Water_Meter_unregister_instance(uint16_t instance_id)
{
  Test_Water_Meter_lock();
  uint16_t index = Test_Water_Meter_get_index_for_instance(instance_id);
  assert_void(index != LWM2M_INVALID_INSTANCE);
  
  if ((Test_Water_Meter_instances[index].id !=  LWM2M_INVALID_INSTANCE) && (Test_Water_Meter_instances[index].registered))
  {
    Test_Water_Meter_instance *instance = 0;
    Test_Water_Meter_lwm2m_object.instanceList = lwm2m_list_remove(Test_Water_Meter_lwm2m_object.instanceList, instance_id, (lwm2m_list_t **)&instance);
    Test_Water_Meter_instances[index].registered = false;
    if (instance != 0)
    { 
      instance->next = 0;
    }
  }
  Test_Water_Meter_unlock();
}

//override this to perform actions when a new instance is created
__attribute__((weak)) void Test_Water_Meter_instance_created(uint16_t instance_id)
{
  
}
  

//called when the server requests an object to be created; we need to create AND register the 
//instance
uint8_t __Test_Water_Meter_create_instance(uint16_t instance_id)
{
  uint8_t res = Test_Water_Meter_create_instance(instance_id);
  if (res == COAP_201_CREATED)
  {
    Test_Water_Meter_register_instance(instance_id);
  }
  return res;
}
  
__attribute__((weak)) uint8_t Test_Water_Meter_create_instance(uint16_t instance_id)
{
  Test_Water_Meter_lock();
  //find an unallocated instance
  for (int i = 0; i < MAX_Test_Water_Meter_INSTANCES; i++)
  {
    if (Test_Water_Meter_instances[i].id == LWM2M_INVALID_INSTANCE)
    {
      Test_Water_Meter_instances[i].id = instance_id;
      Test_Water_Meter_instance_created(instance_id);
      Test_Water_Meter_unlock();
      return COAP_201_CREATED;
    }
  }
  Test_Water_Meter_unlock();
  return COAP_500_INTERNAL_SERVER_ERROR; //none found
}

void Test_Water_Meter_foreach(lwm2m_each_instance_func func, void *data)
{
  Test_Water_Meter_lock();
  
  for (int i = 0; i < MAX_Test_Water_Meter_INSTANCES; i++)
  {
    if (Test_Water_Meter_instances[i].id != LWM2M_INVALID_INSTANCE)
    {
      if (!func(Test_Water_Meter_instances[i].id, data)) break;
    }
  }
  Test_Water_Meter_unlock();
}
  
//provides access to a user data pointer for storing additional instance
//data.
void *Test_Water_Meter_get_user_data(uint16_t instance_id)
{
  uint16_t index = Test_Water_Meter_get_index_for_instance(instance_id);
  if (index == LWM2M_INVALID_INSTANCE) return 0;
  return Test_Water_Meter_instances[index].user_data;
}
  
bool Test_Water_Meter_set_user_data(uint16_t instance_id, void *data)
{
  uint16_t index = Test_Water_Meter_get_index_for_instance(instance_id);
  if (index == LWM2M_INVALID_INSTANCE) return false;
  Test_Water_Meter_instances[index].user_data = data;
  return true;
}
  
//override this to perform actions when a new instance is created
__attribute__((weak)) void Test_Water_Meter_instance_deleted(uint16_t instance_id)
{
  
}

__attribute__((weak)) uint8_t Test_Water_Meter_delete_instance(uint16_t instance_id)
{
  Test_Water_Meter_lock();
  uint16_t index = Test_Water_Meter_get_index_for_instance(instance_id);
  assert(index != LWM2M_INVALID_INSTANCE);
  
  if ((instance_id < MAX_Test_Water_Meter_INSTANCES) && (Test_Water_Meter_instances[index].id !=  LWM2M_INVALID_INSTANCE))
  {
    Test_Water_Meter_unregister_instance(instance_id);
    Test_Water_Meter_instance_deleted(instance_id);
    Test_Water_Meter_instances[index].id = LWM2M_INVALID_INSTANCE;
  }
  Test_Water_Meter_unlock();
  return COAP_202_DELETED;
}


__attribute__((weak)) void Test_Water_Meter_changed(uint16_t instance)
{
  
}

  
__attribute__((weak))  uint8_t Test_Water_Meter_Water_Pressure_get(uint16_t instance,  double *val)
{
  Test_Water_Meter_lock();
  assert(instance >= 0 && instance < MAX_Test_Water_Meter_INSTANCES); 
  uint16_t index = Test_Water_Meter_get_index_for_instance(instance);
  assert(index != LWM2M_INVALID_INSTANCE);
  
  assert(Test_Water_Meter_instances[index].id != LWM2M_INVALID_INSTANCE);
  
  
  *val = Test_Water_Meter_instances[index].Water_Pressure;
  Test_Water_Meter_unlock();
  return COAP_205_CONTENT;
}
static uint8_t __Test_Water_Meter_Water_Pressure_get(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data)
{
  double val;
  uint8_t res = Test_Water_Meter_Water_Pressure_get(instance,  &val);
  lwm2m_data_encode_float(val, data);
  return res;
}
                                         
__attribute__((weak)) uint8_t Test_Water_Meter_Water_Pressure_set(uint16_t instance,  double val, bool notify)
{
  Test_Water_Meter_lock();
  assert(instance >= 0 && instance < MAX_Test_Water_Meter_INSTANCES);
  uint16_t index = Test_Water_Meter_get_index_for_instance(instance);
  assert(index != LWM2M_INVALID_INSTANCE);
  
  assert(Test_Water_Meter_instances[index].id != LWM2M_INVALID_INSTANCE);
 
  Test_Water_Meter_instances[index].Water_Pressure = val;
  
  
  if (notify && Test_Water_Meter_instances[index].registered) lwm2m_object_base_resource_changed(&Test_Water_Meter_lwm2m_object, instance, 0, false);
  Test_Water_Meter_unlock();
  return COAP_204_CHANGED;
}
static uint8_t __Test_Water_Meter_Water_Pressure_set(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data)
{
  double val;
  lwm2m_data_decode_float(data, &val);
  return Test_Water_Meter_Water_Pressure_set(instance, val, false);
}
__attribute__((weak)) uint8_t Test_Water_Meter_Water_Pressure_changed(uint16_t instance)
{
  Test_Water_Meter_lock();
  assert(instance >= 0 && instance < MAX_Test_Water_Meter_INSTANCES);
  uint16_t index = Test_Water_Meter_get_index_for_instance(instance);
  assert(index != LWM2M_INVALID_INSTANCE);
  
  Test_Water_Meter_changed(instance);
  Test_Water_Meter_unlock();
  return COAP_NO_ERROR;
}
