/*
 * mutex.h
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * This is a dummy mutex implementation to allow code that relies in mutexs to
 * be built. Note that the code becomes non re-entrant using this mutex
 * implementation.
 *
 */
#ifndef UTILS_MUTEX_H_
#define UTILS_MUTEX_H_

typedef struct Mutex { } Mutex;

#define MUTEXT_WAIT_FOREVER 0

#define mutex_initialise(m)
#define  mutex_lock(m, w)
#define  mutex_unlock(m)
#define  mutex_uninitialise(m)

#endif /* UTILS_MUTEX_H_ */
