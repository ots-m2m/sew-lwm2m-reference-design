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
/*******************************************************************************
 * MSP430i2xx EUSCI_A SPI - 3 Wire Incrementing Data Master
 *
 * Description: In this example, the device is configured as a SPI master using
 * ACLK as the clock source. The SPI communication is setup to run at 16kHz.
 * The initial data is written to the TX Buffer, once the data is sent out the
 * device gets an RX interrupt that contains the data it recieved from the
 * slave. The slave and master data values should stay in sync and if there is
 * a problem in communication the LED will turn on. The device stays in LPM0
 * and sends the data in the ISR. To properly demo this example connect to
 * slave version of this example and start the slave device first.
 *
 *                MSP430i2041
 *             ------------------
 *         /|\|                  |
 *          | |              P1.4|--->LED
 *          --|RST               |
 *            |     P1.3/UCA0SIMO|--->Data Out
 *            |                  |
 *            |     P1.2/UCA0MISO|<---Data In
 *            |                  |
 *            |      P1.1/UCA0CLK|--->Serial Clock
 *
 * Author: Zack Lalanne
 ******************************************************************************/
#include "driverlib.h"

volatile uint8_t TXData = 0;
volatile uint8_t RXData = 0;

int main(void) {
    EUSCI_A_SPI_initMasterParam spiMasterConfig = {
        EUSCI_A_SPI_CLOCKSOURCE_ACLK,              // ACLK Clock Source
        32000,                                     // ACLK = 32kHz
        16000,                                     // SPICLK = 16kHz
        EUSCI_A_SPI_MSB_FIRST,                     // MSB First
        EUSCI_A_SPI_PHASE_DATA_CHANGED_ONFIRST_CAPTURED_ON_NEXT,    // Phase
        EUSCI_A_SPI_CLOCKPOLARITY_INACTIVITY_HIGH, // High polarity
        EUSCI_A_SPI_3PIN                           // 3Wire SPI Mode
    };

    WDT_hold(WDT_BASE);

    // Setting P1.1, P1.2 and P1.3 as SPI pins.
    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P1,
                                               GPIO_PIN1 | GPIO_PIN2 | GPIO_PIN3,
                                               GPIO_PRIMARY_MODULE_FUNCTION);

    // Setting P1.4 as LED Pin
    GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN4);
    GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN4);

    // Setting the DCO to use the internal resistor. DCO will be at 16.384MHz
    // ACLK is at 32kHz
    CS_setupDCO(CS_INTERNAL_RESISTOR);

    // Configure and enable the SPI peripheral
    EUSCI_A_SPI_initMaster(EUSCI_A0_BASE, &spiMasterConfig);
    EUSCI_A_SPI_enable(EUSCI_A0_BASE);

    // Put the first byte in the transfer buffer
    EUSCI_A_SPI_transmitData(EUSCI_A0_BASE, TXData);

    EUSCI_A_SPI_enableInterrupt(EUSCI_A0_BASE, EUSCI_A_SPI_RECEIVE_INTERRUPT);

    // Go into LPM0 with interrupts enabled
    __bis_SR_register(LPM0_bits | GIE);
}

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=USCI_A0_VECTOR
__interrupt
#elif defined(__GNUC__)
__attribute__((interrupt(USCI_A0_VECTOR)))
#endif
void USCI_A0_ISR(void) {
    switch(__even_in_range(UCA0IV, USCI_SPI_UCTXIFG))
    {
    case USCI_NONE: break;
    case USCI_SPI_UCRXIFG:
        // Read what slave sent. Should be same as transferred. If there
        // is a mismatch turn on the LED
        RXData = EUSCI_A_SPI_receiveData(EUSCI_A0_BASE);
        if(RXData != TXData)
        {
            GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN4);
            while(1)
            {
                ;
            }
        }
        EUSCI_A_SPI_transmitData(EUSCI_A0_BASE, ++TXData);
        break;
    case USCI_SPI_UCTXIFG:
        break;
    default: break;
    }
}
