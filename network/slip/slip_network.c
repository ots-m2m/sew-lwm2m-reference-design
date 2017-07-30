/*
 * slip_network.c
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 */

#include "slip_network.h"
#include "slip_sio.h"
#include "log.h"
#include "lwip/ip.h"
#include "lwip/init.h"
#include "lwip/netif.h"
#include "netif/slipif.h"
#include <stdbool.h>
#include <string.h>

static bool running = false;
static struct netif slip_netif;
static ip4_addr_t ipaddr, netmask, gateway;

void slip_network_start(void)
{
  running = true;
}

void slip_network_stop(void)
{
  running = false;
  slip_network_tick();
}

void slip_network_initialise(void)
{
  IP4_ADDR(&gateway, 192,168,5,1);
  IP4_ADDR(&ipaddr, 192,168,5,2);
  IP4_ADDR(&netmask, 255,255,255,0);
  memset(&slip_netif, 0, sizeof(slip_netif));
}

void slip_network_setdefault(void)
{
  netif_set_default(&slip_netif);
}

bool slip_get_network_running(void)
{
  return running;
}

ip4_addr_t slip_get_network_addr(void)
{
  return ipaddr;
}

//FIXME: this is a hack until the slipif includes a cleanup function
struct slipif_priv
{
  sio_fd_t sd;
  /* q is the whole pbuf chain for a packet, p is the current pbuf in the chain */
  struct pbuf *p, *q;
  u8_t state;
  u16_t i, recved;
#if SLIP_RX_FROM_ISR
  struct pbuf *rxpackets;
#endif
};

void slip_network_tick(void)
{
  static bool last_running = false;
  static bool lwip_intiailsied = false;
  if ((!last_running) && (running))
  {//started
    if (!lwip_intiailsied) lwip_init();
    lwip_intiailsied = true;

    if (netif_add(&slip_netif, &ipaddr, &netmask, &gateway, NULL, slipif_init, ip_input) != 0)
    {
      slip_network_setdefault();
      netif_set_link_up(&slip_netif);
      netif_set_up(&slip_netif);
      log_debug("slip interface initialised");
    }
    else
    {
      log_err("failed to add slip network interface");
    }
  }
  else if ((last_running) && (!running))
  {//stopped
    netif_set_down(&slip_netif);
    netif_set_link_down(&slip_netif);
    netif_remove(&slip_netif);

    //cleanup the slip interface slipif_init allocates a block and sets the state
    //variable to point to it.
    struct slipif_priv *priv = slip_netif.state;
    sio_close(priv->sd);
    mem_free(slip_netif.state);

    log_debug("slip interface stopped");
  }
  else if (running)
  {
    //process packets.
    slipif_poll(&slip_netif);
  }
  last_running = running;
}
