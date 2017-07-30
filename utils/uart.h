/**
 * \file uart.h
 * \author Chris Karaplis
 * \brief UART driver API
 *
 * Copyright (c) 2015, simplyembedded.org
 *
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without 
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, 
 *    this list of conditions and the following disclaimer.
 * 
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE 
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE 
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF 
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * Modified by Operational Technology Solutions M2M Pty. Ltd. to support multiple
 * UART instances and use MSP430 HAL driver.
 *
 */

#include <stdint.h>
#include "ring_buffer.h"

typedef struct
{
    uint32_t baud;
    rb_attr_t recv_buffer;
}
uart_config_t;

typedef enum
{
  UCA0 = 0,
  UCA1,
  UCA2
} uart_id_t;

/**
 * \brief Initialize the UART peripheral
 * \param[in] config - the UART configuration
 * \return 0 on success, -1 otherwise
 */
int uart_init(uart_id_t id, uart_config_t *config);

/**
 * \brief Read a character from UART
 * \param[in] c - the character to read
 * \return 0 on success, -1 if nothing was read
 */
int uart_getchar(uart_id_t id, uint8_t *c);

/**
 * \brief Write a character to UART
 * \param[in] c - the character to write
 * \return 0 on sucess, -1 otherwise
 */
int uart_putchar(uart_id_t id, uint8_t c);

/**
 * \brief Write a string to UART
 * \return 0 on sucesss, -1 otherwise
 */
int uart_puts(uart_id_t id, const char *str);

int uart_put(uart_id_t id, const uint8_t *data, uint16_t len);
