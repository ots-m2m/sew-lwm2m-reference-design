/*******************************************************************************
  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  User Editable configuration

*******************************************************************************/
#ifndef __SEW_CONFIG_H__
#define __SEW_CONFIG_H__

/* General Information About This Device */
#define MANUFACTURER "Operational Technology Solutions M2M Pty. Ltd."
#define MODEL "SEW Water Meter Reference Design"
#define SERIAL "1234567890"
#define FIRMWARE "0.0.0.24243"

/* This enables the ability for this device to use a bootstrap server */
//#define LWM2M_OBJECTS_BOOTSTRAP

/* This is used for internally testing via local broker connected in lan*/
#define INTERNAL_LAN_TEST_BROKER 1 // TODO: Remove when ready

#ifdef INTERNAL_LAN_TEST_BROKER // 5684=secureport
#define SERVER_URI "coap://192.168.0.17:5683"
#define BOOTSTRAP_SERVER_URI "coap://192.168.1.17:5673"// "coap://52.64.149.206:5673" //
#else
#define SERVER_URI "coap://core.gridthings.io:5683"
#define BOOTSTRAP_SERVER_URI "coap://boostrap.gridthings.io:5673"// "coap://52.64.149.206:5673" //
#endif


/**-----------------------------------------------------------------------------
  Enable Debug (Comment Out To Disable)
------------------------------------------------------------------------------*/
//#define DEBUG_INTERVAL_DATA_BASE
//#define DEBUG_EVENT_AND_ALARM_BASE
//#define DEBUG_DELIVERY_SCHEDULE
//#define DEBUG_WATER_FLOW_READINGS
//#define DEBUG_WATER_METER_HIGH_PRESSURE_ALARM
//#define DEBUG_EVENT_DATA_DELIVERY
//#define DEBUG_DATA_DELIVERY_BASE

/**-----------------------------------------------------------------------------
  Define the maximum number of object instances
------------------------------------------------------------------------------*/
#define MAX_Water_Flow_Readings_INSTANCES 4
#define MAX_Water_Meter_High_Pressure_Alarm_INSTANCES 4
#define MAX_Delivery_Schedule_INSTANCES 4
#define MAX_Interval_Data_Delivery_INSTANCES 4
#define MAX_Event_Data_Delivery_INSTANCES 4
#define MAX_Test_Water_Meter_INSTANCES 4

/**-----------------------------------------------------------------------------
  Delivery Schedule
------------------------------------------------------------------------------*/

/* Defaults Settings */

#define CONFIG_Delivery_Schedule_DEFAULT_SCHEDULE_START_TIME           120
#define CONFIG_Delivery_Schedule_DEFAULT_SCHEDULE_UTC_OFFSET           "UTC+10"
#define CONFIG_Delivery_Schedule_DEFAULT_DELIVERY_FREQENCY             86400
#define CONFIG_Delivery_Schedule_DEFAULT_RANDOMISED_DELIVERY_WINDOW    25200
#define CONFIG_Delivery_Schedule_DEFAULT_NUMBER_OF_RETRIES             2
#define CONFIG_Delivery_Schedule_DEFAULT_RETRY_PERIOD                  1800

/**-----------------------------------------------------------------------------
  Water Flow Readings
------------------------------------------------------------------------------*/

/* Instance 0 : Default Settings */
#define CONFIG_Water_Flow_Readings_DEFAULT_INSTANCE_0_VALUE_INTERVAL_PERIOD                    (86400)
#define CONFIG_Water_Flow_Readings_DEFAULT_INSTANCE_0_VALUE_INTERVAL_START_OFFSET              (0)
#define CONFIG_Water_Flow_Readings_DEFAULT_INSTANCE_0_VALUE_INTERVAL_UTC_OFFSET                "UTC+10"
#define CONFIG_Water_Flow_Readings_DEFAULT_INSTANCE_0_VALUE_INTERVAL_DELIVERY_MIDNIGHT_ALIGNED (1)

/* Instance 1 : Default Settings */
#define CONFIG_Water_Flow_Readings_DEFAULT_INSTANCE_1_VALUE_INTERVAL_PERIOD                    (1800)
#define CONFIG_Water_Flow_Readings_DEFAULT_INSTANCE_1_VALUE_INTERVAL_START_OFFSET              (0)
#define CONFIG_Water_Flow_Readings_DEFAULT_INSTANCE_1_VALUE_INTERVAL_UTC_OFFSET                "UTC+10"
#define CONFIG_Water_Flow_Readings_DEFAULT_INSTANCE_1_VALUE_INTERVAL_DELIVERY_MIDNIGHT_ALIGNED (1)


/**-----------------------------------------------------------------------------
  Water Meter High Pressure Alarm
------------------------------------------------------------------------------*/
#define CONFIG_Water_Meter_High_Pressure_Alarm_DEFAULT_EVENT_TYPE              EVENT_AND_ALARM_EVENT_TYPE_ALARM_STATE_CHANGE_LOG
#define CONFIG_Water_Meter_High_Pressure_Alarm_DEFAULT_EVENT_CODE              105
#define CONFIG_Water_Meter_High_Pressure_Alarm_DEFAULT_REALTIME                false
#define CONFIG_Water_Meter_High_Pressure_Alarm_DEFAULT_SET_THRESHOLD           10
#define CONFIG_Water_Meter_High_Pressure_Alarm_DEFAULT_SET_OPERATOR            EVENT_AND_ALARM_GREATER_THAN_OR_EQUAL_TO
#define CONFIG_Water_Meter_High_Pressure_Alarm_DEFAULT_CLEAR_THRESHOLD         5
#define CONFIG_Water_Meter_High_Pressure_Alarm_DEFAULT_CLEAR_OPERATOR          EVENT_AND_ALARM_LESS_THAN_OR_EQUAL_TO
#define CONFIG_Water_Meter_High_Pressure_Alarm_DEFAULT_MAXIMUM_EVENT_COUNT     2
#define CONFIG_Water_Meter_High_Pressure_Alarm_DEFAULT_MAXIMUM_EVENT_PERIOD    86400
#define CONFIG_Water_Meter_High_Pressure_Alarm_DEFAULT_AUTO_CLEAR              false

#endif /* __SEW_CONFIG_H__ */