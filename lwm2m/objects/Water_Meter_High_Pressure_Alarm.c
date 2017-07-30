/*
 * Water_Meter_High_Pressure_Alarm.c
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 *
 */
#include "lwm2m_object_options.h"

#include "Water_Meter_High_Pressure_Alarm.h"
#include "event_and_alarm_base.h"
#include "log.h"
#include <assert.h>

#define DEFAULT_EVENT_TYPE           CONFIG_Water_Meter_High_Pressure_Alarm_DEFAULT_EVENT_TYPE
#define DEFAULT_EVENT_CODE           CONFIG_Water_Meter_High_Pressure_Alarm_DEFAULT_EVENT_CODE
#define DEFAULT_REALTIME             CONFIG_Water_Meter_High_Pressure_Alarm_DEFAULT_REALTIME
#define DEFAULT_SET_THRESHOLD        CONFIG_Water_Meter_High_Pressure_Alarm_DEFAULT_SET_THRESHOLD
#define DEFAULT_SET_OPERATOR         CONFIG_Water_Meter_High_Pressure_Alarm_DEFAULT_SET_OPERATOR
#define DEFAULT_CLEAR_THRESHOLD      CONFIG_Water_Meter_High_Pressure_Alarm_DEFAULT_CLEAR_THRESHOLD
#define DEFAULT_CLEAR_OPERATOR       CONFIG_Water_Meter_High_Pressure_Alarm_DEFAULT_CLEAR_OPERATOR
#define DEFAULT_MAXIMUM_EVENT_COUNT  CONFIG_Water_Meter_High_Pressure_Alarm_DEFAULT_MAXIMUM_EVENT_COUNT
#define DEFAULT_MAXIMUM_EVENT_PERIOD CONFIG_Water_Meter_High_Pressure_Alarm_DEFAULT_MAXIMUM_EVENT_PERIOD
#define DEFAULT_AUTO_CLEAR           CONFIG_Water_Meter_High_Pressure_Alarm_DEFAULT_AUTO_CLEAR


#ifndef DEBUG_WATER_METER_HIGH_PRESSURE_ALARM
#undef log_debug
#define log_debug(...)
#endif

/**-----------------------------------------------------------------------------
 *  @file
 *  @defgroup    Water_Meter_High_Pressure_Alarm
 *  @ingroup     event_and_alarm_base
 *  @{
 *------------------------------------------------------------------------------
 */
extern lwm2m_object_declaration_t Water_Meter_High_Pressure_Alarm_object_declaration;

static void Water_Meter_High_Pressure_Alarm_Payload_Alarm_State_Change_Log_append(cbor_stream_t *stream, double new_sensor_value, bool new_alarm_state);
static void Water_Meter_High_Pressure_Alarm_Payload_Event_Log_append(cbor_stream_t *stream, double new_sensor_value, bool new_alarm_state);

double Water_Meter_High_Pressure_Alarm_get_value(uint16_t instance_id);

// The DECLARE_EVENT_AND_ALARM_INSTANCE macro requires a function <name>_get_value to be
// declared which is used to read the actual alarm value from the sensor.
//
// The instances are pre-declared to avoid dynamic memory allocation. If you add more
// then you need to ensure MAX_Water_Meter_High_Pressure_Alarm_INSTANCES matches the
// number of instances you need.
DECLARE_EVENT_AND_ALARM_INSTANCE(0, Water_Meter_High_Pressure_Alarm, alarm_base_0);
DECLARE_EVENT_AND_ALARM_INSTANCE(1, Water_Meter_High_Pressure_Alarm, alarm_base_1);
DECLARE_EVENT_AND_ALARM_INSTANCE(2, Water_Meter_High_Pressure_Alarm, alarm_base_2);
DECLARE_EVENT_AND_ALARM_INSTANCE(3, Water_Meter_High_Pressure_Alarm, alarm_base_3);

static double event_and_alarm_values[MAX_Water_Meter_High_Pressure_Alarm_INSTANCES];

static event_and_alarm_base_t *event_and_alarm_instance[] = {
  &alarm_base_0,
  &alarm_base_1,
  &alarm_base_2,
  &alarm_base_3
};
/*******************************************************************************
*******************************************************************************/

__static void Water_Meter_High_Pressure_Alarm_Payload_Alarm_State_Change_Log_append(cbor_stream_t *stream, double new_sensor_value, bool new_alarm_state)
{
  cbor_serialize_int(stream, new_alarm_state);
  cbor_serialize_int(stream, (uint16_t) new_sensor_value);
}

__static void Water_Meter_High_Pressure_Alarm_Payload_Event_Log_append(cbor_stream_t *stream, double new_sensor_value, bool new_alarm_state)
{
  cbor_serialize_int(stream, new_alarm_state);
  cbor_serialize_int(stream, (uint16_t) new_sensor_value);
}


/**-----------------------------------------------------------------------------
  # EXTERNAL INTERFACE : GET SENSOR
------------------------------------------------------------------------------*/

// This function must read the sensor value to return the current value of the sensor.
// e.g. read the ADC for its current value and convert to the units that are aligned
// with the units in the threshold operators.
double Water_Meter_High_Pressure_Alarm_get_value(uint16_t instance_id)
{
  assert(instance_id < MAX_Water_Meter_High_Pressure_Alarm_INSTANCES);
  return event_and_alarm_values[instance_id];
}

// This is used in integration test so that test sensor values can be set
void Water_Meter_High_Pressure_Alarm_set_value(uint16_t instance_id, double sensor_value)
{
  assert(instance_id < MAX_Water_Meter_High_Pressure_Alarm_INSTANCES);
  event_and_alarm_values[instance_id] = sensor_value;
}

/*------------------------------------------------------------------------------
  # EXTERNAL INTERFACE
------------------------------------------------------------------------------*/

//This function is called by the lwm2m stack when a new instance is created.
void Water_Meter_High_Pressure_Alarm_instance_created(uint16_t instance_id)
{
    //default values configured per page 48 of Digital Utility IoT LwM2M Technical Specification  v0.7
    Water_Meter_High_Pressure_Alarm_Event_Type_set(instance_id,                 DEFAULT_EVENT_TYPE           , false);
    Water_Meter_High_Pressure_Alarm_Event_Code_set(instance_id,                 DEFAULT_EVENT_CODE           , false);
    Water_Meter_High_Pressure_Alarm_Alarm_Realtime_set(instance_id,             DEFAULT_REALTIME             , false);
    Water_Meter_High_Pressure_Alarm_Alarm_Set_Threshold_set(instance_id,        DEFAULT_SET_THRESHOLD        , false);
    Water_Meter_High_Pressure_Alarm_Alarm_Set_Operator_set(instance_id,         DEFAULT_SET_OPERATOR         , false);
    Water_Meter_High_Pressure_Alarm_Alarm_Clear_Threshold_set(instance_id,      DEFAULT_CLEAR_THRESHOLD      , false);
    Water_Meter_High_Pressure_Alarm_Alarm_Clear_Operator_set(instance_id,       DEFAULT_CLEAR_OPERATOR       , false);
    Water_Meter_High_Pressure_Alarm_Alarm_Maximum_Event_Count_set(instance_id,  DEFAULT_MAXIMUM_EVENT_COUNT  , false);
    Water_Meter_High_Pressure_Alarm_Alarm_Maximum_Event_Period_set(instance_id, DEFAULT_MAXIMUM_EVENT_PERIOD , false);
    Water_Meter_High_Pressure_Alarm_Alarm_Auto_Clear_set(instance_id,           DEFAULT_AUTO_CLEAR           , false);

    event_and_alarm_instance[instance_id]->object_id = Water_Meter_High_Pressure_Alarm_get_lwm2m_object()->objID;
    event_and_alarm_base_initalise_instance(event_and_alarm_instance[instance_id], &Water_Meter_High_Pressure_Alarm_object_declaration);

    lwm2m_object_base_register_confirmable(Water_Meter_High_Pressure_Alarm_get_lwm2m_object(), instance_id, 4014);
}

void Water_Meter_High_Pressure_Alarm_object_initialise(void)
{
  Water_Meter_High_Pressure_Alarm_initialise();
  Water_Meter_High_Pressure_Alarm_create_instance(0);
}

//FIXME: the json must have a space after the object name since the code generator generates two underscores.
uint8_t Water_Meter_High_Pressure_Alarm_Alarm_Clear_execute(lwm2m_object_t *object, uint16_t instance, uint16_t resource_id)
{
  return event_and_alarm_base_alarm_clear_execute(object, instance, resource_id);
}


/*------------------------------------------------------------------------------
  # Overridden Base Function For Latest Payload Get
------------------------------------------------------------------------------*/

uint8_t Water_Meter_High_Pressure_Alarm_Latest_Payload_get(uint16_t instance, uint8_t **val, size_t *length)
{
  uint8_t return_code = 0;
  Water_Meter_High_Pressure_Alarm_lock();
  assert(instance >= 0 && instance < MAX_Water_Meter_High_Pressure_Alarm_INSTANCES);

  // Generate CBOR Payload
  return_code = event_and_alarm_base_Latest_Payload_get(instance, val, length);

  Water_Meter_High_Pressure_Alarm_unlock();

  return return_code;
}

void Water_Meter_High_Pressure_Alarm_register(lwm2m_context_t *context)
{
  Water_Meter_High_Pressure_Alarm_register_object(context);
  Water_Meter_High_Pressure_Alarm_register_instance(0);
  lwm2m_object_base_register_confirmable(Water_Meter_High_Pressure_Alarm_get_lwm2m_object(), 0, 4014);
}
