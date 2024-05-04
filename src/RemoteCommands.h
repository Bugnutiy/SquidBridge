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

namespace STD_COMMANDS
{
    const uint8_t INIT_REQUEST = 1;
    const uint8_t INIT_COMMAND = 2;
    const uint8_t INIT_ANSWER = 3;
    const uint8_t SYNC_COMMAND = 4;
    const uint8_t CHANGE_PATTERN = 5;
}

// class LGM
// {
// public:
//     const IrData btn_CH_minus = IrData{0, 69};
//     const IrData btn_CH = IrData{0, 70};
//     const IrData btn_CH_plus = IrData{0, 71};
//     const IrData btn_PREV = IrData{0, 68};
//     const IrData btn_NEXT = IrData{0, 64};
//     const IrData btn_PLAY = IrData{0, 67};
//     const IrData btn_VOL_minus = IrData{0, 7};
//     const IrData btn_VOL_plus = IrData{0, 21};
//     const IrData btn_EQ = IrData{0, 9};
//     const IrData btn_0 = IrData{4, 16};
//     const IrData btn_1 = IrData{4, 17};
//     const IrData btn_2 = IrData{4, 18};
//     const IrData btn_3 = IrData{4, 19};
//     const IrData btn_4 = IrData{4, 20};
//     const IrData btn_5 = IrData{4, 21};
//     const IrData btn_6 = IrData{4, 22};
//     const IrData btn_7 = IrData{4, 23};
//     const IrData btn_8 = IrData{4, 24};
//     const IrData btn_9 = IrData{4, 25};
//     const IrData btn_100 = IrData{0, 25};
//     const IrData btn_200 = IrData{0, 13};
// };
// LGM LG_REMOTE;
