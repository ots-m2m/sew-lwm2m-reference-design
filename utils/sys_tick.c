/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "sys_tick.h"
#include "driverlib.h"
#include "log.h"

volatile static uint32_t sys_tick_ms = 0;
volatile static uint32_t sys_tick_s = 0;
volatile static uint32_t sys_tick_monotonic_ms = 0;

//change tis to match the SMCLK frequency of the system

#define SYS_TICK_MS 1

#define MCLK_FREQ_KHZ 20000
#define SMCLK_FREQUENCY ((uint32_t)MCLK_FREQ_KHZ * 1000)

//this is the number of SMCLK cycles between sys ticks.
#define CYCLES_PER_TICK (SYS_TICK_MS * SMCLK_FREQUENCY / 1000)

// Reference frequency (Frequency Locked Loop)
#define FLLREF_KHZ 32
// Ratio used to set DCO (Digitally Controlled Oscillator)
#define MCLK_FLLREF_RATIO MCLK_FREQ_KHZ/FLLREF_KHZ

uint32_t ACLK_Freq;
uint32_t SMCLK_Freq;
uint32_t MCLK_Freq;

void log_sys_clock(void)
{
  log_info_ccs("ACLK %lu SMCLK %lu MCLK %lu", ACLK_Freq, SMCLK_Freq, MCLK_Freq);
}

void sys_clock_init(void)
{
    PMM_setVCore(PMM_CORE_LEVEL_2);

    //Set P1.0 to output direction
    GPIO_setAsOutputPin(
      GPIO_PORT_P1,
      GPIO_PIN0
      );

    //ACLK, MCLK, MCLK set out to pins
    GPIO_setAsPeripheralModuleFunctionOutputPin(
      GPIO_PORT_P11,
      GPIO_PIN0 + GPIO_PIN1 + GPIO_PIN2
      );

    //Set DCO FLL reference = REFO
    UCS_initClockSignal(
      UCS_FLLREF,
      UCS_REFOCLK_SELECT,
      UCS_CLOCK_DIVIDER_1
      );

    //Set ACLK = REFO
    UCS_initClockSignal(
      UCS_ACLK,
      UCS_REFOCLK_SELECT,
      UCS_CLOCK_DIVIDER_1
      );

//    //Set Ratio and Desired MCLK Frequency  and initialize DCO
    UCS_initFLLSettle(
        MCLK_FREQ_KHZ,
        MCLK_FLLREF_RATIO
    );

    // Enable global oscillator fault flag
    SFR_clearInterrupt(SFR_OSCILLATOR_FAULT_INTERRUPT);
    SFR_enableInterrupt(SFR_OSCILLATOR_FAULT_INTERRUPT);

    // Enable global interrupt
    __bis_SR_register(GIE);

    // get new clock values
    ACLK_Freq = UCS_getACLK();
    SMCLK_Freq = UCS_getSMCLK();
    MCLK_Freq = UCS_getMCLK();
}

__attribute__((interrupt(UNMI_VECTOR))) void NMI_ISR(void)
{
  uint16_t status;
  do
  {
    // If it still can't clear the oscillator fault flags after the timeout,
    // trap and wait here.
    status = UCS_clearAllOscFlagsWithTimeout(1000);
  }
  while(status != 0);
}

void sys_tick_initialise(void)
{
  //Start timer in continuous mode sourced by SMCLK
  Timer_A_initContinuousModeParam initContParam = {0};
  initContParam.clockSource = TIMER_A_CLOCKSOURCE_SMCLK;
  initContParam.clockSourceDivider = TIMER_A_CLOCKSOURCE_DIVIDER_1;
  initContParam.timerInterruptEnable_TAIE = TIMER_A_TAIE_INTERRUPT_DISABLE;
  initContParam.timerClear = TIMER_A_DO_CLEAR;
  initContParam.startTimer = false;
  Timer_A_initContinuousMode(TIMER_A1_BASE, &initContParam);

  //Initiaze compare mode
  Timer_A_clearCaptureCompareInterrupt(TIMER_A1_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_0);

  Timer_A_initCompareModeParam initCompParam = {0};
  initCompParam.compareRegister = TIMER_A_CAPTURECOMPARE_REGISTER_0;
  initCompParam.compareInterruptEnable =
      TIMER_A_CAPTURECOMPARE_INTERRUPT_ENABLE;
  initCompParam.compareOutputMode = TIMER_A_OUTPUTMODE_OUTBITVALUE;
  initCompParam.compareValue = CYCLES_PER_TICK;
  Timer_A_initCompareMode(TIMER_A1_BASE, &initCompParam);

  Timer_A_startCounter(TIMER_A1_BASE, TIMER_A_CONTINUOUS_MODE);

}

uint32_t sys_tick_get_s(void)
{
  return sys_tick_s;
}

uint32_t sys_tick_get_ms(void)
{
  return sys_tick_ms;
}

uint32_t sys_tick_get_monotonic_ms(void)
{
  return sys_tick_monotonic_ms;
}

void msleep(uint32_t ms)
{
  volatile uint32_t st_s = sys_tick_s;
  volatile uint32_t s_to_wait = ms / 1000;
  volatile uint32_t ms_to_wait = ms % 1000;
  volatile uint32_t now = sys_tick_s;
  //wait the seconds
  while ((st_s + s_to_wait) > now) {  now = sys_tick_s; }
  uint32_t st_ms = sys_tick_ms;
  if (ms_to_wait < 1000 - st_ms)
  {
    now = sys_tick_ms;
    while ((st_ms + ms_to_wait) > now) { now = sys_tick_ms; }
  }
  else //its gonna wrap
  {
     while (sys_tick_ms > 0) { }
     now = sys_tick_ms;
     while ((ms_to_wait - (1000 - st_ms)) < sys_tick_ms) { now = sys_tick_ms; }
  }
}


__attribute__((interrupt(TIMER1_A0_VECTOR)))
__attribute__((used))
void TIMER1_A0_ISR(void)
{
  sys_tick_monotonic_ms++;
  sys_tick_ms++;
  if (sys_tick_ms >= (SYS_TICK_MS * 1000))
  {
    sys_tick_ms = 0;
    sys_tick_s++;
  }
  uint16_t compVal = Timer_A_getCaptureCompareCount(TIMER_A1_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_0) + CYCLES_PER_TICK;

  //Add Offset to CCR0
  Timer_A_setCompareValue(TIMER_A1_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_0, compVal );
}
