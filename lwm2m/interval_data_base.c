/*
 * interval_data_base.c
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 */
/**-----------------------------------------------------------------------------

  This is the base object class for the interval data recording

  Refer to page 32 of "Digital Utility IoT LwM2M Technical Specification v0.7"
    for description of the resource fields.

  # Resource Description Summary

   - interval_period                    : Recording Intervals in Seconds
   - interval_start_offset              : number of seconds past midnight for which the first interval is recorded
   - interval_collection_start_time     : representing the time when the first interval was recorded on the device for the measurement channel
   - oldest_recorded_interval           : representing the oldest available interval on the device for this measurement channel. Interval times represent the end of the interval, not the beginning.
   - last_delivered_interval            : readable and writable resource used to represent the last interval delivered to the LwM2M server
   - latest_recorded_interval           : currently highest recorded interval on the device for the measurement channel
   - interval_delivery_midnight_aligned : indicate if data is delivered only to the previous midnight (1) or if part-day data can be delivered (0).
   - status                             : ?

  # Interval Data Object Resources:

  ## User Configuration (inputs from server):
    - interval_period : Frequency of Recording
    - interval_start_offset : When to first start recording
    - UTC_offset_seconds : UTC Timezone

  ## Timestamp Values (output to server)
    - interval_collection_start_time : When did this start to record?
    - oldest_recorded_interval : Oldest available record in buffer
    - last_delivered_interval : The "newest" record that was delivered previously to server
    - latest_recorded_interval : Newest record available in buffer

  ## Other
  * interval_delivery_midnight_aligned : do we only deliver up to the previous midnight, or all data ready to be delivered.

  ## Query Interval Values Recorded
    - Interval_Historical_Read_execute( First Interval, Last Interval)
    - `-->` Interval_Historical_Read_Payload : Return intervals within range

  ## Update Config
    - Interval_Change_Configuration_execute( Interval Period, Interval Start Offset, Interval UTC Offset)

------------------------------------------------------------------------------*/

#include "interval_data_base.h"
#include "log.h"
#include "lwm2m_object_base.h"
#include "utc_offset_parser.h" // utc_offset_parse()
#include "Device.h" // Device_UTC_Offset_get()
#include "parse_lwm2m_exe_arg.h"  // parse_lwm2m_exe_arg_parse()

#include <assert.h>
#include <math.h> // fabs()
#include <led.h>  // led_set()

#include "lwm2m_object_options.h"


#ifndef DEBUG_INTERVAL_DATA_BASE
#undef log_debug
#define log_debug(...)
#endif

#define PAYLOAD_VALUE_COUNT_MAX   INTERVAL_DATA_BASE_PAYLOAD_VALUE_COUNT_MAX
#define PAYLOAD_BUFFER_SIZE_BYTES INTERVAL_DATA_BASE_PAYLOAD_BUFFER_SIZE_BYTES

/**-----------------------------------------------------------------------------
 *  @file
 *  @defgroup    interval_data_base
 *  @ingroup     lwm2m_object_base
 *  @{
 *------------------------------------------------------------------------------
 */

#define EVENT_AND_ALARM_BASE_VERBOSE_DEBUG 0 // Verbose mode for debug messages

static interval_data_base_t *interval_data[INTERVAL_DATA_BASE_INSTANCES_MAX] = { 0 };
  ///<  interval_data[] contains a list of objects.
  ///<  Index corresponds to objects registered by
  ///<    interval_data_base_initalise_instance()


typedef struct interval_data_base_payload_ref_block_t
{ /* This is used to represent a found block */
  uint32_t timestamp;
  uint32_t interval_period;
  uint8_t  interval_count;
  uint32_t first_interval_index;
} interval_data_base_payload_ref_block_t;


/*******************************************************************************
*******************************************************************************/

/*******************************************************************************
  # INTERNAL DIAGNOSTICS FUNCTONS
*******************************************************************************/

#ifdef UNIT_TEST
/**-----------------------------------------------------------------------------
  Display Current State

  This is used as a diagnostic tool to understand what's going on behind
  this object. Is used by the unit test system to verify key internal system
  state of this object.

------------------------------------------------------------------------------*/
__static uint32_t interval_data_base_Diagnostics(
                    interval_data_base_t *base_ptr,
                    uint32_t unix_epoch_time,
                    int32_t  *out_UTC_offset_seconds,
                    uint32_t *out_next_interval_record_time_s,
                    uint32_t *out_local_time_s
                  )
{ // Returns number of seconds til next transmission
  uint32_t next_record_s = 0;
  uint32_t local_time_s = 0;

  /** Get User Data
  **********************/
  interval_data_base_variable_t *var_ptr = (interval_data_base_variable_t *) &(base_ptr->var);
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
  tx_trigger_time_s = var_ptr->next_interval_record_time_s;
  if (local_time_s <= tx_trigger_time_s)
  { // Avoid integer underflow
    next_record_s = tx_trigger_time_s - local_time_s;
  }

  /** Output
  **********************/

  if (out_UTC_offset_seconds)
  {
    *out_UTC_offset_seconds = var_ptr->UTC_offset_seconds;
  }

  if (out_next_interval_record_time_s)
  {
    *out_next_interval_record_time_s = var_ptr->next_interval_record_time_s;
  }

  if (out_local_time_s)
  {
    *out_local_time_s = local_time_s;
  }

  return next_record_s;
}
#endif


/**-----------------------------------------------------------------------------
  Convert from unix epoch to local time
------------------------------------------------------------------------------*/
__static bool interval_data_base_unix_epoch_to_local_time(interval_data_base_t *base_ptr, const uint32_t unix_epoch_time, uint32_t *local_time_s)
{ // Note: Will we need to deal with the UNIX 32bit Epoch problem in year 2038?
  assert(base_ptr);
  int32_t UTC_offset_seconds = base_ptr->var.UTC_offset_seconds;
  // Adjust UTC current time to the local time
  if (unix_epoch_time >= UTC_offset_seconds)
  { // Unix Time must always be larger than or equal to UTC offset
    *local_time_s = unix_epoch_time + UTC_offset_seconds;
  }
  else
  { // Do not process Delivery Schedule until unix_epoch_time is corrected (e.g. via network time)
    *local_time_s = unix_epoch_time;
    return false;
  }
  return true;
}

/*******************************************************************************
  # PAYLOAD GENERATION (Block Level)
*******************************************************************************/

__static bool interval_data_base_payload_init(interval_data_base_t *base_ptr)
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

__static bool interval_data_base_payload_head(interval_data_base_t *base_ptr, const uint32_t first_interval_timestamp, const uint32_t interval_time_period)
{
  assert(base_ptr);
  cbor_stream_t *stream = &(base_ptr->var.payload.stream);
  bool result = true;

  // Start of CBOR array (Mandatory for all payload types)
  result = cbor_serialize_array_indefinite(stream) ? result : false;
  result = cbor_serialize_uint64_t(stream, first_interval_timestamp) ? result : false;  // Timestamp serialisation needs the large version since it is more than 16bits
  result = cbor_serialize_uint64_t(stream, interval_time_period) ? result : false;      // Period may require more than 16bits if the interval sampling takes more than a day

  log_debug("head:: tstamp:%lu, tperiod:%lu", (long unsigned) first_interval_timestamp, (long unsigned) interval_time_period);

  // Start of CBOR interval array
  result = cbor_serialize_array_indefinite(stream) ? result : false;

  // We need to keep note of this variable
  base_ptr->var.payload.interval_time_period = interval_time_period;

  return result;  // False if failed (e.g. no buffer space)
}

__static bool interval_data_base_payload_add_interval(interval_data_base_t *base_ptr, const uint32_t input_interval_value)
{
  assert(base_ptr);
  cbor_stream_t *stream = &(base_ptr->var.payload.stream);
  bool result = true;

  // Payload State
  uint32_t interval_time_period = base_ptr->var.payload.interval_time_period;
  uint32_t interval_value = input_interval_value;

#if INTERVAL_DATA_BASE_VERBOSE_DEBUG
  log_debug("adding interval to payload: value:%lu stream(pos:%lu, size:%lu)", (long unsigned) input_interval_value, (long unsigned) stream->pos, (long unsigned) stream->size);
#endif

  /** Entry Open
  ****************/
  result = cbor_serialize_array_indefinite(stream) ? result : false; // Start of an Interval Entry

  /** Encode Value(s)
  ********************/
  /*
    Note: In other interval objects you will need to encode multiple values
      in one entry. What this means is you may need to have each object handle
      the buffer array instead.

      For now, we are assuming only one value for now.
  */

  // Depending on
  if (interval_time_period <= 1800)  // 30 mins or less
  { // Range 0-9999
    interval_value = (interval_value >= 9999) ? 9999 : interval_value;
  }
  else
  { // Range 0-99999
    interval_value = (interval_value >= 99999) ? 99999 : interval_value;
  }

  result = cbor_serialize_int(stream, interval_value) ? result : false;

  /** Entry Close
  ****************/
  result = cbor_write_break(stream) ? result : false; // End of an Interval Entry

  return result;  // False if failed (e.g. no space left)
}

__static bool interval_data_base_payload_footer(interval_data_base_t *base_ptr)
{
  assert(base_ptr);
  cbor_stream_t *stream = &(base_ptr->var.payload.stream);
  bool result = true;

  result = cbor_write_break(stream) ? result : false;
  result = cbor_write_break(stream) ? result : false;

  return result;  // False if failed to write footer (e.g. no buffer)
}

/*******************************************************************************
  # INTERVAL LOGGER

  Design Notes:
  To ensure ease of adjustment of timezones of the logs, internally all
  timestamps are done in UNIX epoch ( UTC+00 Offset). When transmitted
  to a remove server, the timestamp recorded is transformed as required.

  Extention Note:
  * For Battery Level Readings, you will need to take note that the payload
    includes two values rather than one value.

*******************************************************************************/


/**-----------------------------------------------------------------------------
  Logger Clear

  This clears the recording buffer

------------------------------------------------------------------------------*/
__static bool interval_data_base_logger_clear(interval_data_base_t *base_ptr)
{
  assert(base_ptr);

  log_debug("clearing interval log for %d", base_ptr->instance_id);
  /** Clear Payload (Via reinitalisation)
  *******************************************/
  interval_data_base_payload_init(base_ptr);

  /** Clear Stream Logger
  *******************************************/
  base_ptr->var.logger.read_pos   = 0;
  base_ptr->var.logger.write_pos  = 0;
  base_ptr->var.logger.count      = 0;
  base_ptr->var.interval_initialised = false;
  return true;
}

void interval_data_base_time_changed(void)
{
  for (uint16_t i = 0; i < INTERVAL_DATA_BASE_INSTANCES_MAX; i++)
  {
    if (interval_data[i] != 0)
    {
      interval_data_base_logger_clear(interval_data[i]);
    }
  }
}

/**-----------------------------------------------------------------------------
  Logger Statistics

------------------------------------------------------------------------------*/
__static bool interval_data_base_logger_stats(
                interval_data_base_t *base_ptr,
                uint32_t *latest_recorded_timestamp,
                uint32_t *oldest_recorded_timestamp
              )
{
  assert(base_ptr);
  uint32_t read_pos   = base_ptr->var.logger.read_pos;    // Head of circular buffer
  //uint32_t write_pos  = base_ptr->var.logger.write_pos;
  uint32_t count      = base_ptr->var.logger.count;

  if (latest_recorded_timestamp)
  { // Get Latest (Just Recently Recorded)
    uint32_t entry_pos  = 0;
    if (count == 0)
    { // No Timestamp Avaliable
      *latest_recorded_timestamp = 0;
    }
    else
    {
      entry_pos = read_pos + count - 1;
      entry_pos = entry_pos % PAYLOAD_VALUE_COUNT_MAX;
      *latest_recorded_timestamp = base_ptr->var.logger.entries[entry_pos].timestamp;
    }
  }

  if (oldest_recorded_timestamp)
  { // Get Latest (Get Oldest Timestamp in buffer)
    uint32_t entry_pos  = 0;
    if (count == 0)
    { // No Timestamp Avaliable
      *oldest_recorded_timestamp = 0;
    }
    else
    {
      entry_pos = read_pos;
      entry_pos = entry_pos % PAYLOAD_VALUE_COUNT_MAX;
      *oldest_recorded_timestamp = base_ptr->var.logger.entries[entry_pos].timestamp;
    }
  }

  return true;
}

/**-----------------------------------------------------------------------------
  Logger Record

------------------------------------------------------------------------------*/
__static bool interval_data_base_logger_record(interval_data_base_t *base_ptr, uint32_t unix_epoch_timestamp, uint32_t interval_period, double new_sensor_value)
{
  assert(base_ptr);

  uint8_t logger_read_pos  = base_ptr->var.logger.read_pos;
  uint8_t logger_write_pos = base_ptr->var.logger.write_pos;
  uint8_t logger_count     = base_ptr->var.logger.count;

  log_debug("Recording Interval: (UTC:%lu, IPeriod:%lu, SENSE:%lu)", (long unsigned) unix_epoch_timestamp, (long unsigned) interval_period, (long unsigned) new_sensor_value);


  /** Record (overwrite old records if full)
  *******************************************/
  base_ptr->var.logger.entries[logger_write_pos].timestamp     = unix_epoch_timestamp;
  base_ptr->var.logger.entries[logger_write_pos].sensor_value  = new_sensor_value;
  base_ptr->var.logger.entries[logger_write_pos].interval_period = interval_period;

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
  base_ptr->var.logger.read_pos   = logger_read_pos  ;
  base_ptr->var.logger.write_pos  = logger_write_pos ;
  base_ptr->var.logger.count      = logger_count     ;

  return true;
}


/**-----------------------------------------------------------------------------
  Logger Payload Search For Block

  This is used by interval_data_base_logger_generate_payload_historical()
  to search for entries to insert into a CBOR block for the Historical Payload.

  Design Note: This is required since in page 37, on "Latest Payload" in
                `Digital Utility IoT LwM2M Technical Specification v0.7`
                if there are missing intervals in the sequence, then the payload
                may consist of multiple blocks of the above serialised data
                (appended into a single binary opaque value), each block
                representing a continuous series of interval data.

                Thus the idea is that if a period is missed, then it will try
                to keep reading ahead, but in a new block.
                To minimise computation, I pass though the i loop value. This is
                so we don't duplicate scanning of datums that has already been
                evaluated.

  Inputs:
    - first_interval_timestamp_s : This defines the oldest record to search
        for. Setting this to 0 will disable this check.
    - last_interval_timestamp_s :  This defines the latest record to search
        for. Setting this to 0 will disable this check.
    - init_log_entry_scan_index : Initial log entry to check against

  Output:
    - oldest_timestamp : This is the oldest timestamp that was found (optional)
    - newest_timestamp : This is the newest timestamp that was found (optional)
    - log_entry_scan_index : last entry that was checked against

------------------------------------------------------------------------------*/
__static bool interval_data_base_logger_generate_payload_historical_search_block(
                  interval_data_base_t *base_ptr,
                  const uint32_t first_interval_timestamp_s,        ///< input  : number of seconds since Jan 1st, 1970 in the UTC time zone
                  const uint32_t last_interval_timestamp_s,         ///< input  : number of seconds since Jan 1st, 1970 in the UTC time zone
                  uint32_t *oldest_timestamp_s,                     ///< output : number of seconds since Jan 1st, 1970 in the UTC time zone
                  uint32_t *newest_timestamp_s,                     ///< output : number of seconds since Jan 1st, 1970 in the UTC time zone
                  /* Special Handling Of Inconsistent Period */
                  uint16_t *log_entry_scan_index,                   ///< input/output
                  bool     *incomplete_read,                        ///< output
                  interval_data_base_payload_ref_block_t *block_ref ///< output
                )
{
  interval_data_base_log_entries_t *log_entry_ptr = NULL;
  uint16_t logger_read_pos  = base_ptr->var.logger.read_pos;
  uint16_t logger_count     = base_ptr->var.logger.count;
  uint16_t logger_pos       = 0;

  uint32_t prev_entry_timestamp = 0;
  uint32_t expected_interval_period = 0;

  uint32_t entry_timestamp       = 0;
  uint32_t entry_interval_period = 0;

  bool     block_exist = false; /* At least one entry found */

  assert(log_entry_scan_index);
  assert(incomplete_read);
  assert(block_ref);

  block_ref->interval_count = 0; /* Interval Count of Zero means empty block */


  /** Scan though logger from Old to New datum
  *********************************************/
  uint16_t entry_index = 0;
  for (entry_index = *log_entry_scan_index ; entry_index < logger_count ; entry_index++)
  {

    /** Get Values **/

    // Get Entry
    logger_pos = (logger_read_pos + entry_index) % PAYLOAD_VALUE_COUNT_MAX; // Index
    log_entry_ptr = &base_ptr->var.logger.entries[logger_pos];

    // Get Entry Values
    entry_timestamp       = log_entry_ptr->timestamp;
    entry_interval_period = log_entry_ptr->interval_period;

#if INTERVAL_DATA_BASE_VERBOSE_DEBUG
    log_debug(" | %lu < (%lu) < %lu | (%u, %u)",
        first_interval_timestamp_s,
        entry_timestamp,
        last_interval_timestamp_s,
        (first_interval_timestamp_s >= entry_timestamp),
        (entry_timestamp >= last_interval_timestamp_s)
      );
#endif

    /** Interval Range Restrict **/
    /*
      This resticts entries in this block to between
        (first_interval < entry_timestamp < last_interval)

      Disable upper or lower range check by setting timestamp to 0 for each
    */
    if ((first_interval_timestamp_s != 0) && (first_interval_timestamp_s >= entry_timestamp))
    { // Skip Initial Old Entry
      continue;
    }
    else
    if ((last_interval_timestamp_s  != 0) && (entry_timestamp >= last_interval_timestamp_s))
    { // Skip This New Entry
      break;
    }

    /** Verify Validity Of Log Entry Timestamp **/
    if (block_exist)
    { /** This kicks in after the header and the first entry is appended **/
      if (entry_timestamp > prev_entry_timestamp)
      { /** Check if interval increment within acceptable range **/
        const int32_t calc_period = entry_timestamp - prev_entry_timestamp;
        const int32_t upper_valid_period = expected_interval_period + 2;
        const int32_t lower_valid_period = expected_interval_period - 2;
        if ( !((lower_valid_period < calc_period) && (calc_period < upper_valid_period)) )
        { /** Break to new block **/
          log_warn("interval skip detected. Incomplete Read Triggered.");
          *incomplete_read = true; // trip incompleate read
          break;
        }
      }
      else
      { /* Timestamp must always be increasing */
        log_err("log entry timestamp must always be increasing");
        return false;
      }

      prev_entry_timestamp = entry_timestamp;
    }

    /** Block Reference Construction **/

    if (!block_exist)
    { // Generate the payload header on first detected entry
      block_exist = true;

      // Setup interval block metadata
      block_ref->timestamp = entry_timestamp;
      block_ref->interval_period = entry_interval_period;
      block_ref->first_interval_index = entry_index;

      // Report this oldest timestamp found
      if (oldest_timestamp_s)
      {
        *oldest_timestamp_s = entry_timestamp;
      }

      // Init expected timestamp guard
      prev_entry_timestamp = entry_timestamp;
      expected_interval_period = entry_interval_period;
    }

    // Add Entry To Payload
    (block_ref->interval_count)++;
  } /* End of Loop */

  *log_entry_scan_index = entry_index;

  return true;
}



/**-----------------------------------------------------------------------------
  Logger Payload Search For Block

  This is used by interval_data_base_logger_generate_payload_historical()
  to generate a CBOR block based on block_ref data.

------------------------------------------------------------------------------*/
__static bool interval_data_base_logger_generate_payload_historical_append_block(
                  interval_data_base_t *base_ptr,
                  interval_data_base_payload_ref_block_t *block_ref /* Generate Payload */
                )
{

  uint16_t logger_read_pos  = base_ptr->var.logger.read_pos;
  bool     error = false;

  assert(base_ptr);
  assert(block_ref);

  /** Record Stream Pos For Reverting Broken Blocks
  ***************************************************/
  const size_t init_stream_pos = base_ptr->var.payload.stream.pos;

  error = interval_data_base_payload_head(base_ptr, block_ref->timestamp, block_ref->interval_period) ? error : true;

  /** Scan though logger from Old to New datum
  *********************************************/
  for (uint16_t entry_index = block_ref->first_interval_index ; entry_index < block_ref->interval_count ; entry_index++)
  {

    uint16_t logger_pos = (logger_read_pos + entry_index) % PAYLOAD_VALUE_COUNT_MAX; // Index

    interval_data_base_log_entries_t *log_entry_ptr = &base_ptr->var.logger.entries[logger_pos];

    // Get Entry Values
    uint32_t entry_sensor_value = (uint32_t) log_entry_ptr->sensor_value;

    // Add Entry To Payload
    error = interval_data_base_payload_add_interval(base_ptr, entry_sensor_value) ? error : true;
  }

  error = interval_data_base_payload_footer(base_ptr) ? error : true;

  if (error)
  { /* CBOR Block Write Failed */
    base_ptr->var.payload.stream.pos = init_stream_pos; // Revert to last known position
    log_err("Failed to write payload. No buffer?");
    return false;
  }

  return true;
}

/**-----------------------------------------------------------------------------
  Logger Payload Generation With Range

  Assumption:
    That interval period stored in the logger will be the same.
    If changed, then the information stored in the data buffer will need to be
    cleared. In a mature system, the device may be smarter enough to
    keep sending old data with the old period. Then in a new payload
    it will then send the new interval period.

  Design Note:
    I previously was trying to avoid CBOR specific functions in this function
    but had to add it in, because of the need to contain multiple block for
    disjointed intervals.

  Inputs:
    - first_interval_timestamp_s : This defines the oldest record to search
        for. Setting this to 0 will disable this check.
    - last_interval_timestamp_s :  This defines the latest record to search
        for. Setting this to 0 will disable this check.

  Output:
    - oldest_timestamp : This is the oldest timestamp that was found
    - newest_timestamp : This is the newest timestamp that was found

------------------------------------------------------------------------------*/
__static bool interval_data_base_logger_generate_payload_historical(
                  interval_data_base_t *base_ptr,
                  const uint32_t first_interval_timestamp_s,  ///< number of seconds since Jan 1st, 1970 in the UTC time zone
                  const uint32_t last_interval_timestamp_s,   ///< number of seconds since Jan 1st, 1970 in the UTC time zone
                  uint32_t *oldest_timestamp_s,               ///< number of seconds since Jan 1st, 1970 in the UTC time zone
                  uint32_t *newest_timestamp_s                ///< number of seconds since Jan 1st, 1970 in the UTC time zone
                )
{
  cbor_stream_t *stream = &(base_ptr->var.payload.stream);
  uint16_t logger_count = base_ptr->var.logger.count;

  bool result = false;
  bool cbor_result = false;
  bool error = false;

  if (logger_count == 0)
  { // Logger is empty
    log_err("logger empty");
    return false;
  }


  /** Clear Payload (Via reinitalisation)
  *******************************************/
  error = interval_data_base_payload_init(base_ptr) ? error : true;
  if (error)
  {
    log_err("Cannot Init Payload");
    return false;
  }


  /** Scan Log Entries For As Many Interval Blocks
  *************************************************/
  uint16_t log_entry_scan_index = 0;
  uint8_t  block_count = 0;
  interval_data_base_payload_ref_block_t block_metadata[5];
  const uint8_t block_metadata_max = sizeof(block_metadata)/sizeof(block_metadata[0]);

  for (int block_index = 0 ; block_index < block_metadata_max ; block_index++)
  { /* Scan and request a block */
    bool incomplete_read = false;
    uint32_t block_oldest_timestamp_s;
    uint32_t block_newest_timestamp_s;

    result = interval_data_base_logger_generate_payload_historical_search_block(
                base_ptr, first_interval_timestamp_s, last_interval_timestamp_s,  /* Inputs */
                &block_oldest_timestamp_s, &block_newest_timestamp_s, /* Outputs */
                /* Special Handling Of Inconsistent Period */
                &log_entry_scan_index,
                &incomplete_read,
                &(block_metadata[block_index])
              );

    /* Stop loop on fatal error */
    if (!result) return false;

    /* Break Loop If Empty */
    if (block_metadata[block_index].interval_count == 0) break;

    /* If interval found in block then we have plus one valid block */
    block_count++;

    /* Initial Block Behaviour*/
    if (block_index == 0)
    { // Record oldest timestamp captured
      if (oldest_timestamp_s)
      {
        *oldest_timestamp_s = block_oldest_timestamp_s;
      }
    }

    /* All block behaviour */
    if (newest_timestamp_s)
    { // Record newest valid timestamp that was captured
      *newest_timestamp_s = block_newest_timestamp_s;
    }

    /* Check if completed */
    if (!incomplete_read) break;

    log_debug("gen extra block %d (%u, %u)", block_index, log_entry_scan_index, entry_written_count);
  }

  /** Payload Container Start
  *******************************************/
  cbor_result = cbor_serialize_array(stream, 2 + block_count );
  if (!cbor_result)
  {
    log_err("Could not open payload container");
    return false;
  }

  cbor_result = cbor_serialize_int(stream, base_ptr->object_id);
  if (!cbor_result)
  {
    log_err("Could not write object id");
    return false;
  }

  cbor_result = cbor_serialize_int(stream, base_ptr->instance_id);
  if (!cbor_result)
  {
    log_err("Could not write object id");
    return false;
  }

  for (int block_index = 0 ; block_index < block_count ; block_index++)
  { /* Scan and construct a CBOR block for the payload */
    cbor_result = interval_data_base_logger_generate_payload_historical_append_block(
                      base_ptr,
                      &(block_metadata[block_index])
                    );

    if (!cbor_result) return false;
  }

  return true;
}

/*******************************************************************************
  # INTERNAL FUNCTONS
*******************************************************************************/


/**-----------------------------------------------------------------------------
  Tick Function

  This is a background service function that monitors and trigger a record
    action periodically.

  Design Note:
    While the logger may record exact timestamp. The transmitted payload
    timestamp is always in increment of 30minutes. This is because it is not
    required to have such deep level of accuracy for the payload considering
    the timing drift of the device in typical usage.

    Note also that a production implementation would likely be using a timer
    and sleeping between interval recording rather than running a tick loop
    like we have here.
------------------------------------------------------------------------------*/
#define GET_INTERVAL_TIME(time, offset, period, num_intervals_ahead) \
   ((((time - offset) / period) + num_intervals_ahead) * period) + offset

__static void interval_data_base_tick_instance(int32_t unix_epoch_time, interval_data_base_t *base_ptr)
{
  // Pointer Guard
  assert(base_ptr);
  interval_data_base_variable_t *var_ptr = &(base_ptr->var);

  // Local Variables
  uint16_t  instance = base_ptr->instance_id;
  uint32_t  local_time_s = 0;


  // Parameter Local Variable
  uint32_t interval_period        = 0; ///< Recording Intervals in Seconds
  uint32_t interval_start_offset  = 0; ///< number of seconds past midnight for

  //are we enabled?
  uint32_t status;
  base_ptr->get_Status(base_ptr->instance_id, &status);
  if (status != 0)
  {
    return;
  }
  ///< which the first interval is recorded

  // Parameters Getter
  base_ptr->get_Interval_Period       (instance, &interval_period       );
  base_ptr->get_Interval_Start_Offset (instance, &interval_start_offset );


  /** UTC to Local Time
  *********************************/
  // Note: Will we need to deal with the UNIX 32bit Epoch problem in year 2038?

  if(!interval_data_base_unix_epoch_to_local_time(base_ptr, unix_epoch_time, &local_time_s))
  { // Do not process Delivery Schedule until unix_epoch_time is corrected (e.g. via network time)
    return;
  }

  /** Initialisation
  *********************************/
  if (!(var_ptr->interval_initialised))
  { // Not Initialised
    var_ptr->interval_initialised = true;

    if ((interval_start_offset < interval_period) && (local_time_s > interval_start_offset))
    {
      var_ptr->next_interval_record_time_s = GET_INTERVAL_TIME(local_time_s, interval_start_offset, interval_period, 1);
    }
    else
    {
      log_err("Invalid interval start offset, ignoring the offset");
      var_ptr->next_interval_record_time_s = GET_INTERVAL_TIME(local_time_s, 0, interval_period, 1);
    }
    // Record Initial Start Time
    base_ptr->set_Interval_Collection_Start_Time(instance, var_ptr->next_interval_record_time_s, true);
    log_debug("instance: %d, local_time_s: %ld interval time: %ld", base_ptr->instance_id, local_time_s, var_ptr->next_interval_record_time_s);

  }

  /** Periodic Record
  *********************************/
  if (local_time_s >= var_ptr->next_interval_record_time_s)
  {
    log_debug("instance: %d, local_time_s: %ld interval time: %ld", base_ptr->instance_id, local_time_s, var_ptr->next_interval_record_time_s);

    double sensor_value = base_ptr->get_value(instance);
    uint32_t latest_recorded_timestamp = 0;
    uint32_t oldest_recorded_timestamp = 0;

#ifdef INTERVAL_DATA_BASE_LED_VISUAL_INDICATOR
  led_set(0, true);
#endif

    // Setup Next Record Trigger
    if ((interval_start_offset < interval_period) && (local_time_s > interval_start_offset))
    {
      var_ptr->next_interval_record_time_s = GET_INTERVAL_TIME(local_time_s, interval_start_offset, interval_period, 1);
    }
    else
    {
       log_err("Invalid interval start offset, ignoring the offset");
       var_ptr->next_interval_record_time_s = GET_INTERVAL_TIME(local_time_s, 0, interval_period, 1);
    }

    // Record Current Time and update statistics
    interval_data_base_logger_record(base_ptr, GET_INTERVAL_TIME(unix_epoch_time, interval_start_offset, interval_period, 0), interval_period, sensor_value);
    interval_data_base_logger_stats(base_ptr, &latest_recorded_timestamp, &oldest_recorded_timestamp);

    // Set base parameter to report oldest and latest interval
    if (oldest_recorded_timestamp != 0)
    {
      base_ptr->set_Oldest_Recorded_Interval(instance, oldest_recorded_timestamp, true);
    }

    if (latest_recorded_timestamp != 0)
    {
      base_ptr->set_Latest_Recorded_Interval(instance, latest_recorded_timestamp, true);
    }

    if (var_ptr->next_interval_record_time_s == oldest_recorded_timestamp)
    { // Check that the
      log_err("timestamp mismatch in buffer");
    }

#ifdef INTERVAL_DATA_BASE_LED_VISUAL_INDICATOR
  led_set(0, false);
#endif
  }

}


void interval_data_base_Latest_Payload_build(interval_data_base_t *base_ptr, uint16_t instance, uint8_t **val, size_t *length)
{
  assert(base_ptr);
  time_t last_delivered_interval = 0;
  base_ptr->get_Last_Delivered_Interval(instance, &last_delivered_interval);

  time_t latest_recorded_interval = 0;
  base_ptr->get_Latest_Recorded_Interval(instance, &latest_recorded_interval);

  log_debug("interval latest payload read (first: %lu, last: %lu)",
      (long unsigned) last_delivered_interval,
      (long unsigned) latest_recorded_interval
    );

  /* Execute Command */
  interval_data_base_logger_generate_payload_historical(
              base_ptr,
              last_delivered_interval,
              latest_recorded_interval,
              &(base_ptr->var.last_interval_payload.oldest_timestamp_s),
              &(base_ptr->var.last_interval_payload.newest_timestamp_s)
            );


  // Transmit the payload of which the size if the CBOR payload plus 1 byte,
  // indicating that this is a CBOR encoded payload
  // (Notify has been set to true)

  log_debug("built payload with length %d", 1 + base_ptr->var.payload.stream.pos);
  base_ptr->var.last_interval_payload.confirmation_required = (base_ptr->var.payload.stream.pos != 0);
  *val = base_ptr->var.payload.buffer;
  *length = 1 + base_ptr->var.payload.stream.pos;
}

//This function finds the specified object instance if it is registered and builds the payload for it.
void interval_data_base_Latest_Payload_build_for_object_instance(lwm2m_object_link_t object, uint8_t **val, size_t *length)
{
  for (int i = 0; i < INTERVAL_DATA_BASE_INSTANCES_MAX; i++)
  {
    if ((interval_data[i] != 0) &&
        (interval_data[i]->object_id == object.object_id) &&
        (interval_data[i]->instance_id == object.instance_id))
    {
      interval_data_base_Latest_Payload_build(interval_data[i], object.instance_id, val, length);
    }
  }
}

/*******************************************************************************
  CALLBACKS

  Design Note:
    I have thought about hooking these functions directly up to the lwm2m
    resource structure. However the main issue is that while we get the
    instance number in the callback. We however do not get any information
    about the child object that calls its.

    Thus because of this issue, we will need to pass the interval data
    base pointer manually via the get_user_data() of each child object base
    class

*******************************************************************************/

/**-----------------------------------------------------------------------------
  Start Handler:

  Called when the interval object is requested to START recording intervals.

------------------------------------------------------------------------------*/
uint8_t interval_data_base_Start_execute_handler(interval_data_base_t *base_ptr, uint16_t instance)
{
  assert(base_ptr != 0);
  base_ptr->set_Status(base_ptr->instance_id, 0, true);
  return COAP_204_CHANGED;
}

/**-----------------------------------------------------------------------------
  Stop Handler:

  Called when the interval object is requested to STOP recording intervals.

------------------------------------------------------------------------------*/
uint8_t interval_data_base_Stop_execute_handler(interval_data_base_t *base_ptr, uint16_t instance)
{
  assert(base_ptr != 0);
  base_ptr->set_Status(base_ptr->instance_id, 1, true);
  return COAP_204_CHANGED;
}

/**-----------------------------------------------------------------------------
  Interval Historical Read Execution Handler

  Executable resource designed to retrieve ad-hoc interval read data.

  The resource takes two arguments:
    - Argument 0: First Interval time to Retrieve represented as number of seconds since Jan 1st, 1970 in the UTC time zone.
    - Argument 1: Last interval time to Retrieve represented as number of seconds since Jan 1st, 1970 in the UTC time zone.

------------------------------------------------------------------------------*/
uint8_t interval_data_base_Interval_Historical_Read_execute_handler(interval_data_base_t *base_ptr, uint16_t instance, uint8_t * buffer, int length)
{
  assert(base_ptr);
  time_t last_delivered_interval = 0;
  base_ptr->get_Last_Delivered_Interval(instance, &last_delivered_interval);
  bool result = false;

  parse_lwm2m_exe_argument_t arglist_array[PARSE_LWM2M_EXE_ARG_MAX_ARGUMENT];
  uint16_t argcount = 0;

  /* Handle Parameters */
  parse_lwm2m_exe_arg_parse(buffer, length, PARSE_LWM2M_EXE_ARG_MAX_ARGUMENT, arglist_array, &argcount);

  result = parse_lwm2m_exe_arg_entry_as_uint32(arglist_array, argcount, 0, &(base_ptr->var.last_interval_historical_read.oldest_timestamp_s));
  if (!result)
  {
    log_err("first interval option invalid");
    return COAP_402_BAD_OPTION; // TODO: confirm return value
  }

  result = parse_lwm2m_exe_arg_entry_as_uint32(arglist_array, argcount, 1, &(base_ptr->var.last_interval_historical_read.newest_timestamp_s));
  if (!result)
  {
    log_err("last interval option invalid");
    return COAP_402_BAD_OPTION; // TODO: confirm return value
  }

  log_debug("interval historical read exe(first: %lu, last: %lu)",
              (long unsigned) base_ptr->var.last_interval_historical_read.oldest_timestamp_s,
              (long unsigned) base_ptr->var.last_interval_historical_read.newest_timestamp_s
            );

  /* Execute Command */
  result = false;
  result = interval_data_base_logger_generate_payload_historical(
              base_ptr,
              last_delivered_interval,
              last_delivered_interval+86400,
              &(base_ptr->var.last_interval_historical_read.oldest_timestamp_s),
              &(base_ptr->var.last_interval_historical_read.newest_timestamp_s)
            );

  if (!result)
  {
    log_err("failed to generate payload");
    return COAP_404_NOT_FOUND;
  }

  // Transmit the payload of which the size if the CBOR payload plus 1 byte,
  // indicating that this is a CBOR encoded payload
  // (Notify has been set to true)
  base_ptr->var.last_interval_historical_read.confirmation_required = true;
  base_ptr->set_Interval_Historical_Read_Payload(instance, base_ptr->var.payload.buffer, 1 + base_ptr->var.payload.stream.pos, true);

  return COAP_204_CHANGED;

}


/**-----------------------------------------------------------------------------
  Interval Historical Read Payload Confirmed Handler

  This is used after a historical read execute command has been processed, and
  the server has received the requested payload. This would then update
  the last delivered interval resource.


------------------------------------------------------------------------------*/
uint8_t interval_data_base_Interval_Historical_Read_Payload_confirmed_handler(interval_data_base_t *base_ptr, uint16_t instance)
{
  assert(base_ptr != 0);
  log_debug("confirming historical payload delivery for %d", instance);

  if (!(base_ptr->var.last_interval_historical_read.confirmation_required))
  { // Ignore
    return COAP_NO_ERROR;
  }

  uint32_t newest_timestamp_s = base_ptr->var.last_interval_historical_read.newest_timestamp_s;
  if (0 != newest_timestamp_s)
  {
    base_ptr->var.last_interval_historical_read.confirmation_required = false;
#if 0
//    TODO: We need to review this, I'm not sure we want to be updating the last delivered
//    points for historical reads since it could cause us to miss intervals if we requested
//    a range that didn't include older intervals. This should be clarified in the design
    base_ptr->set_Last_Delivered_Interval(instance, newest_timestamp_s, true);
#endif

  }

  // Clear Interval Historical Payload
  base_ptr->set_Interval_Historical_Read_Payload(instance, NULL, 0, false);

  return COAP_NO_ERROR;
}



/**-----------------------------------------------------------------------------
  Latest Payload Confirmed Handler

  This is used to advance the interval delivered pointers after we have confirmation
  from the server that it has received the payload.

------------------------------------------------------------------------------*/
uint8_t interval_data_base_Latest_Payload_confirmed_handler(interval_data_base_t *base_ptr, uint16_t instance)
{
  assert(base_ptr != 0);
  log_debug("confirming latest payload delivery for %d", instance);

  if (!(base_ptr->var.last_interval_payload.confirmation_required))
  { // Ignore
    return COAP_NO_ERROR;
  }

  uint32_t newest_timestamp_s = base_ptr->var.last_interval_payload.newest_timestamp_s;
  if (0 != newest_timestamp_s)
  {
    base_ptr->var.last_interval_payload.confirmation_required = false;
    base_ptr->set_Last_Delivered_Interval(instance, newest_timestamp_s, true);
  }

  // Clear Interval Historical Payload
  base_ptr->set_Latest_Payload(instance, NULL, 0, false);

  return COAP_NO_ERROR;
}

uint8_t interval_data_base_Latest_Payload_confirmed_for_object_instance(lwm2m_object_link_t object)
{
  for (int i = 0; i < INTERVAL_DATA_BASE_INSTANCES_MAX; i++)
  {
    if ((interval_data[i] != 0) &&
        (interval_data[i]->object_id == object.object_id) &&
        (interval_data[i]->instance_id == object.instance_id))
    {
        interval_data_base_Latest_Payload_confirmed_handler(interval_data[i], object.instance_id);
    }
  }

  log_warn("requested interval data object was not found for confirmation");
  return COAP_NO_ERROR;
}

/**-----------------------------------------------------------------------------
  Handler for for UTC_Offset_changed functions

  e.g. For Water_Flow_Readings you will need to attach below function to
        Water_Flow_Readings_object_declaration.resources[2].changed

  ```
  Water_Flow_Readings_UTC_Offset_changed(uint16_t instance)
  {
    interval_data_base_Interval_UTC_Offset_changed_handler(
        (interval_data_base_t *) Water_Flow_Readings_get_user_data(instance),
        instance
      );
  }
  ```

------------------------------------------------------------------------------*/
void interval_data_base_Interval_UTC_Offset_changed_handler(interval_data_base_t *base_ptr, uint16_t instance)
{ // This is called by by child objects if any resources has changed
  // We shall used this to reload certain calculated parameters
  // "UTC+10"
  const char *utc_offset_str = "";

  assert(base_ptr != 0);
  assert(base_ptr->instance_id == instance);

  base_ptr->get_Interval_UTC_Offset(base_ptr->instance_id, &utc_offset_str);


  /** Parsing UTC Offset
  **********************/

  // Locate valid/present utc offset string from Delivery Schedule or Device Object
  base_ptr->get_Interval_UTC_Offset(instance, &utc_offset_str);
  if (utc_offset_str[0] == '\0')
  { // Empty String Detected
    Device_UTC_Offset_get(0, &utc_offset_str); // Get Device instance 0's UTC offset string
    if (utc_offset_str[0] == '\0')
    { // Empty String Detected
      base_ptr->var.UTC_offset_seconds = 0; // No UTC offset found
      return;
    }
  }

  // Parse UTC string and save result
  base_ptr->var.UTC_offset_seconds = utc_offset_parse(utc_offset_str);
  log_debug("UTC_offset_seconds: %ld <-- %s", base_ptr->var.UTC_offset_seconds, utc_offset_str);
}


/**-----------------------------------------------------------------------------
  Interval Change Configuration Execution Handler

  Allows for Interval Period, Interval Start Offset and Interval UTC Offset to be reconfigured.

    - Argument 0: [Mandatory]
        Interval Period represented as an integer as defined in the
          Interval Period Resource.

    - Argument 1: [Optional]
        Interval start offset represented as an integer as defined in the
          Interval Start Offset Resource.
        If not provided, leave the value as per the current configuration.

    - Argument 2: [Optional]
        Interval UTC offset represented as a String as defined in the
          Interval UTC Offset Resource.
        If not provided, leave the value as per the current configuration.

  Note:
    Depending on the specifics of the implementation of this object,
    reconfiguring the Interval data object may result in the removal of all
    historical data for this Interval Data Object Instance.
    Please consult with your device vendor as to the implications
    of reconfiguring an Interval Data Object Instance.

------------------------------------------------------------------------------*/
uint8_t interval_data_base_Interval_Change_Configuration_execute_handler(interval_data_base_t *base_ptr, uint16_t instance, uint8_t * buffer, int length)
{ // TODO: Handle incoming user arguments. For now this is just a dummy function

  assert(base_ptr != 0);

  uint32_t interval_period_s = 60;
  uint32_t interval_start_offset = 0;

  bool result = false;
  parse_lwm2m_exe_argument_t arglist_array[PARSE_LWM2M_EXE_ARG_MAX_ARGUMENT];
  uint16_t argcount = 0;

  /* Handle Parameters */
  parse_lwm2m_exe_arg_parse(buffer, length, PARSE_LWM2M_EXE_ARG_MAX_ARGUMENT, arglist_array, &argcount);

  log_debug("got: %s 0X%04x", buffer, base_ptr);

  /* Extract and handle each parameter*/

  // Interval Period
  result = parse_lwm2m_exe_arg_entry_as_uint32(arglist_array, argcount, 0, &interval_period_s);
  if (!result)
  { // This value is mandatory
    log_err("Interval Period Option Missing");
    return COAP_406_NOT_ACCEPTABLE;
  }

  // Interval Start Offset
  result = parse_lwm2m_exe_arg_entry_as_uint32(arglist_array, argcount, 1, &interval_start_offset);
  if (!result)
  {
    log_err("no Interval Start Offset found");
    return COAP_406_NOT_ACCEPTABLE;
  }

  if (interval_start_offset >= interval_period_s)
  {
    log_err("Interval start offset must be less than interval period");
    return COAP_406_NOT_ACCEPTABLE;
  }

  base_ptr->set_Interval_Start_Offset(instance, interval_start_offset, true);
  base_ptr->set_Interval_Period(instance, interval_period_s, true);

  char * utc_offset_str_string_ptr = 0;
  size_t utc_offset_str_length = 0;
  // Interval UTC Offset
  result = parse_lwm2m_exe_arg_entry_as_string_zerocopy(arglist_array, argcount, 2, &utc_offset_str_string_ptr, &utc_offset_str_length);
  if ((result) && (utc_offset_str_string_ptr != 0))
  {
    // Internally set_Interval_UTC_Offset uses memcpy
    base_ptr->set_Interval_UTC_Offset(instance, utc_offset_str_string_ptr, utc_offset_str_length, true);
  }
  else
  {
    log_warn("no UTC offset found");
  }

  log_debug("intervalChangeExe: argcount:%d|bufflen:%d|raw:'%s'", argcount, length, buffer);
  log_debug("intervalChangeExe: interval_period:%d|offset:%d|utc:'%s'", interval_period_s, interval_start_offset, utc_offset_str_string_ptr ? utc_offset_str_string_ptr : "null");


  // Clear Buffer // TODO: Check if we really have to reset the buffer for all changes, or we only have to reset for changes to utc or start offset. For now we always reset
  interval_data_base_logger_clear(base_ptr);
  interval_data_base_Interval_UTC_Offset_changed_handler(base_ptr, instance);

  return COAP_204_CHANGED;
}


/*******************************************************************************
  EXTERNAL FACING INTERFACE
*******************************************************************************/

void interval_data_base_initalise_instance(interval_data_base_t *base_ptr, lwm2m_object_declaration_t *lwm2m_object_declaration_ptr)
{
  log_info("Initialising interval data object %d", base_ptr->instance_id);

  // Set required base structure variables
  base_ptr->object_id = lwm2m_object_declaration_ptr->id;

  bool found = false;
  for (uint16_t i = 0; i < INTERVAL_DATA_BASE_INSTANCES_MAX; i++)
  {
    if (interval_data[i] == 0)
    {
      interval_data[i] = base_ptr;
      found = true;
      break;
    }
  }
  if (!found)
  {
    log_err("too many interval data instance created");
    return;
  }

  interval_data_base_Interval_UTC_Offset_changed_handler(base_ptr, base_ptr->instance_id);
  interval_data_base_logger_clear(base_ptr);
}

bool interval_data_base_release_instance(uint16_t instance_id)
{
  for (uint16_t i = 0; i < INTERVAL_DATA_BASE_INSTANCES_MAX; i++)
  {
    if ((interval_data[i] != 0) && (interval_data[i]->instance_id == instance_id))
    {
      interval_data[i] = 0;
      return true;
    }
  }
  return false;
}

void interval_data_base_tick(uint32_t unix_epoch_time)
{
  for (uint16_t i = 0; i < INTERVAL_DATA_BASE_INSTANCES_MAX; i++)
  {
    if (interval_data[i] != 0)
    {
      interval_data_base_tick_instance(unix_epoch_time, interval_data[i]);
    }
  }
}


bool interval_data_base_initalise_verify_defaults(
    interval_data_base_t *base_ptr,
    const uint32_t    expect_interval_period,
    const uint32_t    expect_start_offset,
    const char        *expect_UTC_offset_seconds,
    const uint16_t    expect_UTC_offset_seconds_size,
    const bool        expect_delivery_midnight_aligned
  )
{
  uint32_t    interval_period = 0;
  uint32_t    start_offset = 0;
  const char  *UTC_offset_seconds = NULL;
  bool        delivery_midnight_aligned = 0;
  bool        mismatch = false;

  base_ptr->get_Interval_Period                    (base_ptr->instance_id, &interval_period);
  base_ptr->get_Interval_Start_Offset              (base_ptr->instance_id, &start_offset);
  base_ptr->get_Interval_UTC_Offset                (base_ptr->instance_id, &UTC_offset_seconds);
  base_ptr->get_Interval_Delivery_Midnight_Aligned (base_ptr->instance_id, &delivery_midnight_aligned);

  mismatch = (expect_interval_period == interval_period) ? mismatch : true;
  mismatch = (expect_start_offset == start_offset) ? mismatch : true;
  mismatch = (0 == memcmp(expect_UTC_offset_seconds, UTC_offset_seconds, expect_UTC_offset_seconds_size)) ? mismatch : true;
  mismatch = (expect_delivery_midnight_aligned == delivery_midnight_aligned) ? mismatch : true;

  if (mismatch)
  {
    log_err("interval object %u mismatched( interval:%lu, startoffset:%lu, utc:%s, delivMidAlign:%d)",
      base_ptr->instance_id,
      interval_period,
      start_offset,
      UTC_offset_seconds,
      delivery_midnight_aligned
      );
    log_err("interval object %u expected( interval:%lu, startoffset:%lu, utc:%s, delivMidAlign:%d)",
      base_ptr->instance_id,
      expect_interval_period,
      expect_start_offset,
      expect_UTC_offset_seconds,
      expect_delivery_midnight_aligned
      );
    return false;
  }
  return true;
}





