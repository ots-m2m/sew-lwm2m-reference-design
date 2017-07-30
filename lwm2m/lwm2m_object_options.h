/*******************************************************************************
  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  This file is used to configure the generated objects. Refer to the
  generated object header files for avaliable configuration options.
*******************************************************************************/
#ifndef MODULES_LWM2M_OBJECTS_LWM2M_OBJECT_OPTIONS_H_
#define MODULES_LWM2M_OBJECTS_LWM2M_OBJECT_OPTIONS_H_

/* This contains user editable information */
#include "sew_config.h"

/*******************************************************************************
  GENERAL SETTINGS FOR OBJECTS
*******************************************************************************/

/**-----------------------------------------------------------------------------
  Object Base Settings
------------------------------------------------------------------------------*/
#define MAX_LWM2M_RESOURCES_PER_OBJECT 32
#define LWM2M_NAME_RESOURCE_LEN 64 + 1
#define LWM2M_MAX_INSTANCES 4
#define MAX_MULTIPLE_RESOURCES 8
#define MAX_CONFIRMATIONS 32


/**-----------------------------------------------------------------------------
  Define the maximum number of object instances
------------------------------------------------------------------------------*/

#ifndef MAX_Water_Flow_Readings_INSTANCES
#define MAX_Water_Flow_Readings_INSTANCES 4
#endif

#ifndef MAX_Water_Meter_High_Pressure_Alarm_INSTANCES
#define MAX_Water_Meter_High_Pressure_Alarm_INSTANCES 4
#endif

#ifndef MAX_Delivery_Schedule_INSTANCES
#define MAX_Delivery_Schedule_INSTANCES 4
#endif

#ifndef MAX_Interval_Data_Delivery_INSTANCES
#define MAX_Interval_Data_Delivery_INSTANCES 4
#endif

#ifndef MAX_Event_Data_Delivery_INSTANCES
#define MAX_Event_Data_Delivery_INSTANCES 4
#endif

#ifndef MAX_Test_Water_Meter_INSTANCES
#define MAX_Test_Water_Meter_INSTANCES 4
#endif

/*******************************************************************************
  Data Delivery Base
*******************************************************************************/
#define DATA_DELIVERY_BASE_MAX_DELVIERY_HANDLERS 16


/*******************************************************************************
  EVENT AND ALARM OBJECT BASE
*******************************************************************************/

#define EVENT_AND_ALARM_BASE_INSTANCES_MAX 32

#define EVENT_AND_ALARM_BASE_PAYLOAD_BYTES_MAXIMUM  1000 // Guard Against Oversized Payload Struct
#define EVENT_AND_ALARM_BASE_PAYLOAD_VALUE_COUNT_MAX 50  // TODO: Confirm size of value payload

// payload
#define EVENT_AND_ALARM_BASE_PAYLOAD_VALUE_ENTRY_MAX_BYTES   2
#define EVENT_AND_ALARM_BASE_PAYLOAD_OVERHEAD_MAX_BYTES      5
#define EVENT_AND_ALARM_BASE_PAYLOAD_BUFFER_SIZE_BYTES (EVENT_AND_ALARM_BASE_PAYLOAD_OVERHEAD_MAX_BYTES + EVENT_AND_ALARM_BASE_PAYLOAD_VALUE_COUNT_MAX * EVENT_AND_ALARM_BASE_PAYLOAD_VALUE_ENTRY_MAX_BYTES)


/*******************************************************************************
  INTERVAL OBJECT BASE
*******************************************************************************/

#define INTERVAL_DATA_BASE_INSTANCES_MAX 32

#define INTERVAL_DATA_BASE_PAYLOAD_BYTES_MAXIMUM  1000 // Guard Against Oversized Payload Struct
#define INTERVAL_DATA_BASE_PAYLOAD_VALUE_COUNT_MAX 50  // Number of interval samples to store.

// payload
#define INTERVAL_DATA_BASE_PAYLOAD_BUFFER_SIZE_BYTES 500
#define INTERVAL_DATA_BASE_PAYLOAD_OVERHEAD_MAX_BYTES 1+(2+8)+(2+8)+10
#define INTERVAL_DATA_BASE_PAYLOAD_PER_ENTRIES_MAX_BYTES 6
#define INTERVAL_DATA_BASE_PAYLOAD_MAX_ENTRIES (INTERVAL_DATA_BASE_PAYLOAD_BUFFER_SIZE_BYTES - INTERVAL_DATA_BASE_PAYLOAD_OVERHEAD_MAX_BYTES)/INTERVAL_DATA_BASE_PAYLOAD_PER_ENTRIES_MAX_BYTES


/*******************************************************************************
  OBJECT SPECIFIC CONFIGURATION
*******************************************************************************/

/**-----------------------------------------------------------------------------
  LWM2M Security
------------------------------------------------------------------------------*/
#define CONFIG_LWM2M_Security_Short_Server_ID_OPTION 1

/**-----------------------------------------------------------------------------
  LWM2M Server
------------------------------------------------------------------------------*/
#define CONFIG_LWM2M_Server_Default_Minimum_Period_OPTION 1

/**-----------------------------------------------------------------------------
  Device
------------------------------------------------------------------------------*/
#define CONFIG_Device_Manufacturer_OPTION 1
#define CONFIG_Device_Model_Number_OPTION 1
#define CONFIG_Device_Serial_Number_OPTION 1
#define CONFIG_Device_Firmware_Version_OPTION 1
#define CONFIG_Device_Factory_Reset_OPTION 1
#define CONFIG_Device_Available_Power_Sources_OPTION 1
#define CONFIG_Device_Power_Source_Voltage_OPTION 1
#define CONFIG_Device_Power_Source_Current_OPTION 1
#define CONFIG_Device_Battery_Level_OPTION 1
#define CONFIG_Device_Memory_Free_OPTION 1
#define CONFIG_Device_Reset_Error_Code_OPTION 1
#define CONFIG_Device_Current_Time_OPTION 1
#define CONFIG_Device_UTC_Offset_OPTION 1
#define CONFIG_Device_Timezone_OPTION 1

/**-----------------------------------------------------------------------------
  Delivery Schedule
------------------------------------------------------------------------------*/
#define CONFIG_Delivery_Schedule_Schedule_Start_Time_OPTION 1
#define CONFIG_Delivery_Schedule_Schedule_UTC_Offset_OPTION 1
#define CONFIG_Delivery_Schedule_Delivery_Frequency_OPTION 1
#define CONFIG_Delivery_Schedule_Randomised_Delivery_Window_OPTION 1
#define CONFIG_Delivery_Schedule_Randomised_Delivery_Window_OPTION 1
#define CONFIG_Delivery_Schedule_Retry_Period_OPTION 1
#define CONFIG_Delivery_Schedule_Number_of_Retries_OPTION 1

/* Defaults Settings */

#ifndef CONFIG_Delivery_Schedule_DEFAULT_SCHEDULE_START_TIME
#define CONFIG_Delivery_Schedule_DEFAULT_SCHEDULE_START_TIME           120
#endif
#ifndef CONFIG_Delivery_Schedule_DEFAULT_SCHEDULE_UTC_OFFSET
#define CONFIG_Delivery_Schedule_DEFAULT_SCHEDULE_UTC_OFFSET           "UTC+10"
#endif
#ifndef CONFIG_Delivery_Schedule_DEFAULT_DELIVERY_FREQENCY
#define CONFIG_Delivery_Schedule_DEFAULT_DELIVERY_FREQENCY             86400
#endif
#ifndef CONFIG_Delivery_Schedule_DEFAULT_RANDOMISED_DELIVERY_WINDOW
#define CONFIG_Delivery_Schedule_DEFAULT_RANDOMISED_DELIVERY_WINDOW    25200
#endif
#ifndef CONFIG_Delivery_Schedule_DEFAULT_NUMBER_OF_RETRIES
#define CONFIG_Delivery_Schedule_DEFAULT_NUMBER_OF_RETRIES             2
#endif
#ifndef CONFIG_Delivery_Schedule_DEFAULT_RETRY_PERIOD
#define CONFIG_Delivery_Schedule_DEFAULT_RETRY_PERIOD                  1800
#endif

/**-----------------------------------------------------------------------------
  Event Data Delivery
------------------------------------------------------------------------------*/
#define CONFIG_Event_Data_Delivery_Name_OPTION 1
#define CONFIG_Event_Data_Delivery_Event_Data_Links_OPTION 1
#define CONFIG_Event_Data_Delivery_Latest_Eventlog_OPTION 1
#define CONFIG_Event_Data_Delivery_Schedule_OPTION 1

/**-----------------------------------------------------------------------------
  Interval Data Delivery
------------------------------------------------------------------------------*/
#define CONFIG_Interval_Data_Delivery_Name_OPTION 1
#define CONFIG_Interval_Data_Delivery_Interval_Data_Links_OPTION 1
#define CONFIG_Interval_Data_Delivery_Latest_Payload_OPTION 1
#define CONFIG_Interval_Data_Delivery_Schedule_OPTION 1

/**-----------------------------------------------------------------------------
  Water Flow Readings
------------------------------------------------------------------------------*/
#define CONFIG_Water_Flow_Readings_Interval_Period_OPTION 1
#define CONFIG_Water_Flow_Readings_Interval_Start_Offset_OPTION 1
#define CONFIG_Water_Flow_Readings_Interval_UTC_Offset_OPTION 1
#define CONFIG_Water_Flow_Readings_Interval_Collection_Start_Time_OPTION 1
#define CONFIG_Water_Flow_Readings_Oldest_Recorded_Interval_OPTION 1
#define CONFIG_Water_Flow_Readings_Last_Delivered_Interval_OPTION 1
#define CONFIG_Water_Flow_Readings_Latest_Recorded_Interval_OPTION 1
#define CONFIG_Water_Flow_Readings_Interval_Delivery_Midnight_Aligned_OPTION 1
#define CONFIG_Water_Flow_Readings_Interval_Historical_Read_OPTION 1
#define CONFIG_Water_Flow_Readings_Interval_Historical_Read_Payload_OPTION 1
#define CONFIG_Water_Flow_Readings_Interval_Change_Configuration_OPTION 1
#define CONFIG_Water_Flow_Readings_Start_OPTION 1
#define CONFIG_Water_Flow_Readings_Stop_OPTION 1
#define CONFIG_Water_Flow_Readings_Status_OPTION 1
#define CONFIG_Water_Flow_Readings_Latest_Payload_OPTION 1

/* Instance 0 : Default Settings */
#ifndef CONFIG_Water_Flow_Readings_DEFAULT_INSTANCE_0_VALUE_INTERVAL_PERIOD
#define CONFIG_Water_Flow_Readings_DEFAULT_INSTANCE_0_VALUE_INTERVAL_PERIOD                    (86400)
#endif
#ifndef CONFIG_Water_Flow_Readings_DEFAULT_INSTANCE_0_VALUE_INTERVAL_START_OFFSET
#define CONFIG_Water_Flow_Readings_DEFAULT_INSTANCE_0_VALUE_INTERVAL_START_OFFSET              (0)
#endif
#ifndef CONFIG_Water_Flow_Readings_DEFAULT_INSTANCE_0_VALUE_INTERVAL_UTC_OFFSET
#define CONFIG_Water_Flow_Readings_DEFAULT_INSTANCE_0_VALUE_INTERVAL_UTC_OFFSET                "UTC+10"
#endif
#ifndef CONFIG_Water_Flow_Readings_DEFAULT_INSTANCE_0_VALUE_INTERVAL_DELIVERY_MIDNIGHT_ALIGNED
#define CONFIG_Water_Flow_Readings_DEFAULT_INSTANCE_0_VALUE_INTERVAL_DELIVERY_MIDNIGHT_ALIGNED (1)
#endif

/* Instance 1 : Default Settings */
#ifndef CONFIG_Water_Flow_Readings_DEFAULT_INSTANCE_1_VALUE_INTERVAL_PERIOD
#define CONFIG_Water_Flow_Readings_DEFAULT_INSTANCE_1_VALUE_INTERVAL_PERIOD                    (1800)
#endif
#ifndef CONFIG_Water_Flow_Readings_DEFAULT_INSTANCE_1_VALUE_INTERVAL_START_OFFSET
#define CONFIG_Water_Flow_Readings_DEFAULT_INSTANCE_1_VALUE_INTERVAL_START_OFFSET              (0)
#endif
#ifndef CONFIG_Water_Flow_Readings_DEFAULT_INSTANCE_1_VALUE_INTERVAL_UTC_OFFSET
#define CONFIG_Water_Flow_Readings_DEFAULT_INSTANCE_1_VALUE_INTERVAL_UTC_OFFSET                "UTC+10"
#endif
#ifndef CONFIG_Water_Flow_Readings_DEFAULT_INSTANCE_1_VALUE_INTERVAL_DELIVERY_MIDNIGHT_ALIGNED
#define CONFIG_Water_Flow_Readings_DEFAULT_INSTANCE_1_VALUE_INTERVAL_DELIVERY_MIDNIGHT_ALIGNED (1)
#endif


/**-----------------------------------------------------------------------------
  Water Meter High Pressure Alarm
------------------------------------------------------------------------------*/
#define CONFIG_Water_Meter_High_Pressure_Alarm_Event_Type_OPTION 1
#define CONFIG_Water_Meter_High_Pressure_Alarm_Alarm_Realtime_OPTION 1
#define CONFIG_Water_Meter_High_Pressure_Alarm_Alarm_State_OPTION 1
#define CONFIG_Water_Meter_High_Pressure_Alarm_Alarm_Set_Threshold_OPTION 1
#define CONFIG_Water_Meter_High_Pressure_Alarm_Alarm_Set_Operator_OPTION 1
#define CONFIG_Water_Meter_High_Pressure_Alarm_Alarm_Clear_Threshold_OPTION 1
#define CONFIG_Water_Meter_High_Pressure_Alarm_Alarm_Clear_Operator_OPTION 1
#define CONFIG_Water_Meter_High_Pressure_Alarm_Alarm_Maximum_Event_Count_OPTION 1
#define CONFIG_Water_Meter_High_Pressure_Alarm_Alarm_Maximum_Event_Period_OPTION 1
#define CONFIG_Water_Meter_High_Pressure_Alarm_Latest_Delivered_Event_Time_OPTION 1
#define CONFIG_Water_Meter_High_Pressure_Alarm_Latest_Recorded_Event_Time_OPTION 1
#define CONFIG_Water_Meter_High_Pressure_Alarm_Alarm_Clear__OPTION 1
#define CONFIG_Water_Meter_High_Pressure_Alarm_Alarm_Auto_Clear_OPTION 1
#define CONFIG_Water_Meter_High_Pressure_Alarm_Event_Code_OPTION 1
#define CONFIG_Water_Meter_High_Pressure_Alarm_Latest_Payload_OPTION 1

#ifndef CONFIG_Water_Meter_High_Pressure_Alarm_DEFAULT_EVENT_TYPE
#define CONFIG_Water_Meter_High_Pressure_Alarm_DEFAULT_EVENT_TYPE              EVENT_AND_ALARM_EVENT_TYPE_ALARM_STATE_CHANGE_LOG
#endif
#ifndef CONFIG_Water_Meter_High_Pressure_Alarm_DEFAULT_EVENT_CODE
#define CONFIG_Water_Meter_High_Pressure_Alarm_DEFAULT_EVENT_CODE              105
#endif
#ifndef CONFIG_Water_Meter_High_Pressure_Alarm_DEFAULT_REALTIME
#define CONFIG_Water_Meter_High_Pressure_Alarm_DEFAULT_REALTIME                false
#endif
#ifndef CONFIG_Water_Meter_High_Pressure_Alarm_DEFAULT_SET_THRESHOLD
#define CONFIG_Water_Meter_High_Pressure_Alarm_DEFAULT_SET_THRESHOLD           10
#endif
#ifndef CONFIG_Water_Meter_High_Pressure_Alarm_DEFAULT_SET_OPERATOR
#define CONFIG_Water_Meter_High_Pressure_Alarm_DEFAULT_SET_OPERATOR            EVENT_AND_ALARM_GREATER_THAN_OR_EQUAL_TO
#endif
#ifndef CONFIG_Water_Meter_High_Pressure_Alarm_DEFAULT_CLEAR_THRESHOLD
#define CONFIG_Water_Meter_High_Pressure_Alarm_DEFAULT_CLEAR_THRESHOLD         5
#endif
#ifndef CONFIG_Water_Meter_High_Pressure_Alarm_DEFAULT_CLEAR_OPERATOR
#define CONFIG_Water_Meter_High_Pressure_Alarm_DEFAULT_CLEAR_OPERATOR          EVENT_AND_ALARM_LESS_THAN_OR_EQUAL_TO
#endif
#ifndef CONFIG_Water_Meter_High_Pressure_Alarm_DEFAULT_MAXIMUM_EVENT_COUNT
#define CONFIG_Water_Meter_High_Pressure_Alarm_DEFAULT_MAXIMUM_EVENT_COUNT     2
#endif
#ifndef CONFIG_Water_Meter_High_Pressure_Alarm_DEFAULT_MAXIMUM_EVENT_PERIOD
#define CONFIG_Water_Meter_High_Pressure_Alarm_DEFAULT_MAXIMUM_EVENT_PERIOD    86400
#endif
#ifndef CONFIG_Water_Meter_High_Pressure_Alarm_DEFAULT_AUTO_CLEAR
#define CONFIG_Water_Meter_High_Pressure_Alarm_DEFAULT_AUTO_CLEAR              false
#endif



#endif /* MODULES_LWM2M_OBJECTS_LWM2M_OBJECT_OPTIONS_H_ */
