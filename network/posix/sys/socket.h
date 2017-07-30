/**
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef SYS_SOCKET_H
#define SYS_SOCKET_H

#include <stdint.h>
#include <sys/time.h>
#include <arpa/inet.h>

typedef uint32_t socklen_t;
typedef int16_t sock_result_t;
typedef int16_t sock_handle_t;

struct sockaddr_in
{
  uint8_t           sin_len;
  uint8_t           sin_family;
  uint16_t          sin_port;
  struct in_addr    sin_addr;
  char              sin_zero[8];
};

struct sockaddr
{
  uint8_t     sa_len;
  uint8_t     sa_family;
  char        sa_data[14];
};

struct sockaddr_storage
{
  uint8_t     s2_len;
  uint8_t     ss_family;
  char        s2_data1[2];
  uint32_t    s2_data2[3];
};


#define SOCK_DGRAM      2
#define AF_INET         2

#define IPPROTO_IP      0
#define IPPROTO_ICMP    1
#define IPPROTO_TCP     6
#define IPPROTO_UDP     17
#define IPPROTO_UDPLITE 136
#define IPPROTO_RAW     255

#define slip_socket_bind           bind
#define slip_socket_close          closesocket
#define slip_socket_receivefrom    recvfrom
#define slip_socket_sendto         sendto
#define slip_socket_create         socket

sock_handle_t slip_socket_create(uint8_t family, uint8_t type, uint8_t protocol, uint16_t port);
sock_result_t slip_socket_close(sock_handle_t sock);
sock_result_t slip_socket_bind(sock_handle_t sock, uint16_t port);
sock_result_t slip_socket_receivefrom(sock_handle_t sock, void* buffer, socklen_t len, uint32_t flags, struct sockaddr* addr, socklen_t* addrsize);
sock_result_t slip_socket_sendto(sock_handle_t sock, const void* buffer, socklen_t len, uint32_t flags, struct sockaddr* addr, socklen_t addr_size);

#endif
