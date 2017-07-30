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
//!   MSP430F530x Demo - Internal LDO enabled after device power-up
//!
//!  Description: This demo code shows example case where device is powered from
//!  an external supply and the LDO input coming on is detected to enable
//!  external system circuitry. When sufficient voltage at LDOI is present, P1.0
//!  is set, else reset; thus reflecting the status of the LDO being enabled or not
//!
//!   Note:
//!    1.External supply is used to power the device VCC. If using TI target board
//!      (MSP-TS430RGC64B), remove jumper on JP4
//!
//!   ACLK = 32.768kHz, MCLK = SMCLK = default DCO~1MHz
//!
//!                MSP430F530x
//!             -----------------
//!        /|\|              VCC|<- External Supply
//!         | |                 |
//!         --|RST              |
//!           |             LDOO|--> 3.3V LDO Output
//!           |             LDOI|<-- LDO Input Voltage (3.76V to 5.5V - refer to the d/s
//!                                     specs for actual range)
//!           |                 |
//!           |             P1.0|--> LED - Reflects LDO status
//!           |                 |
//!
//! This example uses the following peripherals and I/O signals.  You must
//! review these and change as needed for your own board:
//! - LDO-PWR module
//! - GPIO Port peripheral
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
    //Stop WDT
    WDT_A_hold(WDT_A_BASE);

    //Set P1.0 to output direction
    GPIO_setAsOutputPin(
        GPIO_PORT_P1,
        GPIO_PIN0
        );

    //Detect if sufficient voltage at LDOI is present
    if(LDOPWR_LDO_INPUT_VALID == LDOPWR_isLDOInputValid(
           LDOPWR_BASE
           ))
    {
        //Valid LDOI, LED on
        GPIO_setOutputHighOnPin(
            GPIO_PORT_P1,
            GPIO_PIN0
            );
    }
    else
    {
        //Clear P1.0 LED off
        GPIO_setOutputLowOnPin(
            GPIO_PORT_P1,
            GPIO_PIN0
            );
    }

    //Enable access to config registers
    LDOPWR_unLockConfiguration(LDOPWR_BASE);

    //Clear all LDO-PWR IFGs before enabled IE bits
    LDOPWR_clearInterrupt(LDOPWR_BASE,
                          LDOPWR_LDOI_VOLTAGE_GOING_OFF_INTERRUPT +
                          LDOPWR_LDOI_VOLTAGE_COMING_ON_INTERRUPT +
                          LDOPWR_LDO_OVERLOAD_INDICATION_INTERRUPT
                          );

    //Enable LDO-PWR interrupts
    LDOPWR_enableInterrupt(LDOPWR_BASE,
                           LDOPWR_LDOI_VOLTAGE_GOING_OFF_INTERRUPT +
                           LDOPWR_LDOI_VOLTAGE_COMING_ON_INTERRUPT +
                           LDOPWR_LDO_OVERLOAD_INDICATION_INTERRUPT
                           );

    //Disbale access to config registers
    LDOPWR_lockConfiguration(LDOPWR_BASE);

    //Enter LPM3 w/ interrupts
    __bis_SR_register(LPM3_bits + GIE);

    //For debugger
    __no_operation();
}

//******************************************************************************
//
//This is the LDO_PWR_VECTOR interrupt vector service routine.
//
//******************************************************************************
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=LDO_PWR_VECTOR
__interrupt
#elif defined(__GNUC__)
__attribute__((interrupt(LDO_PWR_VECTOR)))
#endif
void LDOInterruptHandler(void)
{
    if(LDOPWR_getInterruptStatus(LDOPWR_BASE,
                                 LDOPWR_LDOI_VOLTAGE_COMING_ON_INTERRUPT
                                 ))
    {
        //Enable access to config registers
        LDOPWR_unLockConfiguration(LDOPWR_BASE);

        //Software clear IFG
        //Its possible that current peaks due to charging cap
        //at LDOO might cause the LDOOVLIFG to trigger
        LDOPWR_clearInterrupt(LDOPWR_BASE,
                              LDOPWR_LDO_OVERLOAD_INDICATION_INTERRUPT +
                              LDOPWR_LDOI_VOLTAGE_COMING_ON_INTERRUPT
                              );

        //Disbale access to config registers
        LDOPWR_lockConfiguration(LDOPWR_BASE);

        //Valid LDOI, LED on
        GPIO_setOutputHighOnPin(
            GPIO_PORT_P1,
            GPIO_PIN0
            );
    }
    else if(LDOPWR_getInterruptStatus(LDOPWR_BASE,
                                      LDOPWR_LDO_OVERLOAD_INDICATION_INTERRUPT
                                      ))
    {
        //Enable access to config registers
        LDOPWR_unLockConfiguration(LDOPWR_BASE);

        //Software clear IFG
        LDOPWR_clearInterrupt(LDOPWR_BASE,
                              LDOPWR_LDO_OVERLOAD_INDICATION_INTERRUPT
                              );

        //Disbale access to config registers
        LDOPWR_lockConfiguration(LDOPWR_BASE);

        //Over load indication; take necessary steps in application firmware
        while(1)
        {
            ;
        }
    }
    else if(LDOPWR_getInterruptStatus(LDOPWR_BASE,
                                      LDOPWR_LDOI_VOLTAGE_GOING_OFF_INTERRUPT
                                      ))
    {
        //Enable access to config registers
        LDOPWR_unLockConfiguration(LDOPWR_BASE);

        //Software clear IFG
        LDOPWR_clearInterrupt(LDOPWR_BASE,
                              LDOPWR_LDOI_VOLTAGE_GOING_OFF_INTERRUPT
                              );

        //Disbale access to config registers
        LDOPWR_lockConfiguration(LDOPWR_BASE);

        //Voltage LDOI < launch voltage, LED off
        GPIO_setOutputLowOnPin(
            GPIO_PORT_P1,
            GPIO_PIN0
            );
    }
}
