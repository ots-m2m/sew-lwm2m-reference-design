/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#define __PARSE_LWM2M_EXE_ARG_H__
#ifdef __PARSE_LWM2M_EXE_ARG_H__

#define PARSE_LWM2M_EXE_ARG_MAX_ARGUMENT 9 // Syntax can only support up to 9 arguments

/**-----------------------------------------------------------------------------
  LWM2M executable function argument parser

  @defgroup parse_lwm2m_exe_arg
  @{
------------------------------------------------------------------------------*/

typedef struct parse_lwm2m_exe_argument_t
{
  uint8_t     key;
  size_t      value_str_length;
  const uint8_t  *value_str;
} parse_lwm2m_exe_argument_t;

/**-----------------------------------------------------------------------------
  Parse arguments string into argument list struct
------------------------------------------------------------------------------*/
bool parse_lwm2m_exe_arg_parse(
        const uint8_t *buffer_input, const uint32_t bufferlength, const uint16_t arguments_max,
        parse_lwm2m_exe_argument_t *arguments, uint16_t *arguments_count
      );

/**-----------------------------------------------------------------------------
  Check if parameter exist (But not if it has a valid value)
------------------------------------------------------------------------------*/
bool parse_lwm2m_exe_arg_arglist_index_by_arg_key(
        const parse_lwm2m_exe_argument_t *arglist, const uint16_t argcount, const uint8_t key,
        uint16_t *index
      );

/**-----------------------------------------------------------------------------
  Get uint32 value
------------------------------------------------------------------------------*/
bool parse_lwm2m_exe_arg_entry_as_uint32(
        const parse_lwm2m_exe_argument_t *arglist, const uint16_t argcount, const uint8_t key,
        uint32_t *output_value
      );

/**-----------------------------------------------------------------------------
  Get string to buffer (string termination safe)
------------------------------------------------------------------------------*/
bool parse_lwm2m_exe_arg_entry_as_string(
      const parse_lwm2m_exe_argument_t *arglist, const uint16_t argcount, const uint8_t key, const size_t string_buffer_length,
      char *string_buffer, size_t *string_length
    );

/**-----------------------------------------------------------------------------
  Zero Copy string (get pointer)

  Output:
    - output_string_ptr
    - string_buffer_length
------------------------------------------------------------------------------*/
bool parse_lwm2m_exe_arg_entry_as_string_zerocopy(
        const parse_lwm2m_exe_argument_t *arglist, const uint16_t argcount, const uint8_t key,
        char **output_string_ptr, size_t *output_string_length
      );



#ifdef UNIT_TEST
const uint8_t *parse_lwm2m_exe_arg_parse_single_argument(
    const uint8_t *p, const uint8_t *pe, const bool first_arg,
    uint32_t *key, const uint8_t **value, size_t *valuelen
  );
#endif

/// @}

#endif /* __PARSE_LWM2M_EXE_ARG_H__ */
