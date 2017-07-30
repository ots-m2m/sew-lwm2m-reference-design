/*
 * lwm2m_objects.c
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "lwm2m_objects.h"
#include "LWM2M_Security.h"
#include "LWM2M_Server.h"
#include "Device.h"

#include "Water_Meter_High_Pressure_Alarm.h"
#include "Water_Flow_Readings.h"
#include "Delivery_Schedule.h"
#include "Event_Data_delivery.h"
#include "Test_Water_Meter.h"
#include "Interval_Data_Delivery.h"

#include "lwm2m_object_options.h"

#include <string.h>

/**-----------------------------------------------------------------------------
 *  @file
 *  @defgroup    lwm2m_objects
 *  @ingroup     lwm2m_client
 *  @{
 *------------------------------------------------------------------------------
 */

void lwm2m_objects_initialise(void)
{
  LWM2M_Security_initialise();
  LWM2M_Security_create_instance(0);

#ifdef LWM2M_OBJECTS_BOOTSTRAP
  LWM2M_Security_LWM2M_Server_URI_set(0, BOOTSTRAP_SERVER_URI, strlen(BOOTSTRAP_SERVER_URI), false);
  LWM2M_Security_Bootstrap_Server_set(0, true, false);
#else
  LWM2M_Security_LWM2M_Server_URI_set(0, SERVER_URI, strlen(SERVER_URI), false);
  LWM2M_Security_Bootstrap_Server_set(0, false, false);
#endif

  LWM2M_Security_Security_Mode_set(0, LWM2M_SECURITY_MODE_NONE, false);
  LWM2M_Security_Short_Server_ID_set(0, 1234, false);
  LWM2M_Security_Client_Hold_Off_Time_set(0, 1, false);

  LWM2M_Server_initialise();
  LWM2M_Server_create_instance(0);
  LWM2M_Server_Short_Server_ID_set(0, 1234, false);
  LWM2M_Server_Lifetime_set(0, 30000, false);
  LWM2M_Server_Binding_set(0, "U", 1, false);
  LWM2M_Server_Notification_Storing_When_Disabled_or_Offline_set(0, false, false);

  Device_initialise();
  Device_create_instance(0);
  Device_Manufacturer_set(0, MANUFACTURER, strlen(MANUFACTURER),false);
  Device_Model_Number_set(0, MODEL, strlen(MODEL), false);
  Device_Serial_Number_set(0, SERIAL, strlen(SERIAL), false);
  Device_Firmware_Version_set(0, FIRMWARE, strlen(FIRMWARE), false);

  Water_Meter_High_Pressure_Alarm_object_initialise();

  Delivery_Schedule_initialise();
  Delivery_Schedule_object_initialise();

  Event_Data_Delivery_object_initialise();
  Interval_Data_Delivery_object_initialise();

  Water_Flow_Readings_object_initialise();

#ifdef INTEGRATION_TEST
  Test_Water_Meter_object_initialise();
#endif
}

uint16_t lwm2m_objects_register(lwm2m_context_t *context, lwm2m_object_t *objects[], uint16_t max_objects)
{
  uint16_t num_objects = 0;
  objects[num_objects++] = LWM2M_Security_get_lwm2m_object();
  if (num_objects >= max_objects) return max_objects;
  LWM2M_Security_register_object(context);
  LWM2M_Security_register_instance(0);

  objects[num_objects++] = LWM2M_Server_get_lwm2m_object();
  if (num_objects >= max_objects) return max_objects;
  LWM2M_Server_register_object(context);
  LWM2M_Server_register_instance(0);

  objects[num_objects++] = Device_get_lwm2m_object();
  if (num_objects >= max_objects) return max_objects;
  Device_register_object(context);

  objects[num_objects++] = Water_Meter_High_Pressure_Alarm_get_lwm2m_object();
  if (num_objects >= max_objects) return max_objects;
  Water_Meter_High_Pressure_Alarm_register(context);

  objects[num_objects++] = Water_Flow_Readings_get_lwm2m_object();
  if (num_objects >= max_objects) return max_objects;
  Water_Flow_Readings_register(context);

  objects[num_objects++] = Delivery_Schedule_get_lwm2m_object();
  if (num_objects >= max_objects) return max_objects;
  Delivery_Schedule_register(context);

  objects[num_objects++] = Event_Data_Delivery_get_lwm2m_object();
  if (num_objects >= max_objects) return max_objects;
  Event_Data_Delivery_register(context);

  objects[num_objects++] = Interval_Data_Delivery_get_lwm2m_object();
  if (num_objects >= max_objects) return max_objects;
  Interval_Data_Delivery_register(context);

#ifdef INTEGRATION_TEST
  objects[num_objects++] = Test_Water_Meter_get_lwm2m_object();
  if (num_objects >= max_objects) return max_objects;
  Test_Water_Meter_register_object(context);
  Test_Water_Meter_register_instance(0);
  Test_Water_Meter_register_instance(1);
  Test_Water_Meter_register_instance(2);
  Test_Water_Meter_register_instance(3);
#endif

  return num_objects;
}
