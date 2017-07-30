/*
 * utc_offset_parser.h
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 */

/**-----------------------------------------------------------------------------
  UTC Offset String Parsing

  This is interpreted to be based on https://en.wikipedia.org/wiki/ISO_8601
  Specification used default value of "UTC+10"

  @defgroup utc_offset_parse
  @{
------------------------------------------------------------------------------*/


int32_t utc_offset_parse(const char *utc_offset_str);

#ifdef UNIT_TEST
int utc_offset_parse_char(const char c);
uint16_t utc_offset_scan_for_int(const char *utc_offset_str, uint16_t *pos, const int max);
bool utc_offset_hour_min_pos_parse(const char *utc_offset_str, uint16_t *hours, uint16_t *minutes, bool *positive_offset);
#endif

/// @}