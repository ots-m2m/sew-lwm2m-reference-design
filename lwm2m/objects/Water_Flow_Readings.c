/*
 * Water_Flow_Readings.c
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 */
#include "lwm2m_object_options.h"

#include "Water_Flow_Readings.h"
#include "interval_data_base.h"
#include "log.h"
#include <assert.h>

#ifndef DEBUG_WATER_FLOW_READINGS
#undef log_debug
#define log_debug(...)
#endif

/**-----------------------------------------------------------------------------
 *  @file
 *  @defgroup    Water_Flow_Readings
 *  @ingroup     interval_data_base
 *  @{
 *------------------------------------------------------------------------------
 */

extern lwm2m_object_declaration_t Water_Flow_Readings_object_declaration;

#ifndef MAX_Water_Flow_Readings_INSTANCES
#define MAX_Water_Flow_Readings_INSTANCES 1
#endif

#define DEFAULT_INSTANCE_0_VALUE_INTERVAL_PERIOD                    CONFIG_Water_Flow_Readings_DEFAULT_INSTANCE_0_VALUE_INTERVAL_PERIOD
#define DEFAULT_INSTANCE_0_VALUE_INTERVAL_START_OFFSET              CONFIG_Water_Flow_Readings_DEFAULT_INSTANCE_0_VALUE_INTERVAL_START_OFFSET
#define DEFAULT_INSTANCE_0_VALUE_INTERVAL_UTC_OFFSET                CONFIG_Water_Flow_Readings_DEFAULT_INSTANCE_0_VALUE_INTERVAL_UTC_OFFSET
#define DEFAULT_INSTANCE_0_VALUE_INTERVAL_DELIVERY_MIDNIGHT_ALIGNED CONFIG_Water_Flow_Readings_DEFAULT_INSTANCE_0_VALUE_INTERVAL_DELIVERY_MIDNIGHT_ALIGNED
#define DEFAULT_INSTANCE_1_VALUE_INTERVAL_PERIOD                    CONFIG_Water_Flow_Readings_DEFAULT_INSTANCE_1_VALUE_INTERVAL_PERIOD
#define DEFAULT_INSTANCE_1_VALUE_INTERVAL_START_OFFSET              CONFIG_Water_Flow_Readings_DEFAULT_INSTANCE_1_VALUE_INTERVAL_START_OFFSET
#define DEFAULT_INSTANCE_1_VALUE_INTERVAL_UTC_OFFSET                CONFIG_Water_Flow_Readings_DEFAULT_INSTANCE_1_VALUE_INTERVAL_UTC_OFFSET
#define DEFAULT_INSTANCE_1_VALUE_INTERVAL_DELIVERY_MIDNIGHT_ALIGNED CONFIG_Water_Flow_Readings_DEFAULT_INSTANCE_1_VALUE_INTERVAL_DELIVERY_MIDNIGHT_ALIGNED

double Water_Flow_Readings_get_value(uint16_t instance_id, void **sensor_ext_ptr);  // Required by interval base declaration

// The DECLARE_EVENT_AND_ALARM_INSTANCE macro requires a function <name>_get_value to be
// declared which is used to read the actual alarm value from the sensor.
DECLARE_INTERVAL_DATA_INSTANCE(0, Water_Flow_Readings, interval_data_base_0);
DECLARE_INTERVAL_DATA_INSTANCE(1, Water_Flow_Readings, interval_data_base_1);
DECLARE_INTERVAL_DATA_INSTANCE(2, Water_Flow_Readings, interval_data_base_2);
DECLARE_INTERVAL_DATA_INSTANCE(3, Water_Flow_Readings, interval_data_base_3);

static interval_data_base_t *interval_data_instances[] = {
  &interval_data_base_0,
  &interval_data_base_1,
  &interval_data_base_2,
  &interval_data_base_3
};

/*******************************************************************************
*******************************************************************************/

__static void Water_Flow_Readings_object_initialise_instance(
    uint16_t instance_id,
    const uint32_t default_interval_period,
    const uint32_t default_start_offset,
    const char     *default_UTC_offset_seconds,
    const uint16_t default_UTC_offset_seconds_size,
    const bool     default_delivery_midnight_aligned
  )
{
  assert(instance_id < MAX_Water_Flow_Readings_INSTANCES);
  interval_data_base_t *base_ptr = interval_data_instances[instance_id];

  /** Initialise Base Class
  ***************************/
  interval_data_base_initalise_instance(base_ptr, &Water_Flow_Readings_object_declaration);

  /** Set Defaults
  *****************/
  Water_Flow_Readings_Interval_Period_set                    (base_ptr->instance_id, default_interval_period,           false);
  Water_Flow_Readings_Interval_Start_Offset_set              (base_ptr->instance_id, default_start_offset,              false);
  Water_Flow_Readings_Interval_UTC_Offset_set                (base_ptr->instance_id, default_UTC_offset_seconds,        default_UTC_offset_seconds_size, false);
  Water_Flow_Readings_Interval_Delivery_Midnight_Aligned_set (base_ptr->instance_id, default_delivery_midnight_aligned, false);

  /** Check Defaults
  *******************/
  interval_data_base_initalise_verify_defaults(base_ptr,
      default_interval_period,
      default_start_offset,
      default_UTC_offset_seconds,
      default_UTC_offset_seconds_size,
      default_delivery_midnight_aligned
    );

  /** Register interval_data_base_t pointer to user data
  *******************************************************/

  if (!Water_Flow_Readings_set_user_data(base_ptr->instance_id, base_ptr))
  {
    log_err("failed to set the base pointer");
  }

  //make these resources confirmable resources, so that when GET is called an ACK is sent by the server
  //which calls the base confirmed handler that advances the delivered pointer.
  lwm2m_object_base_register_confirmable(Water_Flow_Readings_get_lwm2m_object(), base_ptr->instance_id, 4014);
  lwm2m_object_base_register_confirmable(Water_Flow_Readings_get_lwm2m_object(), base_ptr->instance_id, 9009);

}



/**-----------------------------------------------------------------------------
  GET SENSOR
------------------------------------------------------------------------------*/

// TODO: This function is where you hook into your sensor to read the sample
//
// This function must read the sensor value to return the current value of the sensor.
// e.g. read the ADC for its current value and convert to the units that are aligned
// with the units in the threshold operators.
double Water_Flow_Readings_get_value(uint16_t instance_id, void **sensor_ext_ptr)
{
  static double prev_value_array[MAX_Water_Flow_Readings_INSTANCES] = {0};
  double return_value = prev_value_array[instance_id];  // get

  // Go up or go down
  if (rand() % 2)
  {
    return_value += ((double)rand()/(double)RAND_MAX);
  }
  else
  {
    return_value -= ((double)rand()/(double)RAND_MAX);
  }

  // Range restrict
  if (return_value < 0)
  {
    return_value = 0.0;
  }
  else if (return_value > 1000)
  {
    return_value = 0.0;
  }

  prev_value_array[instance_id] = return_value; // save
  log_debug("water flow %d: %ld", instance_id, (long) return_value);
  return return_value;
}

/**-----------------------------------------------------------------------------
  Callback
------------------------------------------------------------------------------*/

//called when a read is requested on the Latest_Payload Resource, we use this
//hook to build the payload and copy it to the Latest Payload opaque resource
uint8_t Water_Flow_Readings_Latest_Payload_get(uint16_t instance,  uint8_t **val, size_t *length)
{
  Water_Flow_Readings_lock();
  interval_data_base_Latest_Payload_build((interval_data_base_t *) Water_Flow_Readings_get_user_data(instance), instance, val, length);

  Water_Flow_Readings_unlock();
  return COAP_205_CONTENT;
}


uint8_t Water_Flow_Readings_UTC_Offset_changed(uint16_t instance)
{
  interval_data_base_Interval_UTC_Offset_changed_handler(
      (interval_data_base_t *) Water_Flow_Readings_get_user_data(instance),
      instance
    );
  return COAP_NO_ERROR;
}

uint8_t Water_Flow_Readings_Interval_Historical_Read_execute(lwm2m_object_t *object, uint16_t instance, uint16_t resource_id, uint8_t * buffer, int length)
{
  return interval_data_base_Interval_Historical_Read_execute_handler(
      (interval_data_base_t *) Water_Flow_Readings_get_user_data(instance),
      instance, buffer, length
    );
}

uint8_t Water_Flow_Readings_Interval_Historical_Read_Payload_confirmed(lwm2m_object_t *object, uint16_t instance, uint16_t resource_id)
{
  interval_data_base_Interval_Historical_Read_Payload_confirmed_handler(
      (interval_data_base_t *) Water_Flow_Readings_get_user_data(instance),
      instance
    );
  return COAP_NO_ERROR;
}

uint8_t Water_Flow_Readings_Latest_Payload_confirmed(lwm2m_object_t *object, uint16_t instance, uint16_t resource_id)
{
  interval_data_base_Latest_Payload_confirmed_handler(
      (interval_data_base_t *) Water_Flow_Readings_get_user_data(instance),
      instance
    );
  return COAP_NO_ERROR;
}

uint8_t Water_Flow_Readings_Interval_Change_Configuration_execute(lwm2m_object_t *object, uint16_t instance_id, uint16_t resource_id, uint8_t * buffer, int length)
{
  // MAX_Water_Flow_Readings_Interval_UTC_Offset is the size limit of UTC offset string (including \0)
  return interval_data_base_Interval_Change_Configuration_execute_handler(
    (interval_data_base_t *) Water_Flow_Readings_get_user_data(instance_id),
    instance_id, buffer, length
  );
}

uint8_t Water_Flow_Readings_Start_execute(lwm2m_object_t *object, uint16_t instance, uint16_t resource_id, uint8_t * buffer, int length)
{
  return interval_data_base_Start_execute_handler(
    (interval_data_base_t *) Water_Flow_Readings_get_user_data(instance),
    instance
  );
}

uint8_t Water_Flow_Readings_Stop_execute(lwm2m_object_t *object, uint16_t instance, uint16_t resource_id, uint8_t * buffer, int length)
{
  return interval_data_base_Stop_execute_handler(
     (interval_data_base_t *) Water_Flow_Readings_get_user_data(instance),
     instance
   );
}
/*******************************************************************************
  EXTERNAL INTERFACE
*******************************************************************************/
//This function is called by the lwm2m stack when a new instance is created.
void Water_Flow_Readings_instance_created(uint16_t instance_id)
{
  Water_Flow_Readings_object_initialise_instance(instance_id,
    DEFAULT_INSTANCE_0_VALUE_INTERVAL_PERIOD                   ,
    DEFAULT_INSTANCE_0_VALUE_INTERVAL_START_OFFSET             ,
    DEFAULT_INSTANCE_0_VALUE_INTERVAL_UTC_OFFSET               ,
    sizeof(DEFAULT_INSTANCE_0_VALUE_INTERVAL_UTC_OFFSET)       ,
    DEFAULT_INSTANCE_0_VALUE_INTERVAL_DELIVERY_MIDNIGHT_ALIGNED
  );
}

//This function is called by the lwm2m stack when the instance is deleted
void Water_Flow_Readings_instance_deleted(uint16_t instance_id)
{
  if (!interval_data_base_release_instance(instance_id))
  {
    log_err("request to delete a non existing interval data instance");
  }
}

void Water_Flow_Readings_object_initialise(void)
{
  Water_Flow_Readings_initialise();

  // Register Callbacks
  Water_Flow_Readings_object_declaration.resources[2].changed   = Water_Flow_Readings_UTC_Offset_changed;                         // UTC Offset
  Water_Flow_Readings_object_declaration.resources[8].execute   = Water_Flow_Readings_Interval_Historical_Read_execute;           // Interval Historical Read Executable
  Water_Flow_Readings_object_declaration.resources[9].confirmed = Water_Flow_Readings_Interval_Historical_Read_Payload_confirmed; // Interval Historical Read Payload Confirmation
  Water_Flow_Readings_object_declaration.resources[14].confirmed = Water_Flow_Readings_Latest_Payload_confirmed; // Interval Historical Read Payload Confirmation
  Water_Flow_Readings_object_declaration.resources[10].execute  = Water_Flow_Readings_Interval_Change_Configuration_execute;      // Interval Change Configuration Executable

  Water_Flow_Readings_create_instance(0);
  Water_Flow_Readings_create_instance(1);

  //instance 0 is configured with the correct defaults, change config
  //for instance 1 to match the SEW design.
  Water_Flow_Readings_Interval_Period_set(1, DEFAULT_INSTANCE_1_VALUE_INTERVAL_PERIOD,false);
  Water_Flow_Readings_Interval_Start_Offset_set(1, DEFAULT_INSTANCE_1_VALUE_INTERVAL_START_OFFSET, false);
  Water_Flow_Readings_Interval_UTC_Offset_set(1, DEFAULT_INSTANCE_1_VALUE_INTERVAL_UTC_OFFSET, strlen(DEFAULT_INSTANCE_1_VALUE_INTERVAL_UTC_OFFSET), false);
  Water_Flow_Readings_Interval_Delivery_Midnight_Aligned_set(1, DEFAULT_INSTANCE_1_VALUE_INTERVAL_DELIVERY_MIDNIGHT_ALIGNED, false);
}


void Water_Flow_Readings_register(lwm2m_context_t *context)
{
  Water_Flow_Readings_register_object(context);
  Water_Flow_Readings_register_instance(0);
  lwm2m_object_base_register_confirmable(Water_Flow_Readings_get_lwm2m_object(), 0, 4014);
  lwm2m_object_base_register_confirmable(Water_Flow_Readings_get_lwm2m_object(), 0, 9009);

  Water_Flow_Readings_register_instance(1);
  lwm2m_object_base_register_confirmable(Water_Flow_Readings_get_lwm2m_object(), 1, 4014);
  lwm2m_object_base_register_confirmable(Water_Flow_Readings_get_lwm2m_object(), 1, 9009);
}
