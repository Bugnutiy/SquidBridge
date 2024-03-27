#pragma once
/**
 * @brief вместо T время, вместо X код
 */
#define TMR32(T, X) \
    ({static uint32_t tmr##T = 0;                 \
    if ((uint32_t)(millis() - tmr##T) >= T) \
    {                                       \
      tmr##T = millis();                    \
      X                                     \
    } })

#define TMR16(T, X) \
    ({static uint16_t tmr##T = 0;                 \
    if ((uint16_t)(millis() - tmr##T) >= T) \
    {                                       \
      tmr##T = millis();                    \
      X                                     \
    } })

#define TMR32_S(T, X) \
    ({static uint32_t tmr##T = 0;                 \
    if ((uint32_t)(millis() - tmr##T) >= T) \
    {                                       \
      tmr##T += T;                    \
      X                                     \
    } })

#define TMR16_S(T, X) \
    ({static uint16_t tmr##T = 0;                 \
    if ((uint16_t)(millis() - tmr##T) >= T) \
    {                                       \
      tmr##T +=T;                    \
      X                                     \
    } })
