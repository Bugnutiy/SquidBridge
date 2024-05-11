#pragma once
#include "TransmitterFunc.h"
#include <Arduino.h>

namespace CARMP3
{
    const uint8_t address = 0;
    const uint8_t btn_CH_minus = 69;
    const uint8_t btn_CH = 70;
    const uint8_t btn_CH_plus = 71;
    const uint8_t btn_PREV = 68;
    const uint8_t btn_NEXT = 64;
    const uint8_t btn_PLAY = 67;
    const uint8_t btn_VOL_minus = 7;
    const uint8_t btn_VOL_plus = 21;
    const uint8_t btn_EQ = 9;
    const uint8_t btn_0 = 22;
    const uint8_t btn_1 = 12;
    const uint8_t btn_2 = 24;
    const uint8_t btn_3 = 94;
    const uint8_t btn_4 = 8;
    const uint8_t btn_5 = 28;
    const uint8_t btn_6 = 90;
    const uint8_t btn_7 = 66;
    const uint8_t btn_8 = 82;
    const uint8_t btn_9 = 74;
    const uint8_t btn_100 = 25;
    const uint8_t btn_200 = 13;
}
uint8_t CARMP3_NUM(const uint8_t &COMMAND)
{
    switch (COMMAND)
    {
    case CARMP3::btn_0:
        return 0;
        break;
    case CARMP3::btn_1:
        return 1;
        break;
    case CARMP3::btn_2:
        return 2;
        break;
    case CARMP3::btn_3:
        return 3;
        break;
    case CARMP3::btn_4:
        return 4;
        break;
    case CARMP3::btn_5:
        return 5;
        break;
    case CARMP3::btn_6:
        return 6;
        break;
    case CARMP3::btn_7:
        return 7;
        break;
    case CARMP3::btn_8:
        return 8;
        break;
    case CARMP3::btn_9:
        return 9;
        break;
    case CARMP3::btn_100:
        return 100;
        break;
    case CARMP3::btn_200:
        return 200;
        break;
    default:
        return 255;
        break;
    }
}
namespace STD_COMMANDS
{
    const uint8_t INIT_REQUEST = 1;
    const uint8_t INIT_COMMAND = 2;
    const uint8_t INIT_ANSWER = 3;

    // const uint8_t SYNC_REQUEST = 4;
    const uint8_t SYNC_COMMAND = 5;
    // const uint8_t SYNC_ANSWER = 6;

    const uint8_t CHANGE_PATTERN = 10;
    const uint8_t SHOW_PATTERN = 6;
}

