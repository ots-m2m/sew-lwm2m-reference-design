/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
#ifndef __log_h__
#define __log_h__

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

#ifdef UNIT_TEST
#define __static 
#else
#define __static static
#endif

//TODO: hack because stdio doesn't compile here, need to investigate
int  snprintf(char * __restrict, size_t, const char * __restrict, ...);

#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)

#ifdef NLOG
  #define log_warn(M, ...)
  #define log_debug(M, ...)
  #define log_raw(M, ...)
  #define log_err(M, ...)
  #define log_info(M, ...)
#elif PLOG
 #define log_warn(M, ...)
 #define log_debug(M, ...)

 #define log_raw(M, ...) \
   log_write(ES(M), ##__VA_ARGS__)

 #define log_err(M, ...) \
   log_write(ES("[E] " M), ##__VA_ARGS__)

#define log_info(M, ...) \
   log_write(ES("[I] " M), ##__VA_ARGS__)

#else

#define log_raw(M, ...) \
  log_write( M, ##__VA_ARGS__)

#define log_info(M, ...) \
  log_write_entry(false, "[I] ", M, ##__VA_ARGS__) \

#define log_err(M, ...) \
  log_write_entry(false, "[E] ", M, ##__VA_ARGS__)

#define log_info_ccs(M, ...) \
  log_write_entry(true, "[I] ", M, ##__VA_ARGS__) \

#define log_err_ccs(M, ...) \
  log_write_entry(true, "[E] ", M, ##__VA_ARGS__)

#ifdef NDEBUG
#define log_debug(M, ...) \

#else
#define log_debug(M, ...) \
  log_write_entry(false, "[D] ", M, ##__VA_ARGS__)

//write to the console and to the Code Composer debugger console
#define log_debug_ccs(M, ...) \
  log_write_entry(true, "[W] ", M, ##__VA_ARGS__)

#endif

#define log_warn(M, ...) \
  log_write_entry(false, "[W] ", M, ##__VA_ARGS__)

#endif

#define LOG_IP(ip) (uint32_t)(ip >> 24) & 0xFF,  (uint32_t)(ip >> 16) & 0xFF,  (uint32_t)(ip >>  8) & 0xFF,  (uint32_t)(ip) & 0xFF

void log_write(const char *pcFormat, ...);
void log_write_entry(bool mirror_to_debugger, const char *tag, const char *pcFormat, ...);
int log_write_console_char(const char c);
void log_initialise();
int log_write_console(const char *str);
int log_getchar(char *c, uint32_t timout_ms);

#endif
