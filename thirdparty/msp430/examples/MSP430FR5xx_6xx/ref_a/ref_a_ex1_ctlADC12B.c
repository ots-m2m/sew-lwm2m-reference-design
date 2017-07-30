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
//
//   REFA - ADC12B, Using REFA Module to control Internal Reference
//
//   This example shows how to configure the internal reference of REFA module
//   using the REFACTL0 control register. The ADC12B uses the internal 2.5V
//   reference and performs a single conversion on channel A0. The conversion
//   results are stored in ADC12BMEM0. Test by applying a voltage to channel A0,
//   then setting and running to a break point at the "__no_operation()"
//   instruction. To view the conversion results, open an ADC12B register window
//   in debugger and view the contents of ADC12BMEM0.
//
//   NOTE: REFAMSTR bit in REFACTL0 regsiter is by default set to 1. This allows
//   the REFACTL0 control regsiters to configure the reference setting. The
//   legacy ADC12B reference control bits ADC12REFAON, ADC12REFA2_5, ADC12BTCOFF
//   and ADC12BREFAOUT are dont care in this case.
//
//                 MSP430FR5969
//              -----------------
//          /|\|                 |
//           | |                 |
//           --|RST              |
//             |                 |
//             |          P1.0/A0|<- Vin
//             |                 |
//
//
//   This example uses the following peripherals and I/O signals.  You must
//   review these and change as needed for your own board:
//   - REFA peripheral
//   - ADC12B peripheral
//   - GPIO Port peripheral
//   - A0
//
//   This example uses the following interrupt handlers.  To use this example
//   in your own application you must add these interrupt handlers to your
//   vector table.
//   - None.
//
//******************************************************************************
#include "driverlib.h"

void main(void)
{
    //Stop watchdog timer
    WDT_A_hold(WDT_A_BASE);

    //Set P1.0 as Ternary Module Function Output.
    /*

     * Select Port 1
     * Set Pin 0 to output Ternary Module Function, (A0, C0, VREFA-, VeREFA-).
     */
    GPIO_setAsPeripheralModuleFunctionOutputPin(
        GPIO_PORT_P1,
        GPIO_PIN0,
        GPIO_TERNARY_MODULE_FUNCTION
        );

    /*
     * Disable the GPIO power-on default high-impedance mode to activate
     * previously configured port settings
     */
    PMM_unlockLPM5();

    //If ref generator busy, WAIT
    while(Ref_A_isRefGenBusy(REF_A_BASE))
    {
        ;
    }
    //Select internal ref = 2.5V
    Ref_A_setReferenceVoltage(REF_A_BASE,
                              REF_A_VREF2_5V);
    //Internal Reference ON
    Ref_A_enableReferenceVoltage(REF_A_BASE);

    //Delay (~75us) for Ref to settle
    __delay_cycles(75);

    //Initialize the ADC12 Module
    /*
     * Base address of ADC12 Module
     * Use internal ADC12 bit as sample/hold signal to start conversion
     * USE MODOSC 5MHZ Digital Oscillator as clock source
     * Use default clock divider/pre-divider of 1
     * Map to internal channel 0
     */
    ADC12_B_initParam initParam = {0};
    initParam.sampleHoldSignalSourceSelect = ADC12_B_SAMPLEHOLDSOURCE_SC;
    initParam.clockSourceSelect = ADC12_B_CLOCKSOURCE_ADC12OSC;
    initParam.clockSourceDivider = ADC12_B_CLOCKDIVIDER_1;
    initParam.clockSourcePredivider = ADC12_B_CLOCKPREDIVIDER__1;
    initParam.internalChannelMap = ADC12_B_MAPINTCH0;
    ADC12_B_init(ADC12_B_BASE, &initParam);

    //Enable the ADC12B module
    ADC12_B_enable(ADC12_B_BASE);

    /*
     * Base address of ADC12B Module
     * For memory buffers 0-7 sample/hold for 64 clock cycles
     * For memory buffers 8-15 sample/hold for 4 clock cycles (default)
     * Disable Multiple Sampling
     */
    ADC12_B_setupSamplingTimer(ADC12_B_BASE,
                               ADC12_B_CYCLEHOLD_64_CYCLES,
                               ADC12_B_CYCLEHOLD_4_CYCLES,
                               ADC12_B_MULTIPLESAMPLESDISABLE);

    //Configure Memory Buffer
    /*
     * Base address of the ADC12B Module
     * Configure memory buffer 0
     * Map input A0 to memory buffer 0
     * Vref+ = Vref+ (INT)
     * Vref- = AVss
     */
    ADC12_B_configureMemoryParam configureMemoryParam = {0};
    configureMemoryParam.memoryBufferControlIndex = ADC12_B_MEMORY_0;
    configureMemoryParam.inputSourceSelect = ADC12_B_INPUT_A0;
    configureMemoryParam.refVoltageSourceSelect =
        ADC12_B_VREFPOS_INTBUF_VREFNEG_VSS;
    configureMemoryParam.endOfSequence = ADC12_B_NOTENDOFSEQUENCE;
    configureMemoryParam.windowComparatorSelect =
        ADC12_B_WINDOW_COMPARATOR_DISABLE;
    configureMemoryParam.differentialModeSelect =
        ADC12_B_DIFFERENTIAL_MODE_DISABLE;
    ADC12_B_configureMemory(ADC12_B_BASE, &configureMemoryParam);

    while(1)
    {
        //Enable/Start sampling and conversion
        /*
         * Base address of ADC12B Module
         * Start the conversion into memory buffer 0
         * Use the single-channel, single-conversion mode
         */
        ADC12_B_startConversion(ADC12_B_BASE,
                                ADC12_B_MEMORY_0,
                                ADC12_B_SINGLECHANNEL);

        //Poll for interrupt on memory buffer 0
        while(!ADC12_B_getInterruptStatus(ADC12_B_BASE,
                                          0,
                                          ADC12IFG0))
        {
            ;
        }

        //SET BREAKPOINT HERE
        __no_operation();
    }
}
