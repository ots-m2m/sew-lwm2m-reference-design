/**
 * \file uart.c
 * \author Chris Karaplis
 * \brief MSP430 hardware UART driver
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
 *
 */

#include "uart.h"
#include "ring_buffer.h"
#include "driverlib.h"

#include <stdint.h>
#include <stddef.h>
#include <msp430.h>
#include <assert.h>

#define ARRAY_SIZE(array) (sizeof(array)/sizeof(array[0]))

struct baud_value
{
  uint32_t baud;
  uint16_t clock_prescalar;
  uint16_t first_mod_reg;
  uint16_t second_mod_reg;
  uint8_t clock_source;
  uint8_t over_sampling;
};

struct uart_port_t
{
  uint16_t base_address;
  uint8_t port;
  uint16_t pins;
};

static const struct uart_port_t uart_ports[] = {
  { USCI_A0_BASE, GPIO_PORT_P2, GPIO_PIN4 + GPIO_PIN5 },
  { USCI_A1_BASE, GPIO_PORT_P8, GPIO_PIN2 + GPIO_PIN3 },
  { USCI_A2_BASE, GPIO_PORT_P3, GPIO_PIN4 + GPIO_PIN5 }
};

/* Table of baud rate register values from
 *
 * http://software-dl.ti.com/msp430/msp430_public_sw/mcu/msp430/MSP430BaudRateConverter/index.html
 *
 * Assumes 20MHz SMCLK, these parameters need to be changed if other clock frequencies
 * are used.
 *
 */
static const struct baud_value baud_values[] = {
  { 9600,    3,   0, 3, USCI_A_UART_CLOCKSOURCE_ACLK, USCI_A_UART_LOW_FREQUENCY_BAUDRATE_GENERATION },
  { 19200,   1,   0, 6, USCI_A_UART_CLOCKSOURCE_ACLK, USCI_A_UART_LOW_FREQUENCY_BAUDRATE_GENERATION },
  { 115200,  10, 14, 0, USCI_A_UART_CLOCKSOURCE_SMCLK, USCI_A_UART_OVERSAMPLING_BAUDRATE_GENERATION },
  { 234000,  5,   7, 0, USCI_A_UART_CLOCKSOURCE_SMCLK, USCI_A_UART_OVERSAMPLING_BAUDRATE_GENERATION }
};

/* RX ring buffer */
static rbd_t ring_buffers[3] = { 0 };

/**
 * \brief Initialize the UART peripheral
 * \param[in] id - the id of the uart, must be one of  UCA0 UCA1, UCA2
 * \param[in] config - the UART configuration. The ring_buffer in the supplied configuration
 * must remain in scope while the UART is in use.
 * \return 0 on success, -1 otherwise
 */
int uart_init(uart_id_t id, uart_config_t *config)
{
  assert(id <= UCA2);

  //uart only supports single byte element ring buffer.
  assert(config->recv_buffer.s_elem == 1);

  int status = -1;

  size_t i;

  /* Find the settings from the baud rate table */
  for (i = 0; i < ARRAY_SIZE(baud_values); i++)
  {
    if (baud_values[i].baud == config->baud)
    {
        break;
    }
  }

  if (i < ARRAY_SIZE(baud_values))
  {
    /* Initialize the ring buffer */
    if (ring_buffer_init(&ring_buffers[id], &config->recv_buffer) == 0)
    {
      GPIO_setAsPeripheralModuleFunctionInputPin(uart_ports[id].port, uart_ports[id].pins);

      USCI_A_UART_initParam param = {0};
      param.selectClockSource = baud_values[i].clock_source;
      param.clockPrescalar = baud_values[i].clock_prescalar;
      param.firstModReg = baud_values[i].first_mod_reg;
      param.secondModReg = baud_values[i].second_mod_reg;
      param.parity = USCI_A_UART_NO_PARITY;
      param.msborLsbFirst = USCI_A_UART_LSB_FIRST;
      param.numberofStopBits = USCI_A_UART_ONE_STOP_BIT;
      param.uartMode = USCI_A_UART_MODE;
      param.overSampling = baud_values[i].over_sampling;

      if (STATUS_FAIL == USCI_A_UART_init(uart_ports[id].base_address, &param))
      {
        return status;
      }

      //Enable UART module for operation
      USCI_A_UART_enable(uart_ports[id].base_address);

      //Enable Receive Interrupt
      USCI_A_UART_clearInterrupt(uart_ports[id].base_address, USCI_A_UART_RECEIVE_INTERRUPT);
      USCI_A_UART_enableInterrupt(uart_ports[id].base_address, USCI_A_UART_RECEIVE_INTERRUPT);

      status = 0;
    }
  }
  return status;
}

/**
 * \brief Read a character from UART
 * \param[in] c - the character to read
 * \return 0 on success, -1 if nothing was read
 */
int uart_getchar(uart_id_t id, uint8_t *c)
{
  assert(id <= UCA2);

  return ring_buffer_get(ring_buffers[id], c);
}

/**
 * \brief Write a character to UART
 * \param[in] c - the character to write
 * \return 0 on sucess, -1 otherwise
 */
int uart_putchar(uart_id_t id, uint8_t c)
{
  assert(id <= UCA2);
  USCI_A_UART_transmitData(uart_ports[id].base_address, c);
  return 0;
}

/**
 * \brief Write a string to UART
 * \return 0 on success, -1 otherwise
 */
int uart_puts(uart_id_t id, const char *str)
{
  assert(id <= UCA2);

  int status = -1;

  if (str != NULL)
  {
    status = 0;

    while (*str != '\0')
    {
      USCI_A_UART_transmitData(uart_ports[id].base_address, *str);
      str++;
    }
  }
  return status;
}

int uart_put(uart_id_t id, const uint8_t *data, uint16_t len)
{
  assert(id <= UCA2);

  int status = -1;

  if (data != NULL)
  {
    status = 0;

    for (uint16_t i = 0; i < len; i++)
    {
      USCI_A_UART_transmitData(uart_ports[id].base_address, data[i]);
    }
  }
  return status;
}

static void rx_isr(uart_id_t id)
{
  uint8_t c = USCI_A_UART_receiveData(uart_ports[id].base_address);
  ring_buffer_put(ring_buffers[id], &c);
}

__attribute__((interrupt(USCI_A0_VECTOR))) void rx_a0_isr(void)
{
  rx_isr(UCA0);
}

__attribute__((interrupt(USCI_A1_VECTOR))) void rx_a1_isr(void)
{
  rx_isr(UCA1);
}

__attribute__((interrupt(USCI_A2_VECTOR))) void rx_a2_isr(void)
{
  rx_isr(UCA2);
}
