/*
 * Water_Meter_High_Pressure_Alarm_base.c
 * 
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * This code was generated using gt-object-gen provided by Operational 
 * Technology Solutions M2M Pty. Ltd. For licensing enquires visit 
 * ot-solutions.com.au.
 */
#include "Water_Meter_High_Pressure_Alarm_base.h"

#include <string.h>
#include <stdbool.h>

#include "mutex.h"
#define assert(condition) if (!(condition)) { Water_Meter_High_Pressure_Alarm_unlock(); return COAP_406_NOT_ACCEPTABLE; }
#define assert_void(condition) if (!(condition)) { Water_Meter_High_Pressure_Alarm_unlock(); return; }

uint8_t __Water_Meter_High_Pressure_Alarm_create_instance(uint16_t instance_id);
#if CONFIG_Water_Meter_High_Pressure_Alarm_Event_Type_OPTION == 1
static uint8_t __Water_Meter_High_Pressure_Alarm_Event_Type_get(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data);
  
#endif
#if CONFIG_Water_Meter_High_Pressure_Alarm_Event_Type_OPTION == 1
static uint8_t __Water_Meter_High_Pressure_Alarm_Event_Type_set(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data);
#endif
#if CONFIG_Water_Meter_High_Pressure_Alarm_Alarm_Realtime_OPTION == 1
static uint8_t __Water_Meter_High_Pressure_Alarm_Alarm_Realtime_get(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data);
  
#endif
#if CONFIG_Water_Meter_High_Pressure_Alarm_Alarm_Realtime_OPTION == 1
static uint8_t __Water_Meter_High_Pressure_Alarm_Alarm_Realtime_set(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data);
#endif
#if CONFIG_Water_Meter_High_Pressure_Alarm_Alarm_State_OPTION == 1
static uint8_t __Water_Meter_High_Pressure_Alarm_Alarm_State_get(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data);
  
#endif
#if CONFIG_Water_Meter_High_Pressure_Alarm_Alarm_Set_Threshold_OPTION == 1
static uint8_t __Water_Meter_High_Pressure_Alarm_Alarm_Set_Threshold_get(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data);
  
#endif
#if CONFIG_Water_Meter_High_Pressure_Alarm_Alarm_Set_Threshold_OPTION == 1
static uint8_t __Water_Meter_High_Pressure_Alarm_Alarm_Set_Threshold_set(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data);
#endif
#if CONFIG_Water_Meter_High_Pressure_Alarm_Alarm_Set_Operator_OPTION == 1
static uint8_t __Water_Meter_High_Pressure_Alarm_Alarm_Set_Operator_get(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data);
  
#endif
#if CONFIG_Water_Meter_High_Pressure_Alarm_Alarm_Set_Operator_OPTION == 1
static uint8_t __Water_Meter_High_Pressure_Alarm_Alarm_Set_Operator_set(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data);
#endif
#if CONFIG_Water_Meter_High_Pressure_Alarm_Alarm_Clear_Threshold_OPTION == 1
static uint8_t __Water_Meter_High_Pressure_Alarm_Alarm_Clear_Threshold_get(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data);
  
#endif
#if CONFIG_Water_Meter_High_Pressure_Alarm_Alarm_Clear_Threshold_OPTION == 1
static uint8_t __Water_Meter_High_Pressure_Alarm_Alarm_Clear_Threshold_set(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data);
#endif
#if CONFIG_Water_Meter_High_Pressure_Alarm_Alarm_Clear_Operator_OPTION == 1
static uint8_t __Water_Meter_High_Pressure_Alarm_Alarm_Clear_Operator_get(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data);
  
#endif
#if CONFIG_Water_Meter_High_Pressure_Alarm_Alarm_Clear_Operator_OPTION == 1
static uint8_t __Water_Meter_High_Pressure_Alarm_Alarm_Clear_Operator_set(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data);
#endif
#if CONFIG_Water_Meter_High_Pressure_Alarm_Alarm_Maximum_Event_Count_OPTION == 1
static uint8_t __Water_Meter_High_Pressure_Alarm_Alarm_Maximum_Event_Count_get(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data);
  
#endif
#if CONFIG_Water_Meter_High_Pressure_Alarm_Alarm_Maximum_Event_Count_OPTION == 1
static uint8_t __Water_Meter_High_Pressure_Alarm_Alarm_Maximum_Event_Count_set(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data);
#endif
#if CONFIG_Water_Meter_High_Pressure_Alarm_Alarm_Maximum_Event_Period_OPTION == 1
static uint8_t __Water_Meter_High_Pressure_Alarm_Alarm_Maximum_Event_Period_get(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data);
  
#endif
#if CONFIG_Water_Meter_High_Pressure_Alarm_Alarm_Maximum_Event_Period_OPTION == 1
static uint8_t __Water_Meter_High_Pressure_Alarm_Alarm_Maximum_Event_Period_set(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data);
#endif
#if CONFIG_Water_Meter_High_Pressure_Alarm_Latest_Delivered_Event_Time_OPTION == 1
static uint8_t __Water_Meter_High_Pressure_Alarm_Latest_Delivered_Event_Time_get(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data);
  
#endif
#if CONFIG_Water_Meter_High_Pressure_Alarm_Latest_Delivered_Event_Time_OPTION == 1
static uint8_t __Water_Meter_High_Pressure_Alarm_Latest_Delivered_Event_Time_set(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data);
#endif
#if CONFIG_Water_Meter_High_Pressure_Alarm_Latest_Recorded_Event_Time_OPTION == 1
static uint8_t __Water_Meter_High_Pressure_Alarm_Latest_Recorded_Event_Time_get(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data);
  
#endif
#if CONFIG_Water_Meter_High_Pressure_Alarm_Alarm_Auto_Clear_OPTION == 1
static uint8_t __Water_Meter_High_Pressure_Alarm_Alarm_Auto_Clear_get(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data);
  
#endif
#if CONFIG_Water_Meter_High_Pressure_Alarm_Alarm_Auto_Clear_OPTION == 1
static uint8_t __Water_Meter_High_Pressure_Alarm_Alarm_Auto_Clear_set(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data);
#endif
#if CONFIG_Water_Meter_High_Pressure_Alarm_Event_Code_OPTION == 1
static uint8_t __Water_Meter_High_Pressure_Alarm_Event_Code_get(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data);
  
#endif
#if CONFIG_Water_Meter_High_Pressure_Alarm_Latest_Payload_OPTION == 1
static uint8_t __Water_Meter_High_Pressure_Alarm_Latest_Payload_get(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data);
  
#endif

static lwm2m_object_t Water_Meter_High_Pressure_Alarm_lwm2m_object = {
  .objID = 8024
};

lwm2m_object_t *Water_Meter_High_Pressure_Alarm_get_lwm2m_object(void)
{
  return &Water_Meter_High_Pressure_Alarm_lwm2m_object;
}

typedef struct Water_Meter_High_Pressure_Alarm_instance_t
{
  struct Water_Meter_High_Pressure_Alarm_instance * next;   // matches lwm2m_list_t::next
  uint16_t id;                                // matches lwm2m_list_t::id
  uint16_t index;
  bool registered;
  void *user_data;
#if CONFIG_Water_Meter_High_Pressure_Alarm_Event_Type_OPTION == 1
  uint32_t Event_Type;
#endif
#if CONFIG_Water_Meter_High_Pressure_Alarm_Alarm_Realtime_OPTION == 1
  bool Alarm_Realtime;
#endif
#if CONFIG_Water_Meter_High_Pressure_Alarm_Alarm_State_OPTION == 1
  bool Alarm_State;
#endif
#if CONFIG_Water_Meter_High_Pressure_Alarm_Alarm_Set_Threshold_OPTION == 1
  float Alarm_Set_Threshold;
#endif
#if CONFIG_Water_Meter_High_Pressure_Alarm_Alarm_Set_Operator_OPTION == 1
  uint32_t Alarm_Set_Operator;
#endif
#if CONFIG_Water_Meter_High_Pressure_Alarm_Alarm_Clear_Threshold_OPTION == 1
  float Alarm_Clear_Threshold;
#endif
#if CONFIG_Water_Meter_High_Pressure_Alarm_Alarm_Clear_Operator_OPTION == 1
  uint32_t Alarm_Clear_Operator;
#endif
#if CONFIG_Water_Meter_High_Pressure_Alarm_Alarm_Maximum_Event_Count_OPTION == 1
  uint32_t Alarm_Maximum_Event_Count;
#endif
#if CONFIG_Water_Meter_High_Pressure_Alarm_Alarm_Maximum_Event_Period_OPTION == 1
  uint32_t Alarm_Maximum_Event_Period;
#endif
#if CONFIG_Water_Meter_High_Pressure_Alarm_Latest_Delivered_Event_Time_OPTION == 1
  time_t Latest_Delivered_Event_Time;
#endif
#if CONFIG_Water_Meter_High_Pressure_Alarm_Latest_Recorded_Event_Time_OPTION == 1
  time_t Latest_Recorded_Event_Time;
#endif
#if CONFIG_Water_Meter_High_Pressure_Alarm_Alarm_Auto_Clear_OPTION == 1
  bool Alarm_Auto_Clear;
#endif
#if CONFIG_Water_Meter_High_Pressure_Alarm_Event_Code_OPTION == 1
  uint32_t Event_Code;
#endif
#if CONFIG_Water_Meter_High_Pressure_Alarm_Latest_Payload_OPTION == 1
  uint8_t *Latest_Payload;
  uint16_t Latest_Payload_length;
#endif
}
Water_Meter_High_Pressure_Alarm_instance;

static Water_Meter_High_Pressure_Alarm_instance Water_Meter_High_Pressure_Alarm_instances[MAX_Water_Meter_High_Pressure_Alarm_INSTANCES];

lwm2m_object_declaration_t Water_Meter_High_Pressure_Alarm_object_declaration = {
  .id = 8024,
  .delete = Water_Meter_High_Pressure_Alarm_delete_instance,
  .create = __Water_Meter_High_Pressure_Alarm_create_instance,
#if CONFIG_Water_Meter_High_Pressure_Alarm_Event_Type_OPTION == 1
  .resources[0] = { 
    .supported = true,  
    .id = 9020,
    .operation = LWM2M_OPERATION_RW,
    .instances = LWM2M_INSTANCETYPE_single,
    .mandatory = false,
    .type = LWM2M_TYPE_integer,
    .changed = Water_Meter_High_Pressure_Alarm_Event_Type_changed,
    .execute = (void *) 0,
    .get = __Water_Meter_High_Pressure_Alarm_Event_Type_get,
    .set = __Water_Meter_High_Pressure_Alarm_Event_Type_set,
  },
#else
  .resources[0] = { .supported = false },
#endif
#if CONFIG_Water_Meter_High_Pressure_Alarm_Alarm_Realtime_OPTION == 1
  .resources[1] = { 
    .supported = true,  
    .id = 9021,
    .operation = LWM2M_OPERATION_RW,
    .instances = LWM2M_INSTANCETYPE_single,
    .mandatory = false,
    .type = LWM2M_TYPE_boolean,
    .changed = Water_Meter_High_Pressure_Alarm_Alarm_Realtime_changed,
    .execute = (void *) 0,
    .get = __Water_Meter_High_Pressure_Alarm_Alarm_Realtime_get,
    .set = __Water_Meter_High_Pressure_Alarm_Alarm_Realtime_set,
  },
#else
  .resources[1] = { .supported = false },
#endif
#if CONFIG_Water_Meter_High_Pressure_Alarm_Alarm_State_OPTION == 1
  .resources[2] = { 
    .supported = true,  
    .id = 9022,
    .operation = LWM2M_OPERATION_R,
    .instances = LWM2M_INSTANCETYPE_single,
    .mandatory = false,
    .type = LWM2M_TYPE_boolean,
    .changed = (void *) 0,
    .execute = (void *) 0,
    .get = __Water_Meter_High_Pressure_Alarm_Alarm_State_get,
    .set = (void *) 0,
  },
#else
  .resources[2] = { .supported = false },
#endif
#if CONFIG_Water_Meter_High_Pressure_Alarm_Alarm_Set_Threshold_OPTION == 1
  .resources[3] = { 
    .supported = true,  
    .id = 9023,
    .operation = LWM2M_OPERATION_RW,
    .instances = LWM2M_INSTANCETYPE_single,
    .mandatory = false,
    .type = LWM2M_TYPE_float,
    .changed = Water_Meter_High_Pressure_Alarm_Alarm_Set_Threshold_changed,
    .execute = (void *) 0,
    .get = __Water_Meter_High_Pressure_Alarm_Alarm_Set_Threshold_get,
    .set = __Water_Meter_High_Pressure_Alarm_Alarm_Set_Threshold_set,
  },
#else
  .resources[3] = { .supported = false },
#endif
#if CONFIG_Water_Meter_High_Pressure_Alarm_Alarm_Set_Operator_OPTION == 1
  .resources[4] = { 
    .supported = true,  
    .id = 9024,
    .operation = LWM2M_OPERATION_RW,
    .instances = LWM2M_INSTANCETYPE_single,
    .mandatory = false,
    .type = LWM2M_TYPE_integer,
    .changed = Water_Meter_High_Pressure_Alarm_Alarm_Set_Operator_changed,
    .execute = (void *) 0,
    .get = __Water_Meter_High_Pressure_Alarm_Alarm_Set_Operator_get,
    .set = __Water_Meter_High_Pressure_Alarm_Alarm_Set_Operator_set,
  },
#else
  .resources[4] = { .supported = false },
#endif
#if CONFIG_Water_Meter_High_Pressure_Alarm_Alarm_Clear_Threshold_OPTION == 1
  .resources[5] = { 
    .supported = true,  
    .id = 9025,
    .operation = LWM2M_OPERATION_RW,
    .instances = LWM2M_INSTANCETYPE_single,
    .mandatory = false,
    .type = LWM2M_TYPE_float,
    .changed = Water_Meter_High_Pressure_Alarm_Alarm_Clear_Threshold_changed,
    .execute = (void *) 0,
    .get = __Water_Meter_High_Pressure_Alarm_Alarm_Clear_Threshold_get,
    .set = __Water_Meter_High_Pressure_Alarm_Alarm_Clear_Threshold_set,
  },
#else
  .resources[5] = { .supported = false },
#endif
#if CONFIG_Water_Meter_High_Pressure_Alarm_Alarm_Clear_Operator_OPTION == 1
  .resources[6] = { 
    .supported = true,  
    .id = 9026,
    .operation = LWM2M_OPERATION_RW,
    .instances = LWM2M_INSTANCETYPE_single,
    .mandatory = false,
    .type = LWM2M_TYPE_integer,
    .changed = Water_Meter_High_Pressure_Alarm_Alarm_Clear_Operator_changed,
    .execute = (void *) 0,
    .get = __Water_Meter_High_Pressure_Alarm_Alarm_Clear_Operator_get,
    .set = __Water_Meter_High_Pressure_Alarm_Alarm_Clear_Operator_set,
  },
#else
  .resources[6] = { .supported = false },
#endif
#if CONFIG_Water_Meter_High_Pressure_Alarm_Alarm_Maximum_Event_Count_OPTION == 1
  .resources[7] = { 
    .supported = true,  
    .id = 9027,
    .operation = LWM2M_OPERATION_RW,
    .instances = LWM2M_INSTANCETYPE_single,
    .mandatory = false,
    .type = LWM2M_TYPE_integer,
    .changed = Water_Meter_High_Pressure_Alarm_Alarm_Maximum_Event_Count_changed,
    .execute = (void *) 0,
    .get = __Water_Meter_High_Pressure_Alarm_Alarm_Maximum_Event_Count_get,
    .set = __Water_Meter_High_Pressure_Alarm_Alarm_Maximum_Event_Count_set,
  },
#else
  .resources[7] = { .supported = false },
#endif
#if CONFIG_Water_Meter_High_Pressure_Alarm_Alarm_Maximum_Event_Period_OPTION == 1
  .resources[8] = { 
    .supported = true,  
    .id = 9028,
    .operation = LWM2M_OPERATION_RW,
    .instances = LWM2M_INSTANCETYPE_single,
    .mandatory = false,
    .type = LWM2M_TYPE_integer,
    .changed = Water_Meter_High_Pressure_Alarm_Alarm_Maximum_Event_Period_changed,
    .execute = (void *) 0,
    .get = __Water_Meter_High_Pressure_Alarm_Alarm_Maximum_Event_Period_get,
    .set = __Water_Meter_High_Pressure_Alarm_Alarm_Maximum_Event_Period_set,
  },
#else
  .resources[8] = { .supported = false },
#endif
#if CONFIG_Water_Meter_High_Pressure_Alarm_Latest_Delivered_Event_Time_OPTION == 1
  .resources[9] = { 
    .supported = true,  
    .id = 9029,
    .operation = LWM2M_OPERATION_RW,
    .instances = LWM2M_INSTANCETYPE_single,
    .mandatory = false,
    .type = LWM2M_TYPE_time,
    .changed = Water_Meter_High_Pressure_Alarm_Latest_Delivered_Event_Time_changed,
    .execute = (void *) 0,
    .get = __Water_Meter_High_Pressure_Alarm_Latest_Delivered_Event_Time_get,
    .set = __Water_Meter_High_Pressure_Alarm_Latest_Delivered_Event_Time_set,
  },
#else
  .resources[9] = { .supported = false },
#endif
#if CONFIG_Water_Meter_High_Pressure_Alarm_Latest_Recorded_Event_Time_OPTION == 1
  .resources[10] = { 
    .supported = true,  
    .id = 9030,
    .operation = LWM2M_OPERATION_R,
    .instances = LWM2M_INSTANCETYPE_single,
    .mandatory = false,
    .type = LWM2M_TYPE_time,
    .changed = (void *) 0,
    .execute = (void *) 0,
    .get = __Water_Meter_High_Pressure_Alarm_Latest_Recorded_Event_Time_get,
    .set = (void *) 0,
  },
#else
  .resources[10] = { .supported = false },
#endif
#if CONFIG_Water_Meter_High_Pressure_Alarm_Alarm_Clear_OPTION == 1
  .resources[11] = { 
    .supported = true,  
    .id = 9031,
    .operation = LWM2M_OPERATION_E,
    .instances = LWM2M_INSTANCETYPE_single,
    .mandatory = false,
    .type = LWM2M_TYPE_string,
    .changed = (void *) 0,
    .execute = Water_Meter_High_Pressure_Alarm_Alarm_Clear_execute,
    .get = (void *) 0,
    .set = (void *) 0,
  },
#else
  .resources[11] = { .supported = false },
#endif
#if CONFIG_Water_Meter_High_Pressure_Alarm_Alarm_Auto_Clear_OPTION == 1
  .resources[12] = { 
    .supported = true,  
    .id = 9032,
    .operation = LWM2M_OPERATION_RW,
    .instances = LWM2M_INSTANCETYPE_single,
    .mandatory = false,
    .type = LWM2M_TYPE_boolean,
    .changed = Water_Meter_High_Pressure_Alarm_Alarm_Auto_Clear_changed,
    .execute = (void *) 0,
    .get = __Water_Meter_High_Pressure_Alarm_Alarm_Auto_Clear_get,
    .set = __Water_Meter_High_Pressure_Alarm_Alarm_Auto_Clear_set,
  },
#else
  .resources[12] = { .supported = false },
#endif
#if CONFIG_Water_Meter_High_Pressure_Alarm_Event_Code_OPTION == 1
  .resources[13] = { 
    .supported = true,  
    .id = 4010,
    .operation = LWM2M_OPERATION_R,
    .instances = LWM2M_INSTANCETYPE_single,
    .mandatory = false,
    .type = LWM2M_TYPE_integer,
    .changed = (void *) 0,
    .execute = (void *) 0,
    .get = __Water_Meter_High_Pressure_Alarm_Event_Code_get,
    .set = (void *) 0,
  },
#else
  .resources[13] = { .supported = false },
#endif
#if CONFIG_Water_Meter_High_Pressure_Alarm_Latest_Payload_OPTION == 1
  .resources[14] = { 
    .supported = true,  
    .id = 4014,
    .operation = LWM2M_OPERATION_R,
    .instances = LWM2M_INSTANCETYPE_single,
    .mandatory = false,
    .type = LWM2M_TYPE_opaque,
    .changed = (void *) 0,
    .execute = (void *) 0,
    .get = __Water_Meter_High_Pressure_Alarm_Latest_Payload_get,
    .set = (void *) 0,
  },
#else
  .resources[14] = { .supported = false },
#endif
};


void Water_Meter_High_Pressure_Alarm_lock() 
{ 
  mutex_lock(&Water_Meter_High_Pressure_Alarm_object_declaration.lock, MUTEXT_WAIT_FOREVER); 
}
void Water_Meter_High_Pressure_Alarm_unlock() 
{ 
  mutex_unlock(&Water_Meter_High_Pressure_Alarm_object_declaration.lock);
}


uint16_t Water_Meter_High_Pressure_Alarm_get_index_for_instance(uint16_t instance)
{
  for (int i = 0; i < MAX_Water_Meter_High_Pressure_Alarm_INSTANCES; i++)
  {
    if (Water_Meter_High_Pressure_Alarm_instances[i].id == instance) return i;
  }
  return LWM2M_INVALID_INSTANCE;
}

__attribute__((weak)) lwm2m_object_t *Water_Meter_High_Pressure_Alarm_initialise(void)
{
  mutex_initialise(&Water_Meter_High_Pressure_Alarm_object_declaration.lock);
  
  memset(Water_Meter_High_Pressure_Alarm_instances, 0, sizeof(Water_Meter_High_Pressure_Alarm_instances));
  for (int i = 0; i < MAX_Water_Meter_High_Pressure_Alarm_INSTANCES; i++)
  {
    Water_Meter_High_Pressure_Alarm_instances[i].id = LWM2M_INVALID_INSTANCE;
    Water_Meter_High_Pressure_Alarm_instances[i].index = i;
  }
  lwm2m_object_base_initialse_object(&Water_Meter_High_Pressure_Alarm_lwm2m_object);
  Water_Meter_High_Pressure_Alarm_lwm2m_object.userData = &Water_Meter_High_Pressure_Alarm_object_declaration; 
  return &Water_Meter_High_Pressure_Alarm_lwm2m_object;
}
  
__attribute__((weak)) void Water_Meter_High_Pressure_Alarm_register_object(lwm2m_context_t *context)
{
  Water_Meter_High_Pressure_Alarm_lock();
  Water_Meter_High_Pressure_Alarm_object_declaration.context = context;
  Water_Meter_High_Pressure_Alarm_unlock();
}
  
__attribute__((weak)) void Water_Meter_High_Pressure_Alarm_register_instance(uint16_t instance_id)
{
  Water_Meter_High_Pressure_Alarm_lock();
  uint16_t index = Water_Meter_High_Pressure_Alarm_get_index_for_instance(instance_id);
  assert_void(index != LWM2M_INVALID_INSTANCE);
  
  if ((Water_Meter_High_Pressure_Alarm_instances[index].id !=  LWM2M_INVALID_INSTANCE) && (!Water_Meter_High_Pressure_Alarm_instances[index].registered))
  {
    Water_Meter_High_Pressure_Alarm_lwm2m_object.instanceList = LWM2M_LIST_ADD(Water_Meter_High_Pressure_Alarm_lwm2m_object.instanceList, (lwm2m_list_t *)&Water_Meter_High_Pressure_Alarm_instances[index]);
    Water_Meter_High_Pressure_Alarm_instances[index].registered = true;
  }
  Water_Meter_High_Pressure_Alarm_unlock();
}

__attribute__((weak)) void Water_Meter_High_Pressure_Alarm_unregister_instance(uint16_t instance_id)
{
  Water_Meter_High_Pressure_Alarm_lock();
  uint16_t index = Water_Meter_High_Pressure_Alarm_get_index_for_instance(instance_id);
  assert_void(index != LWM2M_INVALID_INSTANCE);
  
  if ((Water_Meter_High_Pressure_Alarm_instances[index].id !=  LWM2M_INVALID_INSTANCE) && (Water_Meter_High_Pressure_Alarm_instances[index].registered))
  {
    Water_Meter_High_Pressure_Alarm_instance *instance = 0;
    Water_Meter_High_Pressure_Alarm_lwm2m_object.instanceList = lwm2m_list_remove(Water_Meter_High_Pressure_Alarm_lwm2m_object.instanceList, instance_id, (lwm2m_list_t **)&instance);
    Water_Meter_High_Pressure_Alarm_instances[index].registered = false;
    if (instance != 0)
    { 
      instance->next = 0;
    }
  }
  Water_Meter_High_Pressure_Alarm_unlock();
}

//override this to perform actions when a new instance is created
__attribute__((weak)) void Water_Meter_High_Pressure_Alarm_instance_created(uint16_t instance_id)
{
  
}
  

//called when the server requests an object to be created; we need to create AND register the 
//instance
uint8_t __Water_Meter_High_Pressure_Alarm_create_instance(uint16_t instance_id)
{
  uint8_t res = Water_Meter_High_Pressure_Alarm_create_instance(instance_id);
  if (res == COAP_201_CREATED)
  {
    Water_Meter_High_Pressure_Alarm_register_instance(instance_id);
  }
  return res;
}
  
__attribute__((weak)) uint8_t Water_Meter_High_Pressure_Alarm_create_instance(uint16_t instance_id)
{
  Water_Meter_High_Pressure_Alarm_lock();
  //find an unallocated instance
  for (int i = 0; i < MAX_Water_Meter_High_Pressure_Alarm_INSTANCES; i++)
  {
    if (Water_Meter_High_Pressure_Alarm_instances[i].id == LWM2M_INVALID_INSTANCE)
    {
      Water_Meter_High_Pressure_Alarm_instances[i].id = instance_id;
      Water_Meter_High_Pressure_Alarm_instance_created(instance_id);
      Water_Meter_High_Pressure_Alarm_unlock();
      return COAP_201_CREATED;
    }
  }
  Water_Meter_High_Pressure_Alarm_unlock();
  return COAP_500_INTERNAL_SERVER_ERROR; //none found
}

void Water_Meter_High_Pressure_Alarm_foreach(lwm2m_each_instance_func func, void *data)
{
  Water_Meter_High_Pressure_Alarm_lock();
  
  for (int i = 0; i < MAX_Water_Meter_High_Pressure_Alarm_INSTANCES; i++)
  {
    if (Water_Meter_High_Pressure_Alarm_instances[i].id != LWM2M_INVALID_INSTANCE)
    {
      if (!func(Water_Meter_High_Pressure_Alarm_instances[i].id, data)) break;
    }
  }
  Water_Meter_High_Pressure_Alarm_unlock();
}
  
//provides access to a user data pointer for storing additional instance
//data.
void *Water_Meter_High_Pressure_Alarm_get_user_data(uint16_t instance_id)
{
  uint16_t index = Water_Meter_High_Pressure_Alarm_get_index_for_instance(instance_id);
  if (index == LWM2M_INVALID_INSTANCE) return 0;
  return Water_Meter_High_Pressure_Alarm_instances[index].user_data;
}
  
bool Water_Meter_High_Pressure_Alarm_set_user_data(uint16_t instance_id, void *data)
{
  uint16_t index = Water_Meter_High_Pressure_Alarm_get_index_for_instance(instance_id);
  if (index == LWM2M_INVALID_INSTANCE) return false;
  Water_Meter_High_Pressure_Alarm_instances[index].user_data = data;
  return true;
}
  
//override this to perform actions when a new instance is created
__attribute__((weak)) void Water_Meter_High_Pressure_Alarm_instance_deleted(uint16_t instance_id)
{
  
}

__attribute__((weak)) uint8_t Water_Meter_High_Pressure_Alarm_delete_instance(uint16_t instance_id)
{
  Water_Meter_High_Pressure_Alarm_lock();
  uint16_t index = Water_Meter_High_Pressure_Alarm_get_index_for_instance(instance_id);
  assert(index != LWM2M_INVALID_INSTANCE);
  
  if ((instance_id < MAX_Water_Meter_High_Pressure_Alarm_INSTANCES) && (Water_Meter_High_Pressure_Alarm_instances[index].id !=  LWM2M_INVALID_INSTANCE))
  {
    Water_Meter_High_Pressure_Alarm_unregister_instance(instance_id);
    Water_Meter_High_Pressure_Alarm_instance_deleted(instance_id);
    Water_Meter_High_Pressure_Alarm_instances[index].id = LWM2M_INVALID_INSTANCE;
  }
  Water_Meter_High_Pressure_Alarm_unlock();
  return COAP_202_DELETED;
}


__attribute__((weak)) void Water_Meter_High_Pressure_Alarm_changed(uint16_t instance)
{
  
}

#if CONFIG_Water_Meter_High_Pressure_Alarm_Event_Type_OPTION == 1
  
__attribute__((weak)) uint8_t Water_Meter_High_Pressure_Alarm_Event_Type_get(uint16_t instance,  uint32_t *val)
{
  Water_Meter_High_Pressure_Alarm_lock();
  assert(instance >= 0 && instance < MAX_Water_Meter_High_Pressure_Alarm_INSTANCES);
  uint16_t index = Water_Meter_High_Pressure_Alarm_get_index_for_instance(instance);
  assert(index != LWM2M_INVALID_INSTANCE);
  
  assert(Water_Meter_High_Pressure_Alarm_instances[index].id != LWM2M_INVALID_INSTANCE);
  
  
  *val = Water_Meter_High_Pressure_Alarm_instances[index].Event_Type;
  Water_Meter_High_Pressure_Alarm_unlock();
  return COAP_205_CONTENT;
}
static uint8_t __Water_Meter_High_Pressure_Alarm_Event_Type_get(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data)
{
  uint32_t val;
  uint8_t res = Water_Meter_High_Pressure_Alarm_Event_Type_get(instance,  &val);
  lwm2m_data_encode_int(val, data);
  return res;
}
                                         
#endif
#if CONFIG_Water_Meter_High_Pressure_Alarm_Event_Type_OPTION == 1
__attribute__((weak)) uint8_t Water_Meter_High_Pressure_Alarm_Event_Type_set(uint16_t instance,  uint32_t val, bool notify)
{
  Water_Meter_High_Pressure_Alarm_lock();
  assert(instance >= 0 && instance < MAX_Water_Meter_High_Pressure_Alarm_INSTANCES);
  uint16_t index = Water_Meter_High_Pressure_Alarm_get_index_for_instance(instance);
  assert(index != LWM2M_INVALID_INSTANCE);
  
  assert(Water_Meter_High_Pressure_Alarm_instances[index].id != LWM2M_INVALID_INSTANCE);

  Water_Meter_High_Pressure_Alarm_instances[index].Event_Type = val;
  
  if (notify && Water_Meter_High_Pressure_Alarm_instances[index].registered) lwm2m_object_base_resource_changed(&Water_Meter_High_Pressure_Alarm_lwm2m_object, instance, 9020, false);  
  Water_Meter_High_Pressure_Alarm_unlock();
  return COAP_204_CHANGED;
}
static uint8_t __Water_Meter_High_Pressure_Alarm_Event_Type_set(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data)
{
  int64_t val;
  lwm2m_data_decode_int(data, &val);
  return Water_Meter_High_Pressure_Alarm_Event_Type_set(instance,  val, false);
}
#endif
#if CONFIG_Water_Meter_High_Pressure_Alarm_Event_Type_OPTION == 1
__attribute__((weak)) uint8_t Water_Meter_High_Pressure_Alarm_Event_Type_changed(uint16_t instance)
{
  Water_Meter_High_Pressure_Alarm_lock();
  assert(instance >= 0 && instance < MAX_Water_Meter_High_Pressure_Alarm_INSTANCES);
  uint16_t index = Water_Meter_High_Pressure_Alarm_get_index_for_instance(instance);
  assert(index != LWM2M_INVALID_INSTANCE);
  
  Water_Meter_High_Pressure_Alarm_changed(instance);
  Water_Meter_High_Pressure_Alarm_unlock();
  return COAP_NO_ERROR;
}
#endif
#if CONFIG_Water_Meter_High_Pressure_Alarm_Alarm_Realtime_OPTION == 1
  
                                         
__attribute__((weak)) uint8_t Water_Meter_High_Pressure_Alarm_Alarm_Realtime_get(uint16_t instance,  bool *val)
{
  Water_Meter_High_Pressure_Alarm_lock();
  assert(instance >= 0 && instance < MAX_Water_Meter_High_Pressure_Alarm_INSTANCES);
  uint16_t index = Water_Meter_High_Pressure_Alarm_get_index_for_instance(instance);
  assert(index != LWM2M_INVALID_INSTANCE);
  
  assert(Water_Meter_High_Pressure_Alarm_instances[index].id != LWM2M_INVALID_INSTANCE);
  
  
  *val = Water_Meter_High_Pressure_Alarm_instances[index].Alarm_Realtime;
  Water_Meter_High_Pressure_Alarm_unlock();
  return COAP_205_CONTENT;
}
static uint8_t __Water_Meter_High_Pressure_Alarm_Alarm_Realtime_get(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data)
{
  bool val;
  uint8_t res = Water_Meter_High_Pressure_Alarm_Alarm_Realtime_get(instance,  &val);
  lwm2m_data_encode_bool(val, data);
  return res;
}
#endif
#if CONFIG_Water_Meter_High_Pressure_Alarm_Alarm_Realtime_OPTION == 1
__attribute__((weak)) uint8_t Water_Meter_High_Pressure_Alarm_Alarm_Realtime_set(uint16_t instance,  bool val, bool notify)
{
  Water_Meter_High_Pressure_Alarm_lock();
  assert(instance >= 0 && instance < MAX_Water_Meter_High_Pressure_Alarm_INSTANCES);
  uint16_t index = Water_Meter_High_Pressure_Alarm_get_index_for_instance(instance);
  assert(index != LWM2M_INVALID_INSTANCE);
  
  assert(Water_Meter_High_Pressure_Alarm_instances[index].id != LWM2M_INVALID_INSTANCE);

  Water_Meter_High_Pressure_Alarm_instances[index].Alarm_Realtime = val;  
  
  
  if (notify && Water_Meter_High_Pressure_Alarm_instances[index].registered) lwm2m_object_base_resource_changed(&Water_Meter_High_Pressure_Alarm_lwm2m_object, instance, 9021, false);
  Water_Meter_High_Pressure_Alarm_unlock();
  return COAP_204_CHANGED;
}
static uint8_t __Water_Meter_High_Pressure_Alarm_Alarm_Realtime_set(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data)
{
  bool val;
  lwm2m_data_decode_bool(data, &val);
  return Water_Meter_High_Pressure_Alarm_Alarm_Realtime_set(instance, val, false);
}
#endif
#if CONFIG_Water_Meter_High_Pressure_Alarm_Alarm_Realtime_OPTION == 1
__attribute__((weak)) uint8_t Water_Meter_High_Pressure_Alarm_Alarm_Realtime_changed(uint16_t instance)
{
  Water_Meter_High_Pressure_Alarm_lock();
  assert(instance >= 0 && instance < MAX_Water_Meter_High_Pressure_Alarm_INSTANCES);
  uint16_t index = Water_Meter_High_Pressure_Alarm_get_index_for_instance(instance);
  assert(index != LWM2M_INVALID_INSTANCE);
  
  Water_Meter_High_Pressure_Alarm_changed(instance);
  Water_Meter_High_Pressure_Alarm_unlock();
  return COAP_NO_ERROR;
}
#endif
#if CONFIG_Water_Meter_High_Pressure_Alarm_Alarm_State_OPTION == 1
  
                                         
__attribute__((weak)) uint8_t Water_Meter_High_Pressure_Alarm_Alarm_State_get(uint16_t instance,  bool *val)
{
  Water_Meter_High_Pressure_Alarm_lock();
  assert(instance >= 0 && instance < MAX_Water_Meter_High_Pressure_Alarm_INSTANCES);
  uint16_t index = Water_Meter_High_Pressure_Alarm_get_index_for_instance(instance);
  assert(index != LWM2M_INVALID_INSTANCE);
  
  assert(Water_Meter_High_Pressure_Alarm_instances[index].id != LWM2M_INVALID_INSTANCE);
  
  
  *val = Water_Meter_High_Pressure_Alarm_instances[index].Alarm_State;
  Water_Meter_High_Pressure_Alarm_unlock();
  return COAP_205_CONTENT;
}
static uint8_t __Water_Meter_High_Pressure_Alarm_Alarm_State_get(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data)
{
  bool val;
  uint8_t res = Water_Meter_High_Pressure_Alarm_Alarm_State_get(instance,  &val);
  lwm2m_data_encode_bool(val, data);
  return res;
}
#endif
#if CONFIG_Water_Meter_High_Pressure_Alarm_Alarm_State_OPTION == 1
__attribute__((weak)) uint8_t Water_Meter_High_Pressure_Alarm_Alarm_State_set(uint16_t instance,  bool val, bool notify)
{
  Water_Meter_High_Pressure_Alarm_lock();
  assert(instance >= 0 && instance < MAX_Water_Meter_High_Pressure_Alarm_INSTANCES);
  uint16_t index = Water_Meter_High_Pressure_Alarm_get_index_for_instance(instance);
  assert(index != LWM2M_INVALID_INSTANCE);
  
  assert(Water_Meter_High_Pressure_Alarm_instances[index].id != LWM2M_INVALID_INSTANCE);

  Water_Meter_High_Pressure_Alarm_instances[index].Alarm_State = val;  
  
  
  if (notify && Water_Meter_High_Pressure_Alarm_instances[index].registered) lwm2m_object_base_resource_changed(&Water_Meter_High_Pressure_Alarm_lwm2m_object, instance, 9022, false);
  Water_Meter_High_Pressure_Alarm_unlock();
  return COAP_204_CHANGED;
}
#endif
#if CONFIG_Water_Meter_High_Pressure_Alarm_Alarm_Set_Threshold_OPTION == 1
  
__attribute__((weak))  uint8_t Water_Meter_High_Pressure_Alarm_Alarm_Set_Threshold_get(uint16_t instance,  double *val)
{
  Water_Meter_High_Pressure_Alarm_lock();
  assert(instance >= 0 && instance < MAX_Water_Meter_High_Pressure_Alarm_INSTANCES); 
  uint16_t index = Water_Meter_High_Pressure_Alarm_get_index_for_instance(instance);
  assert(index != LWM2M_INVALID_INSTANCE);
  
  assert(Water_Meter_High_Pressure_Alarm_instances[index].id != LWM2M_INVALID_INSTANCE);
  
  
  *val = Water_Meter_High_Pressure_Alarm_instances[index].Alarm_Set_Threshold;
  Water_Meter_High_Pressure_Alarm_unlock();
  return COAP_205_CONTENT;
}
static uint8_t __Water_Meter_High_Pressure_Alarm_Alarm_Set_Threshold_get(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data)
{
  double val;
  uint8_t res = Water_Meter_High_Pressure_Alarm_Alarm_Set_Threshold_get(instance,  &val);
  lwm2m_data_encode_float(val, data);
  return res;
}
                                         
#endif
#if CONFIG_Water_Meter_High_Pressure_Alarm_Alarm_Set_Threshold_OPTION == 1
__attribute__((weak)) uint8_t Water_Meter_High_Pressure_Alarm_Alarm_Set_Threshold_set(uint16_t instance,  double val, bool notify)
{
  Water_Meter_High_Pressure_Alarm_lock();
  assert(instance >= 0 && instance < MAX_Water_Meter_High_Pressure_Alarm_INSTANCES);
  uint16_t index = Water_Meter_High_Pressure_Alarm_get_index_for_instance(instance);
  assert(index != LWM2M_INVALID_INSTANCE);
  
  assert(Water_Meter_High_Pressure_Alarm_instances[index].id != LWM2M_INVALID_INSTANCE);
 
  Water_Meter_High_Pressure_Alarm_instances[index].Alarm_Set_Threshold = val;
  
  
  if (notify && Water_Meter_High_Pressure_Alarm_instances[index].registered) lwm2m_object_base_resource_changed(&Water_Meter_High_Pressure_Alarm_lwm2m_object, instance, 9023, false);
  Water_Meter_High_Pressure_Alarm_unlock();
  return COAP_204_CHANGED;
}
static uint8_t __Water_Meter_High_Pressure_Alarm_Alarm_Set_Threshold_set(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data)
{
  double val;
  lwm2m_data_decode_float(data, &val);
  return Water_Meter_High_Pressure_Alarm_Alarm_Set_Threshold_set(instance, val, false);
}
#endif
#if CONFIG_Water_Meter_High_Pressure_Alarm_Alarm_Set_Threshold_OPTION == 1
__attribute__((weak)) uint8_t Water_Meter_High_Pressure_Alarm_Alarm_Set_Threshold_changed(uint16_t instance)
{
  Water_Meter_High_Pressure_Alarm_lock();
  assert(instance >= 0 && instance < MAX_Water_Meter_High_Pressure_Alarm_INSTANCES);
  uint16_t index = Water_Meter_High_Pressure_Alarm_get_index_for_instance(instance);
  assert(index != LWM2M_INVALID_INSTANCE);
  
  Water_Meter_High_Pressure_Alarm_changed(instance);
  Water_Meter_High_Pressure_Alarm_unlock();
  return COAP_NO_ERROR;
}
#endif
#if CONFIG_Water_Meter_High_Pressure_Alarm_Alarm_Set_Operator_OPTION == 1
  
__attribute__((weak)) uint8_t Water_Meter_High_Pressure_Alarm_Alarm_Set_Operator_get(uint16_t instance,  uint32_t *val)
{
  Water_Meter_High_Pressure_Alarm_lock();
  assert(instance >= 0 && instance < MAX_Water_Meter_High_Pressure_Alarm_INSTANCES);
  uint16_t index = Water_Meter_High_Pressure_Alarm_get_index_for_instance(instance);
  assert(index != LWM2M_INVALID_INSTANCE);
  
  assert(Water_Meter_High_Pressure_Alarm_instances[index].id != LWM2M_INVALID_INSTANCE);
  
  
  *val = Water_Meter_High_Pressure_Alarm_instances[index].Alarm_Set_Operator;
  Water_Meter_High_Pressure_Alarm_unlock();
  return COAP_205_CONTENT;
}
static uint8_t __Water_Meter_High_Pressure_Alarm_Alarm_Set_Operator_get(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data)
{
  uint32_t val;
  uint8_t res = Water_Meter_High_Pressure_Alarm_Alarm_Set_Operator_get(instance,  &val);
  lwm2m_data_encode_int(val, data);
  return res;
}
                                         
#endif
#if CONFIG_Water_Meter_High_Pressure_Alarm_Alarm_Set_Operator_OPTION == 1
__attribute__((weak)) uint8_t Water_Meter_High_Pressure_Alarm_Alarm_Set_Operator_set(uint16_t instance,  uint32_t val, bool notify)
{
  Water_Meter_High_Pressure_Alarm_lock();
  assert(instance >= 0 && instance < MAX_Water_Meter_High_Pressure_Alarm_INSTANCES);
  uint16_t index = Water_Meter_High_Pressure_Alarm_get_index_for_instance(instance);
  assert(index != LWM2M_INVALID_INSTANCE);
  
  assert(Water_Meter_High_Pressure_Alarm_instances[index].id != LWM2M_INVALID_INSTANCE);

  Water_Meter_High_Pressure_Alarm_instances[index].Alarm_Set_Operator = val;
  
  if (notify && Water_Meter_High_Pressure_Alarm_instances[index].registered) lwm2m_object_base_resource_changed(&Water_Meter_High_Pressure_Alarm_lwm2m_object, instance, 9024, false);  
  Water_Meter_High_Pressure_Alarm_unlock();
  return COAP_204_CHANGED;
}
static uint8_t __Water_Meter_High_Pressure_Alarm_Alarm_Set_Operator_set(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data)
{
  int64_t val;
  lwm2m_data_decode_int(data, &val);
  return Water_Meter_High_Pressure_Alarm_Alarm_Set_Operator_set(instance,  val, false);
}
#endif
#if CONFIG_Water_Meter_High_Pressure_Alarm_Alarm_Set_Operator_OPTION == 1
__attribute__((weak)) uint8_t Water_Meter_High_Pressure_Alarm_Alarm_Set_Operator_changed(uint16_t instance)
{
  Water_Meter_High_Pressure_Alarm_lock();
  assert(instance >= 0 && instance < MAX_Water_Meter_High_Pressure_Alarm_INSTANCES);
  uint16_t index = Water_Meter_High_Pressure_Alarm_get_index_for_instance(instance);
  assert(index != LWM2M_INVALID_INSTANCE);
  
  Water_Meter_High_Pressure_Alarm_changed(instance);
  Water_Meter_High_Pressure_Alarm_unlock();
  return COAP_NO_ERROR;
}
#endif
#if CONFIG_Water_Meter_High_Pressure_Alarm_Alarm_Clear_Threshold_OPTION == 1
  
__attribute__((weak))  uint8_t Water_Meter_High_Pressure_Alarm_Alarm_Clear_Threshold_get(uint16_t instance,  double *val)
{
  Water_Meter_High_Pressure_Alarm_lock();
  assert(instance >= 0 && instance < MAX_Water_Meter_High_Pressure_Alarm_INSTANCES); 
  uint16_t index = Water_Meter_High_Pressure_Alarm_get_index_for_instance(instance);
  assert(index != LWM2M_INVALID_INSTANCE);
  
  assert(Water_Meter_High_Pressure_Alarm_instances[index].id != LWM2M_INVALID_INSTANCE);
  
  
  *val = Water_Meter_High_Pressure_Alarm_instances[index].Alarm_Clear_Threshold;
  Water_Meter_High_Pressure_Alarm_unlock();
  return COAP_205_CONTENT;
}
static uint8_t __Water_Meter_High_Pressure_Alarm_Alarm_Clear_Threshold_get(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data)
{
  double val;
  uint8_t res = Water_Meter_High_Pressure_Alarm_Alarm_Clear_Threshold_get(instance,  &val);
  lwm2m_data_encode_float(val, data);
  return res;
}
                                         
#endif
#if CONFIG_Water_Meter_High_Pressure_Alarm_Alarm_Clear_Threshold_OPTION == 1
__attribute__((weak)) uint8_t Water_Meter_High_Pressure_Alarm_Alarm_Clear_Threshold_set(uint16_t instance,  double val, bool notify)
{
  Water_Meter_High_Pressure_Alarm_lock();
  assert(instance >= 0 && instance < MAX_Water_Meter_High_Pressure_Alarm_INSTANCES);
  uint16_t index = Water_Meter_High_Pressure_Alarm_get_index_for_instance(instance);
  assert(index != LWM2M_INVALID_INSTANCE);
  
  assert(Water_Meter_High_Pressure_Alarm_instances[index].id != LWM2M_INVALID_INSTANCE);
 
  Water_Meter_High_Pressure_Alarm_instances[index].Alarm_Clear_Threshold = val;
  
  
  if (notify && Water_Meter_High_Pressure_Alarm_instances[index].registered) lwm2m_object_base_resource_changed(&Water_Meter_High_Pressure_Alarm_lwm2m_object, instance, 9025, false);
  Water_Meter_High_Pressure_Alarm_unlock();
  return COAP_204_CHANGED;
}
static uint8_t __Water_Meter_High_Pressure_Alarm_Alarm_Clear_Threshold_set(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data)
{
  double val;
  lwm2m_data_decode_float(data, &val);
  return Water_Meter_High_Pressure_Alarm_Alarm_Clear_Threshold_set(instance, val, false);
}
#endif
#if CONFIG_Water_Meter_High_Pressure_Alarm_Alarm_Clear_Threshold_OPTION == 1
__attribute__((weak)) uint8_t Water_Meter_High_Pressure_Alarm_Alarm_Clear_Threshold_changed(uint16_t instance)
{
  Water_Meter_High_Pressure_Alarm_lock();
  assert(instance >= 0 && instance < MAX_Water_Meter_High_Pressure_Alarm_INSTANCES);
  uint16_t index = Water_Meter_High_Pressure_Alarm_get_index_for_instance(instance);
  assert(index != LWM2M_INVALID_INSTANCE);
  
  Water_Meter_High_Pressure_Alarm_changed(instance);
  Water_Meter_High_Pressure_Alarm_unlock();
  return COAP_NO_ERROR;
}
#endif
#if CONFIG_Water_Meter_High_Pressure_Alarm_Alarm_Clear_Operator_OPTION == 1
  
__attribute__((weak)) uint8_t Water_Meter_High_Pressure_Alarm_Alarm_Clear_Operator_get(uint16_t instance,  uint32_t *val)
{
  Water_Meter_High_Pressure_Alarm_lock();
  assert(instance >= 0 && instance < MAX_Water_Meter_High_Pressure_Alarm_INSTANCES);
  uint16_t index = Water_Meter_High_Pressure_Alarm_get_index_for_instance(instance);
  assert(index != LWM2M_INVALID_INSTANCE);
  
  assert(Water_Meter_High_Pressure_Alarm_instances[index].id != LWM2M_INVALID_INSTANCE);
  
  
  *val = Water_Meter_High_Pressure_Alarm_instances[index].Alarm_Clear_Operator;
  Water_Meter_High_Pressure_Alarm_unlock();
  return COAP_205_CONTENT;
}
static uint8_t __Water_Meter_High_Pressure_Alarm_Alarm_Clear_Operator_get(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data)
{
  uint32_t val;
  uint8_t res = Water_Meter_High_Pressure_Alarm_Alarm_Clear_Operator_get(instance,  &val);
  lwm2m_data_encode_int(val, data);
  return res;
}
                                         
#endif
#if CONFIG_Water_Meter_High_Pressure_Alarm_Alarm_Clear_Operator_OPTION == 1
__attribute__((weak)) uint8_t Water_Meter_High_Pressure_Alarm_Alarm_Clear_Operator_set(uint16_t instance,  uint32_t val, bool notify)
{
  Water_Meter_High_Pressure_Alarm_lock();
  assert(instance >= 0 && instance < MAX_Water_Meter_High_Pressure_Alarm_INSTANCES);
  uint16_t index = Water_Meter_High_Pressure_Alarm_get_index_for_instance(instance);
  assert(index != LWM2M_INVALID_INSTANCE);
  
  assert(Water_Meter_High_Pressure_Alarm_instances[index].id != LWM2M_INVALID_INSTANCE);

  Water_Meter_High_Pressure_Alarm_instances[index].Alarm_Clear_Operator = val;
  
  if (notify && Water_Meter_High_Pressure_Alarm_instances[index].registered) lwm2m_object_base_resource_changed(&Water_Meter_High_Pressure_Alarm_lwm2m_object, instance, 9026, false);  
  Water_Meter_High_Pressure_Alarm_unlock();
  return COAP_204_CHANGED;
}
static uint8_t __Water_Meter_High_Pressure_Alarm_Alarm_Clear_Operator_set(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data)
{
  int64_t val;
  lwm2m_data_decode_int(data, &val);
  return Water_Meter_High_Pressure_Alarm_Alarm_Clear_Operator_set(instance,  val, false);
}
#endif
#if CONFIG_Water_Meter_High_Pressure_Alarm_Alarm_Clear_Operator_OPTION == 1
__attribute__((weak)) uint8_t Water_Meter_High_Pressure_Alarm_Alarm_Clear_Operator_changed(uint16_t instance)
{
  Water_Meter_High_Pressure_Alarm_lock();
  assert(instance >= 0 && instance < MAX_Water_Meter_High_Pressure_Alarm_INSTANCES);
  uint16_t index = Water_Meter_High_Pressure_Alarm_get_index_for_instance(instance);
  assert(index != LWM2M_INVALID_INSTANCE);
  
  Water_Meter_High_Pressure_Alarm_changed(instance);
  Water_Meter_High_Pressure_Alarm_unlock();
  return COAP_NO_ERROR;
}
#endif
#if CONFIG_Water_Meter_High_Pressure_Alarm_Alarm_Maximum_Event_Count_OPTION == 1
  
__attribute__((weak)) uint8_t Water_Meter_High_Pressure_Alarm_Alarm_Maximum_Event_Count_get(uint16_t instance,  uint32_t *val)
{
  Water_Meter_High_Pressure_Alarm_lock();
  assert(instance >= 0 && instance < MAX_Water_Meter_High_Pressure_Alarm_INSTANCES);
  uint16_t index = Water_Meter_High_Pressure_Alarm_get_index_for_instance(instance);
  assert(index != LWM2M_INVALID_INSTANCE);
  
  assert(Water_Meter_High_Pressure_Alarm_instances[index].id != LWM2M_INVALID_INSTANCE);
  
  
  *val = Water_Meter_High_Pressure_Alarm_instances[index].Alarm_Maximum_Event_Count;
  Water_Meter_High_Pressure_Alarm_unlock();
  return COAP_205_CONTENT;
}
static uint8_t __Water_Meter_High_Pressure_Alarm_Alarm_Maximum_Event_Count_get(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data)
{
  uint32_t val;
  uint8_t res = Water_Meter_High_Pressure_Alarm_Alarm_Maximum_Event_Count_get(instance,  &val);
  lwm2m_data_encode_int(val, data);
  return res;
}
                                         
#endif
#if CONFIG_Water_Meter_High_Pressure_Alarm_Alarm_Maximum_Event_Count_OPTION == 1
__attribute__((weak)) uint8_t Water_Meter_High_Pressure_Alarm_Alarm_Maximum_Event_Count_set(uint16_t instance,  uint32_t val, bool notify)
{
  Water_Meter_High_Pressure_Alarm_lock();
  assert(instance >= 0 && instance < MAX_Water_Meter_High_Pressure_Alarm_INSTANCES);
  uint16_t index = Water_Meter_High_Pressure_Alarm_get_index_for_instance(instance);
  assert(index != LWM2M_INVALID_INSTANCE);
  
  assert(Water_Meter_High_Pressure_Alarm_instances[index].id != LWM2M_INVALID_INSTANCE);

  Water_Meter_High_Pressure_Alarm_instances[index].Alarm_Maximum_Event_Count = val;
  
  if (notify && Water_Meter_High_Pressure_Alarm_instances[index].registered) lwm2m_object_base_resource_changed(&Water_Meter_High_Pressure_Alarm_lwm2m_object, instance, 9027, false);  
  Water_Meter_High_Pressure_Alarm_unlock();
  return COAP_204_CHANGED;
}
static uint8_t __Water_Meter_High_Pressure_Alarm_Alarm_Maximum_Event_Count_set(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data)
{
  int64_t val;
  lwm2m_data_decode_int(data, &val);
  return Water_Meter_High_Pressure_Alarm_Alarm_Maximum_Event_Count_set(instance,  val, false);
}
#endif
#if CONFIG_Water_Meter_High_Pressure_Alarm_Alarm_Maximum_Event_Count_OPTION == 1
__attribute__((weak)) uint8_t Water_Meter_High_Pressure_Alarm_Alarm_Maximum_Event_Count_changed(uint16_t instance)
{
  Water_Meter_High_Pressure_Alarm_lock();
  assert(instance >= 0 && instance < MAX_Water_Meter_High_Pressure_Alarm_INSTANCES);
  uint16_t index = Water_Meter_High_Pressure_Alarm_get_index_for_instance(instance);
  assert(index != LWM2M_INVALID_INSTANCE);
  
  Water_Meter_High_Pressure_Alarm_changed(instance);
  Water_Meter_High_Pressure_Alarm_unlock();
  return COAP_NO_ERROR;
}
#endif
#if CONFIG_Water_Meter_High_Pressure_Alarm_Alarm_Maximum_Event_Period_OPTION == 1
  
__attribute__((weak)) uint8_t Water_Meter_High_Pressure_Alarm_Alarm_Maximum_Event_Period_get(uint16_t instance,  uint32_t *val)
{
  Water_Meter_High_Pressure_Alarm_lock();
  assert(instance >= 0 && instance < MAX_Water_Meter_High_Pressure_Alarm_INSTANCES);
  uint16_t index = Water_Meter_High_Pressure_Alarm_get_index_for_instance(instance);
  assert(index != LWM2M_INVALID_INSTANCE);
  
  assert(Water_Meter_High_Pressure_Alarm_instances[index].id != LWM2M_INVALID_INSTANCE);
  
  
  *val = Water_Meter_High_Pressure_Alarm_instances[index].Alarm_Maximum_Event_Period;
  Water_Meter_High_Pressure_Alarm_unlock();
  return COAP_205_CONTENT;
}
static uint8_t __Water_Meter_High_Pressure_Alarm_Alarm_Maximum_Event_Period_get(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data)
{
  uint32_t val;
  uint8_t res = Water_Meter_High_Pressure_Alarm_Alarm_Maximum_Event_Period_get(instance,  &val);
  lwm2m_data_encode_int(val, data);
  return res;
}
                                         
#endif
#if CONFIG_Water_Meter_High_Pressure_Alarm_Alarm_Maximum_Event_Period_OPTION == 1
__attribute__((weak)) uint8_t Water_Meter_High_Pressure_Alarm_Alarm_Maximum_Event_Period_set(uint16_t instance,  uint32_t val, bool notify)
{
  Water_Meter_High_Pressure_Alarm_lock();
  assert(instance >= 0 && instance < MAX_Water_Meter_High_Pressure_Alarm_INSTANCES);
  uint16_t index = Water_Meter_High_Pressure_Alarm_get_index_for_instance(instance);
  assert(index != LWM2M_INVALID_INSTANCE);
  
  assert(Water_Meter_High_Pressure_Alarm_instances[index].id != LWM2M_INVALID_INSTANCE);

  Water_Meter_High_Pressure_Alarm_instances[index].Alarm_Maximum_Event_Period = val;
  
  if (notify && Water_Meter_High_Pressure_Alarm_instances[index].registered) lwm2m_object_base_resource_changed(&Water_Meter_High_Pressure_Alarm_lwm2m_object, instance, 9028, false);  
  Water_Meter_High_Pressure_Alarm_unlock();
  return COAP_204_CHANGED;
}
static uint8_t __Water_Meter_High_Pressure_Alarm_Alarm_Maximum_Event_Period_set(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data)
{
  int64_t val;
  lwm2m_data_decode_int(data, &val);
  return Water_Meter_High_Pressure_Alarm_Alarm_Maximum_Event_Period_set(instance,  val, false);
}
#endif
#if CONFIG_Water_Meter_High_Pressure_Alarm_Alarm_Maximum_Event_Period_OPTION == 1
__attribute__((weak)) uint8_t Water_Meter_High_Pressure_Alarm_Alarm_Maximum_Event_Period_changed(uint16_t instance)
{
  Water_Meter_High_Pressure_Alarm_lock();
  assert(instance >= 0 && instance < MAX_Water_Meter_High_Pressure_Alarm_INSTANCES);
  uint16_t index = Water_Meter_High_Pressure_Alarm_get_index_for_instance(instance);
  assert(index != LWM2M_INVALID_INSTANCE);
  
  Water_Meter_High_Pressure_Alarm_changed(instance);
  Water_Meter_High_Pressure_Alarm_unlock();
  return COAP_NO_ERROR;
}
#endif
#if CONFIG_Water_Meter_High_Pressure_Alarm_Latest_Delivered_Event_Time_OPTION == 1
  
__attribute__((weak))  uint8_t Water_Meter_High_Pressure_Alarm_Latest_Delivered_Event_Time_get(uint16_t instance,  time_t *val)
{
  Water_Meter_High_Pressure_Alarm_lock();
  uint16_t index = Water_Meter_High_Pressure_Alarm_get_index_for_instance(instance);
  assert(index != LWM2M_INVALID_INSTANCE);
  
  assert(Water_Meter_High_Pressure_Alarm_instances[index].id != LWM2M_INVALID_INSTANCE);
  
                                                                                           
  *val = Water_Meter_High_Pressure_Alarm_instances[index].Latest_Delivered_Event_Time;
  Water_Meter_High_Pressure_Alarm_unlock();
  return COAP_205_CONTENT;
}
static uint8_t __Water_Meter_High_Pressure_Alarm_Latest_Delivered_Event_Time_get(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data)
{
 time_t val;
 uint8_t res = Water_Meter_High_Pressure_Alarm_Latest_Delivered_Event_Time_get(instance,  &val);
 lwm2m_data_encode_int(val, data);
 return res;
}
                                         
#endif
#if CONFIG_Water_Meter_High_Pressure_Alarm_Latest_Delivered_Event_Time_OPTION == 1
__attribute__((weak)) uint8_t Water_Meter_High_Pressure_Alarm_Latest_Delivered_Event_Time_set(uint16_t instance, time_t val, bool notify)
{
  Water_Meter_High_Pressure_Alarm_lock();
  assert(instance >= 0 && instance < MAX_Water_Meter_High_Pressure_Alarm_INSTANCES);
  uint16_t index = Water_Meter_High_Pressure_Alarm_get_index_for_instance(instance);
  assert(index != LWM2M_INVALID_INSTANCE);
  
  assert(Water_Meter_High_Pressure_Alarm_instances[index].id != LWM2M_INVALID_INSTANCE);

  Water_Meter_High_Pressure_Alarm_instances[index].Latest_Delivered_Event_Time = val;   
  
  
  if (notify && Water_Meter_High_Pressure_Alarm_instances[index].registered) lwm2m_object_base_resource_changed(&Water_Meter_High_Pressure_Alarm_lwm2m_object, instance, 9029,false);
  Water_Meter_High_Pressure_Alarm_unlock();
  return COAP_204_CHANGED;
}
static uint8_t __Water_Meter_High_Pressure_Alarm_Latest_Delivered_Event_Time_set(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data)
{
  time_t val;
  int64_t tmp;
  lwm2m_data_decode_int(data, &tmp);
  val = (time_t)tmp;
  return Water_Meter_High_Pressure_Alarm_Latest_Delivered_Event_Time_set(instance,  val, false);
}
#endif
#if CONFIG_Water_Meter_High_Pressure_Alarm_Latest_Delivered_Event_Time_OPTION == 1
__attribute__((weak)) uint8_t Water_Meter_High_Pressure_Alarm_Latest_Delivered_Event_Time_changed(uint16_t instance)
{
  Water_Meter_High_Pressure_Alarm_lock();
  assert(instance >= 0 && instance < MAX_Water_Meter_High_Pressure_Alarm_INSTANCES);
  uint16_t index = Water_Meter_High_Pressure_Alarm_get_index_for_instance(instance);
  assert(index != LWM2M_INVALID_INSTANCE);
  
  Water_Meter_High_Pressure_Alarm_changed(instance);
  Water_Meter_High_Pressure_Alarm_unlock();
  return COAP_NO_ERROR;
}
#endif
#if CONFIG_Water_Meter_High_Pressure_Alarm_Latest_Recorded_Event_Time_OPTION == 1
  
__attribute__((weak))  uint8_t Water_Meter_High_Pressure_Alarm_Latest_Recorded_Event_Time_get(uint16_t instance,  time_t *val)
{
  Water_Meter_High_Pressure_Alarm_lock();
  uint16_t index = Water_Meter_High_Pressure_Alarm_get_index_for_instance(instance);
  assert(index != LWM2M_INVALID_INSTANCE);
  
  assert(Water_Meter_High_Pressure_Alarm_instances[index].id != LWM2M_INVALID_INSTANCE);
  
                                                                                           
  *val = Water_Meter_High_Pressure_Alarm_instances[index].Latest_Recorded_Event_Time;
  Water_Meter_High_Pressure_Alarm_unlock();
  return COAP_205_CONTENT;
}
static uint8_t __Water_Meter_High_Pressure_Alarm_Latest_Recorded_Event_Time_get(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data)
{
 time_t val;
 uint8_t res = Water_Meter_High_Pressure_Alarm_Latest_Recorded_Event_Time_get(instance,  &val);
 lwm2m_data_encode_int(val, data);
 return res;
}
                                         
#endif
#if CONFIG_Water_Meter_High_Pressure_Alarm_Latest_Recorded_Event_Time_OPTION == 1
__attribute__((weak)) uint8_t Water_Meter_High_Pressure_Alarm_Latest_Recorded_Event_Time_set(uint16_t instance, time_t val, bool notify)
{
  Water_Meter_High_Pressure_Alarm_lock();
  assert(instance >= 0 && instance < MAX_Water_Meter_High_Pressure_Alarm_INSTANCES);
  uint16_t index = Water_Meter_High_Pressure_Alarm_get_index_for_instance(instance);
  assert(index != LWM2M_INVALID_INSTANCE);
  
  assert(Water_Meter_High_Pressure_Alarm_instances[index].id != LWM2M_INVALID_INSTANCE);

  Water_Meter_High_Pressure_Alarm_instances[index].Latest_Recorded_Event_Time = val;   
  
  
  if (notify && Water_Meter_High_Pressure_Alarm_instances[index].registered) lwm2m_object_base_resource_changed(&Water_Meter_High_Pressure_Alarm_lwm2m_object, instance, 9030,false);
  Water_Meter_High_Pressure_Alarm_unlock();
  return COAP_204_CHANGED;
}
#endif
#if CONFIG_Water_Meter_High_Pressure_Alarm_Alarm_Clear_OPTION == 1
__attribute__((weak)) uint8_t Water_Meter_High_Pressure_Alarm_Alarm_Clear_execute(lwm2m_object_t *object, uint16_t instance, uint16_t resource_id, uint8_t * buffer, int length)
{
  return COAP_405_METHOD_NOT_ALLOWED;
}
#endif
#if CONFIG_Water_Meter_High_Pressure_Alarm_Alarm_Auto_Clear_OPTION == 1
  
                                         
__attribute__((weak)) uint8_t Water_Meter_High_Pressure_Alarm_Alarm_Auto_Clear_get(uint16_t instance,  bool *val)
{
  Water_Meter_High_Pressure_Alarm_lock();
  assert(instance >= 0 && instance < MAX_Water_Meter_High_Pressure_Alarm_INSTANCES);
  uint16_t index = Water_Meter_High_Pressure_Alarm_get_index_for_instance(instance);
  assert(index != LWM2M_INVALID_INSTANCE);
  
  assert(Water_Meter_High_Pressure_Alarm_instances[index].id != LWM2M_INVALID_INSTANCE);
  
  
  *val = Water_Meter_High_Pressure_Alarm_instances[index].Alarm_Auto_Clear;
  Water_Meter_High_Pressure_Alarm_unlock();
  return COAP_205_CONTENT;
}
static uint8_t __Water_Meter_High_Pressure_Alarm_Alarm_Auto_Clear_get(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data)
{
  bool val;
  uint8_t res = Water_Meter_High_Pressure_Alarm_Alarm_Auto_Clear_get(instance,  &val);
  lwm2m_data_encode_bool(val, data);
  return res;
}
#endif
#if CONFIG_Water_Meter_High_Pressure_Alarm_Alarm_Auto_Clear_OPTION == 1
__attribute__((weak)) uint8_t Water_Meter_High_Pressure_Alarm_Alarm_Auto_Clear_set(uint16_t instance,  bool val, bool notify)
{
  Water_Meter_High_Pressure_Alarm_lock();
  assert(instance >= 0 && instance < MAX_Water_Meter_High_Pressure_Alarm_INSTANCES);
  uint16_t index = Water_Meter_High_Pressure_Alarm_get_index_for_instance(instance);
  assert(index != LWM2M_INVALID_INSTANCE);
  
  assert(Water_Meter_High_Pressure_Alarm_instances[index].id != LWM2M_INVALID_INSTANCE);

  Water_Meter_High_Pressure_Alarm_instances[index].Alarm_Auto_Clear = val;  
  
  
  if (notify && Water_Meter_High_Pressure_Alarm_instances[index].registered) lwm2m_object_base_resource_changed(&Water_Meter_High_Pressure_Alarm_lwm2m_object, instance, 9032, false);
  Water_Meter_High_Pressure_Alarm_unlock();
  return COAP_204_CHANGED;
}
static uint8_t __Water_Meter_High_Pressure_Alarm_Alarm_Auto_Clear_set(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data)
{
  bool val;
  lwm2m_data_decode_bool(data, &val);
  return Water_Meter_High_Pressure_Alarm_Alarm_Auto_Clear_set(instance, val, false);
}
#endif
#if CONFIG_Water_Meter_High_Pressure_Alarm_Alarm_Auto_Clear_OPTION == 1
__attribute__((weak)) uint8_t Water_Meter_High_Pressure_Alarm_Alarm_Auto_Clear_changed(uint16_t instance)
{
  Water_Meter_High_Pressure_Alarm_lock();
  assert(instance >= 0 && instance < MAX_Water_Meter_High_Pressure_Alarm_INSTANCES);
  uint16_t index = Water_Meter_High_Pressure_Alarm_get_index_for_instance(instance);
  assert(index != LWM2M_INVALID_INSTANCE);
  
  Water_Meter_High_Pressure_Alarm_changed(instance);
  Water_Meter_High_Pressure_Alarm_unlock();
  return COAP_NO_ERROR;
}
#endif
#if CONFIG_Water_Meter_High_Pressure_Alarm_Event_Code_OPTION == 1
  
__attribute__((weak)) uint8_t Water_Meter_High_Pressure_Alarm_Event_Code_get(uint16_t instance,  uint32_t *val)
{
  Water_Meter_High_Pressure_Alarm_lock();
  assert(instance >= 0 && instance < MAX_Water_Meter_High_Pressure_Alarm_INSTANCES);
  uint16_t index = Water_Meter_High_Pressure_Alarm_get_index_for_instance(instance);
  assert(index != LWM2M_INVALID_INSTANCE);
  
  assert(Water_Meter_High_Pressure_Alarm_instances[index].id != LWM2M_INVALID_INSTANCE);
  
  
  *val = Water_Meter_High_Pressure_Alarm_instances[index].Event_Code;
  Water_Meter_High_Pressure_Alarm_unlock();
  return COAP_205_CONTENT;
}
static uint8_t __Water_Meter_High_Pressure_Alarm_Event_Code_get(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data)
{
  uint32_t val;
  uint8_t res = Water_Meter_High_Pressure_Alarm_Event_Code_get(instance,  &val);
  lwm2m_data_encode_int(val, data);
  return res;
}
                                         
#endif
#if CONFIG_Water_Meter_High_Pressure_Alarm_Event_Code_OPTION == 1
__attribute__((weak)) uint8_t Water_Meter_High_Pressure_Alarm_Event_Code_set(uint16_t instance,  uint32_t val, bool notify)
{
  Water_Meter_High_Pressure_Alarm_lock();
  assert(instance >= 0 && instance < MAX_Water_Meter_High_Pressure_Alarm_INSTANCES);
  uint16_t index = Water_Meter_High_Pressure_Alarm_get_index_for_instance(instance);
  assert(index != LWM2M_INVALID_INSTANCE);
  
  assert(Water_Meter_High_Pressure_Alarm_instances[index].id != LWM2M_INVALID_INSTANCE);

  Water_Meter_High_Pressure_Alarm_instances[index].Event_Code = val;
  
  if (notify && Water_Meter_High_Pressure_Alarm_instances[index].registered) lwm2m_object_base_resource_changed(&Water_Meter_High_Pressure_Alarm_lwm2m_object, instance, 4010, false);  
  Water_Meter_High_Pressure_Alarm_unlock();
  return COAP_204_CHANGED;
}
#endif
#if CONFIG_Water_Meter_High_Pressure_Alarm_Latest_Payload_OPTION == 1
  
__attribute__((weak))  uint8_t Water_Meter_High_Pressure_Alarm_Latest_Payload_get(uint16_t instance,  uint8_t **val, size_t *length)
{
  Water_Meter_High_Pressure_Alarm_lock();
  assert(instance >= 0 && instance < MAX_Water_Meter_High_Pressure_Alarm_INSTANCES);
  uint16_t index = Water_Meter_High_Pressure_Alarm_get_index_for_instance(instance);
  assert(index != LWM2M_INVALID_INSTANCE);
  
  assert(Water_Meter_High_Pressure_Alarm_instances[index].id != LWM2M_INVALID_INSTANCE);
  
  
  
  *val = Water_Meter_High_Pressure_Alarm_instances[index].Latest_Payload;
  *length = Water_Meter_High_Pressure_Alarm_instances[index].Latest_Payload_length;
  Water_Meter_High_Pressure_Alarm_unlock();
  return COAP_205_CONTENT;
}
static uint8_t __Water_Meter_High_Pressure_Alarm_Latest_Payload_get(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data)
{
  uint8_t *val;
  size_t length = 0;
  
  uint8_t res = Water_Meter_High_Pressure_Alarm_Latest_Payload_get(instance,  &val, &length);
  
  lwm2m_data_encode_opaque(val, length, data);
  return res;
}
                                         
#endif
#if CONFIG_Water_Meter_High_Pressure_Alarm_Latest_Payload_OPTION == 1
 __attribute__((weak)) uint8_t Water_Meter_High_Pressure_Alarm_Latest_Payload_set(uint16_t instance,  uint8_t* val, size_t length, bool notify)
 {
   Water_Meter_High_Pressure_Alarm_lock();
   assert(instance >= 0 && instance < MAX_Water_Meter_High_Pressure_Alarm_INSTANCES);
 
   uint16_t index = Water_Meter_High_Pressure_Alarm_get_index_for_instance(instance);
   assert(index != LWM2M_INVALID_INSTANCE);
   
   if (Water_Meter_High_Pressure_Alarm_instances[index].Latest_Payload != 0) lwm2m_free(Water_Meter_High_Pressure_Alarm_instances[index].Latest_Payload);
   Water_Meter_High_Pressure_Alarm_instances[index].Latest_Payload = lwm2m_malloc(length);
   if (Water_Meter_High_Pressure_Alarm_instances[index].Latest_Payload == 0)
   {
     Water_Meter_High_Pressure_Alarm_instances[index].Latest_Payload_length  = 0;   
     Water_Meter_High_Pressure_Alarm_unlock();
     return COAP_500_INTERNAL_SERVER_ERROR;                                     
   } 
   Water_Meter_High_Pressure_Alarm_instances[index].Latest_Payload_length  = length;
   memcpy(Water_Meter_High_Pressure_Alarm_instances[index].Latest_Payload, val, length);                                      
                                  
   
  
   if (notify && Water_Meter_High_Pressure_Alarm_instances[index].registered) lwm2m_object_base_resource_changed(&Water_Meter_High_Pressure_Alarm_lwm2m_object, instance, 4014, false);
  
  
   Water_Meter_High_Pressure_Alarm_unlock();
   return COAP_204_CHANGED;
 }
#endif
