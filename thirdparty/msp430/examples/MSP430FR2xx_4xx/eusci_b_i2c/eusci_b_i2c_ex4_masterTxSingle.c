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
//*****************************************************************************
//! This example shows how to configure the I2C module as a master for
//! single byte transmission in interrupt mode. The address of the slave
//! module that the master is communicating with also set in this example.
//!
//! Demo - EUSCI_B0 I2C Master TX single bytes to MSP430 Slave
//!
//!  Description: This demo connects two MSP430's via the I2C bus. The master
//!  transmits to the slave. This is the master code. It continuously
//!  transmits 00h, 01h, ..., 0ffh and demonstrates how to implement an I2C
//!  master transmitter sending a single byte using the USCI_B0 TX interrupt.
//!  ACLK = n/a, MCLK = SMCLK = BRCLK = default DCO = ~1MHz
//!
//!                                /|\  /|\
// //!               MSP430FR4133      10k  10k     MSP430FR4133
//!                   slave         |    |        master
//!             -----------------   |    |   -----------------
//!           -|XIN  P5.2/UCB0SDA|<-|----+->|P5.2/UCB0SDA  XIN|-
//!            |                 |  |       |                 | 32kHz
//!           -|XOUT             |  |       |             XOUT|-
//!            |     P5.3/UCB0SCL|<-+------>|P5.3/UCB0SCL     |
//!            |                 |          |             P1.0|--> LED
//!
//! This example uses the following peripherals and I/O signals.  You must
//! review these and change as needed for your own board:
//! - I2C peripheral
//! - GPIO Port peripheral (for I2C pins)
//! - SCL2
//! - SDA
//! - CS
//!
//! This example uses the following interrupt handlers.  To use this example
//! in your own application you must add these interrupt handlers to your
//! vector table.
//! - USCI_B0_VECTOR.
//!
//
//*****************************************************************************
#include "driverlib.h"

//*****************************************************************************
//
//Set the address for slave module. This is a 7-bit address sent in the
//following format:
//[A6:A5:A4:A3:A2:A1:A0:RS]
//
//A zero in the "RS" position of the first byte means that the master
//transmits (sends) data to the selected slave, and a one in this position
//means that the master receives data from the slave.
//
//*****************************************************************************
#define SLAVE_ADDRESS 0x48

//*****************************************************************************
//
//Target frequency for SMCLK in kHz
//
//*****************************************************************************
#define CS_SMCLK_DESIRED_FREQUENCY_IN_KHZ   1000

//*****************************************************************************
//
//SMCLK/FLLRef Ratio
//
//*****************************************************************************
#define CS_SMCLK_FLLREF_RATIO   30

uint8_t transmitData;

void main(void)
{
    WDT_A_hold(WDT_A_BASE);

    //Set Ratio and Desired MCLK Frequency  and initialize DCO
    CS_initFLLSettle(
        CS_SMCLK_DESIRED_FREQUENCY_IN_KHZ,
        CS_SMCLK_FLLREF_RATIO
        );

    //Set SMCLK = DCO with frequency divider of 1
    CS_initClockSignal(
        CS_SMCLK,
        CS_DCOCLKDIV_SELECT,
        CS_CLOCK_DIVIDER_1
        );

    //Set MCLK = DCO with frequency divider of 1
    CS_initClockSignal(
        CS_MCLK,
        CS_DCOCLKDIV_SELECT,
        CS_CLOCK_DIVIDER_1
        );

    // Configure Pins for I2C
    /*
     * Select Port 5
     * Set Pin 2, 3 to input with function, (UCB0SIMO/UCB0SDA, UCB0SOMI/UCB0SCL).
     */
    GPIO_setAsPeripheralModuleFunctionInputPin(
        GPIO_PORT_P5,
        GPIO_PIN2 + GPIO_PIN3,
        GPIO_PRIMARY_MODULE_FUNCTION
        );

    /*
     * Disable the GPIO power-on default high-impedance mode to activate
     * previously configured port settings
     */
    PMM_unlockLPM5();

    //Initialize transmit data packet
    transmitData = 0x01;

    //Initialize Master
    EUSCI_B_I2C_initMasterParam param = {0};
    param.selectClockSource = EUSCI_B_I2C_CLOCKSOURCE_SMCLK;
    param.i2cClk = CS_getSMCLK();
    param.dataRate = EUSCI_B_I2C_SET_DATA_RATE_400KBPS;
    param.byteCounterThreshold = 1;
    param.autoSTOPGeneration = EUSCI_B_I2C_NO_AUTO_STOP;
    EUSCI_B_I2C_initMaster(EUSCI_B0_BASE, &param);

    //Specify slave address
    EUSCI_B_I2C_setSlaveAddress(EUSCI_B0_BASE,
                                SLAVE_ADDRESS
                                );

    //Set in transmit mode
    EUSCI_B_I2C_setMode(EUSCI_B0_BASE,
                        EUSCI_B_I2C_TRANSMIT_MODE
                        );

    //Enable I2C Module to start operations
    EUSCI_B_I2C_enable(EUSCI_B0_BASE);

    while(1)
    {
        //Send single byte data.
        EUSCI_B_I2C_masterSendSingleByte(EUSCI_B0_BASE,
                                         transmitData
                                         );

        //Delay until transmission completes
        while(EUSCI_B_I2C_isBusBusy(EUSCI_B0_BASE))
        {
            ;
        }

        //Delay between each transaction
        __delay_cycles(50);

        //Increment transmit data counter
        transmitData++;
    }
}
