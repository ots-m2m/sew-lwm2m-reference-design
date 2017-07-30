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
//
// rom_driverlib_fr253x_fr263x.h - Macros to facilitate calling functions in 
// ROM for MSP430FR2532, FR2533, FR2632, FR2633
//
// Copyright (c) 2014-2017 Texas Instruments Incorporated.  All rights reserved.
// TI Information - Selective Disclosure
//
//*****************************************************************************
//

#ifndef __ROM_DRIVERLIB_H__
#define __ROM_DRIVERLIB_H__

#if (defined(__TI_COMPILER_VERSION__) && !defined(__LARGE_CODE_MODEL__)) || \
    (defined(__IAR_SYSTEMS_ICC__) && (__CODE_MODEL__==__CODE_MODEL_SMALL__))

//*****************************************************************************
//
// Pointers to the main API tables.
//
//*****************************************************************************
#define ROM_DRIVERLIB_APITABLE         ((uint16_t *)0x67E0)
#define ROM_DRIVERLIB_VERSIONL         (ROM_DRIVERLIB_APITABLE[0])
#define ROM_DRIVERLIB_VERSIONH         (ROM_DRIVERLIB_APITABLE[1])
#define ROM_EUSCIASPITABLE             ((uint16_t *)(ROM_DRIVERLIB_APITABLE[2]))
#define ROM_EUSCIAUARTTABLE            ((uint16_t *)(ROM_DRIVERLIB_APITABLE[3]))
#define ROM_EUSCIBI2CTABLE             ((uint16_t *)(ROM_DRIVERLIB_APITABLE[4]))
#define ROM_EUSCIBSPITABLE             ((uint16_t *)(ROM_DRIVERLIB_APITABLE[5]))

#define ROM_DRIVERLIB_getVersion()     ((uint32_t)ROM_DRIVERLIB_VERSIONH<<16|\
                                            ROM_DRIVERLIB_VERSIONL)

//*****************************************************************************
//
// Macros for calling ROM functions in the EUSCIASPI API.
//
//*****************************************************************************
#if defined(TARGET_IS_MSP430FR2XX_4XX)
#define ROM_EUSCI_A_SPI_disableInterrupt                                      \
        ((void (*)(uint16_t baseAddress,                                      \
                   uint8_t mask))ROM_EUSCIASPITABLE[0])
#endif
#if defined(TARGET_IS_MSP430FR2XX_4XX)
#define ROM_EUSCI_A_SPI_disable                                               \
        ((void (*)(uint16_t baseAddress))ROM_EUSCIASPITABLE[1])
#endif
#if defined(TARGET_IS_MSP430FR2XX_4XX)
#define ROM_EUSCI_A_SPI_isBusy                                                \
        ((uint16_t (*)(uint16_t baseAddress))ROM_EUSCIASPITABLE[2])
#endif
#if defined(TARGET_IS_MSP430FR2XX_4XX)
#define ROM_EUSCI_A_SPI_getReceiveBufferAddress                               \
        ((uint32_t (*)(uint16_t baseAddress))ROM_EUSCIASPITABLE[3])
#endif
#if defined(TARGET_IS_MSP430FR2XX_4XX)
#define ROM_EUSCI_A_SPI_enableInterrupt                                       \
        ((void (*)(uint16_t baseAddress,                                      \
                   uint8_t mask))ROM_EUSCIASPITABLE[4])
#endif
#if defined(TARGET_IS_MSP430FR2XX_4XX)
#define ROM_EUSCI_A_SPI_transmitData                                          \
        ((void (*)(uint16_t baseAddress,                                      \
                   uint8_t transmitData))ROM_EUSCIASPITABLE[5])
#endif
#if defined(TARGET_IS_MSP430FR2XX_4XX)
#define ROM_EUSCI_A_SPI_changeMasterClock                                     \
        ((void (*)(uint16_t baseAddress,                                      \
                   EUSCI_A_SPI_changeMasterClockParam *param))ROM_EUSCIASPITABLE[6])
#endif
#if defined(TARGET_IS_MSP430FR2XX_4XX)
#define ROM_EUSCI_A_SPI_initMaster                                            \
        ((void (*)(uint16_t baseAddress,                                      \
                   EUSCI_A_SPI_initMasterParam *param))ROM_EUSCIASPITABLE[7])
#endif
#if defined(TARGET_IS_MSP430FR2XX_4XX)
#define ROM_EUSCI_A_SPI_changeClockPhasePolarity                              \
        ((void (*)(uint16_t baseAddress,                                      \
                   uint16_t clockPhase,                                       \
                   uint16_t clockPolarity))ROM_EUSCIASPITABLE[8])
#endif
#if defined(TARGET_IS_MSP430FR2XX_4XX)
#define ROM_EUSCI_A_SPI_getInterruptStatus                                    \
        ((uint8_t (*)(uint16_t baseAddress,                                   \
                      uint8_t mask))ROM_EUSCIASPITABLE[9])
#endif
#if defined(TARGET_IS_MSP430FR2XX_4XX)
#define ROM_EUSCI_A_SPI_select4PinFunctionality                               \
        ((void (*)(uint16_t baseAddress,                                      \
                   uint8_t select4PinFunctionality))ROM_EUSCIASPITABLE[10])
#endif
#if defined(TARGET_IS_MSP430FR2XX_4XX)
#define ROM_EUSCI_A_SPI_initSlave                                             \
        ((void (*)(uint16_t baseAddress,                                      \
                   EUSCI_A_SPI_initSlaveParam *param))ROM_EUSCIASPITABLE[11])
#endif
#if defined(TARGET_IS_MSP430FR2XX_4XX)
#define ROM_EUSCI_A_SPI_enable                                                \
        ((void (*)(uint16_t baseAddress))ROM_EUSCIASPITABLE[12])
#endif
#if defined(TARGET_IS_MSP430FR2XX_4XX)
#define ROM_EUSCI_A_SPI_getTransmitBufferAddress                              \
        ((uint32_t (*)(uint16_t baseAddress))ROM_EUSCIASPITABLE[13])
#endif
#if defined(TARGET_IS_MSP430FR2XX_4XX)
#define ROM_EUSCI_A_SPI_clearInterrupt                                        \
        ((void (*)(uint16_t baseAddress,                                      \
                   uint8_t mask))ROM_EUSCIASPITABLE[14])
#endif
#if defined(TARGET_IS_MSP430FR2XX_4XX)
#define ROM_EUSCI_A_SPI_receiveData                                           \
        ((uint8_t (*)(uint16_t baseAddress))ROM_EUSCIASPITABLE[15])
#endif

//*****************************************************************************
//
// Macros for calling ROM functions in the EUSCIAUART API.
//
//*****************************************************************************
#if defined(TARGET_IS_MSP430FR2XX_4XX)
#define ROM_EUSCI_A_UART_setDormant                                           \
        ((void (*)(uint16_t baseAddress))ROM_EUSCIAUARTTABLE[0])
#endif
#if defined(TARGET_IS_MSP430FR2XX_4XX)
#define ROM_EUSCI_A_UART_getTransmitBufferAddress                             \
        ((uint32_t (*)(uint16_t baseAddress))ROM_EUSCIAUARTTABLE[1])
#endif
#if defined(TARGET_IS_MSP430FR2XX_4XX)
#define ROM_EUSCI_A_UART_disableInterrupt                                     \
        ((void (*)(uint16_t baseAddress,                                      \
                   uint8_t mask))ROM_EUSCIAUARTTABLE[2])
#endif
#if defined(TARGET_IS_MSP430FR2XX_4XX)
#define ROM_EUSCI_A_UART_enable                                               \
        ((void (*)(uint16_t baseAddress))ROM_EUSCIAUARTTABLE[3])
#endif
#if defined(TARGET_IS_MSP430FR2XX_4XX)
#define ROM_EUSCI_A_UART_transmitData                                         \
        ((void (*)(uint16_t baseAddress,                                      \
                   uint8_t transmitData))ROM_EUSCIAUARTTABLE[4])
#endif
#if defined(TARGET_IS_MSP430FR2XX_4XX)
#define ROM_EUSCI_A_UART_transmitBreak                                        \
        ((void (*)(uint16_t baseAddress))ROM_EUSCIAUARTTABLE[5])
#endif
#if defined(TARGET_IS_MSP430FR2XX_4XX)
#define ROM_EUSCI_A_UART_resetDormant                                         \
        ((void (*)(uint16_t baseAddress))ROM_EUSCIAUARTTABLE[6])
#endif
#if defined(TARGET_IS_MSP430FR2XX_4XX)
#define ROM_EUSCI_A_UART_init                                                 \
        ((bool (*)(uint16_t baseAddress,                                      \
                   EUSCI_A_UART_initParam *param))ROM_EUSCIAUARTTABLE[7])
#endif
#if defined(TARGET_IS_MSP430FR2XX_4XX)
#define ROM_EUSCI_A_UART_clearInterrupt                                       \
        ((void (*)(uint16_t baseAddress,                                      \
                   uint8_t mask))ROM_EUSCIAUARTTABLE[8])
#endif
#if defined(TARGET_IS_MSP430FR2XX_4XX)
#define ROM_EUSCI_A_UART_transmitAddress                                      \
        ((void (*)(uint16_t baseAddress,                                      \
                   uint8_t transmitAddress))ROM_EUSCIAUARTTABLE[9])
#endif
#if defined(TARGET_IS_MSP430FR2XX_4XX)
#define ROM_EUSCI_A_UART_receiveData                                          \
        ((uint8_t (*)(uint16_t baseAddress))ROM_EUSCIAUARTTABLE[10])
#endif
#if defined(TARGET_IS_MSP430FR2XX_4XX)
#define ROM_EUSCI_A_UART_enableInterrupt                                      \
        ((void (*)(uint16_t baseAddress,                                      \
                   uint8_t mask))ROM_EUSCIAUARTTABLE[11])
#endif
#if defined(TARGET_IS_MSP430FR2XX_4XX)
#define ROM_EUSCI_A_UART_queryStatusFlags                                     \
        ((uint8_t (*)(uint16_t baseAddress,                                   \
                      uint8_t mask))ROM_EUSCIAUARTTABLE[12])
#endif
#if defined(TARGET_IS_MSP430FR2XX_4XX)
#define ROM_EUSCI_A_UART_getReceiveBufferAddress                              \
        ((uint32_t (*)(uint16_t baseAddress))ROM_EUSCIAUARTTABLE[13])
#endif
#if defined(TARGET_IS_MSP430FR2XX_4XX)
#define ROM_EUSCI_A_UART_getInterruptStatus                                   \
        ((uint8_t (*)(uint16_t baseAddress,                                   \
                      uint8_t mask))ROM_EUSCIAUARTTABLE[14])
#endif
#if defined(TARGET_IS_MSP430FR2XX_4XX)
#define ROM_EUSCI_A_UART_selectDeglitchTime                                   \
        ((void (*)(uint16_t baseAddress,                                      \
                   uint16_t deglitchTime))ROM_EUSCIAUARTTABLE[15])
#endif
#if defined(TARGET_IS_MSP430FR2XX_4XX)
#define ROM_EUSCI_A_UART_disable                                              \
        ((void (*)(uint16_t baseAddress))ROM_EUSCIAUARTTABLE[16])
#endif

//*****************************************************************************
//
// Macros for calling ROM functions in the EUSCIBI2C API.
//
//*****************************************************************************
#if defined(TARGET_IS_MSP430FR2XX_4XX)
#define ROM_EUSCI_B_I2C_initSlave                                             \
        ((void (*)(uint16_t baseAddress,                                      \
                   EUSCI_B_I2C_initSlaveParam *param))ROM_EUSCIBI2CTABLE[0])
#endif
#if defined(TARGET_IS_MSP430FR2XX_4XX)
#define ROM_EUSCI_B_I2C_masterSendMultiByteStop                               \
        ((void (*)(uint16_t baseAddress))ROM_EUSCIBI2CTABLE[1])
#endif
#if defined(TARGET_IS_MSP430FR2XX_4XX)
#define ROM_EUSCI_B_I2C_isBusBusy                                             \
        ((uint16_t (*)(uint16_t baseAddress))ROM_EUSCIBI2CTABLE[2])
#endif
#if defined(TARGET_IS_MSP430FR2XX_4XX)
#define ROM_EUSCI_B_I2C_masterReceiveMultiByteNext                            \
        ((uint8_t (*)(uint16_t baseAddress))ROM_EUSCIBI2CTABLE[3])
#endif
#if defined(TARGET_IS_MSP430FR2XX_4XX)
#define ROM_EUSCI_B_I2C_masterSendMultiByteStartWithTimeout                   \
        ((bool (*)(uint16_t baseAddress,                                      \
                   uint8_t txData,                                            \
                   uint32_t timeout))ROM_EUSCIBI2CTABLE[4])
#endif
#if defined(TARGET_IS_MSP430FR2XX_4XX)
#define ROM_EUSCI_B_I2C_masterSendStart                                       \
        ((void (*)(uint16_t baseAddress))ROM_EUSCIBI2CTABLE[5])
#endif
#if defined(TARGET_IS_MSP430FR2XX_4XX)
#define ROM_EUSCI_B_I2C_disableInterrupt                                      \
        ((void (*)(uint16_t baseAddress,                                      \
                   uint16_t mask))ROM_EUSCIBI2CTABLE[6])
#endif
#if defined(TARGET_IS_MSP430FR2XX_4XX)
#define ROM_EUSCI_B_I2C_initMaster                                            \
        ((void (*)(uint16_t baseAddress,                                      \
                   EUSCI_B_I2C_initMasterParam *param))ROM_EUSCIBI2CTABLE[7])
#endif
#if defined(TARGET_IS_MSP430FR2XX_4XX)
#define ROM_EUSCI_B_I2C_clearInterrupt                                        \
        ((void (*)(uint16_t baseAddress,                                      \
                   uint16_t mask))ROM_EUSCIBI2CTABLE[8])
#endif
#if defined(TARGET_IS_MSP430FR2XX_4XX)
#define ROM_EUSCI_B_I2C_getTransmitBufferAddress                              \
        ((uint32_t (*)(uint16_t baseAddress))ROM_EUSCIBI2CTABLE[9])
#endif
#if defined(TARGET_IS_MSP430FR2XX_4XX)
#define ROM_EUSCI_B_I2C_masterReceiveMultiByteFinishWithTimeout               \
        ((bool (*)(uint16_t baseAddress,                                      \
                   uint8_t *txData,                                           \
                   uint32_t timeout))ROM_EUSCIBI2CTABLE[10])
#endif
#if defined(TARGET_IS_MSP430FR2XX_4XX)
#define ROM_EUSCI_B_I2C_masterReceiveSingleByte                               \
        ((uint8_t (*)(uint16_t baseAddress))ROM_EUSCIBI2CTABLE[11])
#endif
#if defined(TARGET_IS_MSP430FR2XX_4XX)
#define ROM_EUSCI_B_I2C_setSlaveAddress                                       \
        ((void (*)(uint16_t baseAddress,                                      \
                   uint8_t slaveAddress))ROM_EUSCIBI2CTABLE[12])
#endif
#if defined(TARGET_IS_MSP430FR2XX_4XX)
#define ROM_EUSCI_B_I2C_slaveGetData                                          \
        ((uint8_t (*)(uint16_t baseAddress))ROM_EUSCIBI2CTABLE[13])
#endif
#if defined(TARGET_IS_MSP430FR2XX_4XX)
#define ROM_EUSCI_B_I2C_masterSendSingleByte                                  \
        ((void (*)(uint16_t baseAddress,                                      \
                   uint8_t txData))ROM_EUSCIBI2CTABLE[14])
#endif
#if defined(TARGET_IS_MSP430FR2XX_4XX)
#define ROM_EUSCI_B_I2C_masterSendMultiByteFinish                             \
        ((void (*)(uint16_t baseAddress,                                      \
                   uint8_t txData))ROM_EUSCIBI2CTABLE[15])
#endif
#if defined(TARGET_IS_MSP430FR2XX_4XX)
#define ROM_EUSCI_B_I2C_setMode                                               \
        ((void (*)(uint16_t baseAddress,                                      \
                   uint8_t mode))ROM_EUSCIBI2CTABLE[16])
#endif
#if defined(TARGET_IS_MSP430FR2XX_4XX)
#define ROM_EUSCI_B_I2C_enable                                                \
        ((void (*)(uint16_t baseAddress))ROM_EUSCIBI2CTABLE[17])
#endif
#if defined(TARGET_IS_MSP430FR2XX_4XX)
#define ROM_EUSCI_B_I2C_masterSendMultiByteNext                               \
        ((void (*)(uint16_t baseAddress,                                      \
                   uint8_t txData))ROM_EUSCIBI2CTABLE[18])
#endif
#if defined(TARGET_IS_MSP430FR2XX_4XX)
#define ROM_EUSCI_B_I2C_getMode                                               \
        ((uint8_t (*)(uint16_t baseAddress))ROM_EUSCIBI2CTABLE[19])
#endif
#if defined(TARGET_IS_MSP430FR2XX_4XX)
#define ROM_EUSCI_B_I2C_masterReceiveSingle                                   \
        ((uint8_t (*)(uint16_t baseAddress))ROM_EUSCIBI2CTABLE[20])
#endif
#if defined(TARGET_IS_MSP430FR2XX_4XX)
#define ROM_EUSCI_B_I2C_disable                                               \
        ((void (*)(uint16_t baseAddress))ROM_EUSCIBI2CTABLE[21])
#endif
#if defined(TARGET_IS_MSP430FR2XX_4XX)
#define ROM_EUSCI_B_I2C_enableInterrupt                                       \
        ((void (*)(uint16_t baseAddress,                                      \
                   uint16_t mask))ROM_EUSCIBI2CTABLE[22])
#endif
#if defined(TARGET_IS_MSP430FR2XX_4XX)
#define ROM_EUSCI_B_I2C_masterSendMultiByteNextWithTimeout                    \
        ((bool (*)(uint16_t baseAddress,                                      \
                   uint8_t txData,                                            \
                   uint32_t timeout))ROM_EUSCIBI2CTABLE[23])
#endif
#if defined(TARGET_IS_MSP430FR2XX_4XX)
#define ROM_EUSCI_B_I2C_masterSendSingleByteWithTimeout                       \
        ((bool (*)(uint16_t baseAddress,                                      \
                   uint8_t txData,                                            \
                   uint32_t timeout))ROM_EUSCIBI2CTABLE[24])
#endif
#if defined(TARGET_IS_MSP430FR2XX_4XX)
#define ROM_EUSCI_B_I2C_slavePutData                                          \
        ((void (*)(uint16_t baseAddress,                                      \
                   uint8_t transmitData))ROM_EUSCIBI2CTABLE[25])
#endif
#if defined(TARGET_IS_MSP430FR2XX_4XX)
#define ROM_EUSCI_B_I2C_masterReceiveStart                                    \
        ((void (*)(uint16_t baseAddress))ROM_EUSCIBI2CTABLE[26])
#endif
#if defined(TARGET_IS_MSP430FR2XX_4XX)
#define ROM_EUSCI_B_I2C_getReceiveBufferAddress                               \
        ((uint32_t (*)(uint16_t baseAddress))ROM_EUSCIBI2CTABLE[27])
#endif
#if defined(TARGET_IS_MSP430FR2XX_4XX)
#define ROM_EUSCI_B_I2C_disableMultiMasterMode                                \
        ((void (*)(uint16_t baseAddress))ROM_EUSCIBI2CTABLE[28])
#endif
#if defined(TARGET_IS_MSP430FR2XX_4XX)
#define ROM_EUSCI_B_I2C_masterReceiveMultiByteFinish                          \
        ((uint8_t (*)(uint16_t baseAddress))ROM_EUSCIBI2CTABLE[29])
#endif
#if defined(TARGET_IS_MSP430FR2XX_4XX)
#define ROM_EUSCI_B_I2C_masterIsStartSent                                     \
        ((uint16_t (*)(uint16_t baseAddress))ROM_EUSCIBI2CTABLE[30])
#endif
#if defined(TARGET_IS_MSP430FR2XX_4XX)
#define ROM_EUSCI_B_I2C_enableMultiMasterMode                                 \
        ((void (*)(uint16_t baseAddress))ROM_EUSCIBI2CTABLE[31])
#endif
#if defined(TARGET_IS_MSP430FR2XX_4XX)
#define ROM_EUSCI_B_I2C_masterSendMultiByteStart                              \
        ((void (*)(uint16_t baseAddress,                                      \
                   uint8_t txData))ROM_EUSCIBI2CTABLE[32])
#endif
#if defined(TARGET_IS_MSP430FR2XX_4XX)
#define ROM_EUSCI_B_I2C_masterSendMultiByteStopWithTimeout                    \
        ((bool (*)(uint16_t baseAddress,                                      \
                   uint32_t timeout))ROM_EUSCIBI2CTABLE[33])
#endif
#if defined(TARGET_IS_MSP430FR2XX_4XX)
#define ROM_EUSCI_B_I2C_masterSendMultiByteFinishWithTimeout                  \
        ((bool (*)(uint16_t baseAddress,                                      \
                   uint8_t txData,                                            \
                   uint32_t timeout))ROM_EUSCIBI2CTABLE[34])
#endif
#if defined(TARGET_IS_MSP430FR2XX_4XX)
#define ROM_EUSCI_B_I2C_masterIsStopSent                                      \
        ((uint16_t (*)(uint16_t baseAddress))ROM_EUSCIBI2CTABLE[35])
#endif
#if defined(TARGET_IS_MSP430FR2XX_4XX)
#define ROM_EUSCI_B_I2C_masterReceiveMultiByteStop                            \
        ((void (*)(uint16_t baseAddress))ROM_EUSCIBI2CTABLE[36])
#endif
#if defined(TARGET_IS_MSP430FR2XX_4XX)
#define ROM_EUSCI_B_I2C_getInterruptStatus                                    \
        ((uint16_t (*)(uint16_t baseAddress,                                  \
                       uint16_t mask))ROM_EUSCIBI2CTABLE[37])
#endif

//*****************************************************************************
//
// Macros for calling ROM functions in the EUSCIBSPI API.
//
//*****************************************************************************
#if defined(TARGET_IS_MSP430FR2XX_4XX)
#define ROM_EUSCI_B_SPI_enableInterrupt                                       \
        ((void (*)(uint16_t baseAddress,                                      \
                   uint8_t mask))ROM_EUSCIBSPITABLE[0])
#endif
#if defined(TARGET_IS_MSP430FR2XX_4XX)
#define ROM_EUSCI_B_SPI_disable                                               \
        ((void (*)(uint16_t baseAddress))ROM_EUSCIBSPITABLE[1])
#endif
#if defined(TARGET_IS_MSP430FR2XX_4XX)
#define ROM_EUSCI_B_SPI_clearInterrupt                                        \
        ((void (*)(uint16_t baseAddress,                                      \
                   uint8_t mask))ROM_EUSCIBSPITABLE[2])
#endif
#if defined(TARGET_IS_MSP430FR2XX_4XX)
#define ROM_EUSCI_B_SPI_isBusy                                                \
        ((uint16_t (*)(uint16_t baseAddress))ROM_EUSCIBSPITABLE[3])
#endif
#if defined(TARGET_IS_MSP430FR2XX_4XX)
#define ROM_EUSCI_B_SPI_getReceiveBufferAddress                               \
        ((uint32_t (*)(uint16_t baseAddress))ROM_EUSCIBSPITABLE[4])
#endif
#if defined(TARGET_IS_MSP430FR2XX_4XX)
#define ROM_EUSCI_B_SPI_changeMasterClock                                     \
        ((void (*)(uint16_t baseAddress,                                      \
                   EUSCI_B_SPI_changeMasterClockParam *param))ROM_EUSCIBSPITABLE[5])
#endif
#if defined(TARGET_IS_MSP430FR2XX_4XX)
#define ROM_EUSCI_B_SPI_transmitData                                          \
        ((void (*)(uint16_t baseAddress,                                      \
                   uint8_t transmitData))ROM_EUSCIBSPITABLE[6])
#endif
#if defined(TARGET_IS_MSP430FR2XX_4XX)
#define ROM_EUSCI_B_SPI_select4PinFunctionality                               \
        ((void (*)(uint16_t baseAddress,                                      \
                   uint8_t select4PinFunctionality))ROM_EUSCIBSPITABLE[7])
#endif
#if defined(TARGET_IS_MSP430FR2XX_4XX)
#define ROM_EUSCI_B_SPI_initSlave                                             \
        ((void (*)(uint16_t baseAddress,                                      \
                   EUSCI_B_SPI_initSlaveParam *param))ROM_EUSCIBSPITABLE[8])
#endif
#if defined(TARGET_IS_MSP430FR2XX_4XX)
#define ROM_EUSCI_B_SPI_disableInterrupt                                      \
        ((void (*)(uint16_t baseAddress,                                      \
                   uint8_t mask))ROM_EUSCIBSPITABLE[9])
#endif
#if defined(TARGET_IS_MSP430FR2XX_4XX)
#define ROM_EUSCI_B_SPI_getTransmitBufferAddress                              \
        ((uint32_t (*)(uint16_t baseAddress))ROM_EUSCIBSPITABLE[10])
#endif
#if defined(TARGET_IS_MSP430FR2XX_4XX)
#define ROM_EUSCI_B_SPI_changeClockPhasePolarity                              \
        ((void (*)(uint16_t baseAddress,                                      \
                   uint16_t clockPhase,                                       \
                   uint16_t clockPolarity))ROM_EUSCIBSPITABLE[11])
#endif
#if defined(TARGET_IS_MSP430FR2XX_4XX)
#define ROM_EUSCI_B_SPI_receiveData                                           \
        ((uint8_t (*)(uint16_t baseAddress))ROM_EUSCIBSPITABLE[12])
#endif
#if defined(TARGET_IS_MSP430FR2XX_4XX)
#define ROM_EUSCI_B_SPI_getInterruptStatus                                    \
        ((uint8_t (*)(uint16_t baseAddress,                                   \
                      uint8_t mask))ROM_EUSCIBSPITABLE[13])
#endif
#if defined(TARGET_IS_MSP430FR2XX_4XX)
#define ROM_EUSCI_B_SPI_initMaster                                            \
        ((void (*)(uint16_t baseAddress,                                      \
                   EUSCI_B_SPI_initMasterParam *param))ROM_EUSCIBSPITABLE[14])
#endif
#if defined(TARGET_IS_MSP430FR2XX_4XX)
#define ROM_EUSCI_B_SPI_enable                                                \
        ((void (*)(uint16_t baseAddress))ROM_EUSCIBSPITABLE[15])
#endif

#else
#error "Large code model is not supported"
#endif

#endif // __ROM_DRIVERLIB_H__
