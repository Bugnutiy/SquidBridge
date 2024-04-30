#include <Arduino.h>
// #define MY_DEBUG_LED
#define MY_DEBUG
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

#define VIBRATOR_LEFT_SENS 30  // сколько раз должен сработать вибратор, чтобы тригернуть систему
#define VIBRATOR_RIGHT_SENS 30 // сколько раз должен сработать вибратор, чтобы тригернуть систему

#define NUMLEDS 15 // кол-во светодиодов

#define timer_type uint16_t
#define timer_vibro_reset 100 // как часто сбрасывать счётчик вибрации

#define COLOR_DEBTH 1

#include <microLED.h>
// #include <math.h>
#include "Timer.h"
#include "RemoteCommands.h"
#include <IRremote.hpp>
#include "Patterns.h"

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

IrData received;
uint8_t device_id = 255, devices_count = 1;
void setup()
{
#ifdef MY_DEBUG
  Serial.begin(115200);
#endif

  l_led.setBrightness(BRIGHTNESS_MAX);
  r_led.setBrightness(BRIGHTNESS_MAX);
  unsigned long seed = 0;

  seed = analogRead(A0);

  randomSeed(seed);
  pattern = random(2147483648) % 2;
  pinMode(VIBRATOR_LEFT_PIN, INPUT_PULLUP);
  pinMode(VIBRATOR_RIGHT_PIN, INPUT_PULLUP);
  attachInterrupt(VIBRATOR_LEFT, left_vibr, CHANGE);
  attachInterrupt(VIBRATOR_RIGHT, right_vibr, CHANGE);
  randomSeed(analogRead(A0));
  IrReceiver.begin(RECEIVER_PIN);
  IrSender.begin(SENDER_PIN);
  DDD("Pattern: ");
  DD(pattern);
  DDD("Seed: ");
  DD(seed);
}

uint8_t workerMain = 0;
void loop()
{
  switch (workerMain)
  {
  case 0:
  {
    static bool flag = 1;
    static uint16_t InintTimer = millis();
    DD("Waiting for initialization");
    while (flag)
    {
      TMR16(500, {
        static bool f1 = 0;
        f1 = !f1;
        r_led.fill(0, 2, f1 ? mRed : mBlack);
        r_led.show();
      });
      if (millis() - SendTimer >= SEND_DELAY * 2)
      {
        // IrReceiver.stop();
        IrSender.sendNEC(device_id, STD_COMMANDS::INIT_REQUEST, 1);
        // delay(SEND_DELAY);
        // IrReceiver.begin(RECEIVER_PIN);
      }
      if (IrReceiver.decode())
      {
        SendTimer = millis();
        received = IrData{IrReceiver.decodedIRData.address, (uint8_t)IrReceiver.decodedIRData.command};
        if (received.address == device_id || received.address == 0)
        {
          IrReceiver.resume();
        }

        DDD("{");
        DDD(received.address);
        DDD(", ");
        DDD(received.command);
        DD("}");

        if (received.command == STD_COMMANDS::INIT_COMMAND)
        {
          DD("INIT_RECIEVED");
          flag = 0;
          device_id = received.address + 1;
          devices_count = device_id;
        }
        if (received == CARMP3::btn_1)
        {
          DD("INIT_RECIEVED REMOTE 1");
          flag = 0;
          device_id = 1;
          devices_count = 1;
        }
        if (received == CARMP3::btn_2)
        {
          DD("INIT_RECIEVED REMOTE 2");
          flag = 0;
          device_id = 2;
          devices_count = 2;
        }
        if (received == CARMP3::btn_3)
        {
          DD("INIT_RECIEVED REMOTE 3");
          flag = 0;
          device_id = 3;
          devices_count = 3;
        }
        if (received == CARMP3::btn_4)
        {
          DD("INIT_RECIEVED REMOTE 4");
          flag = 0;
          device_id = 4;
          devices_count = 4;
        }
        if (received == CARMP3::btn_5)
        {
          DD("INIT_RECIEVED REMOTE 5");
          flag = 0;
          device_id = 5;
          devices_count = 5;
        }
        if (received == CARMP3::btn_6)
        {
          DD("INIT_RECIEVED REMOTE 6");
          flag = 0;
          device_id = 6;
          devices_count = 6;
        }
        if (received == CARMP3::btn_7)
        {
          DD("INIT_RECIEVED REMOTE 7");
          flag = 0;
          device_id = 7;
          devices_count = 7;
        }
        if (received == CARMP3::btn_8)
        {
          DD("INIT_RECIEVED REMOTE 8");
          flag = 0;
          device_id = 8;
          devices_count = 8;
        }
        if (received == CARMP3::btn_9)
        {
          DD("INIT_RECIEVED REMOTE 9");
          flag = 0;
          device_id = 9;
          devices_count = 9;
        }
        if (received == CARMP3::btn_0)
        {
          DD("INIT_RECIEVED REMOTE 10");
          flag = 0;
          device_id = 10;
          devices_count = 10;
        }
        IrReceiver.resume();
      }
      if (millis() - InintTimer > 5000)
      {
        flag = 0;
        device_id = 1;
        devices_count = 1;
        DD("No devices found");
        DD_LED(0);
      }
    }
    DDD("Device ID: ");
    DD(device_id);
    DDD("Devices count: ");
    DD(devices_count);
    DD_LED(device_id);
    workerMain = 1;
    pattern = pread_8t(PATTERNS[0][device_id - 1]);
  }
  break;

  case 1:
  {
    SendData();
    if (IrReceiver.decode())
    {
      SendTimer = millis();
      received = IrData{IrReceiver.decodedIRData.address, (uint8_t)IrReceiver.decodedIRData.command};
      DDD("{");
      DDD(received.address);
      DDD(", ");
      DDD(received.command);
      DD("}");

      // if (received == CARMP3::btn_CH_minus) // сброс шаблона
      // {
      //   int a = analogRead(A0);
      //   DDD("A0 = ");
      //   DD(a);
      //   pattern = random(2147483648) % 2;
      //   DDD("Pattern changed: ");
      //   DD(pattern);
      //   static uint16_t k0 = 0, k1 = 0;
      //   if (pattern)
      //   {
      //     k1 += 1;
      //   }
      //   else
      //   {
      //     k0 += 1;
      //   }
      //   DDD("k0 = ");
      //   DD(k0);
      //   DDD("k1 = ");
      //   DD(k1);
      //   while (blinkSync(l_led, r_led, mPurple, 1, 100, 0, 100, 0, 0, 255, 1, 1))
      //   {
      //   }
      // }

      switch (received.command)
      {
      case STD_COMMANDS::SYNC_COMMAND:
      {
      }
      break;

      default:
        break;
      }
      IrReceiver.resume();
    }
    static uint16_t t1 = random(200, 1000), t2 = random(200, 2000), t3 = random(200, 1000), t4 = random(5000, 10000);
    if (!blinkSync(l_led, r_led, mAqua, 1, t1, t2, t3, t4, 50, 255, 1, 1))
    {
      t1 = random(200, 1000);
      t2 = random(200, 2000);
      t3 = random(200, 1000);
      t4 = random(5000, 10000);
    }

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
      while (blinkL(l_led, pattern ? mRed : mLime, 3, 100, 0, 100, 0, 0, 255, 1, 1))
      {
        DD("LEFT_RED", 1000);
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
      while (blinkR(r_led, pattern ? mLime : mRed, 3, 100, 0, 100, 0, 0, 255, 1, 1))
      {
        DD("RIGHT_RED", 1000);

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
  break;
  default:
    workerMain = 1;
    break;
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
