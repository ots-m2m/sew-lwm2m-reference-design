

#include "liblwm2m.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <sys/time.h>

#include "lwip/mem.h"
#include "sys_tick.h"

/**-----------------------------------------------------------------------------
 *  @file
 *  @defgroup    lwm2m_platform
 *  @ingroup     lwm2m_client
 *  @{
 *------------------------------------------------------------------------------
 */

void * lwm2m_malloc(size_t s)
{
  return malloc(s);
}

void lwm2m_free(void * p)
{
  free(p);
}

char *lwm2m_strdup(const char * str)
{
  size_t siz;
  char *copy;

  siz = strlen(str) + 1;
  if ((copy = lwm2m_malloc(siz)) == NULL)
  {
    return NULL;
  }
  memcpy(copy, str, siz);
  return copy;
}

int lwm2m_strncmp(const char * s1, const char * s2, size_t n)
{
  return strncmp(s1, s2, n);
}

// This function must return the number of seconds elapsed since origin.
// The origin (Epoch, system boot, etc...) does not matter as this
// function is used only to determine the elapsed time since the last
// call to it.
// In case of error, this must return a negative value.
// Per POSIX specifications, time_t is a signed integer.
time_t lwm2m_gettime(void)
{
  return sys_tick_get_s();
}

size_t lwm2m_platform_get_mem_total()
{
  return 0;//configTOTAL_HEAP_SIZE;
}

#define SECONDS_FROM_UNIX_EPOCH_TO_2000_EPOCH 946684800

time_t lwm2m_platform_get_unix_time()
{
  struct timeval tv;
  gettimeofday(&tv, 0);
  return tv.tv_sec;
}

int lwm2m_platform_set_unix_time(time_t ut)
{
  struct timeval tv;
  tv.tv_sec = ut;
  tv.tv_usec = 0;
  int res = settimeofday(&tv, 0);
  return res;
  return 0;
}

void lwm2m_printf(const char * format, ...)
{
  int result = 0;
  char log_buf[128];
  va_list list;
  va_start(list, format);
  result = vsnprintf(log_buf, sizeof(log_buf), format, list);
  va_end(list);
  if (result < 0)
  {
     return;
  }
  log_write(log_buf);
}
