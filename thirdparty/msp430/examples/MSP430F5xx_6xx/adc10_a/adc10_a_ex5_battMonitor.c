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
//! ADC10_A - Sample A11 (AVcc/2), Battery Monitor, LED OFF if AVcc < 2.3V
//!
//!  MSP430F550x Demo
//!  A single sample is made on A11 (AVcc/2) with reference to internal 1.5V
//!  Vref. Software sets ADC10SC to start sample and conversion - ADC10SC
//!  automatically cleared at EOC. ADC10_A internal oscillator times sample (16x)
//!  and conversion. ADC10BUSY flag is polled for EOC. If A11 (AVcc/2) < 0311h
//!  (1.15V) indicating AVcc is less 2.3V, P1.0 reset indicating a lo_Batt
//!  condition, else set.
//!  ACLK = n/a, MCLK = SMCLK = default DCO ~1.2MHz, ADC10CLK = ADC10OSC
//!
//!                MSP430F550x
//!             -----------------
//!         /|\|              XIN|-
//!          | |                 |
//!          --|RST          XOUT|-
//!            |  (AVcc/2) -> A11|  [ Voltage source->Vcc + external pwr jumper]
//!            |             P1.0|-> LED  [ON if AVcc > 2.3V]
//!            |                 |
//!
//!
//! This example uses the following peripherals and I/O signals.  You must
//! review these and change as needed for your own board:
//! - ADC10_A peripheral
//! - GPIO Port peripheral
//! - A11
//!
//! This example uses the following interrupt handlers.  To use this example
//! in your own application you must add these interrupt handlers to your
//! vector table.
//! - None.
//!
//******************************************************************************
#include "driverlib.h"

void main(void)
{
    //Stop Watchdog Timer
    WDT_A_hold(WDT_A_BASE);

    //Initialize the ADC10_A Module
    /*
     * Base Address for the ADC10_A Module
     * Use internal ADC10_A bit as sample/hold signal to start conversion
     * USE MODOSC 5MHZ Digital Oscillator as clock source
     * Use default clock divider of 1
     */
    ADC10_A_init(ADC10_A_BASE,
                 ADC10_A_SAMPLEHOLDSOURCE_SC,
                 ADC10_A_CLOCKSOURCE_ADC10OSC,
                 ADC10_A_CLOCKDIVIDER_1);

    ADC10_A_enable(ADC10_A_BASE);

    /*
     * Base Address for the ADC10_A Module
     * Sample/hold for 16 clock cycles
     * Do not enable Multiple Sampling
     */
    ADC10_A_setupSamplingTimer(ADC10_A_BASE,
                               ADC10_A_CYCLEHOLD_16_CYCLES,
                               ADC10_A_MULTIPLESAMPLESDISABLE);

    //Configure Memory Buffer
    /*
     * Base Address for the ADC10_A Module
     * Use (Vcc-Vss)/2 as input
     * Use positive reference of Internally generated Vref
     * Use negative reference of AVss
     */
    ADC10_A_configureMemory(ADC10_A_BASE,
                            ADC10_A_INPUT_BATTERYMONITOR,
                            ADC10_A_VREFPOS_INT,
                            ADC10_A_VREFNEG_AVSS);

    //Configure internal reference
    //If ref generator busy, WAIT
    while(REF_BUSY == Ref_isRefGenBusy(REF_BASE))
    {
        ;
    }
    //Select internal ref = 1.5V
    Ref_setReferenceVoltage(REF_BASE,
                            REF_VREF1_5V);
    //Internal Reference ON
    Ref_enableReferenceVoltage(REF_BASE);

    //Delay (~75us) for Ref to settle
    __delay_cycles(75);

    //Set P1.0 to output direction
    GPIO_setAsOutputPin(
        GPIO_PORT_P1,
        GPIO_PIN0
        );

    //Clear P1.0 LED off
    GPIO_setOutputLowOnPin(
        GPIO_PORT_P1,
        GPIO_PIN0
        );

    while(1)
    {
        //Enable and Start the conversion
        //in Single-Channel, Single-Conversion Mode
        ADC10_A_startConversion(ADC10_A_BASE,
                                ADC10_A_SINGLECHANNEL);
        //ADC10BUSY?
        while(ADC10_A_isBusy(ADC10_A_BASE))
        {
            ;
        }

        //For debug only
        __no_operation();

        //Is ADC10MEM = A11 > 1.15V?
        if(ADC10_A_getResults(ADC10_A_BASE) < 0x311)
        {
            //Clear P1.0 LED off
            GPIO_setOutputLowOnPin(
                GPIO_PORT_P1,
                GPIO_PIN0
                );
        }
        else
        {
            //Set P1.0 LED on
            GPIO_setOutputHighOnPin(
                GPIO_PORT_P1,
                GPIO_PIN0
                );
        }
    }
}
