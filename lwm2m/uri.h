/*
 * uri.h -- helper functions for URI treatment
 *
 * Copyright (C) 2010-2011,2016 Olaf Bergmann <bergmann@tzi.org>
 *
 * This file is part of the CoAP library libcoap. Please see README for terms
 * of use.
 */

#ifndef _COAP_URI_H_
#define _COAP_URI_H_

typedef struct {
  size_t length;    /* length of string */
  unsigned char *s;   /* string data */
} str;
#define COAP_SET_STR(st,l,v) { (st)->length = (l), (st)->s = (v); }
/**
 * Representation of parsed URI. Components may be filled from a string with
 * coap_split_uri() and can be used as input for option-creation functions.
 */
typedef struct {
  str host;             /**< host part of the URI */
  unsigned short port;  /**< The port in host byte order */
  str path;             /**< Beginning of the first path segment.
                             Use coap_split_path() to create Uri-Path options */
  str query;            /**<  The query part if present */
} coap_uri_t;


/**
 * @defgroup uri_parse URI Parsing Functions
 *
 * CoAP PDUs contain normalized URIs with their path and query split into
 * multiple segments. The functions in this module help splitting strings.
 * @{
 */

/**
 * Parses a given string into URI components. The identified syntactic
 * components are stored in the result parameter @p uri. Optional URI
 * components that are not specified will be set to { 0, 0 }, except for the
 * port which is set to @c COAP_DEFAULT_PORT. This function returns @p 0 if
 * parsing succeeded, a value less than zero otherwise.
 *
 * @param str_var The string to split up.
 * @param len     The actual length of @p str_var
 * @param uri     The coap_uri_t object to store the result.
 * @return        @c 0 on success, or < 0 on error.
 *
 */
int coap_split_uri(const unsigned char *str_var, size_t len, coap_uri_t *uri);



#endif /* _COAP_URI_H_ */
