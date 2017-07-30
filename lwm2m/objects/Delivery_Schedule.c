/*
 * Delivery_Schedule.c
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 */
#include "lwm2m_object_options.h"
#include "Delivery_Schedule.h"
#include "Device_base.h"       // Device_UTC_Offset_get()
#include "data_delivery_base.h"
#include "utc_offset_parser.h" // utc_offset_parse()
#include "log.h"
#include <assert.h>
#include <stdlib.h> // rand()

#define DEFAULT_SCHEDULE_START_TIME          CONFIG_Delivery_Schedule_DEFAULT_SCHEDULE_START_TIME
#define DEFAULT_SCHEDULE_UTC_OFFSET          CONFIG_Delivery_Schedule_DEFAULT_SCHEDULE_UTC_OFFSET
#define DEFAULT_DELIVERY_FREQENCY            CONFIG_Delivery_Schedule_DEFAULT_DELIVERY_FREQENCY
#define DEFAULT_RANDOMISED_DELIVERY_WINDOW   CONFIG_Delivery_Schedule_DEFAULT_RANDOMISED_DELIVERY_WINDOW
#define DEFAULT_NUMBER_OF_RETRIES            CONFIG_Delivery_Schedule_DEFAULT_NUMBER_OF_RETRIES
#define DEFAULT_RETRY_PERIOD                 CONFIG_Delivery_Schedule_DEFAULT_RETRY_PERIOD


#ifndef DEBUG_DELIVERY_SCHEDULE
#undef log_debug
#define log_debug(...)
#endif

/**-----------------------------------------------------------------------------
 *  @file
 *  @defgroup    Delivery_Schedule
 *  @ingroup     lwm2m_object_base
 *  @{
 *------------------------------------------------------------------------------
 */

/**-----------------------------------------------------------------------------
  @brief Sends aggregated report once every transmission window
------------------------------------------------------------------------------*/

/// This is the object's global static structure.
static Delivery_Schedule_instance_variables_t instance_variables[MAX_Delivery_Schedule_INSTANCES];

/*******************************************************************************
  # INTERNAL FUNCTONS
*******************************************************************************/


#ifdef UNIT_TEST
/**-----------------------------------------------------------------------------
  Display Current State

  This is used as a diagnostic tool to understand what's going on behind
  this object

------------------------------------------------------------------------------*/
__static uint32_t Delivery_Schedule_Diagnostics(  uint16_t instance, uint32_t unix_epoch_time,
                                                  uint32_t *out_next_tx_s, uint32_t *out_next_win_s, uint32_t *out_next_retry_s,
                                                  uint32_t *retry_count,
                                                  bool *transmission_active,
                                                  bool *transmission_confirmed
                                                )
{
  // Returns number of seconds til next transmission
  uint32_t next_tx_s = 0;
  uint32_t next_win_s = 0;
  uint32_t next_retry_s = 0;
  uint32_t local_time_s = 0;

  /** Get User Data
  **********************/
  Delivery_Schedule_instance_variables_t *var_ptr = (Delivery_Schedule_instance_variables_t *) Delivery_Schedule_get_user_data(instance);
  assert(var_ptr);  // Pointer must be defined

  // Adjust UTC current time to the local time
  if (unix_epoch_time >= (var_ptr->UTC_offset_seconds) )
  { // Unix Time must always be larger than or equal to UTC offset
    local_time_s = unix_epoch_time + (var_ptr->UTC_offset_seconds) ;
  }

  /** Calculate Window
  **********************/
  uint32_t tx_trigger_time_s = 0;

  // Next Transmit Countdown
  tx_trigger_time_s = var_ptr->next_transmit_window_s + var_ptr->random_delay_s;
  if (local_time_s <= tx_trigger_time_s)
  { // Avoid integer underflow
    next_tx_s = tx_trigger_time_s - local_time_s;
  }

  // Next Window Countdown
  tx_trigger_time_s = var_ptr->next_transmit_window_s;
  if (local_time_s <= tx_trigger_time_s)
  { // Avoid integer underflow
    next_win_s = tx_trigger_time_s - local_time_s;
  }

  // Next Retry Countdown
  tx_trigger_time_s = var_ptr->next_retransmit_time_s;
  if (local_time_s <= tx_trigger_time_s)
  { // Avoid integer underflow
    next_retry_s = tx_trigger_time_s - local_time_s;
  }

  /** Output
  **********************/
  if (out_next_tx_s)          {*out_next_tx_s            = next_tx_s;                        };
  if (out_next_win_s)         {*out_next_win_s           = next_win_s;                       };
  if (out_next_retry_s)       {*out_next_retry_s         = next_retry_s;                     };
  if (retry_count)            {*retry_count              = var_ptr->retry_count;             };
  if (transmission_active)    {*transmission_active      = var_ptr->transmission_active     ? true : false; };
  if (transmission_confirmed) {*transmission_confirmed   = var_ptr->transmission_confirmed  ? true : false; };

  // Return Seconds to next transmit
  if (var_ptr->transmission_active)
  { // transmission active, return next retry countdown
    return next_retry_s;
  }
  else
  { // Waiting
    return next_tx_s;
  }

  return 0;
}
#endif



/**-----------------------------------------------------------------------------
  Random Window Delay Generation

  Given a transmission window defined in seconds. On each call, give out a
  random delay.

------------------------------------------------------------------------------*/
__static uint32_t generate_random_window_delay_s(uint32_t delay_window_s)
{
  return ( delay_window_s * rand() ) / RAND_MAX;
}

/**-----------------------------------------------------------------------------
  Send Report

  This function is called each time the scheduler has triggered a send report,
  i.e. periodically and for retries. This function must lookup which Delivery
  Objects it is associated with and then trigger them to generate and send their
  payloads.

------------------------------------------------------------------------------*/
void Delivery_Schedule_send_report(uint16_t instance, uint32_t unix_epoch_time)
{
  lwm2m_object_link_t schedule;
  schedule.object_id = Delivery_Schedule_get_lwm2m_object()->objID;
  schedule.instance_id = instance;
  data_delivery_base_process_schedule(schedule);
}

/**-----------------------------------------------------------------------------
  Delivery Schedule Main Tick/Update Function
------------------------------------------------------------------------------*/

/**
  The GET_INTERVAL_TIME() macro, is a helper macro that defines a method for
  setting the next interval to trigger. This will align to the nearest
  period.

  Psudocode breakdown of this macro:

  ```
  GET_INTERVAL_TIME(time, offset, period, num_intervals_ahead)
  {

    // Rounded increment of the time in terms of periods (via divisor)
    period_increment = ((time - offset) / period);
    period_increment_plus_extra += num_intervals_ahead;

    // Return the rounded period plus offset
    return period_increment_plus_extra * period + offset;
  }

  ```
*/
#define GET_INTERVAL_TIME(time, offset, period, num_intervals_ahead) \
   ((((time - offset) / period) + num_intervals_ahead) * period) + offset

__static void Delivery_Schedule_instance_tick(Delivery_Schedule_instance_variables_t *var_ptr, uint16_t  instance, uint32_t unix_epoch_time)
{
  assert(var_ptr);

  bool send_report = false;

  uint32_t  local_time_s = 0;

  // Parameters
  uint32_t  start_time = 0;                   ///< number of seconds past midnight for which this schedule commences
  int32_t   UTC_offset_seconds = 0;           ///< Time zone offset for the Schedule Start Time Resource for this Delivery Schedule instance
  uint32_t  delivery_freqency = 0;            ///< number of seconds between deliveries
  uint32_t  randomised_delivery_window = 0;   ///< randomisation widow in seconds for the triggering of the notification delivery to the LwM2M server
  uint32_t  number_of_retries = 0;            ///< number of retry attempts that should be attempted when the delivery notification is not successful
  uint32_t  retry_period = 0;                 ///< number of seconds between each application level retry

  // Obtain Parameters
  Delivery_Schedule_Schedule_Start_Time_get(instance, &start_time);
  UTC_offset_seconds = var_ptr->UTC_offset_seconds; // Precalcuated in Delivery_Schedule_changed()
  Delivery_Schedule_Delivery_Frequency_get(instance, &delivery_freqency);
  Delivery_Schedule_Randomised_Delivery_Window_get(instance, &randomised_delivery_window);
  Delivery_Schedule_Number_of_Retries_get(instance, &number_of_retries);
  Delivery_Schedule_Retry_Period_get(instance, &retry_period);

  /** UTC to Local Time
  *********************************/
  // Note: Will we need to deal with the UNIX 32bit Epoch problem in year 2038?

  // Adjust UTC current time to the local time
  if (unix_epoch_time >= UTC_offset_seconds)
  { // Unix Time must always be larger than or equal to UTC offset
    local_time_s = unix_epoch_time + UTC_offset_seconds;
  }
  else
  { // Do not process Delivery Schedule until unix_epoch_time is corrected (e.g. via network time)
    return;
  }

  /** Initalise Transmission Window
  *********************************/

  if ( !(var_ptr->initialised_schedule) )
  {
    var_ptr->initialised_schedule = true;

    // Reset to known state
    var_ptr->transmission_active = false;
    var_ptr->transmission_confirmed = false;
    var_ptr->next_transmit_window_s = 0;
    var_ptr->next_retransmit_time_s = 0;
    var_ptr->random_delay_s = 0;

    /** Set Initial Window
    ************************/

    if (start_time > delivery_freqency)
    {
      log_warn("delivery start time of %d was truncated for instance %d because it is greater than delivery frequency %d", start_time, instance, delivery_freqency);
      start_time = 0;
    }

    var_ptr->next_transmit_window_s = GET_INTERVAL_TIME(local_time_s, start_time, delivery_freqency, 1);

    /** Update Random Delay
    ************************/
    var_ptr->random_delay_s = generate_random_window_delay_s(randomised_delivery_window);


    log_debug("schedule init:: tx timestamp %lus", (long unsigned) var_ptr->next_transmit_window_s);
    log_debug("schedule init:: tx win in:    %4lumins (%2luhr)", (var_ptr->next_transmit_window_s - local_time_s)/(60), (var_ptr->next_transmit_window_s - local_time_s)/(3600) );
    log_debug("schedule init:: start_time:   %4lumins (%2luhr)", start_time/(60), start_time/(3600) );
    log_debug("schedule init:: random delay: %4lumins (%2luhr)", var_ptr->random_delay_s/60, var_ptr->random_delay_s/(3600) );


  }


  /** Check for next transmit window
  **********************************/
  // Check if we reached the next transmit window
  uint32_t tx_trigger_time_s = var_ptr->next_transmit_window_s + var_ptr->random_delay_s;
  if ( local_time_s >= tx_trigger_time_s )
  {
    uint32_t prev_random_delay_s = var_ptr->random_delay_s;
    (void) prev_random_delay_s; // Used by below log_debug()

    // Start Transmission Window and reset working variables for it
    var_ptr->transmission_active = true;
    var_ptr->next_retransmit_time_s = tx_trigger_time_s + retry_period;
    var_ptr->transmission_confirmed = false;
    var_ptr->retry_count = 0;

    // Setup Next Window, Find the timestamp of the next transmission window.
    var_ptr->next_transmit_window_s = GET_INTERVAL_TIME(local_time_s, start_time, delivery_freqency, 1);
    var_ptr->random_delay_s = generate_random_window_delay_s(randomised_delivery_window);

    log_debug("Transmitting... (delvFreq:%luhr)(rand: %lumins --> %lumins)(rand: %luhr --> %luhr)",
      delivery_freqency/(60*60),
      prev_random_delay_s/(60), var_ptr->random_delay_s/(60),
      prev_random_delay_s/(60*60), var_ptr->random_delay_s/(60*60)
      );

    // TX
    send_report = true; // SENDING
  }


  /** In transmission Window. Resend as needed
  *******************************************/

  // Check if message reached the observer
  if (var_ptr->transmission_active)
  { // transmission Mode Active

    if (!(var_ptr->transmission_confirmed) )
    { // Not yet confirmed

      if ( local_time_s >= (var_ptr->next_retransmit_time_s))
      { // Last transmission failed, resending

        if (var_ptr->retry_count < number_of_retries)
        { // Resend report

          var_ptr->next_retransmit_time_s += retry_period;
          var_ptr->retry_count++;

          // TX
          send_report = true; // SENDING
        }
        else
        { // Could not send report
          log_err("Max retry limit reached");
          var_ptr->transmission_active = false;
          var_ptr->retry_count = 0;
        }

      }

    }
    else
    { // Confirmed Reception
      var_ptr->transmission_active = false;
      var_ptr->retry_count = 0;
    }
  }

  if (send_report)
  {
    Delivery_Schedule_send_report(instance, unix_epoch_time);
  }

}

__static void Delivery_Schedule_object_initialise_instance(uint16_t instance_id)
{
  log_info("Initialising Delivery Schedule object with instance id of %u", instance_id);

  /** Register Instance Variables with base
  ******************************************/
  Delivery_Schedule_instance_variables_t *instance_variable = &instance_variables[Delivery_Schedule_get_index_for_instance(instance_id)];
  Delivery_Schedule_set_user_data(instance_id, instance_variable);

  /** Internal Instance Initialisation
  *************************************/

  // Clear and register
  memset(instance_variable, 0, sizeof(Delivery_Schedule_instance_variables_t));
  instance_variable->instance_id = instance_id;

  // Set default per "South East Water Default Delivery Schedule Configuration"
  Delivery_Schedule_Schedule_Start_Time_set(        instance_id,  DEFAULT_SCHEDULE_START_TIME       , false);
  Delivery_Schedule_Schedule_UTC_Offset_set(        instance_id,  DEFAULT_SCHEDULE_UTC_OFFSET       , sizeof(DEFAULT_SCHEDULE_UTC_OFFSET), false);
  Delivery_Schedule_Delivery_Frequency_set(         instance_id,  DEFAULT_DELIVERY_FREQENCY         , false);
  Delivery_Schedule_Randomised_Delivery_Window_set( instance_id,  DEFAULT_RANDOMISED_DELIVERY_WINDOW, false);
  Delivery_Schedule_Number_of_Retries_set(          instance_id,  DEFAULT_NUMBER_OF_RETRIES         , false);
  Delivery_Schedule_Retry_Period_set(               instance_id,  DEFAULT_RETRY_PERIOD              , false);

  // Parse the utc offset string
  Delivery_Schedule_changed(instance_id);
}


__static void Delivery_Schedule_object_reset_instance(uint16_t instance_id)
{
  log_info("Resetting Delivery Schedule Instance %u", instance_id);

  /** Get User Data
  **********************/
  Delivery_Schedule_instance_variables_t *var_ptr = (Delivery_Schedule_instance_variables_t *) Delivery_Schedule_get_user_data(instance_id);
  assert(var_ptr);  // Pointer must be defined
  assert(var_ptr->instance_id == instance_id);  // Should have been registered already

  /** Reset Transmission
  **********************/
  var_ptr->initialised_schedule = false;
}



/**-----------------------------------------------------------------------------
  Object Base Callback

  These are weakly linked via `__attribute__((weak))` in base file.

------------------------------------------------------------------------------*/

void Delivery_Schedule_changed(uint16_t instance)
{ // This is called by Delivery_Schedule_base.c if any resources has changed
  // We shall used this to reload certain calculated parameters
  // "UTC+10"

  const char *utc_offset_str = "";


  /** Get User Data
  **********************/
  Delivery_Schedule_instance_variables_t *var_ptr = (Delivery_Schedule_instance_variables_t *) Delivery_Schedule_get_user_data(instance);
  assert(var_ptr);  // Pointer must be defined
  assert(var_ptr->instance_id == instance);  // Should have been registered already


  /** Parsing UTC Offset
  **********************/

  // Locate valid/present utc offset string from Delivery Schedule or Device Object
  Delivery_Schedule_Schedule_UTC_Offset_get(instance, &utc_offset_str);
  if (utc_offset_str[0] == '\0')
  { // Empty String Detected
    Device_UTC_Offset_get(0, &utc_offset_str); // Get Device instance 0's UTC offset string
    if (utc_offset_str[0] == '\0')
    { // Empty String Detected
      var_ptr->UTC_offset_seconds = 0; // No UTC offset found
      return;
    }
  }

  // Parse UTC string and save result
  var_ptr->UTC_offset_seconds = utc_offset_parse(utc_offset_str);
  log_debug("UTC_offset_seconds: %ld <-- %s", var_ptr->UTC_offset_seconds, utc_offset_str);


  /** Reset Transmission
  **********************/
  Delivery_Schedule_object_reset_instance(instance);
}

/**-----------------------------------------------------------------------------
  Tick Call Back Function Adapter for Delivery_Schedule_foreach()

------------------------------------------------------------------------------*/
__static bool Delivery_Schedule_instance_tick_callback(uint16_t instance_id, void *data)
{ // Callback function for Delivery_Schedule_foreach()

  /** Process Parameter
  **********************/
  uint32_t *unix_epoch_time_ptr = (uint32_t *)data;


  /** Get User Data
  **********************/
  Delivery_Schedule_instance_variables_t *var_ptr = (Delivery_Schedule_instance_variables_t *) Delivery_Schedule_get_user_data(instance_id);
  assert(var_ptr);  // Pointer must be defined
  assert(var_ptr->instance_id == instance_id);  // Should have been registered already


  /** Process Instance
  **********************/
  Delivery_Schedule_instance_tick(var_ptr, instance_id, *unix_epoch_time_ptr);

  return true;  // No problems detected
}

__static bool Delivery_Schedule_instance_time_changed_callback(uint16_t instance_id, void *data)
{
  Delivery_Schedule_instance_variables_t *var_ptr = (Delivery_Schedule_instance_variables_t *) Delivery_Schedule_get_user_data(instance_id);
  assert(var_ptr);  // Pointer must be defined
  assert(var_ptr->instance_id == instance_id);  // Should have been registered already

  Delivery_Schedule_object_reset_instance(instance_id);
  return true;
}

/**-----------------------------------------------------------------------------
  # EXTERNAL INTERFACE
------------------------------------------------------------------------------*/

void Delivery_Schedule_confirm(uint16_t instance_id)
{ //called by the aggregator when its got a confirmation back from the server
  //for the payload message.

  /** Get User Data
  **********************/
  Delivery_Schedule_instance_variables_t *var_ptr = (Delivery_Schedule_instance_variables_t *) Delivery_Schedule_get_user_data(instance_id);
  assert(var_ptr);  // Pointer must be defined
  assert(var_ptr->instance_id == instance_id);  // Should have been registered already

  /** Process Instance
  **********************/
  if ( (var_ptr->transmission_active) && !(var_ptr->transmission_confirmed) )
  { // Was not yet confirmed
    var_ptr->transmission_confirmed = true;
  }
  else
  { // Not in transmission window, or it was already previously confirmed
    log_warn("Received tx confirmation. But not in TX window");
  }
}


//
// this function is called by the base code when a new instance has
// been created.
void Delivery_Schedule_instance_created(uint16_t instance_id)
{
  Delivery_Schedule_object_initialise_instance(instance_id);
}

void Delivery_Schedule_object_initialise(void)
{
  Delivery_Schedule_initialise();

  //create instance 0 per "South East Water Default Delivery Schedule Configuration"
  Delivery_Schedule_create_instance(0);
}

void Delivery_Schedule_tick(uint32_t unix_epoch_time)
{
  Delivery_Schedule_foreach( Delivery_Schedule_instance_tick_callback, (void *)&unix_epoch_time);
}

void Delivery_Schedule_time_changed(void)
{
  Delivery_Schedule_foreach( Delivery_Schedule_instance_time_changed_callback, 0);
}

void Delivery_Schedule_register(lwm2m_context_t *context)
{
  Delivery_Schedule_register_object(context);
  Delivery_Schedule_register_instance(0);
}
