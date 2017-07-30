/*
 * lwm2m_object_base.h
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 */

#ifndef LWM2M_OBJECT_BASE_H_
#define LWM2M_OBJECT_BASE_H_

#include "liblwm2m.h"
#include "mutex.h"
#include "enum_to_string.h"
#include <string.h>
#include <stdbool.h>

#include "lwm2m_object_options.h"

/**-----------------------------------------------------------------------------
 *  @file
 *  @defgroup    lwm2m_object_base
 *  @ingroup     lwm2m_client
 *  @{
 *------------------------------------------------------------------------------
 */

#define LWM2M_INVALID_INSTANCE 0xffff

#define operation_values(value)    \
  value(LWM2M_OPERATION_,R,     0) \
  value(LWM2M_OPERATION_,W,     1) \
  value(LWM2M_OPERATION_,RW,    2) \
  value(LWM2M_OPERATION_,E,     3) \
  value(LWM2M_OPERATION_,NONE,  4)

DECLARE_PROPERTY_ENUM(lwm2m_, operation, operation_values)

#define instancetype_values(value) \
  value(LWM2M_INSTANCETYPE_, single, 0) \
  value(LWM2M_INSTANCETYPE_, multiple, 1)

DECLARE_PROPERTY_ENUM(lwm2m_, instancetype, instancetype_values)

#define LWM2M_TYPE_TIME (LWM2M_TYPE_OBJECT_LINK + 1)

#define type_values(value) \
    value(LWM2M_TYPE_,ondefined        ,LWM2M_TYPE_UNDEFINED) \
    value(LWM2M_TYPE_,object           ,LWM2M_TYPE_OBJECT) \
    value(LWM2M_TYPE_,objectInstance   ,LWM2M_TYPE_OBJECT_INSTANCE) \
    value(LWM2M_TYPE_,multipleResource ,LWM2M_TYPE_MULTIPLE_RESOURCE) \
    value(LWM2M_TYPE_,string           ,LWM2M_TYPE_STRING) \
    value(LWM2M_TYPE_,opaque           ,LWM2M_TYPE_OPAQUE) \
    value(LWM2M_TYPE_,integer          ,LWM2M_TYPE_INTEGER) \
    value(LWM2M_TYPE_,float            ,LWM2M_TYPE_FLOAT) \
    value(LWM2M_TYPE_,boolean          ,LWM2M_TYPE_BOOLEAN) \
    value(LWM2M_TYPE_,objlnk           ,LWM2M_TYPE_OBJECT_LINK) \
    value(LWM2M_TYPE_,time             ,LWM2M_TYPE_TIME)

DECLARE_PROPERTY_ENUM(lwm2m_, type, type_values)


typedef bool (*lwm2m_each_instance_func)(uint16_t instance_id, void *data);
typedef uint8_t (*lwm2m_resource_notify_func)(uint16_t instance);
typedef uint8_t (*lwm2m_resource_confirmed_func)(lwm2m_object_t *object, uint16_t instance, uint16_t resource_id);
typedef uint8_t (*lwm2m_resource_get_func)(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data);
typedef uint8_t (*lwm2m_resource_set_func)(lwm2m_object_t *object, uint16_t instance, uint16_t resource_instance, lwm2m_data_t *data);
typedef uint8_t (*lwm2m_resource_execute_func)(lwm2m_object_t *object, uint16_t instance, uint16_t resource_id, uint8_t * buffer, int length);
typedef uint8_t (*lwm2m_resource_delete_func)(uint16_t instance);
typedef uint8_t (*lwm2m_resource_create_func)(uint16_t instance);
typedef bool (*lwm2m_resource_is_set_func)(uint16_t instance, uint16_t resource_instance);

typedef struct lwm2m_resource_declaration_t
{
  bool supported;
  uint16_t id;
  uint8_t name[LWM2M_NAME_RESOURCE_LEN];
  lwm2m_operation_t operation;
  lwm2m_instancetype_t instances;
  bool mandatory;
  bool persistent;
  lwm2m_type_t type;
  uint16_t size;
  lwm2m_resource_notify_func changed;
  lwm2m_resource_confirmed_func confirmed;
  lwm2m_resource_execute_func execute;
  lwm2m_resource_get_func get;
  lwm2m_resource_set_func set;
  lwm2m_resource_is_set_func is_set;
}
lwm2m_resource_declaration_t;

typedef uint8_t (*lwm2m_object_load_func)(
  uint16_t instance_id,
  int * num_data,
  lwm2m_data_t ** data_array,
  lwm2m_object_t * object);

typedef uint8_t (*lwm2m_object_save_func)(
  uint16_t instance_id,
  lwm2m_object_t * object);

typedef struct lwm2m_object_declaration_t
{
  uint16_t id;
  lwm2m_context_t *context;
  lwm2m_resource_delete_func delete;
  lwm2m_resource_create_func create;
  struct Mutex lock;
  lwm2m_resource_declaration_t resources[MAX_LWM2M_RESOURCES_PER_OBJECT];
  uint32_t data;
  lwm2m_instancetype_t instances;
  lwm2m_object_load_func load;
  lwm2m_object_save_func save;
  lwm2m_resource_confirmed_func confirmed;
}
lwm2m_object_declaration_t;

typedef struct lwm2m_object_link_t
{
  uint16_t object_id;
  uint16_t instance_id;
} lwm2m_object_link_t;

void lwm2m_object_base_initialse_object(lwm2m_object_t *object);
void lwm2m_object_base_create_instance(lwm2m_object_t *object, void *instance);

/*
 * Called when the underlying resource value has changed and the lwm2m engine
 * should check if there is an observer of it. Also, calls the changed function
 * for the resource.
 */
void lwm2m_object_base_resource_changed(lwm2m_object_t *object, uint16_t instance_id, uint16_t resource_id, bool confirm);
void lwm2m_object_base_observe_confirm(lwm2m_object_t * object, uint16_t instance_id, uint16_t resource_id);

uint8_t lwm2m_object_base_server_read(uint16_t instance_id,int * num_data,lwm2m_data_t ** data_array,lwm2m_object_t * object);
uint8_t lwm2m_object_base_server_write(uint16_t instance_id,int num_data,lwm2m_data_t * data_array,lwm2m_object_t * object);
uint8_t lwm2m_object_base_server_execute(uint16_t instance_id,uint16_t resource_id,uint8_t * buffer,int length,lwm2m_object_t * object);
void lwm2m_object_base_register_confirmable(lwm2m_object_t *object, uint16_t instance_id, uint16_t resource_id);

#endif /* LWM2M_SOFTWARE_OBJECT_H_ */

/* @} */
