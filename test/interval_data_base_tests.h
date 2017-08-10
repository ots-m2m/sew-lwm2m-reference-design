/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */


#ifndef TEST_INTERVAL_DATA_BASE_TESTS_H_
#define TEST_INTERVAL_DATA_BASE_TESTS_H_

#define UNIT_TEST_PAYLOAD_DATA_DUMP
#define DISABLE_INTERVAL_DATA_BASE_TEST_ASSERTS

#define DEFAULT_INSTANCE_0_VALUE_INTERVAL_PERIOD                    (86400)
#define DEFAULT_INSTANCE_0_VALUE_INTERVAL_START_OFFSET              (0)
#define DEFAULT_INSTANCE_0_VALUE_INTERVAL_UTC_OFFSET                "UTC+10"
#define DEFAULT_INSTANCE_0_VALUE_INTERVAL_DELIVERY_MIDNIGHT_ALIGNED (1)

#define DEFAULT_INSTANCE_1_VALUE_INTERVAL_PERIOD                    (1800)
#define DEFAULT_INSTANCE_1_VALUE_INTERVAL_START_OFFSET              (0)
#define DEFAULT_INSTANCE_1_VALUE_INTERVAL_UTC_OFFSET                "UTC+10"
#define DEFAULT_INSTANCE_1_VALUE_INTERVAL_DELIVERY_MIDNIGHT_ALIGNED (1)


void interval_data_tests_run(void);




#endif /* TEST_INTERVAL_DATA_BASE_TESTS_H_ */
