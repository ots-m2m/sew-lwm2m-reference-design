/*
 * LWM2M_Server_base.c
 * 
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * This code was generated using gt-object-gen provided by Operational 
 * Technology Solutions M2M Pty. Ltd. For licensing enquires visit 
 * ot-solutions.com.au.
 */
#include "LWM2M_Server_base.h"

#include <string.h>
#include <stdbool.h>

#include "mutex.h"
#define assert(condition) if (!(condition)) { LWM2M_Server_unlock(); return COAP_406_NOT_ACCEPTABLE; }
#define assert_void(condition) if (!(condition)) { LWM2M_Server_unlock(); return; }

uint8_t __LWM2M_Server_create_instance(uint16_t instance_id);
static uint8_t __LWM2M_Server_Short_Server_ID_get(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data);
  
static uint8_t __LWM2M_Server_Short_Server_ID_set(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data);
static uint8_t __LWM2M_Server_Lifetime_get(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data);
  
static uint8_t __LWM2M_Server_Lifetime_set(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data);
#if CONFIG_LWM2M_Server_Default_Minimum_Period_OPTION == 1
static uint8_t __LWM2M_Server_Default_Minimum_Period_get(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data);
  
#endif
#if CONFIG_LWM2M_Server_Default_Minimum_Period_OPTION == 1
static uint8_t __LWM2M_Server_Default_Minimum_Period_set(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data);
#endif
#if CONFIG_LWM2M_Server_Default_Maximum_Period_OPTION == 1
static uint8_t __LWM2M_Server_Default_Maximum_Period_get(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data);
  
#endif
#if CONFIG_LWM2M_Server_Default_Maximum_Period_OPTION == 1
static uint8_t __LWM2M_Server_Default_Maximum_Period_set(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data);
#endif
#if CONFIG_LWM2M_Server_Disable_Timeout_OPTION == 1
static uint8_t __LWM2M_Server_Disable_Timeout_get(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data);
  
#endif
#if CONFIG_LWM2M_Server_Disable_Timeout_OPTION == 1
static uint8_t __LWM2M_Server_Disable_Timeout_set(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data);
#endif
static uint8_t __LWM2M_Server_Notification_Storing_When_Disabled_or_Offline_get(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data);
  
static uint8_t __LWM2M_Server_Notification_Storing_When_Disabled_or_Offline_set(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data);
static uint8_t __LWM2M_Server_Binding_get(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data);
  
static uint8_t __LWM2M_Server_Binding_set(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data);

static lwm2m_object_t LWM2M_Server_lwm2m_object = {
  .objID = 1
};

lwm2m_object_t *LWM2M_Server_get_lwm2m_object(void)
{
  return &LWM2M_Server_lwm2m_object;
}

typedef struct LWM2M_Server_instance_t
{
  struct LWM2M_Server_instance * next;   // matches lwm2m_list_t::next
  uint16_t id;                                // matches lwm2m_list_t::id
  uint16_t index;
  bool registered;
  void *user_data;
  uint32_t Short_Server_ID;
  uint32_t Lifetime;
#if CONFIG_LWM2M_Server_Default_Minimum_Period_OPTION == 1
  uint32_t Default_Minimum_Period;
#endif
#if CONFIG_LWM2M_Server_Default_Maximum_Period_OPTION == 1
  uint32_t Default_Maximum_Period;
#endif
#if CONFIG_LWM2M_Server_Disable_Timeout_OPTION == 1
  uint32_t Disable_Timeout;
#endif
  bool Notification_Storing_When_Disabled_or_Offline;
  char Binding [MAX_LWM2M_Server_Binding];
}
LWM2M_Server_instance;

static LWM2M_Server_instance LWM2M_Server_instances[MAX_LWM2M_Server_INSTANCES];

lwm2m_object_declaration_t LWM2M_Server_object_declaration = {
  .id = 1,
  .delete = LWM2M_Server_delete_instance,
  .create = __LWM2M_Server_create_instance,
  .resources[0] = { 
    .supported = true,  
    .id = 0,
    .operation = LWM2M_OPERATION_RW,
    .instances = LWM2M_INSTANCETYPE_single,
    .mandatory = true,
    .type = LWM2M_TYPE_integer,
    .changed = LWM2M_Server_Short_Server_ID_changed,
    .execute = (void *) 0,
    .get = __LWM2M_Server_Short_Server_ID_get,
    .set = __LWM2M_Server_Short_Server_ID_set,
  },
  .resources[1] = { 
    .supported = true,  
    .id = 1,
    .operation = LWM2M_OPERATION_RW,
    .instances = LWM2M_INSTANCETYPE_single,
    .mandatory = true,
    .type = LWM2M_TYPE_integer,
    .changed = LWM2M_Server_Lifetime_changed,
    .execute = (void *) 0,
    .get = __LWM2M_Server_Lifetime_get,
    .set = __LWM2M_Server_Lifetime_set,
  },
#if CONFIG_LWM2M_Server_Default_Minimum_Period_OPTION == 1
  .resources[2] = { 
    .supported = true,  
    .id = 2,
    .operation = LWM2M_OPERATION_RW,
    .instances = LWM2M_INSTANCETYPE_single,
    .mandatory = false,
    .type = LWM2M_TYPE_integer,
    .changed = LWM2M_Server_Default_Minimum_Period_changed,
    .execute = (void *) 0,
    .get = __LWM2M_Server_Default_Minimum_Period_get,
    .set = __LWM2M_Server_Default_Minimum_Period_set,
  },
#else
  .resources[2] = { .supported = false },
#endif
#if CONFIG_LWM2M_Server_Default_Maximum_Period_OPTION == 1
  .resources[3] = { 
    .supported = true,  
    .id = 3,
    .operation = LWM2M_OPERATION_RW,
    .instances = LWM2M_INSTANCETYPE_single,
    .mandatory = false,
    .type = LWM2M_TYPE_integer,
    .changed = LWM2M_Server_Default_Maximum_Period_changed,
    .execute = (void *) 0,
    .get = __LWM2M_Server_Default_Maximum_Period_get,
    .set = __LWM2M_Server_Default_Maximum_Period_set,
  },
#else
  .resources[3] = { .supported = false },
#endif
#if CONFIG_LWM2M_Server_Disable_OPTION == 1
  .resources[4] = { 
    .supported = true,  
    .id = 4,
    .operation = LWM2M_OPERATION_E,
    .instances = LWM2M_INSTANCETYPE_single,
    .mandatory = false,
    .type = LWM2M_TYPE_string,
    .changed = (void *) 0,
    .execute = LWM2M_Server_Disable_execute,
    .get = (void *) 0,
    .set = (void *) 0,
  },
#else
  .resources[4] = { .supported = false },
#endif
#if CONFIG_LWM2M_Server_Disable_Timeout_OPTION == 1
  .resources[5] = { 
    .supported = true,  
    .id = 5,
    .operation = LWM2M_OPERATION_RW,
    .instances = LWM2M_INSTANCETYPE_single,
    .mandatory = false,
    .type = LWM2M_TYPE_integer,
    .changed = LWM2M_Server_Disable_Timeout_changed,
    .execute = (void *) 0,
    .get = __LWM2M_Server_Disable_Timeout_get,
    .set = __LWM2M_Server_Disable_Timeout_set,
  },
#else
  .resources[5] = { .supported = false },
#endif
  .resources[6] = { 
    .supported = true,  
    .id = 6,
    .operation = LWM2M_OPERATION_RW,
    .instances = LWM2M_INSTANCETYPE_single,
    .mandatory = true,
    .type = LWM2M_TYPE_boolean,
    .changed = LWM2M_Server_Notification_Storing_When_Disabled_or_Offline_changed,
    .execute = (void *) 0,
    .get = __LWM2M_Server_Notification_Storing_When_Disabled_or_Offline_get,
    .set = __LWM2M_Server_Notification_Storing_When_Disabled_or_Offline_set,
  },
  .resources[7] = { 
    .supported = true,  
    .id = 7,
    .operation = LWM2M_OPERATION_RW,
    .instances = LWM2M_INSTANCETYPE_single,
    .mandatory = true,
    .type = LWM2M_TYPE_string,
    .changed = LWM2M_Server_Binding_changed,
    .execute = (void *) 0,
    .get = __LWM2M_Server_Binding_get,
    .set = __LWM2M_Server_Binding_set,
  },
  .resources[8] = { 
    .supported = true,  
    .id = 8,
    .operation = LWM2M_OPERATION_E,
    .instances = LWM2M_INSTANCETYPE_single,
    .mandatory = true,
    .type = LWM2M_TYPE_string,
    .changed = (void *) 0,
    .execute = LWM2M_Server_Registration_Update_Trigger_execute,
    .get = (void *) 0,
    .set = (void *) 0,
  },
};


void LWM2M_Server_lock() 
{ 
  mutex_lock(&LWM2M_Server_object_declaration.lock, MUTEXT_WAIT_FOREVER); 
}
void LWM2M_Server_unlock() 
{ 
  mutex_unlock(&LWM2M_Server_object_declaration.lock);
}


uint16_t LWM2M_Server_get_index_for_instance(uint16_t instance)
{
  for (int i = 0; i < MAX_LWM2M_Server_INSTANCES; i++)
  {
    if (LWM2M_Server_instances[i].id == instance) return i;
  }
  return LWM2M_INVALID_INSTANCE;
}

__attribute__((weak)) lwm2m_object_t *LWM2M_Server_initialise(void)
{
  mutex_initialise(&LWM2M_Server_object_declaration.lock);
  
  memset(LWM2M_Server_instances, 0, sizeof(LWM2M_Server_instances));
  for (int i = 0; i < MAX_LWM2M_Server_INSTANCES; i++)
  {
    LWM2M_Server_instances[i].id = LWM2M_INVALID_INSTANCE;
    LWM2M_Server_instances[i].index = i;
  }
  lwm2m_object_base_initialse_object(&LWM2M_Server_lwm2m_object);
  LWM2M_Server_lwm2m_object.userData = &LWM2M_Server_object_declaration; 
  return &LWM2M_Server_lwm2m_object;
}
  
__attribute__((weak)) void LWM2M_Server_register_object(lwm2m_context_t *context)
{
  LWM2M_Server_lock();
  LWM2M_Server_object_declaration.context = context;
  LWM2M_Server_unlock();
}
  
__attribute__((weak)) void LWM2M_Server_register_instance(uint16_t instance_id)
{
  LWM2M_Server_lock();
  uint16_t index = LWM2M_Server_get_index_for_instance(instance_id);
  assert_void(index != LWM2M_INVALID_INSTANCE);
  
  if ((LWM2M_Server_instances[index].id !=  LWM2M_INVALID_INSTANCE) && (!LWM2M_Server_instances[index].registered))
  {
    LWM2M_Server_lwm2m_object.instanceList = LWM2M_LIST_ADD(LWM2M_Server_lwm2m_object.instanceList, (lwm2m_list_t *)&LWM2M_Server_instances[index]);
    LWM2M_Server_instances[index].registered = true;
  }
  LWM2M_Server_unlock();
}

__attribute__((weak)) void LWM2M_Server_unregister_instance(uint16_t instance_id)
{
  LWM2M_Server_lock();
  uint16_t index = LWM2M_Server_get_index_for_instance(instance_id);
  assert_void(index != LWM2M_INVALID_INSTANCE);
  
  if ((LWM2M_Server_instances[index].id !=  LWM2M_INVALID_INSTANCE) && (LWM2M_Server_instances[index].registered))
  {
    LWM2M_Server_instance *instance = 0;
    LWM2M_Server_lwm2m_object.instanceList = lwm2m_list_remove(LWM2M_Server_lwm2m_object.instanceList, instance_id, (lwm2m_list_t **)&instance);
    LWM2M_Server_instances[index].registered = false;
    if (instance != 0)
    { 
      instance->next = 0;
    }
  }
  LWM2M_Server_unlock();
}

//override this to perform actions when a new instance is created
__attribute__((weak)) void LWM2M_Server_instance_created(uint16_t instance_id)
{
  
}
  

//called when the server requests an object to be created; we need to create AND register the 
//instance
uint8_t __LWM2M_Server_create_instance(uint16_t instance_id)
{
  uint8_t res = LWM2M_Server_create_instance(instance_id);
  if (res == COAP_201_CREATED)
  {
    LWM2M_Server_register_instance(instance_id);
  }
  return res;
}
  
__attribute__((weak)) uint8_t LWM2M_Server_create_instance(uint16_t instance_id)
{
  LWM2M_Server_lock();
  //find an unallocated instance
  for (int i = 0; i < MAX_LWM2M_Server_INSTANCES; i++)
  {
    if (LWM2M_Server_instances[i].id == LWM2M_INVALID_INSTANCE)
    {
      LWM2M_Server_instances[i].id = instance_id;
      LWM2M_Server_instance_created(instance_id);
      LWM2M_Server_unlock();
      return COAP_201_CREATED;
    }
  }
  LWM2M_Server_unlock();
  return COAP_500_INTERNAL_SERVER_ERROR; //none found
}

void LWM2M_Server_foreach(lwm2m_each_instance_func func, void *data)
{
  LWM2M_Server_lock();
  
  for (int i = 0; i < MAX_LWM2M_Server_INSTANCES; i++)
  {
    if (LWM2M_Server_instances[i].id != LWM2M_INVALID_INSTANCE)
    {
      if (!func(LWM2M_Server_instances[i].id, data)) break;
    }
  }
  LWM2M_Server_unlock();
}
  
//provides access to a user data pointer for storing additional instance
//data.
void *LWM2M_Server_get_user_data(uint16_t instance_id)
{
  uint16_t index = LWM2M_Server_get_index_for_instance(instance_id);
  if (index == LWM2M_INVALID_INSTANCE) return 0;
  return LWM2M_Server_instances[index].user_data;
}
  
bool LWM2M_Server_set_user_data(uint16_t instance_id, void *data)
{
  uint16_t index = LWM2M_Server_get_index_for_instance(instance_id);
  if (index == LWM2M_INVALID_INSTANCE) return false;
  LWM2M_Server_instances[index].user_data = data;
  return true;
}
  
//override this to perform actions when a new instance is created
__attribute__((weak)) void LWM2M_Server_instance_deleted(uint16_t instance_id)
{
  
}

__attribute__((weak)) uint8_t LWM2M_Server_delete_instance(uint16_t instance_id)
{
  LWM2M_Server_lock();
  uint16_t index = LWM2M_Server_get_index_for_instance(instance_id);
  assert(index != LWM2M_INVALID_INSTANCE);
  
  if ((instance_id < MAX_LWM2M_Server_INSTANCES) && (LWM2M_Server_instances[index].id !=  LWM2M_INVALID_INSTANCE))
  {
    LWM2M_Server_unregister_instance(instance_id);
    LWM2M_Server_instance_deleted(instance_id);
    LWM2M_Server_instances[index].id = LWM2M_INVALID_INSTANCE;
  }
  LWM2M_Server_unlock();
  return COAP_202_DELETED;
}


__attribute__((weak)) void LWM2M_Server_changed(uint16_t instance)
{
  
}

  
__attribute__((weak)) uint8_t LWM2M_Server_Short_Server_ID_get(uint16_t instance,  uint32_t *val)
{
  LWM2M_Server_lock();
  assert(instance >= 0 && instance < MAX_LWM2M_Server_INSTANCES);
  uint16_t index = LWM2M_Server_get_index_for_instance(instance);
  assert(index != LWM2M_INVALID_INSTANCE);
  
  assert(LWM2M_Server_instances[index].id != LWM2M_INVALID_INSTANCE);
  
  
  *val = LWM2M_Server_instances[index].Short_Server_ID;
  LWM2M_Server_unlock();
  return COAP_205_CONTENT;
}
static uint8_t __LWM2M_Server_Short_Server_ID_get(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data)
{
  uint32_t val;
  uint8_t res = LWM2M_Server_Short_Server_ID_get(instance,  &val);
  lwm2m_data_encode_int(val, data);
  return res;
}
                                         
__attribute__((weak)) uint8_t LWM2M_Server_Short_Server_ID_set(uint16_t instance,  uint32_t val, bool notify)
{
  LWM2M_Server_lock();
  assert(instance >= 0 && instance < MAX_LWM2M_Server_INSTANCES);
  uint16_t index = LWM2M_Server_get_index_for_instance(instance);
  assert(index != LWM2M_INVALID_INSTANCE);
  
  assert(LWM2M_Server_instances[index].id != LWM2M_INVALID_INSTANCE);

  LWM2M_Server_instances[index].Short_Server_ID = val;
  
  if (notify && LWM2M_Server_instances[index].registered) lwm2m_object_base_resource_changed(&LWM2M_Server_lwm2m_object, instance, 0, false);  
  LWM2M_Server_unlock();
  return COAP_204_CHANGED;
}
static uint8_t __LWM2M_Server_Short_Server_ID_set(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data)
{
  int64_t val;
  lwm2m_data_decode_int(data, &val);
  return LWM2M_Server_Short_Server_ID_set(instance,  val, false);
}
__attribute__((weak)) uint8_t LWM2M_Server_Short_Server_ID_changed(uint16_t instance)
{
  LWM2M_Server_lock();
  assert(instance >= 0 && instance < MAX_LWM2M_Server_INSTANCES);
  uint16_t index = LWM2M_Server_get_index_for_instance(instance);
  assert(index != LWM2M_INVALID_INSTANCE);
  
  LWM2M_Server_changed(instance);
  LWM2M_Server_unlock();
  return COAP_NO_ERROR;
}
  
__attribute__((weak)) uint8_t LWM2M_Server_Lifetime_get(uint16_t instance,  uint32_t *val)
{
  LWM2M_Server_lock();
  assert(instance >= 0 && instance < MAX_LWM2M_Server_INSTANCES);
  uint16_t index = LWM2M_Server_get_index_for_instance(instance);
  assert(index != LWM2M_INVALID_INSTANCE);
  
  assert(LWM2M_Server_instances[index].id != LWM2M_INVALID_INSTANCE);
  
  
  *val = LWM2M_Server_instances[index].Lifetime;
  LWM2M_Server_unlock();
  return COAP_205_CONTENT;
}
static uint8_t __LWM2M_Server_Lifetime_get(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data)
{
  uint32_t val;
  uint8_t res = LWM2M_Server_Lifetime_get(instance,  &val);
  lwm2m_data_encode_int(val, data);
  return res;
}
                                         
__attribute__((weak)) uint8_t LWM2M_Server_Lifetime_set(uint16_t instance,  uint32_t val, bool notify)
{
  LWM2M_Server_lock();
  assert(instance >= 0 && instance < MAX_LWM2M_Server_INSTANCES);
  uint16_t index = LWM2M_Server_get_index_for_instance(instance);
  assert(index != LWM2M_INVALID_INSTANCE);
  
  assert(LWM2M_Server_instances[index].id != LWM2M_INVALID_INSTANCE);

  LWM2M_Server_instances[index].Lifetime = val;
  
  if (notify && LWM2M_Server_instances[index].registered) lwm2m_object_base_resource_changed(&LWM2M_Server_lwm2m_object, instance, 1, false);  
  LWM2M_Server_unlock();
  return COAP_204_CHANGED;
}
static uint8_t __LWM2M_Server_Lifetime_set(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data)
{
  int64_t val;
  lwm2m_data_decode_int(data, &val);
  return LWM2M_Server_Lifetime_set(instance,  val, false);
}
__attribute__((weak)) uint8_t LWM2M_Server_Lifetime_changed(uint16_t instance)
{
  LWM2M_Server_lock();
  assert(instance >= 0 && instance < MAX_LWM2M_Server_INSTANCES);
  uint16_t index = LWM2M_Server_get_index_for_instance(instance);
  assert(index != LWM2M_INVALID_INSTANCE);
  
  LWM2M_Server_changed(instance);
  LWM2M_Server_unlock();
  return COAP_NO_ERROR;
}
#if CONFIG_LWM2M_Server_Default_Minimum_Period_OPTION == 1
  
__attribute__((weak)) uint8_t LWM2M_Server_Default_Minimum_Period_get(uint16_t instance,  uint32_t *val)
{
  LWM2M_Server_lock();
  assert(instance >= 0 && instance < MAX_LWM2M_Server_INSTANCES);
  uint16_t index = LWM2M_Server_get_index_for_instance(instance);
  assert(index != LWM2M_INVALID_INSTANCE);
  
  assert(LWM2M_Server_instances[index].id != LWM2M_INVALID_INSTANCE);
  
  
  *val = LWM2M_Server_instances[index].Default_Minimum_Period;
  LWM2M_Server_unlock();
  return COAP_205_CONTENT;
}
static uint8_t __LWM2M_Server_Default_Minimum_Period_get(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data)
{
  uint32_t val;
  uint8_t res = LWM2M_Server_Default_Minimum_Period_get(instance,  &val);
  lwm2m_data_encode_int(val, data);
  return res;
}
                                         
#endif
#if CONFIG_LWM2M_Server_Default_Minimum_Period_OPTION == 1
__attribute__((weak)) uint8_t LWM2M_Server_Default_Minimum_Period_set(uint16_t instance,  uint32_t val, bool notify)
{
  LWM2M_Server_lock();
  assert(instance >= 0 && instance < MAX_LWM2M_Server_INSTANCES);
  uint16_t index = LWM2M_Server_get_index_for_instance(instance);
  assert(index != LWM2M_INVALID_INSTANCE);
  
  assert(LWM2M_Server_instances[index].id != LWM2M_INVALID_INSTANCE);

  LWM2M_Server_instances[index].Default_Minimum_Period = val;
  
  if (notify && LWM2M_Server_instances[index].registered) lwm2m_object_base_resource_changed(&LWM2M_Server_lwm2m_object, instance, 2, false);  
  LWM2M_Server_unlock();
  return COAP_204_CHANGED;
}
static uint8_t __LWM2M_Server_Default_Minimum_Period_set(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data)
{
  int64_t val;
  lwm2m_data_decode_int(data, &val);
  return LWM2M_Server_Default_Minimum_Period_set(instance,  val, false);
}
#endif
#if CONFIG_LWM2M_Server_Default_Minimum_Period_OPTION == 1
__attribute__((weak)) uint8_t LWM2M_Server_Default_Minimum_Period_changed(uint16_t instance)
{
  LWM2M_Server_lock();
  assert(instance >= 0 && instance < MAX_LWM2M_Server_INSTANCES);
  uint16_t index = LWM2M_Server_get_index_for_instance(instance);
  assert(index != LWM2M_INVALID_INSTANCE);
  
  LWM2M_Server_changed(instance);
  LWM2M_Server_unlock();
  return COAP_NO_ERROR;
}
#endif
#if CONFIG_LWM2M_Server_Default_Maximum_Period_OPTION == 1
  
__attribute__((weak)) uint8_t LWM2M_Server_Default_Maximum_Period_get(uint16_t instance,  uint32_t *val)
{
  LWM2M_Server_lock();
  assert(instance >= 0 && instance < MAX_LWM2M_Server_INSTANCES);
  uint16_t index = LWM2M_Server_get_index_for_instance(instance);
  assert(index != LWM2M_INVALID_INSTANCE);
  
  assert(LWM2M_Server_instances[index].id != LWM2M_INVALID_INSTANCE);
  
  
  *val = LWM2M_Server_instances[index].Default_Maximum_Period;
  LWM2M_Server_unlock();
  return COAP_205_CONTENT;
}
static uint8_t __LWM2M_Server_Default_Maximum_Period_get(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data)
{
  uint32_t val;
  uint8_t res = LWM2M_Server_Default_Maximum_Period_get(instance,  &val);
  lwm2m_data_encode_int(val, data);
  return res;
}
                                         
#endif
#if CONFIG_LWM2M_Server_Default_Maximum_Period_OPTION == 1
__attribute__((weak)) uint8_t LWM2M_Server_Default_Maximum_Period_set(uint16_t instance,  uint32_t val, bool notify)
{
  LWM2M_Server_lock();
  assert(instance >= 0 && instance < MAX_LWM2M_Server_INSTANCES);
  uint16_t index = LWM2M_Server_get_index_for_instance(instance);
  assert(index != LWM2M_INVALID_INSTANCE);
  
  assert(LWM2M_Server_instances[index].id != LWM2M_INVALID_INSTANCE);

  LWM2M_Server_instances[index].Default_Maximum_Period = val;
  
  if (notify && LWM2M_Server_instances[index].registered) lwm2m_object_base_resource_changed(&LWM2M_Server_lwm2m_object, instance, 3, false);  
  LWM2M_Server_unlock();
  return COAP_204_CHANGED;
}
static uint8_t __LWM2M_Server_Default_Maximum_Period_set(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data)
{
  int64_t val;
  lwm2m_data_decode_int(data, &val);
  return LWM2M_Server_Default_Maximum_Period_set(instance,  val, false);
}
#endif
#if CONFIG_LWM2M_Server_Default_Maximum_Period_OPTION == 1
__attribute__((weak)) uint8_t LWM2M_Server_Default_Maximum_Period_changed(uint16_t instance)
{
  LWM2M_Server_lock();
  assert(instance >= 0 && instance < MAX_LWM2M_Server_INSTANCES);
  uint16_t index = LWM2M_Server_get_index_for_instance(instance);
  assert(index != LWM2M_INVALID_INSTANCE);
  
  LWM2M_Server_changed(instance);
  LWM2M_Server_unlock();
  return COAP_NO_ERROR;
}
#endif
#if CONFIG_LWM2M_Server_Disable_OPTION == 1
__attribute__((weak)) uint8_t LWM2M_Server_Disable_execute(lwm2m_object_t *object, uint16_t instance, uint16_t resource_id, uint8_t * buffer, int length)
{
  return COAP_405_METHOD_NOT_ALLOWED;
}
#endif
#if CONFIG_LWM2M_Server_Disable_Timeout_OPTION == 1
  
__attribute__((weak)) uint8_t LWM2M_Server_Disable_Timeout_get(uint16_t instance,  uint32_t *val)
{
  LWM2M_Server_lock();
  assert(instance >= 0 && instance < MAX_LWM2M_Server_INSTANCES);
  uint16_t index = LWM2M_Server_get_index_for_instance(instance);
  assert(index != LWM2M_INVALID_INSTANCE);
  
  assert(LWM2M_Server_instances[index].id != LWM2M_INVALID_INSTANCE);
  
  
  *val = LWM2M_Server_instances[index].Disable_Timeout;
  LWM2M_Server_unlock();
  return COAP_205_CONTENT;
}
static uint8_t __LWM2M_Server_Disable_Timeout_get(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data)
{
  uint32_t val;
  uint8_t res = LWM2M_Server_Disable_Timeout_get(instance,  &val);
  lwm2m_data_encode_int(val, data);
  return res;
}
                                         
#endif
#if CONFIG_LWM2M_Server_Disable_Timeout_OPTION == 1
__attribute__((weak)) uint8_t LWM2M_Server_Disable_Timeout_set(uint16_t instance,  uint32_t val, bool notify)
{
  LWM2M_Server_lock();
  assert(instance >= 0 && instance < MAX_LWM2M_Server_INSTANCES);
  uint16_t index = LWM2M_Server_get_index_for_instance(instance);
  assert(index != LWM2M_INVALID_INSTANCE);
  
  assert(LWM2M_Server_instances[index].id != LWM2M_INVALID_INSTANCE);

  LWM2M_Server_instances[index].Disable_Timeout = val;
  
  if (notify && LWM2M_Server_instances[index].registered) lwm2m_object_base_resource_changed(&LWM2M_Server_lwm2m_object, instance, 5, false);  
  LWM2M_Server_unlock();
  return COAP_204_CHANGED;
}
static uint8_t __LWM2M_Server_Disable_Timeout_set(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data)
{
  int64_t val;
  lwm2m_data_decode_int(data, &val);
  return LWM2M_Server_Disable_Timeout_set(instance,  val, false);
}
#endif
#if CONFIG_LWM2M_Server_Disable_Timeout_OPTION == 1
__attribute__((weak)) uint8_t LWM2M_Server_Disable_Timeout_changed(uint16_t instance)
{
  LWM2M_Server_lock();
  assert(instance >= 0 && instance < MAX_LWM2M_Server_INSTANCES);
  uint16_t index = LWM2M_Server_get_index_for_instance(instance);
  assert(index != LWM2M_INVALID_INSTANCE);
  
  LWM2M_Server_changed(instance);
  LWM2M_Server_unlock();
  return COAP_NO_ERROR;
}
#endif
  
                                         
__attribute__((weak)) uint8_t LWM2M_Server_Notification_Storing_When_Disabled_or_Offline_get(uint16_t instance,  bool *val)
{
  LWM2M_Server_lock();
  assert(instance >= 0 && instance < MAX_LWM2M_Server_INSTANCES);
  uint16_t index = LWM2M_Server_get_index_for_instance(instance);
  assert(index != LWM2M_INVALID_INSTANCE);
  
  assert(LWM2M_Server_instances[index].id != LWM2M_INVALID_INSTANCE);
  
  
  *val = LWM2M_Server_instances[index].Notification_Storing_When_Disabled_or_Offline;
  LWM2M_Server_unlock();
  return COAP_205_CONTENT;
}
static uint8_t __LWM2M_Server_Notification_Storing_When_Disabled_or_Offline_get(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data)
{
  bool val;
  uint8_t res = LWM2M_Server_Notification_Storing_When_Disabled_or_Offline_get(instance,  &val);
  lwm2m_data_encode_bool(val, data);
  return res;
}
__attribute__((weak)) uint8_t LWM2M_Server_Notification_Storing_When_Disabled_or_Offline_set(uint16_t instance,  bool val, bool notify)
{
  LWM2M_Server_lock();
  assert(instance >= 0 && instance < MAX_LWM2M_Server_INSTANCES);
  uint16_t index = LWM2M_Server_get_index_for_instance(instance);
  assert(index != LWM2M_INVALID_INSTANCE);
  
  assert(LWM2M_Server_instances[index].id != LWM2M_INVALID_INSTANCE);

  LWM2M_Server_instances[index].Notification_Storing_When_Disabled_or_Offline = val;  
  
  
  if (notify && LWM2M_Server_instances[index].registered) lwm2m_object_base_resource_changed(&LWM2M_Server_lwm2m_object, instance, 6, false);
  LWM2M_Server_unlock();
  return COAP_204_CHANGED;
}
static uint8_t __LWM2M_Server_Notification_Storing_When_Disabled_or_Offline_set(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data)
{
  bool val;
  lwm2m_data_decode_bool(data, &val);
  return LWM2M_Server_Notification_Storing_When_Disabled_or_Offline_set(instance, val, false);
}
__attribute__((weak)) uint8_t LWM2M_Server_Notification_Storing_When_Disabled_or_Offline_changed(uint16_t instance)
{
  LWM2M_Server_lock();
  assert(instance >= 0 && instance < MAX_LWM2M_Server_INSTANCES);
  uint16_t index = LWM2M_Server_get_index_for_instance(instance);
  assert(index != LWM2M_INVALID_INSTANCE);
  
  LWM2M_Server_changed(instance);
  LWM2M_Server_unlock();
  return COAP_NO_ERROR;
}
  
__attribute__((weak))  uint8_t LWM2M_Server_Binding_get(uint16_t instance,  const char **val)
{
  LWM2M_Server_lock();
  assert(instance >= 0 && instance < MAX_LWM2M_Server_INSTANCES);
  uint16_t index = LWM2M_Server_get_index_for_instance(instance);
  assert(index != LWM2M_INVALID_INSTANCE);
  
  assert(LWM2M_Server_instances[index].id != LWM2M_INVALID_INSTANCE);
  
  
  *val = LWM2M_Server_instances[index].Binding;
  LWM2M_Server_unlock();
  return COAP_205_CONTENT;
}
static uint8_t __LWM2M_Server_Binding_get(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data)
{
  const char *val;
  uint8_t res = LWM2M_Server_Binding_get(instance,  &val);
  lwm2m_data_encode_string(val, data);
  return res;
}
                                         
__attribute__((weak)) uint8_t LWM2M_Server_Binding_set(uint16_t instance,  const char *val, size_t length, bool notify)
{
  LWM2M_Server_lock();
  assert(instance >= 0 && instance < MAX_LWM2M_Server_INSTANCES);
  uint16_t index = LWM2M_Server_get_index_for_instance(instance);
  assert(index != LWM2M_INVALID_INSTANCE);
  
  assert(LWM2M_Server_instances[index].id != LWM2M_INVALID_INSTANCE);
 
  if (length >= sizeof(LWM2M_Server_instances[index].Binding )) length = sizeof(LWM2M_Server_instances[index].Binding) - 1;
  memcpy(LWM2M_Server_instances[index].Binding, val, length);
  LWM2M_Server_instances[index].Binding[length] = 0;
  
  
  if (notify && LWM2M_Server_instances[index].registered) lwm2m_object_base_resource_changed(&LWM2M_Server_lwm2m_object, instance, 7, false);
  LWM2M_Server_unlock();
  return COAP_204_CHANGED;
}
static uint8_t __LWM2M_Server_Binding_set(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data)
{
  return LWM2M_Server_Binding_set(instance,  (char*)data->value.asBuffer.buffer, data->value.asBuffer.length, false);
}
__attribute__((weak)) uint8_t LWM2M_Server_Binding_changed(uint16_t instance)
{
  LWM2M_Server_lock();
  assert(instance >= 0 && instance < MAX_LWM2M_Server_INSTANCES);
  uint16_t index = LWM2M_Server_get_index_for_instance(instance);
  assert(index != LWM2M_INVALID_INSTANCE);
  
  LWM2M_Server_changed(instance);
  LWM2M_Server_unlock();
  return COAP_NO_ERROR;
}
__attribute__((weak)) uint8_t LWM2M_Server_Registration_Update_Trigger_execute(lwm2m_object_t *object, uint16_t instance, uint16_t resource_id, uint8_t * buffer, int length)
{
  return COAP_405_METHOD_NOT_ALLOWED;
}
