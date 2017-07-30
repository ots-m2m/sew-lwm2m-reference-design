/*
 * LWM2M_Security_base.c
 * 
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * This code was generated using gt-object-gen provided by Operational 
 * Technology Solutions M2M Pty. Ltd. For licensing enquires visit 
 * ot-solutions.com.au.
 */
#include "LWM2M_Security_base.h"

#include <string.h>
#include <stdbool.h>

#include "mutex.h"
#define assert(condition) if (!(condition)) { LWM2M_Security_unlock(); return COAP_406_NOT_ACCEPTABLE; }
#define assert_void(condition) if (!(condition)) { LWM2M_Security_unlock(); return; }

uint8_t __LWM2M_Security_create_instance(uint16_t instance_id);
static uint8_t __LWM2M_Security_LWM2M_Server_URI_get(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data);
  
static uint8_t __LWM2M_Security_LWM2M_Server_URI_set(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data);
static uint8_t __LWM2M_Security_Bootstrap_Server_get(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data);
  
static uint8_t __LWM2M_Security_Bootstrap_Server_set(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data);
static uint8_t __LWM2M_Security_Security_Mode_get(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data);
  
static uint8_t __LWM2M_Security_Security_Mode_set(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data);
static uint8_t __LWM2M_Security_Public_Key_or_Identity_get(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data);
  
static uint8_t __LWM2M_Security_Public_Key_or_Identity_set(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data);
static uint8_t __LWM2M_Security_Server_Public_Key_or_Identity_get(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data);
  
static uint8_t __LWM2M_Security_Server_Public_Key_or_Identity_set(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data);
static uint8_t __LWM2M_Security_Secret_Key_get(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data);
  
static uint8_t __LWM2M_Security_Secret_Key_set(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data);
static uint8_t __LWM2M_Security_SMS_Security_Mode_get(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data);
  
static uint8_t __LWM2M_Security_SMS_Security_Mode_set(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data);
static uint8_t __LWM2M_Security_SMS_Binding_Key_Parameters_get(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data);
  
static uint8_t __LWM2M_Security_SMS_Binding_Key_Parameters_set(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data);
static uint8_t __LWM2M_Security_SMS_Binding_Secret_Keys_get(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data);
  
static uint8_t __LWM2M_Security_SMS_Binding_Secret_Keys_set(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data);
static uint8_t __LWM2M_Security_LWM2M_Server_SMS_Number_get(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data);
  
static uint8_t __LWM2M_Security_LWM2M_Server_SMS_Number_set(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data);
#if CONFIG_LWM2M_Security_Short_Server_ID_OPTION == 1
static uint8_t __LWM2M_Security_Short_Server_ID_get(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data);
  
#endif
#if CONFIG_LWM2M_Security_Short_Server_ID_OPTION == 1
static uint8_t __LWM2M_Security_Short_Server_ID_set(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data);
#endif
static uint8_t __LWM2M_Security_Client_Hold_Off_Time_get(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data);
  
static uint8_t __LWM2M_Security_Client_Hold_Off_Time_set(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data);

static lwm2m_object_t LWM2M_Security_lwm2m_object = {
  .objID = 0
};

lwm2m_object_t *LWM2M_Security_get_lwm2m_object(void)
{
  return &LWM2M_Security_lwm2m_object;
}

typedef struct LWM2M_Security_instance_t
{
  struct LWM2M_Security_instance * next;   // matches lwm2m_list_t::next
  uint16_t id;                                // matches lwm2m_list_t::id
  uint16_t index;
  bool registered;
  void *user_data;
  char LWM2M_Server_URI [MAX_LWM2M_Security_LWM2M_Server_URI];
  bool Bootstrap_Server;
  uint32_t Security_Mode;
  uint8_t *Public_Key_or_Identity;
  uint16_t Public_Key_or_Identity_length;
  uint8_t *Server_Public_Key_or_Identity;
  uint16_t Server_Public_Key_or_Identity_length;
  uint8_t *Secret_Key;
  uint16_t Secret_Key_length;
  uint32_t SMS_Security_Mode;
  uint8_t *SMS_Binding_Key_Parameters;
  uint16_t SMS_Binding_Key_Parameters_length;
  uint8_t *SMS_Binding_Secret_Keys;
  uint16_t SMS_Binding_Secret_Keys_length;
  uint32_t LWM2M_Server_SMS_Number;
#if CONFIG_LWM2M_Security_Short_Server_ID_OPTION == 1
  uint32_t Short_Server_ID;
#endif
  uint32_t Client_Hold_Off_Time;
}
LWM2M_Security_instance;

static LWM2M_Security_instance LWM2M_Security_instances[MAX_LWM2M_Security_INSTANCES];

lwm2m_object_declaration_t LWM2M_Security_object_declaration = {
  .id = 0,
  .delete = LWM2M_Security_delete_instance,
  .create = __LWM2M_Security_create_instance,
  .resources[0] = { 
    .supported = true,  
    .id = 0,
    .operation = LWM2M_OPERATION_NONE,
    .instances = LWM2M_INSTANCETYPE_single,
    .mandatory = true,
    .type = LWM2M_TYPE_string,
    .changed = LWM2M_Security_LWM2M_Server_URI_changed,
    .execute = (void *) 0,
    .get = __LWM2M_Security_LWM2M_Server_URI_get,
    .set = __LWM2M_Security_LWM2M_Server_URI_set,
  },
  .resources[1] = { 
    .supported = true,  
    .id = 1,
    .operation = LWM2M_OPERATION_NONE,
    .instances = LWM2M_INSTANCETYPE_single,
    .mandatory = true,
    .type = LWM2M_TYPE_boolean,
    .changed = LWM2M_Security_Bootstrap_Server_changed,
    .execute = (void *) 0,
    .get = __LWM2M_Security_Bootstrap_Server_get,
    .set = __LWM2M_Security_Bootstrap_Server_set,
  },
  .resources[2] = { 
    .supported = true,  
    .id = 2,
    .operation = LWM2M_OPERATION_NONE,
    .instances = LWM2M_INSTANCETYPE_single,
    .mandatory = true,
    .type = LWM2M_TYPE_integer,
    .changed = LWM2M_Security_Security_Mode_changed,
    .execute = (void *) 0,
    .get = __LWM2M_Security_Security_Mode_get,
    .set = __LWM2M_Security_Security_Mode_set,
  },
  .resources[3] = { 
    .supported = true,  
    .id = 3,
    .operation = LWM2M_OPERATION_NONE,
    .instances = LWM2M_INSTANCETYPE_single,
    .mandatory = true,
    .type = LWM2M_TYPE_opaque,
    .changed = LWM2M_Security_Public_Key_or_Identity_changed,
    .execute = (void *) 0,
    .get = __LWM2M_Security_Public_Key_or_Identity_get,
    .set = __LWM2M_Security_Public_Key_or_Identity_set,
  },
  .resources[4] = { 
    .supported = true,  
    .id = 4,
    .operation = LWM2M_OPERATION_NONE,
    .instances = LWM2M_INSTANCETYPE_single,
    .mandatory = true,
    .type = LWM2M_TYPE_opaque,
    .changed = LWM2M_Security_Server_Public_Key_or_Identity_changed,
    .execute = (void *) 0,
    .get = __LWM2M_Security_Server_Public_Key_or_Identity_get,
    .set = __LWM2M_Security_Server_Public_Key_or_Identity_set,
  },
  .resources[5] = { 
    .supported = true,  
    .id = 5,
    .operation = LWM2M_OPERATION_NONE,
    .instances = LWM2M_INSTANCETYPE_single,
    .mandatory = true,
    .type = LWM2M_TYPE_opaque,
    .changed = LWM2M_Security_Secret_Key_changed,
    .execute = (void *) 0,
    .get = __LWM2M_Security_Secret_Key_get,
    .set = __LWM2M_Security_Secret_Key_set,
  },
  .resources[6] = { 
    .supported = true,  
    .id = 6,
    .operation = LWM2M_OPERATION_NONE,
    .instances = LWM2M_INSTANCETYPE_single,
    .mandatory = true,
    .type = LWM2M_TYPE_integer,
    .changed = LWM2M_Security_SMS_Security_Mode_changed,
    .execute = (void *) 0,
    .get = __LWM2M_Security_SMS_Security_Mode_get,
    .set = __LWM2M_Security_SMS_Security_Mode_set,
  },
  .resources[7] = { 
    .supported = true,  
    .id = 7,
    .operation = LWM2M_OPERATION_NONE,
    .instances = LWM2M_INSTANCETYPE_single,
    .mandatory = true,
    .type = LWM2M_TYPE_opaque,
    .changed = LWM2M_Security_SMS_Binding_Key_Parameters_changed,
    .execute = (void *) 0,
    .get = __LWM2M_Security_SMS_Binding_Key_Parameters_get,
    .set = __LWM2M_Security_SMS_Binding_Key_Parameters_set,
  },
  .resources[8] = { 
    .supported = true,  
    .id = 8,
    .operation = LWM2M_OPERATION_NONE,
    .instances = LWM2M_INSTANCETYPE_single,
    .mandatory = true,
    .type = LWM2M_TYPE_opaque,
    .changed = LWM2M_Security_SMS_Binding_Secret_Keys_changed,
    .execute = (void *) 0,
    .get = __LWM2M_Security_SMS_Binding_Secret_Keys_get,
    .set = __LWM2M_Security_SMS_Binding_Secret_Keys_set,
  },
  .resources[9] = { 
    .supported = true,  
    .id = 9,
    .operation = LWM2M_OPERATION_NONE,
    .instances = LWM2M_INSTANCETYPE_single,
    .mandatory = true,
    .type = LWM2M_TYPE_integer,
    .changed = LWM2M_Security_LWM2M_Server_SMS_Number_changed,
    .execute = (void *) 0,
    .get = __LWM2M_Security_LWM2M_Server_SMS_Number_get,
    .set = __LWM2M_Security_LWM2M_Server_SMS_Number_set,
  },
#if CONFIG_LWM2M_Security_Short_Server_ID_OPTION == 1
  .resources[10] = { 
    .supported = true,  
    .id = 10,
    .operation = LWM2M_OPERATION_NONE,
    .instances = LWM2M_INSTANCETYPE_single,
    .mandatory = false,
    .type = LWM2M_TYPE_integer,
    .changed = LWM2M_Security_Short_Server_ID_changed,
    .execute = (void *) 0,
    .get = __LWM2M_Security_Short_Server_ID_get,
    .set = __LWM2M_Security_Short_Server_ID_set,
  },
#else
  .resources[10] = { .supported = false },
#endif
  .resources[11] = { 
    .supported = true,  
    .id = 11,
    .operation = LWM2M_OPERATION_NONE,
    .instances = LWM2M_INSTANCETYPE_single,
    .mandatory = true,
    .type = LWM2M_TYPE_integer,
    .changed = LWM2M_Security_Client_Hold_Off_Time_changed,
    .execute = (void *) 0,
    .get = __LWM2M_Security_Client_Hold_Off_Time_get,
    .set = __LWM2M_Security_Client_Hold_Off_Time_set,
  },
};


void LWM2M_Security_lock() 
{ 
  mutex_lock(&LWM2M_Security_object_declaration.lock, MUTEXT_WAIT_FOREVER); 
}
void LWM2M_Security_unlock() 
{ 
  mutex_unlock(&LWM2M_Security_object_declaration.lock);
}


uint16_t LWM2M_Security_get_index_for_instance(uint16_t instance)
{
  for (int i = 0; i < MAX_LWM2M_Security_INSTANCES; i++)
  {
    if (LWM2M_Security_instances[i].id == instance) return i;
  }
  return LWM2M_INVALID_INSTANCE;
}

__attribute__((weak)) lwm2m_object_t *LWM2M_Security_initialise(void)
{
  mutex_initialise(&LWM2M_Security_object_declaration.lock);
  
  memset(LWM2M_Security_instances, 0, sizeof(LWM2M_Security_instances));
  for (int i = 0; i < MAX_LWM2M_Security_INSTANCES; i++)
  {
    LWM2M_Security_instances[i].id = LWM2M_INVALID_INSTANCE;
    LWM2M_Security_instances[i].index = i;
  }
  lwm2m_object_base_initialse_object(&LWM2M_Security_lwm2m_object);
  LWM2M_Security_lwm2m_object.userData = &LWM2M_Security_object_declaration; 
  return &LWM2M_Security_lwm2m_object;
}
  
__attribute__((weak)) void LWM2M_Security_register_object(lwm2m_context_t *context)
{
  LWM2M_Security_lock();
  LWM2M_Security_object_declaration.context = context;
  LWM2M_Security_unlock();
}
  
__attribute__((weak)) void LWM2M_Security_register_instance(uint16_t instance_id)
{
  LWM2M_Security_lock();
  uint16_t index = LWM2M_Security_get_index_for_instance(instance_id);
  assert_void(index != LWM2M_INVALID_INSTANCE);
  
  if ((LWM2M_Security_instances[index].id !=  LWM2M_INVALID_INSTANCE) && (!LWM2M_Security_instances[index].registered))
  {
    LWM2M_Security_lwm2m_object.instanceList = LWM2M_LIST_ADD(LWM2M_Security_lwm2m_object.instanceList, (lwm2m_list_t *)&LWM2M_Security_instances[index]);
    LWM2M_Security_instances[index].registered = true;
  }
  LWM2M_Security_unlock();
}

__attribute__((weak)) void LWM2M_Security_unregister_instance(uint16_t instance_id)
{
  LWM2M_Security_lock();
  uint16_t index = LWM2M_Security_get_index_for_instance(instance_id);
  assert_void(index != LWM2M_INVALID_INSTANCE);
  
  if ((LWM2M_Security_instances[index].id !=  LWM2M_INVALID_INSTANCE) && (LWM2M_Security_instances[index].registered))
  {
    LWM2M_Security_instance *instance = 0;
    LWM2M_Security_lwm2m_object.instanceList = lwm2m_list_remove(LWM2M_Security_lwm2m_object.instanceList, instance_id, (lwm2m_list_t **)&instance);
    LWM2M_Security_instances[index].registered = false;
    if (instance != 0)
    { 
      instance->next = 0;
    }
  }
  LWM2M_Security_unlock();
}

//override this to perform actions when a new instance is created
__attribute__((weak)) void LWM2M_Security_instance_created(uint16_t instance_id)
{
  
}
  

//called when the server requests an object to be created; we need to create AND register the 
//instance
uint8_t __LWM2M_Security_create_instance(uint16_t instance_id)
{
  uint8_t res = LWM2M_Security_create_instance(instance_id);
  if (res == COAP_201_CREATED)
  {
    LWM2M_Security_register_instance(instance_id);
  }
  return res;
}
  
__attribute__((weak)) uint8_t LWM2M_Security_create_instance(uint16_t instance_id)
{
  LWM2M_Security_lock();
  //find an unallocated instance
  for (int i = 0; i < MAX_LWM2M_Security_INSTANCES; i++)
  {
    if (LWM2M_Security_instances[i].id == LWM2M_INVALID_INSTANCE)
    {
      LWM2M_Security_instances[i].id = instance_id;
      LWM2M_Security_instance_created(instance_id);
      LWM2M_Security_unlock();
      return COAP_201_CREATED;
    }
  }
  LWM2M_Security_unlock();
  return COAP_500_INTERNAL_SERVER_ERROR; //none found
}

void LWM2M_Security_foreach(lwm2m_each_instance_func func, void *data)
{
  LWM2M_Security_lock();
  
  for (int i = 0; i < MAX_LWM2M_Security_INSTANCES; i++)
  {
    if (LWM2M_Security_instances[i].id != LWM2M_INVALID_INSTANCE)
    {
      if (!func(LWM2M_Security_instances[i].id, data)) break;
    }
  }
  LWM2M_Security_unlock();
}
  
//provides access to a user data pointer for storing additional instance
//data.
void *LWM2M_Security_get_user_data(uint16_t instance_id)
{
  uint16_t index = LWM2M_Security_get_index_for_instance(instance_id);
  if (index == LWM2M_INVALID_INSTANCE) return 0;
  return LWM2M_Security_instances[index].user_data;
}
  
bool LWM2M_Security_set_user_data(uint16_t instance_id, void *data)
{
  uint16_t index = LWM2M_Security_get_index_for_instance(instance_id);
  if (index == LWM2M_INVALID_INSTANCE) return false;
  LWM2M_Security_instances[index].user_data = data;
  return true;
}
  
//override this to perform actions when a new instance is created
__attribute__((weak)) void LWM2M_Security_instance_deleted(uint16_t instance_id)
{
  
}

__attribute__((weak)) uint8_t LWM2M_Security_delete_instance(uint16_t instance_id)
{
  LWM2M_Security_lock();
  uint16_t index = LWM2M_Security_get_index_for_instance(instance_id);
  assert(index != LWM2M_INVALID_INSTANCE);
  
  if ((instance_id < MAX_LWM2M_Security_INSTANCES) && (LWM2M_Security_instances[index].id !=  LWM2M_INVALID_INSTANCE))
  {
    LWM2M_Security_unregister_instance(instance_id);
    LWM2M_Security_instance_deleted(instance_id);
    LWM2M_Security_instances[index].id = LWM2M_INVALID_INSTANCE;
  }
  LWM2M_Security_unlock();
  return COAP_202_DELETED;
}


__attribute__((weak)) void LWM2M_Security_changed(uint16_t instance)
{
  
}

  
__attribute__((weak))  uint8_t LWM2M_Security_LWM2M_Server_URI_get(uint16_t instance,  const char **val)
{
  LWM2M_Security_lock();
  assert(instance >= 0 && instance < MAX_LWM2M_Security_INSTANCES);
  uint16_t index = LWM2M_Security_get_index_for_instance(instance);
  assert(index != LWM2M_INVALID_INSTANCE);
  
  assert(LWM2M_Security_instances[index].id != LWM2M_INVALID_INSTANCE);
  
  
  *val = LWM2M_Security_instances[index].LWM2M_Server_URI;
  LWM2M_Security_unlock();
  return COAP_205_CONTENT;
}
static uint8_t __LWM2M_Security_LWM2M_Server_URI_get(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data)
{
  const char *val;
  uint8_t res = LWM2M_Security_LWM2M_Server_URI_get(instance,  &val);
  lwm2m_data_encode_string(val, data);
  return res;
}
                                         
__attribute__((weak)) uint8_t LWM2M_Security_LWM2M_Server_URI_set(uint16_t instance,  const char *val, size_t length, bool notify)
{
  LWM2M_Security_lock();
  assert(instance >= 0 && instance < MAX_LWM2M_Security_INSTANCES);
  uint16_t index = LWM2M_Security_get_index_for_instance(instance);
  assert(index != LWM2M_INVALID_INSTANCE);
  
  assert(LWM2M_Security_instances[index].id != LWM2M_INVALID_INSTANCE);
 
  if (length >= sizeof(LWM2M_Security_instances[index].LWM2M_Server_URI )) length = sizeof(LWM2M_Security_instances[index].LWM2M_Server_URI) - 1;
  memcpy(LWM2M_Security_instances[index].LWM2M_Server_URI, val, length);
  LWM2M_Security_instances[index].LWM2M_Server_URI[length] = 0;
  
  
  if (notify && LWM2M_Security_instances[index].registered) lwm2m_object_base_resource_changed(&LWM2M_Security_lwm2m_object, instance, 0, false);
  LWM2M_Security_unlock();
  return COAP_204_CHANGED;
}
static uint8_t __LWM2M_Security_LWM2M_Server_URI_set(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data)
{
  return LWM2M_Security_LWM2M_Server_URI_set(instance,  (char*)data->value.asBuffer.buffer, data->value.asBuffer.length, false);
}
__attribute__((weak)) uint8_t LWM2M_Security_LWM2M_Server_URI_changed(uint16_t instance)
{
  LWM2M_Security_lock();
  assert(instance >= 0 && instance < MAX_LWM2M_Security_INSTANCES);
  uint16_t index = LWM2M_Security_get_index_for_instance(instance);
  assert(index != LWM2M_INVALID_INSTANCE);
  
  LWM2M_Security_changed(instance);
  LWM2M_Security_unlock();
  return COAP_NO_ERROR;
}
  
                                         
__attribute__((weak)) uint8_t LWM2M_Security_Bootstrap_Server_get(uint16_t instance,  bool *val)
{
  LWM2M_Security_lock();
  assert(instance >= 0 && instance < MAX_LWM2M_Security_INSTANCES);
  uint16_t index = LWM2M_Security_get_index_for_instance(instance);
  assert(index != LWM2M_INVALID_INSTANCE);
  
  assert(LWM2M_Security_instances[index].id != LWM2M_INVALID_INSTANCE);
  
  
  *val = LWM2M_Security_instances[index].Bootstrap_Server;
  LWM2M_Security_unlock();
  return COAP_205_CONTENT;
}
static uint8_t __LWM2M_Security_Bootstrap_Server_get(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data)
{
  bool val;
  uint8_t res = LWM2M_Security_Bootstrap_Server_get(instance,  &val);
  lwm2m_data_encode_bool(val, data);
  return res;
}
__attribute__((weak)) uint8_t LWM2M_Security_Bootstrap_Server_set(uint16_t instance,  bool val, bool notify)
{
  LWM2M_Security_lock();
  assert(instance >= 0 && instance < MAX_LWM2M_Security_INSTANCES);
  uint16_t index = LWM2M_Security_get_index_for_instance(instance);
  assert(index != LWM2M_INVALID_INSTANCE);
  
  assert(LWM2M_Security_instances[index].id != LWM2M_INVALID_INSTANCE);

  LWM2M_Security_instances[index].Bootstrap_Server = val;  
  
  
  if (notify && LWM2M_Security_instances[index].registered) lwm2m_object_base_resource_changed(&LWM2M_Security_lwm2m_object, instance, 1, false);
  LWM2M_Security_unlock();
  return COAP_204_CHANGED;
}
static uint8_t __LWM2M_Security_Bootstrap_Server_set(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data)
{
  bool val;
  lwm2m_data_decode_bool(data, &val);
  return LWM2M_Security_Bootstrap_Server_set(instance, val, false);
}
__attribute__((weak)) uint8_t LWM2M_Security_Bootstrap_Server_changed(uint16_t instance)
{
  LWM2M_Security_lock();
  assert(instance >= 0 && instance < MAX_LWM2M_Security_INSTANCES);
  uint16_t index = LWM2M_Security_get_index_for_instance(instance);
  assert(index != LWM2M_INVALID_INSTANCE);
  
  LWM2M_Security_changed(instance);
  LWM2M_Security_unlock();
  return COAP_NO_ERROR;
}
  
__attribute__((weak)) uint8_t LWM2M_Security_Security_Mode_get(uint16_t instance,  uint32_t *val)
{
  LWM2M_Security_lock();
  assert(instance >= 0 && instance < MAX_LWM2M_Security_INSTANCES);
  uint16_t index = LWM2M_Security_get_index_for_instance(instance);
  assert(index != LWM2M_INVALID_INSTANCE);
  
  assert(LWM2M_Security_instances[index].id != LWM2M_INVALID_INSTANCE);
  
  
  *val = LWM2M_Security_instances[index].Security_Mode;
  LWM2M_Security_unlock();
  return COAP_205_CONTENT;
}
static uint8_t __LWM2M_Security_Security_Mode_get(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data)
{
  uint32_t val;
  uint8_t res = LWM2M_Security_Security_Mode_get(instance,  &val);
  lwm2m_data_encode_int(val, data);
  return res;
}
                                         
__attribute__((weak)) uint8_t LWM2M_Security_Security_Mode_set(uint16_t instance,  uint32_t val, bool notify)
{
  LWM2M_Security_lock();
  assert(instance >= 0 && instance < MAX_LWM2M_Security_INSTANCES);
  uint16_t index = LWM2M_Security_get_index_for_instance(instance);
  assert(index != LWM2M_INVALID_INSTANCE);
  
  assert(LWM2M_Security_instances[index].id != LWM2M_INVALID_INSTANCE);

  LWM2M_Security_instances[index].Security_Mode = val;
  
  if (notify && LWM2M_Security_instances[index].registered) lwm2m_object_base_resource_changed(&LWM2M_Security_lwm2m_object, instance, 2, false);  
  LWM2M_Security_unlock();
  return COAP_204_CHANGED;
}
static uint8_t __LWM2M_Security_Security_Mode_set(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data)
{
  int64_t val;
  lwm2m_data_decode_int(data, &val);
  return LWM2M_Security_Security_Mode_set(instance,  val, false);
}
__attribute__((weak)) uint8_t LWM2M_Security_Security_Mode_changed(uint16_t instance)
{
  LWM2M_Security_lock();
  assert(instance >= 0 && instance < MAX_LWM2M_Security_INSTANCES);
  uint16_t index = LWM2M_Security_get_index_for_instance(instance);
  assert(index != LWM2M_INVALID_INSTANCE);
  
  LWM2M_Security_changed(instance);
  LWM2M_Security_unlock();
  return COAP_NO_ERROR;
}
  
__attribute__((weak))  uint8_t LWM2M_Security_Public_Key_or_Identity_get(uint16_t instance,  uint8_t **val, size_t *length)
{
  LWM2M_Security_lock();
  assert(instance >= 0 && instance < MAX_LWM2M_Security_INSTANCES);
  uint16_t index = LWM2M_Security_get_index_for_instance(instance);
  assert(index != LWM2M_INVALID_INSTANCE);
  
  assert(LWM2M_Security_instances[index].id != LWM2M_INVALID_INSTANCE);
  
  
  
  *val = LWM2M_Security_instances[index].Public_Key_or_Identity;
  *length = LWM2M_Security_instances[index].Public_Key_or_Identity_length;
  LWM2M_Security_unlock();
  return COAP_205_CONTENT;
}
static uint8_t __LWM2M_Security_Public_Key_or_Identity_get(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data)
{
  uint8_t *val;
  size_t length = 0;
  
  uint8_t res = LWM2M_Security_Public_Key_or_Identity_get(instance,  &val, &length);
  
  lwm2m_data_encode_opaque(val, length, data);
  return res;
}
                                         
 __attribute__((weak)) uint8_t LWM2M_Security_Public_Key_or_Identity_set(uint16_t instance,  uint8_t* val, size_t length, bool notify)
 {
   LWM2M_Security_lock();
   assert(instance >= 0 && instance < MAX_LWM2M_Security_INSTANCES);
 
   uint16_t index = LWM2M_Security_get_index_for_instance(instance);
   assert(index != LWM2M_INVALID_INSTANCE);
   
   if (LWM2M_Security_instances[index].Public_Key_or_Identity != 0) lwm2m_free(LWM2M_Security_instances[index].Public_Key_or_Identity);
   LWM2M_Security_instances[index].Public_Key_or_Identity = lwm2m_malloc(length);
   if (LWM2M_Security_instances[index].Public_Key_or_Identity == 0)
   {
     LWM2M_Security_instances[index].Public_Key_or_Identity_length  = 0;   
     LWM2M_Security_unlock();
     return COAP_500_INTERNAL_SERVER_ERROR;                                     
   } 
   LWM2M_Security_instances[index].Public_Key_or_Identity_length  = length;
   memcpy(LWM2M_Security_instances[index].Public_Key_or_Identity, val, length);                                      
                                  
   
  
   if (notify && LWM2M_Security_instances[index].registered) lwm2m_object_base_resource_changed(&LWM2M_Security_lwm2m_object, instance, 3, false);
  
  
   LWM2M_Security_unlock();
   return COAP_204_CHANGED;
 }
 static uint8_t __LWM2M_Security_Public_Key_or_Identity_set(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data)
 {
   return LWM2M_Security_Public_Key_or_Identity_set(instance,  (uint8_t *)data->value.asBuffer.buffer,data->value.asBuffer.length, false);
 }
__attribute__((weak)) uint8_t LWM2M_Security_Public_Key_or_Identity_changed(uint16_t instance)
{
  LWM2M_Security_lock();
  assert(instance >= 0 && instance < MAX_LWM2M_Security_INSTANCES);
  uint16_t index = LWM2M_Security_get_index_for_instance(instance);
  assert(index != LWM2M_INVALID_INSTANCE);
  
  LWM2M_Security_changed(instance);
  LWM2M_Security_unlock();
  return COAP_NO_ERROR;
}
  
__attribute__((weak))  uint8_t LWM2M_Security_Server_Public_Key_or_Identity_get(uint16_t instance,  uint8_t **val, size_t *length)
{
  LWM2M_Security_lock();
  assert(instance >= 0 && instance < MAX_LWM2M_Security_INSTANCES);
  uint16_t index = LWM2M_Security_get_index_for_instance(instance);
  assert(index != LWM2M_INVALID_INSTANCE);
  
  assert(LWM2M_Security_instances[index].id != LWM2M_INVALID_INSTANCE);
  
  
  
  *val = LWM2M_Security_instances[index].Server_Public_Key_or_Identity;
  *length = LWM2M_Security_instances[index].Server_Public_Key_or_Identity_length;
  LWM2M_Security_unlock();
  return COAP_205_CONTENT;
}
static uint8_t __LWM2M_Security_Server_Public_Key_or_Identity_get(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data)
{
  uint8_t *val;
  size_t length = 0;
  
  uint8_t res = LWM2M_Security_Server_Public_Key_or_Identity_get(instance,  &val, &length);
  
  lwm2m_data_encode_opaque(val, length, data);
  return res;
}
                                         
 __attribute__((weak)) uint8_t LWM2M_Security_Server_Public_Key_or_Identity_set(uint16_t instance,  uint8_t* val, size_t length, bool notify)
 {
   LWM2M_Security_lock();
   assert(instance >= 0 && instance < MAX_LWM2M_Security_INSTANCES);
 
   uint16_t index = LWM2M_Security_get_index_for_instance(instance);
   assert(index != LWM2M_INVALID_INSTANCE);
   
   if (LWM2M_Security_instances[index].Server_Public_Key_or_Identity != 0) lwm2m_free(LWM2M_Security_instances[index].Server_Public_Key_or_Identity);
   LWM2M_Security_instances[index].Server_Public_Key_or_Identity = lwm2m_malloc(length);
   if (LWM2M_Security_instances[index].Server_Public_Key_or_Identity == 0)
   {
     LWM2M_Security_instances[index].Server_Public_Key_or_Identity_length  = 0;   
     LWM2M_Security_unlock();
     return COAP_500_INTERNAL_SERVER_ERROR;                                     
   } 
   LWM2M_Security_instances[index].Server_Public_Key_or_Identity_length  = length;
   memcpy(LWM2M_Security_instances[index].Server_Public_Key_or_Identity, val, length);                                      
                                  
   
  
   if (notify && LWM2M_Security_instances[index].registered) lwm2m_object_base_resource_changed(&LWM2M_Security_lwm2m_object, instance, 4, false);
  
  
   LWM2M_Security_unlock();
   return COAP_204_CHANGED;
 }
 static uint8_t __LWM2M_Security_Server_Public_Key_or_Identity_set(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data)
 {
   return LWM2M_Security_Server_Public_Key_or_Identity_set(instance,  (uint8_t *)data->value.asBuffer.buffer,data->value.asBuffer.length, false);
 }
__attribute__((weak)) uint8_t LWM2M_Security_Server_Public_Key_or_Identity_changed(uint16_t instance)
{
  LWM2M_Security_lock();
  assert(instance >= 0 && instance < MAX_LWM2M_Security_INSTANCES);
  uint16_t index = LWM2M_Security_get_index_for_instance(instance);
  assert(index != LWM2M_INVALID_INSTANCE);
  
  LWM2M_Security_changed(instance);
  LWM2M_Security_unlock();
  return COAP_NO_ERROR;
}
  
__attribute__((weak))  uint8_t LWM2M_Security_Secret_Key_get(uint16_t instance,  uint8_t **val, size_t *length)
{
  LWM2M_Security_lock();
  assert(instance >= 0 && instance < MAX_LWM2M_Security_INSTANCES);
  uint16_t index = LWM2M_Security_get_index_for_instance(instance);
  assert(index != LWM2M_INVALID_INSTANCE);
  
  assert(LWM2M_Security_instances[index].id != LWM2M_INVALID_INSTANCE);
  
  
  
  *val = LWM2M_Security_instances[index].Secret_Key;
  *length = LWM2M_Security_instances[index].Secret_Key_length;
  LWM2M_Security_unlock();
  return COAP_205_CONTENT;
}
static uint8_t __LWM2M_Security_Secret_Key_get(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data)
{
  uint8_t *val;
  size_t length = 0;
  
  uint8_t res = LWM2M_Security_Secret_Key_get(instance,  &val, &length);
  
  lwm2m_data_encode_opaque(val, length, data);
  return res;
}
                                         
 __attribute__((weak)) uint8_t LWM2M_Security_Secret_Key_set(uint16_t instance,  uint8_t* val, size_t length, bool notify)
 {
   LWM2M_Security_lock();
   assert(instance >= 0 && instance < MAX_LWM2M_Security_INSTANCES);
 
   uint16_t index = LWM2M_Security_get_index_for_instance(instance);
   assert(index != LWM2M_INVALID_INSTANCE);
   
   if (LWM2M_Security_instances[index].Secret_Key != 0) lwm2m_free(LWM2M_Security_instances[index].Secret_Key);
   LWM2M_Security_instances[index].Secret_Key = lwm2m_malloc(length);
   if (LWM2M_Security_instances[index].Secret_Key == 0)
   {
     LWM2M_Security_instances[index].Secret_Key_length  = 0;   
     LWM2M_Security_unlock();
     return COAP_500_INTERNAL_SERVER_ERROR;                                     
   } 
   LWM2M_Security_instances[index].Secret_Key_length  = length;
   memcpy(LWM2M_Security_instances[index].Secret_Key, val, length);                                      
                                  
   
  
   if (notify && LWM2M_Security_instances[index].registered) lwm2m_object_base_resource_changed(&LWM2M_Security_lwm2m_object, instance, 5, false);
  
  
   LWM2M_Security_unlock();
   return COAP_204_CHANGED;
 }
 static uint8_t __LWM2M_Security_Secret_Key_set(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data)
 {
   return LWM2M_Security_Secret_Key_set(instance,  (uint8_t *)data->value.asBuffer.buffer,data->value.asBuffer.length, false);
 }
__attribute__((weak)) uint8_t LWM2M_Security_Secret_Key_changed(uint16_t instance)
{
  LWM2M_Security_lock();
  assert(instance >= 0 && instance < MAX_LWM2M_Security_INSTANCES);
  uint16_t index = LWM2M_Security_get_index_for_instance(instance);
  assert(index != LWM2M_INVALID_INSTANCE);
  
  LWM2M_Security_changed(instance);
  LWM2M_Security_unlock();
  return COAP_NO_ERROR;
}
  
__attribute__((weak)) uint8_t LWM2M_Security_SMS_Security_Mode_get(uint16_t instance,  uint32_t *val)
{
  LWM2M_Security_lock();
  assert(instance >= 0 && instance < MAX_LWM2M_Security_INSTANCES);
  uint16_t index = LWM2M_Security_get_index_for_instance(instance);
  assert(index != LWM2M_INVALID_INSTANCE);
  
  assert(LWM2M_Security_instances[index].id != LWM2M_INVALID_INSTANCE);
  
  
  *val = LWM2M_Security_instances[index].SMS_Security_Mode;
  LWM2M_Security_unlock();
  return COAP_205_CONTENT;
}
static uint8_t __LWM2M_Security_SMS_Security_Mode_get(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data)
{
  uint32_t val;
  uint8_t res = LWM2M_Security_SMS_Security_Mode_get(instance,  &val);
  lwm2m_data_encode_int(val, data);
  return res;
}
                                         
__attribute__((weak)) uint8_t LWM2M_Security_SMS_Security_Mode_set(uint16_t instance,  uint32_t val, bool notify)
{
  LWM2M_Security_lock();
  assert(instance >= 0 && instance < MAX_LWM2M_Security_INSTANCES);
  uint16_t index = LWM2M_Security_get_index_for_instance(instance);
  assert(index != LWM2M_INVALID_INSTANCE);
  
  assert(LWM2M_Security_instances[index].id != LWM2M_INVALID_INSTANCE);

  LWM2M_Security_instances[index].SMS_Security_Mode = val;
  
  if (notify && LWM2M_Security_instances[index].registered) lwm2m_object_base_resource_changed(&LWM2M_Security_lwm2m_object, instance, 6, false);  
  LWM2M_Security_unlock();
  return COAP_204_CHANGED;
}
static uint8_t __LWM2M_Security_SMS_Security_Mode_set(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data)
{
  int64_t val;
  lwm2m_data_decode_int(data, &val);
  return LWM2M_Security_SMS_Security_Mode_set(instance,  val, false);
}
__attribute__((weak)) uint8_t LWM2M_Security_SMS_Security_Mode_changed(uint16_t instance)
{
  LWM2M_Security_lock();
  assert(instance >= 0 && instance < MAX_LWM2M_Security_INSTANCES);
  uint16_t index = LWM2M_Security_get_index_for_instance(instance);
  assert(index != LWM2M_INVALID_INSTANCE);
  
  LWM2M_Security_changed(instance);
  LWM2M_Security_unlock();
  return COAP_NO_ERROR;
}
  
__attribute__((weak))  uint8_t LWM2M_Security_SMS_Binding_Key_Parameters_get(uint16_t instance,  uint8_t **val, size_t *length)
{
  LWM2M_Security_lock();
  assert(instance >= 0 && instance < MAX_LWM2M_Security_INSTANCES);
  uint16_t index = LWM2M_Security_get_index_for_instance(instance);
  assert(index != LWM2M_INVALID_INSTANCE);
  
  assert(LWM2M_Security_instances[index].id != LWM2M_INVALID_INSTANCE);
  
  
  
  *val = LWM2M_Security_instances[index].SMS_Binding_Key_Parameters;
  *length = LWM2M_Security_instances[index].SMS_Binding_Key_Parameters_length;
  LWM2M_Security_unlock();
  return COAP_205_CONTENT;
}
static uint8_t __LWM2M_Security_SMS_Binding_Key_Parameters_get(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data)
{
  uint8_t *val;
  size_t length = 0;
  
  uint8_t res = LWM2M_Security_SMS_Binding_Key_Parameters_get(instance,  &val, &length);
  
  lwm2m_data_encode_opaque(val, length, data);
  return res;
}
                                         
 __attribute__((weak)) uint8_t LWM2M_Security_SMS_Binding_Key_Parameters_set(uint16_t instance,  uint8_t* val, size_t length, bool notify)
 {
   LWM2M_Security_lock();
   assert(instance >= 0 && instance < MAX_LWM2M_Security_INSTANCES);
 
   uint16_t index = LWM2M_Security_get_index_for_instance(instance);
   assert(index != LWM2M_INVALID_INSTANCE);
   
   if (LWM2M_Security_instances[index].SMS_Binding_Key_Parameters != 0) lwm2m_free(LWM2M_Security_instances[index].SMS_Binding_Key_Parameters);
   LWM2M_Security_instances[index].SMS_Binding_Key_Parameters = lwm2m_malloc(length);
   if (LWM2M_Security_instances[index].SMS_Binding_Key_Parameters == 0)
   {
     LWM2M_Security_instances[index].SMS_Binding_Key_Parameters_length  = 0;   
     LWM2M_Security_unlock();
     return COAP_500_INTERNAL_SERVER_ERROR;                                     
   } 
   LWM2M_Security_instances[index].SMS_Binding_Key_Parameters_length  = length;
   memcpy(LWM2M_Security_instances[index].SMS_Binding_Key_Parameters, val, length);                                      
                                  
   
  
   if (notify && LWM2M_Security_instances[index].registered) lwm2m_object_base_resource_changed(&LWM2M_Security_lwm2m_object, instance, 7, false);
  
  
   LWM2M_Security_unlock();
   return COAP_204_CHANGED;
 }
 static uint8_t __LWM2M_Security_SMS_Binding_Key_Parameters_set(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data)
 {
   return LWM2M_Security_SMS_Binding_Key_Parameters_set(instance,  (uint8_t *)data->value.asBuffer.buffer,data->value.asBuffer.length, false);
 }
__attribute__((weak)) uint8_t LWM2M_Security_SMS_Binding_Key_Parameters_changed(uint16_t instance)
{
  LWM2M_Security_lock();
  assert(instance >= 0 && instance < MAX_LWM2M_Security_INSTANCES);
  uint16_t index = LWM2M_Security_get_index_for_instance(instance);
  assert(index != LWM2M_INVALID_INSTANCE);
  
  LWM2M_Security_changed(instance);
  LWM2M_Security_unlock();
  return COAP_NO_ERROR;
}
  
__attribute__((weak))  uint8_t LWM2M_Security_SMS_Binding_Secret_Keys_get(uint16_t instance,  uint8_t **val, size_t *length)
{
  LWM2M_Security_lock();
  assert(instance >= 0 && instance < MAX_LWM2M_Security_INSTANCES);
  uint16_t index = LWM2M_Security_get_index_for_instance(instance);
  assert(index != LWM2M_INVALID_INSTANCE);
  
  assert(LWM2M_Security_instances[index].id != LWM2M_INVALID_INSTANCE);
  
  
  
  *val = LWM2M_Security_instances[index].SMS_Binding_Secret_Keys;
  *length = LWM2M_Security_instances[index].SMS_Binding_Secret_Keys_length;
  LWM2M_Security_unlock();
  return COAP_205_CONTENT;
}
static uint8_t __LWM2M_Security_SMS_Binding_Secret_Keys_get(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data)
{
  uint8_t *val;
  size_t length = 0;
  
  uint8_t res = LWM2M_Security_SMS_Binding_Secret_Keys_get(instance,  &val, &length);
  
  lwm2m_data_encode_opaque(val, length, data);
  return res;
}
                                         
 __attribute__((weak)) uint8_t LWM2M_Security_SMS_Binding_Secret_Keys_set(uint16_t instance,  uint8_t* val, size_t length, bool notify)
 {
   LWM2M_Security_lock();
   assert(instance >= 0 && instance < MAX_LWM2M_Security_INSTANCES);
 
   uint16_t index = LWM2M_Security_get_index_for_instance(instance);
   assert(index != LWM2M_INVALID_INSTANCE);
   
   if (LWM2M_Security_instances[index].SMS_Binding_Secret_Keys != 0) lwm2m_free(LWM2M_Security_instances[index].SMS_Binding_Secret_Keys);
   LWM2M_Security_instances[index].SMS_Binding_Secret_Keys = lwm2m_malloc(length);
   if (LWM2M_Security_instances[index].SMS_Binding_Secret_Keys == 0)
   {
     LWM2M_Security_instances[index].SMS_Binding_Secret_Keys_length  = 0;   
     LWM2M_Security_unlock();
     return COAP_500_INTERNAL_SERVER_ERROR;                                     
   } 
   LWM2M_Security_instances[index].SMS_Binding_Secret_Keys_length  = length;
   memcpy(LWM2M_Security_instances[index].SMS_Binding_Secret_Keys, val, length);                                      
                                  
   
  
   if (notify && LWM2M_Security_instances[index].registered) lwm2m_object_base_resource_changed(&LWM2M_Security_lwm2m_object, instance, 8, false);
  
  
   LWM2M_Security_unlock();
   return COAP_204_CHANGED;
 }
 static uint8_t __LWM2M_Security_SMS_Binding_Secret_Keys_set(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data)
 {
   return LWM2M_Security_SMS_Binding_Secret_Keys_set(instance,  (uint8_t *)data->value.asBuffer.buffer,data->value.asBuffer.length, false);
 }
__attribute__((weak)) uint8_t LWM2M_Security_SMS_Binding_Secret_Keys_changed(uint16_t instance)
{
  LWM2M_Security_lock();
  assert(instance >= 0 && instance < MAX_LWM2M_Security_INSTANCES);
  uint16_t index = LWM2M_Security_get_index_for_instance(instance);
  assert(index != LWM2M_INVALID_INSTANCE);
  
  LWM2M_Security_changed(instance);
  LWM2M_Security_unlock();
  return COAP_NO_ERROR;
}
  
__attribute__((weak)) uint8_t LWM2M_Security_LWM2M_Server_SMS_Number_get(uint16_t instance,  uint32_t *val)
{
  LWM2M_Security_lock();
  assert(instance >= 0 && instance < MAX_LWM2M_Security_INSTANCES);
  uint16_t index = LWM2M_Security_get_index_for_instance(instance);
  assert(index != LWM2M_INVALID_INSTANCE);
  
  assert(LWM2M_Security_instances[index].id != LWM2M_INVALID_INSTANCE);
  
  
  *val = LWM2M_Security_instances[index].LWM2M_Server_SMS_Number;
  LWM2M_Security_unlock();
  return COAP_205_CONTENT;
}
static uint8_t __LWM2M_Security_LWM2M_Server_SMS_Number_get(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data)
{
  uint32_t val;
  uint8_t res = LWM2M_Security_LWM2M_Server_SMS_Number_get(instance,  &val);
  lwm2m_data_encode_int(val, data);
  return res;
}
                                         
__attribute__((weak)) uint8_t LWM2M_Security_LWM2M_Server_SMS_Number_set(uint16_t instance,  uint32_t val, bool notify)
{
  LWM2M_Security_lock();
  assert(instance >= 0 && instance < MAX_LWM2M_Security_INSTANCES);
  uint16_t index = LWM2M_Security_get_index_for_instance(instance);
  assert(index != LWM2M_INVALID_INSTANCE);
  
  assert(LWM2M_Security_instances[index].id != LWM2M_INVALID_INSTANCE);

  LWM2M_Security_instances[index].LWM2M_Server_SMS_Number = val;
  
  if (notify && LWM2M_Security_instances[index].registered) lwm2m_object_base_resource_changed(&LWM2M_Security_lwm2m_object, instance, 9, false);  
  LWM2M_Security_unlock();
  return COAP_204_CHANGED;
}
static uint8_t __LWM2M_Security_LWM2M_Server_SMS_Number_set(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data)
{
  int64_t val;
  lwm2m_data_decode_int(data, &val);
  return LWM2M_Security_LWM2M_Server_SMS_Number_set(instance,  val, false);
}
__attribute__((weak)) uint8_t LWM2M_Security_LWM2M_Server_SMS_Number_changed(uint16_t instance)
{
  LWM2M_Security_lock();
  assert(instance >= 0 && instance < MAX_LWM2M_Security_INSTANCES);
  uint16_t index = LWM2M_Security_get_index_for_instance(instance);
  assert(index != LWM2M_INVALID_INSTANCE);
  
  LWM2M_Security_changed(instance);
  LWM2M_Security_unlock();
  return COAP_NO_ERROR;
}
#if CONFIG_LWM2M_Security_Short_Server_ID_OPTION == 1
  
__attribute__((weak)) uint8_t LWM2M_Security_Short_Server_ID_get(uint16_t instance,  uint32_t *val)
{
  LWM2M_Security_lock();
  assert(instance >= 0 && instance < MAX_LWM2M_Security_INSTANCES);
  uint16_t index = LWM2M_Security_get_index_for_instance(instance);
  assert(index != LWM2M_INVALID_INSTANCE);
  
  assert(LWM2M_Security_instances[index].id != LWM2M_INVALID_INSTANCE);
  
  
  *val = LWM2M_Security_instances[index].Short_Server_ID;
  LWM2M_Security_unlock();
  return COAP_205_CONTENT;
}
static uint8_t __LWM2M_Security_Short_Server_ID_get(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data)
{
  uint32_t val;
  uint8_t res = LWM2M_Security_Short_Server_ID_get(instance,  &val);
  lwm2m_data_encode_int(val, data);
  return res;
}
                                         
#endif
#if CONFIG_LWM2M_Security_Short_Server_ID_OPTION == 1
__attribute__((weak)) uint8_t LWM2M_Security_Short_Server_ID_set(uint16_t instance,  uint32_t val, bool notify)
{
  LWM2M_Security_lock();
  assert(instance >= 0 && instance < MAX_LWM2M_Security_INSTANCES);
  uint16_t index = LWM2M_Security_get_index_for_instance(instance);
  assert(index != LWM2M_INVALID_INSTANCE);
  
  assert(LWM2M_Security_instances[index].id != LWM2M_INVALID_INSTANCE);

  LWM2M_Security_instances[index].Short_Server_ID = val;
  
  if (notify && LWM2M_Security_instances[index].registered) lwm2m_object_base_resource_changed(&LWM2M_Security_lwm2m_object, instance, 10, false);  
  LWM2M_Security_unlock();
  return COAP_204_CHANGED;
}
static uint8_t __LWM2M_Security_Short_Server_ID_set(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data)
{
  int64_t val;
  lwm2m_data_decode_int(data, &val);
  return LWM2M_Security_Short_Server_ID_set(instance,  val, false);
}
#endif
#if CONFIG_LWM2M_Security_Short_Server_ID_OPTION == 1
__attribute__((weak)) uint8_t LWM2M_Security_Short_Server_ID_changed(uint16_t instance)
{
  LWM2M_Security_lock();
  assert(instance >= 0 && instance < MAX_LWM2M_Security_INSTANCES);
  uint16_t index = LWM2M_Security_get_index_for_instance(instance);
  assert(index != LWM2M_INVALID_INSTANCE);
  
  LWM2M_Security_changed(instance);
  LWM2M_Security_unlock();
  return COAP_NO_ERROR;
}
#endif
  
__attribute__((weak)) uint8_t LWM2M_Security_Client_Hold_Off_Time_get(uint16_t instance,  uint32_t *val)
{
  LWM2M_Security_lock();
  assert(instance >= 0 && instance < MAX_LWM2M_Security_INSTANCES);
  uint16_t index = LWM2M_Security_get_index_for_instance(instance);
  assert(index != LWM2M_INVALID_INSTANCE);
  
  assert(LWM2M_Security_instances[index].id != LWM2M_INVALID_INSTANCE);
  
  
  *val = LWM2M_Security_instances[index].Client_Hold_Off_Time;
  LWM2M_Security_unlock();
  return COAP_205_CONTENT;
}
static uint8_t __LWM2M_Security_Client_Hold_Off_Time_get(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data)
{
  uint32_t val;
  uint8_t res = LWM2M_Security_Client_Hold_Off_Time_get(instance,  &val);
  lwm2m_data_encode_int(val, data);
  return res;
}
                                         
__attribute__((weak)) uint8_t LWM2M_Security_Client_Hold_Off_Time_set(uint16_t instance,  uint32_t val, bool notify)
{
  LWM2M_Security_lock();
  assert(instance >= 0 && instance < MAX_LWM2M_Security_INSTANCES);
  uint16_t index = LWM2M_Security_get_index_for_instance(instance);
  assert(index != LWM2M_INVALID_INSTANCE);
  
  assert(LWM2M_Security_instances[index].id != LWM2M_INVALID_INSTANCE);

  LWM2M_Security_instances[index].Client_Hold_Off_Time = val;
  
  if (notify && LWM2M_Security_instances[index].registered) lwm2m_object_base_resource_changed(&LWM2M_Security_lwm2m_object, instance, 11, false);  
  LWM2M_Security_unlock();
  return COAP_204_CHANGED;
}
static uint8_t __LWM2M_Security_Client_Hold_Off_Time_set(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data)
{
  int64_t val;
  lwm2m_data_decode_int(data, &val);
  return LWM2M_Security_Client_Hold_Off_Time_set(instance,  val, false);
}
__attribute__((weak)) uint8_t LWM2M_Security_Client_Hold_Off_Time_changed(uint16_t instance)
{
  LWM2M_Security_lock();
  assert(instance >= 0 && instance < MAX_LWM2M_Security_INSTANCES);
  uint16_t index = LWM2M_Security_get_index_for_instance(instance);
  assert(index != LWM2M_INVALID_INSTANCE);
  
  LWM2M_Security_changed(instance);
  LWM2M_Security_unlock();
  return COAP_NO_ERROR;
}
