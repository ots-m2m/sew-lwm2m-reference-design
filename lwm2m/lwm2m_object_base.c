/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * This is the base object code for the generated LWM2M objects
 */
#include "lwm2m_object_base.h"
#include "liblwm2m.h"
#include "internals.h"
#include "lwm2m_platform.h"
#include "sys_tick.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "log.h"

/**-----------------------------------------------------------------------------
 *  @file
 *  @defgroup    lwm2m_object_base
 *  @ingroup     lwm2m_client
 *  @{
 *------------------------------------------------------------------------------
 */


#undef log_debug
#define log_debug(M, ...)

typedef struct lwm2m_observe_confirmation_t
{
    lwm2m_object_t *object;
    uint16_t instance_id;
    uint16_t resource_id;
    uint32_t time;
} lwm2m_observe_confirmation_t;

static lwm2m_observe_confirmation_t lwm2m_observe_confirmations[MAX_CONFIRMATIONS];

/*******************************************************************************
 *******************************************************************************/

static int get_resource_index_for_id(lwm2m_object_declaration_t *object_decl,
                                     int id)
{ // Scan the resource array for a resource with a specific ID number
// Returns the index of the resource ID with matching ID number

    for (int i = 0; i < MAX_LWM2M_RESOURCES_PER_OBJECT; i++)
    {  // Scan Resource In Object
        if (object_decl->resources[i].supported
                && object_decl->resources[i].id == id)
        { // Resource with matching ID located. Return index value.
            return i;
        }
    }

    log_err("invalid resource id encountered");
    return -1;
}

/*------------------------------------------------------------------------------
 ------------------------------------------------------------------------------*/

uint8_t lwm2m_object_base_server_read(uint16_t instance_id, int * num_data,
                                      lwm2m_data_t ** data_array,
                                      lwm2m_object_t * object)
{
    uint8_t result = 0;
    //check if this instance is registered
    void *instance = lwm2m_list_find(object->instanceList, instance_id);
    if (0 == instance)
    {
        return COAP_404_NOT_FOUND ;
    }

    lwm2m_object_declaration_t *object_decl =
            (lwm2m_object_declaration_t *) object->userData;
    if (object_decl == 0)
    {
        return COAP_500_INTERNAL_SERVER_ERROR ;
    }

    bool requesting_full_object = (*num_data == 0);
    unsigned int max_resource_id = sizeof(object_decl->resources)
            / sizeof(lwm2m_resource_declaration_t);

    unsigned int i = 0;

    /* Server asking for the full instance
     ****************************************/
    unsigned int num_resources = 0;
    if (requesting_full_object)
    {
        for (i = 0; i < max_resource_id; i++)
        {
            if ((object_decl->resources[i].supported)
                    && ((object_decl->resources[i].operation
                            == LWM2M_OPERATION_R)
                            || (object_decl->resources[i].operation
                                    == LWM2M_OPERATION_RW)
                            || (object_decl->resources[i].operation
                                    == LWM2M_OPERATION_NONE)))
            {
                num_resources++;
            }
        }

        log_debug("loading %d resources", num_resources);
        *data_array = lwm2m_data_new(num_resources);
        if (*data_array == 0)
            return COAP_500_INTERNAL_SERVER_ERROR ;
        *num_data = num_resources;

        int resource_num = 0;
        for (i = 0; i < max_resource_id; i++)
        {
            if ((object_decl->resources[i].supported)
                    && ((object_decl->resources[i].operation
                            == LWM2M_OPERATION_R)
                            || (object_decl->resources[i].operation
                                    == LWM2M_OPERATION_RW)
                            || (object_decl->resources[i].operation
                                    == LWM2M_OPERATION_NONE)))
            {
                (*data_array)[resource_num++].id = object_decl->resources[i].id;
            }
        }
    }

    /* Load Instance
     ****************************************/
    if (object_decl->load)
    { // Call
        uint8_t res = object_decl->load(instance_id, num_data, data_array,
                                        object);

        if (res == COAP_NO_ERROR)
        {
            //we've been asked to defer response
            return COAP_NO_ERROR ;
        }

        if (res == COAP_500_INTERNAL_SERVER_ERROR)
        {
            //we have not been asked to defer response
            return COAP_500_INTERNAL_SERVER_ERROR ;
        }

    }

    /* Find Resource
     ****************************************/
    i = 0;
    unsigned int actual_resources = num_resources;
    do
    { // Check for resource in ID. If not there, then increment ID and check next resource
        int id = (*data_array)[i].id;
        int index = get_resource_index_for_id(object_decl, id);

        if (index >= 0)
        {  // Resource ID Is Within Range
            log_debug("read resource: %d/%d/%d", object_decl->id, instance_id, id);

            if (object_decl->resources[index].instances == LWM2M_INSTANCETYPE_multiple)
            {
               int resource_instance;
               int num_instances = 0;
               for (resource_instance = 0; resource_instance < MAX_MULTIPLE_RESOURCES; resource_instance++)
               {
                   if (object_decl->resources[index].is_set(instance_id, resource_instance)) num_instances++;
               }

               lwm2m_data_t* resource_tlv = lwm2m_data_new(num_instances);
               num_instances = 0;
               for (resource_instance = 0; resource_instance < MAX_MULTIPLE_RESOURCES; resource_instance++)
               {
                   if (object_decl->resources[index].is_set(instance_id, resource_instance))
                   {
                       resource_tlv[num_instances++].id = resource_instance;

                       result = object_decl->resources[index].get ?
                         object_decl->resources[index].get(object, instance_id, resource_instance, resource_tlv + resource_instance) :
                         COAP_404_NOT_FOUND;

                       if (result != COAP_205_CONTENT) break;
                   }
                   lwm2m_data_encode_instances(resource_tlv, num_instances, &(*data_array)[i]);
               }
            }
            else
            {
                // Call the .get() callback function
                result = object_decl->resources[index].get ?
                                object_decl->resources[index].get(object, instance_id, 0, &(*data_array)[i]) :
                                COAP_404_NOT_FOUND;
            }
        }
        else
        { // Resource ID is out of range and is non sensical
            log_debug("request for out of range resource id");
            result = COAP_404_NOT_FOUND;
        }

        if (result == COAP_404_NOT_FOUND)
        {
            (*data_array)[i].id = 0xffff;
            actual_resources--;
        }

        i++;

    }
    while ((int) i < *num_data
            && ((result == COAP_205_CONTENT ) || (result == COAP_404_NOT_FOUND )));

    /* Now copy what was actually returned
     ****************************************/
    if ((actual_resources != num_resources) && (requesting_full_object))
    {
        log_debug("removing %d resources", num_resources - actual_resources);
        int j = 0;
        lwm2m_data_t *tmp_array = lwm2m_data_new(actual_resources);
        for (i = 0; i < num_resources; i++)
        {
            if ((*data_array)[i].id != 0xffff)
            {
                memcpy(&tmp_array[j++], &(*data_array)[i],
                       sizeof(lwm2m_data_t));
            }
        }
        lwm2m_free(*data_array);
        *data_array = tmp_array;
        *num_data = actual_resources;
        result = COAP_205_CONTENT;
    }

    return result;
}

uint8_t lwm2m_object_base_server_execute(uint16_t instance_id,
                                         uint16_t resource_id, uint8_t * buffer,
                                         int length, lwm2m_object_t * object)
{
    uint8_t result = COAP_405_METHOD_NOT_ALLOWED;

    /** Check if instance exist in lwm2m object
    **********************************************************/
    void *instance = lwm2m_list_find(object->instanceList, instance_id);
    if (0 == instance)
    {
      return COAP_404_NOT_FOUND;
    }

    /** Get Object Declaration from lwm2m object
    *********************************************************/
    lwm2m_object_declaration_t *object_decl = 0;
    object_decl = (lwm2m_object_declaration_t *) object->userData;
    if (object_decl == 0)
    {
      return COAP_500_INTERNAL_SERVER_ERROR ;
    }

    /** Execute Resource
    *********************************************************/
    log_debug("executing resource: %d/%d/%d", object_decl->id, instance_id,
              resource_id);

    int index = get_resource_index_for_id(object_decl, resource_id);
    if (index >= 0)
    { // Resource Located. Now calling function
      // If function pointer is defined then call function else report error.
      result = (object_decl->resources[index].execute) ? object_decl->resources[index].execute(
        object,
        instance_id,
        resource_id,
        buffer,
        length) : COAP_405_METHOD_NOT_ALLOWED;
    }

    /** Report Execution Result
    *********************************************************/
    if (result != COAP_204_CHANGED)
    {
        log_err("error %d while executing resource: %d/%d/%d", result,
                object_decl->id, instance_id, resource_id);
    }
    return result;
}

//this function corrects the lwm2m data structure for object links so that
//they can be properly processed. We need to revisit wakaama and investigate
//why we need to do this.
static void lwm2m_process_object_link(lwm2m_resource_declaration_t *resource, lwm2m_data_t * data_array)
{
  if ((data_array->type == LWM2M_TYPE_OPAQUE) &&
     (resource->type == LWM2M_TYPE_objlnk))
  {
    data_array->type = LWM2M_TYPE_OBJECT_LINK;

    uint8_t *buffer = data_array->value.asBuffer.buffer;

    utils_copyValue(&data_array->value.asObjLink.objectId, buffer, sizeof(uint16_t));
    utils_copyValue(&data_array->value.asObjLink.objectInstanceId, buffer + sizeof(uint16_t), sizeof(uint16_t));
  }
}

uint8_t lwm2m_object_base_server_write(uint16_t instance_id, int num_data,
                                       lwm2m_data_t * data_array,
                                       lwm2m_object_t * object)
{ // Server writes to device
    uint8_t result = COAP_400_BAD_REQUEST; //we'll return this on empty write requests

    // Check if this instance is registered
    void *instance = lwm2m_list_find(object->instanceList, instance_id);
    if (0 == instance)
    {
      return COAP_404_NOT_FOUND;
    }

    // Get object declaration
    lwm2m_object_declaration_t *object_decl = 0;
    object_decl = (lwm2m_object_declaration_t *) object->userData;
    if (object_decl == 0)
    {
        return COAP_500_INTERNAL_SERVER_ERROR ;
    }

    // Write from `data_array[ sized: num_data ]` to remote server
    for (int i = 0; i < num_data; i++)
    {
        log_debug("write resource: %d/%d/%d",
                    object_decl->id,
                    instance_id,
                    data_array[i].id
                  );

        //
        int index = get_resource_index_for_id(object_decl, data_array[i].id);
        if (index >= 0)
        {
          // Check if data_array[i] represents single value or an array
          if (data_array[i].type == LWM2M_TYPE_MULTIPLE_RESOURCE)
          { // This data chunk is a multi value array

            // Pointer to next data chunk
            lwm2m_data_t* sub_tlv = data_array[i].value.asChildren.array;

            if (data_array[i].value.asChildren.count == 0)
            { // Empty Array... do nothing
              result = COAP_204_CHANGED;
            }
            else if (sub_tlv == NULL)
            { // Invalid pointer to child multi value array
              result = COAP_400_BAD_REQUEST;
            }
            else
            { // Iterate across each child value inside the multi value array
              for (int ri = 0; ri < data_array[i].value.asChildren.count; ri++)
              { // A child data chunk to process
                lwm2m_process_object_link(&object_decl->resources[index], &sub_tlv[ri]);

                // Call .set() handlers for each lwm2m object resource
                result = object_decl->resources[index].set ?
                  object_decl->resources[index].set(object, instance_id, ri, &sub_tlv[ri]) :
                  COAP_500_INTERNAL_SERVER_ERROR;
              }
            }
          }
          else
          { // This data chunk represents a singular value
            lwm2m_process_object_link(&object_decl->resources[index], &data_array[i]);

            // Call .set() handlers for each lwm2m object resource
            result = object_decl->resources[index].set ?
              object_decl->resources[index].set(object, instance_id, 0, &data_array[i]) :
              COAP_500_INTERNAL_SERVER_ERROR;
          }
        }

        // Check for errors
        if (result != COAP_204_CHANGED)
        {
          log_err("error 0x%.2x while writing resource: %d/%d/%d", result,
                    object_decl->id, instance_id, data_array[i].id);
            break;
        }

        // Call the changed handler for the resource
        if ((index >= 0) && (object_decl->resources[index].changed))
        {
            object_decl->resources[index].changed(instance_id);
        }
    }

    // Call the object save handler if resources was updated (if exist)
    if (object_decl->save && (result == COAP_204_CHANGED ))
    {
        uint8_t res = object_decl->save(instance_id, object);
        return res;
    }

    return result;
}

static uint8_t lwm2m_object_base_server_discover(uint16_t instance,
                                                 int * num_data,
                                                 lwm2m_data_t ** data_array,
                                                 lwm2m_object_t * object)
{
    return COAP_501_NOT_IMPLEMENTED ;
}

static uint8_t lwm2m_object_base_server_delete(uint16_t instance_id,
                                               lwm2m_object_t * object)
{
    lwm2m_object_declaration_t *object_decl =
            (lwm2m_object_declaration_t *) object->userData;
//  assert(object_decl != 0);
//  assert(object_decl->delete != 0);
    log_debug("deleting instance: %d/%d", object_decl->id, instance_id);
    return object_decl->delete ?
            object_decl->delete(instance_id) : COAP_500_INTERNAL_SERVER_ERROR ;
}

static uint8_t lwm2m_object_base_server_create(uint16_t instance_id,
                                               int num_data,
                                               lwm2m_data_t * data_array,
                                               lwm2m_object_t * object)
{
    lwm2m_object_declaration_t *object_decl =
            (lwm2m_object_declaration_t *) object->userData;
    if (object_decl == 0)
    {
        return COAP_500_INTERNAL_SERVER_ERROR ;
    }

    if (object_decl->create == 0)
    {
        return COAP_500_INTERNAL_SERVER_ERROR ;
    }
    uint8_t result = COAP_500_INTERNAL_SERVER_ERROR;
    if (object_decl->create)
    {
        log_debug("request to created instance of object %d", object_decl->id);
        result = object_decl->create(instance_id);
        if (result == COAP_201_CREATED)
        {
            log_debug("created instance: %d/%d", object_decl->id, instance_id);
            result = lwm2m_object_base_server_write(instance_id, num_data,
                                                    data_array, object);
            if (result != COAP_204_CHANGED)
            {
                lwm2m_object_base_server_delete(instance_id, object);
            }
            else
            {
                result = COAP_201_CREATED;
            }
        }
        else
        {
            log_debug("failed to created instance: %d/%d (0x%02x)", object_decl->id, instance_id, result);
        }
    }
    return result;
}

void lwm2m_object_base_initialse_object(lwm2m_object_t *object)
{
    if (0 != object)
    {
        uint16_t tmp = object->objID;
        memset(object, 0, sizeof(lwm2m_object_t));
        object->objID = tmp;
        object->readFunc = lwm2m_object_base_server_read;
        object->writeFunc = lwm2m_object_base_server_write;
        object->createFunc = lwm2m_object_base_server_create;
        object->deleteFunc = lwm2m_object_base_server_delete;
        object->executeFunc = lwm2m_object_base_server_execute;
        object->discoverFunc = lwm2m_object_base_server_discover;
    }
}

void lwm2m_observe_confirm_handler(void * data)
{
    size_t i = (size_t) data;
    if (i >= MAX_CONFIRMATIONS)
    {
      return;
    }

    if (lwm2m_observe_confirmations[i].object == 0)
    {
      return;
    }

    lwm2m_object_declaration_t *object_decl = (lwm2m_object_declaration_t *) lwm2m_observe_confirmations[i].object->userData;

    log_debug("########### confirming response on %d/%d/%d (%d)",
      lwm2m_observe_confirmations[i].object->objID,
      lwm2m_observe_confirmations[i].instance_id,
      lwm2m_observe_confirmations[i].resource_id, i);

    int resource_index = get_resource_index_for_id(object_decl, lwm2m_observe_confirmations[i].resource_id);
    if (resource_index != -1)
    {
        if (object_decl->resources[resource_index].confirmed)
        {
            object_decl->resources[resource_index].confirmed(
              lwm2m_observe_confirmations[i].object,
              lwm2m_observe_confirmations[i].instance_id,
              lwm2m_observe_confirmations[i].resource_id);
        }
    }
    if (object_decl->confirmed)
    {
      object_decl->confirmed(lwm2m_observe_confirmations[i].object,
        lwm2m_observe_confirmations[i].instance_id,
        lwm2m_observe_confirmations[i].resource_id);
    }

    //release the confirmation if it is not a permanent
    if (lwm2m_observe_confirmations[i].time != 0xffffffff)
    {
      lwm2m_observe_confirmations[i].object = 0;
      lwm2m_observe_confirmations[i].time = 0;
    }
}


static size_t lwm2m_object_base_allocate_confirmation(lwm2m_object_t *object,
                                                    uint16_t instance_id,
                                                    uint16_t resource_id,
                                                    lwm2m_uri_t *uri)
{
   lwm2m_object_declaration_t *object_decl =
               (lwm2m_object_declaration_t *) object->userData;

   if (object_decl->context != 0)
   {
       uri->flag = 0;
       uri->objectId = object->objID;
       uri->instanceId = instance_id;
       uri->resourceId = resource_id;
       uri->flag |= LWM2M_URI_FLAG_OBJECT_ID;
       uri->flag |= LWM2M_URI_FLAG_INSTANCE_ID;
       uri->flag |= LWM2M_URI_FLAG_RESOURCE_ID;

       size_t i = 0;
       int oldest_i = 0;

       uint32_t oldest_timer = 0xffffffff;
       for (i = 0; i < MAX_CONFIRMATIONS; i++)
       {
           if (lwm2m_observe_confirmations[i].time < oldest_timer)
           {
               oldest_timer = lwm2m_observe_confirmations[i].time;
               oldest_i = i;
           }
       }

       i = oldest_i;
       lwm2m_observe_confirmations[i].object = object;
       lwm2m_observe_confirmations[i].instance_id = instance_id;
       lwm2m_observe_confirmations[i].resource_id = resource_id;
       lwm2m_observe_confirmations[i].time = sys_tick_get_s();
       return i;
   }
   return -1;
}

void lwm2m_object_base_resource_changed(lwm2m_object_t *object,
                                        uint16_t instance_id,
                                        uint16_t resource_id,
                                        bool confirm)
{
    lwm2m_object_declaration_t *object_decl =
                   (lwm2m_object_declaration_t *) object->userData;
    lwm2m_uri_t uri;
    size_t i = lwm2m_object_base_allocate_confirmation(object, instance_id, resource_id, &uri);

    if (i != -1)
    {
        lwm2m_resource_value_changed_with_confirm(
                object_decl->context, &uri, confirm,
                confirm ? lwm2m_observe_confirm_handler : 0, (void *) i);
    }
}


void lwm2m_object_base_register_confirmable(lwm2m_object_t *object,
                                            uint16_t instance_id,
                                            uint16_t resource_id)
{
  lwm2m_object_declaration_t *object_decl =
                       (lwm2m_object_declaration_t *) object->userData;
  lwm2m_uri_t uri;
  size_t i = lwm2m_object_base_allocate_confirmation(object, instance_id, resource_id, &uri);
  if (i != -1)
  {
    lwm2m_observe_confirmations[i].time = 0xffffffff;//never timeout
    lwm2m_register_confirmable(object_decl->context, &uri, lwm2m_observe_confirm_handler, (void *) i);
  }
  else
  {
    log_warn("failed to register confirmable resource since the object is not registered");
  }
}



