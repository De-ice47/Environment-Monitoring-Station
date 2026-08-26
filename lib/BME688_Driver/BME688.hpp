#pragma once
#include "I2C.h"
namespace BME688
{
    class BME688
    {
    public:
        BME688();
        // Config
        void Config(uint8_t pinSDA, uint8_t pinSCL, uint8_t busNumber);
        // Fetching Data
        void Measure();

        float TemperatureFloat_K();
        float TemperatureFloat_F();
        float TemperatureFloat_C();

    private:
        I2C::I2C_Wire myWire;
        uint8_t myAddress;
        bool usingAlternateAddress = false;

        // Helpers
        void Write(uint8_t reg, uint8_t data);
        uint8_t Read(uint8_t reg);
    };
}