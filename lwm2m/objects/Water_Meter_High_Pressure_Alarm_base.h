/*
 * Water_Meter_High_Pressure_Alarm_base.h
 * 
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * This code was generated using gt-object-gen provided by Operational 
 * Technology Solutions M2M Pty. Ltd. For licensing enquires visit 
 * ot-solutions.com.au.
 */

#ifndef LWM2M_Water_Meter_High_Pressure_Alarm_OBJECT_BASE_H_
#define LWM2M_Water_Meter_High_Pressure_Alarm_OBJECT_BASE_H_

#include "lwm2m_object_base.h"
#include "lwm2m_object_options.h"

#ifndef MAX_Water_Meter_High_Pressure_Alarm_INSTANCES
#define MAX_Water_Meter_High_Pressure_Alarm_INSTANCES 2
#endif


typedef enum Water_Meter_High_Pressure_Alarm_resources_t
{
#if CONFIG_Water_Meter_High_Pressure_Alarm_Event_Type_OPTION == 1
  Water_Meter_High_Pressure_Alarm_Event_Type_id = 9020, 
#endif
#if CONFIG_Water_Meter_High_Pressure_Alarm_Alarm_Realtime_OPTION == 1
  Water_Meter_High_Pressure_Alarm_Alarm_Realtime_id = 9021, 
#endif
#if CONFIG_Water_Meter_High_Pressure_Alarm_Alarm_State_OPTION == 1
  Water_Meter_High_Pressure_Alarm_Alarm_State_id = 9022, 
#endif
#if CONFIG_Water_Meter_High_Pressure_Alarm_Alarm_Set_Threshold_OPTION == 1
  Water_Meter_High_Pressure_Alarm_Alarm_Set_Threshold_id = 9023, 
#endif
#if CONFIG_Water_Meter_High_Pressure_Alarm_Alarm_Set_Operator_OPTION == 1
  Water_Meter_High_Pressure_Alarm_Alarm_Set_Operator_id = 9024, 
#endif
#if CONFIG_Water_Meter_High_Pressure_Alarm_Alarm_Clear_Threshold_OPTION == 1
  Water_Meter_High_Pressure_Alarm_Alarm_Clear_Threshold_id = 9025, 
#endif
#if CONFIG_Water_Meter_High_Pressure_Alarm_Alarm_Clear_Operator_OPTION == 1
  Water_Meter_High_Pressure_Alarm_Alarm_Clear_Operator_id = 9026, 
#endif
#if CONFIG_Water_Meter_High_Pressure_Alarm_Alarm_Maximum_Event_Count_OPTION == 1
  Water_Meter_High_Pressure_Alarm_Alarm_Maximum_Event_Count_id = 9027, 
#endif
#if CONFIG_Water_Meter_High_Pressure_Alarm_Alarm_Maximum_Event_Period_OPTION == 1
  Water_Meter_High_Pressure_Alarm_Alarm_Maximum_Event_Period_id = 9028, 
#endif
#if CONFIG_Water_Meter_High_Pressure_Alarm_Latest_Delivered_Event_Time_OPTION == 1
  Water_Meter_High_Pressure_Alarm_Latest_Delivered_Event_Time_id = 9029, 
#endif
#if CONFIG_Water_Meter_High_Pressure_Alarm_Latest_Recorded_Event_Time_OPTION == 1
  Water_Meter_High_Pressure_Alarm_Latest_Recorded_Event_Time_id = 9030, 
#endif
#if CONFIG_Water_Meter_High_Pressure_Alarm_Alarm_Auto_Clear_OPTION == 1
  Water_Meter_High_Pressure_Alarm_Alarm_Auto_Clear_id = 9032, 
#endif
#if CONFIG_Water_Meter_High_Pressure_Alarm_Event_Code_OPTION == 1
  Water_Meter_High_Pressure_Alarm_Event_Code_id = 4010, 
#endif
#if CONFIG_Water_Meter_High_Pressure_Alarm_Latest_Payload_OPTION == 1
  Water_Meter_High_Pressure_Alarm_Latest_Payload_id = 4014, 
#endif
} 
Water_Meter_High_Pressure_Alarm_resources_t;

void Water_Meter_High_Pressure_Alarm_lock();
void Water_Meter_High_Pressure_Alarm_unlock();
/*
 * This must be called during startup and before any other functions are called 
 * on this object. Returns a pointer to the internal lwm2m_object_t which
 * can then be passed to liblwm2m to register the object.
 */
lwm2m_object_t *Water_Meter_High_Pressure_Alarm_initialise(void);

/*
 * Retrieves a pointer to the lwm2m_object structure for this object.
 */
lwm2m_object_t *Water_Meter_High_Pressure_Alarm_get_lwm2m_object(void);

/*
 * The application calls this when it wants to create a new instance, returns
 * LWM2M_INVALID_INSTANCE if there are no free instances, otherwise the id of 
 * the new instance is returned. The application can define 
 * MAX_Water_Meter_High_Pressure_Alarm_INSTANCES before including 
 * Water_Meter_High_Pressure_Alarm_generated_object.h to increase the number of available 
 * instances.
 */
uint8_t Water_Meter_High_Pressure_Alarm_create_instance(uint16_t instance);

/*
 * Delete the specified Water_Meter_High_Pressure_Alarm instance. Returns COAP_404_NOT_FOUND if
 * the instance id is not found.
 */
__attribute__((weak)) uint8_t Water_Meter_High_Pressure_Alarm_delete_instance(uint16_t instance);

/*
 * Use this function to iterate over each instance of this object. To halt
 * the iteration (e.g. when you have found what you are looking for) return
 * false from the each_instance_func.
 */
void Water_Meter_High_Pressure_Alarm_foreach(lwm2m_each_instance_func func, void *data);

/*
 * provides access to a user data pointer for storing additional instance
 * data.
 */ 
void *Water_Meter_High_Pressure_Alarm_get_user_data(uint16_t instance_id);
bool Water_Meter_High_Pressure_Alarm_set_user_data(uint16_t instance_id, void *data);

/*
 * Set the lwm2m context on the object
 */
__attribute__((weak)) void Water_Meter_High_Pressure_Alarm_register_object(lwm2m_context_t *context);

/*
 * Registers the instance with the lwm2m engine, must be called after create
 */ 
__attribute__((weak)) void Water_Meter_High_Pressure_Alarm_register_instance(uint16_t instance);

/*
 * Unregisters the instance from the lwm2m engine, must be called after register
 */ 
__attribute__((weak)) void Water_Meter_High_Pressure_Alarm_unregister_instance(uint16_t instance);


/*
 * Override this function to perform actions on the creation of an instance
 */
__attribute__((weak)) void Water_Meter_High_Pressure_Alarm_instance_created(uint16_t instance);

/*
 * This function is called when the server has written a new value to ANY
 * writeable resource
 */
void Water_Meter_High_Pressure_Alarm_changed(uint16_t instance);

/*
 * set/get/execute functions for each resource. The default implementation for
 * set and get simply writes to the instance variable for the resource. 
 * 
 * The _execute functions notifies the application that the server has requested 
 * an execute on the resource. The default implementation does nothing
 * 
 * The _changed function notifies the application when the server has written a 
 * resource. Note the global Water_Meter_High_Pressure_Alarm_changed function is also called.
 * 
 * The _read and _write functions are called when opaque resources are read or
 * written from the server respectively. The default does nothing, you will
 * likely need to override these.
 * 
 * These functions are declared __weak to allow the application to override 
 * them should additional logic be needed. 
 * 
 * To include optional resources you need to set the associated configuration definition
 * to 1. The following optional resource are avialable for this object. Note
 * these are disabled by default.
 * 
 * #define CONFIG_Water_Meter_High_Pressure_Alarm_Event_Type_OPTION 1
 * #define CONFIG_Water_Meter_High_Pressure_Alarm_Alarm_Realtime_OPTION 1
 * #define CONFIG_Water_Meter_High_Pressure_Alarm_Alarm_State_OPTION 1
 * #define CONFIG_Water_Meter_High_Pressure_Alarm_Alarm_Set_Threshold_OPTION 1
 * #define CONFIG_Water_Meter_High_Pressure_Alarm_Alarm_Set_Operator_OPTION 1
 * #define CONFIG_Water_Meter_High_Pressure_Alarm_Alarm_Clear_Threshold_OPTION 1
 * #define CONFIG_Water_Meter_High_Pressure_Alarm_Alarm_Clear_Operator_OPTION 1
 * #define CONFIG_Water_Meter_High_Pressure_Alarm_Alarm_Maximum_Event_Count_OPTION 1
 * #define CONFIG_Water_Meter_High_Pressure_Alarm_Alarm_Maximum_Event_Period_OPTION 1
 * #define CONFIG_Water_Meter_High_Pressure_Alarm_Latest_Delivered_Event_Time_OPTION 1
 * #define CONFIG_Water_Meter_High_Pressure_Alarm_Latest_Recorded_Event_Time_OPTION 1
 * #define CONFIG_Water_Meter_High_Pressure_Alarm_Alarm_Clear_OPTION 1
 * #define CONFIG_Water_Meter_High_Pressure_Alarm_Alarm_Auto_Clear_OPTION 1
 * #define CONFIG_Water_Meter_High_Pressure_Alarm_Event_Code_OPTION 1
 * #define CONFIG_Water_Meter_High_Pressure_Alarm_Latest_Payload_OPTION 1
 */

#if CONFIG_Water_Meter_High_Pressure_Alarm_Event_Type_OPTION == 1
uint8_t Water_Meter_High_Pressure_Alarm_Event_Type_get(uint16_t instance,  uint32_t *val);


#endif
#if CONFIG_Water_Meter_High_Pressure_Alarm_Event_Type_OPTION == 1
uint8_t Water_Meter_High_Pressure_Alarm_Event_Type_set(uint16_t instance,  uint32_t val, bool notify);
#endif
  
#if CONFIG_Water_Meter_High_Pressure_Alarm_Event_Type_OPTION == 1
uint8_t Water_Meter_High_Pressure_Alarm_Event_Type_changed(uint16_t instance);
#endif
  

#if CONFIG_Water_Meter_High_Pressure_Alarm_Alarm_Realtime_OPTION == 1
uint8_t Water_Meter_High_Pressure_Alarm_Alarm_Realtime_get(uint16_t instance,  bool *val);


#endif
#if CONFIG_Water_Meter_High_Pressure_Alarm_Alarm_Realtime_OPTION == 1
uint8_t Water_Meter_High_Pressure_Alarm_Alarm_Realtime_set(uint16_t instance,  bool val, bool notify);
#endif
  
#if CONFIG_Water_Meter_High_Pressure_Alarm_Alarm_Realtime_OPTION == 1
uint8_t Water_Meter_High_Pressure_Alarm_Alarm_Realtime_changed(uint16_t instance);
#endif
  

#if CONFIG_Water_Meter_High_Pressure_Alarm_Alarm_State_OPTION == 1
uint8_t Water_Meter_High_Pressure_Alarm_Alarm_State_get(uint16_t instance,  bool *val);


#endif
#if CONFIG_Water_Meter_High_Pressure_Alarm_Alarm_State_OPTION == 1
uint8_t Water_Meter_High_Pressure_Alarm_Alarm_State_set(uint16_t instance,  bool val, bool notify);
#endif

#if CONFIG_Water_Meter_High_Pressure_Alarm_Alarm_Set_Threshold_OPTION == 1
uint8_t Water_Meter_High_Pressure_Alarm_Alarm_Set_Threshold_get(uint16_t instance,  double* val);


#endif
#if CONFIG_Water_Meter_High_Pressure_Alarm_Alarm_Set_Threshold_OPTION == 1
uint8_t Water_Meter_High_Pressure_Alarm_Alarm_Set_Threshold_set(uint16_t instance,  double val, bool notify);
#endif
  
#if CONFIG_Water_Meter_High_Pressure_Alarm_Alarm_Set_Threshold_OPTION == 1
uint8_t Water_Meter_High_Pressure_Alarm_Alarm_Set_Threshold_changed(uint16_t instance);
#endif
  

#if CONFIG_Water_Meter_High_Pressure_Alarm_Alarm_Set_Operator_OPTION == 1
uint8_t Water_Meter_High_Pressure_Alarm_Alarm_Set_Operator_get(uint16_t instance,  uint32_t *val);


#endif
#if CONFIG_Water_Meter_High_Pressure_Alarm_Alarm_Set_Operator_OPTION == 1
uint8_t Water_Meter_High_Pressure_Alarm_Alarm_Set_Operator_set(uint16_t instance,  uint32_t val, bool notify);
#endif
  
#if CONFIG_Water_Meter_High_Pressure_Alarm_Alarm_Set_Operator_OPTION == 1
uint8_t Water_Meter_High_Pressure_Alarm_Alarm_Set_Operator_changed(uint16_t instance);
#endif
  

#if CONFIG_Water_Meter_High_Pressure_Alarm_Alarm_Clear_Threshold_OPTION == 1
uint8_t Water_Meter_High_Pressure_Alarm_Alarm_Clear_Threshold_get(uint16_t instance,  double* val);


#endif
#if CONFIG_Water_Meter_High_Pressure_Alarm_Alarm_Clear_Threshold_OPTION == 1
uint8_t Water_Meter_High_Pressure_Alarm_Alarm_Clear_Threshold_set(uint16_t instance,  double val, bool notify);
#endif
  
#if CONFIG_Water_Meter_High_Pressure_Alarm_Alarm_Clear_Threshold_OPTION == 1
uint8_t Water_Meter_High_Pressure_Alarm_Alarm_Clear_Threshold_changed(uint16_t instance);
#endif
  

#if CONFIG_Water_Meter_High_Pressure_Alarm_Alarm_Clear_Operator_OPTION == 1
uint8_t Water_Meter_High_Pressure_Alarm_Alarm_Clear_Operator_get(uint16_t instance,  uint32_t *val);


#endif
#if CONFIG_Water_Meter_High_Pressure_Alarm_Alarm_Clear_Operator_OPTION == 1
uint8_t Water_Meter_High_Pressure_Alarm_Alarm_Clear_Operator_set(uint16_t instance,  uint32_t val, bool notify);
#endif
  
#if CONFIG_Water_Meter_High_Pressure_Alarm_Alarm_Clear_Operator_OPTION == 1
uint8_t Water_Meter_High_Pressure_Alarm_Alarm_Clear_Operator_changed(uint16_t instance);
#endif
  

#if CONFIG_Water_Meter_High_Pressure_Alarm_Alarm_Maximum_Event_Count_OPTION == 1
uint8_t Water_Meter_High_Pressure_Alarm_Alarm_Maximum_Event_Count_get(uint16_t instance,  uint32_t *val);


#endif
#if CONFIG_Water_Meter_High_Pressure_Alarm_Alarm_Maximum_Event_Count_OPTION == 1
uint8_t Water_Meter_High_Pressure_Alarm_Alarm_Maximum_Event_Count_set(uint16_t instance,  uint32_t val, bool notify);
#endif
  
#if CONFIG_Water_Meter_High_Pressure_Alarm_Alarm_Maximum_Event_Count_OPTION == 1
uint8_t Water_Meter_High_Pressure_Alarm_Alarm_Maximum_Event_Count_changed(uint16_t instance);
#endif
  

#if CONFIG_Water_Meter_High_Pressure_Alarm_Alarm_Maximum_Event_Period_OPTION == 1
uint8_t Water_Meter_High_Pressure_Alarm_Alarm_Maximum_Event_Period_get(uint16_t instance,  uint32_t *val);


#endif
#if CONFIG_Water_Meter_High_Pressure_Alarm_Alarm_Maximum_Event_Period_OPTION == 1
uint8_t Water_Meter_High_Pressure_Alarm_Alarm_Maximum_Event_Period_set(uint16_t instance,  uint32_t val, bool notify);
#endif
  
#if CONFIG_Water_Meter_High_Pressure_Alarm_Alarm_Maximum_Event_Period_OPTION == 1
uint8_t Water_Meter_High_Pressure_Alarm_Alarm_Maximum_Event_Period_changed(uint16_t instance);
#endif
  

#if CONFIG_Water_Meter_High_Pressure_Alarm_Latest_Delivered_Event_Time_OPTION == 1
uint8_t Water_Meter_High_Pressure_Alarm_Latest_Delivered_Event_Time_get(uint16_t instance,  time_t *val);


#endif
#if CONFIG_Water_Meter_High_Pressure_Alarm_Latest_Delivered_Event_Time_OPTION == 1
uint8_t Water_Meter_High_Pressure_Alarm_Latest_Delivered_Event_Time_set(uint16_t instance,  time_t val, bool notify);
#endif
  
#if CONFIG_Water_Meter_High_Pressure_Alarm_Latest_Delivered_Event_Time_OPTION == 1
uint8_t Water_Meter_High_Pressure_Alarm_Latest_Delivered_Event_Time_changed(uint16_t instance);
#endif
  

#if CONFIG_Water_Meter_High_Pressure_Alarm_Latest_Recorded_Event_Time_OPTION == 1
uint8_t Water_Meter_High_Pressure_Alarm_Latest_Recorded_Event_Time_get(uint16_t instance,  time_t *val);


#endif
#if CONFIG_Water_Meter_High_Pressure_Alarm_Latest_Recorded_Event_Time_OPTION == 1
uint8_t Water_Meter_High_Pressure_Alarm_Latest_Recorded_Event_Time_set(uint16_t instance,  time_t val, bool notify);
#endif
  
#if CONFIG_Water_Meter_High_Pressure_Alarm_Alarm_Clear_OPTION == 1
uint8_t Water_Meter_High_Pressure_Alarm_Alarm_Clear_execute(lwm2m_object_t *object, uint16_t instance, uint16_t resource_id, uint8_t * buffer, int length);
 
#endif

#if CONFIG_Water_Meter_High_Pressure_Alarm_Alarm_Auto_Clear_OPTION == 1
uint8_t Water_Meter_High_Pressure_Alarm_Alarm_Auto_Clear_get(uint16_t instance,  bool *val);


#endif
#if CONFIG_Water_Meter_High_Pressure_Alarm_Alarm_Auto_Clear_OPTION == 1
uint8_t Water_Meter_High_Pressure_Alarm_Alarm_Auto_Clear_set(uint16_t instance,  bool val, bool notify);
#endif
  
#if CONFIG_Water_Meter_High_Pressure_Alarm_Alarm_Auto_Clear_OPTION == 1
uint8_t Water_Meter_High_Pressure_Alarm_Alarm_Auto_Clear_changed(uint16_t instance);
#endif
  

#if CONFIG_Water_Meter_High_Pressure_Alarm_Event_Code_OPTION == 1
uint8_t Water_Meter_High_Pressure_Alarm_Event_Code_get(uint16_t instance,  uint32_t *val);


#endif
#if CONFIG_Water_Meter_High_Pressure_Alarm_Event_Code_OPTION == 1
uint8_t Water_Meter_High_Pressure_Alarm_Event_Code_set(uint16_t instance,  uint32_t val, bool notify);
#endif

#if CONFIG_Water_Meter_High_Pressure_Alarm_Latest_Payload_OPTION == 1
uint8_t Water_Meter_High_Pressure_Alarm_Latest_Payload_get(uint16_t instance,  uint8_t ** buffer, size_t *length);


#endif
#if CONFIG_Water_Meter_High_Pressure_Alarm_Latest_Payload_OPTION == 1
uint8_t Water_Meter_High_Pressure_Alarm_Latest_Payload_set(uint16_t instance,  uint8_t* val, size_t length, bool notify);
#endif

#endif
