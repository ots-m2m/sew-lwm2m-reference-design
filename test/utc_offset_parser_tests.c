/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <stdint.h>
#include <stdbool.h>
#include "unity.h"

#include "utc_offset_parser.h"

/**-----------------------------------------------------------------------------
  @defgroup unit_test_utc_offset_parse
  @{

------------------------------------------------------------------------------*/

void test_utc_offset_scan_for_int(void)
{
  // Test Str Number to Int
  uint16_t pos = 0;

  TEST_ASSERT_EQUAL_INT16( 0, utc_offset_scan_for_int( "" ,  &pos, 2));
  TEST_ASSERT_EQUAL_INT16( 0, pos);
  pos = 0;

  TEST_ASSERT_EQUAL_INT16( 0, utc_offset_scan_for_int( ":" ,  &pos, 2));
  TEST_ASSERT_EQUAL_INT16( 0, pos);
  pos = 0;

  TEST_ASSERT_EQUAL_INT16( 0, utc_offset_scan_for_int( "0:" ,  &pos, 2));
  TEST_ASSERT_EQUAL_INT16( 1, pos);
  pos = 0;

#define X(NUM, STR) TEST_ASSERT_EQUAL_INT16( NUM, utc_offset_scan_for_int( STR ,  &pos, 2)); TEST_ASSERT_EQUAL_INT16(sizeof(STR), pos+1); pos = 0;
  X(  0,  "0"); // X
  X(  1,  "1");
  X(  2,  "2");
  X(  3,  "3");
  X(  4,  "4");
  X(  5,  "5");
  X(  6,  "6");
  X(  7,  "7");
  X(  8,  "8");
  X(  9,  "9");
  X(  0, "00"); // 0X
  X(  1, "01");
  X(  2, "02");
  X(  3, "03");
  X(  4, "04");
  X(  5, "05");
  X(  6, "06");
  X(  7, "07");
  X(  8, "08");
  X(  9, "09");
  X( 10, "10"); // 1X
  X( 11, "11");
  X( 12, "12");
  X( 13, "13");
  X( 14, "14");
  X( 15, "15");
  X( 16, "16");
  X( 17, "17");
  X( 18, "18");
  X( 19, "19");
  X( 20, "20"); // 2X
  X( 21, "21");
  X( 22, "22");
  X( 23, "23");
  X( 24, "24");
  X( 25, "25");
  X( 26, "26");
  X( 27, "27");
  X( 28, "28");
  X( 29, "29");
  X( 30, "30"); // 3X
  X( 31, "31");
  X( 32, "32");
  X( 33, "33");
  X( 34, "34");
  X( 35, "35");
  X( 36, "36");
  X( 37, "37");
  X( 38, "38");
  X( 39, "39");
  X( 40, "40"); // 4X
  X( 41, "41");
  X( 42, "42");
  X( 43, "43");
  X( 44, "44");
  X( 45, "45");
  X( 46, "46");
  X( 47, "47");
  X( 48, "48");
  X( 49, "49");
  X( 50, "50"); // 5X
  X( 51, "51");
  X( 52, "52");
  X( 53, "53");
  X( 54, "54");
  X( 55, "55");
  X( 56, "56");
  X( 57, "57");
  X( 58, "58");
  X( 59, "59");
  X( 60, "60"); // 6X
  X( 61, "61");
  X( 62, "62");
  X( 63, "63");
  X( 64, "64");
  X( 65, "65");
  X( 66, "66");
  X( 67, "67");
  X( 68, "68");
  X( 69, "69");
  X( 70, "70"); // 7X
  X( 71, "71");
  X( 72, "72");
  X( 73, "73");
  X( 74, "74");
  X( 75, "75");
  X( 76, "76");
  X( 77, "77");
  X( 78, "78");
  X( 79, "79");
  X( 80, "80"); // 8X
  X( 81, "81");
  X( 82, "82");
  X( 83, "83");
  X( 84, "84");
  X( 85, "85");
  X( 86, "86");
  X( 87, "87");
  X( 88, "88");
  X( 89, "89");
  X( 90, "90"); // 9X
  X( 91, "91");
  X( 92, "92");
  X( 93, "93");
  X( 94, "94");
  X( 95, "95");
  X( 96, "96");
  X( 97, "97");
  X( 98, "98");
  X( 99, "99");
#undef X
}

void test_utc_offset_hour_min_pos_parse(void)
{
  uint16_t hours = 0;
  uint16_t minutes = 0;
  bool positive_offset = false;

  // Test Parse (ISO 8601) Offset String to component values
  utc_offset_hour_min_pos_parse("UTC+10:30", &hours, &minutes, &positive_offset);
  TEST_ASSERT_EQUAL_UINT16(10, hours);
  TEST_ASSERT_EQUAL_UINT16(30, minutes);
  TEST_ASSERT_TRUE(positive_offset);

  // Test Parse (ISO 8601) Offset String to component values
  utc_offset_hour_min_pos_parse("UTC-12:34", &hours, &minutes, &positive_offset);
  TEST_ASSERT_EQUAL_UINT16(12, hours);
  TEST_ASSERT_EQUAL_UINT16(34, minutes);
  TEST_ASSERT_FALSE(positive_offset);
}

void test_utc_offset_parse(void)
{
  TEST_ASSERT_EQUAL_INT32( 0, utc_offset_parse(""));

  // Test Parse (ISO 8601) Offset String in seconds summation
  TEST_ASSERT_EQUAL_INT32( 36000, utc_offset_parse("UTC+10"));
  TEST_ASSERT_EQUAL_INT32( 1800,  utc_offset_parse("UTC+0:30"));
  TEST_ASSERT_EQUAL_INT32( 37800, utc_offset_parse("UTC+10:30"));

  // Test Parse (ISO 8601) Offset String in seconds summation with negative offset
  TEST_ASSERT_EQUAL_INT32( -36000, utc_offset_parse("UTC-10"));
  TEST_ASSERT_EQUAL_INT32( -1800,  utc_offset_parse("UTC-0:30"));
  TEST_ASSERT_EQUAL_INT32( -37800, utc_offset_parse("UTC-10:30"));
}



void utc_offset_parse_tests_run(void)
{
  // UTC String Offset Parser
  RUN_TEST(test_utc_offset_scan_for_int);
  RUN_TEST(test_utc_offset_hour_min_pos_parse);
  RUN_TEST(test_utc_offset_parse);
}

/// @}
