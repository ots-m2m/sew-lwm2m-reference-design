/*
 * slip_socket.c
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 */

#include <sys/socket.h>
#include "lwip/udp.h"
#include "lwip/timeouts.h"
#include "lwip/debug.h"
#include "lwip/mem.h"

#include <stdint.h>
#include <string.h>

#include "log.h"

#define MAX_QUEUED_PACKETS 5

struct slip_message {
  struct pbuf *p;
  uint32_t addr;
  uint16_t port;
};

struct slip_state {
  struct udp_pcb *upcb;
  struct slip_message slip_recv[MAX_QUEUED_PACKETS];
  struct slip_state* next;
};

static struct slip_state *ss_list = NULL;
static struct slip_state *ss_list_last = NULL;

static void slip_recv(void *arg, struct udp_pcb *upcb, struct pbuf *p, const ip_addr_t *addr, uint16_t port)
{
  LWIP_UNUSED_ARG(arg);
  struct slip_state *ss = NULL;
  for(ss = ss_list; ss != NULL; ss=ss->next)
  {
    if(ss->upcb == upcb)
    {
      break;
    }
  }

  if(!ss)
    pbuf_free(p);

  int p_i = -1;
  for (p_i = 0; p_i < MAX_QUEUED_PACKETS; p_i++)
  {
    if (ss->slip_recv[p_i].p == 0) break;
  }

  if (p_i == -1)
  {
    // Release any packets that failed to deliver
    pbuf_free(p);
    log_err("SLIP recv queue was full!! Data has been thrown away!!!");
  }
  else
  {
    ss->slip_recv[p_i].p = p;
    ss->slip_recv[p_i].addr = addr->addr;
    ss->slip_recv[p_i].port = port;
  }
}

sock_handle_t slip_socket_create(uint8_t family, uint8_t type, uint8_t protocol, uint16_t port)
{
  struct udp_pcb *pcb = udp_new_ip_type(IPADDR_TYPE_ANY);
  struct slip_state *ss = NULL;
  if(pcb)
  {
    // Create new slip_state
    ss = mem_malloc(sizeof(struct slip_state));
    ss->upcb = pcb;
    ss->next = NULL;
    memset(&ss->slip_recv, 0, sizeof(ss->slip_recv));

    // Attach it to ss_list
    if(!ss_list)ss_list = ss;
    if(ss_list_last)ss_list_last->next = ss;
    ss_list_last = ss;

    udp_recv(pcb, slip_recv, NULL);
  }
  else
  {
    return (sock_handle_t) 0;
  }
  return (sock_handle_t)ss;
}

sock_result_t slip_socket_close(sock_handle_t sock)
{
  if(!sock) return -1;

  struct slip_state *ss = (struct slip_state *)sock;

  // Remove it from ss list
  if(ss_list == ss)
  {
    if(ss_list_last == ss)ss_list_last = NULL;
    ss_list = ss->next;
  }
  else
  {
    for(struct slip_state *p = ss_list; p != NULL; p=p->next)
    {
      if(p->next == ss)
      {
        if(p->next == ss_list_last)ss_list_last = p;
        p->next = ss->next;
        break;
      }
    }
  }

  // Release resources
  udp_remove(ss->upcb);
  mem_free(ss);
  return 0;
}

sock_result_t slip_socket_bind(sock_handle_t sock, uint16_t port)
{
  if(!sock) return -1;

  return udp_bind((struct udp_pcb *)((struct slip_state *)sock)->upcb, IP_ANY_TYPE, port);
}

sock_result_t slip_socket_receivefrom(sock_handle_t sock, void* buffer, socklen_t len, uint32_t flags, struct sockaddr* addr, socklen_t* addrsize)
{
  if (!sock) return -1;

  struct slip_state *ss = (struct slip_state *)sock;

  int p_i = -1;
  for (p_i = 0; p_i < MAX_QUEUED_PACKETS; p_i++)
  {
    if (ss->slip_recv[p_i].p != 0)
    {
      int res = 0;
      // Copy all data to output buffer and then release pbuf
      uint8_t *p_buffer = buffer;
      for(struct pbuf *p = ss->slip_recv[p_i].p; p != NULL; p = p->next)
      {
        socklen_t to_copy = (len < p->len)?len:p->len;
        memcpy(p_buffer, p->payload, to_copy);

        res += to_copy;
        p_buffer += to_copy;
        len -= to_copy;
      }
      pbuf_free(ss->slip_recv[p_i].p);
      ss->slip_recv[p_i].p = 0;

      // Try to fill address info
      if (res >= 0)
      {
        // Added '\0' to the end in case we are dealing with strings
        if (res < (int16_t)(len - 1))
        {
          *p_buffer = '\0';
        }

        if(addr)
        {
          uint32_t ipv4 = ss->slip_recv[p_i].addr;
          uint16_t port = ss->slip_recv[p_i].port;

          if (addrsize)
          {
            *addrsize = sizeof(struct sockaddr_in);
          }

          struct sockaddr_in *inaddr = (struct sockaddr_in *)addr;
          inaddr->sin_addr.s_addr = ipv4;
          inaddr->sin_port = htons(port);
        }
        return res; //process the packet.
      }
    }
  }
  return 0; //no packet available
}

sock_result_t slip_socket_sendto(sock_handle_t sock, const void* buffer, socklen_t len, uint32_t flags, struct sockaddr* addr, socklen_t addr_size)
{
  if(!sock) return -1;

  struct pbuf send_buf = {
      .next = NULL,
      .payload = (void *)buffer,
      .tot_len = len,
      .len = len,
      .flags = flags,
      .ref = 1,
      .if_idx = 0,
  };

  struct sockaddr_in *inaddr = (struct sockaddr_in *)addr;
  ip_addr_t tmp_addr;
  tmp_addr.addr = inaddr->sin_addr.s_addr;
  if(udp_sendto(((struct slip_state *)sock)->upcb, &send_buf, (const ip_addr_t *)&tmp_addr, ntohs(inaddr->sin_port)) == ERR_OK)
    return len;
  else
    return -1;
}

