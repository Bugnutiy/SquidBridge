#pragma once
#include <Arduino.h>
#include "Timer.h"
#include "TransmitterFunc.h"
#include "RemoteCommands.h"
#include "My_Debug.h"
#include <QList.h>
#include <MemoryFree.h>

class Randomizer
{
private:
public:
    QList<uint8_t> PlayersList;
    Randomizer();
    bool init(uint8_t &min, uint8_t &max);
    ~Randomizer();
};

Randomizer::Randomizer()
{
}

bool Randomizer::init(uint8_t &min, uint8_t &max)
{
    randomSeed(analogRead(A0));
    for (uint8_t i = 0; i < max - min; i++)
    {
        bool flag = true;
        uint8_t tmp;
        while (flag)
        {
            tmp = random(min, max + 1);
            flag = false;
            for (uint8_t j = 0; j < PlayersList.size(); j++)
            {
                if (PlayersList[j] == tmp)
                    flag = true;
            }
        }
        if (static_cast<unsigned int>(freeMemory()) > sizeof(PlayersList))
            PlayersList.push_back(tmp);
        else
            return false;
    }
    return true;
}
Randomizer::~Randomizer()
{
    PlayersList.clear();
    PlayersList.~QList();
}
