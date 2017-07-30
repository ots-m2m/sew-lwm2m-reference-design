/*
 * Interval_Data_Delivery.c
 * 
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * This code was generated using gt-object-gen provided by Operational 
 * Technology Solutions M2M Pty. Ltd. For licensing enquires visit 
 * ot-solutions.com.au.
 *  
 * This file is a template for implementing application logic for the 
 * Interval_Data_Delivery object. Rename this file to have a .c extension 
 * and the associated temaple_h file to have a .h extension then
 * add your application behaviour.
 *
 */
#include "Interval_Data_Delivery.h"

#include "Device_base.h" // Device_UTC_Offset_get()
#include "utc_offset_parser.h" // utc_offset_parse()
#include "data_delivery_base.h"
#include "interval_data_base.h"
#include "Delivery_Schedule.h"
#include "log.h"

#include <assert.h>

//#undef log_debug
//#define log_debug(...)
extern lwm2m_object_declaration_t Interval_Data_Delivery_object_declaration;

#define INTERVAL_DATA_DELIVERY_DEFAULT_NAME "Default Interval Data Delivery"

//The default set of interval data objects to be delivered. This reference implementation
//only supports object 8010, a compliant meter must support the others here.
//1.  /8010/0 Flow Data Readings (daily)
//2.  /8010/1 Flow Data Readings (30 minutes)
//3.  /8011/0 Daily Maximum Flow Rate Readings
//4.  /8012/0 Water Temperature Readings *
//5.  /8013/0 Water Pressure Readings *
//6.  /8014/0 Battery Level Readings
//7.  /8015/0 Meter Communication Activity Time Readings
static lwm2m_object_link_t default_interval_data_links[] = {
  { 8010, 0},
  { 8010, 1},
//  { 8011, 0},
//  { 8012, 0},
//  { 8013, 0},
//  { 8014, 0},
//  { 8015, 0}
};

/**-----------------------------------------------------------------------------
 *  @file
 *  @defgroup    Interval_Data_Delivery
 *  @ingroup     lwm2m_object_base
 *  @{
 *------------------------------------------------------------------------------
 */

/**-----------------------------------------------------------------------------
  @brief Sends aggregated report once every transmission window
------------------------------------------------------------------------------*/

/// This is the object's global static structure.
static Interval_Data_Delivery_instance_variables_t instance_variables[MAX_Interval_Data_Delivery_INSTANCES];


/*******************************************************************************
  # INTERNAL FUNCTONS
*******************************************************************************/

__static void Interval_Data_Delivery_instance_tick(Interval_Data_Delivery_instance_variables_t *var_ptr, uint16_t  instance, uint32_t unix_epoch_time)
{
  assert(var_ptr);

}

/**-----------------------------------------------------------------------------
  Tick Call Back Function Adapter for Interval_Data_foreach()

------------------------------------------------------------------------------*/
__static bool Interval_Data_Delivery_instance_tick_callback(uint16_t instance_id, void *data)
{ // Callback function for Interval_Data_foreach()

  /** Process Parameter
  **********************/
  uint32_t *unix_epoch_time_ptr = (uint32_t *)data;


  /** Get User Data
  **********************/
  Interval_Data_Delivery_instance_variables_t *var_ptr = (Interval_Data_Delivery_instance_variables_t *) Interval_Data_Delivery_get_user_data(instance_id);
  assert(var_ptr);  // Pointer must be defined
  assert(var_ptr->instance_id == instance_id);  // Should have been registered already


  /** Process Instance
  **********************/
  Interval_Data_Delivery_instance_tick(var_ptr, instance_id, *unix_epoch_time_ptr);

  return true;  // No problems detected
}

/*******************************************************************************
  # CALLBACK FUCNTIONS
*******************************************************************************/

//
// This function is called when the scheduler wants us to report data.
void Interval_Data_Delivery_report(lwm2m_object_link_t delivery_object, lwm2m_object_link_t schedule, void *user_data)
{
  for (uint16_t i = 0; i < MAX_MULTIPLE_RESOURCES; i++)
  {
    if (Interval_Data_Delivery_Interval_Data_Links_is_set(delivery_object.instance_id, i))
    {
      lwm2m_object_link_t interval_object;
      Interval_Data_Delivery_Interval_Data_Links_get(delivery_object.instance_id, i, &interval_object);

      //interval_object now points to an object that this delivery object must deliver the payload from. So we must
      //generate the payload and set it onto the value field for this resource.
      uint8_t *payload;
      size_t payload_length;
      interval_data_base_Latest_Payload_build_for_object_instance(interval_object, &payload, &payload_length);

      //set the payload onto the resource. Note we do NOT notify when we set this resource because we want to
      //get a confirmation and the default notify does not request confirmation, AND we want to
      //aggregate all payloads before notifying the change of the resource.
      Interval_Data_Delivery_Latest_Payload_set(delivery_object.instance_id, i, payload, payload_length, false);
    }
  }

  //FIXME: The hard coded resource ID '2' needs to be replaced with a constant generated by the code generator.
  lwm2m_object_base_resource_changed(Interval_Data_Delivery_get_lwm2m_object(), delivery_object.instance_id, 2, true);
}

//called when the schedule object link is changed.
uint8_t Interval_Data_Delivery_Schedule_changed(uint16_t instance)
{
  lwm2m_object_link_t delivery_object;
  delivery_object.instance_id = instance;
  delivery_object.object_id = Interval_Data_Delivery_get_lwm2m_object()->objID;

  data_delivery_base_remove_handlers(delivery_object);

  lwm2m_object_link_t schedule;
  Interval_Data_Delivery_Schedule_get(instance, &schedule);

  data_delivery_base_add_handler(delivery_object, schedule, Interval_Data_Delivery_report, 0);
  return COAP_NO_ERROR;
}

//called when the payload delivery has been confirmed by the server, this is where we must
//advance the delivered pointer.
uint8_t Interval_Data_Delivery_Latest_Payload_confirmed(lwm2m_object_t *object, uint16_t instance, uint16_t resource_id)
{
   log_debug("Confirming Interval Data Delivery Latest Payload on instance %d", instance);
  //iterate over each object link and call the confirmed handler for each interval object
  for (uint16_t i = 0; i < MAX_MULTIPLE_RESOURCES; i++)
  {
    if (Interval_Data_Delivery_Interval_Data_Links_is_set(instance, i))
    {
      lwm2m_object_link_t interval_object;
      Interval_Data_Delivery_Interval_Data_Links_get(instance, i, &interval_object);

      interval_data_base_Latest_Payload_confirmed_for_object_instance(interval_object);
    }
  }
  lwm2m_object_link_t schedule;
  Interval_Data_Delivery_Schedule_get(instance, &schedule);
  Delivery_Schedule_confirm(schedule.instance_id);

  return COAP_NO_ERROR;
}

/*******************************************************************************
  # EXTERNAL INTERFACE
*******************************************************************************/

__static void Interval_Data_Delivery_object_initialise_instance(uint16_t instance_id)
{
  log_debug("Initialising Interval Data Delivery object with instance id of %u", instance_id);

  /** Register Instance Variables with base
  ******************************************/
  Interval_Data_Delivery_instance_variables_t *instance_variable = &instance_variables[Interval_Data_Delivery_get_index_for_instance(instance_id)];
  Interval_Data_Delivery_set_user_data(instance_id, instance_variable);

  /** Internal Instance Initialisation
  *************************************/
  memset(instance_variable, 0, sizeof(Interval_Data_Delivery_instance_variables_t));
  instance_variable->instance_id = instance_id;

  //register that the payload resource is a confirmable resource
  lwm2m_object_base_register_confirmable(Interval_Data_Delivery_get_lwm2m_object(), instance_id, 2);
}


// this function is called by the base code when a new instance has
// been created.
void Interval_Data_Delivery_instance_created(uint16_t instance_id)
{
  Interval_Data_Delivery_object_initialise_instance(instance_id);
}

void Interval_Data_Delivery_object_initialise(void)
{
  Interval_Data_Delivery_initialise();

  Interval_Data_Delivery_object_declaration.resources[2].confirmed = Interval_Data_Delivery_Latest_Payload_confirmed;

  Interval_Data_Delivery_create_instance(0);
  Interval_Data_Delivery_Name_set(0, INTERVAL_DATA_DELIVERY_DEFAULT_NAME, strlen(INTERVAL_DATA_DELIVERY_DEFAULT_NAME), false);
  for (int i = 0; i < sizeof(default_interval_data_links)/sizeof(lwm2m_object_link_t); i++)
  {
    Interval_Data_Delivery_Interval_Data_Links_set(0, i, default_interval_data_links[i], false);
  }

  lwm2m_object_link_t schedule_object_link = { 8002, 0};
  Interval_Data_Delivery_Schedule_set(0, schedule_object_link, false);

}

void Interval_Data_Delivery_tick(uint32_t unix_epoch_time)
{
  Interval_Data_Delivery_foreach( Interval_Data_Delivery_instance_tick_callback, (void *)&unix_epoch_time);
}

void Interval_Data_Delivery_register(lwm2m_context_t *context)
{
  Interval_Data_Delivery_register_object(context);
  Interval_Data_Delivery_register_instance(0);
  //register that the payload resource is a confirmable resource
  lwm2m_object_base_register_confirmable(Interval_Data_Delivery_get_lwm2m_object(), 0, 2);
}

