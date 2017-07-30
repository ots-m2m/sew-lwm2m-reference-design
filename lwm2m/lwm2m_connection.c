/*
 * lwm2m_connection.c
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 */
#include "lwm2m_connection.h"


#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netdb.h>
#include <assert.h>

#include "liblwm2m.h"
#include "er-coap-13.h"
#include "log.h"
#include "sys_tick.h"
#include "uri.h"
#include "LWM2M_Security.h"

/**-----------------------------------------------------------------------------
 *  @file
 *  @defgroup    lwm2m_object_base 
 *  @ingroup     lwm2m_client
 *  @{
 *------------------------------------------------------------------------------
 */

#define min(X,Y) ((X) < (Y) ? (X) : (Y))

#define MAX_PACKETS_PER_LOOP 1
#define MAX_PACKET_SIZE REST_MAX_CHUNK_SIZE

#define MAX_DNS_RETRIES 0xffff //3

static uint8_t packet[MAX_PACKET_SIZE];

typedef struct lwm2m_connection_t
{
  struct lwm2m_connection_t *  next;
  int                     sock;
  char host_name[64];
  unsigned short host_port;
  struct sockaddr_in address;
  socklen_t address_length;
  unsigned long nextMID;
}
lwm2m_connection_t;

lwm2m_connection_t *connection_list = 0;

#define IP(addr, octet) ((char *)&addr)[octet]

static int lwm2m_connection_send(lwm2m_connection_t *connection, uint8_t *buffer, size_t length)
{
  size_t num_sent;
  size_t offset;

  offset = 0;
  while (offset != length)
  {
    num_sent = sendto(connection->sock, buffer + offset, length - offset, 0, (struct sockaddr *)&(connection->address), connection->address_length);
    if (num_sent == -1) return -1;
    offset += num_sent;
  }
  return 0;
}

static uint8_t lwm2m_socket_connection_send(lwm2m_connection_t * connection, uint8_t * buffer, size_t length, void * userdata)
{
  if (connection == NULL)
  {
    log_err("failed sending %lu bytes, missing connection", length);
    return COAP_500_INTERNAL_SERVER_ERROR;
  }

  uint16_t retry = 0;
  do
  {
    int res = lwm2m_connection_send(connection, buffer, length);
    if (res == 0)
    {
      log_debug("sent %lu bytes (retry %d)", length, retry);
      return COAP_NO_ERROR;
    }
    log_err("failed sending %lu bytes (retry %d)", length, retry);
    msleep(100);
  }
  while (retry++ < 5);
  return COAP_500_INTERNAL_SERVER_ERROR;
}


//
// Called by wakaama when it wants to send a packet.
uint8_t lwm2m_buffer_send(void * session_handle, uint8_t * buffer, size_t length, void * user_data)
{
  lwm2m_connection_t * connection = (lwm2m_connection_t* ) session_handle;
  assert(connection != 0);
  return lwm2m_socket_connection_send(connection, buffer, length, user_data);
}

//
// Called by wakaama to identify a connection
bool lwm2m_session_is_equal(void * session1, void * session2, void * user_data)
{
  return session1 == session2;
}

static int lwm2m_connection_create_socket(int port)
{
  //FIXME: By passing 5683 as the source port for the UDP
  //socket we work around NAT firewalls which timeout the UDP
  //session. this should be set to 0 when not running through
  //a NAT firewall.
  int s =  socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP, 5683);
  if (s >= 0) bind(s, port);
  return s;
}

lwm2m_connection_t * lwm2m_connection_new_incoming(
  lwm2m_connection_t * connList,
  int sock,
  struct sockaddr * addr,
  size_t addrLen)
{
  lwm2m_connection_t *connection;

  connection = (lwm2m_connection_t *) lwm2m_malloc(sizeof(lwm2m_connection_t));
  if (connection != 0)
  {
    connection->sock = sock;
    memcpy(&(connection->address), addr, addrLen);
    connection->address_length = addrLen;
    connection->next = connList;
  }
  return connection;
}

static lwm2m_connection_t *lwm2m_connection_create(
  lwm2m_connection_t * connList,
  int sock,
  coap_uri_t *uri)
{
  struct sockaddr_in  s_addr;

  char host_str[128];
  memset(host_str, 0, sizeof(host_str));
  memcpy(host_str, uri->host.s, min(uri->host.length, sizeof(host_str) -1));

  //not an ip, let's try to lookup the hostname

  int retry = 0;
  struct hostent *he = 0;
  for (retry = 0; retry < MAX_DNS_RETRIES; retry++)
  {
    he = gethostbyname(host_str);

    if (he != 0)
    {
      break;
    }

    log_debug("host not found (try %d)", retry);
  }
  if (retry >= MAX_DNS_RETRIES)
  {
    log_err("Failed to lookup host name '%s'", host_str);
    return 0;
  }

  memset(&s_addr, 0, sizeof(s_addr));
  s_addr.sin_family = AF_INET;
  s_addr.sin_port = htons(uri->port);
  struct in_addr **addr_list;
  addr_list = (struct in_addr **) he->h_addr_list;
  s_addr.sin_addr.s_addr = addr_list[0]->s_addr;
  memset(&s_addr.sin_zero, 0, sizeof(s_addr.sin_zero));
  log_debug("host found at IP: %d.%d.%d.%d", LOG_IP(ntohl(addr_list[0]->s_addr)));

  return lwm2m_connection_new_incoming(connList, sock, (struct sockaddr *)&s_addr, sizeof(struct sockaddr_in));
}

lwm2m_connection_t *lwm2m_connect_server_uri(coap_uri_t *uri, int port)
{
  lwm2m_connection_t * connection = 0;
  char tmp[128];
  memset(tmp, 0, sizeof(tmp));
  memcpy(tmp,  uri->host.s, min(sizeof(tmp) - 1, uri->host.length));
  log_info("Connecting to %s", tmp);

  int sock = lwm2m_connection_create_socket(port);

  if (sock < 0)
  {
    log_err("open socket failed: %d", sock);
    return 0;
  }
  log_debug("socket %d created", sock);

  connection = lwm2m_connection_create(connection_list, sock, uri);

  if (connection == 0)
  {
    log_err("Connection creation failed.");
    closesocket(sock);
  }
  else
  {
    connection_list = connection;
  }
  return connection;
}

//
// Called by wakaama when it wants to create a new connection
void *lwm2m_connect_server(uint16_t security_instance_id, void * data)
{
  const char * uri;
  lwm2m_connection_t * connection = 0;

  LWM2M_Security_LWM2M_Server_URI_get(security_instance_id, &uri);

  coap_uri_t coap_uri;
  if (coap_split_uri((unsigned char *)uri, strlen(uri), &coap_uri) == 0)
  {
    connection = lwm2m_connect_server_uri(&coap_uri, coap_uri.port);
  }

  return (void *)connection;
}

//
// Called by wakaama when it wants to close a connection
void lwm2m_close_connection(void *session_handle, void *data)
{
  lwm2m_connection_t * targetP = (lwm2m_connection_t *)session_handle;

  if (targetP == connection_list)
  {
    connection_list = targetP->next;
    log_debug("closing socket %d", targetP->sock);

    closesocket(targetP->sock);
    lwm2m_free(targetP);
  }
  else
  {
    lwm2m_connection_t * parentP;

    parentP = connection_list;
    while (parentP != NULL && parentP->next != targetP)
    {
      parentP = parentP->next;
    }
    if (parentP != NULL)
    {
      parentP->next = targetP->next;
      closesocket(targetP->sock);
      lwm2m_free(targetP);
    }
  }
}
static bool lwm2m_connection_check_peer(lwm2m_connection_t *connection, struct sockaddr_in *peer, socklen_t address_length)
{
  return ((connection->address_length == address_length)   &&
         (connection->address.sin_port == peer->sin_port)  &&
         (connection->address.sin_addr.s_addr == peer->sin_addr.s_addr));
}

static void lwm2m_connection_receive(lwm2m_context_t *lwm2m_handle, lwm2m_connection_t *connection, unsigned char *buffer, int buffer_length)
{
  int packet_count = 0; //prevent packets arriving starving the system from processing other things.

  struct sockaddr_in addr;
  memset(&addr, 0, sizeof(addr));
  addr.sin_family = AF_INET;

  socklen_t addrLen;
  addrLen = sizeof(addr);

  int num_bytes = 0;
  do
  {
    if (connection->sock >= 0)
    {
      num_bytes = recvfrom(connection->sock, buffer, buffer_length, 0, (struct sockaddr *)&addr, &addrLen);

      if (0 > num_bytes)
      {
        log_err("receive failed %d", num_bytes);
      }
      else if (0 < num_bytes)
      {
#ifndef NDEBUG
        int hport = ntohs(addr.sin_port);
        uint32_t ip = ntohl(addr.sin_addr.s_addr);
        log_debug("got %d bytes from %.3d.%.3d.%.3d.%.3d:%d", num_bytes, IP(ip, 3), IP(ip, 2),IP(ip, 1), IP(ip, 0), hport);
#endif
        if (lwm2m_connection_check_peer(connection, &addr, addrLen))
        {
          lwm2m_handle_packet(lwm2m_handle, buffer, num_bytes, connection);
        }
        else
        {
          log_info("packet from unknown peer");
        }
        packet_count++;
      }
    }
    else
    {
      log_err("socket not open");
    }
  }
  while ((num_bytes > 0) && (packet_count < MAX_PACKETS_PER_LOOP));
}

//
// call periodically to see if there is a new packet to process
void lwm2m_connection_poll(lwm2m_context_t *lwm2m_handle)
{
  lwm2m_connection_t * connection = connection_list;
  do
  {
    if (connection != 0)
    {
      lwm2m_connection_receive(lwm2m_handle, connection, packet, sizeof(packet));
      connection = connection->next;
    }
  }
  while (connection != 0);
}


