#pragma once
#include <Arduino.h>
#include <QList.h>
#include <IRremote.hpp>
#include <MemoryFree.h>

#define SEND_DELAY 20

#pragma pack(push, 1)
struct IrData
{
    uint16_t address;
    uint8_t command;

    bool operator==(const IrData& other) const
    {
        return address == other.address && command == other.command;
    }
};
#pragma pack(pop)

QList<IrData> SendDataQueue;

void SendDataAdd(uint16_t address, uint8_t command)
{
    if (static_cast<unsigned int>(freeMemory()) > sizeof(SendDataQueue))
        SendDataQueue.push_back(IrData{address, command});
}

void SendDataAddFront(uint16_t address, uint8_t command)
{
    SendDataQueue.push_front(IrData{address, command});
}

uint8_t SendTimer = 0;
bool SendData()
{
    if (SendDataQueue.size() == 0)
        return false;
    if (uint8_t(millis() - SendTimer) > SEND_DELAY)
    {
        SendTimer = millis();
        IrSender.sendNEC(SendDataQueue.front().address, SendDataQueue.front().command, 3);
        SendDataQueue.pop_front();
        return true;
    }
    return false;
}