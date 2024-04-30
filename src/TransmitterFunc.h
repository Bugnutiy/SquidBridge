#pragma once
#include <Arduino.h>
#include <QList.h>
#include <IRremote.hpp>
#include <MemoryFree.h>

#define SEND_DELAY 200

#pragma pack(push, 1)

/**
 * @brief Structure to hold address and command data for IR data.
 *
 * This structure is used to store the address and command data for IR data.
 * The address and command are stored as uint16_t and uint8_t respectively.
 * The operator== function is used to compare two IrData objects for equality.
 */
struct IrData {
    /**
     * @brief Address of the IR data.
     *
     * This is a 16-bit unsigned integer representing the address of the IR data.
     */
    uint16_t address;

    /**
     * @brief Command of the IR data.
     *
     * This is an 8-bit unsigned integer representing the command of the IR data.
     */
    uint8_t command;

    /**
     * @brief Compare two IrData objects for equality.
     *
     * This function compares two IrData objects for equality based on their address and command.
     * It returns true if the address and command of the two objects are the same, and false otherwise.
     *
     * @param[in] other The other IrData object to compare with.
     *
     * @return true if the address and command of the two objects are the same, and false otherwise.
     */
    bool operator==(const IrData& other) const {
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

uint16_t SendTimer = 0;
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