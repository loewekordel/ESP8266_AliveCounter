#ifndef _UTILS_H_
#define _UTILS_H_

#include <Arduino.h>

/** @brief Activate debug output to serial */
#define DEBUG_SERIAL

#ifdef DEBUG_SERIAL
/** @brief Makro for debug output to serial */
  #define LOG(msg) do { \
                          Serial.print(msg); \
                        } while (0)
/** @brief Makro for debug output to serial with newline */
  #define LOGLN(msg) do { \
                            Serial.println(msg); \
                          } while (0)
#else
/** @brief Makro for debug output to serial */
  #define LOG(msg)
/** @brief Makro for debug output to serial with newline */
  #define LOGLN(msg) 
#endif

#endif
