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
//  MSP430FR59x Demo - MPU Write protection violation in the device's boot process.
//  The MPU configuration code is put in the _system_pre_init() for CCS and
//  __low_level_init() for IAR.
//
//  Description: The MPU segment boundaries are defined by:
//  Border 1 = 0x6000 [MPUSEGB1 = 0x0600]
//  Border 2 = 0x8000 [MPUSEGB2 = 0x0800]
//  Segment 1 = 0x4400 - 0x5FFF
//  Segment 2 = 0x6000 - 0x7FFF
//  Segment 3 = 0x8000 - 0x13FFF
//  Segment 2 is write protected. Any write to an address in the segment 2 range
//  causes a PUC.
//
//  ACLK = n/a, MCLK = SMCLK = default DCO
//
//
//           MSP430FR5969
//         ---------------
//     /|\|               |
//      | |               |
//      --|RST            |
//        |               |
//
//******************************************************************************

#include "driverlib.h"

#if defined(__TI_COMPILER_VERSION__)
int _system_pre_init(void)
#elif defined(__IAR_SYSTEMS_ICC__)
int __low_level_init(void)
#elif defined(__GNUC__)
int __attribute__((naked, section(".crt_0000init"),
                   used)) _system_pre_init(void)
#endif
{
    // Stop WDT
    WDT_A_hold(WDT_A_BASE);

    // Configure MPU
    MPU_initThreeSegmentsParam param = {0};
    param.seg1boundary = 0x0600;
    param.seg2boundary = 0x0800;
    param.seg1accmask = MPU_READ | MPU_WRITE | MPU_EXEC;
    param.seg2accmask = MPU_READ;
    param.seg3accmask = MPU_READ | MPU_WRITE | MPU_EXEC;
    MPU_initThreeSegments(MPU_BASE, &param);

    // Disable PUC on violation
    MPU_disablePUCOnViolation(MPU_BASE,MPU_SECOND_SEG);

    // Enable NMI event on violation
    MPU_enableNMIevent(MPU_BASE);

    // enable MPU operation
    MPU_start(MPU_BASE);

    return(1);
}

void main(void)
{
}
