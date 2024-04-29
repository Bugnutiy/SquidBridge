#include <Arduino.h>
// #define MY_DEBUG_LED
// #define MY_DEBUG
#include "My_Debug.h"

// #define DEBUG 1
#define RECEIVER_PIN 11
#define SENDER_PIN 12

#define LEFT_PIN 7  // пин L-ленты
#define RIGHT_PIN 8 // пин R-ленты

#define BRIGHTNESS_MAX 255 // 10..255

#define VIBRATOR_LEFT 0 // 0=2 Прерывания датчиков вибрации
#define VIBRATOR_LEFT_PIN 2
#define VIBRATOR_RIGHT 1 // 1=3
#define VIBRATOR_RIGHT_PIN 3

#define VIBRATOR_LEFT_SENS 20  // сколько раз должен сработать вибратор, чтобы тригернуть систему
#define VIBRATOR_RIGHT_SENS 20 // сколько раз должен сработать вибратор, чтобы тригернуть систему

#define NUMLEDS 15 // кол-во светодиодов

#define timer_type uint16_t
#define timer_vibro_reset 100 // как часто сбрасывать счётчик вибрации

#define COLOR_DEBTH 3

#include <microLED.h>
// #include <math.h>
#include "Timer.h"
#include "RemoteCommands.h"
#include <IRremote.hpp>

microLED<NUMLEDS, LEFT_PIN, MLED_NO_CLOCK, LED_WS2812, ORDER_GRB, CLI_AVER, SAVE_MILLIS> l_led;
microLED<NUMLEDS, RIGHT_PIN, MLED_NO_CLOCK, LED_WS2812, ORDER_GRB, CLI_AVER, SAVE_MILLIS> r_led;

// put function declarations here:
volatile uint16_t l_vib = 0;
timer_type timer_l_vibr = 0;
void left_vibr();

volatile uint16_t r_vib = 0;
timer_type timer_r_vibr = 0;
void right_vibr();

bool pattern = 0;
#include "LedFunc.h"
#include "TransmitterFunc.h"
void setup()
{
#ifdef MY_DEBUG
  Serial.begin(115200);
#endif

  l_led.setBrightness(BRIGHTNESS_MAX);
  r_led.setBrightness(BRIGHTNESS_MAX);
  randomSeed(analogRead(A0));
  pattern = random(2);
  pinMode(VIBRATOR_LEFT_PIN, INPUT_PULLUP);
  pinMode(VIBRATOR_RIGHT_PIN, INPUT_PULLUP);
  attachInterrupt(VIBRATOR_LEFT, left_vibr, CHANGE);
  attachInterrupt(VIBRATOR_RIGHT, right_vibr, CHANGE);
  randomSeed(analogRead(A0));
  IrReceiver.begin(RECEIVER_PIN);
  IrSender.begin(SENDER_PIN);
}

void loop()
{
  SendData();
  if (IrReceiver.decode())
  {
    SendTimer = millis();
    IrData received = IrData{IrReceiver.decodedIRData.address, (uint8_t)IrReceiver.decodedIRData.command};
    DDD("{");
    DDD(received.address);
    DDD(", ");
    DDD(received.command);
    DD("}");

    if (received == CARMP3.btn_CH_minus) // сброс шаблона
    {
      pattern = random(2);
      DD("Pattern changed");
      static bool flag = 1;
      static bool flagL = 1, flagR = 1;
      flag = 1;
      flagL = 1;
      flagR = 1;
      while (flag)
      {

        if (flagL && !blinkL(l_led, mPurple, 3, 100, 200, 100, 50, 255, 1, 1))
        {
          flagL = 0;
        }
        if (flagR && !blinkR(r_led, mPurple, 3, 100, 200, 100, 50, 255, 1, 1))
        {
          flagR = 0;
        }
        if (!flagL && !flagR)
        {
          flag = 0;
        }
      }
    }

    IrReceiver.resume();
  }

  blinkL(l_led, mAqua, 1, 2000, 200, 1000, 100, 180, 1, 1);
  blinkR(r_led, mAqua, 1, 2000, 200, 1000, 100, 180, 1, 1);

  if ((timer_type)(millis() - timer_l_vibr) >= timer_vibro_reset)
  {
    l_vib = 0;
    timer_l_vibr = millis(); // Таймер сбрасывается ещё в одном месте!
  }
  if ((timer_type)(millis() - timer_r_vibr) >= timer_vibro_reset)
  {
    r_vib = 0;
    timer_r_vibr = millis(); // Таймер сбрасывается ещё в одном месте!
  }

  if (l_vib > VIBRATOR_LEFT_SENS)
  {
    while (blinkL(l_led, pattern ? mRed : mGreen, 3, 100, 500, 100, 0, 255, 30, 1))
    {
#ifdef MY_DEBUG_LED
      static uint16_t DBTime = 0;
      if ((uint16_t)(millis() - DBTime) > 50)
      {
        DBTime += 50;
        PORTB ^= B00100000; // Переключить 13 порт
      }
#endif
    }
    r_vib = 0;
  }
  if (r_vib > VIBRATOR_RIGHT_SENS)
  {
    while (blinkR(r_led, pattern ? mGreen : mRed, 3, 100, 500, 100, 0, 255, 30, 1))
    {
#ifdef MY_DEBUG_LED
      static uint16_t DBTime = 0;
      if ((uint16_t)(millis() - DBTime) > 50)
      {
        DBTime += 50;
        PORTB ^= B00100000; // Переключить 13 порт
      }
#endif
    }
    l_vib = 0;
  }
}

// put function definitions here:
void left_vibr()
{
  ++l_vib;
#ifdef MY_DEBUG_LED
  PORTB ^= B00100000; // Переключить 13 порт
#endif
  timer_l_vibr = millis();
}
void right_vibr()
{
  ++r_vib;
#ifdef MY_DEBUG_LED
  PORTB ^= B00100000; // Переключить 13 порт
#endif
  timer_r_vibr = millis();
}
