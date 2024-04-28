#pragma once
#include "TransmitterFunc.h"
#include <Arduino.h>

class CAR_MP3_REMOTE
{
public:
    const IrData btn_CH_minus = IrData{0, 69};
    const IrData btn_CH = IrData{0, 70};
    const IrData btn_CH_plus = IrData{0, 71};
    const IrData btn_PREV = IrData{0, 68};
    const IrData btn_NEXT = IrData{0, 64};
    const IrData btn_PLAY = IrData{0, 67};
    const IrData btn_VOL_minus = IrData{0, 7};
    const IrData btn_VOL_plus = IrData{0, 21};
    const IrData btn_EQ = IrData{0, 9};
    const IrData btn_0 = IrData{0, 22};
    const IrData btn_1 = IrData{0, 12};
    const IrData btn_2 = IrData{0, 24};
    const IrData btn_3 = IrData{0, 94};
    const IrData btn_4 = IrData{0, 8};
    const IrData btn_5 = IrData{0, 28};
    const IrData btn_6 = IrData{0, 90};
    const IrData btn_7 = IrData{0, 66};
    const IrData btn_8 = IrData{0, 82};
    const IrData btn_9 = IrData{0, 74};
    const IrData btn_100 = IrData{0, 25};
    const IrData btn_200 = IrData{0, 13};
};
CAR_MP3_REMOTE CARMP3;

class LG_REMOTE
{
public:
    const IrData btn_CH_minus = IrData{0, 69};
    const IrData btn_CH = IrData{0, 70};
    const IrData btn_CH_plus = IrData{0, 71};
    const IrData btn_PREV = IrData{0, 68};
    const IrData btn_NEXT = IrData{0, 64};
    const IrData btn_PLAY = IrData{0, 67};
    const IrData btn_VOL_minus = IrData{0, 7};
    const IrData btn_VOL_plus = IrData{0, 21};
    const IrData btn_EQ = IrData{0, 9};
    const IrData btn_0 = IrData{4, 16};
    const IrData btn_1 = IrData{4, 17};
    const IrData btn_2 = IrData{4, 18};
    const IrData btn_3 = IrData{4, 19};
    const IrData btn_4 = IrData{4, 20};
    const IrData btn_5 = IrData{4, 21};
    const IrData btn_6 = IrData{4, 22};
    const IrData btn_7 = IrData{4, 23};
    const IrData btn_8 = IrData{4, 24};
    const IrData btn_9 = IrData{4, 25};
    const IrData btn_100 = IrData{0, 25};
    const IrData btn_200 = IrData{0, 13};
};
LG_REMOTE LGM;