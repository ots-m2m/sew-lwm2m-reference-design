/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "ring_buffer_tests.h"
#include "ring_buffer.h"
#include "unity.h"

char recv_buffer[4];
rb_attr_t  rb = { .s_elem = 1, .n_elem = sizeof(recv_buffer), .buffer = recv_buffer };

void test_put_get(void)
{
  rbd_t rbd;
  TEST_ASSERT_EQUAL_INT(0, ring_buffer_init(&rbd, &rb));

  uint8_t c = 'a';
  TEST_ASSERT_EQUAL_INT(0, ring_buffer_put(rbd, &c));
  c++;
  TEST_ASSERT_EQUAL_INT(0, ring_buffer_put(rbd, &c));
  c++;
  TEST_ASSERT_EQUAL_INT(0, ring_buffer_put(rbd, &c));
  c++;
  TEST_ASSERT_EQUAL_INT(0, ring_buffer_put(rbd, &c));
  c++;
  TEST_ASSERT_EQUAL_INT(-1, ring_buffer_put(rbd, &c));
  c++;
  TEST_ASSERT_EQUAL_INT(-1, ring_buffer_put(rbd, &c));
  c++;

  c = 'a';
  uint8_t o;
  TEST_ASSERT_EQUAL_INT(0, ring_buffer_get(rbd, &o));
  TEST_ASSERT_EQUAL_INT8(c++, o);
  TEST_ASSERT_EQUAL_INT(0, ring_buffer_get(rbd, &o));
  TEST_ASSERT_EQUAL_INT8(c++, o);
  TEST_ASSERT_EQUAL_INT(0, ring_buffer_get(rbd, &o));
  TEST_ASSERT_EQUAL_INT8(c++, o);
  TEST_ASSERT_EQUAL_INT(0, ring_buffer_get(rbd, &o));
  TEST_ASSERT_EQUAL_INT8(c++, o);
  TEST_ASSERT_EQUAL_INT(-1, ring_buffer_get(rbd, &o));

  c = '0';
  TEST_ASSERT_EQUAL_INT(0, ring_buffer_put(rbd, &c));
  TEST_ASSERT_EQUAL_INT(0, ring_buffer_get(rbd, &o));
  TEST_ASSERT_EQUAL_INT8(c, o);
  c++;
  TEST_ASSERT_EQUAL_INT(0, ring_buffer_put(rbd, &c));
  TEST_ASSERT_EQUAL_INT(0, ring_buffer_get(rbd, &o));
  TEST_ASSERT_EQUAL_INT8(c, o);

  c++;
  TEST_ASSERT_EQUAL_INT(0, ring_buffer_put(rbd, &c));
  TEST_ASSERT_EQUAL_INT(0, ring_buffer_get(rbd, &o));
  TEST_ASSERT_EQUAL_INT8(c, o);

}

void test_get(void)
{

}
void ring_buffer_tests_run(void)
{
  RUN_TEST(test_put_get);
}
