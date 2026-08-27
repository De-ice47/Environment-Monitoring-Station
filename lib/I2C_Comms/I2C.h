#pragma once
#include "Arduino.h"
#include "Wire.h"

namespace I2C
{
    class I2C_Wire
    {
    public:
        TwoWire thisWire;

        I2C_Wire();
        I2C_Wire(uint8_t pinSDA, uint8_t pinSCL);
        I2C_Wire(uint8_t pinSDA, uint8_t pinSCL, uint8_t busNumber);
        I2C_Wire(uint8_t pinSDA, uint8_t pinSCL, uint8_t busNumber, uint32_t busFrequency);

        void SetPins(int sda, int scl);
        void Start();
        void Stop();
        void ScanBus();
        void ChangeFrequency(uint32_t frequency);
        void Write(uint8_t deviceAddress, uint8_t deviceRegister, uint8_t data);
        void Write_NoReg(uint8_t deviceAddress, uint8_t data);
        uint8_t Read(uint8_t deviceAddress, uint8_t deviceRegister);
        void ReadMultiple(uint8_t deviceAddress, uint8_t deviceRegister, uint8_t *dataBuffer, uint8_t dataCount);

        bool WireEnabled(){return wireEnabled;}

    private:
        bool wireEnabled;
        
        uint8_t pinSDA;
        uint8_t pinSCL;
        uint8_t busNumber;
        uint32_t busFrequency;
    };
}