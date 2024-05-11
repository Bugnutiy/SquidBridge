#pragma once
#include <Arduino.h>
#include <QList.h>
#include <IRremote.hpp>
#include <MemoryFree.h>

#define SEND_DELAY 800
#define SEND_DELAY_2 SEND_DELAY * 2

#pragma pack(push, 1)

/**
 * @brief Structure to hold address and command data for IR data.
 *
 * This structure is used to store the address and command data for IR data.
 * The address and command are stored as uint16_t and uint8_t respectively.
 * The operator== function is used to compare two IrData objects for equality.
 */
struct IrData
{
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
    bool operator==(const IrData &other) const
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
    if (static_cast<unsigned int>(freeMemory()) > sizeof(SendDataQueue))
        SendDataQueue.push_front(IrData{address, command});
}

uint16_t SendTimer = 0;
/**
 * @brief Function to send data using the IR protocol.
 * @return true when data can be sent immediately, false otherwise
 */
bool SendData(uint8_t k = 1)
{
    if (uint16_t(millis() - SendTimer) > SEND_DELAY)
    {
        if (SendDataQueue.size() < 1)
        {
            return true;
        }
        SendTimer = millis();
        DDD("Sending: {");
        DDD(SendDataQueue.front().address);
        DDD(",");
        DDD(SendDataQueue.front().command);
        DD("}");
        IrSender.sendNEC(SendDataQueue.front().address, SendDataQueue.front().command, k);
        SendDataQueue.pop_front();
    }
    return false;
}