#include "DeviceDriver.h"
#include <vector>
#include <iostream>

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

Application::Application(DeviceDriver* devicedriver) : m_devicedriver(devicedriver)
{
}

void Application::readAndPrint(long startAddr, long endAddr)
{
    std::vector<long> data;

    for (long addr = startAddr; addr <= endAddr; addr++) {
        data.push_back(m_devicedriver->read(addr));
    }

    for (long value : data) {
        std::cout << value;
    }
    std::cout << "\n";
}

void Application::writeall(unsigned char data)
{
    const long minAddr = 0x00;
    const long maxAddr = 0x04;
    for (long addr = minAddr; addr <= maxAddr; addr++) {
        m_devicedriver->write(addr, data);
    }
}
