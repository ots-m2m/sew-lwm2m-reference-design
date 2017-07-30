/**

 */

#ifndef NETDB_H
#define	NETDB_H

#include <stdint.h>
#include <stdbool.h>
#include <sys/socket.h>

typedef struct hostent
{
  char  *h_name;      /* Official name of the host. */
  char **h_aliases;   /* A pointer to an array of pointers to alternative host names,
                         terminated by a null pointer. */
  int    h_addrtype;  /* Address type. */
  int    h_length;    /* The length, in bytes, of the address. */
  char **h_addr_list; /* A pointer to an array of pointers to network addresses (in
                           network byte order) for the host, terminated by a null pointer. */
}
hostent;

typedef struct addrinfo
{
  int               ai_flags;      /* Input flags. */
  int               ai_family;     /* Address family of socket. */
  int               ai_socktype;   /* Socket type. */
  int               ai_protocol;   /* Protocol of socket. */
  socklen_t         ai_addrlen;    /* Length of socket address. */
  struct sockaddr  *ai_addr;       /* Socket address of socket. */
  char             *ai_canonname;  /* Canonical name of service location. */
  struct addrinfo  *ai_next;       /* Pointer to next in list. */
}
addrinfo;

struct hostent *gethostbyname(const char* host_name);

#endif	/* NETDB_H */

