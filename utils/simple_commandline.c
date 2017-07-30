/*==============================================================================

 This Source Code Form is subject to the terms of the Mozilla Public
 License, v. 2.0. If a copy of the MPL was not distributed with this
 file, You can obtain one at http://mozilla.org/MPL/2.0/.

==============================================================================*/
/**-----------------------------------------------------------------------------

  This provides a very simple command line interface. Key feature
    * Uses Argc and Argv format for command function calls
    * Supports backspace deletion of character in command buffer
    * Tab will retrieve last command processed

  To add new commands refer to `int command_dispatch(int argc, char *argv[])`.

------------------------------------------------------------------------------*/

#define COMMAND_BUFFER_SIZE 25

#include <stdio.h> // sscanf
#include <stdint.h>
#include <getopt.h> // getopt
#include <ctype.h> // isprint

#include "sys_tick.h"
#include "log.h"
#include "led.h"
#include "Water_Meter_High_Pressure_Alarm.h" // Water_Meter_High_Pressure_Alarm_set_value()
#include "delivery_schedule.h"  // Delivery_Schedule_Diagnostics()
#include "sys_tick.h" // sys_tick_get_s()
#include "event_and_alarm_base.h"

/**-----------------------------------------------------------------------------
 *  @file
 *  @defgroup    simple_commandline
 *  @ingroup     simple_commandline
 *  @{
 *------------------------------------------------------------------------------
 */


static struct
{
  char commandbuff[COMMAND_BUFFER_SIZE];
  uint16_t commandbuff_pos;

  // For reloading last command
  char prev_commandbuff[COMMAND_BUFFER_SIZE];
  uint16_t prev_commandbuff_pos;
}
module;


#define printf(M, ...) log_raw(M, ##__VA_ARGS__)


/*******************************************************************************
  Command Logic
*******************************************************************************/

#ifdef UNIT_TEST
int cmd_event_and_alarm_base_diagnostics(int argc, char *argv[])
{
  event_and_alarm_base_diagnostics(sys_tick_get_s());
  return 0;
}
#else
int cmd_event_and_alarm_base_diagnostics(int argc, char *argv[]){return 0;}
#endif

int cmd_set_water_meter_alarm_value(int argc, char *argv[])
{
  int var = 0;
  sscanf( argv[1], "%d", &var );
  printf(": command 0: value=%d\r\n", var);
  #ifdef UNIT_TEST
  Water_Meter_High_Pressure_Alarm_set_value(0, (double) var);
  #endif

  return 0;
}

int cmd_delivery_schedule_diagnostics(int argc, char *argv[])
{
  uint32_t time_s = sys_tick_get_s(); // TODO: Convert to actual UNIX time
  uint32_t mock_unix_time = time_s + 86400000 - 3600; // 1000 days - 10 hours

  uint16_t instance           = 0;
  uint32_t next_tx_s          = 0;
  uint32_t next_win_s         = 0;
  uint32_t next_retry_s       = 0;
  uint32_t retry_count        = 0;
  bool transmission_active    = false;
  bool transmission_confirmed = false;  (void) transmission_confirmed;

  Delivery_Schedule_Diagnostics(instance, mock_unix_time, &next_tx_s, &next_win_s, &next_retry_s, &retry_count, &transmission_active, &transmission_confirmed);

  log_debug(": UNIX: %lumin | tx: %4lu mins | win: %4lu mins| rty: %4lu mins | rtry: %3lu mins | rty: %u | tx-on: %u",
      time_s/60,
      next_tx_s/60,
      next_win_s/60,
      next_retry_s/60,
      retry_count,
      transmission_active
    );

  return 0;
}

/*------------------------------------------------------------------------------
  This just prints a short message on common usage
  Embedded device don't have much space, so keep em short.
  You could perhaps print links to the help page for more details
------------------------------------------------------------------------------*/
static int cmd_help(int argc, char *argv[])
{
  printf(": e.g. `led 1` or `led 0`\r\n");
  printf(": For detailed readme refer to `https://github.com/ots-m2m/sew-lwm2m-reference-design`\r\n");
  return 0;
}

/*******************************************************************************
  Command List (Add new commands here)
*******************************************************************************/

static int command_dispatch(int argc, char *argv[])
{

// With return code
            /* Command ,  Function to launch                                  */
#define CMD_CASE(CMD_STR, FUNC) if (!strcmp(CMD_STR,argv[0])) { return FUNC; }
  CMD_CASE(     "help" ,            cmd_help(argc, argv));
  CMD_CASE(     "delivdiag" ,       cmd_delivery_schedule_diagnostics(argc, argv));
  CMD_CASE(     "set-pressure" ,    cmd_set_water_meter_alarm_value(argc, argv));
  CMD_CASE(     "eabdiag" ,         cmd_event_and_alarm_base_diagnostics(argc, argv));
#undef CMD_CASE



// No return code
            /* Command ,  Function to launch                                  */
#define CMD_CASE(CMD_STR, FUNC) if (!strcmp(CMD_STR,argv[0])) { FUNC; return 0; }
  CMD_CASE(     "led" ,              led_set(0, (argv[1][0]=='1')?true:false ));
#undef CMD_CASE


  printf(": '%s' Not Recognised (%s)\r\n", argv[0], argc==2 ? argv[1] : "");

  return 0;
}

/*******************************************************************************
  Super Simple Command Parser (argc argv style)
*******************************************************************************/

/**-----------------------------------------------------------------------------
  @brief Tokenise a string buffer into argc and argv format

  Tokenise string buffer to argc and argv form via strtok_r()
  Warning: Using strtok_r will modify the string buffer

  Returns: Number of tokens extracted

------------------------------------------------------------------------------*/
static int tokenise_to_argc_argv(
        char     *buffer,     ///< In/Out : Modifiable String Buffer To Tokenise
        int      *argc,       ///< Out    : Argument Count
        char     *argv[],     ///< Out    : Argument String Vector Array
        const int argv_length ///< In     : Maximum Count For `*argv[]`
      )
{ /* Tokenise string buffer into argc and argv format (req: string.h) */
  int i = 0;
  for (i = 0 ; i < argv_length ; i++)
  { /* Fill argv via strtok_r() */
    if ( NULL == (argv[i] = strtok_r( NULL , " ", &buffer)) ) break;
  }
  *argc = i;
  return i; // Argument Count
}

static int process_command_buffer(void)
{
  int   argc     = 0;
  char *argv[15] = {0};

  tokenise_to_argc_argv(module.commandbuff, &argc, argv, sizeof(argv));

  int return_code = command_dispatch(argc, argv);

  if (return_code != 0)
  {
      log_err("command returned: %d (0x%02X)", return_code, return_code);
  }

  return 0;
}

#if 0
static void print_buffer(char *buffer, int size)
{ /* Diagnostics for the command buffer */
  log_raw(" - Buffer Content `");
  for (int i = 0 ; i < size; i++) log_raw("%c",isprint(buffer[i])?buffer[i]:'0');
  log_raw("` | HEX: ");
  for (int i = 0 ; i < size; i++) log_raw("%02X ", buffer[i]);
  log_raw("\n");
}
#endif

/*******************************************************************************
  EXTERNAL INTERFACE
*******************************************************************************/

void simple_commandline_stream_parse(char inchar)
{

  if ( inchar == '\t')
  { // Reload last line
    memcpy(module.commandbuff, module.prev_commandbuff, module.prev_commandbuff_pos);
    module.commandbuff_pos = module.prev_commandbuff_pos;
    printf("\r\n%s", module.commandbuff);
  }
  else if ( (inchar == '\n') || (inchar == '\r') )
  { // Line Ready

    // Save command
    memcpy(module.prev_commandbuff, module.commandbuff, module.commandbuff_pos);
    module.prev_commandbuff_pos = module.commandbuff_pos;

    if (module.commandbuff_pos > 0)
    { // Buffer not empty
      printf("\n");
      process_command_buffer();
    }

    // Command Buffer Reset
    module.commandbuff_pos = 0;  // Reset command stream
    memset(module.commandbuff, 0, sizeof(module.commandbuff));
  }
  if ( (inchar == '\b') )
  {
    if (module.commandbuff_pos > 0)
    { // delete character
      printf("\b \b"); /* destructive backspace ('\b' only moves the cursor backwards) */
      module.commandbuff_pos--;
      module.commandbuff[module.commandbuff_pos] = '\0';
    }
  }
  else if (isprint(inchar))
  { // Save to buffer
    if (module.commandbuff_pos < (sizeof(module.commandbuff)-1) )
    {
      module.commandbuff[module.commandbuff_pos] = inchar;
      module.commandbuff[module.commandbuff_pos+1] = '\0';
      module.commandbuff_pos++;
      printf("%c", inchar);
    }
  }
#if 0
  print_buffer(module.commandbuff, sizeof(module.commandbuff));
#endif
}
