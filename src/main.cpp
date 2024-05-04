#include <Arduino.h>
#define MY_DEBUG_LED 1000
#define MY_DEBUG
#include "My_Debug.h"

// #define DEBUG 1
#define RECEIVER_PIN 11
#define SENDER_PIN 12

#define LEFT_PIN 7  // пин L-ленты
#define RIGHT_PIN 8 // пин R-ленты
#define DEKAY 15

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

#define PATTERN_CHANGE_TIME 20000
#define INIT_TIME 50000

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

uint8_t workerMain = 0;
uint8_t pattern_number = 0;
uint32_t patternChangeTimer = 0;

bool initSender = 0, initSent = 0;
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
  // DDD("Pattern: ");
  // DD(pattern);
  // DDD("Seed: ");
  // DD(seed);

  static bool flag = 1;
  static uint16_t InintTimer = millis();
  DD("Waiting for initialization");
  while (flag)
  {
    TMR16(500, {
      static bool f1 = 0;
      f1 = !f1;
      r_led.fill(0, 2, f1 ? mRed : mBlack);
      l_led.fill(6, 8, f1 ? mRed : mBlack);
      r_led.show();
      l_led.show();
    });
    if (uint16_t(millis() - SendTimer) >= SEND_DELAY * 2)
    {
      IrSender.sendNEC(device_id, STD_COMMANDS::INIT_REQUEST, 1);
    }
    if (IrReceiver.decode())
    {
      received = IrData{IrReceiver.decodedIRData.address, (uint8_t)IrReceiver.decodedIRData.command};
      if (received.address == device_id || received.command == 0)
      {
        goto stop;
      }
      if (received.address != 0)
        SendTimer = uint16_t(millis() + (SEND_DELAY / 10));
      else
        SendTimer = millis();

      // DDD("{");
      // DDD(received.address);
      // DDD(", ");
      // DDD(received.command);
      // DD("}");

      if (received.command == STD_COMMANDS::INIT_COMMAND)
      {
        DD("INIT_RECIEVED");
        flag = 0;
        device_id = received.address + 1;
        devices_count = device_id;
        SendDataAdd(device_id, STD_COMMANDS::INIT_ANSWER);
        SendDataAdd(device_id, STD_COMMANDS::INIT_ANSWER);
        SendDataAdd(device_id, STD_COMMANDS::INIT_ANSWER);
        goto stop;
      }
      if (received == IrData{CARMP3::address, CARMP3::btn_1})
      {
        DD("INIT_RECIEVED REMOTE 1");
        flag = 0;
        device_id = 1;
        devices_count = 1;
        goto stop;
      }
      if (received == IrData{CARMP3::address, CARMP3::btn_2})
      {
        DD("INIT_RECIEVED REMOTE 2");
        flag = 0;
        device_id = 2;
        devices_count = 2;
        goto stop;
      }
      if (received == IrData{CARMP3::address, CARMP3::btn_3})
      {
        DD("INIT_RECIEVED REMOTE 3");
        flag = 0;
        device_id = 3;
        devices_count = 3;
        goto stop;
      }
      if (received == IrData{CARMP3::address, CARMP3::btn_4})
      {
        DD("INIT_RECIEVED REMOTE 4");
        flag = 0;
        device_id = 4;
        devices_count = 4;
        goto stop;
      }
      if (received == IrData{CARMP3::address, CARMP3::btn_5})
      {
        DD("INIT_RECIEVED REMOTE 5");
        flag = 0;
        device_id = 5;
        devices_count = 5;
        goto stop;
      }
      if (received == IrData{CARMP3::address, CARMP3::btn_6})
      {
        DD("INIT_RECIEVED REMOTE 6");
        flag = 0;
        device_id = 6;
        devices_count = 6;
        goto stop;
      }
      if (received == IrData{CARMP3::address, CARMP3::btn_7})
      {
        DD("INIT_RECIEVED REMOTE 7");
        flag = 0;
        device_id = 7;
        devices_count = 7;
        goto stop;
      }
      if (received == IrData{CARMP3::address, CARMP3::btn_8})
      {
        DD("INIT_RECIEVED REMOTE 8");
        flag = 0;
        device_id = 8;
        devices_count = 8;
        goto stop;
      }
      if (received == IrData{CARMP3::address, CARMP3::btn_9})
      {
        DD("INIT_RECIEVED REMOTE 9");
        flag = 0;
        device_id = 9;
        devices_count = 9;
        goto stop;
      }
      if (received == IrData{CARMP3::address, CARMP3::btn_0})
      {
        DD("INIT_RECIEVED REMOTE 10");
        flag = 0;
        device_id = 10;
        devices_count = 10;
      }
    stop:
      IrReceiver.resume();
    }
    if (uint16_t(millis() - InintTimer) > INIT_TIME)
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
  workerMain = 0;
  if (device_id <= 10)
    pattern = pread_8t(PATTERNS[pattern_number][device_id - 1]);
  else
    pattern = random(2147483647) % 2;
}

void loop()
{
  switch (workerMain)
  {
  case 0:
  {
    if (IrReceiver.decode())
    {
      received = IrData{IrReceiver.decodedIRData.address, (uint8_t)IrReceiver.decodedIRData.command};
      if (received.address == device_id || received.command == 0)
      {
        goto stop1;
      }
      if (received.address == uint8_t(device_id - 1))
        SendTimer = uint16_t(millis() + (SEND_DELAY / 10));
      else
        SendTimer = millis();
      DDD("{");
      DDD(received.address);
      DDD(", ");
      DDD(received.command);
      DD("}");

      switch (received.command)
      {

      case STD_COMMANDS::INIT_COMMAND:
      {
        if (received.address == uint8_t(device_id - 1))
        {

          SendDataAdd(device_id, STD_COMMANDS::INIT_ANSWER);
          SendDataAdd(device_id, STD_COMMANDS::INIT_ANSWER);
          SendDataAdd(device_id, STD_COMMANDS::INIT_ANSWER);

          goto stop1;
        }
      }
      break;

      case STD_COMMANDS::INIT_REQUEST:
      {
        if (received.address != device_id && !initSent)
        {
          initSender = 1;
          goto stop1;
        }
      }
      break;

      case STD_COMMANDS::INIT_ANSWER:
      {
        if (received.address == uint8_t(device_id + 1))
        {
          if (!initSent)
          {
            devices_count++;
          }
          initSent = 1;
          initSender = 0;
          DD_LED((devices_count));
          goto stop1;
        }
      }
      break;

      case STD_COMMANDS::SYNC_COMMAND:
      {
        if (received.address == uint8_t(device_id - 1))
        {
          blinkSync(l_led, r_led, mAqua, 1, 200, 0, 200, 4600, 50, 255, DEKAY, 1, 0);
          DD("Sync completed");
          goto stop1;
        }
      }
      break;

      case CARMP3::btn_CH_minus: // Change pattern
      {
        if (received.address == CARMP3::address)
        {
          if (millis() - patternChangeTimer > PATTERN_CHANGE_TIME)
          {
            pattern_number++;
            if (pattern_number < PATTERNS_COUNT && device_id <= 10)
              pattern = pread_8t(PATTERNS[pattern_number][device_id - 1]);
            else
              pattern = random(2147483647) % 2;

            while (blinkSync(l_led, r_led, mPurple, 1, 100, 100, 100, 0, 20, 255, DEKAY, 1))
            {
              SHOW_NUM_R(r_led, pattern_number % 10, mPurple, mBlack);
              SHOW_NUM_L(l_led, (pattern_number / 10) % 10, mPurple, mBlack);
            }
            DDD("Pattern changed: ");
            DD(pattern);
            DDD("Pattern number:");
            DD(pattern_number);
            patternChangeTimer = millis();
          }
          DD("Pattern request sending");
          SendDataAdd(device_id, STD_COMMANDS::CHANGE_PATTERN);
          SendDataAdd(device_id, STD_COMMANDS::CHANGE_PATTERN);
          SendDataAdd(device_id, STD_COMMANDS::CHANGE_PATTERN);

          goto stop1;
        }
      }
      break;

      case STD_COMMANDS::CHANGE_PATTERN:
      {
        if (received.address < device_id)
        {
          if (millis() - patternChangeTimer > PATTERN_CHANGE_TIME && device_id <= 10)
          {
            pattern_number++;
            if (pattern_number < PATTERNS_COUNT)
              pattern = pread_8t(PATTERNS[pattern_number][device_id - 1]);
            else
              pattern = random(2147483647) % 2;
            while (blinkSync(l_led, r_led, mPurple, 1, 100, 100, 100, 0, 0, 255, DEKAY, 0))
            {
              SHOW_NUM_R(r_led, pattern_number % 10, mPurple);
              SHOW_NUM_L(l_led, (pattern_number / 10) % 10, mPurple);
            }
            DDD("Pattern changed: ");
            DD(pattern);
            DDD("Pattern number:");
            DD(pattern_number);
            patternChangeTimer = millis();
          }
          DD("Pattern request sending");
          SendDataAdd(device_id, STD_COMMANDS::CHANGE_PATTERN);
          SendDataAdd(device_id, STD_COMMANDS::CHANGE_PATTERN);
          SendDataAdd(device_id, STD_COMMANDS::CHANGE_PATTERN);
        }
      }
      break;

      default:
        break;
      }
    stop1:
      IrReceiver.resume();
    }
    if (initSender)
    {
      TMR16(SEND_DELAY_2, {
        if (SendData())
        {
          SendDataAdd(device_id, STD_COMMANDS::INIT_COMMAND);
          SendData();
        }
      });
    }

    if (blinkSync(l_led, r_led, mAqua, 1, 200, 200, 200, 4400, 50, 255, DEKAY, 1) == 3)
    {
      TMR16(10000, {
        if (SendData())
          SendDataAdd(device_id, STD_COMMANDS::SYNC_COMMAND);
        // DD("Send Sync Command:");
      });
    }
    SendData();
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
      while (blinkL(l_led, pattern ? mRed : mLime, 1, 100, 1000, 100, 0, 0, 255, DEKAY, 1))
      {
      }
      r_vib = 0;
    }
    if (r_vib > VIBRATOR_RIGHT_SENS)
    {
      while (blinkR(r_led, pattern ? mLime : mRed, 1, 100, 1000, 100, 0, 0, 255, DEKAY, 1))
      {
      }
      l_vib = 0;
    }
  }
  break;
  default:
    workerMain = 0;
    break;
  }
}

// put function definitions here:
void left_vibr()
{
  ++l_vib;

  timer_l_vibr = millis();
}
void right_vibr()
{
  ++r_vib;

  timer_r_vibr = millis();
}
