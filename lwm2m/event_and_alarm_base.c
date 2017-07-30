/*
 * event_and_alarm_base.c
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 */

#include "event_and_alarm_base.h"
#include "log.h"
#include "lwm2m_object_base.h"
#include <assert.h>
#include <math.h> // fabs()
#include <led.h>  // led_set()

#include "lwm2m_object_options.h"


#ifndef DEBUG_EVENT_AND_ALARM_BASE
#undef log_debug
#define log_debug(...)
#endif


#define PAYLOAD_VALUE_COUNT_MAX   EVENT_AND_ALARM_BASE_PAYLOAD_VALUE_COUNT_MAX
#define PAYLOAD_BUFFER_SIZE_BYTES EVENT_AND_ALARM_BASE_PAYLOAD_BUFFER_SIZE_BYTES

/**-----------------------------------------------------------------------------
 *  @file
 *  @defgroup    event_and_alarm_base
 *  @ingroup     lwm2m_object_base
 *  @{
 *------------------------------------------------------------------------------
 */

#define EVENT_AND_ALARM_FLOAT_EQUALITY_EDGE_CASE_EPSILON 0.1


static event_and_alarm_base_t *event_and_alarms[EVENT_AND_ALARM_BASE_INSTANCES_MAX] = { 0 };
  ///<  event_and_alarms[] contains a list of objects.
  ///<  Index corresponds to objects registered by
  ///<    event_and_alarm_base_initalise_instance()

static uint16_t num_events_alarms = 0;
  ///< This variable keeps track of instances registered in event_and_alarms[]


/*******************************************************************************
********************************************************************************/

__static char* event_and_alarm_base_event_type_string(uint32_t var)
{
  switch (var)
  {
#define X(casevar,STRING) case(casevar): return STRING ;
    X(EVENT_AND_ALARM_EVENT_TYPE_DISABLED              , "Disabled");
    X(EVENT_AND_ALARM_EVENT_TYPE_ALARM_CURRENT_STATE   , "Current State");
    X(EVENT_AND_ALARM_EVENT_TYPE_ALARM_STATE_CHANGE_LOG, "Change Log");
    X(EVENT_AND_ALARM_EVENT_TYPE_EVENT_LOG             , "Event Log");
#undef X
    default: return "unknown";
  }
}

/*******************************************************************************
********************************************************************************/

/**-----------------------------------------------------------------------------
  # INTERNAL PAYLOAD HANDLING FUNCTIONS

  ## General Flow

  - event_and_alarm_base_payload_init() : On start of module
  - event_and_alarm_base_payload_head() :  On new mode
  - event_and_alarm_base_delivery_scheduler_body() :  Record Events/Alarms
    - event_and_alarm_base_payload_alarm_current_state_generate()
    - event_and_alarm_base_payload_alarm_state_change_log_generate()
    - event_and_alarm_base_payload_event_log_generate()
  - event_and_alarm_base_payload_footer() :  On ready to send to server

  @defgroup event_and_alarm_base_payload
  @{
------------------------------------------------------------------------------*/


/*******************************************************************************
  CBOR payload generation
*******************************************************************************/

__static bool event_and_alarm_base_payload_event_type_check(event_and_alarm_base_t *base_ptr)
{ // Check that current event type state matches the state when the payload was initialised or reset
  // This is because the payload during recording should not be switching modes.

  assert(base_ptr);
  uint32_t payload_event_type = 0;
  uint32_t event_type = 0;

  // Check Current Parameters
  base_ptr->get_event_type(base_ptr->instance_id, &event_type);

  // Check What Payload Was Initialised To
  payload_event_type = base_ptr->var.payload.event_type;

  if (event_type!=payload_event_type)
  {
    log_err("payload event type (%u) != current event (%u)", payload_event_type, event_type);
    return false; // Invalid due to event type mode switching between write
  }

  return true;  // Valid
}

__static bool event_and_alarm_base_payload_head(event_and_alarm_base_t *base_ptr)
{
  assert(base_ptr);
  cbor_stream_t *stream = &(base_ptr->var.payload.stream);
  uint32_t event_code = 0;
  uint32_t event_type = 0;

  // Check parameters
  base_ptr->get_event_code(base_ptr->instance_id, &event_code);
  base_ptr->get_event_type(base_ptr->instance_id, &event_type);

  cbor_clear(stream);

  base_ptr->var.payload.event_type = event_type;

  // Start of CBOR array (Mandatory for all payload types)
  cbor_serialize_array_indefinite(stream);

  switch (event_type)
  { // Setup the header

    case EVENT_AND_ALARM_EVENT_TYPE_ALARM_CURRENT_STATE:
    {
      // Not Required
    } break;

    case EVENT_AND_ALARM_EVENT_TYPE_ALARM_STATE_CHANGE_LOG:
    {
      cbor_serialize_int(stream, event_code);
      cbor_serialize_int(stream, event_type);

      // Opening Indefinite Array of State Change Logs
      cbor_serialize_array_indefinite(stream);
    } break;

    case EVENT_AND_ALARM_EVENT_TYPE_EVENT_LOG:
    {
      cbor_serialize_int(stream, event_code);
      cbor_serialize_int(stream, event_type);

      // Opening Indefinite Array of Events
      cbor_serialize_array_indefinite(stream);
    } break;

    // Unhandled
    default:
      break;
  }

  return true;
}


__static bool event_and_alarm_base_payload_alarm_current_state_generate(event_and_alarm_base_t *base_ptr, uint32_t timestamp, double new_sensor_value, bool new_alarm_state)
{
  assert(base_ptr);
  cbor_stream_t *stream = &(base_ptr->var.payload.stream);
  uint32_t event_code = 0;
  uint32_t event_type = 0;

  // Check parameters
  base_ptr->get_event_code(base_ptr->instance_id, &event_code);
  base_ptr->get_event_type(base_ptr->instance_id, &event_type);

  // Reset payload stream
  event_and_alarm_base_payload_head(base_ptr);

  // Generate payload again
  cbor_serialize_int(stream, event_code);
  cbor_serialize_int64_t(stream, timestamp);  // Must ensure that we can handle timestamp without overflow
  cbor_serialize_int(stream, event_type);
  cbor_serialize_int(stream, new_alarm_state);

  return true;
}

__static bool event_and_alarm_base_payload_alarm_state_change_log_generate(event_and_alarm_base_t *base_ptr, uint32_t timestamp, double new_sensor_value, bool new_alarm_state)
{
  assert(base_ptr);
  cbor_stream_t *stream = &(base_ptr->var.payload.stream);

  // Opening Indefinite Array (To deliniate each event)
  cbor_serialize_array_indefinite(stream);    // CBOR ARRAY [

  // First element is always a timestamp
  cbor_serialize_int64_t(stream, timestamp);  // Must ensure that we can handle timestamp without overflow

  // Append Values According To Event or Alarm Type
  if (base_ptr->payload_alarm_state_change_log_append)
  { // Append Function Handler Missing
    cbor_serialize_array_indefinite(stream);  // CBOR ARRAY [
    base_ptr->payload_alarm_state_change_log_append(stream, new_sensor_value, new_alarm_state);
    cbor_write_break(stream);                 // CBOR BREAK ]
  }

  // Closing Indefinite Array
  cbor_write_break(stream);                   // CBOR BREAK ]

  return true;
}

__static bool event_and_alarm_base_payload_event_log_generate(event_and_alarm_base_t *base_ptr, uint32_t timestamp, double new_sensor_value, bool new_alarm_state)
{
  assert(base_ptr);
  cbor_stream_t *stream = &(base_ptr->var.payload.stream);

  // Opening Indefinite Array (To deliniate each event)
  cbor_serialize_array_indefinite(stream);    // CBOR ARRAY [

  // First element is always a timestamp
  cbor_serialize_int64_t(stream, timestamp);  // Must ensure that we can handle timestamp without overflow

  // Append Values According To Event or Alarm Type
  if (base_ptr->payload_event_log_append)
  { // Handler Exist
    cbor_serialize_array_indefinite(stream);  // CBOR ARRAY [
    base_ptr->payload_event_log_append(stream, new_sensor_value, new_alarm_state);
    cbor_write_break(stream);                 // CBOR BREAK ]
  }

  // Closing Indefinite Array
  cbor_write_break(stream);                   // CBOR BREAK ]

  return true;
}

__static bool event_and_alarm_base_payload_footer(event_and_alarm_base_t *base_ptr)
{
  cbor_stream_t *stream = &(base_ptr->var.payload.stream);
  uint32_t event_type = 0;

  // Safer to stick to event that it was initialised during last reset
  event_type = base_ptr->var.payload.event_type;  switch (event_type)
  { // Setup the header
    case EVENT_AND_ALARM_EVENT_TYPE_ALARM_CURRENT_STATE:
    {
      // None required
    } break;
    case EVENT_AND_ALARM_EVENT_TYPE_ALARM_STATE_CHANGE_LOG:
    {
      // Closing Indefinite Array of State Change Logs
      cbor_write_break(stream);
    } break;
    case EVENT_AND_ALARM_EVENT_TYPE_EVENT_LOG:
    {
      // Closing Indefinite Array of Events
      cbor_write_break(stream);
    } break;

    // Unhandled
    default:
      break;
  };

  // Closing Overall Indefinite Array
  cbor_write_break(stream);

  return true;
}

__static bool event_and_alarm_base_payload_init(event_and_alarm_base_t *base_ptr)
{
  assert(base_ptr);
  uint8_t *buffer_ptr = base_ptr->var.payload.buffer;
  uint16_t buffer_size = PAYLOAD_BUFFER_SIZE_BYTES;

  // First character of the payload buffer is a binary constant value of 2
  base_ptr->var.payload.buffer[0] = 2; // 2 == Represents OMA-LwM2M CBOR format

  // CBOR content in payload starts after the first 8bit byte
  buffer_ptr++;
  buffer_size--;

  // This initialises the content of the instance's payload cbor stream
  cbor_init(&(base_ptr->var.payload.stream), buffer_ptr, buffer_size);
  return true;
}


/*******************************************************************************
  Latest Payload Generation And Event Logging
*******************************************************************************/

__static bool event_and_alarm_base_logger_clear(event_and_alarm_base_t *base_ptr)
{
  assert(base_ptr);

  /** Clear Payload (Via reinitalisation)
  *******************************************/
  event_and_alarm_base_payload_init(base_ptr);

  /** Clear Stream Logger
  *******************************************/
  base_ptr->var.payload.logger_read_pos   = 0;
  base_ptr->var.payload.logger_write_pos  = 0;
  base_ptr->var.payload.logger_count      = 0;
  base_ptr->var.payload.latest_event_time = 0;

  /** Clear Current State Log
  *******************************************/
  base_ptr->var.payload.logger_current_state_filled = false;
  base_ptr->var.payload.logger_current_state.event_type   = 0;
  base_ptr->var.payload.logger_current_state.timestamp    = 0;
  base_ptr->var.payload.logger_current_state.alarm_state  = 0;
  base_ptr->var.payload.logger_current_state.sensor_value = 0;

  return true;
}

__static bool event_and_alarm_base_logger_record(event_and_alarm_base_t *base_ptr, uint32_t timestamp, double new_sensor_value, bool new_alarm_state)
{
  assert(base_ptr);

  uint8_t logger_read_pos  = base_ptr->var.payload.logger_read_pos;
  uint8_t logger_write_pos = base_ptr->var.payload.logger_write_pos;
  uint8_t logger_count     = base_ptr->var.payload.logger_count;

  uint32_t event_type = 0;
  base_ptr->get_event_type(base_ptr->instance_id, &event_type);

  /** Record Current State (always)
  *******************************************/
  base_ptr->var.payload.logger_current_state_filled = true;
  base_ptr->var.payload.logger_current_state.event_type = event_type;
  base_ptr->var.payload.logger_current_state.timestamp = timestamp;
  base_ptr->var.payload.logger_current_state.alarm_state = new_alarm_state;
  base_ptr->var.payload.logger_current_state.sensor_value = new_sensor_value;

  /** Current State can be omitted from stream logger
  ****************************************************/
  if (EVENT_AND_ALARM_EVENT_TYPE_ALARM_CURRENT_STATE == event_type)
  {
    return true;
  }

  /** Record (overwrite old records if full)
  *******************************************/
  base_ptr->var.payload.logger[logger_write_pos].event_type    = event_type;
  base_ptr->var.payload.logger[logger_write_pos].timestamp     = timestamp;
  base_ptr->var.payload.logger[logger_write_pos].alarm_state   = new_alarm_state;
  base_ptr->var.payload.logger[logger_write_pos].sensor_value  = new_sensor_value;

  /** Update Position Reference
  *******************************************/
  /**

    ```
         :    [CASE1-FULL]    :    [CASE2-EMPTY]  :     [CASE3-NORMAL]  :
         :                    :                   :                     :
         :     Read==Write+1  :    Read==Write    :   Read==x           :
         :     V              :    V              :   V                 :
    |----:--------------------:-------------------:---------------------:---|...
         :    ^               :    ^              :     ^               :
         :    Write==Read-1   :    Write==Read    :     Write==y        :
    ```

  */


  if ( logger_read_pos == ((logger_write_pos+1)%PAYLOAD_VALUE_COUNT_MAX) )
  { // Full (CASE1)
    logger_read_pos++; // Old Records are overwritten
  }
  else
  { // Empty (CASE2) OR Normal (CASE3)
    logger_count++;
  }

  // CASE1, CASE2, CASE3 will always increment write
  logger_write_pos++;

  // Normalise to buffer positon
  logger_read_pos  = logger_read_pos  % PAYLOAD_VALUE_COUNT_MAX;
  logger_write_pos = logger_write_pos % PAYLOAD_VALUE_COUNT_MAX;


  /** Update Position Tracker/Stats
  *******************************************/
  base_ptr->var.payload.logger_read_pos   = logger_read_pos  ;
  base_ptr->var.payload.logger_write_pos  = logger_write_pos ;
  base_ptr->var.payload.logger_count      = logger_count     ;

#if EVENT_AND_ALARM_BASE_VERBOSE_DEBUG
  log_debug("logger{rpos: %d, wpos: %d, count:%d} | et:%d | ts:%lu | as:%d | sv: %lu",
              (int)base_ptr->var.payload.logger_read_pos   ,
              (int)base_ptr->var.payload.logger_write_pos  ,
              (int)base_ptr->var.payload.logger_count      ,
              (int)base_ptr->var.payload.logger[logger_read_pos].event_type     ,
              base_ptr->var.payload.logger[logger_read_pos].timestamp           ,
              (int)base_ptr->var.payload.logger[logger_read_pos].alarm_state    ,
              base_ptr->var.payload.logger[logger_read_pos].sensor_value
            );
#endif

  return true;
}


__static bool event_and_alarm_base_logger_generate_payload(event_and_alarm_base_t *base_ptr)
{ //  This is structured this way, so we don't need to change the payload
  //  generator too much, since the payload generator is already unit tested
  //  to be correctly working.
  uint32_t event_type = 0;
  event_and_alarm_base_log_t *log_entry_ptr = NULL;

  assert(base_ptr);

  base_ptr->get_event_type(base_ptr->instance_id, &event_type);


  /** Clear Payload (Via reinitalisation)
  *******************************************/
  event_and_alarm_base_payload_init(base_ptr);

  /** Generate Payload for Current State
  ***************************************/
  if (EVENT_AND_ALARM_EVENT_TYPE_ALARM_CURRENT_STATE == event_type)
  {
    // Generate as long as there has been one current state log entry ready
    if (base_ptr->var.payload.logger_current_state_filled)
    {
      log_entry_ptr = &base_ptr->var.payload.logger_current_state;
      base_ptr->var.payload.latest_event_time = log_entry_ptr->timestamp;
      event_and_alarm_base_payload_head(base_ptr);
      event_and_alarm_base_payload_alarm_current_state_generate(
          base_ptr,
          log_entry_ptr->timestamp,
          log_entry_ptr->sensor_value,
          log_entry_ptr->alarm_state
        );
      event_and_alarm_base_payload_footer(base_ptr);
      return true;
    }
    else
    { // No recorded event detected
      event_and_alarm_base_payload_init(base_ptr);  // Reset Payload Buffer
      return false;
    }
  }


  /** Generate Payload for Event Log and State Change Log
  ********************************************************/
  {
    uint16_t logger_read_pos  = base_ptr->var.payload.logger_read_pos;
    uint16_t logger_count     = base_ptr->var.payload.logger_count;
    uint16_t logger_pos       = 0;

    if (logger_count == 0)
    { // Logger is empty
      return false;
    }

    // Head
    event_and_alarm_base_payload_head(base_ptr);

    // Body
    for (uint16_t i=0 ; i < logger_count ; i++)
    { // Scan though logger

      // Get Entry
      logger_pos = (logger_read_pos + i) % PAYLOAD_VALUE_COUNT_MAX;
      log_entry_ptr = &base_ptr->var.payload.logger[logger_pos];

      base_ptr->var.payload.latest_event_time = log_entry_ptr->timestamp;
#if EVENT_AND_ALARM_BASE_VERBOSE_DEBUG
      // Verbose log
      log_debug("%d: (stream_pos: %d)(logread{.pos: %d, .count:%d, .pos:%d}) | et:%d | ts:%lu | as:%d | sv: %lu",
                  (int) i,
                  (int) base_ptr->var.payload.stream.pos,
                  (int) logger_read_pos,
                  (int) logger_count,
                  (int) logger_pos,
                  (int) log_entry_ptr->event_type,
                  log_entry_ptr->timestamp,
                  (int) log_entry_ptr->alarm_state,
                  log_entry_ptr->sensor_value
                );
#endif

      // How to handle this entry
      switch (log_entry_ptr->event_type)
      {
        case EVENT_AND_ALARM_EVENT_TYPE_ALARM_CURRENT_STATE:
        {
          // Skip
        } break;
        case EVENT_AND_ALARM_EVENT_TYPE_ALARM_STATE_CHANGE_LOG:
        {
          if (log_entry_ptr->event_type == event_type)
          { // Include Entry if is part of the same event type mode
            event_and_alarm_base_payload_alarm_state_change_log_generate(
                base_ptr,
                log_entry_ptr->timestamp,
                log_entry_ptr->sensor_value,
                log_entry_ptr->alarm_state
              );
          }
        } break;
        case EVENT_AND_ALARM_EVENT_TYPE_EVENT_LOG:
        {
          if (log_entry_ptr->event_type == event_type)
          { // Include Entry if is part of the same event type mode
            event_and_alarm_base_payload_event_log_generate(
                base_ptr,
                log_entry_ptr->timestamp,
                log_entry_ptr->sensor_value,
                log_entry_ptr->alarm_state
              );
          }
        } break;
        default:
        { // No handler for this
          log_err("logger entry has corrupted event_type");
          event_and_alarm_base_payload_init(base_ptr);  // Reset Payload Buffer
          return false;
        } break;
      }

    }

    // Footer
    event_and_alarm_base_payload_footer(base_ptr);

  }

  return true;
}


/// @}


/**-----------------------------------------------------------------------------
  ## THRESHOLD CHECK

        CASE 1
        ------------------------------------------------------ <--- Max Value
                   .--.         .--.          |
                  .    .       .    .         |<--- CASE1.WAS_ASSERTED    (Now checking for next clear threshold)
          ....set_threshold.................  |
                .       .    .       .        V
               .         .--.         .
             .                         .
          ....clear_threshold...............  ^
           .                             .    |<--- CASE1.WAS_DEASSERTED  (Now checking for next Set threshold)
          .                                   |
        ------------------------------------------------------ <-- 0

        CASE 2
        ------------------------------------------------------ <--- Max Value
          .                                   |
           .                             .    |<--- CASE2.WAS_DEASSERTED  (Now checking for next Set threshold)
          ....clear_threshold................ |
             .                         .      V
               .         .--.         .
                .       .    .       .
          ....set_threshold.................. ^
                  .    .       .    .         |<--- CASE2.WAS_ASSERTED.   (Now checking for next clear threshold)
                   .--.         .--.          |
        ------------------------------------------------------ <-- 0


------------------------------------------------------------------------------*/

// This will return true if the alarm should be asserted,
// false if the alarm should not be asserted.
__static bool event_and_alarm_check_thresholds(event_and_alarm_base_t *base_ptr, const double sensor_value, const bool initial_alarm_state)
{
  bool alarm_state = initial_alarm_state;
  double set_threshold_value = 0;
  double clear_threshold_value = 0;
  uint32_t set_operator   = 0;
  uint32_t clear_operator = 0;

  // Internal debugging
  bool case_tripped = false;
  bool case_tripped_edge = false;
  // Ignore warnings for this
  (void)(case_tripped);
  (void)(case_tripped_edge);

  // Obtain Hystersis Threshold Value
  base_ptr->get_set_threshold(    base_ptr->instance_id,    &set_threshold_value   );
  base_ptr->get_clear_threshold(  base_ptr->instance_id,    &clear_threshold_value );

  // Obtain Set/Clear Operator Value
  base_ptr->get_set_operator(     base_ptr->instance_id,    (uint32_t *)&set_operator   );
  base_ptr->get_clear_operator(   base_ptr->instance_id,    (uint32_t *)&clear_operator );

#if 0 // Parameter Diagnostics
  log_debug("event_and_alarm_check_thresholds: %u %u %u %u %u %u", alarm_state,
                      (unsigned int)(sensor_value*1000.0),
                      (unsigned int)(set_threshold_value*1000.0),
                      (unsigned int)(clear_threshold_value*1000.0),
                      set_operator,
                      clear_operator
                      );
  switch (set_operator)
  {
    case EVENT_AND_ALARM_GREATER_THAN_OR_EQUAL_TO:
      log_debug("set_operator: %u -- EVENT_AND_ALARM_GREATER_THAN_OR_EQUAL_TO", set_operator);
      break;
  case EVENT_AND_ALARM_LESS_THAN_OR_EQUAL_TO:
      log_debug("set_operator: %u -- EVENT_AND_ALARM_LESS_THAN_OR_EQUAL_TO ", set_operator);
      break;
    default:
      log_debug("set_operator: %u -- unknown", set_operator);
      break;
  }
  switch (clear_operator)
  {
    case EVENT_AND_ALARM_GREATER_THAN_OR_EQUAL_TO:
      log_debug("clear_operator: %u -- EVENT_AND_ALARM_GREATER_THAN_OR_EQUAL_TO", clear_operator);
      break;
    case EVENT_AND_ALARM_LESS_THAN_OR_EQUAL_TO:
      log_debug("clear_operator: %u -- EVENT_AND_ALARM_LESS_THAN_OR_EQUAL_TO", clear_operator);
      break;
    default:
      log_debug("clear_operator: %u -- unknown", clear_operator);
      break;
  }
#endif

  if (alarm_state) //asserted so need to check cleared threshold
  {
    if (clear_operator == EVENT_AND_ALARM_GREATER_THAN_OR_EQUAL_TO)
    { // CASE2.WAS_ASSERTED
      if (fabs(clear_threshold_value - sensor_value) < EVENT_AND_ALARM_FLOAT_EQUALITY_EDGE_CASE_EPSILON)
      { // If float is nearly equal, assume equal
        alarm_state = false;
        case_tripped_edge = true;
      }
      else if (sensor_value > clear_threshold_value)
      { // Clear Threshold
        alarm_state = false;
        case_tripped = true;
      }
#if EVENT_AND_ALARM_BASE_VERBOSE_DEBUG
      log_debug("CASE2.WAS_ASSERTED (%u,%u) (%u > %u) (%u <: %u)", case_tripped, case_tripped_edge, (unsigned int)(sensor_value*1000.0), (unsigned int)(clear_threshold_value*1000.0), (unsigned int)(fabs(clear_threshold_value - sensor_value)*1000.0), (unsigned int)(EVENT_AND_ALARM_FLOAT_EQUALITY_EDGE_CASE_EPSILON*1000.0));
#endif
    }
    else if (clear_operator == EVENT_AND_ALARM_LESS_THAN_OR_EQUAL_TO)
    { // CASE1.WAS_ASSERTED
      if (fabs(clear_threshold_value - sensor_value) < EVENT_AND_ALARM_FLOAT_EQUALITY_EDGE_CASE_EPSILON)
      { // If float is nearly equal, assume equal
        alarm_state = false;
        case_tripped_edge = true;
      }
      else if (sensor_value < clear_threshold_value)
      { // Clear Threshold
        alarm_state = false;
        case_tripped = true;
      }
#if EVENT_AND_ALARM_BASE_VERBOSE_DEBUG
      log_debug("CASE1.WAS_ASSERTED (%u,%u) (%u < %u) (%u <: %u)", case_tripped, case_tripped_edge, (unsigned int)(sensor_value*1000.0), (unsigned int)(clear_threshold_value*1000.0), (unsigned int)(fabs(clear_threshold_value - sensor_value)*1000.0), (unsigned int)(EVENT_AND_ALARM_FLOAT_EQUALITY_EDGE_CASE_EPSILON*1000.0));
#endif
    }
    else
    {
#if EVENT_AND_ALARM_BASE_VERBOSE_DEBUG
      log_debug("CASE3.WAS_ASSERTED --- Clear Operator Disabled");
#endif
    }
  }
  else
  { //clear need to check set threshold
    if (set_operator == EVENT_AND_ALARM_GREATER_THAN_OR_EQUAL_TO)
    { // CASE1.WAS_DEASSERTED
      if (fabs(set_threshold_value - sensor_value) < EVENT_AND_ALARM_FLOAT_EQUALITY_EDGE_CASE_EPSILON)
      { // If float is nearly equal, assume equal
        alarm_state = true;
        case_tripped_edge = true;
      }
      else if (sensor_value > set_threshold_value)
      { // Set Threshold
        alarm_state = true;
        case_tripped = true;
      }
#if EVENT_AND_ALARM_BASE_VERBOSE_DEBUG
      log_debug("CASE1.WAS_DEASSERTED (%u,%u) (%u > %u) (%u <: %u)", case_tripped, case_tripped_edge, (unsigned int)(sensor_value*1000.0), (unsigned int)(set_threshold_value*1000.0), (unsigned int)(fabs(set_threshold_value - sensor_value)*1000.0), (unsigned int)(EVENT_AND_ALARM_FLOAT_EQUALITY_EDGE_CASE_EPSILON*1000.0));
#endif
    }
    else if (set_operator == EVENT_AND_ALARM_LESS_THAN_OR_EQUAL_TO)
    { // CASE2.WAS_DEASSERTED
      if (fabs(set_threshold_value - sensor_value) < EVENT_AND_ALARM_FLOAT_EQUALITY_EDGE_CASE_EPSILON)
      { // If float is nearly equal, assume equal
        alarm_state = true;
        case_tripped_edge = true;
      }
      else if (sensor_value < set_threshold_value)
      { // Set Threshold
        alarm_state = true;
        case_tripped = true;
      }
#if EVENT_AND_ALARM_BASE_VERBOSE_DEBUG
      log_debug("CASE2.WAS_DEASSERTED (%u,%u) (%u < %u) (%u <: %u)", case_tripped, case_tripped_edge, (unsigned int)(sensor_value*1000.0), (unsigned int)(set_threshold_value*1000.0), (unsigned int)(fabs(set_threshold_value - sensor_value)*1000.0), (unsigned int)(EVENT_AND_ALARM_FLOAT_EQUALITY_EDGE_CASE_EPSILON*1000.0));
#endif
    }
    else
    {
#if EVENT_AND_ALARM_BASE_VERBOSE_DEBUG
      log_debug("CASE3.WAS_DEASSERTED --- Set Operator Disabled");
#endif
    }
  }

  return alarm_state;
}


/**-----------------------------------------------------------------------------
------------------------------------------------------------------------------*/

__static void event_and_alarm_base_rate_limit_increment_event_count(event_and_alarm_base_t *base_ptr)
{ // Event Count
  base_ptr->var.event_count++;
}

__static bool event_and_alarm_base_rate_limit_check(int32_t time_s, event_and_alarm_base_t *base_ptr)
{ // Check if too many messages has been sent

  // Rate Limiting
  uint32_t max_event_count = 0;
  uint32_t max_event_period_s = 0;
  uint32_t max_event_period_window_previous = 0;
  uint32_t max_event_period_window_current = 0;

  /*
    The day is divided into periods. During the period window. If there is too
    many alarm events triggered for this alarm instance. Then we want to stop
    sending any more alarm notifications. At least until the next period has
    incremented. A period number is defined as `uptime_seconds/period_seconds`
  */

  // Get parameter
  base_ptr->get_maximum_event_count(base_ptr->instance_id, &max_event_count);
  base_ptr->get_maximum_event_period(base_ptr->instance_id, &max_event_period_s);

  // Check if rate limit should be disabled
  // (Also avoids division by zero error for max_event_period_s)
  if ((0 == max_event_period_s)||(0 == max_event_count))
  { // Disable Rate Limit
    return false;
  }

  // Calculate Period Window
  max_event_period_window_previous = base_ptr->var.period_window;
  max_event_period_window_current  = time_s / max_event_period_s;

  // On transition between period window
  if (max_event_period_window_current != max_event_period_window_previous)
  { // Period window incremented
    log_debug("max event count reset (period window: %u)", max_event_period_window_current);
    base_ptr->var.period_window = max_event_period_window_current;
    base_ptr->var.event_count = 0;  // Reset Count
  }

  // Rate Limit exceeded?
  if (base_ptr->var.event_count >= max_event_count)
  { // Do not raise too many events within a given time period
    log_debug("rate limit exceeded (count:%u) >= (max:%u)", base_ptr->var.event_count, max_event_count);
    return true; // Rate Limit Exceeded
  }

  return false; // Under Rate Limit
}

/**-----------------------------------------------------------------------------
  # Recording Logic
------------------------------------------------------------------------------*/


__static bool event_and_alarm_base_save_next_state(const uint16_t event_type, const bool state_changed, const bool next_state)
{ // Returns if we can save this state
  bool record_falling_edge = true;
  // Black List
  switch (event_type)
  {
    // Allowed
    case EVENT_AND_ALARM_EVENT_TYPE_ALARM_CURRENT_STATE:
    {
    } break;
    case EVENT_AND_ALARM_EVENT_TYPE_ALARM_STATE_CHANGE_LOG:
    {
    } break;
    case EVENT_AND_ALARM_EVENT_TYPE_EVENT_LOG:
    {
      record_falling_edge = false;
    } break;

    // Disabled
    case EVENT_AND_ALARM_EVENT_TYPE_DISABLED:
    default:
    { // We do not use these state thus always disable it
      return false;
    } break;
  }

  // Only Record Event/Alarm Edge Transitions
  if (!state_changed)
  {
    return false;
  }

  // Rising or falling edge behaviour
  if (next_state)
  { // Rising
      return true;
  }
  else
  { // Falling
    if (record_falling_edge)
    {
      return true;
    }
  }

  return false;
}



__static void event_and_alarm_base_tick_update_alarm_state(event_and_alarm_base_t *base_ptr, const uint32_t timestamp, const bool next_state, const bool notify_observer)
{
  log_debug("alarm state saved:: notify:%d ts:%lu state:%d", (int) notify_observer, (long unsigned) timestamp, (int) next_state);

  /// Consider DDD-301 dilivery mode at a later time in lwm2m infrastructure
  base_ptr->set_alarm_state(base_ptr->instance_id, next_state, notify_observer);

  // Record timestamp
  base_ptr->set_latest_recorded_event_time(base_ptr->instance_id, timestamp, notify_observer);
}


/**-----------------------------------------------------------------------------
    # Update Tick handler
------------------------------------------------------------------------------*/

__static void event_and_alarm_base_tick_instance(int32_t time_s, event_and_alarm_base_t *base_ptr)
{ //this function is called for each instance of the event and alarm object each tick
  bool next_state = false;
  bool current_state = false;
  bool state_changed = false;
  bool save_next_state = false;
  uint32_t event_type = EVENT_AND_ALARM_EVENT_TYPE_DISABLED;
  double sensor_value = 0.0;
  bool realtime = false;
  // Pointer Guard
  assert(base_ptr);

  // Check Event Type
  base_ptr->get_event_type(base_ptr->instance_id, &event_type);

  switch (event_type)
  {
    // Allowed
    case EVENT_AND_ALARM_EVENT_TYPE_ALARM_CURRENT_STATE:
    case EVENT_AND_ALARM_EVENT_TYPE_ALARM_STATE_CHANGE_LOG:
    case EVENT_AND_ALARM_EVENT_TYPE_EVENT_LOG:
      break;

    // Disabled
    case EVENT_AND_ALARM_EVENT_TYPE_DISABLED:
      return; // Escape and do nothing since this instance is disabled
      break;

    // Unhandled
    default:
      return; // Escape and do nothing due to unhandled mode
      break;
  }

  // Obtain Current Sensor Value
  sensor_value = base_ptr->get_value(base_ptr->instance_id);


  /* Get Current State And Params
  ************************************/
  base_ptr->get_realtime(base_ptr->instance_id, &realtime);
  base_ptr->get_alarm_state(base_ptr->instance_id, &current_state);

  /* Evaluate Alarm State
  ************************************/
  next_state = event_and_alarm_check_thresholds(base_ptr, sensor_value, current_state);
  state_changed = current_state != next_state;

  /* Check if this transition can be recorded
  ********************************************/
  save_next_state = event_and_alarm_base_save_next_state(event_type, state_changed, next_state);

  /* Setting States and Notifying Server
  ****************************************/
  if (save_next_state)
  { // This event/alarm transition is recorded
    bool network_rate_limited = event_and_alarm_base_rate_limit_check(time_s, base_ptr);
    bool notify_observer = false;

    /* Show Next Saved State*/
    log_debug("save req: time:%lu, sensor:%lu/100, (prev:%s -> now:%s) => %s %s%s%s| %s",
                (long unsigned) time_s,
                (long unsigned) (sensor_value*100),
                current_state ? "SET":"CLEAR",
                next_state ? "SET":"CLEAR",
                state_changed ? "CHANGED":"NO_CHANGE",
                network_rate_limited ? "| RATE LIMITED ":"",
                realtime ? "| REALTIME ":"",
                notify_observer ? "| NOTIFY OBSERVER ":"",
                event_and_alarm_base_event_type_string(event_type)
              );

    /* Check if network related actions are allowed to occour */
    if (!network_rate_limited)
    {
      // If this transition is recorded or notified thus should count this event
      event_and_alarm_base_rate_limit_increment_event_count(base_ptr);

      // Record events for the delivery scheduler
      event_and_alarm_base_logger_record(base_ptr, time_s, sensor_value, next_state);

      // Allowed
      notify_observer = realtime;
    }

    // Save alarm state
    event_and_alarm_base_tick_update_alarm_state(base_ptr, time_s, next_state, notify_observer);

#ifdef EVENT_AND_ALARM_BASE_VISUAL_LED_ALARM_INDICATION_DEBUG
    // Display LED
    led_set(0, next_state);
#endif

  }
}

#ifdef UNIT_TEST
/**-----------------------------------------------------------------------------
  # Diagnostic Print
------------------------------------------------------------------------------*/
__static void event_and_alarm_base_diagnostics_instance(int32_t time_s, event_and_alarm_base_t *base_ptr)
{ // This function is called on every instance. It prints out the current state
  // to serial debug
  double    set_threshold_value = 0;
  double    clear_threshold_value = 0;
  uint32_t  set_operator = 0;
  uint32_t  clear_operator = 0;
  uint32_t  max_event_count = 0;
  uint32_t  max_event_period_s = 0;
  uint32_t  event_code = 0;
  uint32_t  event_type = 0;
  bool      current_state = 0;
  double    sensor_value = 0;

  sensor_value = base_ptr->get_value( base_ptr->instance_id );
  base_ptr->get_set_threshold(        base_ptr->instance_id, &set_threshold_value   );
  base_ptr->get_clear_threshold(      base_ptr->instance_id, &clear_threshold_value );
  base_ptr->get_set_operator(         base_ptr->instance_id, (uint32_t *)&set_operator   );
  base_ptr->get_clear_operator(       base_ptr->instance_id, (uint32_t *)&clear_operator );
  base_ptr->get_maximum_event_count(  base_ptr->instance_id, &max_event_count);
  base_ptr->get_maximum_event_period( base_ptr->instance_id, &max_event_period_s);
  base_ptr->get_event_code(           base_ptr->instance_id, &event_code);
  base_ptr->get_event_type(           base_ptr->instance_id, &event_type);

  base_ptr->get_alarm_state(base_ptr->instance_id, &current_state);

  log_raw(": AlarmD: ecode:%lu | %s | ALARM: %s | sens:%lu | set(thr:%lu,opt:%lu) | clear(thr:%lu,opt:%lu) | max_evtcnt:%lu max_evntperod:%lu \r\n",
      event_code,
      event_and_alarm_base_event_type_string(event_type),
      current_state ? "SET" : "UNSET",
      (long unsigned) sensor_value,
      (long unsigned) set_threshold_value,
      (long unsigned) set_operator,
      (long unsigned) clear_threshold_value,
      (long unsigned) clear_operator,
      (long unsigned) max_event_count,
      (long unsigned) max_event_period_s
    );
}
#endif

/*******************************************************************************
  # EXTERNAL FACING INTERFACE
*******************************************************************************/

void event_and_alarm_base_initalise_instance(event_and_alarm_base_t *base_ptr, lwm2m_object_declaration_t *lwm2m_object_declaration_ptr)
{
  log_info("Initialising event and alarm object %d", num_events_alarms);
  assert(num_events_alarms < EVENT_AND_ALARM_BASE_INSTANCES_MAX - 1);

  //check that manadatory functions are provided
  assert(base_ptr->payload_alarm_state_change_log_append  != 0);
  assert(base_ptr->payload_event_log_append               != 0);

  event_and_alarms[num_events_alarms++] = base_ptr;

  event_and_alarm_base_payload_init(base_ptr);
  event_and_alarm_base_logger_clear(base_ptr);

  // Register Object Specific Callbacks
  lwm2m_object_declaration_ptr->resources[14].confirmed = event_and_alarm_base_alarm_latest_payload_confirmed;
}

void event_and_alarm_base_tick(uint32_t time_s)
{
  for (uint16_t i = 0; i < num_events_alarms; i++)
  {
    event_and_alarm_base_tick_instance(time_s, event_and_alarms[i]);
  }
}

uint8_t event_and_alarm_base_Latest_Payload_get(uint16_t instance, uint8_t **val, size_t *length)
{
  event_and_alarm_base_t *base_ptr = 0;

  // Guard
  if (!event_and_alarms[instance])
  { // Instance not found
    return COAP_404_NOT_FOUND; // TODO: Confirm that this is the right error code;
  }

  // Get Instance Pointer
  base_ptr = event_and_alarms[instance];  // Assumption: that instance id corresponds to position in event_and_alarms[]

  /** Payload Generation
  *******************************************/

  // Generate payload at buffer and return true if successful
  if ( !event_and_alarm_base_logger_generate_payload(base_ptr) )
  { // Failed possibly due to reading before recording
    *val = 0;
    *length = 0;
    return COAP_205_CONTENT;
  };

  /** Return Buffer Payload
  *******************************************/

  // Update Pointer to payload buffer
  *val = base_ptr->var.payload.buffer;

  // Payload Buffer Size is "lwm2m cbor type (1 byte)" + "cbor payload"
  *length = base_ptr->var.payload.stream.pos + 1;
  log_debug("length %d", *length);
  return COAP_205_CONTENT;
}

uint8_t event_and_alarm_base_Latest_Payload_get_for_object_instance(lwm2m_object_link_t object, uint8_t **val, size_t *length)
{
  for (int i = 0; i < EVENT_AND_ALARM_BASE_INSTANCES_MAX; i++)
  {
    if ((event_and_alarms[i] != 0) &&
        (event_and_alarms[i]->object_id == object.object_id) &&
        (event_and_alarms[i]->instance_id == object.instance_id))
    {
       event_and_alarm_base_Latest_Payload_get(i, val, length);
    }
  }
  return COAP_NO_ERROR;
}

#ifdef UNIT_TEST
void event_and_alarm_base_diagnostics(uint32_t time_s)
{
  for (uint16_t i = 0; i < num_events_alarms; i++)
  {
    event_and_alarm_base_diagnostics_instance(time_s, event_and_alarms[i]);
  }
}
#endif


/*******************************************************************************
  CALLBACKS
*******************************************************************************/

uint8_t event_and_alarm_base_alarm_clear_execute(lwm2m_object_t *object, uint16_t instance, uint16_t resource_id)
{
  bool next_state = false;
  event_and_alarm_base_t *base_ptr = 0;
  if (!event_and_alarms[instance])
  { // Instance not found
    return COAP_404_NOT_FOUND; // TODO: Confirm that this is the right error code;
  }

  // Get Instance Pointer
  base_ptr = event_and_alarms[instance];

  // Set Alarm State to false
  base_ptr->set_alarm_state(instance, next_state, true);

  //FIXME: check that this is the correct return code
  return COAP_204_CHANGED;
}

uint8_t event_and_alarm_base_alarm_latest_payload_confirmed(lwm2m_object_t *object, uint16_t instance, uint16_t resource_id)
{
  bool autoclear = false;
  bool next_state = false;
  event_and_alarm_base_t *base_ptr = 0;
  if (!event_and_alarms[instance])
  { // Instance not found
    return COAP_404_NOT_FOUND; // TODO: Confirm that this is the right error code;
  }

  // Get Instance Pointer
  base_ptr = event_and_alarms[instance];

  // Auto clear on delivery confirmation?
  base_ptr->get_alarm_auto_clear(instance, &autoclear);
  base_ptr->set_lastest_delivered_event_time(instance,  base_ptr->var.payload.latest_event_time, true);
  /** Auto Clearing Alarm
  *******************************************/
  if (autoclear)
  { // Where the alarm state is automatically cleared
    // once the delivery of the event data payload is complete

    // Set Alarm State to false
    base_ptr->set_alarm_state(instance, next_state, true);
  }

  /** Clear Payload (Via reinitalisation)
  *******************************************/
  /*
    TODO: Need to deal with potential race condition where the logger receieved
      data inbetween the last payload being sent and the payload confirmation.
      In production quality build, we would usually store the data in flash
      rather than ram.
  */
  event_and_alarm_base_payload_init(base_ptr); // warn: race condition may lead to loss of data

  /** Clear Stream Logger
  *******************************************/
  base_ptr->var.payload.logger_read_pos   = 0;
  base_ptr->var.payload.logger_write_pos  = 0;
  base_ptr->var.payload.logger_count      = 0;


  //FIXME: check that this is the correct return code
  return COAP_204_CHANGED;
}


uint8_t event_and_alarm_base_alarm_Latest_Payload_confirmed_for_object_instance(lwm2m_object_link_t object)
{
  for (int i = 0; i < EVENT_AND_ALARM_BASE_INSTANCES_MAX; i++)
  {
    if ((event_and_alarms[i] != 0) &&
        (event_and_alarms[i]->object_id == object.object_id) &&
        (event_and_alarms[i]->instance_id == object.instance_id))
    {
        event_and_alarm_base_alarm_latest_payload_confirmed(0, i, 2);
    }
  }
  return COAP_NO_ERROR;
}

void event_and_alarm_base_time_changed(void)
{
  for (uint16_t i = 0; i < EVENT_AND_ALARM_BASE_INSTANCES_MAX; i++)
  {
    if (event_and_alarms[i] != 0)
    {
      //TODO: need to reset rate limiting
    }
  }
}
