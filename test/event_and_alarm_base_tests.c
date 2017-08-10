/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "event_and_alarm_base_tests.h"
#include "event_and_alarm_base.h"
#include "unity.h"
#include "cbor.h"
#include "log.h"

lwm2m_object_declaration_t mock_object_declaration;


#define COAP_205_CONTENT                (uint8_t)0x45

#ifdef  DISABLE_EVENT_AND_ALARM_TEST_ASSERTS
#undef  TEST_ASSERT_TRUE
#define TEST_ASSERT_TRUE(X) do {X;} while(0);
#undef  TEST_ASSERT_FALSE
#define TEST_ASSERT_FALSE(X) do {X;} while(0);
#undef  TEST_ASSERT_EQUAL_INT8_ARRAY
#define  TEST_ASSERT_EQUAL_INT8_ARRAY(...)
#endif

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

/*                           VARIABLE             |  INIT     |  GETTER FUNCTION                  |    SETTER FUNCTIONS                 */
DECLARE_MOCK_FUNC_BOOL(      mock_alarm_state     ,  false    ,  mock_Alarm_State_get             ,    mock_Alarm_State_set             );
DECLARE_MOCK_FUNC_DOUBLE(    mock_set_threshold   ,  0.0      ,  mock_Alarm_Set_Threshold_get     ,    mock_Alarm_Set_Threshold_set     );
DECLARE_MOCK_FUNC_DOUBLE(    mock_clear_threshold ,  0.0      ,  mock_Alarm_Clear_Threshold_get   ,    mock_Alarm_Clear_Threshold_set   );
DECLARE_MOCK_FUNC_INTEGER(   mock_set_operator    ,  1        ,  mock_Alarm_Set_Operator_get      ,    mock_Alarm_Set_Operator_set      );
DECLARE_MOCK_FUNC_INTEGER(   mock_clear_operator  ,  1        ,  mock_Alarm_Clear_Operator_get    ,    mock_Alarm_Clear_Operator_set    );
DECLARE_MOCK_FUNC_INTEGER(   mock_event_type      ,  0        ,  mock_Event_Type_get              ,    mock_Event_Type_set              );
DECLARE_MOCK_FUNC_TIME(   mock_lastest_delivered_event_time, 0,  mock_Latest_Delivered_Event_Time_get, mock_Latest_Delivered_Event_Time_set );
DECLARE_MOCK_FUNC_TIME(    mock_lastest_recorded_event_time, 0,  mock_Latest_Recorded_Event_Time_get,  mock_Latest_Recorded_Event_Time_set );
DECLARE_MOCK_FUNC_BOOL(      mock_alarm_auto_clear, false     ,  mock_Alarm_Auto_Clear_get        ,    mock_Alarm_Auto_Clear_set        );
DECLARE_MOCK_FUNC_INTEGER(   mock_max_event_count ,  0        ,  mock_Alarm_Maximum_Event_Count_get,   mock_Alarm_Maximum_Event_Count_set );
DECLARE_MOCK_FUNC_INTEGER(   mock_max_event_period,  0        ,  mock_Alarm_Maximum_Event_Period_get,  mock_Alarm_Maximum_Event_Period_set );
DECLARE_MOCK_FUNC_BOOL(      mock_alarm_realtime  , false     ,  mock_Alarm_Realtime_get          ,    mock_Alarm_Realtime_set          );
DECLARE_MOCK_FUNC_INTEGER(   mock_event_code      ,  0        ,  mock_Event_Code_get              ,    mock_Event_Code_set              );


static void mock_Payload_Alarm_State_Change_Log_append(cbor_stream_t *stream, uint32_t timestamp, double new_sensor_value, bool new_alarm_state)
{
  cbor_serialize_array(stream, 3);
  cbor_serialize_int64_t(stream, timestamp);  // Must ensure that we can handle timestamp without overflow
  cbor_serialize_int(stream, (uint16_t) new_sensor_value);
  cbor_serialize_int(stream, new_alarm_state);
}

static void mock_Payload_Event_Log_append(cbor_stream_t *stream, uint32_t timestamp, double new_sensor_value, bool new_alarm_state)
{
  cbor_serialize_array(stream, 3);
  cbor_serialize_int64_t(stream, timestamp);  // Must ensure that we can handle timestamp without overflow
  cbor_serialize_int(stream, (uint16_t) new_sensor_value);
  cbor_serialize_int(stream, new_alarm_state);
}


DECLARE_EVENT_AND_ALARM_INSTANCE(0, mock, mock_base);


/**-----------------------------------------------------------------------------

  This tests event_and_alarm_check_thresholds() if it is correctly changing
  alarm state based on threshold settings

  ```
      ------------------------------------------------------ <--- Max Value
                 .--.         .--.
                .    .       .    .
        ....set_threshold.....    <--- mock_set_threshold
              .       .    .       .
             .         .--.         .
           .                         .
        ....clear_threshold...    <--- mock_clear_threshold
         .                             .
        .
      ------------------------------------------------------ <-- 0

      ------------------------------------------------------ <--- Max Value
        .
         .                             .
        ....clear_threshold...    <--- mock_clear_threshold
           .                         .
             .         .--.         .
              .       .    .       .
        ....set_threshold.....    <--- mock_set_threshold
                .    .       .    .
                 .--.         .--.
      ------------------------------------------------------ <-- 0
  ```

------------------------------------------------------------------------------*/
void test_threshold_breached(void)
{


  /////////////////////////////////
  // ALARM INITALLY FALSE CASES ///
  /////////////////////////////////

  /*
          Alarm Not Asserted ---> Alarm Not Asserted
        ------------------------------------------------------ <--- Max Value
          ....set_threshold.....    <--- mock_set_threshold
             .<-- mock value.
            . <-- initial alarm state = 0
          ....clear_threshold...    <--- mock_clear_threshold
        ------------------------------------------------------ <-- 0
  */
  mock_alarm_state = false;
  mock_value = 1.0;
  mock_set_threshold = 2.0;
  mock_clear_threshold = 0.5;
  mock_set_operator = EVENT_AND_ALARM_GREATER_THAN_OR_EQUAL_TO;
  mock_clear_operator = EVENT_AND_ALARM_LESS_THAN_OR_EQUAL_TO;
  TEST_ASSERT_FALSE(event_and_alarm_check_thresholds(&mock_base, mock_value, mock_alarm_state));

  /*
          Alarm Not Asserted ---> Alarm Asserted
        ------------------------------------------------------ <--- Max Value
              .<-- mock value
          ....set_threshold.....    <--- mock_set_threshold
            . <-- initial alarm state = 0
          ....clear_threshold...    <--- mock_clear_threshold
        ------------------------------------------------------ <-- 0
  */
  mock_alarm_state = false;
  mock_value = 3.0;
  mock_set_threshold = 2.0;
  mock_clear_threshold = 0.5;
  mock_set_operator = EVENT_AND_ALARM_GREATER_THAN_OR_EQUAL_TO;
  mock_clear_operator = EVENT_AND_ALARM_LESS_THAN_OR_EQUAL_TO;
  TEST_ASSERT_TRUE(event_and_alarm_check_thresholds(&mock_base, mock_value, mock_alarm_state));

  /*
          Alarm Not Asserted ---> Alarm Not Asserted
        ------------------------------------------------------ <--- Max Value
          ....set_threshold.....    <--- mock_set_threshold
            . <-- initial alarm state = 0
          ....clear_threshold...    <--- mock_clear_threshold
              .<-- mock value
        ------------------------------------------------------ <-- 0
  */
  mock_alarm_state = false;
  mock_value = 1.0;
  mock_set_threshold = 3.0;
  mock_clear_threshold = 2.0;
  mock_set_operator = EVENT_AND_ALARM_GREATER_THAN_OR_EQUAL_TO;
  mock_clear_operator = EVENT_AND_ALARM_LESS_THAN_OR_EQUAL_TO;
  TEST_ASSERT_FALSE(event_and_alarm_check_thresholds(&mock_base, mock_value, mock_alarm_state));

  /*
          Alarm Not Asserted ---> Alarm Not Asserted
        ------------------------------------------------------ <--- Max Value
          ....clear_threshold...    <--- mock_clear_threshold
            . <-- initial alarm state = 0
             .<-- mock value.
          ....set_threshold.....    <--- mock_set_threshold
        ------------------------------------------------------ <-- 0
  */
  mock_alarm_state = false;
  mock_value = 3.0;
  mock_set_threshold = 2.0;
  mock_clear_threshold = 4.0;
  mock_set_operator = EVENT_AND_ALARM_LESS_THAN_OR_EQUAL_TO;
  mock_clear_operator = EVENT_AND_ALARM_GREATER_THAN_OR_EQUAL_TO;
  TEST_ASSERT_FALSE(event_and_alarm_check_thresholds(&mock_base, mock_value, mock_alarm_state));

  /*
          Alarm Not Asserted ---> Alarm Asserted
        ------------------------------------------------------ <--- Max Value
          ....clear_threshold...    <--- mock_clear_threshold
            . <-- initial alarm state = 0
          ....set_threshold.....    <--- mock_set_threshold
              .<-- mock value
        ------------------------------------------------------ <-- 0
  */
  mock_alarm_state = false;
  mock_value = 1.0;
  mock_set_threshold = 2.0;
  mock_clear_threshold = 3.0;
  mock_set_operator = EVENT_AND_ALARM_LESS_THAN_OR_EQUAL_TO;
  mock_clear_operator = EVENT_AND_ALARM_GREATER_THAN_OR_EQUAL_TO;
  TEST_ASSERT_TRUE(event_and_alarm_check_thresholds(&mock_base, mock_value, mock_alarm_state));

  /*
          Alarm Not Asserted ---> Alarm Not Asserted
        ------------------------------------------------------ <--- Max Value
              .<-- mock value
          ....clear_threshold...    <--- mock_clear_threshold
            . <-- initial alarm state = 0
          ....set_threshold.....    <--- mock_set_threshold
        ------------------------------------------------------ <-- 0
  */
  mock_alarm_state = false;
  mock_value = 4.0;
  mock_set_threshold = 2.0;
  mock_clear_threshold = 3.0;
  mock_set_operator = EVENT_AND_ALARM_LESS_THAN_OR_EQUAL_TO;
  mock_clear_operator = EVENT_AND_ALARM_GREATER_THAN_OR_EQUAL_TO;
  TEST_ASSERT_FALSE(event_and_alarm_check_thresholds(&mock_base, mock_value, mock_alarm_state));



  /////////////////////////////////
  // ALARM INITALLY TRUE CASES  ///
  /////////////////////////////////

  /*
          Alarm Asserted ---> Alarm Asserted
        ------------------------------------------------------ <--- Max Value
          ....set_threshold.....    <--- mock_set_threshold
            . <-- initial alarm state = 1
             .<-- mock value.
          ....clear_threshold...    <--- mock_clear_threshold
        ------------------------------------------------------ <-- 0
  */
  mock_alarm_state = true;
  mock_value = 1.0;
  mock_set_threshold = 2.0;
  mock_clear_threshold = 0.5;
  mock_set_operator = EVENT_AND_ALARM_GREATER_THAN_OR_EQUAL_TO;
  mock_clear_operator = EVENT_AND_ALARM_LESS_THAN_OR_EQUAL_TO;
  TEST_ASSERT_TRUE(event_and_alarm_check_thresholds(&mock_base, mock_value, mock_alarm_state));

  /*
          Alarm Asserted ---> Alarm Asserted
        ------------------------------------------------------ <--- Max Value
              .<-- mock value
          ....set_threshold.....    <--- mock_set_threshold
            . <-- initial alarm state = 1
          ....clear_threshold...    <--- mock_clear_threshold
        ------------------------------------------------------ <-- 0
  */
  mock_alarm_state = true;
  mock_value = 3.0;
  mock_set_threshold = 2.0;
  mock_clear_threshold = 0.5;
  mock_set_operator = EVENT_AND_ALARM_GREATER_THAN_OR_EQUAL_TO;
  mock_clear_operator = EVENT_AND_ALARM_LESS_THAN_OR_EQUAL_TO;
  TEST_ASSERT_TRUE(event_and_alarm_check_thresholds(&mock_base, mock_value, mock_alarm_state));

  /*
          Alarm Asserted ---> Alarm Not Asserted
        ------------------------------------------------------ <--- Max Value
          ....set_threshold.....    <--- mock_set_threshold
            . <-- initial alarm state = 1
          ....clear_threshold...    <--- mock_clear_threshold
              .<-- mock value
        ------------------------------------------------------ <-- 0
  */
  mock_alarm_state = true;
  mock_value = 1.0;
  mock_set_threshold = 3.0;
  mock_clear_threshold = 2.0;
  mock_set_operator = EVENT_AND_ALARM_GREATER_THAN_OR_EQUAL_TO;
  mock_clear_operator = EVENT_AND_ALARM_LESS_THAN_OR_EQUAL_TO;
  TEST_ASSERT_FALSE(event_and_alarm_check_thresholds(&mock_base, mock_value, mock_alarm_state));

  /*
          Alarm Asserted ---> Alarm Asserted
        ------------------------------------------------------ <--- Max Value
          ....clear_threshold...    <--- mock_clear_threshold
            . <-- initial alarm state = 1
             .<-- mock value
          ....set_threshold.....    <--- mock_set_threshold
        ------------------------------------------------------ <-- 0
  */
  mock_alarm_state = true;
  mock_value = 3.0;
  mock_set_threshold = 2.0;
  mock_clear_threshold = 4.0;
  mock_set_operator = EVENT_AND_ALARM_LESS_THAN_OR_EQUAL_TO;
  mock_clear_operator = EVENT_AND_ALARM_GREATER_THAN_OR_EQUAL_TO;
  TEST_ASSERT_TRUE(event_and_alarm_check_thresholds(&mock_base, mock_value, mock_alarm_state));

  /*
          Alarm Asserted ---> Alarm Asserted
        ------------------------------------------------------ <--- Max Value
          ....clear_threshold...    <--- mock_clear_threshold
            . <-- initial alarm state = 1
          ....set_threshold.....    <--- mock_set_threshold
              .<-- mock value
        ------------------------------------------------------ <-- 0
  */
  mock_alarm_state = true;
  mock_value = 1.0;
  mock_set_threshold = 2.0;
  mock_clear_threshold = 3.0;
  mock_set_operator = EVENT_AND_ALARM_LESS_THAN_OR_EQUAL_TO;
  mock_clear_operator = EVENT_AND_ALARM_GREATER_THAN_OR_EQUAL_TO;
  TEST_ASSERT_TRUE(event_and_alarm_check_thresholds(&mock_base, mock_value, mock_alarm_state));

  /*
          Alarm Asserted ---> Alarm Not Asserted
        ------------------------------------------------------ <--- Max Value
              .<-- mock value
          ....clear_threshold...    <--- mock_clear_threshold
            . <-- initial alarm state = 1
          ....set_threshold.....    <--- mock_set_threshold
        ------------------------------------------------------ <-- 0
  */
  mock_alarm_state = true;
  mock_value = 4.0;
  mock_set_threshold = 2.0;
  mock_clear_threshold = 3.0;
  mock_set_operator = EVENT_AND_ALARM_LESS_THAN_OR_EQUAL_TO;
  mock_clear_operator = EVENT_AND_ALARM_GREATER_THAN_OR_EQUAL_TO;
  TEST_ASSERT_FALSE(event_and_alarm_check_thresholds(&mock_base, mock_value, mock_alarm_state));



  /////////////////////////////////
  // ALARM POSSIBLE EDGE CASE  ///
  /////////////////////////////////

  /*
          Alarm Not Asserted ---> Alarm Asserted
        ------------------------------------------------------ <--- Max Value
          ....set_threshold.....  <--- mock_set_threshold <===> .<-- mock value
            . <-- initial alarm state = 0
          ....clear_threshold...  <--- mock_clear_threshold
        ------------------------------------------------------ <-- 0
  */
  mock_alarm_state = false;
  mock_value = 2.0;
  mock_set_threshold = 2.0;
  mock_clear_threshold = 0.5;
  mock_set_operator = EVENT_AND_ALARM_GREATER_THAN_OR_EQUAL_TO;
  mock_clear_operator = EVENT_AND_ALARM_LESS_THAN_OR_EQUAL_TO;
  TEST_ASSERT_TRUE(event_and_alarm_check_thresholds(&mock_base, mock_value, mock_alarm_state));

  /*
          Alarm Not Asserted ---> Alarm Asserted
        ------------------------------------------------------ <--- Max Value
          ....set_threshold..... <--- mock_set_threshold <===> .<-- mock value
            . <-- initial alarm state = 0
          ....clear_threshold...    <--- mock_clear_threshold
        ------------------------------------------------------ <-- 0
  */
  mock_alarm_state = false;
  mock_value = 1.999999;
  mock_set_threshold = 2.0;
  mock_clear_threshold = 0.5;
  mock_set_operator = EVENT_AND_ALARM_GREATER_THAN_OR_EQUAL_TO;
  mock_clear_operator = EVENT_AND_ALARM_LESS_THAN_OR_EQUAL_TO;
  TEST_ASSERT_TRUE(event_and_alarm_check_thresholds(&mock_base, mock_value, mock_alarm_state));

  /*
          Alarm Asserted ---> Alarm Not Asserted
        ------------------------------------------------------ <--- Max Value
          ....clear_threshold... <-- mock_clear_threshold <===> .<-- mock value
            . <-- initial alarm state = 1
          ....set_threshold.....    <--- mock_set_threshold.
        ------------------------------------------------------ <-- 0
  */
  mock_alarm_state = true;
  mock_value = 2.0;
  mock_set_threshold = 1.0;
  mock_clear_threshold = 2.0;
  mock_set_operator = EVENT_AND_ALARM_GREATER_THAN_OR_EQUAL_TO;
  mock_clear_operator = EVENT_AND_ALARM_LESS_THAN_OR_EQUAL_TO;
  TEST_ASSERT_FALSE(event_and_alarm_check_thresholds(&mock_base, mock_value, mock_alarm_state));

  /*
          Alarm Asserted ---> Alarm Not Asserted
        ------------------------------------------------------ <--- Max Value
          ....clear_threshold... <-- mock_clear_threshold <===> .<-- mock value
            . <-- initial alarm state = 1
          ....set_threshold.....    <--- mock_set_threshold
        ------------------------------------------------------ <-- 0
  */
  mock_alarm_state = true;
  mock_value = 1.999999;
  mock_set_threshold = 1.0;
  mock_clear_threshold = 2.;
  mock_set_operator = EVENT_AND_ALARM_GREATER_THAN_OR_EQUAL_TO;
  mock_clear_operator = EVENT_AND_ALARM_LESS_THAN_OR_EQUAL_TO;
  TEST_ASSERT_FALSE(event_and_alarm_check_thresholds(&mock_base, mock_value, mock_alarm_state));


  /////////////////////////////////
  // ALARM Negative EDGE CASE  ////
  /////////////////////////////////

  /*
          Alarm Not Asserted ---> Alarm Not Asserted
        ------------------------------------------------------ <--- Max Value
          ....set_threshold.....    <--- mock_set_threshold
            . <-- initial alarm state = 0
          ....clear_threshold...    <--- mock_clear_threshold
        ------------------------------------------------------ <-- 0

              .<-- mock value

  */
  mock_alarm_state = false;
  mock_value = -5.34;
  mock_set_threshold = 3.0;
  mock_clear_threshold = 2.0;
  mock_set_operator = EVENT_AND_ALARM_GREATER_THAN_OR_EQUAL_TO;
  mock_clear_operator = EVENT_AND_ALARM_LESS_THAN_OR_EQUAL_TO;
  TEST_ASSERT_FALSE(event_and_alarm_check_thresholds(&mock_base, mock_value, mock_alarm_state));

  /*
          Alarm Asserted ---> Alarm Asserted
        ------------------------------------------------------ <--- Max Value
          ....clear_threshold...    <--- mock_clear_threshold
            . <-- initial alarm state = 1
          ....set_threshold.....    <--- mock_set_threshold
        ------------------------------------------------------ <-- 0

              .<-- mock value

  */
  mock_alarm_state = true;
  mock_value = -10.0;
  mock_set_threshold = 2.0;
  mock_clear_threshold = 3.0;
  mock_set_operator = EVENT_AND_ALARM_LESS_THAN_OR_EQUAL_TO;
  mock_clear_operator = EVENT_AND_ALARM_GREATER_THAN_OR_EQUAL_TO;
  TEST_ASSERT_TRUE(event_and_alarm_check_thresholds(&mock_base, mock_value, mock_alarm_state));
}

void test_rate_limit_check(void)
{
  bool result = false;
  uint32_t time_s = 0;

  /* Edge Case */

  /* Edge Case: period window if set to 16bit will overflow if window period is too low */
  // Check that function is enabled correctly if limit exceeded
  result = false;
  time_s = 10000000; // Overflows 16bit register
  mock_base.var.event_count = 100;
  mock_base.var.period_window = 100000; /* This will throw a warning if this variable is incorrectly set to 16bit size */
  mock_max_event_count  = 100;
  mock_max_event_period = 100;
  result = event_and_alarm_base_rate_limit_check(time_s, &mock_base);
  TEST_ASSERT_TRUE(result);


  /* General Case*/

  // All set to zero (no limit should be seen)
  result = false;
  time_s = 0;
  mock_base.var.event_count = 0;
  mock_base.var.period_window = 0;
  mock_max_event_count  = 0;
  mock_max_event_period = 0;
  result = event_and_alarm_base_rate_limit_check(time_s, &mock_base);
  TEST_ASSERT_FALSE(result);

  // Check that function is disabled correctly if max event count is set to 0
  result = false;
  time_s = 0;
  mock_base.var.event_count = 100;
  mock_base.var.period_window = 0;
  mock_max_event_count  = 0;
  mock_max_event_period = 60;
  result = event_and_alarm_base_rate_limit_check(time_s, &mock_base);
  TEST_ASSERT_FALSE(result);

  // Check that function is disabled correctly if period is set to 0
  result = false;
  time_s = 0;
  mock_base.var.event_count = 100;
  mock_base.var.period_window = 0;
  mock_max_event_count  = 100;
  mock_max_event_period = 0;
  result = event_and_alarm_base_rate_limit_check(time_s, &mock_base);
  TEST_ASSERT_FALSE(result);

  // Check that function is disabled if just under limit
  result = false;
  time_s = 55;
  mock_base.var.event_count = 99;
  mock_base.var.period_window = 0;
  mock_max_event_count  = 100;
  mock_max_event_period = 60;
  result = event_and_alarm_base_rate_limit_check(time_s, &mock_base);
  TEST_ASSERT_FALSE(result);

  // Check that function is enabled correctly if limit exceeded
  result = false;
  time_s = 55;
  mock_base.var.event_count = 100;
  mock_base.var.period_window = 0;
  mock_max_event_count  = 100;
  mock_max_event_period = 60;
  result = event_and_alarm_base_rate_limit_check(time_s, &mock_base);
  TEST_ASSERT_TRUE(result);

  // Check that function is enabled correctly if limit exceeded
  result = false;
  time_s = 55;
  mock_base.var.event_count = 101;
  mock_base.var.period_window = 0;
  mock_max_event_count  = 100;
  mock_max_event_period = 60;
  result = event_and_alarm_base_rate_limit_check(time_s, &mock_base);
  TEST_ASSERT_TRUE(result);


  // Check that function is clears on new window
  result = false;
  time_s = 60;
  mock_base.var.event_count = 101;
  mock_base.var.period_window = 0;
  mock_max_event_count  = 100;
  mock_max_event_period = 60;
  result = event_and_alarm_base_rate_limit_check(time_s, &mock_base);
  TEST_ASSERT_FALSE(result);

  // Check that function is enabled correctly if incremented over the limit
  result = false;
  time_s = 55;
  mock_base.var.event_count = 99;
  mock_base.var.period_window = 0;
  mock_max_event_count  = 100;
  mock_max_event_period = 60;
  event_and_alarm_base_rate_limit_increment_event_count(&mock_base);
  result = event_and_alarm_base_rate_limit_check(time_s, &mock_base);
  TEST_ASSERT_TRUE(result);


}

void test_save_next_state(void)
{
  uint32_t event_type = 0;

  /*----------------------------------------------------------*/
  // If event type is in disabled state, should always be false
  event_type = EVENT_AND_ALARM_EVENT_TYPE_DISABLED;
  /*                                                       event_type   state_changed    next_state    */
  TEST_ASSERT_FALSE( event_and_alarm_base_save_next_state( event_type ,     false      ,  false     )); // SUPPRESSED
  TEST_ASSERT_FALSE( event_and_alarm_base_save_next_state( event_type ,     false      ,  true      )); // SUPPRESSED
  TEST_ASSERT_FALSE( event_and_alarm_base_save_next_state( event_type ,     true       ,  false     )); // SUPPRESSED
  TEST_ASSERT_FALSE( event_and_alarm_base_save_next_state( event_type ,     true       ,  true      )); // SUPPRESSED

  /*----------------------------------------------------------*/
  event_type = EVENT_AND_ALARM_EVENT_TYPE_ALARM_CURRENT_STATE;
  /*                                                       event_type   state_changed    next_state    */
  TEST_ASSERT_FALSE( event_and_alarm_base_save_next_state( event_type ,     false      ,  false     ));
  TEST_ASSERT_FALSE( event_and_alarm_base_save_next_state( event_type ,     false      ,  true      ));
  TEST_ASSERT_TRUE(  event_and_alarm_base_save_next_state( event_type ,     true       ,  false     ));  // Save this falling edge
  TEST_ASSERT_TRUE(  event_and_alarm_base_save_next_state( event_type ,     true       ,  true      ));  // Save this rising edge

  /*----------------------------------------------------------*/
  event_type = EVENT_AND_ALARM_EVENT_TYPE_ALARM_STATE_CHANGE_LOG;
  /*                                                       event_type   state_changed    next_state    */
  TEST_ASSERT_FALSE( event_and_alarm_base_save_next_state( event_type ,     false      ,  false     ));
  TEST_ASSERT_FALSE( event_and_alarm_base_save_next_state( event_type ,     false      ,  true      ));
  TEST_ASSERT_TRUE(  event_and_alarm_base_save_next_state( event_type ,     true       ,  false     ));  // Save this falling edge
  TEST_ASSERT_TRUE(  event_and_alarm_base_save_next_state( event_type ,     true       ,  true      ));  // Save this rising edge

  /*----------------------------------------------------------*/
  event_type = EVENT_AND_ALARM_EVENT_TYPE_EVENT_LOG;
  /*                                                       event_type   state_changed    next_state    */
  TEST_ASSERT_FALSE( event_and_alarm_base_save_next_state( event_type ,     false      ,  false     ));
  TEST_ASSERT_FALSE( event_and_alarm_base_save_next_state( event_type ,     false      ,  true      ));
  TEST_ASSERT_FALSE( event_and_alarm_base_save_next_state( event_type ,     true       ,  false     ));
  TEST_ASSERT_TRUE(  event_and_alarm_base_save_next_state( event_type ,     true       ,  true      ));  // Save this rising edge

}


/*******************************************************************************
  PAYLOAD GENERATOR TEST
*******************************************************************************/

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
/*

Learning CBOR?
* Works very well and was used for testing generated vectors : cbor.me
* Works well... except for understanding indefinate arrays: https://github.com/cabo/cbor-diag/blob/master/README.md

You can alternatively use http://cbor.me/ to inspect the test vectors

*/
void test_payload_generator(void) {
  cbor_stream_t *stream = &(mock_base.var.payload.stream);

  event_and_alarm_base_payload_init(&mock_base);

  /*
  JSON: [123, 1488775953, 2, 1]
  --> `json2cbor.rb test.json | xxd -i` (via cbor-diag)
  CBOR: 0x84, 0x18, 0x7b, 0x1a, 0x58, 0xbc, 0xeb, 0x11, 0x02, 0x01
  */

  /*  Testing current state alarm
  ********************************/
  mock_event_code = 123;
  mock_event_type = EVENT_AND_ALARM_EVENT_TYPE_ALARM_CURRENT_STATE;
  mock_alarm_state = true;
  event_and_alarm_base_payload_head(&mock_base, 0);
                                                                                /* | timestamp | Sensor | alarm | */
  event_and_alarm_base_payload_alarm_current_state_generate(&mock_base,                3453,       463,     true); // Should be overwritten
  event_and_alarm_base_payload_alarm_current_state_generate(&mock_base,                6366,       234,     true); // Should be overwritten
  event_and_alarm_base_payload_alarm_current_state_generate(&mock_base,                666,        0,       true);

  event_and_alarm_base_payload_footer(&mock_base);

  test_utility_cbor_stream_print_hex(stream, 0, "curr state check");            // 9f 18 7b 19 02 9a 02 01 ff --> cbor.me --> [123, 666, 2, 1]

  { // Verification
    uint8_t expected[] = {0x84, 0x18, 0x7b, 0x01, 0x19, 0x02, 0x9a, 0x01}; (void)(expected);
    TEST_ASSERT_EQUAL_INT8_ARRAY(expected, stream->data, sizeof(expected)/sizeof(expected[0]) );
  }

  /*  Testing for timestamp overflow using current state alarm scenario
  **********************************************************************/
  mock_event_code = 123;
  mock_event_type = EVENT_AND_ALARM_EVENT_TYPE_ALARM_CURRENT_STATE;
  mock_alarm_state = true;
  event_and_alarm_base_payload_head(&mock_base, 0);
                                                                                /* | timestamp    | Sensor |  alarm | */
  event_and_alarm_base_payload_alarm_current_state_generate(&mock_base,                3453,          463,      true); // Should be overwritten
  event_and_alarm_base_payload_alarm_current_state_generate(&mock_base,                6366,          234,      true); // Should be overwritten
  event_and_alarm_base_payload_alarm_current_state_generate(&mock_base,                1488775953,    234,      true);

  event_and_alarm_base_payload_footer(&mock_base);

  test_utility_cbor_stream_print_hex(stream, 0, "timestamp overflow check");

  { // Verification
    char expected[] = {0x84, 0x18, 0x7b, 0x01, 0x1a, 0x58, 0xbc, 0xeb, 0x11, 0x01}; (void)(expected);
    TEST_ASSERT_EQUAL_INT8_ARRAY(expected, stream->data, sizeof(expected)/sizeof(expected[0]) );
  }

  /*  Alarm state change log scenario
  *************************************************************************/
  mock_event_code = 123;
  mock_event_type = EVENT_AND_ALARM_EVENT_TYPE_ALARM_STATE_CHANGE_LOG;
  mock_alarm_state = true;
  event_and_alarm_base_payload_head(&mock_base, 3);
                                                                                /* | timestamp    | Sensor |  alarm | */
  event_and_alarm_base_payload_alarm_state_change_log_generate(&mock_base,             1488775951,    0,        true);
  event_and_alarm_base_payload_alarm_state_change_log_generate(&mock_base,             1488775952,    200,      true);
  event_and_alarm_base_payload_alarm_state_change_log_generate(&mock_base,             1488775953,    400,      true);

  event_and_alarm_base_payload_footer(&mock_base);

  test_utility_cbor_stream_print_hex(stream, 0, "state change");

  { // Verification
    char expected[] = {
                        0x83, 0x18, 0x7b, 0x02, 0x83, 0x83, 0x1a, 0x58, 0xbc,
                        0xeb, 0x0f, 0x00, 0x01, 0x83, 0x1a, 0x58, 0xbc, 0xeb,
                        0x10, 0x18, 0xc8, 0x01, 0x83, 0x1a, 0x58, 0xbc, 0xeb,
                        0x11, 0x19, 0x01, 0x90, 0x01
                      }; (void)(expected);
    TEST_ASSERT_EQUAL_INT8_ARRAY(expected, stream->data, sizeof(expected)/sizeof(expected[0]) );
  }



  /*  Event log scenario
  *************************************************************************/
  mock_event_code = 123;
  mock_event_type = EVENT_AND_ALARM_EVENT_TYPE_EVENT_LOG;
  mock_alarm_state = true;
  event_and_alarm_base_payload_head(&mock_base, 3);
                                                                                 /* | timestamp    | Sensor |  alarm | */
  event_and_alarm_base_payload_event_log_generate(&mock_base,                           1488775951,    0,        true);
  event_and_alarm_base_payload_event_log_generate(&mock_base,                           1488775952,    200,      true);
  event_and_alarm_base_payload_event_log_generate(&mock_base,                           1488775953,    400,      true);

  event_and_alarm_base_payload_footer(&mock_base);

  test_utility_cbor_stream_print_hex(stream, 0, "event log");

  { // Verification
    char expected[] = {
                        0x83, 0x18, 0x7b, 0x03, 0x83, 0x83, 0x1a, 0x58, 0xbc,
                        0xeb, 0x0f, 0x00, 0x01, 0x83, 0x1a, 0x58, 0xbc, 0xeb,
                        0x10, 0x18, 0xc8, 0x01, 0x83, 0x1a, 0x58, 0xbc, 0xeb,
                        0x11, 0x19, 0x01, 0x90, 0x01
                      }; (void)(expected);
    TEST_ASSERT_EQUAL_INT8_ARRAY(expected, stream->data, sizeof(expected)/sizeof(expected[0]) );
  }

}



void test_logger(void)
{
  cbor_stream_t *stream = &(mock_base.var.payload.stream);

  bool test = 0;

  // Size check
  test = sizeof(mock_base.var.payload) < EVENT_AND_ALARM_BASE_PAYLOAD_BYTES_MAXIMUM;
  if (!test)
  {
    log_warn("payload size: %lu (max: %luB)", sizeof(mock_base.var.payload), EVENT_AND_ALARM_BASE_PAYLOAD_BYTES_MAXIMUM);
  }
  TEST_ASSERT_TRUE(test);



  /*  Current State log scenario
  **********************************************************************/
  mock_event_code = 123;
  mock_event_type = EVENT_AND_ALARM_EVENT_TYPE_ALARM_CURRENT_STATE;
  mock_alarm_state = true;
  event_and_alarm_base_logger_clear(&mock_base);
                                                    /* | timestamp    | Sensor |  alarm | */
  event_and_alarm_base_logger_record(&mock_base,         3453,          463,      true); // Should be overwritten
  event_and_alarm_base_logger_record(&mock_base,         6366,          234,      true); // Should be overwritten
  event_and_alarm_base_logger_record(&mock_base,         1488775953,    234,      true);

  event_and_alarm_base_logger_generate_payload(&mock_base);

  test_utility_cbor_stream_print_hex(stream, 0, "timestamp overflow check");

  { // Verification
    char expected[] = {0x84, 0x18, 0x7b, 0x01, 0x1a, 0x58, 0xbc, 0xeb, 0x11, 0x01}; (void)(expected);


    TEST_ASSERT_EQUAL_INT8_ARRAY(expected, stream->data, sizeof(expected)/sizeof(expected[0]) );
  }



  /*  Alarm state change log scenario
  *************************************************************************/
  mock_event_code = 123;
  mock_event_type = EVENT_AND_ALARM_EVENT_TYPE_ALARM_STATE_CHANGE_LOG;
  mock_alarm_state = true;
  event_and_alarm_base_logger_clear(&mock_base);
                                                    /* | timestamp    | Sensor |  alarm | */
  event_and_alarm_base_logger_record(&mock_base,         1488775951,    0,        true);
  event_and_alarm_base_logger_record(&mock_base,         1488775952,    200,      true);
  event_and_alarm_base_logger_record(&mock_base,         1488775953,    400,      true);

  event_and_alarm_base_logger_generate_payload(&mock_base);

  test_utility_cbor_stream_print_hex(stream, 0, "state change");

  { // Verification
    char expected[] = {
                        0x83, 0x18, 0x7b, 0x02, 0x83, 0x83, 0x1a, 0x58, 0xbc,
                        0xeb, 0x0f, 0x00, 0x01, 0x83, 0x1a, 0x58, 0xbc, 0xeb,
                        0x10, 0x18, 0xc8, 0x01, 0x83, 0x1a, 0x58, 0xbc, 0xeb,
                        0x11, 0x19, 0x01, 0x90, 0x01
                      }; (void)(expected);
    TEST_ASSERT_EQUAL_INT8_ARRAY(expected, stream->data, sizeof(expected)/sizeof(expected[0]) );
  }

  /* Event Log
  ***************************/
  mock_event_code = 123;
  mock_event_type = EVENT_AND_ALARM_EVENT_TYPE_EVENT_LOG;
  mock_alarm_state = true;
  event_and_alarm_base_logger_clear(&mock_base);
                                                    /* | timestamp    | Sensor |  alarm | */
  event_and_alarm_base_logger_record(&mock_base,         1488775951,    0,        true);
  event_and_alarm_base_logger_record(&mock_base,         1488775952,    200,      true);
  event_and_alarm_base_logger_record(&mock_base,         1488775953,    400,      true);

  event_and_alarm_base_logger_generate_payload(&mock_base);

  test_utility_cbor_stream_print_hex(stream, 0, "event log");

  { // Verification
    char expected[] = {
                        0x83, 0x18, 0x7b, 0x03, 0x83, 0x83, 0x1a, 0x58, 0xbc,
                        0xeb, 0x0f, 0x00, 0x01, 0x83, 0x1a, 0x58, 0xbc, 0xeb,
                        0x10, 0x18, 0xc8, 0x01, 0x83, 0x1a, 0x58, 0xbc, 0xeb,
                        0x11, 0x19, 0x01, 0x90, 0x01
                      }; (void)(expected);
    TEST_ASSERT_EQUAL_INT8_ARRAY(expected, stream->data, sizeof(expected)/sizeof(expected[0]) );
  }

  /* Check latest payload and confirmation interface
  ****************************************************/
  mock_event_code = 123;
  mock_event_type = EVENT_AND_ALARM_EVENT_TYPE_EVENT_LOG;
  mock_alarm_state = true;
  event_and_alarm_base_logger_clear(&mock_base);
  event_and_alarm_base_logger_record(&mock_base,         1488775951,    0,        true);
  event_and_alarm_base_logger_record(&mock_base,         1488775952,    200,      true);
  event_and_alarm_base_logger_record(&mock_base,         1488775953,    400,      true);
  event_and_alarm_base_logger_generate_payload(&mock_base);

  { // Verification (Note that first byte must be 0x02. It is the lwm2m type code for CBOR )
    char expected[] = {
                        0x02,
                        0x83, 0x18, 0x7b, 0x03, 0x83, 0x83, 0x1a, 0x58, 0xbc,
                        0xeb, 0x0f, 0x00, 0x01, 0x83, 0x1a, 0x58, 0xbc, 0xeb,
                        0x10, 0x18, 0xc8, 0x01, 0x83, 0x1a, 0x58, 0xbc, 0xeb,
                        0x11, 0x19, 0x01, 0x90, 0x01
                      }; (void)(expected);
    uint8_t *val_ptr = 0;
    size_t length = 0;

    // A server calling this, would generate and return the payload
    TEST_ASSERT_TRUE( event_and_alarm_base_Latest_Payload_get(0, &val_ptr, &length) == COAP_205_CONTENT );
    TEST_ASSERT_EQUAL_INT8_ARRAY(expected, val_ptr, sizeof(expected)/sizeof(expected[0]));

    // A server calling this, would generate and return the payload (With no change!)
    TEST_ASSERT_TRUE( event_and_alarm_base_Latest_Payload_get(0, &val_ptr, &length) == COAP_205_CONTENT );
    TEST_ASSERT_EQUAL_INT8_ARRAY(expected, val_ptr, sizeof(expected)/sizeof(expected[0]));

    // But on confirmation, payload is destroyed
    event_and_alarm_base_alarm_latest_payload_confirmed(NULL, 0, 0);

    // Now payload is empty (Thus return size and pointer is sent to zero)
    TEST_ASSERT_TRUE(event_and_alarm_base_Latest_Payload_get(0, &val_ptr, &length) == COAP_205_CONTENT );
    TEST_ASSERT_TRUE(val_ptr == 0);
    TEST_ASSERT_TRUE(length == 0);
  }


}



void event_and_alarm_tests_run(void)
{

  event_and_alarm_base_initalise_instance(&mock_base, &mock_object_declaration);

  RUN_TEST(test_threshold_breached);
  RUN_TEST(test_rate_limit_check);
  RUN_TEST(test_save_next_state);
  RUN_TEST(test_payload_generator);
  RUN_TEST(test_logger);
}
