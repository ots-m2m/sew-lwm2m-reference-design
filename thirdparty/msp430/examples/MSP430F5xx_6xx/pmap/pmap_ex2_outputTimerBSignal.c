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
//! MSP430F5529 Demo - Port Mapping single function to multiple pins; Single
//!                    runtime configuration.
//! Description:  Port 4 is port mapped to output TIMER_B digital signals.
//! TBCCR1-3 generate different DutyCycles and have been output on multiple pins.
//! ACLK = REFO~32kHz; MCLK = SMCLK = default DCO;
//!
//!                 MSP430F5529
//!             ------------------
//!         /|\|                  |
//!          | |                  |
//!          --|RST               |
//!            |     P4.0(TB0CCR1)|--> 25%
//!            |     P4.1(TB0CCR1)|--> 25%
//!            |     P4.2(TB0CCR2)|--> 50%
//!            |     P4.3(TB0CCR2)|--> 50%
//!            |     P4.4(TB0CCR3)|--> 75%
//!            |     P4.5(TB0CCR3)|--> 75%
//!            |     P4.6(default)|--> DVSS
//!            |     P4.7(default)|--> DVSS
//!
//! This example uses the following peripherals and I/O signals.  You must
//! review these and change as needed for your own board:
//! - PMAP module
//! - GPIO Port peripheral
//! - UCS module
//! - Timer module
//!
//! This example uses the following interrupt handlers.  To use this example
//! in your own application you must add these interrupt handlers to your
//! vector table.
//! - None.
//!
//******************************************************************************
#include "driverlib.h"

//*****************************************************************************
//
//Refer Datasheet for Port4 Port Mapping definitions
//
//*****************************************************************************
const uint8_t port_mapping[] = {
    //Port P4:
    PM_TB0CCR1A,
    PM_TB0CCR1A,
    PM_TB0CCR2A,
    PM_TB0CCR2A,
    PM_TB0CCR3A,
    PM_TB0CCR3A,
    PM_NONE,
    PM_NONE,
};

void main(void)
{
    //Stop WDT
    WDT_A_hold(WDT_A_BASE);

    //Set DCO FLL reference = REFO
    UCS_initClockSignal(
        UCS_FLLREF,
        UCS_REFOCLK_SELECT,
        UCS_CLOCK_DIVIDER_1
        );

    //VLO Clock Sources ACLK
    UCS_initClockSignal(
        UCS_ACLK,
        UCS_REFOCLK_SELECT,
        UCS_CLOCK_DIVIDER_1
        );

    //Set Ratio and Desired MCLK Frequency  and initialize DCO
    UCS_initFLLSettle(
        1048,
        32
        );

    //Setup Port Pins
    //P4.0 - P4.7 output
    //P4.0 - P4.6 Port Map functions
    GPIO_setAsPeripheralModuleFunctionOutputPin(
        GPIO_PORT_P4,
        GPIO_PIN0 + GPIO_PIN1 + GPIO_PIN2 +
        GPIO_PIN3 + GPIO_PIN4 + GPIO_PIN5 +
        GPIO_PIN6 + GPIO_PIN7 + GPIO_PIN8 +
        GPIO_PIN9 + GPIO_PIN10 + GPIO_PIN11 +
        GPIO_PIN12 + GPIO_PIN13 + GPIO_PIN14 +
        GPIO_PIN15
        );

    //CONFIGURE PORTS- pass the port_mapping array, start @ P4MAP01, initialize
    //a single port, do not allow run-time reconfiguration of port mapping
    PMAP_initPortsParam initPortsParam = {0};
    initPortsParam.portMapping = port_mapping;
    initPortsParam.PxMAPy = (uint8_t  *)&P4MAP01;
    initPortsParam.numberOfPorts = 1;
    initPortsParam.portMapReconfigure = PMAP_DISABLE_RECONFIGURATION;
    PMAP_initPorts(PMAP_CTRL_BASE, &initPortsParam);

    //Start Up Down Timer
    Timer_B_initUpDownModeParam param1 = {0};
    param1.clockSource = TIMER_B_CLOCKSOURCE_ACLK;
    param1.clockSourceDivider = TIMER_B_CLOCKSOURCE_DIVIDER_2;
    param1.timerPeriod = 128;
    param1.timerInterruptEnable_TBIE = TIMER_B_TBIE_INTERRUPT_DISABLE;
    param1.captureCompareInterruptEnable_CCR0_CCIE =
        TIMER_B_CCIE_CCR0_INTERRUPT_DISABLE;
    param1.timerClear = TIMER_B_SKIP_CLEAR;
    param1.startTimer = true;
    Timer_B_initUpDownMode(TIMER_B0_BASE, &param1);

    //Generate PWM 1
    Timer_B_initCompareModeParam param2 = {0};
    param2.compareRegister = TIMER_B_CAPTURECOMPARE_REGISTER_1;
    param2.compareInterruptEnable = TIMER_B_CAPTURECOMPARE_INTERRUPT_DISABLE;
    param2.compareOutputMode = TIMER_B_OUTPUTMODE_TOGGLE_SET;
    param2.compareValue = 96;
    Timer_B_initCompareMode(TIMER_B0_BASE, &param2);

    //Generate PWM 2
    param2.compareRegister = TIMER_B_CAPTURECOMPARE_REGISTER_2;
    param2.compareInterruptEnable = TIMER_B_CAPTURECOMPARE_INTERRUPT_DISABLE;
    param2.compareOutputMode = TIMER_B_OUTPUTMODE_TOGGLE_SET;
    param2.compareValue = 64;
    Timer_B_initCompareMode(TIMER_B0_BASE, &param2);

    //Generate PWM 3
    param2.compareRegister = TIMER_B_CAPTURECOMPARE_REGISTER_3;
    param2.compareInterruptEnable = TIMER_B_CAPTURECOMPARE_INTERRUPT_DISABLE;
    param2.compareOutputMode = TIMER_B_OUTPUTMODE_TOGGLE_SET;
    param2.compareValue = 32;
    Timer_B_initCompareMode(TIMER_B0_BASE, &param2);

    //Enter LPM3
    __bis_SR_register(LPM3_bits);

    //For debugger
    __no_operation();
}
