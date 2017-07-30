/*
 * LWM2M_Server_base.h
 * 
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * This code was generated using gt-object-gen provided by Operational 
 * Technology Solutions M2M Pty. Ltd. For licensing enquires visit 
 * ot-solutions.com.au.
 */

#ifndef LWM2M_LWM2M_Server_OBJECT_BASE_H_
#define LWM2M_LWM2M_Server_OBJECT_BASE_H_

#include "lwm2m_object_base.h"
#include "lwm2m_object_options.h"

#ifndef MAX_LWM2M_Server_INSTANCES
#define MAX_LWM2M_Server_INSTANCES 2
#endif

#ifndef MAX_LWM2M_Server_Binding
#define MAX_LWM2M_Server_Binding 64
#endif

typedef enum LWM2M_Server_resources_t
{
  LWM2M_Server_Short_Server_ID_id = 0, 
  LWM2M_Server_Lifetime_id = 1, 
#if CONFIG_LWM2M_Server_Default_Minimum_Period_OPTION == 1
  LWM2M_Server_Default_Minimum_Period_id = 2, 
#endif
#if CONFIG_LWM2M_Server_Default_Maximum_Period_OPTION == 1
  LWM2M_Server_Default_Maximum_Period_id = 3, 
#endif
#if CONFIG_LWM2M_Server_Disable_Timeout_OPTION == 1
  LWM2M_Server_Disable_Timeout_id = 5, 
#endif
  LWM2M_Server_Notification_Storing_When_Disabled_or_Offline_id = 6, 
  LWM2M_Server_Binding_id = 7, 
} 
LWM2M_Server_resources_t;

void LWM2M_Server_lock();
void LWM2M_Server_unlock();
/*
 * This must be called during startup and before any other functions are called 
 * on this object. Returns a pointer to the internal lwm2m_object_t which
 * can then be passed to liblwm2m to register the object.
 */
lwm2m_object_t *LWM2M_Server_initialise(void);

/*
 * Retrieves a pointer to the lwm2m_object structure for this object.
 */
lwm2m_object_t *LWM2M_Server_get_lwm2m_object(void);

/*
 * The application calls this when it wants to create a new instance, returns
 * LWM2M_INVALID_INSTANCE if there are no free instances, otherwise the id of 
 * the new instance is returned. The application can define 
 * MAX_LWM2M_Server_INSTANCES before including 
 * LWM2M_Server_generated_object.h to increase the number of available 
 * instances.
 */
uint8_t LWM2M_Server_create_instance(uint16_t instance);

/*
 * Delete the specified LWM2M_Server instance. Returns COAP_404_NOT_FOUND if
 * the instance id is not found.
 */
__attribute__((weak)) uint8_t LWM2M_Server_delete_instance(uint16_t instance);

/*
 * Use this function to iterate over each instance of this object. To halt
 * the iteration (e.g. when you have found what you are looking for) return
 * false from the each_instance_func.
 */
void LWM2M_Server_foreach(lwm2m_each_instance_func func, void *data);

/*
 * provides access to a user data pointer for storing additional instance
 * data.
 */ 
void *LWM2M_Server_get_user_data(uint16_t instance_id);
bool LWM2M_Server_set_user_data(uint16_t instance_id, void *data);

/*
 * Set the lwm2m context on the object
 */
__attribute__((weak)) void LWM2M_Server_register_object(lwm2m_context_t *context);

/*
 * Registers the instance with the lwm2m engine, must be called after create
 */ 
__attribute__((weak)) void LWM2M_Server_register_instance(uint16_t instance);

/*
 * Unregisters the instance from the lwm2m engine, must be called after register
 */ 
__attribute__((weak)) void LWM2M_Server_unregister_instance(uint16_t instance);


/*
 * Override this function to perform actions on the creation of an instance
 */
__attribute__((weak)) void LWM2M_Server_instance_created(uint16_t instance);

/*
 * This function is called when the server has written a new value to ANY
 * writeable resource
 */
void LWM2M_Server_changed(uint16_t instance);

/*
 * set/get/execute functions for each resource. The default implementation for
 * set and get simply writes to the instance variable for the resource. 
 * 
 * The _execute functions notifies the application that the server has requested 
 * an execute on the resource. The default implementation does nothing
 * 
 * The _changed function notifies the application when the server has written a 
 * resource. Note the global LWM2M_Server_changed function is also called.
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
 * #define CONFIG_LWM2M_Server_Default_Minimum_Period_OPTION 1
 * #define CONFIG_LWM2M_Server_Default_Maximum_Period_OPTION 1
 * #define CONFIG_LWM2M_Server_Disable_OPTION 1
 * #define CONFIG_LWM2M_Server_Disable_Timeout_OPTION 1
 */
uint8_t LWM2M_Server_Short_Server_ID_get(uint16_t instance,  uint32_t *val);
uint8_t LWM2M_Server_Short_Server_ID_set(uint16_t instance,  uint32_t val, bool notify);
uint8_t LWM2M_Server_Short_Server_ID_changed(uint16_t instance);
uint8_t LWM2M_Server_Lifetime_get(uint16_t instance,  uint32_t *val);
uint8_t LWM2M_Server_Lifetime_set(uint16_t instance,  uint32_t val, bool notify);
uint8_t LWM2M_Server_Lifetime_changed(uint16_t instance);

#if CONFIG_LWM2M_Server_Default_Minimum_Period_OPTION == 1
uint8_t LWM2M_Server_Default_Minimum_Period_get(uint16_t instance,  uint32_t *val);


#endif
#if CONFIG_LWM2M_Server_Default_Minimum_Period_OPTION == 1
uint8_t LWM2M_Server_Default_Minimum_Period_set(uint16_t instance,  uint32_t val, bool notify);
#endif
  
#if CONFIG_LWM2M_Server_Default_Minimum_Period_OPTION == 1
uint8_t LWM2M_Server_Default_Minimum_Period_changed(uint16_t instance);
#endif
  

#if CONFIG_LWM2M_Server_Default_Maximum_Period_OPTION == 1
uint8_t LWM2M_Server_Default_Maximum_Period_get(uint16_t instance,  uint32_t *val);


#endif
#if CONFIG_LWM2M_Server_Default_Maximum_Period_OPTION == 1
uint8_t LWM2M_Server_Default_Maximum_Period_set(uint16_t instance,  uint32_t val, bool notify);
#endif
  
#if CONFIG_LWM2M_Server_Default_Maximum_Period_OPTION == 1
uint8_t LWM2M_Server_Default_Maximum_Period_changed(uint16_t instance);
#endif
  
  
#if CONFIG_LWM2M_Server_Disable_OPTION == 1
uint8_t LWM2M_Server_Disable_execute(lwm2m_object_t *object, uint16_t instance, uint16_t resource_id, uint8_t * buffer, int length);
 
#endif

#if CONFIG_LWM2M_Server_Disable_Timeout_OPTION == 1
uint8_t LWM2M_Server_Disable_Timeout_get(uint16_t instance,  uint32_t *val);


#endif
#if CONFIG_LWM2M_Server_Disable_Timeout_OPTION == 1
uint8_t LWM2M_Server_Disable_Timeout_set(uint16_t instance,  uint32_t val, bool notify);
#endif
  
#if CONFIG_LWM2M_Server_Disable_Timeout_OPTION == 1
uint8_t LWM2M_Server_Disable_Timeout_changed(uint16_t instance);
#endif
  
uint8_t LWM2M_Server_Notification_Storing_When_Disabled_or_Offline_get(uint16_t instance,  bool *val);
uint8_t LWM2M_Server_Notification_Storing_When_Disabled_or_Offline_set(uint16_t instance,  bool val, bool notify);
uint8_t LWM2M_Server_Notification_Storing_When_Disabled_or_Offline_changed(uint16_t instance);
uint8_t LWM2M_Server_Binding_get(uint16_t instance,  const char **val);
uint8_t LWM2M_Server_Binding_set(uint16_t instance,  const char *val, size_t length, bool notify);
uint8_t LWM2M_Server_Binding_changed(uint16_t instance);
uint8_t LWM2M_Server_Registration_Update_Trigger_execute(lwm2m_object_t *object, uint16_t instance, uint16_t resource_id, uint8_t * buffer, int length);

#endif
