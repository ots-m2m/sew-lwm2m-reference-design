/*
 * Device_base.h
 * 
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * This code was generated using gt-object-gen provided by Operational 
 * Technology Solutions M2M Pty. Ltd. For licensing enquires visit 
 * ot-solutions.com.au.
 */

#ifndef LWM2M_Device_OBJECT_BASE_H_
#define LWM2M_Device_OBJECT_BASE_H_

#include "lwm2m_object_base.h"
#include "lwm2m_object_options.h"

#define MAX_Device_INSTANCES 1

#if CONFIG_Device_Manufacturer_OPTION == 1
#ifndef MAX_Device_Manufacturer
#define MAX_Device_Manufacturer 64
#endif
#endif
#if CONFIG_Device_Model_Number_OPTION == 1
#ifndef MAX_Device_Model_Number
#define MAX_Device_Model_Number 64
#endif
#endif
#if CONFIG_Device_Serial_Number_OPTION == 1
#ifndef MAX_Device_Serial_Number
#define MAX_Device_Serial_Number 64
#endif
#endif
#if CONFIG_Device_Firmware_Version_OPTION == 1
#ifndef MAX_Device_Firmware_Version
#define MAX_Device_Firmware_Version 64
#endif
#endif
#if CONFIG_Device_UTC_Offset_OPTION == 1
#ifndef MAX_Device_UTC_Offset
#define MAX_Device_UTC_Offset 64
#endif
#endif
#if CONFIG_Device_Timezone_OPTION == 1
#ifndef MAX_Device_Timezone
#define MAX_Device_Timezone 64
#endif
#endif
#ifndef MAX_Device_Supported_Binding_and_Modes
#define MAX_Device_Supported_Binding_and_Modes 64
#endif

typedef enum Device_resources_t
{
#if CONFIG_Device_Manufacturer_OPTION == 1
  Device_Manufacturer_id = 0, 
#endif
#if CONFIG_Device_Model_Number_OPTION == 1
  Device_Model_Number_id = 1, 
#endif
#if CONFIG_Device_Serial_Number_OPTION == 1
  Device_Serial_Number_id = 2, 
#endif
#if CONFIG_Device_Firmware_Version_OPTION == 1
  Device_Firmware_Version_id = 3, 
#endif
#if CONFIG_Device_Available_Power_Sources_OPTION == 1
  Device_Available_Power_Sources_id = 6, 
#endif
#if CONFIG_Device_Power_Source_Voltage_OPTION == 1
  Device_Power_Source_Voltage_id = 7, 
#endif
#if CONFIG_Device_Power_Source_Current_OPTION == 1
  Device_Power_Source_Current_id = 8, 
#endif
#if CONFIG_Device_Battery_Level_OPTION == 1
  Device_Battery_Level_id = 9, 
#endif
#if CONFIG_Device_Memory_Free_OPTION == 1
  Device_Memory_Free_id = 10, 
#endif
  Device_Error_Code_id = 11, 
#if CONFIG_Device_Current_Time_OPTION == 1
  Device_Current_Time_id = 13, 
#endif
#if CONFIG_Device_UTC_Offset_OPTION == 1
  Device_UTC_Offset_id = 14, 
#endif
#if CONFIG_Device_Timezone_OPTION == 1
  Device_Timezone_id = 15, 
#endif
  Device_Supported_Binding_and_Modes_id = 16, 
} 
Device_resources_t;

void Device_lock();
void Device_unlock();
/*
 * This must be called during startup and before any other functions are called 
 * on this object. Returns a pointer to the internal lwm2m_object_t which
 * can then be passed to liblwm2m to register the object.
 */
lwm2m_object_t *Device_initialise(void);

/*
 * Retrieves a pointer to the lwm2m_object structure for this object.
 */
lwm2m_object_t *Device_get_lwm2m_object(void);

/*
 * The application calls this when it wants to create a new instance, returns
 * LWM2M_INVALID_INSTANCE if there are no free instances, otherwise the id of 
 * the new instance is returned. The application can define 
 * MAX_Device_INSTANCES before including 
 * Device_generated_object.h to increase the number of available 
 * instances.
 */
uint8_t Device_create_instance(uint16_t instance);

/*
 * Delete the specified Device instance. Returns COAP_404_NOT_FOUND if
 * the instance id is not found.
 */
__attribute__((weak)) uint8_t Device_delete_instance(uint16_t instance);

/*
 * Use this function to iterate over each instance of this object. To halt
 * the iteration (e.g. when you have found what you are looking for) return
 * false from the each_instance_func.
 */
void Device_foreach(lwm2m_each_instance_func func, void *data);

/*
 * provides access to a user data pointer for storing additional instance
 * data.
 */ 
void *Device_get_user_data(uint16_t instance_id);
bool Device_set_user_data(uint16_t instance_id, void *data);

/*
 * Set the lwm2m context on the object
 */
__attribute__((weak)) void Device_register_object(lwm2m_context_t *context);

/*
 * Registers the instance with the lwm2m engine, must be called after create
 */ 
__attribute__((weak)) void Device_register_instance(uint16_t instance);

/*
 * Unregisters the instance from the lwm2m engine, must be called after register
 */ 
__attribute__((weak)) void Device_unregister_instance(uint16_t instance);


/*
 * Override this function to perform actions on the creation of an instance
 */
__attribute__((weak)) void Device_instance_created(uint16_t instance);

/*
 * This function is called when the server has written a new value to ANY
 * writeable resource
 */
void Device_changed(uint16_t instance);

/*
 * set/get/execute functions for each resource. The default implementation for
 * set and get simply writes to the instance variable for the resource. 
 * 
 * The _execute functions notifies the application that the server has requested 
 * an execute on the resource. The default implementation does nothing
 * 
 * The _changed function notifies the application when the server has written a 
 * resource. Note the global Device_changed function is also called.
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
 * #define CONFIG_Device_Manufacturer_OPTION 1
 * #define CONFIG_Device_Model_Number_OPTION 1
 * #define CONFIG_Device_Serial_Number_OPTION 1
 * #define CONFIG_Device_Firmware_Version_OPTION 1
 * #define CONFIG_Device_Factory_Reset_OPTION 1
 * #define CONFIG_Device_Available_Power_Sources_OPTION 1
 * #define CONFIG_Device_Power_Source_Voltage_OPTION 1
 * #define CONFIG_Device_Power_Source_Current_OPTION 1
 * #define CONFIG_Device_Battery_Level_OPTION 1
 * #define CONFIG_Device_Memory_Free_OPTION 1
 * #define CONFIG_Device_Reset_Error_Code_OPTION 1
 * #define CONFIG_Device_Current_Time_OPTION 1
 * #define CONFIG_Device_UTC_Offset_OPTION 1
 * #define CONFIG_Device_Timezone_OPTION 1
 */

#if CONFIG_Device_Manufacturer_OPTION == 1
uint8_t Device_Manufacturer_get(uint16_t instance,  const char **val);


#endif
#if CONFIG_Device_Manufacturer_OPTION == 1
uint8_t Device_Manufacturer_set(uint16_t instance,  const char *val, size_t length, bool notify);
#endif

#if CONFIG_Device_Model_Number_OPTION == 1
uint8_t Device_Model_Number_get(uint16_t instance,  const char **val);


#endif
#if CONFIG_Device_Model_Number_OPTION == 1
uint8_t Device_Model_Number_set(uint16_t instance,  const char *val, size_t length, bool notify);
#endif

#if CONFIG_Device_Serial_Number_OPTION == 1
uint8_t Device_Serial_Number_get(uint16_t instance,  const char **val);


#endif
#if CONFIG_Device_Serial_Number_OPTION == 1
uint8_t Device_Serial_Number_set(uint16_t instance,  const char *val, size_t length, bool notify);
#endif

#if CONFIG_Device_Firmware_Version_OPTION == 1
uint8_t Device_Firmware_Version_get(uint16_t instance,  const char **val);


#endif
#if CONFIG_Device_Firmware_Version_OPTION == 1
uint8_t Device_Firmware_Version_set(uint16_t instance,  const char *val, size_t length, bool notify);
#endif
uint8_t Device_Reboot_execute(lwm2m_object_t *object, uint16_t instance, uint16_t resource_id, uint8_t * buffer, int length);
  
#if CONFIG_Device_Factory_Reset_OPTION == 1
uint8_t Device_Factory_Reset_execute(lwm2m_object_t *object, uint16_t instance, uint16_t resource_id, uint8_t * buffer, int length);
 
#endif
bool Device_Available_Power_Sources_is_set(uint16_t instance, uint16_t resource_instance);                                                                    

#if CONFIG_Device_Available_Power_Sources_OPTION == 1
uint8_t Device_Available_Power_Sources_get(uint16_t instance, uint16_t resource_instance, uint32_t *val);


#endif
#if CONFIG_Device_Available_Power_Sources_OPTION == 1
uint8_t Device_Available_Power_Sources_set(uint16_t instance, uint16_t resource_instance, uint32_t val, bool notify);
#endif
bool Device_Power_Source_Voltage_is_set(uint16_t instance, uint16_t resource_instance);                                                                    

#if CONFIG_Device_Power_Source_Voltage_OPTION == 1
uint8_t Device_Power_Source_Voltage_get(uint16_t instance, uint16_t resource_instance, uint32_t *val);


#endif
#if CONFIG_Device_Power_Source_Voltage_OPTION == 1
uint8_t Device_Power_Source_Voltage_set(uint16_t instance, uint16_t resource_instance, uint32_t val, bool notify);
#endif
bool Device_Power_Source_Current_is_set(uint16_t instance, uint16_t resource_instance);                                                                    

#if CONFIG_Device_Power_Source_Current_OPTION == 1
uint8_t Device_Power_Source_Current_get(uint16_t instance, uint16_t resource_instance, uint32_t *val);


#endif
#if CONFIG_Device_Power_Source_Current_OPTION == 1
uint8_t Device_Power_Source_Current_set(uint16_t instance, uint16_t resource_instance, uint32_t val, bool notify);
#endif

#if CONFIG_Device_Battery_Level_OPTION == 1
uint8_t Device_Battery_Level_get(uint16_t instance,  uint32_t *val);


#endif
#if CONFIG_Device_Battery_Level_OPTION == 1
uint8_t Device_Battery_Level_set(uint16_t instance,  uint32_t val, bool notify);
#endif

#if CONFIG_Device_Memory_Free_OPTION == 1
uint8_t Device_Memory_Free_get(uint16_t instance,  uint32_t *val);


#endif
#if CONFIG_Device_Memory_Free_OPTION == 1
uint8_t Device_Memory_Free_set(uint16_t instance,  uint32_t val, bool notify);
#endif
bool Device_Error_Code_is_set(uint16_t instance, uint16_t resource_instance);                                                                    
uint8_t Device_Error_Code_get(uint16_t instance, uint16_t resource_instance, uint32_t *val);
uint8_t Device_Error_Code_set(uint16_t instance, uint16_t resource_instance, uint32_t val, bool notify);
  
#if CONFIG_Device_Reset_Error_Code_OPTION == 1
uint8_t Device_Reset_Error_Code_execute(lwm2m_object_t *object, uint16_t instance, uint16_t resource_id, uint8_t * buffer, int length);
 
#endif

#if CONFIG_Device_Current_Time_OPTION == 1
uint8_t Device_Current_Time_get(uint16_t instance,  time_t *val);


#endif
#if CONFIG_Device_Current_Time_OPTION == 1
uint8_t Device_Current_Time_set(uint16_t instance,  time_t val, bool notify);
#endif
  
#if CONFIG_Device_Current_Time_OPTION == 1
uint8_t Device_Current_Time_changed(uint16_t instance);
#endif
  

#if CONFIG_Device_UTC_Offset_OPTION == 1
uint8_t Device_UTC_Offset_get(uint16_t instance,  const char **val);


#endif
#if CONFIG_Device_UTC_Offset_OPTION == 1
uint8_t Device_UTC_Offset_set(uint16_t instance,  const char *val, size_t length, bool notify);
#endif
  
#if CONFIG_Device_UTC_Offset_OPTION == 1
uint8_t Device_UTC_Offset_changed(uint16_t instance);
#endif
  

#if CONFIG_Device_Timezone_OPTION == 1
uint8_t Device_Timezone_get(uint16_t instance,  const char **val);


#endif
#if CONFIG_Device_Timezone_OPTION == 1
uint8_t Device_Timezone_set(uint16_t instance,  const char *val, size_t length, bool notify);
#endif
  
#if CONFIG_Device_Timezone_OPTION == 1
uint8_t Device_Timezone_changed(uint16_t instance);
#endif
  
uint8_t Device_Supported_Binding_and_Modes_get(uint16_t instance,  const char **val);
uint8_t Device_Supported_Binding_and_Modes_set(uint16_t instance,  const char *val, size_t length, bool notify);

#endif
