 /*
  * This Source Code Form is subject to the terms of the Mozilla Public
  * License, v. 2.0. If a copy of the MPL was not distributed with this
  * file, You can obtain one at http://mozilla.org/MPL/2.0/.
  */

#define __DEBUG_UTILITIES_H__
#ifdef  __DEBUG_UTILITIES_H__

void debug_utilities_hexdump(const void *bytearray_input, const uint32_t bytearray_size, uint16_t offset, const char *str);

#endif /* __DEBUG_UTILITIES_H__ */