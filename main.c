/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "log.h"
#include "driverlib.h"
#include "slip_network.h"
#include "sys_tick.h"
#include "lwm2m.h"
#include <assert.h>
#ifdef UNIT_TEST
#include "unit_tests.h"
#endif

#include <stdio.h> //sscanf

#include "led.h"  // led_init()
#include "simple_commandline.h" // simple_commandline_stream_parse()
#include "adc.h" // hardware_adc_init() hardware_adc_sample_tick()

/**-----------------------------------------------------------------------------
 *  @file
 *  @defgroup    lwm2m_client
 *  @ingroup     lwm2m_client
 *  @{
 *------------------------------------------------------------------------------
 */



void mcu_init(void)
{
  //Stop WDT
  WDT_A_hold(WDT_A_BASE);

  sys_clock_init();
}

int main(void)
{
  mcu_init();
  sys_tick_initialise();
  log_initialise();
  log_sys_clock();

  log_info_ccs("----------------------------------------");
  log_info_ccs("LWM2M Water Metering Reference Design");
  log_info_ccs("");
  log_info_ccs("Developed by Operational Technology Solutions");
  log_info_ccs("ot-solutions.com.au");
  log_info_ccs("----------------------------------------");

#ifdef UNIT_TEST
  log_info_ccs("press any key to run unit tests...");
  char c;
  if (log_getchar(&c, 2000) == 0)
  {
    log_info_ccs("----------------------------------------");
    log_info_ccs("running unit tests");
    unit_tests_run();
    log_info_ccs("----------------------------------------");
    log_info_ccs("unit test done. press any key to reset...");
    while(log_getchar(&c, 3000) != 0); // Press any key to proceed
    WDTCTL = 0; // Trip watchdog to trigger reset
    log_info_ccs("resetting...");
    return 0;
  }
  else
  {
    log_info_ccs("skipping unit tests...");
  }
#endif

  // SLIP Setup
  slip_network_initialise();
  slip_network_start();

  // Hardware Platform Initalisation
  led_init();
  hardware_adc_init();

  // ENTERING MAIN LOOP
  if (lwm2m_initialise())
  {
    while (1)
    {
      //process any packets arriving on the uart
      slip_network_tick();
      lwm2m_tick();

      // Hardware services
      hardware_adc_sample_tick();

#ifdef UNIT_TEST
      // Test UART interface handler
      char inchar = 0;
      if (log_getchar(&inchar, 0) == 0)
      { // New Input Character Detected
        simple_commandline_stream_parse(inchar);
      }
#endif
    }
  }

  return 0;
}

//
//FIXME: The watchdog is biting during startup init_bss because the memset is taking too long. There
//must be a better way of dealing with this that doesn't involve disabling the watchdog.
__attribute__((section(".crt_0001pre_init"))) __attribute__((naked)) void _system_pre_init (void)
{
  // disable watchdog timer
  WDTCTL = WDTPW + WDTHOLD;
}



