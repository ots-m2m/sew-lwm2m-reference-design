/*
 * lwm2m.h
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef LWM2M_LWM2M_H_
#define LWM2M_LWM2M_H_

#include <stdbool.h>
#include <stdint.h>

bool lwm2m_initialise(void);
void lwm2m_tick(void);


#endif /* LWM2M_LWM2M_H_ */
