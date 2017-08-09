/*
 * event_and_alarm_base.h
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 */

#ifndef __EVENT_AND_ALARM_BASE_H_
#define __EVENT_AND_ALARM_BASE_H_


#include <stdint.h>
#include <stdbool.h>
#include <assert.h>
#include <log.h>

#include "cbor.h"   // cbor_stream_t, cbor_init(), cbor_serialize_int(), cbor_destroy()

#include "lwm2m_object_base.h"

#if 0 /* This will make the led turn on/off based on alarm state */
#define EVENT_AND_ALARM_BASE_VISUAL_LED_ALARM_INDICATION_DEBUG
#endif

#define EVENT_AND_ALARM_BASE_VERBOSE_DEBUG 0 // Verbose mode for debug messages


typedef struct event_and_alarm_base_log_t
{
  uint16_t event_type;
  uint32_t timestamp;
  bool alarm_state;
  double sensor_value;
} event_and_alarm_base_log_t;

typedef struct event_and_alarm_base_payload_t
{ // Internal Variables
  uint16_t event_type;  // This is used to ensure the buffer is correctly encoded with the right payload structure for a given event type

  // Current State
  bool logger_current_state_filled;
  event_and_alarm_base_log_t logger_current_state;

  // Event Logger array
  event_and_alarm_base_log_t logger[EVENT_AND_ALARM_BASE_PAYLOAD_VALUE_COUNT_MAX];
  uint8_t logger_read_pos;
  uint8_t logger_write_pos;
  uint8_t logger_count;

  // Payload Generator
  uint8_t buffer[EVENT_AND_ALARM_BASE_PAYLOAD_BUFFER_SIZE_BYTES];
  cbor_stream_t stream;
  uint32_t latest_event_time;
} event_and_alarm_base_payload_t;


// Sensor Getter
typedef double  (*get_value_func_t)(uint16_t instance_id);

// Append event to stream
typedef void (*payload_append_event_func_t)(cbor_stream_t *stream, uint32_t timestamp, double new_sensor_value, bool new_alarm_state);

// Alarm Getter Function Types
typedef uint8_t (*bool_get_func_t     )(uint16_t instance, bool *val);
typedef uint8_t (*time_get_func_t     )(uint16_t instance, time_t *val);
typedef uint8_t (*double_get_func_t   )(uint16_t instance, double *val);
typedef uint8_t (*integer_get_func_t  )(uint16_t instance, uint32_t *val);

// Alarm Setter Function Types
typedef uint8_t (*bool_set_func_t     )(uint16_t instance, bool val, bool notify);
typedef uint8_t (*time_set_func_t     )(uint16_t instance, time_t val, bool notify);
typedef uint8_t (*double_set_func_t   )(uint16_t instance, double val, bool notify);
typedef uint8_t (*integer_set_func_t  )(uint16_t instance, uint32_t val, bool notify);

typedef enum event_and_alarm_threshold_operator_t
{
  EVENT_AND_ALARM_THRESHOLD_OPERATOR_DISABLED = 0,
  EVENT_AND_ALARM_GREATER_THAN_OR_EQUAL_TO    = 1,
  EVENT_AND_ALARM_LESS_THAN_OR_EQUAL_TO       = 2
}
event_and_alarm_threshold_operator_t;

typedef enum event_and_alarm_event_type_t
{
  EVENT_AND_ALARM_EVENT_TYPE_DISABLED                   = 0,
  EVENT_AND_ALARM_EVENT_TYPE_ALARM_CURRENT_STATE        = 1,
  EVENT_AND_ALARM_EVENT_TYPE_ALARM_STATE_CHANGE_LOG     = 2,
  EVENT_AND_ALARM_EVENT_TYPE_EVENT_LOG                  = 3
}
event_and_alarm_event_type_t;

// This defines the structure of a event/alarm instance
typedef struct event_and_alarm_base_t
{
  uint16_t instance_id;
  uint16_t object_id;
  struct
  { // Internal Variables
    uint16_t event_count;   // Total event count within a period
    uint32_t period_window; // Current time period
    event_and_alarm_base_payload_t payload;
  } var;

  /* Sensor
  *************/

  // Sensor Data
  get_value_func_t    get_value;            //  sensor value


  /* Payload Append Events
  *************************/
  payload_append_event_func_t payload_alarm_state_change_log_append;
  payload_append_event_func_t payload_event_log_append;

  /* Parameters
  *************/

  // Event Code
  integer_get_func_t  get_event_code;       // ID number of alarm event

  // Event Type
  integer_get_func_t  get_event_type;       // Indicates how this event/alarm instance will behave

  // Alarm Realtime
  bool_get_func_t     get_realtime;

  // Alarm State -- Indicates if alarm is actives
  bool_get_func_t     get_alarm_state;
  bool_set_func_t     set_alarm_state;

  /* Set */
  // Set Threshold -- threshold for when an alarm is triggered
  double_get_func_t   get_set_threshold;
  // Set Operator
  integer_get_func_t  get_set_operator;

  /* Clear */
  // Clear Threshold -- threshold for when an alarm is cleared
  double_get_func_t   get_clear_threshold;
  // Clear Operator
  integer_get_func_t  get_clear_operator;


  /* Rate Limting Events */ // (In technical specification (v0.7) ( "DD-302 Managing Spamming Events" )
  // Maximum Event Count -- Maximum number of events that can be raised in a given maximum event period
  integer_get_func_t  get_maximum_event_count;
  // Maximum Event Period -- Given time period that maximum event count can occour within (seconds)
  integer_get_func_t  get_maximum_event_period;

  /* TimeStamps */
  // Latest Delivered Event Time
  time_get_func_t  get_lastest_delivered_event_time;
  time_set_func_t  set_lastest_delivered_event_time;
  // Latest Recorded Event Time
  time_get_func_t  get_latest_recorded_event_time;
  time_set_func_t  set_latest_recorded_event_time;

  /* Alarm Clearing */
  // Alarm Clear -- (is a remote executable function call. No Getter or Setter)

  // Alarm Auto Clear
  bool_get_func_t get_alarm_auto_clear;

}
event_and_alarm_base_t;

#define DECLARE_EVENT_AND_ALARM_INSTANCE(instance, name, name_base)            \
static event_and_alarm_base_t name_base = {                                    \
  .instance_id              = instance,                                        \
  .get_value                = name ## _get_value,                              \
  .get_event_code           = name ## _Event_Code_get,                         \
  .get_event_type           = name ## _Event_Type_get,                         \
  .get_realtime             = name ## _Alarm_Realtime_get,                     \
  .get_alarm_state          = name ## _Alarm_State_get,                        \
  .set_alarm_state          = name ## _Alarm_State_set,                        \
  .get_set_threshold        = name ## _Alarm_Set_Threshold_get,                \
  .get_set_operator         = name ## _Alarm_Set_Operator_get,                 \
  .get_clear_threshold      = name ## _Alarm_Clear_Threshold_get,              \
  .get_clear_operator       = name ## _Alarm_Clear_Operator_get,               \
  .get_maximum_event_count  = name ## _Alarm_Maximum_Event_Count_get,          \
  .get_maximum_event_period = name ## _Alarm_Maximum_Event_Period_get,         \
  .get_lastest_delivered_event_time = name ## _Latest_Delivered_Event_Time_get,\
  .set_lastest_delivered_event_time = name ## _Latest_Delivered_Event_Time_set,\
  .get_latest_recorded_event_time  = name ## _Latest_Recorded_Event_Time_get,  \
  .set_latest_recorded_event_time  = name ## _Latest_Recorded_Event_Time_set,  \
  .get_alarm_auto_clear     = name ## _Alarm_Auto_Clear_get,                   \
  .payload_alarm_state_change_log_append  = name ## _Payload_Alarm_State_Change_Log_append,   \
  .payload_event_log_append               = name ## _Payload_Event_Log_append           \
};

/*******************************************************************************
*******************************************************************************/

// API
void event_and_alarm_base_initalise_instance(event_and_alarm_base_t *base_ptr, lwm2m_object_declaration_t *lwm2m_object_declaration_ptr);
void event_and_alarm_base_tick(uint32_t time_s);
uint8_t event_and_alarm_base_Latest_Payload_get(uint16_t instance, uint8_t **val, size_t *length);
uint8_t event_and_alarm_base_Latest_Payload_get_for_object_instance(lwm2m_object_link_t object, uint8_t **val, size_t *length);
void event_and_alarm_base_time_changed(void);

// Callbacks
uint8_t event_and_alarm_base_alarm_clear_execute(lwm2m_object_t *object, uint16_t instance, uint16_t resource_id);
uint8_t event_and_alarm_base_alarm_latest_payload_confirmed(lwm2m_object_t *object, uint16_t instance, uint16_t resource_id);
uint8_t event_and_alarm_base_alarm_Latest_Payload_confirmed_for_object_instance(lwm2m_object_link_t object);

#ifdef UNIT_TEST
void event_and_alarm_base_diagnostics(uint32_t time_s);
bool event_and_alarm_check_thresholds(event_and_alarm_base_t *base_ptr, const double sensor_value, const bool initial_alarm_state);
void event_and_alarm_base_rate_limit_increment_event_count(event_and_alarm_base_t *base_ptr);
bool event_and_alarm_base_rate_limit_check(int32_t time_s, event_and_alarm_base_t *base_ptr);
bool event_and_alarm_base_save_next_state(const uint16_t event_type, const bool state_changed, const bool next_state);
void event_and_alarm_base_tick_instance(int32_t time_s, event_and_alarm_base_t *base_ptr);
void event_and_alarm_base_tick_update_alarm_state(event_and_alarm_base_t *base_ptr, const uint32_t timestamp, const bool next_state, const bool notify_observer);



/* Payload Handling
********************/
bool event_and_alarm_base_payload_init(event_and_alarm_base_t *base_ptr);
bool event_and_alarm_base_payload_event_type_check(event_and_alarm_base_t *base_ptr);

// Payload Head
bool event_and_alarm_base_payload_head(event_and_alarm_base_t *base_ptr, const uint16_t logger_count);

// Payload Body
bool event_and_alarm_base_payload_alarm_current_state_generate(event_and_alarm_base_t *base_ptr, uint32_t timestamp, double new_sensor_value, bool new_alarm_state);
bool event_and_alarm_base_payload_alarm_state_change_log_generate(event_and_alarm_base_t *base_ptr, uint32_t timestamp, double new_sensor_value, bool new_alarm_state);
bool event_and_alarm_base_payload_event_log_generate(event_and_alarm_base_t *base_ptr, uint32_t timestamp, double new_sensor_value, bool new_alarm_state);

// Payload Footer
bool event_and_alarm_base_payload_footer(event_and_alarm_base_t *base_ptr);

/* Event Logging
*******************/
bool event_and_alarm_base_logger_clear(event_and_alarm_base_t *base_ptr);
bool event_and_alarm_base_logger_generate_payload(event_and_alarm_base_t *base_ptr);
bool event_and_alarm_base_logger_record(event_and_alarm_base_t *base_ptr, uint32_t timestamp, double new_sensor_value, bool new_alarm_state);


#endif


#endif /*__EVENT_AND_ALARM_BASE_H_*/
