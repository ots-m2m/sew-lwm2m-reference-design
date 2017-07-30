/*
 * Device_base.c
 * 
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * This code was generated using gt-object-gen provided by Operational 
 * Technology Solutions M2M Pty. Ltd. For licensing enquires visit 
 * ot-solutions.com.au.
 */
#include "Device_base.h"

#include <string.h>
#include <stdbool.h>

#include "mutex.h"
#define assert(condition) if (!(condition)) { Device_unlock(); return COAP_406_NOT_ACCEPTABLE; }
#define assert_void(condition) if (!(condition)) { Device_unlock(); return; }

uint8_t __Device_create_instance(uint16_t instance_id);
#if CONFIG_Device_Manufacturer_OPTION == 1
static uint8_t __Device_Manufacturer_get(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data);
  
#endif
#if CONFIG_Device_Model_Number_OPTION == 1
static uint8_t __Device_Model_Number_get(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data);
  
#endif
#if CONFIG_Device_Serial_Number_OPTION == 1
static uint8_t __Device_Serial_Number_get(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data);
  
#endif
#if CONFIG_Device_Firmware_Version_OPTION == 1
static uint8_t __Device_Firmware_Version_get(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data);
  
#endif
#if CONFIG_Device_Available_Power_Sources_OPTION == 1
static uint8_t __Device_Available_Power_Sources_get(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data);
  
#endif
#if CONFIG_Device_Power_Source_Voltage_OPTION == 1
static uint8_t __Device_Power_Source_Voltage_get(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data);
  
#endif
#if CONFIG_Device_Power_Source_Current_OPTION == 1
static uint8_t __Device_Power_Source_Current_get(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data);
  
#endif
#if CONFIG_Device_Battery_Level_OPTION == 1
static uint8_t __Device_Battery_Level_get(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data);
  
#endif
#if CONFIG_Device_Memory_Free_OPTION == 1
static uint8_t __Device_Memory_Free_get(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data);
  
#endif
static uint8_t __Device_Error_Code_get(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data);
  
#if CONFIG_Device_Current_Time_OPTION == 1
static uint8_t __Device_Current_Time_get(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data);
  
#endif
#if CONFIG_Device_Current_Time_OPTION == 1
static uint8_t __Device_Current_Time_set(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data);
#endif
#if CONFIG_Device_UTC_Offset_OPTION == 1
static uint8_t __Device_UTC_Offset_get(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data);
  
#endif
#if CONFIG_Device_UTC_Offset_OPTION == 1
static uint8_t __Device_UTC_Offset_set(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data);
#endif
#if CONFIG_Device_Timezone_OPTION == 1
static uint8_t __Device_Timezone_get(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data);
  
#endif
#if CONFIG_Device_Timezone_OPTION == 1
static uint8_t __Device_Timezone_set(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data);
#endif
static uint8_t __Device_Supported_Binding_and_Modes_get(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data);
  

static lwm2m_object_t Device_lwm2m_object = {
  .objID = 3
};

lwm2m_object_t *Device_get_lwm2m_object(void)
{
  return &Device_lwm2m_object;
}

typedef struct Device_instance_t
{
  struct Device_instance * next;   // matches lwm2m_list_t::next
  uint16_t id;                                // matches lwm2m_list_t::id
  uint16_t index;
  bool registered;
  void *user_data;
#if CONFIG_Device_Manufacturer_OPTION == 1
  char Manufacturer [MAX_Device_Manufacturer];
#endif
#if CONFIG_Device_Model_Number_OPTION == 1
  char Model_Number [MAX_Device_Model_Number];
#endif
#if CONFIG_Device_Serial_Number_OPTION == 1
  char Serial_Number [MAX_Device_Serial_Number];
#endif
#if CONFIG_Device_Firmware_Version_OPTION == 1
  char Firmware_Version [MAX_Device_Firmware_Version];
#endif
#if CONFIG_Device_Available_Power_Sources_OPTION == 1
  bool Available_Power_Sources_is_set[MAX_MULTIPLE_RESOURCES];
  uint32_t Available_Power_Sources[MAX_MULTIPLE_RESOURCES];
#endif
#if CONFIG_Device_Power_Source_Voltage_OPTION == 1
  bool Power_Source_Voltage_is_set[MAX_MULTIPLE_RESOURCES];
  uint32_t Power_Source_Voltage[MAX_MULTIPLE_RESOURCES];
#endif
#if CONFIG_Device_Power_Source_Current_OPTION == 1
  bool Power_Source_Current_is_set[MAX_MULTIPLE_RESOURCES];
  uint32_t Power_Source_Current[MAX_MULTIPLE_RESOURCES];
#endif
#if CONFIG_Device_Battery_Level_OPTION == 1
  uint32_t Battery_Level;
#endif
#if CONFIG_Device_Memory_Free_OPTION == 1
  uint32_t Memory_Free;
#endif
  bool Error_Code_is_set[MAX_MULTIPLE_RESOURCES];
  uint32_t Error_Code[MAX_MULTIPLE_RESOURCES];
#if CONFIG_Device_Current_Time_OPTION == 1
  time_t Current_Time;
#endif
#if CONFIG_Device_UTC_Offset_OPTION == 1
  char UTC_Offset [MAX_Device_UTC_Offset];
#endif
#if CONFIG_Device_Timezone_OPTION == 1
  char Timezone [MAX_Device_Timezone];
#endif
  char Supported_Binding_and_Modes [MAX_Device_Supported_Binding_and_Modes];
}
Device_instance;

static Device_instance Device_instances[MAX_Device_INSTANCES];

lwm2m_object_declaration_t Device_object_declaration = {
  .id = 3,
  .delete = Device_delete_instance,
  .create = __Device_create_instance,
#if CONFIG_Device_Manufacturer_OPTION == 1
  .resources[0] = { 
    .supported = true,  
    .id = 0,
    .operation = LWM2M_OPERATION_R,
    .instances = LWM2M_INSTANCETYPE_single,
    .mandatory = false,
    .type = LWM2M_TYPE_string,
    .changed = (void *) 0,
    .execute = (void *) 0,
    .get = __Device_Manufacturer_get,
    .set = (void *) 0,
  },
#else
  .resources[0] = { .supported = false },
#endif
#if CONFIG_Device_Model_Number_OPTION == 1
  .resources[1] = { 
    .supported = true,  
    .id = 1,
    .operation = LWM2M_OPERATION_R,
    .instances = LWM2M_INSTANCETYPE_single,
    .mandatory = false,
    .type = LWM2M_TYPE_string,
    .changed = (void *) 0,
    .execute = (void *) 0,
    .get = __Device_Model_Number_get,
    .set = (void *) 0,
  },
#else
  .resources[1] = { .supported = false },
#endif
#if CONFIG_Device_Serial_Number_OPTION == 1
  .resources[2] = { 
    .supported = true,  
    .id = 2,
    .operation = LWM2M_OPERATION_R,
    .instances = LWM2M_INSTANCETYPE_single,
    .mandatory = false,
    .type = LWM2M_TYPE_string,
    .changed = (void *) 0,
    .execute = (void *) 0,
    .get = __Device_Serial_Number_get,
    .set = (void *) 0,
  },
#else
  .resources[2] = { .supported = false },
#endif
#if CONFIG_Device_Firmware_Version_OPTION == 1
  .resources[3] = { 
    .supported = true,  
    .id = 3,
    .operation = LWM2M_OPERATION_R,
    .instances = LWM2M_INSTANCETYPE_single,
    .mandatory = false,
    .type = LWM2M_TYPE_string,
    .changed = (void *) 0,
    .execute = (void *) 0,
    .get = __Device_Firmware_Version_get,
    .set = (void *) 0,
  },
#else
  .resources[3] = { .supported = false },
#endif
  .resources[4] = { 
    .supported = true,  
    .id = 4,
    .operation = LWM2M_OPERATION_E,
    .instances = LWM2M_INSTANCETYPE_single,
    .mandatory = true,
    .type = LWM2M_TYPE_string,
    .changed = (void *) 0,
    .execute = Device_Reboot_execute,
    .get = (void *) 0,
    .set = (void *) 0,
  },
#if CONFIG_Device_Factory_Reset_OPTION == 1
  .resources[5] = { 
    .supported = true,  
    .id = 5,
    .operation = LWM2M_OPERATION_E,
    .instances = LWM2M_INSTANCETYPE_single,
    .mandatory = false,
    .type = LWM2M_TYPE_string,
    .changed = (void *) 0,
    .execute = Device_Factory_Reset_execute,
    .get = (void *) 0,
    .set = (void *) 0,
  },
#else
  .resources[5] = { .supported = false },
#endif
#if CONFIG_Device_Available_Power_Sources_OPTION == 1
  .resources[6] = { 
    .supported = true,  
    .id = 6,
    .operation = LWM2M_OPERATION_R,
    .instances = LWM2M_INSTANCETYPE_multiple,
    .mandatory = false,
    .type = LWM2M_TYPE_integer,
    .changed = (void *) 0,
    .execute = (void *) 0,
    .is_set = Device_Available_Power_Sources_is_set,
    .get = __Device_Available_Power_Sources_get,
    .set = (void *) 0,
  },
#else
  .resources[6] = { .supported = false },
#endif
#if CONFIG_Device_Power_Source_Voltage_OPTION == 1
  .resources[7] = { 
    .supported = true,  
    .id = 7,
    .operation = LWM2M_OPERATION_R,
    .instances = LWM2M_INSTANCETYPE_multiple,
    .mandatory = false,
    .type = LWM2M_TYPE_integer,
    .changed = (void *) 0,
    .execute = (void *) 0,
    .is_set = Device_Power_Source_Voltage_is_set,
    .get = __Device_Power_Source_Voltage_get,
    .set = (void *) 0,
  },
#else
  .resources[7] = { .supported = false },
#endif
#if CONFIG_Device_Power_Source_Current_OPTION == 1
  .resources[8] = { 
    .supported = true,  
    .id = 8,
    .operation = LWM2M_OPERATION_R,
    .instances = LWM2M_INSTANCETYPE_multiple,
    .mandatory = false,
    .type = LWM2M_TYPE_integer,
    .changed = (void *) 0,
    .execute = (void *) 0,
    .is_set = Device_Power_Source_Current_is_set,
    .get = __Device_Power_Source_Current_get,
    .set = (void *) 0,
  },
#else
  .resources[8] = { .supported = false },
#endif
#if CONFIG_Device_Battery_Level_OPTION == 1
  .resources[9] = { 
    .supported = true,  
    .id = 9,
    .operation = LWM2M_OPERATION_R,
    .instances = LWM2M_INSTANCETYPE_single,
    .mandatory = false,
    .type = LWM2M_TYPE_integer,
    .changed = (void *) 0,
    .execute = (void *) 0,
    .get = __Device_Battery_Level_get,
    .set = (void *) 0,
  },
#else
  .resources[9] = { .supported = false },
#endif
#if CONFIG_Device_Memory_Free_OPTION == 1
  .resources[10] = { 
    .supported = true,  
    .id = 10,
    .operation = LWM2M_OPERATION_R,
    .instances = LWM2M_INSTANCETYPE_single,
    .mandatory = false,
    .type = LWM2M_TYPE_integer,
    .changed = (void *) 0,
    .execute = (void *) 0,
    .get = __Device_Memory_Free_get,
    .set = (void *) 0,
  },
#else
  .resources[10] = { .supported = false },
#endif
  .resources[11] = { 
    .supported = true,  
    .id = 11,
    .operation = LWM2M_OPERATION_R,
    .instances = LWM2M_INSTANCETYPE_multiple,
    .mandatory = true,
    .type = LWM2M_TYPE_integer,
    .changed = (void *) 0,
    .execute = (void *) 0,
    .is_set = Device_Error_Code_is_set,
    .get = __Device_Error_Code_get,
    .set = (void *) 0,
  },
#if CONFIG_Device_Reset_Error_Code_OPTION == 1
  .resources[12] = { 
    .supported = true,  
    .id = 12,
    .operation = LWM2M_OPERATION_E,
    .instances = LWM2M_INSTANCETYPE_single,
    .mandatory = false,
    .type = LWM2M_TYPE_string,
    .changed = (void *) 0,
    .execute = Device_Reset_Error_Code_execute,
    .get = (void *) 0,
    .set = (void *) 0,
  },
#else
  .resources[12] = { .supported = false },
#endif
#if CONFIG_Device_Current_Time_OPTION == 1
  .resources[13] = { 
    .supported = true,  
    .id = 13,
    .operation = LWM2M_OPERATION_RW,
    .instances = LWM2M_INSTANCETYPE_single,
    .mandatory = false,
    .type = LWM2M_TYPE_time,
    .changed = Device_Current_Time_changed,
    .execute = (void *) 0,
    .get = __Device_Current_Time_get,
    .set = __Device_Current_Time_set,
  },
#else
  .resources[13] = { .supported = false },
#endif
#if CONFIG_Device_UTC_Offset_OPTION == 1
  .resources[14] = { 
    .supported = true,  
    .id = 14,
    .operation = LWM2M_OPERATION_RW,
    .instances = LWM2M_INSTANCETYPE_single,
    .mandatory = false,
    .type = LWM2M_TYPE_string,
    .changed = Device_UTC_Offset_changed,
    .execute = (void *) 0,
    .get = __Device_UTC_Offset_get,
    .set = __Device_UTC_Offset_set,
  },
#else
  .resources[14] = { .supported = false },
#endif
#if CONFIG_Device_Timezone_OPTION == 1
  .resources[15] = { 
    .supported = true,  
    .id = 15,
    .operation = LWM2M_OPERATION_RW,
    .instances = LWM2M_INSTANCETYPE_single,
    .mandatory = false,
    .type = LWM2M_TYPE_string,
    .changed = Device_Timezone_changed,
    .execute = (void *) 0,
    .get = __Device_Timezone_get,
    .set = __Device_Timezone_set,
  },
#else
  .resources[15] = { .supported = false },
#endif
  .resources[16] = { 
    .supported = true,  
    .id = 16,
    .operation = LWM2M_OPERATION_R,
    .instances = LWM2M_INSTANCETYPE_single,
    .mandatory = true,
    .type = LWM2M_TYPE_string,
    .changed = (void *) 0,
    .execute = (void *) 0,
    .get = __Device_Supported_Binding_and_Modes_get,
    .set = (void *) 0,
  },
};


void Device_lock() 
{ 
  mutex_lock(&Device_object_declaration.lock, MUTEXT_WAIT_FOREVER); 
}
void Device_unlock() 
{ 
  mutex_unlock(&Device_object_declaration.lock);
}


uint16_t Device_get_index_for_instance(uint16_t instance)
{
  for (int i = 0; i < MAX_Device_INSTANCES; i++)
  {
    if (Device_instances[i].id == instance) return i;
  }
  return LWM2M_INVALID_INSTANCE;
}

__attribute__((weak)) lwm2m_object_t *Device_initialise(void)
{
  mutex_initialise(&Device_object_declaration.lock);
  
  memset(Device_instances, 0, sizeof(Device_instances));
  for (int i = 0; i < MAX_Device_INSTANCES; i++)
  {
    Device_instances[i].id = LWM2M_INVALID_INSTANCE;
    Device_instances[i].index = i;
  }
  lwm2m_object_base_initialse_object(&Device_lwm2m_object);
  Device_lwm2m_object.userData = &Device_object_declaration; 
  //singleton so register the only instance
  //Device_create_instance(0);
  return &Device_lwm2m_object;
}
  
__attribute__((weak)) void Device_register_object(lwm2m_context_t *context)
{
  Device_lock();
  Device_object_declaration.context = context;
  Device_unlock();
}
  
__attribute__((weak)) void Device_register_instance(uint16_t instance_id)
{
  Device_lock();
  uint16_t index = Device_get_index_for_instance(instance_id);
  assert_void(index != LWM2M_INVALID_INSTANCE);
  
  if ((Device_instances[index].id !=  LWM2M_INVALID_INSTANCE) && (!Device_instances[index].registered))
  {
    Device_lwm2m_object.instanceList = LWM2M_LIST_ADD(Device_lwm2m_object.instanceList, (lwm2m_list_t *)&Device_instances[index]);
    Device_instances[index].registered = true;
  }
  Device_unlock();
}

__attribute__((weak)) void Device_unregister_instance(uint16_t instance_id)
{
  Device_lock();
  uint16_t index = Device_get_index_for_instance(instance_id);
  assert_void(index != LWM2M_INVALID_INSTANCE);
  
  if ((Device_instances[index].id !=  LWM2M_INVALID_INSTANCE) && (Device_instances[index].registered))
  {
    Device_instance *instance = 0;
    Device_lwm2m_object.instanceList = lwm2m_list_remove(Device_lwm2m_object.instanceList, instance_id, (lwm2m_list_t **)&instance);
    Device_instances[index].registered = false;
    if (instance != 0)
    { 
      instance->next = 0;
    }
  }
  Device_unlock();
}

//override this to perform actions when a new instance is created
__attribute__((weak)) void Device_instance_created(uint16_t instance_id)
{
  
}
  

//called when the server requests an object to be created; we need to create AND register the 
//instance
uint8_t __Device_create_instance(uint16_t instance_id)
{
  uint8_t res = Device_create_instance(instance_id);
  if (res == COAP_201_CREATED)
  {
    Device_register_instance(instance_id);
  }
  return res;
}
  
__attribute__((weak)) uint8_t Device_create_instance(uint16_t instance_id)
{
  Device_lock();
  //find an unallocated instance
  for (int i = 0; i < MAX_Device_INSTANCES; i++)
  {
    if (Device_instances[i].id == LWM2M_INVALID_INSTANCE)
    {
      Device_instances[i].id = instance_id;
      Device_instance_created(instance_id);
      Device_unlock();
      return COAP_201_CREATED;
    }
  }
  Device_unlock();
  return COAP_500_INTERNAL_SERVER_ERROR; //none found
}

void Device_foreach(lwm2m_each_instance_func func, void *data)
{
  Device_lock();
  
  for (int i = 0; i < MAX_Device_INSTANCES; i++)
  {
    if (Device_instances[i].id != LWM2M_INVALID_INSTANCE)
    {
      if (!func(Device_instances[i].id, data)) break;
    }
  }
  Device_unlock();
}
  
//provides access to a user data pointer for storing additional instance
//data.
void *Device_get_user_data(uint16_t instance_id)
{
  uint16_t index = Device_get_index_for_instance(instance_id);
  if (index == LWM2M_INVALID_INSTANCE) return 0;
  return Device_instances[index].user_data;
}
  
bool Device_set_user_data(uint16_t instance_id, void *data)
{
  uint16_t index = Device_get_index_for_instance(instance_id);
  if (index == LWM2M_INVALID_INSTANCE) return false;
  Device_instances[index].user_data = data;
  return true;
}
  
//override this to perform actions when a new instance is created
__attribute__((weak)) void Device_instance_deleted(uint16_t instance_id)
{
  
}

__attribute__((weak)) uint8_t Device_delete_instance(uint16_t instance_id)
{
  Device_lock();
  uint16_t index = Device_get_index_for_instance(instance_id);
  assert(index != LWM2M_INVALID_INSTANCE);
  
  if ((instance_id < MAX_Device_INSTANCES) && (Device_instances[index].id !=  LWM2M_INVALID_INSTANCE))
  {
    Device_unregister_instance(instance_id);
    Device_instance_deleted(instance_id);
    Device_instances[index].id = LWM2M_INVALID_INSTANCE;
  }
  Device_unlock();
  return COAP_202_DELETED;
}


__attribute__((weak)) void Device_changed(uint16_t instance)
{
  
}

#if CONFIG_Device_Manufacturer_OPTION == 1
  
__attribute__((weak))  uint8_t Device_Manufacturer_get(uint16_t instance,  const char **val)
{
  Device_lock();
  assert(instance >= 0 && instance < MAX_Device_INSTANCES);
  uint16_t index = Device_get_index_for_instance(instance);
  assert(index != LWM2M_INVALID_INSTANCE);
  
  assert(Device_instances[index].id != LWM2M_INVALID_INSTANCE);
  
  
  *val = Device_instances[index].Manufacturer;
  Device_unlock();
  return COAP_205_CONTENT;
}
static uint8_t __Device_Manufacturer_get(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data)
{
  const char *val;
  uint8_t res = Device_Manufacturer_get(instance,  &val);
  lwm2m_data_encode_string(val, data);
  return res;
}
                                         
#endif
#if CONFIG_Device_Manufacturer_OPTION == 1
__attribute__((weak)) uint8_t Device_Manufacturer_set(uint16_t instance,  const char *val, size_t length, bool notify)
{
  Device_lock();
  assert(instance >= 0 && instance < MAX_Device_INSTANCES);
  uint16_t index = Device_get_index_for_instance(instance);
  assert(index != LWM2M_INVALID_INSTANCE);
  
  assert(Device_instances[index].id != LWM2M_INVALID_INSTANCE);
 
  if (length >= sizeof(Device_instances[index].Manufacturer )) length = sizeof(Device_instances[index].Manufacturer) - 1;
  memcpy(Device_instances[index].Manufacturer, val, length);
  Device_instances[index].Manufacturer[length] = 0;
  
  
  if (notify && Device_instances[index].registered) lwm2m_object_base_resource_changed(&Device_lwm2m_object, instance, 0, false);
  Device_unlock();
  return COAP_204_CHANGED;
}
#endif
#if CONFIG_Device_Model_Number_OPTION == 1
  
__attribute__((weak))  uint8_t Device_Model_Number_get(uint16_t instance,  const char **val)
{
  Device_lock();
  assert(instance >= 0 && instance < MAX_Device_INSTANCES);
  uint16_t index = Device_get_index_for_instance(instance);
  assert(index != LWM2M_INVALID_INSTANCE);
  
  assert(Device_instances[index].id != LWM2M_INVALID_INSTANCE);
  
  
  *val = Device_instances[index].Model_Number;
  Device_unlock();
  return COAP_205_CONTENT;
}
static uint8_t __Device_Model_Number_get(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data)
{
  const char *val;
  uint8_t res = Device_Model_Number_get(instance,  &val);
  lwm2m_data_encode_string(val, data);
  return res;
}
                                         
#endif
#if CONFIG_Device_Model_Number_OPTION == 1
__attribute__((weak)) uint8_t Device_Model_Number_set(uint16_t instance,  const char *val, size_t length, bool notify)
{
  Device_lock();
  assert(instance >= 0 && instance < MAX_Device_INSTANCES);
  uint16_t index = Device_get_index_for_instance(instance);
  assert(index != LWM2M_INVALID_INSTANCE);
  
  assert(Device_instances[index].id != LWM2M_INVALID_INSTANCE);
 
  if (length >= sizeof(Device_instances[index].Model_Number )) length = sizeof(Device_instances[index].Model_Number) - 1;
  memcpy(Device_instances[index].Model_Number, val, length);
  Device_instances[index].Model_Number[length] = 0;
  
  
  if (notify && Device_instances[index].registered) lwm2m_object_base_resource_changed(&Device_lwm2m_object, instance, 1, false);
  Device_unlock();
  return COAP_204_CHANGED;
}
#endif
#if CONFIG_Device_Serial_Number_OPTION == 1
  
__attribute__((weak))  uint8_t Device_Serial_Number_get(uint16_t instance,  const char **val)
{
  Device_lock();
  assert(instance >= 0 && instance < MAX_Device_INSTANCES);
  uint16_t index = Device_get_index_for_instance(instance);
  assert(index != LWM2M_INVALID_INSTANCE);
  
  assert(Device_instances[index].id != LWM2M_INVALID_INSTANCE);
  
  
  *val = Device_instances[index].Serial_Number;
  Device_unlock();
  return COAP_205_CONTENT;
}
static uint8_t __Device_Serial_Number_get(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data)
{
  const char *val;
  uint8_t res = Device_Serial_Number_get(instance,  &val);
  lwm2m_data_encode_string(val, data);
  return res;
}
                                         
#endif
#if CONFIG_Device_Serial_Number_OPTION == 1
__attribute__((weak)) uint8_t Device_Serial_Number_set(uint16_t instance,  const char *val, size_t length, bool notify)
{
  Device_lock();
  assert(instance >= 0 && instance < MAX_Device_INSTANCES);
  uint16_t index = Device_get_index_for_instance(instance);
  assert(index != LWM2M_INVALID_INSTANCE);
  
  assert(Device_instances[index].id != LWM2M_INVALID_INSTANCE);
 
  if (length >= sizeof(Device_instances[index].Serial_Number )) length = sizeof(Device_instances[index].Serial_Number) - 1;
  memcpy(Device_instances[index].Serial_Number, val, length);
  Device_instances[index].Serial_Number[length] = 0;
  
  
  if (notify && Device_instances[index].registered) lwm2m_object_base_resource_changed(&Device_lwm2m_object, instance, 2, false);
  Device_unlock();
  return COAP_204_CHANGED;
}
#endif
#if CONFIG_Device_Firmware_Version_OPTION == 1
  
__attribute__((weak))  uint8_t Device_Firmware_Version_get(uint16_t instance,  const char **val)
{
  Device_lock();
  assert(instance >= 0 && instance < MAX_Device_INSTANCES);
  uint16_t index = Device_get_index_for_instance(instance);
  assert(index != LWM2M_INVALID_INSTANCE);
  
  assert(Device_instances[index].id != LWM2M_INVALID_INSTANCE);
  
  
  *val = Device_instances[index].Firmware_Version;
  Device_unlock();
  return COAP_205_CONTENT;
}
static uint8_t __Device_Firmware_Version_get(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data)
{
  const char *val;
  uint8_t res = Device_Firmware_Version_get(instance,  &val);
  lwm2m_data_encode_string(val, data);
  return res;
}
                                         
#endif
#if CONFIG_Device_Firmware_Version_OPTION == 1
__attribute__((weak)) uint8_t Device_Firmware_Version_set(uint16_t instance,  const char *val, size_t length, bool notify)
{
  Device_lock();
  assert(instance >= 0 && instance < MAX_Device_INSTANCES);
  uint16_t index = Device_get_index_for_instance(instance);
  assert(index != LWM2M_INVALID_INSTANCE);
  
  assert(Device_instances[index].id != LWM2M_INVALID_INSTANCE);
 
  if (length >= sizeof(Device_instances[index].Firmware_Version )) length = sizeof(Device_instances[index].Firmware_Version) - 1;
  memcpy(Device_instances[index].Firmware_Version, val, length);
  Device_instances[index].Firmware_Version[length] = 0;
  
  
  if (notify && Device_instances[index].registered) lwm2m_object_base_resource_changed(&Device_lwm2m_object, instance, 3, false);
  Device_unlock();
  return COAP_204_CHANGED;
}
#endif
__attribute__((weak)) uint8_t Device_Reboot_execute(lwm2m_object_t *object, uint16_t instance, uint16_t resource_id, uint8_t * buffer, int length)
{
  return COAP_405_METHOD_NOT_ALLOWED;
}
#if CONFIG_Device_Factory_Reset_OPTION == 1
__attribute__((weak)) uint8_t Device_Factory_Reset_execute(lwm2m_object_t *object, uint16_t instance, uint16_t resource_id, uint8_t * buffer, int length)
{
  return COAP_405_METHOD_NOT_ALLOWED;
}
#endif
#if CONFIG_Device_Available_Power_Sources_OPTION == 1
bool Device_Available_Power_Sources_is_set(uint16_t instance_id, uint16_t resource_instance)
{
  uint16_t index = Device_get_index_for_instance(instance_id);
  if ((index == LWM2M_INVALID_INSTANCE) || (resource_instance >= MAX_MULTIPLE_RESOURCES)) return false;
  return Device_instances[index].Available_Power_Sources_is_set[resource_instance];
}
  
__attribute__((weak)) uint8_t Device_Available_Power_Sources_get(uint16_t instance, uint16_t resource_instance, uint32_t *val)
{
  Device_lock();
  assert(instance >= 0 && instance < MAX_Device_INSTANCES);
  uint16_t index = Device_get_index_for_instance(instance);
  assert(index != LWM2M_INVALID_INSTANCE);
  assert(resource_instance < MAX_MULTIPLE_RESOURCES);
  assert(Device_instances[index].id != LWM2M_INVALID_INSTANCE);
  assert(Device_instances[index].Available_Power_Sources_is_set[resource_instance]);
  
  *val = Device_instances[index].Available_Power_Sources[resource_instance];
  Device_unlock();
  return COAP_205_CONTENT;
}
static uint8_t __Device_Available_Power_Sources_get(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data)
{
  uint32_t val;
  uint8_t res = Device_Available_Power_Sources_get(instance, resource_instance, &val);
  lwm2m_data_encode_int(val, data);
  return res;
}
                                         
#endif
#if CONFIG_Device_Available_Power_Sources_OPTION == 1
__attribute__((weak)) uint8_t Device_Available_Power_Sources_set(uint16_t instance, uint16_t resource_instance, uint32_t val, bool notify)
{
  Device_lock();
  assert(instance >= 0 && instance < MAX_Device_INSTANCES);
  uint16_t index = Device_get_index_for_instance(instance);
  assert(index != LWM2M_INVALID_INSTANCE);
  assert(resource_instance < MAX_MULTIPLE_RESOURCES);
  assert(Device_instances[index].id != LWM2M_INVALID_INSTANCE);

  Device_instances[index].Available_Power_Sources[resource_instance] = val;
  Device_instances[index].Available_Power_Sources_is_set[resource_instance] = true;
  if (notify && Device_instances[index].registered) lwm2m_object_base_resource_changed(&Device_lwm2m_object, instance, 6, false);  
  Device_unlock();
  return COAP_204_CHANGED;
}
#endif
#if CONFIG_Device_Power_Source_Voltage_OPTION == 1
bool Device_Power_Source_Voltage_is_set(uint16_t instance_id, uint16_t resource_instance)
{
  uint16_t index = Device_get_index_for_instance(instance_id);
  if ((index == LWM2M_INVALID_INSTANCE) || (resource_instance >= MAX_MULTIPLE_RESOURCES)) return false;
  return Device_instances[index].Power_Source_Voltage_is_set[resource_instance];
}
  
__attribute__((weak)) uint8_t Device_Power_Source_Voltage_get(uint16_t instance, uint16_t resource_instance, uint32_t *val)
{
  Device_lock();
  assert(instance >= 0 && instance < MAX_Device_INSTANCES);
  uint16_t index = Device_get_index_for_instance(instance);
  assert(index != LWM2M_INVALID_INSTANCE);
  assert(resource_instance < MAX_MULTIPLE_RESOURCES);
  assert(Device_instances[index].id != LWM2M_INVALID_INSTANCE);
  assert(Device_instances[index].Power_Source_Voltage_is_set[resource_instance]);
  
  *val = Device_instances[index].Power_Source_Voltage[resource_instance];
  Device_unlock();
  return COAP_205_CONTENT;
}
static uint8_t __Device_Power_Source_Voltage_get(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data)
{
  uint32_t val;
  uint8_t res = Device_Power_Source_Voltage_get(instance, resource_instance, &val);
  lwm2m_data_encode_int(val, data);
  return res;
}
                                         
#endif
#if CONFIG_Device_Power_Source_Voltage_OPTION == 1
__attribute__((weak)) uint8_t Device_Power_Source_Voltage_set(uint16_t instance, uint16_t resource_instance, uint32_t val, bool notify)
{
  Device_lock();
  assert(instance >= 0 && instance < MAX_Device_INSTANCES);
  uint16_t index = Device_get_index_for_instance(instance);
  assert(index != LWM2M_INVALID_INSTANCE);
  assert(resource_instance < MAX_MULTIPLE_RESOURCES);
  assert(Device_instances[index].id != LWM2M_INVALID_INSTANCE);

  Device_instances[index].Power_Source_Voltage[resource_instance] = val;
  Device_instances[index].Power_Source_Voltage_is_set[resource_instance] = true;
  if (notify && Device_instances[index].registered) lwm2m_object_base_resource_changed(&Device_lwm2m_object, instance, 7, false);  
  Device_unlock();
  return COAP_204_CHANGED;
}
#endif
#if CONFIG_Device_Power_Source_Current_OPTION == 1
bool Device_Power_Source_Current_is_set(uint16_t instance_id, uint16_t resource_instance)
{
  uint16_t index = Device_get_index_for_instance(instance_id);
  if ((index == LWM2M_INVALID_INSTANCE) || (resource_instance >= MAX_MULTIPLE_RESOURCES)) return false;
  return Device_instances[index].Power_Source_Current_is_set[resource_instance];
}
  
__attribute__((weak)) uint8_t Device_Power_Source_Current_get(uint16_t instance, uint16_t resource_instance, uint32_t *val)
{
  Device_lock();
  assert(instance >= 0 && instance < MAX_Device_INSTANCES);
  uint16_t index = Device_get_index_for_instance(instance);
  assert(index != LWM2M_INVALID_INSTANCE);
  assert(resource_instance < MAX_MULTIPLE_RESOURCES);
  assert(Device_instances[index].id != LWM2M_INVALID_INSTANCE);
  assert(Device_instances[index].Power_Source_Current_is_set[resource_instance]);
  
  *val = Device_instances[index].Power_Source_Current[resource_instance];
  Device_unlock();
  return COAP_205_CONTENT;
}
static uint8_t __Device_Power_Source_Current_get(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data)
{
  uint32_t val;
  uint8_t res = Device_Power_Source_Current_get(instance, resource_instance, &val);
  lwm2m_data_encode_int(val, data);
  return res;
}
                                         
#endif
#if CONFIG_Device_Power_Source_Current_OPTION == 1
__attribute__((weak)) uint8_t Device_Power_Source_Current_set(uint16_t instance, uint16_t resource_instance, uint32_t val, bool notify)
{
  Device_lock();
  assert(instance >= 0 && instance < MAX_Device_INSTANCES);
  uint16_t index = Device_get_index_for_instance(instance);
  assert(index != LWM2M_INVALID_INSTANCE);
  assert(resource_instance < MAX_MULTIPLE_RESOURCES);
  assert(Device_instances[index].id != LWM2M_INVALID_INSTANCE);

  Device_instances[index].Power_Source_Current[resource_instance] = val;
  Device_instances[index].Power_Source_Current_is_set[resource_instance] = true;
  if (notify && Device_instances[index].registered) lwm2m_object_base_resource_changed(&Device_lwm2m_object, instance, 8, false);  
  Device_unlock();
  return COAP_204_CHANGED;
}
#endif
#if CONFIG_Device_Battery_Level_OPTION == 1
  
__attribute__((weak)) uint8_t Device_Battery_Level_get(uint16_t instance,  uint32_t *val)
{
  Device_lock();
  assert(instance >= 0 && instance < MAX_Device_INSTANCES);
  uint16_t index = Device_get_index_for_instance(instance);
  assert(index != LWM2M_INVALID_INSTANCE);
  
  assert(Device_instances[index].id != LWM2M_INVALID_INSTANCE);
  
  
  *val = Device_instances[index].Battery_Level;
  Device_unlock();
  return COAP_205_CONTENT;
}
static uint8_t __Device_Battery_Level_get(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data)
{
  uint32_t val;
  uint8_t res = Device_Battery_Level_get(instance,  &val);
  lwm2m_data_encode_int(val, data);
  return res;
}
                                         
#endif
#if CONFIG_Device_Battery_Level_OPTION == 1
__attribute__((weak)) uint8_t Device_Battery_Level_set(uint16_t instance,  uint32_t val, bool notify)
{
  Device_lock();
  assert(instance >= 0 && instance < MAX_Device_INSTANCES);
  uint16_t index = Device_get_index_for_instance(instance);
  assert(index != LWM2M_INVALID_INSTANCE);
  
  assert(Device_instances[index].id != LWM2M_INVALID_INSTANCE);

  Device_instances[index].Battery_Level = val;
  
  if (notify && Device_instances[index].registered) lwm2m_object_base_resource_changed(&Device_lwm2m_object, instance, 9, false);  
  Device_unlock();
  return COAP_204_CHANGED;
}
#endif
#if CONFIG_Device_Memory_Free_OPTION == 1
  
__attribute__((weak)) uint8_t Device_Memory_Free_get(uint16_t instance,  uint32_t *val)
{
  Device_lock();
  assert(instance >= 0 && instance < MAX_Device_INSTANCES);
  uint16_t index = Device_get_index_for_instance(instance);
  assert(index != LWM2M_INVALID_INSTANCE);
  
  assert(Device_instances[index].id != LWM2M_INVALID_INSTANCE);
  
  
  *val = Device_instances[index].Memory_Free;
  Device_unlock();
  return COAP_205_CONTENT;
}
static uint8_t __Device_Memory_Free_get(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data)
{
  uint32_t val;
  uint8_t res = Device_Memory_Free_get(instance,  &val);
  lwm2m_data_encode_int(val, data);
  return res;
}
                                         
#endif
#if CONFIG_Device_Memory_Free_OPTION == 1
__attribute__((weak)) uint8_t Device_Memory_Free_set(uint16_t instance,  uint32_t val, bool notify)
{
  Device_lock();
  assert(instance >= 0 && instance < MAX_Device_INSTANCES);
  uint16_t index = Device_get_index_for_instance(instance);
  assert(index != LWM2M_INVALID_INSTANCE);
  
  assert(Device_instances[index].id != LWM2M_INVALID_INSTANCE);

  Device_instances[index].Memory_Free = val;
  
  if (notify && Device_instances[index].registered) lwm2m_object_base_resource_changed(&Device_lwm2m_object, instance, 10, false);  
  Device_unlock();
  return COAP_204_CHANGED;
}
#endif
bool Device_Error_Code_is_set(uint16_t instance_id, uint16_t resource_instance)
{
  uint16_t index = Device_get_index_for_instance(instance_id);
  if ((index == LWM2M_INVALID_INSTANCE) || (resource_instance >= MAX_MULTIPLE_RESOURCES)) return false;
  return Device_instances[index].Error_Code_is_set[resource_instance];
}
  
__attribute__((weak)) uint8_t Device_Error_Code_get(uint16_t instance, uint16_t resource_instance, uint32_t *val)
{
  Device_lock();
  assert(instance >= 0 && instance < MAX_Device_INSTANCES);
  uint16_t index = Device_get_index_for_instance(instance);
  assert(index != LWM2M_INVALID_INSTANCE);
  assert(resource_instance < MAX_MULTIPLE_RESOURCES);
  assert(Device_instances[index].id != LWM2M_INVALID_INSTANCE);
  assert(Device_instances[index].Error_Code_is_set[resource_instance]);
  
  *val = Device_instances[index].Error_Code[resource_instance];
  Device_unlock();
  return COAP_205_CONTENT;
}
static uint8_t __Device_Error_Code_get(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data)
{
  uint32_t val;
  uint8_t res = Device_Error_Code_get(instance, resource_instance, &val);
  lwm2m_data_encode_int(val, data);
  return res;
}
                                         
__attribute__((weak)) uint8_t Device_Error_Code_set(uint16_t instance, uint16_t resource_instance, uint32_t val, bool notify)
{
  Device_lock();
  assert(instance >= 0 && instance < MAX_Device_INSTANCES);
  uint16_t index = Device_get_index_for_instance(instance);
  assert(index != LWM2M_INVALID_INSTANCE);
  assert(resource_instance < MAX_MULTIPLE_RESOURCES);
  assert(Device_instances[index].id != LWM2M_INVALID_INSTANCE);

  Device_instances[index].Error_Code[resource_instance] = val;
  Device_instances[index].Error_Code_is_set[resource_instance] = true;
  if (notify && Device_instances[index].registered) lwm2m_object_base_resource_changed(&Device_lwm2m_object, instance, 11, false);  
  Device_unlock();
  return COAP_204_CHANGED;
}
#if CONFIG_Device_Reset_Error_Code_OPTION == 1
__attribute__((weak)) uint8_t Device_Reset_Error_Code_execute(lwm2m_object_t *object, uint16_t instance, uint16_t resource_id, uint8_t * buffer, int length)
{
  return COAP_405_METHOD_NOT_ALLOWED;
}
#endif
#if CONFIG_Device_Current_Time_OPTION == 1
  
__attribute__((weak))  uint8_t Device_Current_Time_get(uint16_t instance,  time_t *val)
{
  Device_lock();
  uint16_t index = Device_get_index_for_instance(instance);
  assert(index != LWM2M_INVALID_INSTANCE);
  
  assert(Device_instances[index].id != LWM2M_INVALID_INSTANCE);
  
                                                                                           
  *val = Device_instances[index].Current_Time;
  Device_unlock();
  return COAP_205_CONTENT;
}
static uint8_t __Device_Current_Time_get(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data)
{
 time_t val;
 uint8_t res = Device_Current_Time_get(instance,  &val);
 lwm2m_data_encode_int(val, data);
 return res;
}
                                         
#endif
#if CONFIG_Device_Current_Time_OPTION == 1
__attribute__((weak)) uint8_t Device_Current_Time_set(uint16_t instance, time_t val, bool notify)
{
  Device_lock();
  assert(instance >= 0 && instance < MAX_Device_INSTANCES);
  uint16_t index = Device_get_index_for_instance(instance);
  assert(index != LWM2M_INVALID_INSTANCE);
  
  assert(Device_instances[index].id != LWM2M_INVALID_INSTANCE);

  Device_instances[index].Current_Time = val;   
  
  
  if (notify && Device_instances[index].registered) lwm2m_object_base_resource_changed(&Device_lwm2m_object, instance, 13,false);
  Device_unlock();
  return COAP_204_CHANGED;
}
static uint8_t __Device_Current_Time_set(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data)
{
  time_t val;
  int64_t tmp;
  lwm2m_data_decode_int(data, &tmp);
  val = (time_t)tmp;
  return Device_Current_Time_set(instance,  val, false);
}
#endif
#if CONFIG_Device_Current_Time_OPTION == 1
__attribute__((weak)) uint8_t Device_Current_Time_changed(uint16_t instance)
{
  Device_lock();
  assert(instance >= 0 && instance < MAX_Device_INSTANCES);
  uint16_t index = Device_get_index_for_instance(instance);
  assert(index != LWM2M_INVALID_INSTANCE);
  
  Device_changed(instance);
  Device_unlock();
  return COAP_NO_ERROR;
}
#endif
#if CONFIG_Device_UTC_Offset_OPTION == 1
  
__attribute__((weak))  uint8_t Device_UTC_Offset_get(uint16_t instance,  const char **val)
{
  Device_lock();
  assert(instance >= 0 && instance < MAX_Device_INSTANCES);
  uint16_t index = Device_get_index_for_instance(instance);
  assert(index != LWM2M_INVALID_INSTANCE);
  
  assert(Device_instances[index].id != LWM2M_INVALID_INSTANCE);
  
  
  *val = Device_instances[index].UTC_Offset;
  Device_unlock();
  return COAP_205_CONTENT;
}
static uint8_t __Device_UTC_Offset_get(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data)
{
  const char *val;
  uint8_t res = Device_UTC_Offset_get(instance,  &val);
  lwm2m_data_encode_string(val, data);
  return res;
}
                                         
#endif
#if CONFIG_Device_UTC_Offset_OPTION == 1
__attribute__((weak)) uint8_t Device_UTC_Offset_set(uint16_t instance,  const char *val, size_t length, bool notify)
{
  Device_lock();
  assert(instance >= 0 && instance < MAX_Device_INSTANCES);
  uint16_t index = Device_get_index_for_instance(instance);
  assert(index != LWM2M_INVALID_INSTANCE);
  
  assert(Device_instances[index].id != LWM2M_INVALID_INSTANCE);
 
  if (length >= sizeof(Device_instances[index].UTC_Offset )) length = sizeof(Device_instances[index].UTC_Offset) - 1;
  memcpy(Device_instances[index].UTC_Offset, val, length);
  Device_instances[index].UTC_Offset[length] = 0;
  
  
  if (notify && Device_instances[index].registered) lwm2m_object_base_resource_changed(&Device_lwm2m_object, instance, 14, false);
  Device_unlock();
  return COAP_204_CHANGED;
}
static uint8_t __Device_UTC_Offset_set(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data)
{
  return Device_UTC_Offset_set(instance,  (char*)data->value.asBuffer.buffer, data->value.asBuffer.length, false);
}
#endif
#if CONFIG_Device_UTC_Offset_OPTION == 1
__attribute__((weak)) uint8_t Device_UTC_Offset_changed(uint16_t instance)
{
  Device_lock();
  assert(instance >= 0 && instance < MAX_Device_INSTANCES);
  uint16_t index = Device_get_index_for_instance(instance);
  assert(index != LWM2M_INVALID_INSTANCE);
  
  Device_changed(instance);
  Device_unlock();
  return COAP_NO_ERROR;
}
#endif
#if CONFIG_Device_Timezone_OPTION == 1
  
__attribute__((weak))  uint8_t Device_Timezone_get(uint16_t instance,  const char **val)
{
  Device_lock();
  assert(instance >= 0 && instance < MAX_Device_INSTANCES);
  uint16_t index = Device_get_index_for_instance(instance);
  assert(index != LWM2M_INVALID_INSTANCE);
  
  assert(Device_instances[index].id != LWM2M_INVALID_INSTANCE);
  
  
  *val = Device_instances[index].Timezone;
  Device_unlock();
  return COAP_205_CONTENT;
}
static uint8_t __Device_Timezone_get(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data)
{
  const char *val;
  uint8_t res = Device_Timezone_get(instance,  &val);
  lwm2m_data_encode_string(val, data);
  return res;
}
                                         
#endif
#if CONFIG_Device_Timezone_OPTION == 1
__attribute__((weak)) uint8_t Device_Timezone_set(uint16_t instance,  const char *val, size_t length, bool notify)
{
  Device_lock();
  assert(instance >= 0 && instance < MAX_Device_INSTANCES);
  uint16_t index = Device_get_index_for_instance(instance);
  assert(index != LWM2M_INVALID_INSTANCE);
  
  assert(Device_instances[index].id != LWM2M_INVALID_INSTANCE);
 
  if (length >= sizeof(Device_instances[index].Timezone )) length = sizeof(Device_instances[index].Timezone) - 1;
  memcpy(Device_instances[index].Timezone, val, length);
  Device_instances[index].Timezone[length] = 0;
  
  
  if (notify && Device_instances[index].registered) lwm2m_object_base_resource_changed(&Device_lwm2m_object, instance, 15, false);
  Device_unlock();
  return COAP_204_CHANGED;
}
static uint8_t __Device_Timezone_set(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data)
{
  return Device_Timezone_set(instance,  (char*)data->value.asBuffer.buffer, data->value.asBuffer.length, false);
}
#endif
#if CONFIG_Device_Timezone_OPTION == 1
__attribute__((weak)) uint8_t Device_Timezone_changed(uint16_t instance)
{
  Device_lock();
  assert(instance >= 0 && instance < MAX_Device_INSTANCES);
  uint16_t index = Device_get_index_for_instance(instance);
  assert(index != LWM2M_INVALID_INSTANCE);
  
  Device_changed(instance);
  Device_unlock();
  return COAP_NO_ERROR;
}
#endif
  
__attribute__((weak))  uint8_t Device_Supported_Binding_and_Modes_get(uint16_t instance,  const char **val)
{
  Device_lock();
  assert(instance >= 0 && instance < MAX_Device_INSTANCES);
  uint16_t index = Device_get_index_for_instance(instance);
  assert(index != LWM2M_INVALID_INSTANCE);
  
  assert(Device_instances[index].id != LWM2M_INVALID_INSTANCE);
  
  
  *val = Device_instances[index].Supported_Binding_and_Modes;
  Device_unlock();
  return COAP_205_CONTENT;
}
static uint8_t __Device_Supported_Binding_and_Modes_get(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data)
{
  const char *val;
  uint8_t res = Device_Supported_Binding_and_Modes_get(instance,  &val);
  lwm2m_data_encode_string(val, data);
  return res;
}
                                         
__attribute__((weak)) uint8_t Device_Supported_Binding_and_Modes_set(uint16_t instance,  const char *val, size_t length, bool notify)
{
  Device_lock();
  assert(instance >= 0 && instance < MAX_Device_INSTANCES);
  uint16_t index = Device_get_index_for_instance(instance);
  assert(index != LWM2M_INVALID_INSTANCE);
  
  assert(Device_instances[index].id != LWM2M_INVALID_INSTANCE);
 
  if (length >= sizeof(Device_instances[index].Supported_Binding_and_Modes )) length = sizeof(Device_instances[index].Supported_Binding_and_Modes) - 1;
  memcpy(Device_instances[index].Supported_Binding_and_Modes, val, length);
  Device_instances[index].Supported_Binding_and_Modes[length] = 0;
  
  
  if (notify && Device_instances[index].registered) lwm2m_object_base_resource_changed(&Device_lwm2m_object, instance, 16, false);
  Device_unlock();
  return COAP_204_CHANGED;
}
