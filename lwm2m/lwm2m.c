/*
 * lwm2m.c
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "lwm2m.h"
#include "lwm2m_objects.h"
#include "lwm2m_connection.h"
#include "liblwm2m.h"
#include "sys_tick.h"
#include "log.h"

#include "Device.h" // TODO: Find a way to move Device_register_instance() to lwm2m_objects_register()

// Sevices
#include "Delivery_Schedule.h"
#include "event_and_alarm_base.h"
#include "interval_data_base.h"
#include "Water_Meter_High_Pressure_Alarm_base.h" // TODO: Find a way to move Water_Meter_High_Pressure_Alarm_register_instance() to lwm2m_objects_register()

#include <sys/time.h>

/**-----------------------------------------------------------------------------
 *  @file
 *  @defgroup    lwm2m
 *  @ingroup     lwm2m_client
 *  @{
 *------------------------------------------------------------------------------
 */

#define MAX_OBJECTS 32
#define STATE_MACHINE_PRINT_PERIOD_ms 30000

static lwm2m_context_t *lwm2m_handle = 0;
static lwm2m_object_t *lwm2m_objects[MAX_OBJECTS];

void lwm2m_print_state(lwm2m_context_t * lwm2mH);

bool lwm2m_initialise(void)
{
  lwm2m_objects_initialise();

  lwm2m_handle = lwm2m_init(0);
  if (lwm2m_handle == 0)
  {
    log_err("Failed to initialise LWM2M");
    return false;
  }

  uint16_t num_objects = lwm2m_objects_register(lwm2m_handle, lwm2m_objects, MAX_OBJECTS);

  uint16_t result = lwm2m_configure(
    lwm2m_handle,
    "urn:slip:1234567890",
    0,
    0,
    num_objects,
    lwm2m_objects
    );

  if (result != 0)
  {
    log_err("Failed to configure LWM2M client: 0x%X", result);
    return false;
  }

  return true;
}

void lwm2m_tick(void)
{
  int before_state = 0;
  time_t time_s = 0;

  /** Network
  *************/

  // Handle incoming messages
  lwm2m_connection_poll(lwm2m_handle);

  /** State Machine Update
  *************************/

  // Previous State
  before_state = lwm2m_handle->state;

  // Increment State Machine
  time_s = sys_tick_get_s();
  lwm2m_step(lwm2m_handle, &time_s);

  // Check State Machine State
  if (lwm2m_handle->state !=  before_state)
  { // State Change Detected
    lwm2m_print_state(lwm2m_handle);
    if (lwm2m_handle->state  == STATE_READY)
    {
      //FIXME: why do we need to wait until after the device is registered
      //before registering this instance
      Device_register_instance(0);
      Water_Meter_High_Pressure_Alarm_register_instance(0);
    }
  }
  else
  { // No Change To State
    if ((sys_tick_get_monotonic_ms()) % STATE_MACHINE_PRINT_PERIOD_ms == 0)
    { // Periodic Printing of state machine state
      lwm2m_print_state(lwm2m_handle);
    }
  }


  /** Object Update
  ******************/

  // Get UTC time
  struct timeval tv = {0};
  gettimeofday(&tv, 0);

  // Base Superclass Services
  event_and_alarm_base_tick(tv.tv_sec);
  interval_data_base_tick(tv.tv_sec);

  // Singular Object Services
  Delivery_Schedule_tick(tv.tv_sec);
}


void lwm2m_print_state(lwm2m_context_t * lwm2mH)
{
  lwm2m_server_t * targetP;

  log_info("--------------------------");
  log_write("State: ");
  switch(lwm2mH->state)
  {
  case STATE_INITIAL:
    log_write("STATE_INITIAL");
    break;
  case STATE_BOOTSTRAP_REQUIRED:
    log_write("STATE_BOOTSTRAP_REQUIRED");
    break;
  case STATE_BOOTSTRAPPING:
    log_write("STATE_BOOTSTRAPPING");
    break;
  case STATE_REGISTER_REQUIRED:
    log_write("STATE_REGISTER_REQUIRED");
    break;
  case STATE_REGISTERING:
    log_write("STATE_REGISTERING");
    break;
  case STATE_READY:
    log_write("STATE_READY");
    break;
  default:
    log_write("Unknown !");
    break;
  }
  log_write("\r\n");


  log_write("Bootstrap Servers:");
  log_write("\r\n");

  targetP = lwm2mH->bootstrapServerList;

  if (lwm2mH->bootstrapServerList == NULL)
  {
    log_write("  none");
  }
  else
  {
    for (targetP = lwm2mH->bootstrapServerList ; targetP != NULL ; targetP = targetP->next)
    {
      log_write( "  Security Object ID %d", targetP->secObjInstID);
      log_write("\tHold Off Time: %lu s", (unsigned long)targetP->lifetime);
      log_write("\tstatus: ");
      switch(targetP->status)
      {
      case STATE_DEREGISTERED:
        log_write("DEREGISTERED");
          break;
      case STATE_BS_HOLD_OFF:
        log_write("CLIENT HOLD OFF");
          break;
      case STATE_BS_INITIATED:
        log_write("BOOTSTRAP INITIATED");
          break;
      case STATE_BS_PENDING:
        log_write("BOOTSTRAP PENDING");
          break;
      case STATE_BS_FINISHED:
        log_write("BOOTSTRAP FINISHED");
          break;
      case STATE_BS_FAILED:
        log_write("BOOTSTRAP FAILED");
          break;
      default:
        log_write("INVALID (%d)", (int)targetP->status);
      }
    }
  }
  log_write("\r\n");

  log_write("LWM2M Servers:");
  log_write("\r\n");

  if (lwm2mH->serverList == NULL)
  {
    log_write("  none");
  }
  else
  {
    for (targetP = lwm2mH->serverList ; targetP != NULL ; targetP = targetP->next)
    {
      log_write("  Server ID %d", targetP->shortID);
      log_write("\tstatus: ");
      switch(targetP->status)
      {
      case STATE_DEREGISTERED:
        log_write("DEREGISTERED");
          break;
      case STATE_REG_PENDING:
        log_write("REGISTRATION PENDING");
          break;
      case STATE_REGISTERED:
        log_write("REGISTERED\tlocation: \"%s\"\tLifetime: %lus", targetP->location, (unsigned long)targetP->lifetime);
          break;
      case STATE_REG_UPDATE_PENDING:
        log_write("REGISTRATION UPDATE PENDING");
          break;
      case STATE_DEREG_PENDING:
        log_write("DEREGISTRATION PENDING");
          break;
      case STATE_REG_FAILED:
        log_write("REGISTRATION FAILED");
          break;
      default:
        log_write("INVALID (%d)", (int)targetP->status);
      }
      log_write("\r\n");

    }
    log_write("\r\n");

  }
  log_write("\r\n");
}

