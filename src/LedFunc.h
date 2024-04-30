#pragma once

#include <Arduino.h>
#include <ArduinoQueue.h>
#include <microLED.h>
// #define MY_DEBUG
#include "My_Debug.h"
#define L_INIT microLED<NUMLEDS, LEFT_PIN, MLED_NO_CLOCK, LED_WS2812, ORDER_GRB, CLI_AVER, SAVE_MILLIS>
#define R_INIT microLED<NUMLEDS, RIGHT_PIN, MLED_NO_CLOCK, LED_WS2812, ORDER_GRB, CLI_AVER, SAVE_MILLIS>
/// @brief
/// @param led Управляющая светодиодов
/// @param color Цвет мерцания
/// @param N Количество обязательных повторений
/// @param T1 Тайминг fade-in
/// @param T2 Тайминг полной яркости
/// @param T3 Тайминг fade-out
/// @param T4 Тайминг низкой яркости
/// @param Br_min Минимальная яркость (можно не тухнуть полностью)
/// @param Br_max Максимальная яркость
/// @param dekay Задердка исполнения (Больше -> Меньше FPS)
/// @param Show Обновлять автоматически?
/// @return Идёт выполнение
bool blinkL(L_INIT &led, mData color, uint8_t N, uint16_t T1, uint16_t T2, uint16_t T3, uint16_t T4, uint8_t Br_min = 0, uint8_t Br_max = 255, uint8_t dekay = 15, bool Show = 1)
{
  static uint16_t TT1 = 0, TT2 = 0, TT3 = 0, TT4 = 0;
  static uint8_t worker = 0, i = 0;
  static int32_t BR = 0;
  static mData prev_color = mBlack;
  if (prev_color != color)
  {
    prev_color = color;
    worker = 0;
  }

  switch (worker)
  {
  case 0: // инициализация минимальной яркости
  {
    worker = 1;
    i = 0;
    TT1 = millis() - 1;
    led.fill(color);
    led.setBrightness(Br_min);
    BR = Br_min;
    if (Show)
      led.show();
    break;
  }
  case 1: // fade-in
  {
    uint8_t dekay1 = dekay;
    uint16_t BT = (Br_max - Br_min) / T1 * dekay1;
    // DD(BT, 1000);
    if (BT == 0)
    {
      BT = 1;
      dekay1 = T1 / (Br_max - Br_min);
    }
    uint16_t T = (uint16_t)(millis() - TT1);
    if (T >= dekay1)
    {
      TT1 += dekay1;
      BR += BT;
      if (BR > Br_max)
        BR = Br_max;
      led.setBrightness(BR);
      if (Show)
        led.show();
    }
    if (BR >= Br_max)
    {
      worker++;
      TT2 = millis();
    }

    break;
  }
  case 2: // полная яркость
  {
    if (T2)
      if ((uint16_t)(millis() - TT2) >= T2)
      {
        worker++;
        TT2 = millis();
        TT3 = millis();
      }
    break;
  }
  case 3: // fade-out
  {
    uint8_t dekay3 = dekay;
    uint16_t BT = (Br_max - Br_min) / T3 * dekay3;
    if (BT == 0)
    {
      BT = 1;
      dekay3 = T3 / (Br_max - Br_min);
    }
    uint16_t T = (uint16_t)(millis() - TT3);
    if (T >= dekay3)
    {
      TT3 += dekay3;

      BR -= BT;
      if (BR < 0)
        BR = 0;
      led.setBrightness(BR);
      if (Show)
        led.show();
    }
    if (BR <= Br_min)
    {
      i++;
      if (i < N)
      {
        worker = 1;
        TT1 = millis() - 1;
      }
      else
        worker = 0;
      // TT3 = millis();
    }
    break;
  }
  case 4: //минимальная яркость
  {
    if (T4)
      if ((uint16_t)(millis() - TT4) >= T4)
      {
        worker++;
        TT1 = millis();
        TT4 = millis()-1;
      }
    break;
  }
  default:
  {
    worker = 0;
    break;
  }
  }

  return worker;
}
/// @brief
/// @param led Управляющая светодиодов
/// @param color Цвет мерцания
/// @param N Количество обязательных повторений
/// @param T1 Тайминг fade-in
/// @param T2 Тайминг полной яркости
/// @param T3 Тайминг fade-out
/// @param Br_min Минимальная яркость (можно не тухнуть полностью)
/// @param Br_max Максимальная яркость
/// @param dekay Задердка исполнения (Больше -> Меньше FPS)
/// @param Show Обновлять автоматически?
/// @return Идёт выполнение
bool blinkR(R_INIT &led, mData color, uint8_t N, uint16_t T1, uint16_t T2, uint16_t T3, uint8_t Br_min = 0, uint8_t Br_max = 255, uint8_t dekay = 15, bool Show = 1)
{
  static uint16_t TT1 = 0, TT2 = 0, TT3 = 0;
  static uint8_t worker = 0, i = 0;
  static int32_t BR = 0;
  static mData prev_color = mBlack;
  if (prev_color != color)
  {
    prev_color = color;
    worker = 0;
  }

  switch (worker)
  {
  case 0:
  {
    worker = 1;
    i = 0;
    TT1 = millis() - 1;
    led.fill(color);
    led.setBrightness(Br_min);
    BR = Br_min;
    if (Show)
      led.show();
    break;
  }
  case 1:
  {
    uint8_t dekay1 = dekay;
    uint16_t BT = (Br_max - Br_min) / T1 * dekay1;
    // DD(BT, 1000);
    if (BT == 0)
    {
      BT = 1;
      dekay1 = T1 / (Br_max - Br_min);
    }
    uint16_t T = (uint16_t)(millis() - TT1);
    if (T >= dekay1)
    {
      TT1 += dekay1;
      BR += BT;
      if (BR > Br_max)
        BR = Br_max;
      led.setBrightness(BR);
      if (Show)
        led.show();
    }
    if (BR >= Br_max)
    {
      worker++;
      TT2 = millis();
    }

    break;
  }
  case 2:
  {
    if (T2)
      if ((uint16_t)(millis() - TT2) >= T2)
      {
        worker++;
        TT2 = millis();
        TT3 = millis();
      }
    break;
  }
  case 3:
  {
    uint8_t dekay3 = dekay;
    uint16_t BT = (Br_max - Br_min) / T3 * dekay3;
    if (BT == 0)
    {
      BT = 1;
      dekay3 = T3 / (Br_max - Br_min);
    }
    uint16_t T = (uint16_t)(millis() - TT3);
    if (T >= dekay3)
    {
      TT3 += dekay3;

      BR -= BT;
      if (BR < 0)
        BR = 0;
      led.setBrightness(BR);
      if (Show)
        led.show();
    }
    if (BR <= Br_min)
    {
      i++;
      if (i < N)
      {
        worker = 1;
        TT1 = millis() - 1;
      }
      else
        worker = 0;
      // TT3 = millis();
    }
    break;
  }
  default:
  {
    worker = 0;
    break;
  }
  }

  return worker;
}

bool blinkSync(L_INIT &l_led, R_INIT &r_led, mData color, uint8_t N, uint16_t T1, uint16_t T2, uint16_t T3, uint8_t Br_min = 0, uint8_t Br_max = 255, uint8_t dekay = 15, bool Show = 1)
{
}
const uint8_t NUMB_L[][15] PROGMEM = {
    {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 00, 00, 00}, // 0
    {0, 2, 0, 0, 0, 0, 0, 8, 0, 00, 00, 00, 13, 14, 15}, // 1
    {1, 2, 3, 0, 5, 6, 7, 8, 9, 00, 11, 12, 00, 14, 00}, // 2
    {1, 2, 3, 0, 5, 0, 7, 8, 9, 10, 11, 12, 00, 14, 00}, // 3
    {1, 0, 3, 4, 5, 0, 0, 0, 9, 10, 11, 12, 00, 14, 00}, // 4
    {1, 2, 3, 4, 5, 0, 7, 8, 9, 10, 11, 00, 00, 14, 00}, // 5
    {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 00, 00, 14, 00}, // 6
    {1, 2, 3, 0, 0, 0, 0, 8, 0, 00, 11, 12, 00, 00, 15}, // 7
    {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 00, 14, 00}, // 8
    {1, 2, 3, 4, 5, 0, 7, 8, 9, 10, 11, 12, 00, 14, 00}, // 9
};
const uint8_t NUMB_R[][15] PROGMEM = {
    {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 00, 00, 00}, // 0
    {0, 2, 0, 0, 0, 0, 0, 8, 0, 00, 00, 00, 13, 14, 15}, // 1
    {1, 2, 3, 0, 5, 6, 7, 8, 9, 00, 11, 12, 00, 14, 00}, // 2
    {1, 2, 3, 4, 5, 6, 7, 8, 9, 00, 11, 00, 00, 14, 00}, // 3
    {0, 0, 3, 4, 5, 6, 7, 0, 9, 10, 11, 00, 00, 14, 00}, // 4
    {1, 2, 3, 4, 5, 0, 7, 8, 9, 10, 11, 00, 00, 14, 00}, // 5
    {1, 2, 3, 4, 5, 0, 7, 8, 9, 10, 11, 12, 00, 14, 00}, // 6
    {0, 2, 0, 0, 5, 6, 7, 8, 9, 00, 00, 00, 13, 00, 00}, // 7
    {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 00, 14, 00}, // 8
    {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 00, 00, 14, 00}, // 9

};

#define pread_8t(X) pgm_read_byte(&X)
#define pread_16t(X) pgm_read_word(&X)

/// @brief
/// @param led Управляющая светодиодов
/// @param number Цифра
/// @param color1 Цвет цифры
/// @param color2 Цвет фона
void SHOW_NUM_L(L_INIT &led, uint8_t &number, mData color1, mData color2 = mBlack)
{
  for (uint8_t i = 0; i < NUMLEDS; i++)
  {
    if (pread_8t(NUMB_L[number][i]))
    {
      led.set(i, color1);
    }
    else
    {
      led.set(i, color2);
    }
  }
  led.show();
}

/// @brief
/// @param led Управляющая светодиодов
/// @param number Цифра
/// @param color1 Цвет цифры
/// @param color2 Цвет фона
void SHOW_NUM_R(R_INIT &led, uint8_t &number, mData color1, mData color2 = mBlack)
{
  for (uint8_t i = 0; i < NUMLEDS; i++)
  {
    if (pread_8t(NUMB_R[number][i]))
    {
      led.set(i, color1);
    }
    else
    {
      led.set(i, color2);
    }
  }
  led.show();
}
