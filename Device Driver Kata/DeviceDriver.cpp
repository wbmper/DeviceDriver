#include "DeviceDriver.h"
#include <unordered_map>
#include <vector>

DeviceDriver::DeviceDriver(FlashMemoryDevice* hardware) : m_hardware(hardware)
{
}

int DeviceDriver::read(long address)
{
    std::vector<unsigned char> results;

    for (int readTimes = 0; readTimes < 5; readTimes++) {
        results.push_back(m_hardware->read(address));
    }

    unsigned char value = results[0];
    for (int readTimes = 1; readTimes < 5; readTimes++) {
        if (value != results[readTimes]) {
            throw ReadFailException();
        }
    }
    return value;
}

void DeviceDriver::write(long address, int data)
{
    if (m_hardware->read(address) == 0xFF) {
        m_hardware->write(address, (unsigned char)data);
        return;
    }

    throw WriteFailException();
}