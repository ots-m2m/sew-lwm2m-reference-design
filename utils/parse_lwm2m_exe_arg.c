/*
 * parse_lwm2m_exe_arg.c
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 */

#include <stdint.h>
#include <stdbool.h>
#include <assert.h> // assert()
#include <string.h> // strsep()
#include <ctype.h>  // isdigit() isgraph() // TODO: Check that this covers all correct values

#include "parse_lwm2m_exe_arg.h"
#include "log.h"
#include <stdio.h> //sscanf


/* Set parse_lwm2m_exe_arg_parse_single_argument() show error code on NULL*/
//#define PARSE_LWM2M_EXE_ARG_DIAGNOSTIC_MODE
#ifdef PARSE_LWM2M_EXE_ARG_DIAGNOSTIC_MODE
#define __NULL(X) ((uint8_t *) X)
#else
#define __NULL(X) (NULL)
#endif

/**-----------------------------------------------------------------------------
  # LWM2M executable function argument parser

  ## Reference:
  * OMA-TS-LightweightM2M-V1_0-20170208-A Page 36 (138)

  ## ABNF Form:

  ```
  arglist = arg *( "," arg )
  arg = DIGIT / DIGIT "=" "'" *CHAR "'"
  DIGIT = "0" / "1" / "2" / "3" / "4" / "5" / "6" / "7" / "8" / "9"
  CHAR = "!" / %x23-26 / %x28-5B / %x5D-7E
  ```

  ## Design Note:
  * Could try to tokenize via `uint8_t *strsep(uint8_t **stringp, const uint8_t *delim)`
  * Could try to tokenize via `uint8_t *strtok(uint8_t *restrict str, const uint8_t *restrict sep)` -- Obsoleted by strsep


  @defgroup parse_lwm2m_exe_arg
  @{
------------------------------------------------------------------------------*/

/*******************************************************************************
*******************************************************************************/

__static const uint8_t *parse_lwm2m_exe_arg_parse_single_argument(
    const uint8_t *p, const uint8_t *pe, const bool first_arg,
    uint32_t *key, const uint8_t **value, size_t *valuelen
  )
{
  const uint8_t *k   = 0; // Key
  const uint8_t *v   = 0; // Value String Start
  const uint8_t *ve  = 0; // Value String End

  // Input Pointers Must Be Valid
  if (!p)   return __NULL(0);
  if (!pe)  return __NULL(1);

  /* Already at end of Buffer */
  if (p >= pe) return __NULL(2);

  /* Already at end of String */
  if (*p == '\0') return __NULL(3);


  /* Default Values */
  *key = 0;
  *value = (uint8_t *) "";
  *valuelen = 0;

  /* Tokenising Argument Number */

  /* there is a required `,` after first argument */
  if (!first_arg)
  {

    /* there is a `,` */
    if (*p != ',') return __NULL(4);

    p++;

  }

  /* there is a single digit key */
  if ((p > pe) && !isdigit(*k)) return __NULL(5);

  k = p;

  p++;

  if ((p < pe) && (*p != ',') && (*p != '\0'))
  { // Scanning for: `='<<stream of string>>'`

    /* there is a `=` */
    if (*p != '=') return __NULL(6);

    p++;

    /* there is an start value marker */
    if ((p > pe) || (*p != '\'')) return __NULL(7);

    p++;

    v = p;

    for (;p <= pe; p++)
    { /* stream of printable characters */
      if (!(isgraph(*p) && *p != '"' && *p != '\\' ))
      { /* Not Allowed Characters */
        return __NULL(8);
      }
      else if (*p == '\'')
      { /* end value marker detected */
        break;
      }
    }

    /* there is an end value marker */
    if ((p > pe) || *p != '\'') return __NULL(9);

    ve = p;

    p++;
  }

  /* Setting Output Arguments */
  *key = *k - '0';  // convert from uint8_t 0-9 to integer
  *value = v;
  *valuelen = (ve-v);

  return p;
}


/*******************************************************************************
  INTERFACE
*******************************************************************************/

bool parse_lwm2m_exe_arg_parse(
        const uint8_t *buffer_input, const uint32_t bufferlength, const uint16_t arguments_max,
        parse_lwm2m_exe_argument_t *arguments, uint16_t *arguments_count
      )
{
  uint32_t    key = 0;  // Key
  const uint8_t  *value_ptr = 0;
  size_t      value_length = 0;
  bool        first_run = true;

  assert(arguments);
  assert(arguments_count);

  *arguments_count = 0;

  for (const uint8_t *p = buffer_input ; (p = parse_lwm2m_exe_arg_parse_single_argument(p, buffer_input + bufferlength, first_run, &key, &value_ptr, &value_length)) ;)
  { // For every detected arguments
    /**--------------------------------------------
      Each argument is assume to take the form of
      `<ID: INTEGER > = "<VALUE: String >"`

      We are interested in
        * Argument Key Value
        * Pointer to start of value string
        * length of value string
    ----------------------------------------------*/

    if (*arguments_count > arguments_max)
    { // No space left for new arguments
      return false; // Not enough argument space
    }
    else
    { // Save this entry to argument list
      arguments[*arguments_count].key = key;
      arguments[*arguments_count].value_str_length = value_length;
      arguments[*arguments_count].value_str = value_ptr;
    }

    *arguments_count = *arguments_count + 1;
    first_run = false;
  }

  return true;
}

/**-----------------------------------------------------------------------------
  Find argument id and outputs

  You can use this as a way to check for presence of an entry id.
  (e.g. Mandatory Values)

  This does not check if the entry has valid value

  Output:
    * index - position in arglist with the argument

  Returns:
    * true - valid entry found in arglist
    * false - no valid entry not found in arglist
------------------------------------------------------------------------------*/
bool parse_lwm2m_exe_arg_arglist_index_by_arg_key(
        const parse_lwm2m_exe_argument_t *arglist, const uint16_t argcount, const uint8_t key,
        uint16_t *index
      )
{
  assert(arglist);
  assert(index);

  for (uint16_t i=0 ; i < argcount ; i++)
  {
    //log_debug(":%d %d %d %d", i, argcount, arglist[argcount].key, key);
    if (arglist[i].key == key)
    {
      *index = i;
      return true;
    }
  }

  return false; // No index found
}



/**-----------------------------------------------------------------------------
  Get value from argument entry by key

  Returns:
    * true - valid entry found in arglist
    * false - no valid entry not found in arglist
------------------------------------------------------------------------------*/
bool parse_lwm2m_exe_arg_entry_as_uint32(
    const parse_lwm2m_exe_argument_t *arglist, const uint16_t argcount, const uint8_t key,
    uint32_t *output_value
  )
{
  assert(arglist);
  assert(output_value);

  char buffer[11] = {0};  // 10 characters to store a 32bit int (But must also account for null string terminator)
  unsigned long int parsed_value = 0; /* Is at least 32bit according to ULONG_MAX in ISO/IEC 9899:TC3 (page 22) */
  uint16_t index = 0;
  if (!parse_lwm2m_exe_arg_arglist_index_by_arg_key(arglist, argcount, key, &index))
  { // No entry found
    return false;
  }

  /* setup buffer */
  memcpy(buffer, arglist[index].value_str, arglist[index].value_str_length);
  buffer[10] = '\0';

  /* Value String Cannot Be Empty or more than 10 characters long */
  if ((arglist[index].value_str_length == 0) || (arglist[index].value_str_length > 10))
  {
    log_err("parse '%s' to uint32_t failed (invalid length: %lu)", buffer, (long unsigned) arglist[index].value_str_length);
    return false;
  }
  /* Always have a null terminator at the 11th char position in the buffer */

  /* Parse String As Interger*/
  if( 0 == sscanf( buffer, "%10lu", &parsed_value ) )
  { // Parse fail
    log_err("parse '%s' to uint32_t failed (scanf)", buffer);
    return false;
  }

  *output_value = (uint32_t) parsed_value;

  return true;
}

/**-----------------------------------------------------------------------------
  Get character string from argument entry by key

  Returns:
    * true - valid entry found in arglist
    * false - no valid entry not found in arglist
------------------------------------------------------------------------------*/
bool parse_lwm2m_exe_arg_entry_as_string(
    const parse_lwm2m_exe_argument_t *arglist, const uint16_t argcount, const uint8_t key, const size_t string_buffer_length,
    char *string_buffer, size_t *string_length
  )
{
  assert(arglist);
  assert(string_buffer);
  assert(string_length);

  uint16_t index = 0;
  if (!parse_lwm2m_exe_arg_arglist_index_by_arg_key(arglist, argcount, key, &index))
  { // No entry found
    return false;
  }

  /* Check if value plus '\0' fits into output buffer*/
  if ((arglist[index].value_str_length + 1) > string_buffer_length)
  { // Exceeded output buffer size
    return false;
  }

  /* Copy to destination */
  memcpy(string_buffer, arglist[index].value_str, arglist[index].value_str_length);

  /* Always ensure '\0' exist in string */
  string_buffer[arglist[index].value_str_length + 1] = '\0';

  *string_length = (size_t) arglist[index].value_str_length;

  return true;
}


/**-----------------------------------------------------------------------------
  Get character string from argument entry by key

  WARN: This points directly to the arglist variables,
          This means that the string pointed to does not have '\0' terminator.
          You need to check output string length. If this is a concern, then
          use parse_lwm2m_exe_arg_entry_as_string()

  Returns:
    * true - valid entry found in arglist
    * false - no valid entry not found in arglist
------------------------------------------------------------------------------*/
bool parse_lwm2m_exe_arg_entry_as_string_zerocopy(
    const parse_lwm2m_exe_argument_t *arglist, const uint16_t argcount, const uint8_t key,
    char **output_string_ptr, size_t *output_string_length
  )
{
  assert(arglist);

  uint16_t index = 0;
  if (!parse_lwm2m_exe_arg_arglist_index_by_arg_key(arglist, argcount, key, &index))
  { // No entry found
    return false;
  }

  /* Return pointer to corresponding entries */
  *output_string_ptr = (char *) (arglist[index].value_str);

  *output_string_length = (size_t) arglist[index].value_str_length;

  return true;
}

