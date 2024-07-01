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
    bool init(uint8_t &minimal, uint8_t &maximal);
    void clear();
    ~Randomizer();
};

Randomizer::Randomizer()
{
}

bool Randomizer::init(uint8_t &minimal, uint8_t &maximal)
{
    randomSeed(analogRead(A0));
    PlayersList.clear();
    for (uint8_t i = 0; i < maximal - minimal + 1; i++)
    {
        bool flag = true;
        uint8_t tmp;
        while (flag)
        {
            tmp = random(minimal, maximal + 1);
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
    DDD("Size of players list:");
    DD(PlayersList.size());
    return true;
}
void Randomizer::clear()
{
    PlayersList.clear();
}
Randomizer::~Randomizer()
{
    PlayersList.clear();
    PlayersList.~QList();
}
