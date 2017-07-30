/* --COPYRIGHT--,BSD
 * Copyright (c) 2016, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * --/COPYRIGHT--*/
//******************************************************************************
//!
//! ADC - Sample A2 input, 1.5V Shared Ref, LED ON if A2 > 0.5V
//!
//!  MSP430FR4133 Demo
//!  A single sample is made on A2 with reference to internal 1.5V Vref.Software
//!  sets ADC10SC to start sample and conversion - ADC10SC automatically cleared
//!  at EOC. ADC internal oscillator times sample (16x) and conversion. In
//!  Mainloop MSP430 waits in LPM0 to save power until ADC conversion complete,
//!  ADC10_ISR will force exit from LPM0 in Mainloop on reti. If A2 > 0.5V, P1.0
//!  set, else reset.
//!
//!                MSP430FR4133
//!             -----------------
//!         /|\|              XIN|-
//!          | |                 |
//!          --|RST          XOUT|-
//!            |          P1.2/A2|<- Sample Signal
//!            |             P1.0|-> LED
//!            |             P3.5|-->LED
//!
//!
//! This example uses the following peripherals and I/O signals.  You must
//! review these and change as needed for your own board:
//! - ADC peripheral
//! - REF peripheral
//! - Timer_A peripheral
//! - GPIO Port peripheral
//!
//! This example uses the following interrupt handlers.  To use this example
//! in your own application you must add these interrupt handlers to your
//! vector table.
//! - ADC10_VECTOR
//! - TIMER0_A0_VECTOR
//!
//!   Built with IAR Embedded Workbench Version: 5.40 & Code Composer Studio V5.1
//******************************************************************************
#include "driverlib.h"

#define TIMER_PERIOD 80

void main(void)
{
    //Stop Watchdog Timer
    WDT_A_hold(WDT_A_BASE);

    //Set P1.2 as an input pin.
    /*
     * Select Port 1
     * Set Pin 2 as input
     * Set Ternary module function
     */
    GPIO_setAsPeripheralModuleFunctionInputPin(
        GPIO_PORT_P1,
        GPIO_PIN2,
        GPIO_PRIMARY_MODULE_FUNCTION);

    //Set P1.0 as an output pin.
    /*
     * Select Port 1
     * Set Pin 0 as output
     */
    GPIO_setAsOutputPin(
        GPIO_PORT_P1,
        GPIO_PIN0);

    //Set P3.5 as an output pin.
    /*

     * Select Port 3
     * Set Pin 5 as output
     */
    GPIO_setAsOutputPin(
        GPIO_PORT_P3,
        GPIO_PIN5);

    /*
     * Disable the GPIO power-on default high-impedance mode to activate
     * previously configured port settings
     */
    PMM_unlockLPM5();

    //Initialize the ADC Module
    /*
     * Base Address for the ADC Module
     * Use internal ADC bit as sample/hold signal to start conversion
     * USE MODOSC 5MHZ Digital Oscillator as clock source
     * Use default clock divider of 1
     */
    ADC_init(ADC_BASE,
             ADC_SAMPLEHOLDSOURCE_SC,
             ADC_CLOCKSOURCE_ADCOSC,
             ADC_CLOCKDIVIDER_1);

    ADC_enable(ADC_BASE);

    /*
     * Base Address for the ADC Module
     * Sample/hold for 16 clock cycles
     * Do not enable Multiple Sampling
     */
    ADC_setupSamplingTimer(ADC_BASE,
                           ADC_CYCLEHOLD_16_CYCLES,
                           ADC_MULTIPLESAMPLESDISABLE);

    //Configure Memory Buffer
    /*
     * Base Address for the ADC Module
     * Use input A2
     * Use positive reference of Internally generated Vref
     * Use negative reference of AVss
     */
    ADC_configureMemory(ADC_BASE,
                        ADC_INPUT_A2,
                        ADC_VREFPOS_INT,
                        ADC_VREFNEG_AVSS);

    ADC_clearInterrupt(ADC_BASE,
                       ADC_COMPLETED_INTERRUPT);

    //Enable Memory Buffer interrupt
    ADC_enableInterrupt(ADC_BASE,
                        ADC_COMPLETED_INTERRUPT);

    //Configure internal reference
    //If ref voltage no ready, WAIT
    while(PMM_REFGEN_NOTREADY ==
          PMM_getVariableReferenceVoltageStatus())
    {
        ;
    }

    //Internal Reference ON
    PMM_enableInternalReference();

    // Configure TA0 to provide delay for reference settling ~75us
    Timer_A_initUpModeParam initUpModeParam = {0};
    initUpModeParam.clockSource = TIMER_A_CLOCKSOURCE_SMCLK;
    initUpModeParam.clockSourceDivider = TIMER_A_CLOCKSOURCE_DIVIDER_1;
    initUpModeParam.timerPeriod = TIMER_PERIOD;
    initUpModeParam.timerInterruptEnable_TAIE = TIMER_A_TAIE_INTERRUPT_DISABLE;
    initUpModeParam.captureCompareInterruptEnable_CCR0_CCIE =
        TIMER_A_CAPTURECOMPARE_INTERRUPT_ENABLE;
    initUpModeParam.timerClear = TIMER_A_DO_CLEAR;
    initUpModeParam.startTimer = true;
    Timer_A_initUpMode(TIMER_A0_BASE, &initUpModeParam);

    __bis_SR_register(CPUOFF + GIE);           // LPM0, TA0_ISR will force exit

    for(;; )
    {
        //Delay between conversions
        __delay_cycles(5000);

        //Enable and Start the conversion
        //in Single-Channel, Single Conversion Mode
        ADC_startConversion(ADC_BASE,
                            ADC_SINGLECHANNEL);

        //LPM0, ADC10_ISR will force exit
        __bis_SR_register(CPUOFF + GIE);
        //For debug only
        __no_operation();
    }
}

//ADC10 interrupt service routine
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=ADC_VECTOR
__interrupt
#elif defined(__GNUC__)
__attribute__((interrupt(ADC_VECTOR)))
#endif
void ADC_ISR(void)
{
    switch(__even_in_range(ADCIV,12))
    {
    case  0: break;     //No interrupt
    case  2: break;     //conversion result overflow
    case  4: break;     //conversion time overflow
    case  6: break;     //ADC10HI
    case  8: break;     //ADC10LO
    case 10: break;     //ADC10IN
    case 12:            //ADC10IFG0

        //Automatically clears ADC10IFG0 by reading memory buffer
        //ADC10MEM = A0 > 0.5V?
        if(ADC_getResults(ADC_BASE) < 0x155)
        {
            //Clear P1.0 LED off
            /*
             * Select Port 1
             * Set Pin 0 to output Low.
             */
            GPIO_setOutputLowOnPin(
                GPIO_PORT_P1,
                GPIO_PIN0
                );

            //Clear P3.5 LED off
            /*

             * Select Port 3
             * Set Pin 5 to output Low.
             */
            GPIO_setOutputLowOnPin(
                GPIO_PORT_P3,
                GPIO_PIN5
                );
        }
        else
        {
            //Set P1.0 LED on
            /*

             * Select Port 1
             * Set Pin 0 to output high.
             */
            GPIO_setOutputHighOnPin(
                GPIO_PORT_P1,
                GPIO_PIN0
                );

            //Set P3.5 LED on
            /*

             * Select Port 3
             * Set Pin 5 to output high.
             */
            GPIO_setOutputHighOnPin(
                GPIO_PORT_P3,
                GPIO_PIN5
                );
        }

        //Clear CPUOFF bit from 0(SR)
        //Breakpoint here and watch ADC_Result
        __bic_SR_register_on_exit(CPUOFF);
        break;
    default: break;
    }
}

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=TIMER0_A0_VECTOR
__interrupt
#elif defined(__GNUC__)
__attribute__((interrupt(TIMER0_A0_VECTOR)))
#endif
void TA0_ISR(void)
{
    TA0CTL = 0;
    LPM0_EXIT;                                  // Exit LPM0 on return
}
