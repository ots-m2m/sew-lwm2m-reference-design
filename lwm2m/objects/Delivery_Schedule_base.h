/*
 * Delivery_Schedule_base.h
 * 
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * This code was generated using gt-object-gen provided by Operational 
 * Technology Solutions M2M Pty. Ltd. For licensing enquires visit 
 * ot-solutions.com.au.
 */

#ifndef LWM2M_Delivery_Schedule_OBJECT_BASE_H_
#define LWM2M_Delivery_Schedule_OBJECT_BASE_H_

#include "lwm2m_object_base.h"
#include "lwm2m_object_options.h"

#ifndef MAX_Delivery_Schedule_INSTANCES
#define MAX_Delivery_Schedule_INSTANCES 2
#endif

#if CONFIG_Delivery_Schedule_Schedule_UTC_Offset_OPTION == 1
#ifndef MAX_Delivery_Schedule_Schedule_UTC_Offset
#define MAX_Delivery_Schedule_Schedule_UTC_Offset 64
#endif
#endif

typedef enum Delivery_Schedule_resources_t
{
#if CONFIG_Delivery_Schedule_Schedule_Start_Time_OPTION == 1
  Delivery_Schedule_Schedule_Start_Time_id = 0, 
#endif
#if CONFIG_Delivery_Schedule_Schedule_UTC_Offset_OPTION == 1
  Delivery_Schedule_Schedule_UTC_Offset_id = 1, 
#endif
#if CONFIG_Delivery_Schedule_Delivery_Frequency_OPTION == 1
  Delivery_Schedule_Delivery_Frequency_id = 2, 
#endif
#if CONFIG_Delivery_Schedule_Randomised_Delivery_Window_OPTION == 1
  Delivery_Schedule_Randomised_Delivery_Window_id = 3, 
#endif
#if CONFIG_Delivery_Schedule_Number_of_Retries_OPTION == 1
  Delivery_Schedule_Number_of_Retries_id = 4, 
#endif
#if CONFIG_Delivery_Schedule_Retry_Period_OPTION == 1
  Delivery_Schedule_Retry_Period_id = 5, 
#endif
} 
Delivery_Schedule_resources_t;

void Delivery_Schedule_lock();
void Delivery_Schedule_unlock();
/*
 * This must be called during startup and before any other functions are called 
 * on this object. Returns a pointer to the internal lwm2m_object_t which
 * can then be passed to liblwm2m to register the object.
 */
lwm2m_object_t *Delivery_Schedule_initialise(void);

/*
 * Retrieves a pointer to the lwm2m_object structure for this object.
 */
lwm2m_object_t *Delivery_Schedule_get_lwm2m_object(void);

/*
 * The application calls this when it wants to create a new instance, returns
 * LWM2M_INVALID_INSTANCE if there are no free instances, otherwise the id of 
 * the new instance is returned. The application can define 
 * MAX_Delivery_Schedule_INSTANCES before including 
 * Delivery_Schedule_generated_object.h to increase the number of available 
 * instances.
 */
uint8_t Delivery_Schedule_create_instance(uint16_t instance);

/*
 * Delete the specified Delivery_Schedule instance. Returns COAP_404_NOT_FOUND if
 * the instance id is not found.
 */
__attribute__((weak)) uint8_t Delivery_Schedule_delete_instance(uint16_t instance);

/*
 * Use this function to iterate over each instance of this object. To halt
 * the iteration (e.g. when you have found what you are looking for) return
 * false from the each_instance_func.
 */
void Delivery_Schedule_foreach(lwm2m_each_instance_func func, void *data);

/*
 * provides access to a user data pointer for storing additional instance
 * data.
 */ 
void *Delivery_Schedule_get_user_data(uint16_t instance_id);
bool Delivery_Schedule_set_user_data(uint16_t instance_id, void *data);

/*
 * Set the lwm2m context on the object
 */
__attribute__((weak)) void Delivery_Schedule_register_object(lwm2m_context_t *context);

/*
 * Registers the instance with the lwm2m engine, must be called after create
 */ 
__attribute__((weak)) void Delivery_Schedule_register_instance(uint16_t instance);

/*
 * Unregisters the instance from the lwm2m engine, must be called after register
 */ 
__attribute__((weak)) void Delivery_Schedule_unregister_instance(uint16_t instance);


/*
 * Override this function to perform actions on the creation of an instance
 */
__attribute__((weak)) void Delivery_Schedule_instance_created(uint16_t instance);

/*
 * This function is called when the server has written a new value to ANY
 * writeable resource
 */
void Delivery_Schedule_changed(uint16_t instance);

/*
 * set/get/execute functions for each resource. The default implementation for
 * set and get simply writes to the instance variable for the resource. 
 * 
 * The _execute functions notifies the application that the server has requested 
 * an execute on the resource. The default implementation does nothing
 * 
 * The _changed function notifies the application when the server has written a 
 * resource. Note the global Delivery_Schedule_changed function is also called.
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
 * #define CONFIG_Delivery_Schedule_Schedule_Start_Time_OPTION 1
 * #define CONFIG_Delivery_Schedule_Schedule_UTC_Offset_OPTION 1
 * #define CONFIG_Delivery_Schedule_Delivery_Frequency_OPTION 1
 * #define CONFIG_Delivery_Schedule_Randomised_Delivery_Window_OPTION 1
 * #define CONFIG_Delivery_Schedule_Number_of_Retries_OPTION 1
 * #define CONFIG_Delivery_Schedule_Retry_Period_OPTION 1
 */

#if CONFIG_Delivery_Schedule_Schedule_Start_Time_OPTION == 1
uint8_t Delivery_Schedule_Schedule_Start_Time_get(uint16_t instance,  uint32_t *val);


#endif
#if CONFIG_Delivery_Schedule_Schedule_Start_Time_OPTION == 1
uint8_t Delivery_Schedule_Schedule_Start_Time_set(uint16_t instance,  uint32_t val, bool notify);
#endif
  
#if CONFIG_Delivery_Schedule_Schedule_Start_Time_OPTION == 1
uint8_t Delivery_Schedule_Schedule_Start_Time_changed(uint16_t instance);
#endif
  

#if CONFIG_Delivery_Schedule_Schedule_UTC_Offset_OPTION == 1
uint8_t Delivery_Schedule_Schedule_UTC_Offset_get(uint16_t instance,  const char **val);


#endif
#if CONFIG_Delivery_Schedule_Schedule_UTC_Offset_OPTION == 1
uint8_t Delivery_Schedule_Schedule_UTC_Offset_set(uint16_t instance,  const char *val, size_t length, bool notify);
#endif
  
#if CONFIG_Delivery_Schedule_Schedule_UTC_Offset_OPTION == 1
uint8_t Delivery_Schedule_Schedule_UTC_Offset_changed(uint16_t instance);
#endif
  

#if CONFIG_Delivery_Schedule_Delivery_Frequency_OPTION == 1
uint8_t Delivery_Schedule_Delivery_Frequency_get(uint16_t instance,  uint32_t *val);


#endif
#if CONFIG_Delivery_Schedule_Delivery_Frequency_OPTION == 1
uint8_t Delivery_Schedule_Delivery_Frequency_set(uint16_t instance,  uint32_t val, bool notify);
#endif
  
#if CONFIG_Delivery_Schedule_Delivery_Frequency_OPTION == 1
uint8_t Delivery_Schedule_Delivery_Frequency_changed(uint16_t instance);
#endif
  

#if CONFIG_Delivery_Schedule_Randomised_Delivery_Window_OPTION == 1
uint8_t Delivery_Schedule_Randomised_Delivery_Window_get(uint16_t instance,  uint32_t *val);


#endif
#if CONFIG_Delivery_Schedule_Randomised_Delivery_Window_OPTION == 1
uint8_t Delivery_Schedule_Randomised_Delivery_Window_set(uint16_t instance,  uint32_t val, bool notify);
#endif
  
#if CONFIG_Delivery_Schedule_Randomised_Delivery_Window_OPTION == 1
uint8_t Delivery_Schedule_Randomised_Delivery_Window_changed(uint16_t instance);
#endif
  

#if CONFIG_Delivery_Schedule_Number_of_Retries_OPTION == 1
uint8_t Delivery_Schedule_Number_of_Retries_get(uint16_t instance,  uint32_t *val);


#endif
#if CONFIG_Delivery_Schedule_Number_of_Retries_OPTION == 1
uint8_t Delivery_Schedule_Number_of_Retries_set(uint16_t instance,  uint32_t val, bool notify);
#endif
  
#if CONFIG_Delivery_Schedule_Number_of_Retries_OPTION == 1
uint8_t Delivery_Schedule_Number_of_Retries_changed(uint16_t instance);
#endif
  

#if CONFIG_Delivery_Schedule_Retry_Period_OPTION == 1
uint8_t Delivery_Schedule_Retry_Period_get(uint16_t instance,  uint32_t *val);


#endif
#if CONFIG_Delivery_Schedule_Retry_Period_OPTION == 1
uint8_t Delivery_Schedule_Retry_Period_set(uint16_t instance,  uint32_t val, bool notify);
#endif
  
#if CONFIG_Delivery_Schedule_Retry_Period_OPTION == 1
uint8_t Delivery_Schedule_Retry_Period_changed(uint16_t instance);
#endif
  

#endif
