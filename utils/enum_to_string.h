/*
 * enum_to_string.h
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 */
#ifndef UTILS_ENUM_TO_STRING_H_
#define UTILS_ENUM_TO_STRING_H_

#define GENERATE_ENUM(prefix, ENUM, v) prefix##ENUM = v,
#define GENERATE_STRING(prefix, STRING, v) (uint8_t *)#STRING,

#define DECLARE_PROPERTY_ENUM(prefix, name, foreach_value) \
  typedef enum prefix##name##_t \
  { \
    foreach_value(GENERATE_ENUM) \
  } \
  prefix##name##_t; \
  \
  static inline const uint8_t * name##_to_string(prefix##name##_t value) \
  { \
    static const uint8_t *strings[] = { \
      foreach_value(GENERATE_STRING) \
    }; \
   \
    return strings[value]; \
  } \
   \
  static inline bool string_to_##name(const uint8_t *value_str, prefix##name##_t *value_enum) \
  { \
    static const uint8_t *strings[] = { \
        foreach_value(GENERATE_STRING) \
    }; \
  \
    for (unsigned int i = 0; i < sizeof(strings) / sizeof(const char *); i++) \
    { \
      if (strcmp((char *)strings[i], (char *)value_str) == 0) \
      { \
        *value_enum = (prefix##name##_t) i; \
        return true; \
      } \
    } \
    return false; \
  }

#endif /* UTILS_ENUM_TO_STRING_H_ */
