#pragma once
#include "FlashMemoryDevice.h"
#include <stdexcept>

class DeviceDriver
{
public:
    DeviceDriver(FlashMemoryDevice* hardware);
    int read(long address);
    void write(long address, int data);

protected:
    FlashMemoryDevice* m_hardware;
};

class Application
{
public:
    Application(DeviceDriver* devicedriver);
    void readAndPrint(long startAddr, long endAddr);
    void writeall(unsigned char data);

protected:
    DeviceDriver* m_devicedriver;
};

class ReadFailException : public std::runtime_error {
public:
    ReadFailException() : std::runtime_error("Read Values are not same") {}
};

class WriteFailException : public std::runtime_error {
public:
    WriteFailException() : std::runtime_error("data is already written") {}
};