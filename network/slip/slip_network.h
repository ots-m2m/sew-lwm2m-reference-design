/*
 * slip_network.h
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 */


#ifndef MODULES_NETWORK_SLIP_NETWORK_H_
#define MODULES_NETWORK_SLIP_NETWORK_H_

#include <stdbool.h>

void slip_network_initialise(void);
void slip_network_tick(void);
void slip_network_start(void);
void slip_network_stop(void);
void slip_network_setdefault(void);
bool slip_get_network_running(void);

#endif /* MODULES_NETWORK_SLIP_NETWORK_H_ */
