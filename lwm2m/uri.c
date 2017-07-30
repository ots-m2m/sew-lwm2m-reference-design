/* uri.c -- helper functions for URI treatment
 *
 * Copyright (C) 2010--2012,2015-2016 Olaf Bergmann <bergmann@tzi.org>
 *
 * This file is part of the CoAP library libcoap. Please see
 * README for terms of use.
 */


# include <assert.h>

#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "uri.h"

/**-----------------------------------------------------------------------------
 *  @file
 *  @defgroup    uri
 *  @ingroup     lwm2m_client
 *  @{
 *------------------------------------------------------------------------------
 */

#define COAP_DEFAULT_PORT 5683
#define   COAP_DEFAULT_SCHEME   "coap"
///**
// * A length-safe version of strchr(). This function returns a pointer
// * to the first occurrence of @p c  in @p s, or @c NULL if not found.
// *
// * @param s   The string to search for @p c.
// * @param len The length of @p s.
// * @param c   The character to search.
// *
// * @return A pointer to the first occurence of @p c, or @c NULL
// * if not found.
// */
//static inline unsigned char *
//strnchr(unsigned char *s, size_t len, unsigned char c) {
//  while (len && *s++ != c)
//    --len;
//
//  return len ? s : NULL;
//}

#define ISEQUAL_CI(a,b) \
  ((a) == (b) || (islower(b) && ((a) == ((b) - 0x20))))

int
coap_split_uri(const unsigned char *str_var, size_t len, coap_uri_t *uri) {
  const unsigned char *p, *q;
  int secure = 0, res = 0;

  if (!str_var || !uri)
    return -1;

  memset(uri, 0, sizeof(coap_uri_t));
  uri->port = COAP_DEFAULT_PORT;

  /* search for scheme */
  p = str_var;
  if (*p == '/') {
    q = p;
    goto path;
  }

  q = (unsigned char *)COAP_DEFAULT_SCHEME;
  while (len && *q && ISEQUAL_CI(*p, *q)) {
    ++p; ++q; --len;
  }

  /* If q does not point to the string end marker '\0', the schema
   * identifier is wrong. */
  if (*q) {
    res = -1;
    goto error;
  }

  /* There might be an additional 's', indicating the secure version: */
  if (len && (secure = *p == 's')) {
    ++p; --len;
  }

  q = (unsigned char *)"://";
  while (len && *q && *p == *q) {
    ++p; ++q; --len;
  }

  if (*q) {
    res = -2;
    goto error;
  }

  /* p points to beginning of Uri-Host */
  q = p;
  if (len && *p == '[') { /* IPv6 address reference */
    ++p;

    while (len && *q != ']') {
      ++q; --len;
    }

    if (!len || *q != ']' || p == q) {
      res = -3;
      goto error;
    }

    COAP_SET_STR(&uri->host, q - p, (unsigned char *)p);
    ++q; --len;
  } else {      /* IPv4 address or FQDN */
    while (len && *q != ':' && *q != '/' && *q != '?') {
      ++q;
      --len;
    }

    if (p == q) {
      res = -3;
      goto error;
    }

    COAP_SET_STR(&uri->host, q - p, (unsigned char *)p);
  }

  /* check for Uri-Port */
  if (len && *q == ':') {
    p = ++q;
    --len;

    while (len && isdigit(*q)) {
      ++q;
      --len;
    }

    if (p < q) {    /* explicit port number given */
      int uri_port = 0;

      while (p < q)
  uri_port = uri_port * 10 + (*p++ - '0');

      /* check if port number is in allowed range */
      if (uri_port > 65535) {
  res = -4;
  goto error;
      }

      uri->port = uri_port;
    }
  }

 path:     /* at this point, p must point to an absolute path */

  if (!len)
    goto end;

  if (*q == '/') {
    p = ++q;
    --len;

    while (len && *q != '?') {
      ++q;
      --len;
    }

    if (p < q) {
      COAP_SET_STR(&uri->path, q - p, (unsigned char *)p);
      p = q;
    }
  }

  /* Uri_Query */
  if (len && *p == '?') {
    ++p;
    --len;
    COAP_SET_STR(&uri->query, len, (unsigned char *)p);
    len = 0;
  }

  end:
  return len ? -1 : 0;

  error:
  return res;
}

