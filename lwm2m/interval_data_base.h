/*
 * interval_data_base.h
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 */

#ifndef __INTERVAL_DATA_BASE_H_
#define __INTERVAL_DATA_BASE_H_


#include <stdint.h>
#include <stdbool.h>
#include <assert.h>
#include <time.h> // time_t

#include "lwm2m_object_base.h" // lwm2m_object_t
#include "cbor.h"   // cbor_stream_t, cbor_init(), cbor_serialize_int(), cbor_destroy()

#if 0 /* This will blink the led if an interval was saved */
#define INTERVAL_DATA_BASE_LED_VISUAL_INDICATOR
#endif

#define INTERVAL_DATA_BASE_VERBOSE_DEBUG 0 // Verbose mode for debug messages

// Sensor Getter // Note: If sensor_ext_ptr is passed to this sensor getter then internally it would capture the value and return a pointer to the extended value structure. This is not used by this base. Instead its used by the child object to construct extended values in the payload.
typedef double  (*interval_data_base_get_value_func_t)(uint16_t instance_id, void **sensor_ext_ptr);

// Alarm Getter Function Types
typedef uint8_t (*bool_get_func_t          )(uint16_t instance, bool *val);
typedef uint8_t (*time_get_func_t          )(uint16_t instance, time_t *val);
typedef uint8_t (*double_get_func_t        )(uint16_t instance, double *val);
typedef uint8_t (*integer_get_func_t       )(uint16_t instance, uint32_t *val);
typedef uint8_t (*string_get_func_t        )(uint16_t instance, const char **val);
typedef uint8_t (*byte_array_get_func_t    )(uint16_t instance, uint8_t **val, size_t *length);

// Alarm Setter Function Types
typedef uint8_t (*bool_set_func_t          )(uint16_t instance, bool val, bool notify);
typedef uint8_t (*time_set_func_t          )(uint16_t instance, time_t val, bool notify);
typedef uint8_t (*double_set_func_t        )(uint16_t instance, double val, bool notify);
typedef uint8_t (*integer_set_func_t       )(uint16_t instance, uint32_t val, bool notify);
typedef uint8_t (*string_set_func_t        )(uint16_t instance, const char *val, size_t length, bool notify);
typedef uint8_t (*byte_array_set_func_t    )(uint16_t instance, uint8_t* val, size_t length, bool notify);

// Changed Function Type
typedef uint8_t (*changed_func_t           )(uint16_t instance);

// Execute
typedef uint8_t (*execute_func_t           )(lwm2m_object_t *object, uint16_t instance, uint16_t resource_id, uint8_t *buffer, int length);

// User Data
typedef void*   (*get_user_data_func_t     )(uint16_t instance);
typedef bool    (*set_user_data_func_t     )(uint16_t instance);


/*******************************************************************************
*******************************************************************************/

typedef struct interval_data_base_last_interval_historical_read_t
{ // Parameters and variables from last historical read execution
  // Design Note: This is kept as we may use it for finding new sensor
  bool     confirmation_required;
  uint32_t oldest_timestamp_s;  ///< number of seconds since Jan 1st, 1970 in the UTC time zone
  uint32_t newest_timestamp_s;    ///< number of seconds since Jan 1st, 1970 in the UTC time zone
} interval_data_base_last_interval_payload_t;

typedef struct interval_data_base_log_entries_t
{ // Entries
  uint32_t timestamp; // UNIX EPOCH (Convert as needed to local time)
  uint32_t interval_period;
  double   sensor_value;
  void *   sensor_ext;     // This points to a custom extra entry
} interval_data_base_log_entries_t;

typedef struct interval_data_base_logger_t
{ // Circular buffer
  interval_data_base_log_entries_t entries[INTERVAL_DATA_BASE_PAYLOAD_VALUE_COUNT_MAX];
  uint8_t read_pos;
  uint8_t write_pos;
  uint8_t count;
} interval_data_base_logger_t;

typedef struct interval_data_base_payload_t
{ // Payload Generation
  uint8_t buffer[INTERVAL_DATA_BASE_PAYLOAD_BUFFER_SIZE_BYTES];  // Staging Area for transmission
  cbor_stream_t stream;

  // Required for internal payload specific logic
  uint32_t interval_time_period;  // Seconds
} interval_data_base_payload_t;

typedef struct interval_data_base_variable_t
{ // Internal Variables
  bool interval_initialised;

  // Helps to convert UTC to local time
  int32_t  UTC_offset_seconds; // Calculated from UTC Offset String: Delivery_Schedule_Schedule_UTC_Offset_get()

  // This keeps track of the next timestamp to sample and record to logger
  uint32_t next_interval_record_time_s;

  // This is used together to store individual interval entries and then construct the payload to send over the wire
  interval_data_base_logger_t logger;
  interval_data_base_payload_t payload;
  interval_data_base_last_interval_payload_t last_interval_historical_read;
  interval_data_base_last_interval_payload_t last_interval_payload;

} interval_data_base_variable_t;


/**-----------------------------------------------------------------------------
  # Interval Data Base Main Data Structure For Child Instance
------------------------------------------------------------------------------*/

// This defines the structure of a event/alarm instance
typedef struct interval_data_base_t
{
  uint16_t instance_id;
  uint16_t object_id;

  interval_data_base_variable_t var;

  /* Sensor
  *************/

  // Sensor Application Function Call
  interval_data_base_get_value_func_t get_value;            //  sensor value

  /* Base Object Interface
  *************************/

  // Getter
  integer_get_func_t      get_Interval_Period                        ;
  integer_get_func_t      get_Interval_Start_Offset                  ;
  string_get_func_t       get_Interval_UTC_Offset                    ;
  time_get_func_t         get_Interval_Collection_Start_Time         ;
  time_get_func_t         get_Oldest_Recorded_Interval               ;
  time_get_func_t         get_Last_Delivered_Interval                ;
  time_get_func_t         get_Latest_Recorded_Interval               ;
  bool_get_func_t         get_Interval_Delivery_Midnight_Aligned     ;
  byte_array_get_func_t   get_Interval_Historical_Read_Payload       ;
  integer_get_func_t      get_Status                                 ;
  byte_array_get_func_t   get_Latest_Payload                         ;
  // Setter
  integer_set_func_t      set_Interval_Period                        ;
  integer_set_func_t      set_Interval_Start_Offset                  ;
  string_set_func_t       set_Interval_UTC_Offset                    ;
  time_set_func_t         set_Interval_Collection_Start_Time         ;
  time_set_func_t         set_Oldest_Recorded_Interval               ;
  time_set_func_t         set_Last_Delivered_Interval                ;
  time_set_func_t         set_Latest_Recorded_Interval               ;
  bool_set_func_t         set_Interval_Delivery_Midnight_Aligned     ;
  byte_array_set_func_t   set_Interval_Historical_Read_Payload       ;
  integer_set_func_t      set_Status                                 ;
  byte_array_set_func_t   set_Latest_Payload                         ;
}
interval_data_base_t;

/*******************************************************************************
*******************************************************************************/

#define DECLARE_INTERVAL_DATA_INSTANCE(instance, name, name_base)                                \
static interval_data_base_t name_base = {                                                        \
  .instance_id                            = instance                                            ,\
  .get_value                              = name ## _get_value                                  ,\
  .get_Interval_Period                    = name ## _Interval_Period_get                        ,\
  .get_Interval_Start_Offset              = name ## _Interval_Start_Offset_get                  ,\
  .get_Interval_UTC_Offset                = name ## _Interval_UTC_Offset_get                    ,\
  .get_Interval_Collection_Start_Time     = name ## _Interval_Collection_Start_Time_get         ,\
  .get_Oldest_Recorded_Interval           = name ## _Oldest_Recorded_Interval_get               ,\
  .get_Last_Delivered_Interval            = name ## _Last_Delivered_Interval_get                ,\
  .get_Latest_Recorded_Interval           = name ## _Latest_Recorded_Interval_get               ,\
  .get_Interval_Delivery_Midnight_Aligned = name ## _Interval_Delivery_Midnight_Aligned_get     ,\
  .get_Interval_Historical_Read_Payload   = name ## _Interval_Historical_Read_Payload_get       ,\
  .get_Status                             = name ## _Status_get                                 ,\
  .get_Latest_Payload                     = name ## _Latest_Payload_get                         ,\
  .set_Interval_Period                    = name ## _Interval_Period_set                        ,\
  .set_Interval_Start_Offset              = name ## _Interval_Start_Offset_set                  ,\
  .set_Interval_UTC_Offset                = name ## _Interval_UTC_Offset_set                    ,\
  .set_Interval_Collection_Start_Time     = name ## _Interval_Collection_Start_Time_set         ,\
  .set_Oldest_Recorded_Interval           = name ## _Oldest_Recorded_Interval_set               ,\
  .set_Last_Delivered_Interval            = name ## _Last_Delivered_Interval_set                ,\
  .set_Latest_Recorded_Interval           = name ## _Latest_Recorded_Interval_set               ,\
  .set_Interval_Delivery_Midnight_Aligned = name ## _Interval_Delivery_Midnight_Aligned_set     ,\
  .set_Interval_Historical_Read_Payload   = name ## _Interval_Historical_Read_Payload_set       ,\
  .set_Status                             = name ## _Status_set                                 ,\
  .set_Latest_Payload                     = name ## _Latest_Payload_set                         ,\
}

/*******************************************************************************
*******************************************************************************/

void interval_data_base_initalise_instance(interval_data_base_t *base_ptr, lwm2m_object_declaration_t *lwm2m_object_declaration_ptr);
bool interval_data_base_release_instance(uint16_t instance_id);
void interval_data_base_tick(uint32_t unix_epoch_time);
bool interval_data_base_initalise_verify_defaults(
    interval_data_base_t *base_ptr,
    const uint32_t    expect_interval_period,
    const uint32_t    expect_start_offset,
    const char        *expect_UTC_offset_seconds,
    const uint16_t    expect_UTC_offset_seconds_size,
    const bool        expect_delivery_midnight_aligned
  );

void interval_data_base_changed(uint16_t instance);
void interval_data_base_Interval_UTC_Offset_changed_handler(interval_data_base_t *base_ptr, uint16_t instance);
uint8_t interval_data_base_Interval_Historical_Read_execute_handler(interval_data_base_t *base_ptr, uint16_t instance, uint8_t * buffer, int length);
uint8_t interval_data_base_Interval_Historical_Read_Payload_confirmed_handler(interval_data_base_t *base_ptr, uint16_t instance);
uint8_t interval_data_base_Latest_Payload_confirmed_handler(interval_data_base_t *base_ptr, uint16_t instance);
uint8_t interval_data_base_Interval_Change_Configuration_execute_handler(interval_data_base_t *base_ptr, uint16_t instance, uint8_t * buffer, int length);
void interval_data_base_Latest_Payload_build_for_object_instance(lwm2m_object_link_t object, uint8_t **val, size_t *length);
void interval_data_base_Latest_Payload_build(interval_data_base_t *base_ptr, uint16_t instance, uint8_t **val, size_t *length);
uint8_t interval_data_base_Latest_Payload_confirmed_for_object_instance(lwm2m_object_link_t object);

uint8_t interval_data_base_Start_execute_handler(interval_data_base_t *base_ptr, uint16_t instance);
uint8_t interval_data_base_Stop_execute_handler(interval_data_base_t *base_ptr, uint16_t instance);

void interval_data_base_time_changed(void);

#ifdef UNIT_TEST
uint32_t interval_data_base_Diagnostics(
            interval_data_base_t *base_ptr,
            uint32_t unix_epoch_time,
            int32_t  *out_UTC_offset_seconds,
            uint32_t *out_next_interval_record_time_s,
            uint32_t *out_local_time_s
          );
bool interval_data_base_logger_clear(interval_data_base_t *base_ptr);
bool interval_data_base_logger_stats(
                interval_data_base_t *base_ptr,
                uint32_t *latest_recorded_timestamp,
                uint32_t *oldest_recorded_timestamp
              );
bool interval_data_base_logger_record(interval_data_base_t *base_ptr, uint32_t unix_epoch_timestamp, uint32_t interval_period, double new_sensor_value, void *sensor_ext_ptr);
bool interval_data_base_logger_generate_payload_historical(
         interval_data_base_t *base_ptr,
         const uint32_t first_interval_timestamp_s,  ///< number of seconds since Jan 1st, 1970 in the UTC time zone
         const uint32_t last_interval_timestamp_s,   ///< number of seconds since Jan 1st, 1970 in the UTC time zone
         uint32_t *oldest_timestamp_s,               ///< number of seconds since Jan 1st, 1970 in the UTC time zone
         uint32_t *newest_timestamp_s                ///< number of seconds since Jan 1st, 1970 in the UTC time zone
       );
#endif


#endif /*__INTERVAL_DATA_BASE_H_*/


