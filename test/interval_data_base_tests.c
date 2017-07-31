/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "interval_data_base_tests.h"
#include "interval_data_base.h"
#include "unity.h"
#include "cbor.h"
#include "log.h"
#include "utc_offset_parser.h" // utc_offset_parse()

lwm2m_object_declaration_t mock_object_declaration = {
  .id = 8010  /* Object ID */
};


#define COAP_205_CONTENT                (uint8_t)0x45

static double mock_value = 0.0;
static double mock_get_value(uint16_t instance)
{
  return mock_value;
}

#define DECLARE_MOCK_FUNC_DOUBLE( VAR, INIT_VALUE, GETTER_FUNC, SETTER_FUNC )  \
double VAR = INIT_VALUE;                                                       \
uint8_t GETTER_FUNC(uint16_t instance, double *val)                            \
{                                                                              \
  *val = VAR;                                                                  \
  return COAP_205_CONTENT;                                                     \
}                                                                              \
uint8_t SETTER_FUNC(uint16_t instance, double val, bool notify)                \
{                                                                              \
  VAR = val;                                                                   \
  return COAP_205_CONTENT;                                                     \
}

#define DECLARE_MOCK_FUNC_INTEGER( VAR, INIT_VALUE, GETTER_FUNC, SETTER_FUNC ) \
uint32_t VAR = INIT_VALUE;                                                     \
uint8_t GETTER_FUNC(uint16_t instance, uint32_t *val)                          \
{                                                                              \
  *val = VAR;                                                                  \
  return COAP_205_CONTENT;                                                     \
}                                                                              \
uint8_t SETTER_FUNC(uint16_t instance, uint32_t val, bool notify)              \
{                                                                              \
  VAR = val;                                                                   \
  return COAP_205_CONTENT;                                                     \
}

#define DECLARE_MOCK_FUNC_BOOL( VAR, INIT_VALUE, GETTER_FUNC, SETTER_FUNC )    \
bool VAR = INIT_VALUE;                                                         \
uint8_t GETTER_FUNC(uint16_t instance, bool *val)                              \
{                                                                              \
  *val = VAR;                                                                  \
  return COAP_205_CONTENT;                                                     \
}                                                                              \
uint8_t SETTER_FUNC(uint16_t instance, bool val, bool notify)                  \
{                                                                              \
  VAR = val;                                                                   \
  return COAP_205_CONTENT;                                                     \
}

#define DECLARE_MOCK_FUNC_TIME( VAR, INIT_VALUE, GETTER_FUNC, SETTER_FUNC )    \
time_t VAR = INIT_VALUE;                                                       \
uint8_t GETTER_FUNC(uint16_t instance, time_t *val)                            \
{                                                                              \
  *val = VAR;                                                                  \
  return COAP_205_CONTENT;                                                     \
}                                                                              \
uint8_t SETTER_FUNC(uint16_t instance, time_t val, bool notify)                \
{                                                                              \
  VAR = val;                                                                   \
  return COAP_205_CONTENT;                                                     \
}

#define DECLARE_MOCK_FUNC_STRING( VAR, INIT_VALUE, GETTER_FUNC, SETTER_FUNC )  \
char VAR[100] = INIT_VALUE;                                                        \
uint8_t GETTER_FUNC(uint16_t instance,  const char **val)                      \
{                                                                              \
  *val = VAR;                                                                  \
  return COAP_205_CONTENT;                                                     \
}                                                                              \
uint8_t SETTER_FUNC(uint16_t instance,  const char *val, size_t length, bool notify)\
{                                                                              \
  memcpy(VAR, val, length);                                                    \
  return COAP_205_CONTENT;                                                     \
}

#define DECLARE_MOCK_FUNC_BYTEBUFF( VAR, INIT_VALUE, GETTER_FUNC, SETTER_FUNC )\
uint8_t VAR[100] = INIT_VALUE;                                                     \
uint8_t GETTER_FUNC(uint16_t instance, uint8_t **val, size_t *length)                   \
{                                                                              \
  *val = VAR;                                                                  \
  return COAP_205_CONTENT;                                                     \
}                                                                              \
uint8_t SETTER_FUNC(uint16_t instance, uint8_t* val, size_t length, bool notify)\
{                                                                              \
  memcpy(VAR, val, length);                                                    \
  return COAP_205_CONTENT;                                                     \
}

/*                           VARIABLE                                 | INIT | GETTER FUNCTION                              | SETTER FUNCTIONS                            */
DECLARE_MOCK_FUNC_INTEGER(   mock_Interval_Period,                      0,     mock_Interval_Period_get,                      mock_Interval_Period_set                    );
DECLARE_MOCK_FUNC_INTEGER(   mock_Interval_Start_Offset,                0,     mock_Interval_Start_Offset_get,                mock_Interval_Start_Offset_set              );
DECLARE_MOCK_FUNC_STRING(    mock_Interval_UTC_Offset,                  "",    mock_Interval_UTC_Offset_get,                  mock_Interval_UTC_Offset_set                );
DECLARE_MOCK_FUNC_TIME(      mock_Interval_Collection_Start_Time,       0,     mock_Interval_Collection_Start_Time_get,       mock_Interval_Collection_Start_Time_set     );
DECLARE_MOCK_FUNC_TIME(      mock_Oldest_Recorded_Interval,             0,     mock_Oldest_Recorded_Interval_get,             mock_Oldest_Recorded_Interval_set           );
DECLARE_MOCK_FUNC_TIME(      mock_Last_Delivered_Interval,              0,     mock_Last_Delivered_Interval_get,              mock_Last_Delivered_Interval_set            );
DECLARE_MOCK_FUNC_TIME(      mock_Latest_Recorded_Interval,             0,     mock_Latest_Recorded_Interval_get,             mock_Latest_Recorded_Interval_set           );
DECLARE_MOCK_FUNC_BOOL(      mock_Interval_Delivery_Midnight_Aligned,   0,     mock_Interval_Delivery_Midnight_Aligned_get,   mock_Interval_Delivery_Midnight_Aligned_set );
DECLARE_MOCK_FUNC_BYTEBUFF(  mock_Interval_Historical_Read_Payload,     {0},   mock_Interval_Historical_Read_Payload_get,     mock_Interval_Historical_Read_Payload_set   );
DECLARE_MOCK_FUNC_INTEGER(   mock_Status,                               0,     mock_Status_get,                               mock_Status_set                             );
DECLARE_MOCK_FUNC_BYTEBUFF(  mock_Latest_Payload,                       {0},   mock_Latest_Payload_get,                       mock_Latest_Payload_set                     );




DECLARE_INTERVAL_DATA_INSTANCE(0, mock, mock_base);


#ifdef UNIT_TEST_PAYLOAD_DATA_DUMP
static void test_utility_cbor_stream_print_hex(cbor_stream_t *stream, uint16_t offset, char *str)
{
    log_raw("Hex: {");
    while (offset < stream->pos)
    {
      log_raw("%s0x%02x%s", (offset%9) == 0 ? "\r\n        ":"", stream->data[offset], (offset + 1) < stream->pos ? ", ":"");
      offset ++;
    }
    log_raw("\r\n     }; // --- %s\r\n", str ? str : "");
}
#else
#define test_utility_cbor_stream_print_hex(...)
#endif


//#define INTERVAL_DATA_VISUAL_TEST
#ifdef INTERVAL_DATA_VISUAL_TEST
void test_interval_data_visual(void)
{
  uint32_t mock_unix_time = 0;
  mock_unix_time = 86400000; // 1000 days since unix epoch at midnight
  mock_unix_time -= 60*60*2; // Subtract 2 hours before midnight
  mock_unix_time -= utc_offset_parse(DEFAULT_INSTANCE_0_VALUE_INTERVAL_UTC_OFFSET);  // Midnight in reference to local time

  uint32_t till_next_tx_s = 0;

  // Setup State
  mock_object_declaration.id = 8010;
  mock_Interval_Period_set(                     0 , DEFAULT_INSTANCE_0_VALUE_INTERVAL_PERIOD                    , false);
  mock_Interval_Start_Offset_set(               0 , DEFAULT_INSTANCE_0_VALUE_INTERVAL_START_OFFSET              , false);
  mock_Interval_UTC_Offset_set(                 0 , DEFAULT_INSTANCE_0_VALUE_INTERVAL_UTC_OFFSET                , sizeof(DEFAULT_INSTANCE_0_VALUE_INTERVAL_UTC_OFFSET) , false);
  mock_Interval_Delivery_Midnight_Aligned_set(  0 , DEFAULT_INSTANCE_0_VALUE_INTERVAL_DELIVERY_MIDNIGHT_ALIGNED , false);

  interval_data_base_Interval_UTC_Offset_changed_handler(&mock_base, 0);

  // Init
  interval_data_base_tick(mock_unix_time);

  for (uint32_t i = 0 ; i < 100 ; i++)
  {
    int32_t  increment_s = 0;
    int32_t  UTC_offset_seconds;
    uint32_t next_interval_record_time_s;
    uint32_t local_time_s = 0;
    till_next_tx_s = interval_data_base_Diagnostics(&mock_base, mock_unix_time, &UTC_offset_seconds, &next_interval_record_time_s, &local_time_s);

    // Skip Logic
    if ( 3800 < till_next_tx_s )
    { // Skip Ahead time
      increment_s = till_next_tx_s - 3600;
    }
    else
    { // Slow scan
      increment_s = (10*60);  // 10 mins increment
    }

    // Increment Time
    mock_unix_time += increment_s;

    // Print
    log_debug("Record in %5lumins (%2luhr) | tick+ %5lumins | UTCoff %2luh | Next Time at %5lus | local time is %5lus",
        (long unsigned) till_next_tx_s/(60), (long unsigned) till_next_tx_s/(60*60),
        (long unsigned) increment_s/60,
        (long unsigned) UTC_offset_seconds/(60*60),
        (long unsigned) next_interval_record_time_s,
        (long unsigned) local_time_s
      );

    // TICK
    interval_data_base_tick(mock_unix_time);

    // Simulated sensor change
    mock_value += 0.1;
  }

  // Checkout what the payload looks like
  cbor_stream_t *stream = &(mock_base.var.payload.stream);
  interval_data_base_logger_generate_payload_historical(&mock_base, 0, 0, NULL, NULL);
  test_utility_cbor_stream_print_hex(stream, 0, "test interval latest payload");

  interval_data_base_logger_generate_payload_historical(&mock_base, 86400000+259200, 86400000+259200+259200, NULL, NULL);
  test_utility_cbor_stream_print_hex(stream, 0, "test interval latest payload with range");
}
#else
void test_interval_data_visual(void){ /* Dummy Function */ };
#endif



void test_interval_data_overall(void)
{
  int32_t  UTC_offset_seconds;
  uint32_t next_interval_record_time_s;
  uint32_t local_time_s = 0;

  // Setup mock system time to be two hour before midnight local time.
  uint32_t mock_unix_time = 0;
  mock_unix_time = 86400000; // 1000 days since unix epoch at midnight
  mock_unix_time -= 60*60*2; // Subtract 2 hours before midnight
  mock_unix_time -= utc_offset_parse(DEFAULT_INSTANCE_0_VALUE_INTERVAL_UTC_OFFSET);  // Midnight in reference to local time

  uint32_t till_next_tx_s = 0;

  // Setup State
  mock_object_declaration.id = 8010;
  mock_Interval_Period_set(                     0 , DEFAULT_INSTANCE_0_VALUE_INTERVAL_PERIOD                    , false);
  mock_Interval_Start_Offset_set(               0 , DEFAULT_INSTANCE_0_VALUE_INTERVAL_START_OFFSET              , false);
  mock_Interval_UTC_Offset_set(                 0 , DEFAULT_INSTANCE_0_VALUE_INTERVAL_UTC_OFFSET                , sizeof(DEFAULT_INSTANCE_0_VALUE_INTERVAL_UTC_OFFSET) , false);
  mock_Interval_Delivery_Midnight_Aligned_set(  0 , DEFAULT_INSTANCE_0_VALUE_INTERVAL_DELIVERY_MIDNIGHT_ALIGNED , false);

  interval_data_base_Interval_UTC_Offset_changed_handler(&mock_base, 0);

  // Init
  interval_data_base_tick(mock_unix_time);
  till_next_tx_s = interval_data_base_Diagnostics(&mock_base, mock_unix_time, &UTC_offset_seconds, &next_interval_record_time_s, &local_time_s);
  TEST_ASSERT_EQUAL_INT32(7200, till_next_tx_s);      // 2hr from midnight (as start time is at midnight by default)
  TEST_ASSERT_EQUAL_INT32(36000, UTC_offset_seconds); // 10hr
  TEST_ASSERT_EQUAL_INT32(86392800, local_time_s);    // Intial System time is 1000days since epoch - 2hr - (UTC+10 hr)
  TEST_ASSERT_EQUAL_INT32(86400000, next_interval_record_time_s); // local_time_s + 2hr

  mock_unix_time += till_next_tx_s;
  mock_value += 1;

  for (int i = 0 ; i < 100 ; i++)
  {
    interval_data_base_tick(mock_unix_time);
    till_next_tx_s = interval_data_base_Diagnostics(&mock_base, mock_unix_time, &UTC_offset_seconds, &next_interval_record_time_s, &local_time_s);
    TEST_ASSERT_EQUAL_INT32(86400, till_next_tx_s);

    mock_unix_time += till_next_tx_s;
    mock_value += 1;
  }

#if 0 // TODO: Currently Just visual inspection, but lets try to convert this into unit test as well. Oh and maybe check the handler logic as well
      // Currently there is an issue with the payload generation if its too big
      // Checkout what the payload looks like
  cbor_stream_t *stream = &(mock_base.var.payload.stream);
  interval_data_base_logger_generate_payload_historical(&mock_base, 0, 0, NULL, NULL);
  test_utility_cbor_stream_print_hex(stream, 0, "test interval latest payload");

  interval_data_base_logger_generate_payload_historical(&mock_base, 86400000+259200, 1186400000, NULL, NULL);
  test_utility_cbor_stream_print_hex(stream, 0, "test interval latest payload with range");

  log_debug("buffsize: %lu, %lu", (unsigned long)stream->pos, (unsigned long)stream->size);
#endif
}

static void test_interval_data_base_cbor_payload_basic(void)
{ /* Check basic payload function (and if it has the required LWM2M signature )*/
  uint32_t mock_unix_time = 0;
  uint32_t interval_period_s = 0;
  cbor_stream_t *stream = &(mock_base.var.payload.stream);

  char expected[] = {
                      0x9f /* Payload Container Start*/
                      ,
                      0x19,0x1f,0x4a, // Object ID : 8010
                      0x00,           // Instance  : 0
                      0x9f /* Blocks Container Start*/
                      ,
                      /* Block 0*/
                      0x9f, 0x1a, 0x05, 0x26, 0x5c, 0x00, 0x1a, 0x00, 0x01,
                      0x51, 0x80, 0x9f, 0x9f, 0x01, 0xff, 0x9f, 0x02, 0xff,
                      0x9f, 0x03, 0xff, 0x9f, 0x04, 0xff, 0xff, 0xff
                      ,
                      0xff /* Blocks Container Closes*/
                      ,
                      0xff /* Payload Container Closes*/
                    };
  char expected_buffer[] = {
                      0x02 /* LWM2M Signature Value For CBOR Object */
                      ,
                      0x9f /* Payload Container Start*/
                      ,
                      0x19,0x1f,0x4a, // Object ID : 8010
                      0x00,           // Instance  : 0
                      0x9f /* Blocks Container Start*/
                      ,
                      /* Block 0*/
                      0x9f, 0x1a, 0x05, 0x26, 0x5c, 0x00, 0x1a, 0x00, 0x01,
                      0x51, 0x80, 0x9f, 0x9f, 0x01, 0xff, 0x9f, 0x02, 0xff,
                      0x9f, 0x03, 0xff, 0x9f, 0x04, 0xff, 0xff, 0xff
                      ,
                      0xff /* Blocks Container Closes*/
                      ,
                      0xff /* Payload Container Closes*/
                    };
  mock_unix_time = 86400000; // 1000 days since unix epoch at midnight
  interval_period_s = 86400;
  interval_data_base_logger_clear(&mock_base);
                                /* base_ptr, unix_epoch_timestamp, interval_period, new_sensor_value */
  interval_data_base_logger_record(&mock_base, mock_unix_time, interval_period_s, 1 ); mock_unix_time += interval_period_s;
  interval_data_base_logger_record(&mock_base, mock_unix_time, interval_period_s, 2 ); mock_unix_time += interval_period_s;
  interval_data_base_logger_record(&mock_base, mock_unix_time, interval_period_s, 3 ); mock_unix_time += interval_period_s;
  interval_data_base_logger_record(&mock_base, mock_unix_time, interval_period_s, 4 ); mock_unix_time += interval_period_s;

  /* Generate and test */
  TEST_ASSERT_TRUE(interval_data_base_logger_generate_payload_historical(&mock_base, 0, 0, NULL, NULL));
  test_utility_cbor_stream_print_hex(stream, 0, "basic payload test");
  TEST_ASSERT_EQUAL_INT8_ARRAY(expected, stream->data, sizeof(expected)/sizeof(expected[0]));

  /* Check Output Buffer */
  TEST_ASSERT_EQUAL_INT8_ARRAY(expected_buffer, mock_base.var.payload.buffer, sizeof(expected_buffer)/sizeof(expected_buffer[0]));
}

static void test_interval_data_base_cbor_payload_empty(void)
{ /* Check behaviour empty logger */
  interval_data_base_logger_clear(&mock_base);
  /* Generate and test */
  TEST_ASSERT_FALSE(interval_data_base_logger_generate_payload_historical(&mock_base, 0, 0, NULL, NULL));
}

static void test_interval_data_base_cbor_payload_missing_interval(void)
{ /* Check behaviour with missing interval () */
  uint32_t mock_unix_time = 0;
  uint32_t interval_period_s = 0;
  cbor_stream_t *stream = &(mock_base.var.payload.stream);

  char expected[] = {
                      0x9f /* Payload Container Start*/
                      ,
                      0x19,0x1f,0x4a, // Object ID : 8010
                      0x00,           // Instance  : 0
                      0x9f /* Blocks Container Start*/
                      ,
                      /* Block 0*/
                      0x9f, 0x1a, 0x05, 0x26, 0x5c, 0x00, 0x1a, 0x00,
                      0x01, 0x51, 0x80, 0x9f, 0x9f, 0x01, 0xff, 0x9f, 0x02,
                      0xff, 0xff, 0xff
                      ,
                      /* Block 1*/
                      0x9f, 0x1a, 0x05, 0x2a, 0x50, 0x80, 0x1a, 0x00,
                      0x01, 0x51, 0x80, 0x9f, 0x9f, 0x03, 0xff, 0x9f, 0x04,
                      0xff, 0xff, 0xff
                      ,
                      0xff /* Blocks Container Closes*/
                      ,
                      0xff /* Payload Container Closes*/
                    };
  mock_unix_time = 86400000; // 1000 days since unix epoch at midnight
  interval_period_s = 86400;  // 24hr
  interval_data_base_logger_clear(&mock_base);
                                  /* base_ptr, unix_epoch_timestamp, new_sensor_value */
  interval_data_base_logger_record(&mock_base, mock_unix_time, interval_period_s, 1 ); mock_unix_time += interval_period_s;
  interval_data_base_logger_record(&mock_base, mock_unix_time, interval_period_s, 2 ); mock_unix_time += interval_period_s;
  mock_unix_time += interval_period_s; /* Skipped Period */
  interval_data_base_logger_record(&mock_base, mock_unix_time, interval_period_s, 3 ); mock_unix_time += interval_period_s;
  interval_data_base_logger_record(&mock_base, mock_unix_time, interval_period_s, 4 ); mock_unix_time += interval_period_s;

  /* Generate and test */ // TODO: Must throw some error or partial payload
  TEST_ASSERT_TRUE(interval_data_base_logger_generate_payload_historical(&mock_base, 0, 0, NULL, NULL));
  test_utility_cbor_stream_print_hex(stream, 0, "basic payload test");
  TEST_ASSERT_EQUAL_INT8_ARRAY(expected, stream->data, sizeof(expected)/sizeof(expected[0]));
}

void interval_data_tests_run(void)
{
  interval_data_base_initalise_instance(&mock_base, &mock_object_declaration);

  RUN_TEST(test_interval_data_base_cbor_payload_basic);
  RUN_TEST(test_interval_data_base_cbor_payload_empty);
  RUN_TEST(test_interval_data_base_cbor_payload_missing_interval);
  RUN_TEST(test_interval_data_overall);

  test_interval_data_visual();

}




