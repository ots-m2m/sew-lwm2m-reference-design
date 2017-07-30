/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <stdint.h>
#include <stdbool.h>
#include <string.h> // memset()

#include "unity.h"
#include "parse_lwm2m_exe_arg.h"

/**-----------------------------------------------------------------------------
  @defgroup unit_test_parse_lwm2m_exe_arg
  @{

------------------------------------------------------------------------------*/

void test_parse_lwm2m_exe_arg_parse_single_argument(void)
{
  uint32_t    key          = 0;  // Key
  const uint8_t  *value_ptr   = 0;
  size_t      value_length = 0;
  const uint8_t  *result      = 0;

  {
    uint8_t test_string[] = "5";
    result = parse_lwm2m_exe_arg_parse_single_argument(
                  test_string,
                  test_string + sizeof(test_string),
                  true,
                  &key,
                  &value_ptr,
                  &value_length
                );
    TEST_ASSERT(result == (test_string + sizeof(test_string) - 1));
    TEST_ASSERT_EQUAL_UINT32(5, key);
    TEST_ASSERT_EQUAL_UINT32(0, value_length);
  }

  {
    uint8_t test_string[] = "2='10.3'";
    result = parse_lwm2m_exe_arg_parse_single_argument(
                  test_string,
                  test_string + sizeof(test_string),
                  true,
                  &key,
                  &value_ptr,
                  &value_length
                );
    TEST_ASSERT(result == (test_string + sizeof(test_string) - 1));
    TEST_ASSERT_EQUAL_UINT32(2, key);
    TEST_ASSERT_EQUAL_UINT32(4, value_length);
    TEST_ASSERT_EQUAL_INT8_ARRAY("10.3", value_ptr, value_length);
  }

  for (uint8_t i = 0 ; i < 255 ; i++)
  { // Check that the accepted character in the value string is valid
    uint8_t test_string[] = "2='X'";

    test_string[3] = i;

    result = parse_lwm2m_exe_arg_parse_single_argument(
                  test_string,
                  test_string + sizeof(test_string),
                  true,
                  &key,
                  &value_ptr,
                  &value_length
                );

    if ( i == '\'')
    { /*  This is a special case where there is a triple quote.
          This will still pass this round as an empty string.
          However on next argument scan it will encounter an erronious `'` and
          exit early */
      //log_debug("%d! (%c)", i, isprint(i) ? i : ' ');
      TEST_ASSERT(result == (test_string + sizeof(test_string) - 2)); /* Because it closes early*/
      TEST_ASSERT_EQUAL_UINT32(2, key);
      TEST_ASSERT_EQUAL_UINT32(0, value_length);
    }
    else if ( (i =='!') || ((i>=0x23)&(i<=0x26)) || ((i>=0x28)&(i<=0x5B)) || ((i>=0x5D)&(i<=0x7E)) )
    { /* valid character range: "!" / %x23-26 / %x28-5B / %x5D-7E */
      //log_debug("%d! (%c)", i, isprint(i) ? i : ' ');
      TEST_ASSERT(result == (test_string + sizeof(test_string) - 1));
      TEST_ASSERT_EQUAL_UINT32(2, key);
      TEST_ASSERT_EQUAL_UINT32(1, value_length);
      TEST_ASSERT_EQUAL_UINT32(i, value_ptr[0]);
    }
    else
    {
      //log_debug("%d  (%c)", i, isprint(i) ? i : ' ');
      TEST_ASSERT(result == NULL);
    }
  }

  /** NULL Cases
  ***************/

  { /* Expected to fail because this is the second argument
        And thus expects an initial `,` character */
    uint8_t test_string[] = "5";
    result = parse_lwm2m_exe_arg_parse_single_argument(
                  test_string,
                  test_string + sizeof(test_string),
                  false,
                  &key,
                  &value_ptr,
                  &value_length
                );
    TEST_ASSERT(result == NULL);
  }

  {
    uint8_t test_string[] = "2=";
    result = parse_lwm2m_exe_arg_parse_single_argument(
                  test_string,
                  test_string + sizeof(test_string),
                  true,
                  &key,
                  &value_ptr,
                  &value_length
                );
    TEST_ASSERT(result == NULL);
  }

  {
    uint8_t test_string[] = "2=10.3'";
    result = parse_lwm2m_exe_arg_parse_single_argument(
                  test_string,
                  test_string + sizeof(test_string),
                  true,
                  &key,
                  &value_ptr,
                  &value_length
                );
    TEST_ASSERT(result == NULL);
  }

  {
    uint8_t test_string[] = "";
    result = parse_lwm2m_exe_arg_parse_single_argument(
                  test_string,
                  test_string + sizeof(test_string),
                  true,
                  &key,
                  &value_ptr,
                  &value_length
                );
    TEST_ASSERT(result == NULL);
  }

  {
    uint8_t test_string[] = "2='10.3";
    result = parse_lwm2m_exe_arg_parse_single_argument(
                  test_string,
                  test_string + sizeof(test_string),
                  true,
                  &key,
                  &value_ptr,
                  &value_length
                );
    TEST_ASSERT(result == NULL);
  }

#ifdef PARSE_LWM2M_EXE_ARG_DIAGNOSTIC_MODE
  TEST_ASSERT(false); // Ensure that diagnostic mode is disabled
#endif

}

void test_parse_lwm2m_exe_arg_parse(void)
{
  parse_lwm2m_exe_argument_t arglist[PARSE_LWM2M_EXE_ARG_MAX_ARGUMENT];

  { // Special case, empty string
    uint8_t test_string[] = "";
    uint16_t arg_count = 0;
    parse_lwm2m_exe_arg_parse(
        test_string, sizeof(test_string), PARSE_LWM2M_EXE_ARG_MAX_ARGUMENT,
        arglist, &arg_count
      );

    TEST_ASSERT_EQUAL_UINT16( 0, arg_count);
  }

  {
    uint8_t test_string[] = "5";
    uint16_t arg_count = 0;
    parse_lwm2m_exe_arg_parse(
        test_string, sizeof(test_string), PARSE_LWM2M_EXE_ARG_MAX_ARGUMENT,
        arglist, &arg_count
      );


    TEST_ASSERT_EQUAL_UINT16( 1, arg_count);

    TEST_ASSERT_EQUAL_UINT8(  5, arglist[0].key);
    TEST_ASSERT_EQUAL_UINT8(  0, arglist[0].value_str_length);
  }

  {
    uint8_t test_string[] = "5='cborlol'";
    uint16_t arg_count = 0;
    parse_lwm2m_exe_arg_parse(
        test_string, sizeof(test_string), PARSE_LWM2M_EXE_ARG_MAX_ARGUMENT,
        arglist, &arg_count
      );


    TEST_ASSERT_EQUAL_UINT16( 1, arg_count);

    TEST_ASSERT_EQUAL_UINT8(  5, arglist[0].key);
    TEST_ASSERT_EQUAL_UINT8(  7, arglist[0].value_str_length);
    TEST_ASSERT_EQUAL_INT8_ARRAY("cborlol", arglist[0].value_str, arglist[0].value_str_length);
  }

  {
    uint8_t test_string[] = "5='cborlol',6,8,7,9";
    uint16_t arg_count = 0;
    parse_lwm2m_exe_arg_parse(
        test_string, sizeof(test_string), PARSE_LWM2M_EXE_ARG_MAX_ARGUMENT,
        arglist, &arg_count
      );


    TEST_ASSERT_EQUAL_UINT16( 5, arg_count);

    TEST_ASSERT_EQUAL_UINT8(  5, arglist[0].key);
    TEST_ASSERT_EQUAL_UINT8(  7, arglist[0].value_str_length);
    TEST_ASSERT_EQUAL_INT8_ARRAY("cborlol", arglist[0].value_str, arglist[0].value_str_length);

    TEST_ASSERT_EQUAL_UINT8(  6, arglist[1].key);
    TEST_ASSERT_EQUAL_UINT8(  0, arglist[1].value_str_length);

    TEST_ASSERT_EQUAL_UINT8(  8, arglist[2].key);
    TEST_ASSERT_EQUAL_UINT8(  0, arglist[2].value_str_length);

    TEST_ASSERT_EQUAL_UINT8(  7, arglist[3].key);
    TEST_ASSERT_EQUAL_UINT8(  0, arglist[3].value_str_length);

    TEST_ASSERT_EQUAL_UINT8(  9, arglist[4].key);
    TEST_ASSERT_EQUAL_UINT8(  0, arglist[4].value_str_length);
  }

  {
    uint8_t test_string[] = "5='cborlol',6='cborlel'";
    uint16_t arg_count = 0;
    parse_lwm2m_exe_arg_parse(
        test_string, sizeof(test_string), PARSE_LWM2M_EXE_ARG_MAX_ARGUMENT,
        arglist, &arg_count
      );

    TEST_ASSERT_EQUAL_UINT16( 2, arg_count);

    TEST_ASSERT_EQUAL_UINT8(  5, arglist[0].key);
    TEST_ASSERT_EQUAL_UINT8(  7, arglist[0].value_str_length);
    TEST_ASSERT_EQUAL_INT8_ARRAY("cborlol", arglist[0].value_str, arglist[0].value_str_length);

    TEST_ASSERT_EQUAL_UINT8(  6, arglist[1].key);
    TEST_ASSERT_EQUAL_UINT8(  7, arglist[1].value_str_length);
    TEST_ASSERT_EQUAL_INT8_ARRAY("cborlel", arglist[1].value_str, arglist[1].value_str_length);
  }

}

void test_parse_lwm2m_exe_arg_use_case(void)
{
  parse_lwm2m_exe_argument_t arglist[PARSE_LWM2M_EXE_ARG_MAX_ARGUMENT];
  uint16_t index = 0;
  uint16_t argcount = 0;
  uint32_t output_value = 0;


  char test_output_string[15] = {0};
  char * test_output_string_ptr = 0;
  size_t test_output_string_length= 0;

  {
    uint8_t test_string[] = "0='1488463200',1='1488808800',7,5='UTC+10'";
    parse_lwm2m_exe_arg_parse(
        test_string, sizeof(test_string), PARSE_LWM2M_EXE_ARG_MAX_ARGUMENT,
        arglist, &argcount
      );

    /* Check Arglist is correct */
    TEST_ASSERT_EQUAL_UINT16( 4, argcount);
    TEST_ASSERT_EQUAL_UINT8(  0, arglist[0].key);
    TEST_ASSERT_EQUAL_UINT8(  10, arglist[0].value_str_length);
    TEST_ASSERT_EQUAL_INT8_ARRAY("1488463200", arglist[0].value_str, 10);
    TEST_ASSERT_EQUAL_UINT8(  1, arglist[1].key);
    TEST_ASSERT_EQUAL_UINT8(  10, arglist[1].value_str_length);
    TEST_ASSERT_EQUAL_INT8_ARRAY("1488808800", arglist[1].value_str, 10);
    TEST_ASSERT_EQUAL_UINT8(  7, arglist[2].key);
    TEST_ASSERT_EQUAL_UINT8(  0, arglist[2].value_str_length);
    TEST_ASSERT_EQUAL_UINT8(  5, arglist[3].key);
    TEST_ASSERT_EQUAL_UINT8(  6, arglist[3].value_str_length);
    TEST_ASSERT_EQUAL_INT8_ARRAY("UTC+10", arglist[3].value_str, 6);

    /* Test arglist index search */
    TEST_ASSERT_TRUE(parse_lwm2m_exe_arg_arglist_index_by_arg_key(arglist, argcount, 0, &index));
    TEST_ASSERT_EQUAL_UINT16( 0, index);
    TEST_ASSERT_TRUE(parse_lwm2m_exe_arg_arglist_index_by_arg_key(arglist, argcount, 1, &index));
    TEST_ASSERT_EQUAL_UINT16( 1, index);
    TEST_ASSERT_TRUE(parse_lwm2m_exe_arg_arglist_index_by_arg_key(arglist, argcount, 7, &index));
    TEST_ASSERT_EQUAL_UINT16( 2, index);
    TEST_ASSERT_FALSE(parse_lwm2m_exe_arg_arglist_index_by_arg_key(arglist, argcount, 8, &index));

    /* Test 32bit value extraction */
    TEST_ASSERT_TRUE(parse_lwm2m_exe_arg_entry_as_uint32(arglist, argcount, 0, &output_value));
    TEST_ASSERT_EQUAL_UINT32(1488463200, output_value);

    TEST_ASSERT_TRUE(parse_lwm2m_exe_arg_entry_as_uint32(arglist, argcount, 1, &output_value));
    TEST_ASSERT_EQUAL_UINT32(1488808800, output_value);

    /* Test zerocopy string extraction */
    TEST_ASSERT_TRUE(parse_lwm2m_exe_arg_entry_as_string_zerocopy(arglist, argcount, 5, &test_output_string_ptr, &test_output_string_length));
    TEST_ASSERT_EQUAL_UINT8( 6, test_output_string_length);
    TEST_ASSERT_EQUAL_INT8_ARRAY("UTC+10", test_output_string_ptr, 6);

    /* Test memcpy string extraction */
    TEST_ASSERT_TRUE(parse_lwm2m_exe_arg_entry_as_string(arglist, argcount, 5, sizeof(test_output_string), test_output_string, &test_output_string_length));
    TEST_ASSERT_EQUAL_UINT8( 6, test_output_string_length);
    TEST_ASSERT_EQUAL_INT8_ARRAY("UTC+10", test_output_string, 7); // Must have string termination '\0' as well
  }

  { // Simulate receiving from deilvery schedule
    uint8_t test_string[] = "0='3600',1='0',2='UTC+10'";

    memset(test_output_string, 0, sizeof(test_output_string));
    argcount = 0;
    test_output_string_ptr = 0;
    test_output_string_length = 0;

    parse_lwm2m_exe_arg_parse(
        test_string, sizeof(test_string), PARSE_LWM2M_EXE_ARG_MAX_ARGUMENT,
        arglist, &argcount
      );

    /* Test 32bit value extraction */
    TEST_ASSERT_TRUE(parse_lwm2m_exe_arg_entry_as_uint32(arglist, argcount, 0, &output_value));
    TEST_ASSERT_EQUAL_UINT32(3600, output_value);

    TEST_ASSERT_TRUE(parse_lwm2m_exe_arg_entry_as_uint32(arglist, argcount, 1, &output_value));
    TEST_ASSERT_EQUAL_UINT32(0, output_value);

    /* Test zerocopy string extraction */
    TEST_ASSERT_TRUE(parse_lwm2m_exe_arg_entry_as_string_zerocopy(arglist, argcount, 2, &test_output_string_ptr, &test_output_string_length));
    TEST_ASSERT_EQUAL_UINT8( 6, test_output_string_length);
    TEST_ASSERT_EQUAL_INT8_ARRAY("UTC+10", test_output_string_ptr, 6);

    /* Test memcpy string extraction */
    TEST_ASSERT_TRUE(parse_lwm2m_exe_arg_entry_as_string(arglist, argcount, 2, sizeof(test_output_string), test_output_string, &test_output_string_length));
    TEST_ASSERT_EQUAL_UINT8( 6, test_output_string_length);
    TEST_ASSERT_EQUAL_INT8_ARRAY("UTC+10", test_output_string, 7); // Must have string termination '\0' as well
  }
}



void parse_lwm2m_exe_arg_test_run(void)
{
  RUN_TEST(test_parse_lwm2m_exe_arg_parse_single_argument);
  RUN_TEST(test_parse_lwm2m_exe_arg_parse);
  RUN_TEST(test_parse_lwm2m_exe_arg_use_case);
}

/// @}