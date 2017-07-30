/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <stdint.h>
#include <stdbool.h>
#include <assert.h> // assert()
#include "log.h" // __static

/**-----------------------------------------------------------------------------
  UTC Offset String Parsing

  This is interpreted to be based on https://en.wikipedia.org/wiki/ISO_8601
  Specification used default value of "UTC+10"

  @defgroup utc_offset_parse
  @{
------------------------------------------------------------------------------*/

__static int utc_offset_parse_char(const char c)
{
  switch (c)
  {
    case '0': return  0;
    case '1': return  1;
    case '2': return  2;
    case '3': return  3;
    case '4': return  4;
    case '5': return  5;
    case '6': return  6;
    case '7': return  7;
    case '8': return  8;
    case '9': return  9;
  }
  return -1;  // No match
}



__static uint16_t utc_offset_scan_for_int(const char *utc_offset_str, uint16_t *pos, const int max)
{
  uint16_t  output = 0;
  for (int i = 0 ; i < max ; i++)
  {
    int var = utc_offset_parse_char(utc_offset_str[*pos]);

    // Check for error code
    if (var == -1)
    { // Not a number. Skip
      break;
    }

    // Running Calculation
    output += var;
    output *= 10;  // Base 10 shift to the left
    *pos = *pos + 1;
  }
  output = output / 10; // Base 10 shift to the left
  return output;
}



__static bool utc_offset_hour_min_pos_parse(const char *utc_offset_str, uint16_t *hours, uint16_t *minutes, bool *positive_offset)
{
  // Reference: https://en.wikipedia.org/wiki/List_of_UTC_time_offsets

  // Scan Position
  uint16_t pos = 0;

  // Ignore "UTC" header
  if (utc_offset_str[pos] == 'U') pos++;
  if (utc_offset_str[pos] == 'T') pos++;
  if (utc_offset_str[pos] == 'C') pos++;

  // Pos or neg offset
  if (utc_offset_str[pos] == '+')
  {
    *positive_offset = true;
    pos++;
  }
  else if (utc_offset_str[pos] == '-')
  {
    *positive_offset = false;
    pos++;
  }
  else
  { // Omitted signs defaults to positive
    *positive_offset = true;
  }

  // Hour Number
  *hours = utc_offset_scan_for_int(utc_offset_str, &pos, 2);

  // If next is ':' then scan for minutes
  if (utc_offset_str[pos] == ':')
  {
    pos++;  // Skip over ':'
    *minutes = utc_offset_scan_for_int(utc_offset_str, &pos, 2);
  }

  return true;
}



/*******************************************************************************
  INTERFACE
*******************************************************************************/
int32_t utc_offset_parse(const char *utc_offset_str)
{
  // Reference: https://en.wikipedia.org/wiki/List_of_UTC_time_offsets

  // Values to extract
  uint16_t hours = 0;
  uint16_t minutes = 0;
  bool positive_offset = false;

  // absolute seconds offset
  int32_t absolute_seconds_offset = 0;

  // Parse
  utc_offset_hour_min_pos_parse(utc_offset_str, &hours, &minutes, &positive_offset);

  // Convert hours and minutes to absolute seconds
  absolute_seconds_offset = (hours*60*60) + (minutes*60);

  // Output with signed offset value
  if (positive_offset)
  {
    return +absolute_seconds_offset;
  }
  else
  {
    return -absolute_seconds_offset;
  }

  return 0;
}


/// @}
