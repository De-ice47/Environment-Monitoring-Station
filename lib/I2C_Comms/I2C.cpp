#include "I2C.h"

namespace I2C
{
    uint8_t pinSDA = -1;
    uint8_t pinSCL = -1;
    uint8_t busNumber = -1;
    uint32_t busFrequency = 100000;
    TwoWire thisWire(0);
    bool wireEnabled = false;

    I2C_Wire::I2C_Wire() : I2C_Wire::I2C_Wire(4, 5, 0, 100000) {}
    I2C_Wire::I2C_Wire(uint8_t pinSDA, uint8_t pinSCL) : I2C_Wire(pinSDA, pinSCL, 0, 100000) {}
    I2C_Wire::I2C_Wire(uint8_t pinSDA, uint8_t pinSCL, uint8_t busNumber) : I2C_Wire(pinSDA, pinSCL, busNumber, 100000) {}
    I2C_Wire::I2C_Wire(uint8_t pinSDA, uint8_t pinSCL, uint8_t busNumber, uint32_t busFrequency) : thisWire(busNumber)
    {
        this->pinSDA = pinSDA;
        this->pinSCL = pinSCL;
        this->busNumber = busNumber;
        this->busFrequency = busFrequency;
    }

    void I2C_Wire::SetPins(int sda, int scl)
    {
        pinSDA = sda;
        pinSCL = scl;
    }
    void I2C_Wire::Start()
    {
        wireEnabled = thisWire.begin(pinSDA, pinSCL, busFrequency);
    }
    void I2C_Wire::Stop()
    {
        wireEnabled = thisWire.end();
    }
    void I2C_Wire::ScanBus()
    {
        bool wirePreviouslyOn = wireEnabled;
        if (!wirePreviouslyOn)
            Start();
        for (uint8_t i = 0x08; i < 0x7f; i++) // only 128 addresses
        {
            thisWire.beginTransmission(i);
            uint8_t resultMessage = thisWire.endTransmission();
            switch (resultMessage)
            {
            case 0:
                LL::Println("[I2C] Found device at 0x" + String(i, HEX) + " (" + String(i) + ")");
                break;
            case 1:
                LL::Println("[I2C] Data too long to fit in transmit buffer for device at 0x" + String(i, HEX) + " (" + String(i) + ")");
                break;
            case 2:
                break;
            case 3:
                LL::Println("[I2C] NACK on data for device at 0x" + String(i, HEX) + " (" + String(i) + ")");
                break;
            case 4:
                LL::Println("[I2C] Other error for device at 0x" + String(i, HEX) + " (" + String(i) + ")");
                break;
            case 5:
                LL::Println("[I2C] Timeout error for device at 0x" + String(i, HEX) + " (" + String(i) + ")");
                break;
            default:
                break;
            }
        }
        if (!wirePreviouslyOn)
            Stop();
    }
    bool I2C_Wire::CallAddress(uint8_t address)
    {
        if (address >= 0x7f || address < 0x08)
        {
            LL::Println("[I2C] Address out of bounds");
            return false;
        }
        thisWire.beginTransmission(address);
        uint8_t result = thisWire.endTransmission();
        if(result == 0) return true;
        else return false;
    }
    void I2C_Wire::ChangeFrequency(uint32_t frequency)
    {
        busFrequency = frequency;
        thisWire.setClock(frequency);
    }
    void I2C_Wire::Write(uint8_t deviceAddress, uint8_t deviceRegister, uint8_t data)
    {
        thisWire.beginTransmission(deviceAddress); // Calling the desired device on the I2C bus
        thisWire.write(deviceRegister);            // Asking to access a certain register on the called device

        thisWire.write(data); // Setting value at register

        thisWire.endTransmission();
    }
    void Write_NoReg(uint8_t deviceAddress, uint8_t data)
    {
        thisWire.beginTransmission(deviceAddress); // Calling the desired device on the I2C bus

        thisWire.write(data); // Setting value at register

        thisWire.endTransmission();
    }
    uint8_t I2C_Wire::Read(uint8_t deviceAddress, uint8_t deviceRegister)
    {
        thisWire.beginTransmission(deviceAddress);
        thisWire.write(deviceRegister);

        if (thisWire.endTransmission(false) != 0)
            return 0;

        if (thisWire.requestFrom(deviceAddress, (uint8_t)1) != 1)
            return 0;

        return thisWire.read();
    }
    void I2C_Wire::ReadMultiple(uint8_t deviceAddress, uint8_t deviceRegister, uint8_t *dataBuffer, uint8_t dataCount)
    {
        thisWire.beginTransmission(deviceAddress);
        thisWire.write(deviceRegister);
        thisWire.endTransmission(false);
        // request from register a certain length of data
        uint8_t bytesReceived = thisWire.requestFrom(deviceAddress, dataCount);

        if (bytesReceived != dataCount)
        {
            // Handle error
            return;
        }
        // Reading each byte as them come
        for (int i = 0; i < dataCount; i++)
            dataBuffer[i] = thisWire.read();
    }
}