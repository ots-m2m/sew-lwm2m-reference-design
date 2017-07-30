 /*
  * This Source Code Form is subject to the terms of the Mozilla Public
  * License, v. 2.0. If a copy of the MPL was not distributed with this
  * file, You can obtain one at http://mozilla.org/MPL/2.0/.
  */
#include <stdint.h>
#include <stdbool.h>

#include "debug_utilities.h"
#include "log.h"

void debug_utilities_hexdump(const void *bytearray_input, const uint32_t bytearray_size, uint16_t offset, const char *str)
{
  const uint8_t *bytearray_ptr = bytearray_input;
  log_raw("Hex: {");
  while (offset < bytearray_size)
  {
    log_raw("%s0x%02x%s", (offset%9) == 0 ? "\n        ":"", bytearray_ptr[offset], (offset + 1) < bytearray_size ? ", ":"");
    offset ++;
  }
  log_raw("\n.     }; // --- %s\r\n", str ? str : "");
}
