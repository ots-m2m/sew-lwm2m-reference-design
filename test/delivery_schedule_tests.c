/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "delivery_schedule.h"
#include "unity.h"
#include "log.h"
#include "utc_offset_parser.h" // utc_offset_parse()

#define TEST_INSTANCE_ID 0

// Specification Default Value as of July 2017
#define DEFAULT_SCHEDULE_START_TIME           120
#define DEFAULT_SCHEDULE_UTC_OFFSET           "UTC+10"
#define DEFAULT_DELIVERY_FREQENCY             86400
#define DEFAULT_RANDOMISED_DELIVERY_WINDOW    25200
#define DEFAULT_NUMBER_OF_RETRIES             2
#define DEFAULT_RETRY_PERIOD                  1800


void test_generate_random_window_delay_s(void)
{
  uint32_t same_count = 0;
  uint32_t generated_value = 0;
  uint32_t previous_value = 0;

  // Should not see same value per call more than 3 times
  for (int i = 0 ; i < 100 ; i++)
  {
    generated_value = generate_random_window_delay_s(100000);
    if (generated_value == previous_value)
    {
        same_count++;
    }
    TEST_ASSERT_UNLESS(same_count > 3); // generate_random_window_delay_s doesn't change value per call
    previous_value = generated_value;
  }

  // Should not exceed window
#define X(seconds) for (int i = 0; i < 100 ; i++){ TEST_ASSERT(generate_random_window_delay_s(seconds) <= seconds); }
  X(0);
  X(10);
  X(100);
  X(1000);
  X(10000);
  X(100000);
  X(1000000);
#undef X
}


void test_delivery_schedule(void)
{
  uint16_t instance           = 0;
  uint32_t next_tx_s          = 0;
  uint32_t next_win_s         = 0;
  uint32_t next_retry_s       = 0;
  uint32_t retry_count        = 0;
  bool transmission_active    = false;
  bool transmission_confirmed = false;

  // Setting up test time
  uint32_t mock_unix_time = 0;
  mock_unix_time = 86400000; // 1000 days since unix epoch at midnight
  mock_unix_time -= 60*60*2; // Subtract 2 hours before midnight
  mock_unix_time -= utc_offset_parse(DEFAULT_SCHEDULE_UTC_OFFSET);  // Midnight in reference to local time

  uint32_t till_next_tx_s = 0;

  Delivery_Schedule_object_reset_instance(instance);

  // Set default
  Delivery_Schedule_Schedule_Start_Time_set(instance,         DEFAULT_SCHEDULE_START_TIME       , false);
  Delivery_Schedule_Schedule_UTC_Offset_set(instance,         DEFAULT_SCHEDULE_UTC_OFFSET       , sizeof(DEFAULT_SCHEDULE_UTC_OFFSET), false);
  Delivery_Schedule_Delivery_Frequency_set(instance,          DEFAULT_DELIVERY_FREQENCY         , false);
  Delivery_Schedule_Randomised_Delivery_Window_set(instance,  DEFAULT_RANDOMISED_DELIVERY_WINDOW, false);
  Delivery_Schedule_Number_of_Retries_set(instance,           DEFAULT_NUMBER_OF_RETRIES         , false);
  Delivery_Schedule_Retry_Period_set(instance,                DEFAULT_RETRY_PERIOD              , false);

  // Set custom
  Delivery_Schedule_Schedule_UTC_Offset_set(instance,         DEFAULT_SCHEDULE_UTC_OFFSET       , sizeof(DEFAULT_SCHEDULE_UTC_OFFSET), false);
  Delivery_Schedule_Delivery_Frequency_set(instance,          86400                             , false); // Set delivery freqency to every 1 days
  Delivery_Schedule_Schedule_Start_Time_set(instance,         120                               , false); // first trigger 2 minutes after midnight
  Delivery_Schedule_Randomised_Delivery_Window_set(instance,  0                                 , false); // Disable randomness to ensure determanistic behaviour

  // Initial State
  Delivery_Schedule_tick(mock_unix_time);
  till_next_tx_s = Delivery_Schedule_Diagnostics(instance, mock_unix_time, &next_tx_s, &next_win_s, &next_retry_s, &retry_count, &transmission_active, &transmission_confirmed);
#ifdef TEST_DELIVERY_SCHEDULE_DEBUG_MODE
  log_debug("next TX in %luhr (%lumins)", till_next_tx_s/(60*60), till_next_tx_s/60);
#endif

  // Check Initial State
  TEST_ASSERT_FALSE(transmission_active);
  TEST_ASSERT_FALSE(transmission_confirmed);
  TEST_ASSERT_EQUAL_INT32( 60*60*2 + 120, till_next_tx_s); // 2 hours + 2 minutes

  mock_unix_time += till_next_tx_s; // Jump to next trigger point

  // Triggered
  Delivery_Schedule_tick(mock_unix_time);
  till_next_tx_s = Delivery_Schedule_Diagnostics(instance, mock_unix_time, &next_tx_s, &next_win_s, &next_retry_s, &retry_count, &transmission_active, &transmission_confirmed);
#ifdef TEST_DELIVERY_SCHEDULE_DEBUG_MODE
  log_debug("next TX in %luhr (%lumins)", till_next_tx_s/(60*60), till_next_tx_s/60);
#endif
  TEST_ASSERT_TRUE(transmission_active);
  TEST_ASSERT_FALSE(transmission_confirmed);
  TEST_ASSERT_EQUAL_INT32( 60*30, till_next_tx_s); // 30mins

  // Confirm Immedately
  Delivery_Schedule_confirm(0);
  Delivery_Schedule_tick(mock_unix_time);
  till_next_tx_s = Delivery_Schedule_Diagnostics(instance, mock_unix_time, &next_tx_s, &next_win_s, &next_retry_s, &retry_count, &transmission_active, &transmission_confirmed);
#ifdef TEST_DELIVERY_SCHEDULE_DEBUG_MODE
  log_debug("next TX in %luhr (%lumins)", till_next_tx_s/(60*60), till_next_tx_s/60);
#endif
  TEST_ASSERT_TRUE(transmission_confirmed);

  // Expect next trigger in 24hr
  TEST_ASSERT_EQUAL_INT32(86400, till_next_tx_s);

  mock_unix_time += till_next_tx_s; // Jump to next trigger point

  // Triggered
  Delivery_Schedule_tick(mock_unix_time);
  till_next_tx_s = Delivery_Schedule_Diagnostics(instance, mock_unix_time, &next_tx_s, &next_win_s, &next_retry_s, &retry_count, &transmission_active, &transmission_confirmed);
#ifdef TEST_DELIVERY_SCHEDULE_DEBUG_MODE
  log_debug("next TX in %luhr (%lumins)", till_next_tx_s/(60*60), till_next_tx_s/60);
#endif
  TEST_ASSERT_TRUE(transmission_active);
  TEST_ASSERT_FALSE(transmission_confirmed);
  TEST_ASSERT_EQUAL_INT32( 60*30, till_next_tx_s); // 30mins
  TEST_ASSERT_EQUAL_INT32(0, retry_count);

  mock_unix_time += till_next_tx_s; // Jump to next trigger point

  // Retry 1
  Delivery_Schedule_tick(mock_unix_time);
  till_next_tx_s = Delivery_Schedule_Diagnostics(instance, mock_unix_time, &next_tx_s, &next_win_s, &next_retry_s, &retry_count, &transmission_active, &transmission_confirmed);
#ifdef TEST_DELIVERY_SCHEDULE_DEBUG_MODE
  log_debug("next TX in %luhr (%lumins)", till_next_tx_s/(60*60), till_next_tx_s/60);
#endif
  TEST_ASSERT_TRUE(transmission_active);
  TEST_ASSERT_FALSE(transmission_confirmed);
  TEST_ASSERT_EQUAL_INT32( 60*30, till_next_tx_s); // 30mins
  TEST_ASSERT_EQUAL_INT32(1, retry_count);

  mock_unix_time += till_next_tx_s; // Jump to next trigger point

  // Retry 2
  Delivery_Schedule_tick(mock_unix_time);
  till_next_tx_s = Delivery_Schedule_Diagnostics(instance, mock_unix_time, &next_tx_s, &next_win_s, &next_retry_s, &retry_count, &transmission_active, &transmission_confirmed);
#ifdef TEST_DELIVERY_SCHEDULE_DEBUG_MODE
  log_debug("next TX in %luhr (%lumins)", till_next_tx_s/(60*60), till_next_tx_s/60);
#endif
  TEST_ASSERT_TRUE(transmission_active);
  TEST_ASSERT_FALSE(transmission_confirmed);
  TEST_ASSERT_EQUAL_INT32( 60*30, till_next_tx_s); // 30mins
  TEST_ASSERT_EQUAL_INT32(2, retry_count);

  mock_unix_time += till_next_tx_s; // Jump to next trigger point

  // Retry 3 ( Stop sending and wait for next window instead )
  Delivery_Schedule_tick(mock_unix_time);
  till_next_tx_s = Delivery_Schedule_Diagnostics(instance, mock_unix_time, &next_tx_s, &next_win_s, &next_retry_s, &retry_count, &transmission_active, &transmission_confirmed);
#ifdef TEST_DELIVERY_SCHEDULE_DEBUG_MODE
  log_debug("next TX in %luhr (%lumins)", till_next_tx_s/(60*60), till_next_tx_s/60);
#endif
  TEST_ASSERT_FALSE(transmission_active);          // Transmission now turned off
  TEST_ASSERT_FALSE(transmission_confirmed);

  TEST_ASSERT_EQUAL_INT32( 86400 - 3*30*60, till_next_tx_s); // 24hours - 3 * 30 minutes send attempt

  mock_unix_time += till_next_tx_s; // Jump to next trigger point

  // Triggered (Needs to trigger like normal even if previously there was a timeout)
  Delivery_Schedule_tick(mock_unix_time);
  till_next_tx_s = Delivery_Schedule_Diagnostics(instance, mock_unix_time, &next_tx_s, &next_win_s, &next_retry_s, &retry_count, &transmission_active, &transmission_confirmed);
#ifdef TEST_DELIVERY_SCHEDULE_DEBUG_MODE
  log_debug("next TX in %luhr (%lumins)", till_next_tx_s/(60*60), till_next_tx_s/60);
#endif
  TEST_ASSERT_TRUE(transmission_active);
  TEST_ASSERT_FALSE(transmission_confirmed);
  TEST_ASSERT_EQUAL_INT32( 60*30, till_next_tx_s); // 30mins
  TEST_ASSERT_EQUAL_INT32(0, retry_count);
}



//#define DELIVERY_SCHEDULE_VISUAL_TEST
#ifdef DELIVERY_SCHEDULE_VISUAL_TEST // Visual Test of Delivery Schedule   // TODO: Convert to actual unit test
void test_delivery_schedule_visual(void)
{
  uint16_t instance           = 0;
  uint32_t next_tx_s          = 0;
  uint32_t next_win_s         = 0;
  uint32_t next_retry_s       = 0;
  uint32_t retry_count        = 0;
  bool transmission_active    = false;
  bool transmission_confirmed = false;

  Delivery_Schedule_object_reset_instance(instance);

  uint32_t mock_unix_time = 0;
  mock_unix_time = 86400000; // 1000 days since unix epoch at midnight
  mock_unix_time -= 60*60*2; // Subtract 2 hours before midnight
  mock_unix_time -= utc_offset_parse(DEFAULT_SCHEDULE_UTC_OFFSET);  // Midnight in reference to local time

  uint32_t till_next_tx_s = 0;
  uint32_t skip_s = 0;

  // Set default
  Delivery_Schedule_Schedule_Start_Time_set(instance,         DEFAULT_SCHEDULE_START_TIME       , false);
  Delivery_Schedule_Schedule_UTC_Offset_set(instance,         DEFAULT_SCHEDULE_UTC_OFFSET       , sizeof(DEFAULT_SCHEDULE_UTC_OFFSET), false);
  Delivery_Schedule_Delivery_Frequency_set(instance,          DEFAULT_DELIVERY_FREQENCY         , false);
  Delivery_Schedule_Randomised_Delivery_Window_set(instance,  DEFAULT_RANDOMISED_DELIVERY_WINDOW, false);
  Delivery_Schedule_Number_of_Retries_set(instance,           DEFAULT_NUMBER_OF_RETRIES         , false);
  Delivery_Schedule_Retry_Period_set(instance,                DEFAULT_RETRY_PERIOD              , false);

  // Set custom
  Delivery_Schedule_Schedule_UTC_Offset_set(instance,         DEFAULT_SCHEDULE_UTC_OFFSET       , sizeof(DEFAULT_SCHEDULE_UTC_OFFSET), false);
  Delivery_Schedule_Schedule_Start_Time_set(instance,         120                               , false); // first trigger 2 minutes after midnight
  //Delivery_Schedule_Randomised_Delivery_Window_set(instance,  0                                 , false); // Disable randomness to ensure determanistic behaviour


  // Init
  Delivery_Schedule_tick(mock_unix_time);

  for (uint32_t i = 0 ; i < 200 ; i++)
  {
    till_next_tx_s = Delivery_Schedule_Diagnostics(instance, mock_unix_time, &next_tx_s, &next_win_s, &next_retry_s, &retry_count, &transmission_active, &transmission_confirmed);

    // Print
    log_debug("tx: %4lu mins (%2luh) | win: %4lu mins (%2luh) | retry: %4lu mins (%2luh) | rty: %u | tx-on: %u",
        next_tx_s/60, next_tx_s/(60*60),
        next_win_s/60, next_win_s/(60*60),
        next_retry_s/60, next_retry_s/(60*60),
        retry_count,
        transmission_active
      );

    // Skip Logic
    if ( 3600 < till_next_tx_s )
    { // Skip Ahead time
      skip_s = till_next_tx_s - 3600;
      log_debug("SKIPPING %2u hours (%4us)", skip_s/(60*60), skip_s);
      mock_unix_time += skip_s;  // hour increment
    }
    else
    { // Slow scan
      mock_unix_time += (10*60);  // 10 mins increment

      // Check if confirmation works
      if (transmission_active && (retry_count == 1))
      {
        log_debug("Mock Messaged confirmation");
        Delivery_Schedule_confirm(0);
      }
    }


    // TICK
    Delivery_Schedule_tick(mock_unix_time);
  }
}
#endif


void test_initialisation_check_default_values(void)
{
  uint16_t instance_id = 0;

#if 0 // Test that the checker works
  // Set default
  Delivery_Schedule_Schedule_Start_Time_set(instance_id,        DEFAULT_SCHEDULE_START_TIME       , false);
  Delivery_Schedule_Schedule_UTC_Offset_set(instance_id,        DEFAULT_SCHEDULE_UTC_OFFSET       , sizeof(DEFAULT_SCHEDULE_UTC_OFFSET), false);
  Delivery_Schedule_Randomised_Delivery_Window_set(instance_id, DEFAULT_RANDOMISED_DELIVERY_WINDOW, false);
  Delivery_Schedule_Number_of_Retries_set(instance_id,          DEFAULT_NUMBER_OF_RETRIES         , false);
  Delivery_Schedule_Retry_Period_set(instance_id,               DEFAULT_RETRY_PERIOD              , false);
#endif

  // Check that it is written to correctly
  uint32_t    start_time = 0;                 ///< number of seconds past midnight for which this schedule commences
  const char  *UTC_offset_seconds = NULL;     ///< Time zone offset for the Schedule Start Time Resource for this Delivery Schedule instance
  uint32_t    delivery_freqency = 0;          ///< number of seconds between deliveries
  uint32_t    randomised_delivery_window = 0; ///< randomisation widow in seconds for the triggering of the notification delivery to the LwM2M server
  uint32_t    number_of_retries = 0;          ///< number of retry attempts that should be attempted when the delivery notification is not successful
  uint32_t    retry_period = 0;               ///< number of seconds between each application level retry

  Delivery_Schedule_Schedule_Start_Time_get(instance_id,        &start_time                 );
  Delivery_Schedule_Schedule_UTC_Offset_get(instance_id,        &UTC_offset_seconds         );
  Delivery_Schedule_Delivery_Frequency_get(instance_id,         &delivery_freqency          );
  Delivery_Schedule_Randomised_Delivery_Window_get(instance_id, &randomised_delivery_window );
  Delivery_Schedule_Number_of_Retries_get(instance_id,          &number_of_retries          );
  Delivery_Schedule_Retry_Period_get(instance_id,               &retry_period               );

  TEST_ASSERT_EQUAL_INT32(      DEFAULT_SCHEDULE_START_TIME        , start_time                 );
  TEST_ASSERT_EQUAL_STRING_LEN( DEFAULT_SCHEDULE_UTC_OFFSET        , UTC_offset_seconds         , sizeof(DEFAULT_SCHEDULE_UTC_OFFSET));
  TEST_ASSERT_EQUAL_INT32(      DEFAULT_DELIVERY_FREQENCY          , delivery_freqency          );
  TEST_ASSERT_EQUAL_INT32(      DEFAULT_RANDOMISED_DELIVERY_WINDOW , randomised_delivery_window );
  TEST_ASSERT_EQUAL_INT32(      DEFAULT_NUMBER_OF_RETRIES          , number_of_retries          );
  TEST_ASSERT_EQUAL_INT32(      DEFAULT_RETRY_PERIOD               , retry_period               );
}


// for log debug to reference
#define GET_INTERVAL_TIME(time, offset, period, num_intervals_ahead) \
   ((((time - offset) / period) + num_intervals_ahead) * period) + offset

void test_delivery_schedule_initial_state_check(void)
{ /* This checks that alignement to every midnight is done correctly */
  uint16_t instance           = 0;
  uint32_t next_tx_s          = 0;
  uint32_t next_win_s         = 0;
  uint32_t next_retry_s       = 0;
  uint32_t retry_count        = 0;
  bool transmission_active    = false;
  bool transmission_confirmed = false;
  uint32_t till_next_tx_s = 0;

  uint32_t offset_s = 0; // Start Time
  uint32_t period_s = 0; // Delivery Freqency
  uint32_t intervals_ahead_s = 1; // Intervals Ahead
  uint32_t utc_offset_s = utc_offset_parse(DEFAULT_SCHEDULE_UTC_OFFSET);
  (void) intervals_ahead_s; // Used in log_debug


  { // Before Midnight
#ifdef TEST_DELIVERY_SCHEDULE_DEBUG_MODE
    log_debug("2 hour before midnight");
#endif

    period_s = 86400; // Delivery Freqency
    offset_s = 0; // Start Time

    Delivery_Schedule_object_reset_instance(instance);

    // Setting up test time
    uint32_t mock_unix_time = 0;
    mock_unix_time = 86400000; // 1000 days since unix epoch at midnight
    mock_unix_time -= 60*60*2; // Subtract 2 hours before midnight
    mock_unix_time -= utc_offset_s;  // Midnight in reference to local time

    // Set default
    Delivery_Schedule_Schedule_Start_Time_set(instance,         DEFAULT_SCHEDULE_START_TIME       , false);
    Delivery_Schedule_Schedule_UTC_Offset_set(instance,         DEFAULT_SCHEDULE_UTC_OFFSET       , sizeof(DEFAULT_SCHEDULE_UTC_OFFSET), false);
    Delivery_Schedule_Delivery_Frequency_set(instance,          DEFAULT_DELIVERY_FREQENCY         , false);
    Delivery_Schedule_Randomised_Delivery_Window_set(instance,  DEFAULT_RANDOMISED_DELIVERY_WINDOW, false);
    Delivery_Schedule_Number_of_Retries_set(instance,           DEFAULT_NUMBER_OF_RETRIES         , false);
    Delivery_Schedule_Retry_Period_set(instance,                DEFAULT_RETRY_PERIOD              , false);

    // Set custom
    Delivery_Schedule_Schedule_UTC_Offset_set(instance,         DEFAULT_SCHEDULE_UTC_OFFSET       , sizeof(DEFAULT_SCHEDULE_UTC_OFFSET), false);
    Delivery_Schedule_Delivery_Frequency_set(instance,          period_s                          , false); // Set delivery freqency to every 1 days
    Delivery_Schedule_Schedule_Start_Time_set(instance,         offset_s                          , false); // first trigger 0 minutes after midnight
    Delivery_Schedule_Randomised_Delivery_Window_set(instance,  0                                 , false); // Disable randomness to ensure determanistic behaviour

    // Initial State
    Delivery_Schedule_tick(mock_unix_time);
    till_next_tx_s = Delivery_Schedule_Diagnostics(instance, mock_unix_time, &next_tx_s, &next_win_s, &next_retry_s, &retry_count, &transmission_active, &transmission_confirmed);
#ifdef TEST_DELIVERY_SCHEDULE_DEBUG_MODE
    log_debug("next TX in %luhr (%lumins)", till_next_tx_s/(60*60), till_next_tx_s/60);
    log_debug("time:%05lu offset:%lu period:%lu num_intervals_ahead:%lu --> GET_INTERVAL_TIME:%lu", (long unsigned) mock_unix_time, (long unsigned) offset_s, (long unsigned) period_s, (long unsigned) intervals_ahead_s, (long unsigned) GET_INTERVAL_TIME(mock_unix_time, offset_s, period_s, intervals_ahead_s));
    log_debug("next_win_s:%lu, transmission_active:%d, transmission_confirmed:%d", (long unsigned) next_win_s, (int) transmission_active, (int) transmission_confirmed);
    log_debug("next_transmit_window_s:%lu, GET_INTERVAL_TIME:%lu", (long unsigned)(mock_unix_time+next_win_s+utc_offset_s), (long unsigned) GET_INTERVAL_TIME(mock_unix_time, offset_s, period_s, intervals_ahead_s));
#endif

    /*
      ```
      curr midnight                                               next midnight
      |---------------------:-------------------------------------------|
      |<--- 22hr from last midnight alignment --->|<---   2hr --------->|
      |<--- time % 24hr ------------------------->|
      ```
    */

    // Check Initial State
    TEST_ASSERT_FALSE(transmission_active);
    TEST_ASSERT_FALSE(transmission_confirmed);
    TEST_ASSERT_EQUAL_INT32( 60*60*2, till_next_tx_s); // 2 hours
  }

  { // Before Midnight
#ifdef TEST_DELIVERY_SCHEDULE_DEBUG_MODE
    log_debug("2 hour after midnight");
#endif

    period_s = 86400; // Delivery Freqency
    offset_s = 0; // Start Time

    Delivery_Schedule_object_reset_instance(instance);

    // Setting up test time
    uint32_t mock_unix_time = 0;
    mock_unix_time = 86400000; // 1000 days since unix epoch at midnight
    mock_unix_time += 60*60*2; // Subtract 2 hours before midnight
    mock_unix_time -= utc_offset_s;  // Midnight in reference to local time

    // Set default
    Delivery_Schedule_Schedule_Start_Time_set(instance,         DEFAULT_SCHEDULE_START_TIME       , false);
    Delivery_Schedule_Schedule_UTC_Offset_set(instance,         DEFAULT_SCHEDULE_UTC_OFFSET       , sizeof(DEFAULT_SCHEDULE_UTC_OFFSET), false);
    Delivery_Schedule_Delivery_Frequency_set(instance,          DEFAULT_DELIVERY_FREQENCY         , false);
    Delivery_Schedule_Randomised_Delivery_Window_set(instance,  DEFAULT_RANDOMISED_DELIVERY_WINDOW, false);
    Delivery_Schedule_Number_of_Retries_set(instance,           DEFAULT_NUMBER_OF_RETRIES         , false);
    Delivery_Schedule_Retry_Period_set(instance,                DEFAULT_RETRY_PERIOD              , false);

    // Set custom
    Delivery_Schedule_Schedule_UTC_Offset_set(instance,         DEFAULT_SCHEDULE_UTC_OFFSET       , sizeof(DEFAULT_SCHEDULE_UTC_OFFSET), false);
    Delivery_Schedule_Delivery_Frequency_set(instance,          period_s                          , false); // Set delivery freqency to every 1 days
    Delivery_Schedule_Schedule_Start_Time_set(instance,         offset_s                          , false); // first trigger 0 minutes after midnight
    Delivery_Schedule_Randomised_Delivery_Window_set(instance,  0                                 , false); // Disable randomness to ensure determanistic behaviour

    // Initial State
    Delivery_Schedule_tick(mock_unix_time);
    till_next_tx_s = Delivery_Schedule_Diagnostics(instance, mock_unix_time, &next_tx_s, &next_win_s, &next_retry_s, &retry_count, &transmission_active, &transmission_confirmed);
  #ifdef TEST_DELIVERY_SCHEDULE_DEBUG_MODE
    log_debug("next TX in %luhr (%lumins)", till_next_tx_s/(60*60), till_next_tx_s/60);
    log_debug("time:%05lu offset:%lu period:%lu num_intervals_ahead:%lu --> GET_INTERVAL_TIME:%lu", (long unsigned) mock_unix_time, (long unsigned) offset_s, (long unsigned) period_s, (long unsigned) intervals_ahead_s, (long unsigned) GET_INTERVAL_TIME(mock_unix_time, offset_s, period_s, intervals_ahead_s));
    log_debug("next_win_s:%lu, transmission_active:%d, transmission_confirmed:%d", (long unsigned) next_win_s, (int) transmission_active, (int) transmission_confirmed);
    log_debug("next_transmit_window_s:%lu, GET_INTERVAL_TIME:%lu", (long unsigned)(mock_unix_time+next_win_s+utc_offset_s), (long unsigned) GET_INTERVAL_TIME(mock_unix_time, offset_s, period_s, intervals_ahead_s));
#endif

    /*
      ```
      curr midnight                                               next midnight
      |---------------------:-------------------------------------------|
      |<---   2hr --------->|<--- 22hr till next midnight alignment --->|
      |<--- time % 24hr --->|
      ```
    */

    // Check Initial State
    TEST_ASSERT_FALSE(transmission_active);
    TEST_ASSERT_FALSE(transmission_confirmed);
    TEST_ASSERT_EQUAL_INT32( 86400 - 60*60*2, till_next_tx_s); // 22 hours
  }

}

void delivery_schedule_tests_run(void)
{
  // Window Delay Generator
  RUN_TEST(test_generate_random_window_delay_s);

  Delivery_Schedule_initialise();
  Delivery_Schedule_create_instance(TEST_INSTANCE_ID);
  Delivery_Schedule_object_initialise_instance(TEST_INSTANCE_ID);
  RUN_TEST(test_initialisation_check_default_values);
  RUN_TEST(test_delivery_schedule);
  RUN_TEST(test_delivery_schedule_initial_state_check);

#if 0 // Check if underlying schedule change function is working
  Delivery_Schedule_Schedule_UTC_Offset_set(TEST_INSTANCE_ID, "UTC+12:34", sizeof("UTC+12:34"), true);
  Delivery_Schedule_changed(TEST_INSTANCE_ID);
#endif

#ifdef DELIVERY_SCHEDULE_VISUAL_TEST
  test_delivery_schedule_visual();
#endif

}
