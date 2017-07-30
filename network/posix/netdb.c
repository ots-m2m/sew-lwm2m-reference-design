/*
 * netdb.c
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 */


#include <netdb.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <string.h>
#include "lwip/err.h"
#include "lwip/dns.h"
#include "log.h"
#include "sys_tick.h"
#include "slip_network.h"

static struct hostent he;
static in_addr *addrs[1];
static in_addr addr;
static err_t result;
static ip_addr_t address;

//called when a DNS entry has been found
void network_dns_found(const char *name, const ip_addr_t *ipaddr, void *callback_arg)
{
  if (ipaddr != 0)
  {
    result = ERR_OK;
    memcpy(&address, ipaddr, sizeof(ip_addr_t));
  }
  else
  {
    result = ERR_TIMEOUT;
  }
}

struct hostent *gethostbyname(const char* hostname)
{
  static bool init = false;

  if (!init)
  {
    init = true;
    ip_addr_t ns1;
    ip_addr_t ns2;

    //default to google DNS
    IP4_ADDR(&ns1,8,8,8,8);
    IP4_ADDR(&ns2,8,8,4,4);

    dns_setserver(0, &ns1);
    dns_setserver(1, &ns2);
  }

  result = dns_gethostbyname(hostname, &address, network_dns_found, 0);

  int to = 5000;//5s
  if (result == ERR_INPROGRESS)
  {
    do
    {
      msleep(1);
      slip_network_tick();
      if ((to % 1000) == 0) dns_tmr();
    }
    while ((result == ERR_INPROGRESS) && (--to > 0));
  }

  if (result == ERR_OK)
  {
    he.h_name = (char *)hostname;
    addr.s_addr = address.addr;
    addrs[0] = &addr;
    he.h_addr_list = (char **)&addrs;
    return &he;
  }
  else
  {
    log_debug("get host by name timeout");
  }

  return 0;
}
