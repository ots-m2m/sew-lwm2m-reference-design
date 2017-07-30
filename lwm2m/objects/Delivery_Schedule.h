/*
 * Delivery_Schedule.h
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 */

#ifndef LWM2M_Delivery_Schedule_OBJECT_H_
#define LWM2M_Delivery_Schedule_OBJECT_H_

#include <stdint.h>
#include "Delivery_Schedule_base.h"

// This defines the structure of an object's instance internal variable
typedef struct Delivery_Schedule_instance_variables_t
{
  int16_t instance_id;
  bool initialised_schedule;

  // Helps to convert UTC to local time
  int32_t   UTC_offset_seconds;     // Calculated from UTC Offset String: Delivery_Schedule_Schedule_UTC_Offset_get()

  // Flags
  bool transmission_active;         // Flag to check if actively trying to transmit (Else is just waiting)
  bool transmission_confirmed;      // Flag to check if retransmission is needed

  // This is in reference to local unix time
  uint32_t next_transmit_window_s;  // This tracks the change in window period
  uint32_t random_delay_s;          // This is used for lost packets

  // Retransmission
  uint32_t next_retransmit_time_s;  // This is when to retransmit
  uint16_t retry_count;             // Counts number of transmit attempt

} Delivery_Schedule_instance_variables_t;


/******************************************************************************/

//FIXME: this should be declared by the code generator in the
//_base.h file.
uint16_t Delivery_Schedule_get_index_for_instance(uint16_t instance);

void Delivery_Schedule_confirm(uint16_t instance_id);
void Delivery_Schedule_tick(uint32_t unix_epoch_time);
void Delivery_Schedule_object_initialise(void);
void Delivery_Schedule_time_changed(void);
void Delivery_Schedule_register(lwm2m_context_t *context);

#ifdef UNIT_TEST
uint32_t Delivery_Schedule_Diagnostics(uint16_t instance, uint32_t unix_epoch_time, uint32_t *out_next_tx_s, uint32_t *out_next_win_s, uint32_t *out_next_retry_s, uint32_t *retry_count, bool *transmission_active, bool *transmission_confirmed);
void Delivery_Schedule_object_reset_instance(uint16_t instance_id);

// Tick
void Delivery_Schedule_instance_tick(Delivery_Schedule_instance_variables_t *var_ptr, uint16_t  instance, uint32_t unix_epoch_time);

// UTC Int Parse
uint16_t Delivery_Schedule_utc_offset_scan_for_int(const char *utc_offset_str, uint16_t *pos, int max);
bool Delivery_Schedule_utc_offset_hour_min_pos_parse(const char *utc_offset_str, uint16_t *hours, uint16_t *minutes, bool *positive_offset);
bool Delivery_Schedule_utc_offset_parse(const char *utc_offset_str, int32_t *out_seconds);

// etc...
void Delivery_Schedule_object_initialise_instance(uint16_t instance_id);
uint32_t generate_random_window_delay_s(uint32_t delay_window_s);
#endif


#endif
