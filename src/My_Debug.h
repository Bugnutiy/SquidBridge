#pragma once
#include <Arduino.h>
// #define MY_DEBUG
// #define MY_DEBUG_LED 1000

#ifdef MY_DEBUG_LED
#define DD_LED(X)                           \
  {                                         \
    SHOW_NUM_L(l_led, (X / 10) % 10, mRed); \
    SHOW_NUM_R(r_led, X % 10, mRed);        \
    delay(MY_DEBUG_LED);                    \
  }
#else
#define DD_LED(X)
#endif

#ifdef MY_DEBUG

#define DD_1(X) Serial.println(X)
#define DDD_1(X) Serial.print(X)
#define DD_2(X, T)                          \
  {                                         \
    static uint16_t TMR##T;                 \
    if ((uint16_t)(millis() - TMR##T) >= T) \
    {                                       \
      TMR##T = millis();                    \
      Serial.println(X);                    \
    }                                       \
  }
#define DDD_2(X, T)                         \
  {                                         \
    static uint16_t TMR##T;                 \
    if ((uint16_t)(millis() - TMR##T) >= T) \
    {                                       \
      TMR##T = millis();                    \
      Serial.print(X);                      \
    }                                       \
  }

#define DD_3(X, Y, T)                       \
  {                                         \
    static uint16_t TMR##T;                 \
    if ((uint16_t)(millis() - TMR##T) >= T) \
    {                                       \
      TMR##T = millis();                    \
      Serial.println(X, Y);                 \
    }                                       \
  }
#define DDD_3(X, Y, T)                      \
  {                                         \
    static uint16_t TMR##T;                 \
    if ((uint16_t)(millis() - TMR##T) >= T) \
    {                                       \
      TMR##T = millis();                    \
      Serial.print(X, Y);                   \
    }                                       \
  }

#define DD_SELECT(_1, _2, _3, macro, ...) macro

/**
 * @brief
 * DD(X), DD(X, Time), DD(X, Y, Time)
 */
#define DD(...) DD_SELECT(__VA_ARGS__, DD_3, DD_2, DD_1, dummy)(__VA_ARGS__)
#define DDD(...) DD_SELECT(__VA_ARGS__, DDD_3, DDD_2, DDD_1, dummy)(__VA_ARGS__)

#else

#define DD(...)
#define DDD(...)

#endif
