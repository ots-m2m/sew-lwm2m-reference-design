/*
 * slip_sio.c
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 *
 * This implementation is limited to supporting a single SLIP interface.
 *
 */

#include "slip_sio.h"
#include "uart.h"
#include <assert.h>
#include <stdbool.h>

#include "log.h"

#define BAUD_RATE 115200
#define SLIP_UART UCA1

#define SIO_FD(u) (sio_fd_t)u
#define FD_TO_UART(f) (uart_id_t) f

static uint8_t recv_buffer[512];
static bool wait = false;

static uart_config_t slip_uart = {
  .baud = BAUD_RATE,
  .recv_buffer = { .s_elem = 1, .n_elem = sizeof(recv_buffer), .buffer = recv_buffer }
};

/**
 * Opens a serial device for communication.
 *
 * @param devnum device number
 * @return handle to serial device if successful, NULL otherwise
 */
sio_fd_t sio_open(u8_t devnum)
{
  assert(uart_init(SLIP_UART, &slip_uart) != -1);
  return SIO_FD(SLIP_UART);
}


/**
 * Sends a single character to the serial device.
 *
 * @param c character to send
 * @param fd serial device handle
 *
 * @note This function will block until the character can be sent.
 */
void sio_send(u8_t c, sio_fd_t fd)
{
  uart_putchar(FD_TO_UART(fd), c);
}


/**
 * Receives a single character from the serial device.
 *
 * @param fd serial device handle
 *
 * @note This function will block until a character is received.
 */
u8_t sio_recv(sio_fd_t fd)
{
  uint8_t c;
  while (uart_getchar(FD_TO_UART(fd), &c) == -1){}
  return c;
}


/**
 * Reads from the serial device.
 *
 * @param fd serial device handle
 * @param data pointer to data buffer for receiving
 * @param len maximum length (in bytes) of data to receive
 * @return number of bytes actually received - may be 0 if aborted by sio_read_abort
 *
 * @note This function will block until data can be received. The blocking
 * can be cancelled by calling sio_read_abort().
 */
u32_t sio_read(sio_fd_t fd, u8_t *data, u32_t len)
{
  uint8_t c;
  unsigned int i = 0;
  wait = true;
  while (wait && (i < len))
  {
    if (uart_getchar(FD_TO_UART(fd), &c) != -1)
    {
      data[i++] = c;
    }
  }
  return i;
}


/**
 * Tries to read from the serial device. Same as sio_read but returns
 * immediately if no data is available and never blocks.
 *
 * @param fd serial device handle
 * @param data pointer to data buffer for receiving
 * @param len maximum length (in bytes) of data to receive
 * @return number of bytes actually received
 */
#include <stdio.h>
#include "log.h"
u32_t sio_tryread(sio_fd_t fd, u8_t *data, u32_t len)
{
  //TODO: this function only works with a len of one which is OK
  //in this version of slipif.c
  assert(len == 1);

  uint8_t c;
  if (uart_getchar(FD_TO_UART(fd), &c) == 0)
  {
    *data = c;
    return 1;
  }
  return 0;
}


/**
 * Writes to the serial device.
 *
 * @param fd serial device handle
 * @param data pointer to data to send
 * @param len length (in bytes) of data to send
 * @return number of bytes actually sent
 *
 * @note This function will block until all data can be sent.
 */
u32_t sio_write(sio_fd_t fd, u8_t *data, u32_t len)
{
  return uart_put(FD_TO_UART(fd), data, len) == 0 ? len : 0;
}


/**
 * Aborts a blocking sio_read() call.
 *
 * @param fd serial device handle
 */
void sio_read_abort(sio_fd_t fd)
{
  wait = false;
}

void sio_close(sio_fd_t fd)
{

}
