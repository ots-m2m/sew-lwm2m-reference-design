/*
 * Firmware_Update_base.c
 * 
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * This code was generated using gt-object-gen provided by Operational 
 * Technology Solutions M2M Pty. Ltd. For licensing enquires visit 
 * ot-solutions.com.au.
 */
#include "Firmware_Update_base.h"

#include <string.h>
#include <stdbool.h>

#include "mutex.h"
#define assert(condition) if (!(condition)) { Firmware_Update_unlock(); return COAP_406_NOT_ACCEPTABLE; }
#define assert_void(condition) if (!(condition)) { Firmware_Update_unlock(); return; }

uint8_t __Firmware_Update_create_instance(uint16_t instance_id);
static uint8_t __Firmware_Update_Package_set(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data);
static uint8_t __Firmware_Update_Package_URI_set(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data);
static uint8_t __Firmware_Update_State_get(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data);
  
#if CONFIG_Firmware_Update_Update_Supported_Objects_OPTION == 1
static uint8_t __Firmware_Update_Update_Supported_Objects_get(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data);
  
#endif
#if CONFIG_Firmware_Update_Update_Supported_Objects_OPTION == 1
static uint8_t __Firmware_Update_Update_Supported_Objects_set(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data);
#endif
static uint8_t __Firmware_Update_Update_Result_get(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data);
  

static lwm2m_object_t Firmware_Update_lwm2m_object = {
  .objID = 5
};

lwm2m_object_t *Firmware_Update_get_lwm2m_object(void)
{
  return &Firmware_Update_lwm2m_object;
}

typedef struct Firmware_Update_instance_t
{
  struct Firmware_Update_instance * next;   // matches lwm2m_list_t::next
  uint16_t id;                                // matches lwm2m_list_t::id
  uint16_t index;
  bool registered;
  void *user_data;
  uint8_t *Package;
  uint16_t Package_length;
  char Package_URI [MAX_Firmware_Update_Package_URI];
  uint32_t State;
#if CONFIG_Firmware_Update_Update_Supported_Objects_OPTION == 1
  bool Update_Supported_Objects;
#endif
  uint32_t Update_Result;
}
Firmware_Update_instance;

static Firmware_Update_instance Firmware_Update_instances[MAX_Firmware_Update_INSTANCES];

lwm2m_object_declaration_t Firmware_Update_object_declaration = {
  .id = 5,
  .delete = Firmware_Update_delete_instance,
  .create = __Firmware_Update_create_instance,
  .resources[0] = { 
    .supported = true,  
    .id = 0,
    .operation = LWM2M_OPERATION_W,
    .instances = LWM2M_INSTANCETYPE_single,
    .mandatory = true,
    .type = LWM2M_TYPE_opaque,
    .changed = Firmware_Update_Package_changed,
    .execute = (void *) 0,
    .get = (void *) 0,
    .set = __Firmware_Update_Package_set,
  },
  .resources[1] = { 
    .supported = true,  
    .id = 1,
    .operation = LWM2M_OPERATION_W,
    .instances = LWM2M_INSTANCETYPE_single,
    .mandatory = true,
    .type = LWM2M_TYPE_string,
    .changed = Firmware_Update_Package_URI_changed,
    .execute = (void *) 0,
    .get = (void *) 0,
    .set = __Firmware_Update_Package_URI_set,
  },
  .resources[2] = { 
    .supported = true,  
    .id = 2,
    .operation = LWM2M_OPERATION_E,
    .instances = LWM2M_INSTANCETYPE_single,
    .mandatory = true,
    .type = LWM2M_TYPE_string,
    .changed = (void *) 0,
    .execute = Firmware_Update_Update_execute,
    .get = (void *) 0,
    .set = (void *) 0,
  },
  .resources[3] = { 
    .supported = true,  
    .id = 3,
    .operation = LWM2M_OPERATION_R,
    .instances = LWM2M_INSTANCETYPE_single,
    .mandatory = true,
    .type = LWM2M_TYPE_integer,
    .changed = (void *) 0,
    .execute = (void *) 0,
    .get = __Firmware_Update_State_get,
    .set = (void *) 0,
  },
#if CONFIG_Firmware_Update_Update_Supported_Objects_OPTION == 1
  .resources[4] = { 
    .supported = true,  
    .id = 4,
    .operation = LWM2M_OPERATION_RW,
    .instances = LWM2M_INSTANCETYPE_single,
    .mandatory = false,
    .type = LWM2M_TYPE_boolean,
    .changed = Firmware_Update_Update_Supported_Objects_changed,
    .execute = (void *) 0,
    .get = __Firmware_Update_Update_Supported_Objects_get,
    .set = __Firmware_Update_Update_Supported_Objects_set,
  },
#else
  .resources[4] = { .supported = false },
#endif
  .resources[5] = { 
    .supported = true,  
    .id = 5,
    .operation = LWM2M_OPERATION_R,
    .instances = LWM2M_INSTANCETYPE_single,
    .mandatory = true,
    .type = LWM2M_TYPE_integer,
    .changed = (void *) 0,
    .execute = (void *) 0,
    .get = __Firmware_Update_Update_Result_get,
    .set = (void *) 0,
  },
};


void Firmware_Update_lock() 
{ 
  mutex_lock(&Firmware_Update_object_declaration.lock, MUTEXT_WAIT_FOREVER); 
}
void Firmware_Update_unlock() 
{ 
  mutex_unlock(&Firmware_Update_object_declaration.lock);
}


uint16_t Firmware_Update_get_index_for_instance(uint16_t instance)
{
  for (int i = 0; i < MAX_Firmware_Update_INSTANCES; i++)
  {
    if (Firmware_Update_instances[i].id == instance) return i;
  }
  return LWM2M_INVALID_INSTANCE;
}

__attribute__((weak)) lwm2m_object_t *Firmware_Update_initialise(void)
{
  mutex_initialise(&Firmware_Update_object_declaration.lock);
  
  memset(Firmware_Update_instances, 0, sizeof(Firmware_Update_instances));
  for (int i = 0; i < MAX_Firmware_Update_INSTANCES; i++)
  {
    Firmware_Update_instances[i].id = LWM2M_INVALID_INSTANCE;
    Firmware_Update_instances[i].index = i;
  }
  lwm2m_object_base_initialse_object(&Firmware_Update_lwm2m_object);
  Firmware_Update_lwm2m_object.userData = &Firmware_Update_object_declaration; 
  //singleton so register the only instance
  //Firmware_Update_create_instance(0);
  return &Firmware_Update_lwm2m_object;
}
  
__attribute__((weak)) void Firmware_Update_register_object(lwm2m_context_t *context)
{
  Firmware_Update_lock();
  Firmware_Update_object_declaration.context = context;
  Firmware_Update_unlock();
}
  
__attribute__((weak)) void Firmware_Update_register_instance(uint16_t instance_id)
{
  Firmware_Update_lock();
  uint16_t index = Firmware_Update_get_index_for_instance(instance_id);
  assert_void(index != LWM2M_INVALID_INSTANCE);
  
  if ((Firmware_Update_instances[index].id !=  LWM2M_INVALID_INSTANCE) && (!Firmware_Update_instances[index].registered))
  {
    Firmware_Update_lwm2m_object.instanceList = LWM2M_LIST_ADD(Firmware_Update_lwm2m_object.instanceList, (lwm2m_list_t *)&Firmware_Update_instances[index]);
    Firmware_Update_instances[index].registered = true;
  }
  Firmware_Update_unlock();
}

__attribute__((weak)) void Firmware_Update_unregister_instance(uint16_t instance_id)
{
  Firmware_Update_lock();
  uint16_t index = Firmware_Update_get_index_for_instance(instance_id);
  assert_void(index != LWM2M_INVALID_INSTANCE);
  
  if ((Firmware_Update_instances[index].id !=  LWM2M_INVALID_INSTANCE) && (Firmware_Update_instances[index].registered))
  {
    Firmware_Update_instance *instance = 0;
    Firmware_Update_lwm2m_object.instanceList = lwm2m_list_remove(Firmware_Update_lwm2m_object.instanceList, instance_id, (lwm2m_list_t **)&instance);
    Firmware_Update_instances[index].registered = false;
    if (instance != 0)
    { 
      instance->next = 0;
    }
  }
  Firmware_Update_unlock();
}

//override this to perform actions when a new instance is created
__attribute__((weak)) void Firmware_Update_instance_created(uint16_t instance_id)
{
  
}
  

//called when the server requests an object to be created; we need to create AND register the 
//instance
uint8_t __Firmware_Update_create_instance(uint16_t instance_id)
{
  uint8_t res = Firmware_Update_create_instance(instance_id);
  if (res == COAP_201_CREATED)
  {
    Firmware_Update_register_instance(instance_id);
  }
  return res;
}
  
__attribute__((weak)) uint8_t Firmware_Update_create_instance(uint16_t instance_id)
{
  Firmware_Update_lock();
  //find an unallocated instance
  for (int i = 0; i < MAX_Firmware_Update_INSTANCES; i++)
  {
    if (Firmware_Update_instances[i].id == LWM2M_INVALID_INSTANCE)
    {
      Firmware_Update_instances[i].id = instance_id;
      Firmware_Update_instance_created(instance_id);
      Firmware_Update_unlock();
      return COAP_201_CREATED;
    }
  }
  Firmware_Update_unlock();
  return COAP_500_INTERNAL_SERVER_ERROR; //none found
}

void Firmware_Update_foreach(lwm2m_each_instance_func func, void *data)
{
  Firmware_Update_lock();
  
  for (int i = 0; i < MAX_Firmware_Update_INSTANCES; i++)
  {
    if (Firmware_Update_instances[i].id != LWM2M_INVALID_INSTANCE)
    {
      if (!func(Firmware_Update_instances[i].id, data)) break;
    }
  }
  Firmware_Update_unlock();
}
  
//provides access to a user data pointer for storing additional instance
//data.
void *Firmware_Update_get_user_data(uint16_t instance_id)
{
  uint16_t index = Firmware_Update_get_index_for_instance(instance_id);
  if (index == LWM2M_INVALID_INSTANCE) return 0;
  return Firmware_Update_instances[index].user_data;
}
  
bool Firmware_Update_set_user_data(uint16_t instance_id, void *data)
{
  uint16_t index = Firmware_Update_get_index_for_instance(instance_id);
  if (index == LWM2M_INVALID_INSTANCE) return false;
  Firmware_Update_instances[index].user_data = data;
  return true;
}
  
//override this to perform actions when a new instance is created
__attribute__((weak)) void Firmware_Update_instance_deleted(uint16_t instance_id)
{
  
}

__attribute__((weak)) uint8_t Firmware_Update_delete_instance(uint16_t instance_id)
{
  Firmware_Update_lock();
  uint16_t index = Firmware_Update_get_index_for_instance(instance_id);
  assert(index != LWM2M_INVALID_INSTANCE);
  
  if ((instance_id < MAX_Firmware_Update_INSTANCES) && (Firmware_Update_instances[index].id !=  LWM2M_INVALID_INSTANCE))
  {
    Firmware_Update_unregister_instance(instance_id);
    Firmware_Update_instance_deleted(instance_id);
    Firmware_Update_instances[index].id = LWM2M_INVALID_INSTANCE;
  }
  Firmware_Update_unlock();
  return COAP_202_DELETED;
}


__attribute__((weak)) void Firmware_Update_changed(uint16_t instance)
{
  
}

  
__attribute__((weak))  uint8_t Firmware_Update_Package_get(uint16_t instance,  uint8_t **val, size_t *length)
{
  Firmware_Update_lock();
  assert(instance >= 0 && instance < MAX_Firmware_Update_INSTANCES);
  uint16_t index = Firmware_Update_get_index_for_instance(instance);
  assert(index != LWM2M_INVALID_INSTANCE);
  
  assert(Firmware_Update_instances[index].id != LWM2M_INVALID_INSTANCE);
  
  
  
  *val = Firmware_Update_instances[index].Package;
  *length = Firmware_Update_instances[index].Package_length;
  Firmware_Update_unlock();
  return COAP_205_CONTENT;
}
                                         
 __attribute__((weak)) uint8_t Firmware_Update_Package_set(uint16_t instance,  uint8_t* val, size_t length, bool notify)
 {
   Firmware_Update_lock();
   assert(instance >= 0 && instance < MAX_Firmware_Update_INSTANCES);
 
   uint16_t index = Firmware_Update_get_index_for_instance(instance);
   assert(index != LWM2M_INVALID_INSTANCE);
   
   if (Firmware_Update_instances[index].Package != 0) lwm2m_free(Firmware_Update_instances[index].Package);
   Firmware_Update_instances[index].Package = lwm2m_malloc(length);
   if (Firmware_Update_instances[index].Package == 0)
   {
     Firmware_Update_instances[index].Package_length  = 0;   
     Firmware_Update_unlock();
     return COAP_500_INTERNAL_SERVER_ERROR;                                     
   } 
   Firmware_Update_instances[index].Package_length  = length;
   memcpy(Firmware_Update_instances[index].Package, val, length);                                      
                                  
   
  
   if (notify && Firmware_Update_instances[index].registered) lwm2m_object_base_resource_changed(&Firmware_Update_lwm2m_object, instance, 0, false);
  
  
   Firmware_Update_unlock();
   return COAP_204_CHANGED;
 }
 static uint8_t __Firmware_Update_Package_set(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data)
 {
   return Firmware_Update_Package_set(instance,  (uint8_t *)data->value.asBuffer.buffer,data->value.asBuffer.length, false);
 }
__attribute__((weak)) uint8_t Firmware_Update_Package_changed(uint16_t instance)
{
  Firmware_Update_lock();
  assert(instance >= 0 && instance < MAX_Firmware_Update_INSTANCES);
  uint16_t index = Firmware_Update_get_index_for_instance(instance);
  assert(index != LWM2M_INVALID_INSTANCE);
  
  Firmware_Update_changed(instance);
  Firmware_Update_unlock();
  return COAP_NO_ERROR;
}
  
__attribute__((weak))  uint8_t Firmware_Update_Package_URI_get(uint16_t instance,  const char **val)
{
  Firmware_Update_lock();
  assert(instance >= 0 && instance < MAX_Firmware_Update_INSTANCES);
  uint16_t index = Firmware_Update_get_index_for_instance(instance);
  assert(index != LWM2M_INVALID_INSTANCE);
  
  assert(Firmware_Update_instances[index].id != LWM2M_INVALID_INSTANCE);
  
  
  *val = Firmware_Update_instances[index].Package_URI;
  Firmware_Update_unlock();
  return COAP_205_CONTENT;
}
                                         
static uint8_t Firmware_Update_Package_URI_set(uint16_t instance,  const char *val, size_t length, bool notify)
{
  Firmware_Update_lock();
  assert(instance >= 0 && instance < MAX_Firmware_Update_INSTANCES);
  uint16_t index = Firmware_Update_get_index_for_instance(instance);
  assert(index != LWM2M_INVALID_INSTANCE);
  
  assert(Firmware_Update_instances[index].id != LWM2M_INVALID_INSTANCE);
 
  if (length >= sizeof(Firmware_Update_instances[index].Package_URI )) length = sizeof(Firmware_Update_instances[index].Package_URI) - 1;
  memcpy(Firmware_Update_instances[index].Package_URI, val, length);
  Firmware_Update_instances[index].Package_URI[length] = 0;
  
  
  if (notify && Firmware_Update_instances[index].registered) lwm2m_object_base_resource_changed(&Firmware_Update_lwm2m_object, instance, 1, false);
  Firmware_Update_unlock();
  return COAP_204_CHANGED;
}
static uint8_t __Firmware_Update_Package_URI_set(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data)
{
  return Firmware_Update_Package_URI_set(instance,  (char*)data->value.asBuffer.buffer, data->value.asBuffer.length, false);
}
__attribute__((weak)) uint8_t Firmware_Update_Package_URI_changed(uint16_t instance)
{
  Firmware_Update_lock();
  assert(instance >= 0 && instance < MAX_Firmware_Update_INSTANCES);
  uint16_t index = Firmware_Update_get_index_for_instance(instance);
  assert(index != LWM2M_INVALID_INSTANCE);
  
  Firmware_Update_changed(instance);
  Firmware_Update_unlock();
  return COAP_NO_ERROR;
}
__attribute__((weak)) uint8_t Firmware_Update_Update_execute(lwm2m_object_t *object, uint16_t instance, uint16_t resource_id, uint8_t * buffer, int length)
{
  return COAP_405_METHOD_NOT_ALLOWED;
}
  
__attribute__((weak)) uint8_t Firmware_Update_State_get(uint16_t instance,  uint32_t *val)
{
  Firmware_Update_lock();
  assert(instance >= 0 && instance < MAX_Firmware_Update_INSTANCES);
  uint16_t index = Firmware_Update_get_index_for_instance(instance);
  assert(index != LWM2M_INVALID_INSTANCE);
  
  assert(Firmware_Update_instances[index].id != LWM2M_INVALID_INSTANCE);
  
  
  *val = Firmware_Update_instances[index].State;
  Firmware_Update_unlock();
  return COAP_205_CONTENT;
}
static uint8_t __Firmware_Update_State_get(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data)
{
  uint32_t val;
  uint8_t res = Firmware_Update_State_get(instance,  &val);
  lwm2m_data_encode_int(val, data);
  return res;
}
                                         
__attribute__((weak)) uint8_t Firmware_Update_State_set(uint16_t instance,  uint32_t val, bool notify)
{
  Firmware_Update_lock();
  assert(instance >= 0 && instance < MAX_Firmware_Update_INSTANCES);
  uint16_t index = Firmware_Update_get_index_for_instance(instance);
  assert(index != LWM2M_INVALID_INSTANCE);
  
  assert(Firmware_Update_instances[index].id != LWM2M_INVALID_INSTANCE);

  Firmware_Update_instances[index].State = val;
  
  if (notify && Firmware_Update_instances[index].registered) lwm2m_object_base_resource_changed(&Firmware_Update_lwm2m_object, instance, 3, false);  
  Firmware_Update_unlock();
  return COAP_204_CHANGED;
}
#if CONFIG_Firmware_Update_Update_Supported_Objects_OPTION == 1
  
                                         
__attribute__((weak)) uint8_t Firmware_Update_Update_Supported_Objects_get(uint16_t instance,  bool *val)
{
  Firmware_Update_lock();
  assert(instance >= 0 && instance < MAX_Firmware_Update_INSTANCES);
  uint16_t index = Firmware_Update_get_index_for_instance(instance);
  assert(index != LWM2M_INVALID_INSTANCE);
  
  assert(Firmware_Update_instances[index].id != LWM2M_INVALID_INSTANCE);
  
  
  *val = Firmware_Update_instances[index].Update_Supported_Objects;
  Firmware_Update_unlock();
  return COAP_205_CONTENT;
}
static uint8_t __Firmware_Update_Update_Supported_Objects_get(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data)
{
  bool val;
  uint8_t res = Firmware_Update_Update_Supported_Objects_get(instance,  &val);
  lwm2m_data_encode_bool(val, data);
  return res;
}
#endif
#if CONFIG_Firmware_Update_Update_Supported_Objects_OPTION == 1
__attribute__((weak)) uint8_t Firmware_Update_Update_Supported_Objects_set(uint16_t instance,  bool val, bool notify)
{
  Firmware_Update_lock();
  assert(instance >= 0 && instance < MAX_Firmware_Update_INSTANCES);
  uint16_t index = Firmware_Update_get_index_for_instance(instance);
  assert(index != LWM2M_INVALID_INSTANCE);
  
  assert(Firmware_Update_instances[index].id != LWM2M_INVALID_INSTANCE);

  Firmware_Update_instances[index].Update_Supported_Objects = val;  
  
  
  if (notify && Firmware_Update_instances[index].registered) lwm2m_object_base_resource_changed(&Firmware_Update_lwm2m_object, instance, 4, false);
  Firmware_Update_unlock();
  return COAP_204_CHANGED;
}
static uint8_t __Firmware_Update_Update_Supported_Objects_set(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data)
{
  bool val;
  lwm2m_data_decode_bool(data, &val);
  return Firmware_Update_Update_Supported_Objects_set(instance, val, false);
}
#endif
#if CONFIG_Firmware_Update_Update_Supported_Objects_OPTION == 1
__attribute__((weak)) uint8_t Firmware_Update_Update_Supported_Objects_changed(uint16_t instance)
{
  Firmware_Update_lock();
  assert(instance >= 0 && instance < MAX_Firmware_Update_INSTANCES);
  uint16_t index = Firmware_Update_get_index_for_instance(instance);
  assert(index != LWM2M_INVALID_INSTANCE);
  
  Firmware_Update_changed(instance);
  Firmware_Update_unlock();
  return COAP_NO_ERROR;
}
#endif
  
__attribute__((weak)) uint8_t Firmware_Update_Update_Result_get(uint16_t instance,  uint32_t *val)
{
  Firmware_Update_lock();
  assert(instance >= 0 && instance < MAX_Firmware_Update_INSTANCES);
  uint16_t index = Firmware_Update_get_index_for_instance(instance);
  assert(index != LWM2M_INVALID_INSTANCE);
  
  assert(Firmware_Update_instances[index].id != LWM2M_INVALID_INSTANCE);
  
  
  *val = Firmware_Update_instances[index].Update_Result;
  Firmware_Update_unlock();
  return COAP_205_CONTENT;
}
static uint8_t __Firmware_Update_Update_Result_get(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data)
{
  uint32_t val;
  uint8_t res = Firmware_Update_Update_Result_get(instance,  &val);
  lwm2m_data_encode_int(val, data);
  return res;
}
                                         
__attribute__((weak)) uint8_t Firmware_Update_Update_Result_set(uint16_t instance,  uint32_t val, bool notify)
{
  Firmware_Update_lock();
  assert(instance >= 0 && instance < MAX_Firmware_Update_INSTANCES);
  uint16_t index = Firmware_Update_get_index_for_instance(instance);
  assert(index != LWM2M_INVALID_INSTANCE);
  
  assert(Firmware_Update_instances[index].id != LWM2M_INVALID_INSTANCE);

  Firmware_Update_instances[index].Update_Result = val;
  
  if (notify && Firmware_Update_instances[index].registered) lwm2m_object_base_resource_changed(&Firmware_Update_lwm2m_object, instance, 5, false);  
  Firmware_Update_unlock();
  return COAP_204_CHANGED;
}
