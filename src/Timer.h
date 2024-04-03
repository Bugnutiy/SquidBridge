#pragma once
/**
 * @brief вместо T время, вместо CODE код
 */
#define TMR32(T, CODE) \
  ({static uint32_t tmr##T = 0;                 \
    if ((uint32_t)(millis() - tmr##T) >= T) \
    {                                       \
      tmr##T = millis();                    \
      CODE                                     \
    } })

#define TMR16(T, CODE) \
  ({static uint16_t tmr##T = 0;                 \
    if ((uint16_t)(millis() - tmr##T) >= T) \
    {                                       \
      tmr##T = millis();                    \
      CODE                                     \
    } })

#define TMR8(T, CODE) \
  ({static uint8_t tmr##T = 0;                 \
    if ((uint8_t)(millis() - tmr##T) >= T) \
    {                                       \
      tmr##T = millis();                    \
      CODE                                     \
    } })

#define TMR32_S(T, CODE) \
  ({static uint32_t tmr##T = 0;                 \
    if ((uint32_t)(millis() - tmr##T) >= T) \
    {                                       \
      tmr##T += T;                    \
      CODE                                     \
    } })

#define TMR16_S(T, CODE) \
  ({static uint16_t tmr##T = 0;                 \
    if ((uint16_t)(millis() - tmr##T) >= T) \
    {                                       \
      tmr##T +=T;                    \
      CODE                                     \
    } })

#define TMR8_S(T, CODE) \
  ({static uint8_t tmr##T = 0;                 \
    if ((uint8_t)(millis() - tmr##T) >= T) \
    {                                       \
      tmr##T +=T;                    \
      CODE                                     \
    } })