/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef SYS_TICK_H_
#define SYS_TICK_H_

#include <stdint.h>

uint32_t sys_tick_get_s(void);
uint32_t sys_tick_get_ms(void);
uint32_t sys_tick_get_monotonic_ms(void);
void sys_tick_initialise(void);
void msleep(uint32_t ms);
void log_sys_clock(void);
void sys_clock_init(void);

#endif /* SYS_TICK_H_ */
