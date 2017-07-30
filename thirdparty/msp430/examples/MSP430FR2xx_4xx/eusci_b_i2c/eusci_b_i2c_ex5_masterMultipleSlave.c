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
//  Demo - EUSCI_B0 I2C Master TX bytes to Multiple Slaves
//
//  Description: This demo connects two MSP430's via the I2C bus.
//  The master transmits to 4 different I2C slave addresses 0x0A,0x0B,0x0C&0x0D.
//  Each slave address has a specific related data in the array TXData[].
//  At the end of four I2C transactions the slave address rolls over and begins
//  again at 0x0A.
//  ACLK = n/a, MCLK = SMCLK = BRCLK = default DCO = ~1MHz
//
//!                                /|\  /|\
// //!               MSP430FR4133      10k  10k     MSP430FR4133
//!                   slave         |    |        master
//!             -----------------   |    |   -----------------
//!           -|XIN  P5.2/UCB0SDA|<-|----+->|P5.2/UCB0SDA  XIN|-
//!            |                 |  |       |                 | 32kHz
//!           -|XOUT             |  |       |             XOUT|-
//!            |     P5.3/UCB0SCL|<-+------>|P5.3/UCB0SCL     |
//!            |                 |          |             P1.0|--> LED
//
//******************************************************************************
#include "driverlib.h"

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

uint8_t TXData[] = {0xA1,0xB1,0xC1,0xD1}; // Pointer to TX data
uint8_t SlaveAddress[] = {0x0A,0x0B,0x0C,0x0D};
uint8_t TXByteCtr;
uint8_t SlaveFlag = 0;

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

    EUSCI_B_I2C_initMasterParam param = {0};
    param.selectClockSource = EUSCI_B_I2C_CLOCKSOURCE_SMCLK;
    param.i2cClk = CS_getSMCLK();
    param.dataRate = EUSCI_B_I2C_SET_DATA_RATE_400KBPS;
    param.byteCounterThreshold = 0;
    param.autoSTOPGeneration = EUSCI_B_I2C_NO_AUTO_STOP;
    EUSCI_B_I2C_initMaster(EUSCI_B0_BASE, &param);

    //Set Master in receive mode
    EUSCI_B_I2C_setMode(EUSCI_B0_BASE,
                        EUSCI_B_I2C_TRANSMIT_MODE
                        );
    //Enable I2C Module to start operations
    EUSCI_B_I2C_enable(EUSCI_B0_BASE);

    EUSCI_B_I2C_clearInterrupt(EUSCI_B0_BASE,
                               EUSCI_B_I2C_TRANSMIT_INTERRUPT0 +
                               EUSCI_B_I2C_NAK_INTERRUPT
                               );

    //Enable master Receive interrupt
    EUSCI_B_I2C_enableInterrupt(EUSCI_B0_BASE,
                                EUSCI_B_I2C_TRANSMIT_INTERRUPT0 +
                                EUSCI_B_I2C_NAK_INTERRUPT
                                );

    SlaveFlag = 0;
    while(1)
    {
        //Specify slave address
        EUSCI_B_I2C_setSlaveAddress(EUSCI_B0_BASE,
                                    SlaveAddress[SlaveFlag]
                                    );

        TXByteCtr = 1;                          // Load TX byte counter

        while(EUSCI_B_I2C_SENDING_STOP == EUSCI_B_I2C_masterIsStopSent
                  (EUSCI_B0_BASE))
        {
            ;
        }

        EUSCI_B_I2C_masterSendStart(EUSCI_B0_BASE);

        __bis_SR_register(CPUOFF + GIE);        // Enter LPM0 w/ interrupts
        // Remain in LPM0 until all data
        // is TX'd
        // Change Slave address
        SlaveFlag++;
        if(SlaveFlag > 3)                        // Roll over slave address
        {
            SlaveFlag = 0;
            __delay_cycles(1000);               // Delay between transmissions
        }
    }
}

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=USCI_B0_VECTOR
__interrupt
#elif defined(__GNUC__)
__attribute__((interrupt(USCI_B0_VECTOR)))
#endif
void USCIB0_ISR(void)
{
    switch(__even_in_range(UCB0IV,0x1E))
    {
    case 0x00: break;                       // Vector 0: No interrupts break;
    case 0x02: break;
    case 0x04:
        EUSCI_B_I2C_masterSendStart(EUSCI_B0_BASE);
        break;                                // Vector 4: NACKIFG break;
    case 0x06: break;                       // Vector 6: STTIFG break;
    case 0x08: break;                       // Vector 8: STPIFG break;
    case 0x0a: break;                       // Vector 10: RXIFG3 break;
    case 0x0c: break;                       // Vector 14: TXIFG3 break;
    case 0x0e: break;                       // Vector 16: RXIFG2 break;
    case 0x10: break;                       // Vector 18: TXIFG2 break;
    case 0x12: break;                       // Vector 20: RXIFG1 break;
    case 0x14: break;                       // Vector 22: TXIFG1 break;
    case 0x16: break;                       // Vector 24: RXIFG0 break;
    case 0x18:
        if(TXByteCtr)                         // Check TX byte counter
        {
            EUSCI_B_I2C_masterSendMultiByteNext(EUSCI_B0_BASE,
                                                TXData[SlaveFlag]);
            TXByteCtr--;                        // Decrement TX byte counter
        }
        else
        {
            EUSCI_B_I2C_masterSendMultiByteStop(EUSCI_B0_BASE);
            __bic_SR_register_on_exit(CPUOFF);    // Exit LPM0
        }
        break;                              // Vector 26: TXIFG0 break;
    default: break;
    }
}
