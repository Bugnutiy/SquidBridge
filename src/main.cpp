#include <Arduino.h>
#define MY_DEBUG_LED 1000
// #define MY_DEBUG
#include "My_Debug.h"

// #define DEBUG 1
#define RECEIVER_PIN 11
#define SENDER_PIN 12

#define LEFT_PIN 7  // пин L-ленты
#define RIGHT_PIN 8 // пин R-ленты
#define DEKAY 1

#define BRIGHTNESS_MAX 255 // 10..255
#define WAWE_IN 200
#define WAWE_FULL 100
#define WAWE_OUT 1000
#define WAWE_MIN 4000
#define WAWE_MIN_BRIGHT 100
#define WAWE_MAX_BRIGHT 255
#define SYNC_TIMER 30000

#define BLINK_N 1
#define BLINK_IN 100
#define BLINK_FULL 1000
#define BLINK_OUT 100
#define BLINK_MIN 0
#define BLINK_MIN_BRIGHT 100
#define BLINK_MAX_BRIGHT 255

#define SHOW_BLINK_N 1
#define SHOW_BLINK_IN 100
#define SHOW_BLINK_FULL 1000
#define SHOW_BLINK_OUT 100
#define SHOW_BLINK_MIN 0
#define SHOW_BLINK_MIN_BRIGHT 100
#define SHOW_BLINK_MAX_BRIGHT 255

#define SHOW_PATTERN_TIMER 5000

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
#define INIT_TIME 10000

#define COLOR_DEBTH 1

#include <microLED.h>
// #include <math.h>
#include "Timer.h"
#include "RemoteCommands.h"
#include <IRremote.hpp>
#include "Patterns.h"
#include <EEManager.h>

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
#include "Randomizer.h"

IrData received;
uint8_t device_id = 255, device_next = 1;

// uint8_t workerMain = 0;
uint8_t pattern_number = 0;
uint32_t patternChangeTimer = 0;

bool initSender = 0, initSent = 0;

Randomizer randomizer;

uint8_t syncRequired = 0;
uint32_t synchronized = 0;
void setup()
{
#ifdef MY_DEBUG
  Serial.begin(115200);
#endif

  l_led.setBrightness(BRIGHTNESS_MAX);
  r_led.setBrightness(BRIGHTNESS_MAX);
  unsigned long seed = 0;

  // seed = analogRead(A0);

  // randomSeed(seed);
  // pattern = random(2147483648) % 2;
  pinMode(VIBRATOR_LEFT_PIN, INPUT_PULLUP);
  pinMode(VIBRATOR_RIGHT_PIN, INPUT_PULLUP);
  attachInterrupt(VIBRATOR_LEFT, left_vibr, CHANGE);
  attachInterrupt(VIBRATOR_RIGHT, right_vibr, CHANGE);
  IrReceiver.begin(RECEIVER_PIN);
  IrSender.begin(SENDER_PIN);
  // DDD("Pattern: ");
  // DD(pattern);
  // DDD("Seed: ");
  // DD(seed);

  bool flag = 1;
  uint16_t InintTimer = millis();
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
    if (uint16_t(millis()) - SendTimer > SEND_DELAY)
    {
      SendTimer = millis() + SEND_DELAY * 4;
      IrSender.sendNEC(device_id, STD_COMMANDS::INIT_REQUEST, 3);
    }
    if (IrReceiver.decode())
    {
      received = IrData{IrReceiver.decodedIRData.address, uint8_t(IrReceiver.decodedIRData.command)};
      if (received.address == device_id || received.command == 0)
      {
        goto stop;
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
        device_next = device_id;
        SendDataAdd(device_id, STD_COMMANDS::INIT_ANSWER, 5);
        SendDataAdd(device_id, STD_COMMANDS::INIT_ANSWER, 5);
        SendDataAdd(device_id, STD_COMMANDS::INIT_ANSWER, 5);
        goto stop;
      }
      if (received.address == CARMP3::address)
      {
        if (CARMP3_NUM(received.command) < 10)
        {
          flag = 0;
          device_id = CARMP3_NUM(received.command);
          if (!device_id)
            device_id = 10;
          device_next = device_id;
        }
      }
    stop:
      IrReceiver.resume();
    }
    if (uint16_t(millis() - InintTimer) > INIT_TIME)
    {
      flag = 0;
      device_id = 1;
      device_next = 1;
      DD("No devices found");
      DD_LED(0);
    }
  }
  DDD("Device ID: ");
  DD(device_id);
  DDD("Devices count: ");
  DD(device_next);
  DD_LED(device_id);
  // workerMain = 0;
  randomSeed(analogRead(A0));
  if (device_id <= 10)
    pattern = pread_8t(PATTERNS[pattern_number][device_id - 1]);
  else
    pattern = random(2147483647) % 2;

  if (device_id == 1)
  {
    synchronized = 1;
  }
}

void loop()
{
  // switch (workerMain)
  // {
  // case 0:
  // {
  int8_t digit = -1;
  bool btn_play = 0, btn_next = 0, btn_prev = 0, btn_100 = 0, btn_200 = 0;
  if (IrReceiver.decode())
  {
    received = IrData{IrReceiver.decodedIRData.address, uint8_t(IrReceiver.decodedIRData.command)};
    if (received.address == device_id || received.command == 0)
    {
      goto stop1;
    }
    // SendTimer = uint16_t(millis() - (SEND_DELAY / 10) * (10 - device_id));
    SendTimer = uint16_t(millis()) + device_id;

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
        SendDataAdd(device_id, STD_COMMANDS::INIT_ANSWER, 5);
        SendDataAdd(device_id, STD_COMMANDS::INIT_ANSWER, 5);
        SendDataAdd(device_id, STD_COMMANDS::INIT_ANSWER, 5);
      }
    }
    break;

    case STD_COMMANDS::INIT_REQUEST:
    {
      if (received.address != device_id)
      {
        DD("INIT_REQUEST <-");
        if (!initSent)
          initSender = 1;
      }
    }
    break;

    case STD_COMMANDS::INIT_ANSWER:
    {
      if (received.address == uint8_t(device_id + 1))
      {
        DD("INIT_ANSWER <-");
        if (!initSent)
        {
          device_next++;
        }
        initSent = 1;
        initSender = 0;
        syncRequired = 3;
        DD_LED((device_next));
      }
    }
    break;

    case STD_COMMANDS::SYNC_COMMAND:
    {
      DD("SYNC_COMMAND <-");
      if (received.address == uint16_t(device_id) - 1)
      {
        TMR16(1000, {
          synchronized = millis() - WAWE_FULL / 2;
          if (device_id != device_next)
            syncRequired = 3;
        });
      }
    }
    break;

    case CARMP3::btn_CH_minus: // Change pattern
    {
      if (received.address == CARMP3::address)
      {
        if (millis() - patternChangeTimer <= PATTERN_CHANGE_TIME)
        {
          break;
        }
      }
      else
        break;
    }
    case STD_COMMANDS::CHANGE_PATTERN:
    {
      DD("CHANGE_PATTERN <-");
      // if (received.address < device_id)
      // {
      if (millis() - patternChangeTimer > PATTERN_CHANGE_TIME)
      {
        pattern_number++;
        if (pattern_number < PATTERNS_COUNT && device_id <= 10)
          pattern = pread_8t(PATTERNS[pattern_number][device_id - 1]);
        else
          pattern = random(2147483647) % 2;
        DD("CHANGE_PATTERN ->");
        SendDataAdd(device_id, STD_COMMANDS::CHANGE_PATTERN, 3);
        SendDataAdd(device_id, STD_COMMANDS::CHANGE_PATTERN, 3);
        SendDataAdd(device_id, STD_COMMANDS::CHANGE_PATTERN, 3);
        while (blinkSync(l_led, r_led, mPurple, 1, 200, 1000, 200, 0, 0, 255, DEKAY, 0))
        {
          SendData();
          SHOW_NUM(r_led, l_led, pattern_number, mPurple);
        }
        DDD("Pattern changed: ");
        DD(pattern);
        DDD("Pattern number:");
        DD(pattern_number);
        patternChangeTimer = millis();
      }
      // }
    }
    break;

    case CARMP3::btn_CH: // Show pattern locally
    {
      DD("SHOW_PATTERN_LOCAL <-");
      if (received.address == CARMP3::address)
      {
        while (blinkR(r_led, pattern ? mLime : mRed, SHOW_BLINK_N, SHOW_BLINK_IN, SHOW_BLINK_FULL, SHOW_BLINK_OUT, SHOW_BLINK_MIN, SHOW_BLINK_MIN_BRIGHT, SHOW_BLINK_MAX_BRIGHT, DEKAY, 1))
        {
          DD("SHOW_PATTERN_", 1000);
          blinkL(l_led, pattern ? mRed : mLime, SHOW_BLINK_N, SHOW_BLINK_IN, SHOW_BLINK_FULL, SHOW_BLINK_OUT, SHOW_BLINK_MIN, SHOW_BLINK_MIN_BRIGHT, SHOW_BLINK_MAX_BRIGHT, DEKAY, 1);
        }
      }
    }
    break;

    case CARMP3::btn_CH_plus: // Show pattern global
    {
      if (received.address != CARMP3::address)
        break;
    }
    case STD_COMMANDS::SHOW_PATTERN:
    {
      DD("SHOW_PATTERN_GLOBAL <-");
      if (received.address != device_id || received.address == CARMP3::address)
        TMR16(SHOW_PATTERN_TIMER, {
          DD("SHOW_PATTERN_GLOBAL ->");
          SendDataAdd(device_id, STD_COMMANDS::SHOW_PATTERN, 5);
          SendDataAdd(device_id, STD_COMMANDS::SHOW_PATTERN, 5);
          SendDataAdd(device_id, STD_COMMANDS::SHOW_PATTERN, 5);
          SendDataAdd(device_id, STD_COMMANDS::SHOW_PATTERN, 5);

          while (blinkL(l_led, pattern ? mRed : mLime, SHOW_BLINK_N, SHOW_BLINK_IN, SHOW_BLINK_FULL, SHOW_BLINK_OUT, SHOW_BLINK_MIN, SHOW_BLINK_MIN_BRIGHT, SHOW_BLINK_MAX_BRIGHT, DEKAY, 1))
          {
            blinkR(r_led, pattern ? mLime : mRed, SHOW_BLINK_N, SHOW_BLINK_IN, SHOW_BLINK_FULL, SHOW_BLINK_OUT, SHOW_BLINK_MIN, SHOW_BLINK_MIN_BRIGHT, SHOW_BLINK_MAX_BRIGHT, DEKAY, 1);
            SendData();
          }
        });
    }
    break;

    case CARMP3::btn_PLAY:
    {
      if (received.address == CARMP3::address)
        btn_play = true;
    }
    break;

    case CARMP3::btn_PREV:
    {
      if (received.address == CARMP3::address)
        btn_prev = true;
    }
    break;

    case CARMP3::btn_NEXT:
    {
      if (received.address == CARMP3::address)
        btn_next = true;
    }
    break;

    case CARMP3::btn_100:
    {
      if (received.address == CARMP3::address)
      {
        btn_100 = true;
      }
    }
    break;
    case CARMP3::btn_200:
    {
      if (received.address == CARMP3::address)
        btn_200 = true;
    }
    break;
    default:
      if (received.address == CARMP3::address)
        digit = CARMP3_NUM(received.command);
      break;
    }
  stop1:
    IrReceiver.resume();
  }
  if (initSender)
  {
    // TMR16(SEND_DELAY, {
    if (SendData())
    {
      DD("INIT_COMMAND ->");
      SendDataAdd(device_id, STD_COMMANDS::INIT_COMMAND, 4);
      SendData();
    }
    // });
  }

  if (WAWE_SYNC(r_led, l_led, mAqua, WAWE_IN, WAWE_FULL, WAWE_OUT, WAWE_MIN, WAWE_MIN_BRIGHT, WAWE_MAX_BRIGHT, synchronized, 0) == WAWE_IN + WAWE_FULL / 2)
  {
    if (syncRequired && synchronized && SendData())
    {
      DD("SYNC_COMMAND ->");
      SendDataAdd(device_id, STD_COMMANDS::SYNC_COMMAND);
      SendData();
      syncRequired--;
    }
    // DD("Send Sync Command:");
  }
  SendData();
  if (device_id == 1 && device_id != device_next)
  {
    TMR16(SYNC_TIMER, { syncRequired = 1; });
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
    while (blinkL(l_led, pattern ? mRed : mLime, BLINK_N, BLINK_IN, BLINK_FULL, BLINK_OUT, BLINK_MIN, BLINK_MIN_BRIGHT, BLINK_MAX_BRIGHT, DEKAY, 1))
    {
    }
    // if (device_id > 1)
    //   synchronized = 0;
    r_vib = 0;
  }
  if (r_vib > VIBRATOR_RIGHT_SENS)
  {
    while (blinkR(r_led, pattern ? mLime : mRed, BLINK_N, BLINK_IN, BLINK_FULL, BLINK_OUT, BLINK_MIN, BLINK_MIN_BRIGHT, BLINK_MAX_BRIGHT, DEKAY, 1))
    {
    }
    // if (device_id > 1)
    //   synchronized = 0;
    l_vib = 0;
  }
  // вызов ребят
  if (device_id == device_next)
  {
    static uint16_t btn_play_timer = 0;
    static uint8_t minimal = 0, maximal = 0;
    static uint8_t mode = 3;
    if (btn_100)
      mode = 3;
    if (btn_200)
    {
      minimal = 0;
      maximal = 0;
      mode = 3;
      randomizer.clear();
    }

    if (mode == 0 || mode == 1)
    {
      if ((!minimal && mode == 0) || (!maximal && mode == 1))
      {
        static bool f1 = 0;
        TMR16(1000, {
          f1 = !f1;
        });
        if (f1)
        {
          r_led.set(1, mRed);
          l_led.set(7, mRed);
        }
      }
      else
      {
        l_led.setBrightness(BRIGHTNESS_MAX);
        r_led.setBrightness(BRIGHTNESS_MAX);
        if (mode == 0)
          SHOW_NUM(r_led, l_led, minimal, mRed);
        if (mode == 1)
          SHOW_NUM(r_led, l_led, maximal, mRed);
      }
      if (digit >= 0 && digit < 10)
      {
        TMR16(100, {
          if (mode == 0)
          {
            minimal = (uint16_t(minimal) * 10) % 100 + digit;
          }
          if (mode == 1)
          {
            maximal = (uint16_t(maximal) * 10) % 100 + digit;
          }
        });
      }
      if (btn_play)
      {
        if ((uint16_t)(millis() - btn_play_timer) >= 300)
        {
          btn_play_timer = millis();
          {
            if ((mode == 0 && minimal) || (mode == 1 && maximal))
              ++mode;
            if (mode == 2)
            {
              randomizer.init(minimal, maximal);
            }
          }
        }
      }
    }
    if (mode == 2)
    {
      static uint8_t i = 0;
      if (i < randomizer.PlayersList.size())
      {
        SHOW_NUM(r_led, l_led, randomizer.PlayersList[i], mPurple, mBlack, 0, 1);
        if (btn_next || btn_prev)
        {
          TMR16(100, {
            if (btn_next)
            {
              ++i;
              if (i >= randomizer.PlayersList.size())
              {
                i = 0;
                mode++;
              }
            }
            else if (btn_prev)
            {
              --i;
              if (i >= randomizer.PlayersList.size())
                i = randomizer.PlayersList.size() - 1;
            }
          });
        }
      }
    }
    if (mode == 3)
    {
      if (btn_play)
      {
        btn_play_timer = millis();
        mode = 0;
      }
    }
  }

  l_led.show();
  r_led.show();
  // }
  // break;
  // case 1:
  // {
  //   static uint8_t minimal = 0, maximal = 0;
  //   static uint8_t mode = 0;
  //   while (mode == 0)
  //   {
  //     if (!minimal)
  //     {
  //       TMR16(1000, {
  //         static bool f1 = 0;
  //         f1 = !f1;
  //         r_led.fill(mBlack);
  //         l_led.fill(mBlack);
  //         r_led.show();
  //         l_led.show();
  //         r_led.fill(0, 2, f1 ? mRed : mBlack);
  //         l_led.fill(6, 8, f1 ? mRed : mBlack);
  //         r_led.show();
  //         l_led.show();
  //         r_led.fill(0, 2, f1 ? mRed : mBlack);
  //         l_led.fill(6, 8, f1 ? mRed : mBlack);
  //         r_led.show();
  //         l_led.show();
  //       });
  //     }
  //     else
  //     {
  //       l_led.setBrightness(BRIGHTNESS_MAX);
  //       r_led.setBrightness(BRIGHTNESS_MAX);
  //       SHOW_NUM(r_led, l_led, minimal, mRed);
  //     }
  //     if (IrReceiver.decode())
  //     {
  //       received = IrData{IrReceiver.decodedIRData.address, (uint8_t)IrReceiver.decodedIRData.command};
  //       if (received.address == CARMP3::address)
  //       {
  //         if (received.command == CARMP3::btn_PLAY && minimal > 0)
  //         {
  //           mode = 1;
  //         }
  //         else
  //         {
  //           TMR16(100, {
  //             uint8_t digit = CARMP3_NUM(received.command);
  //             if (digit < 10)
  //             {
  //               minimal = (uint16_t(minimal) * 10) % 100 + digit;
  //             }
  //           });
  //         }
  //       }
  //       IrReceiver.resume();
  //     }
  //   }
  //   while (mode == 1)
  //   {
  //     if (!maximal)
  //     {
  //       TMR16(500, {
  //         static bool f1 = 0;
  //         f1 = !f1;
  //         r_led.fill(mBlack);
  //         l_led.fill(mBlack);
  //         r_led.show();
  //         l_led.show();
  //         r_led.fill(0, 2, f1 ? mRed : mBlack);
  //         l_led.fill(6, 8, f1 ? mRed : mBlack);
  //         r_led.show();
  //         l_led.show();
  //         r_led.fill(0, 2, f1 ? mRed : mBlack);
  //         l_led.fill(6, 8, f1 ? mRed : mBlack);
  //         r_led.show();
  //         l_led.show();
  //       });
  //     }
  //     else
  //     {
  //       SHOW_NUM(r_led, l_led, maximal, mRed);
  //     }
  //     if (IrReceiver.decode())
  //     {
  //       received = IrData{IrReceiver.decodedIRData.address, (uint8_t)IrReceiver.decodedIRData.command};
  //       if (received.address == CARMP3::address)
  //       {
  //         if (received.command == CARMP3::btn_PLAY && maximal > 0)
  //         {
  //           mode = 2;
  //         }
  //         else
  //         {
  //           TMR16(100, {
  //             uint8_t digit = CARMP3_NUM(received.command);
  //             if (digit < 10)
  //             {
  //               maximal = (uint16_t(maximal) * 10) % 100 + digit;
  //             }
  //           });
  //         }
  //       }
  //       IrReceiver.resume();
  //     }
  //   }
  //   while (mode == 2)
  //   {
  //     if (randomizer.init(minimal, maximal))
  //     {
  //       uint8_t i = 0;
  //       while (i < randomizer.PlayersList.size())
  //       {
  //         TMR8(200, { SHOW_NUM(r_led, l_led, randomizer.PlayersList[i], mRed); });
  //
  //         if (IrReceiver.decode())
  //         {
  //           received = IrData{IrReceiver.decodedIRData.address, (uint8_t)IrReceiver.decodedIRData.command};
  //
  //           TMR16(100, {
  //             switch (received.command)
  //             {
  //             case CARMP3::btn_NEXT:
  //               i++;
  //               break;
  //             case CARMP3::btn_PREV:
  //               i--;
  //             default:
  //               break;
  //             }
  //           });
  //           IrReceiver.resume();
  //         }
  //         if (i == 255)
  //           i = 0;
  //       }
  //     }
  //
  //     mode = 0;
  //     workerMain = 0;
  //     minimal = 0;
  //     maximal = 0;
  //     // randomizer.~Randomizer();
  //   }
  // }
  // break;
  // default:
  //   workerMain = 0;
  //   break;
  // }
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
