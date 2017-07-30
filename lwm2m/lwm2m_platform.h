/*
 * lwm2m_platform.h
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 */

#ifndef LWM2M_LWM2M_PLATFORM_H_
#define LWM2M_LWM2M_PLATFORM_H_

#include <sys/time.h>

char * lwm2m_strdup(const char * str);
void lwm2m_free(void * p);
void * lwm2m_malloc(size_t s);
time_t lwm2m_gettime(void);
const char *lwm2m_platfrom_get_client_urn();
size_t lwm2m_platform_get_mem_free();
size_t lwm2m_platform_get_mem_total();
time_t lwm2m_platform_get_unix_time();
int lwm2m_platform_set_unix_time(time_t ut);

#endif /* LWM2M_LWM2M_PLATFORM_H_ */
