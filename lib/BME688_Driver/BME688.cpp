#include "BME688.hpp"
namespace BME688
{
    I2C::I2C_Wire myWire;
    uint8_t myAddress;
    uint8_t AlternateAddress = false;

    // Data Measured
    float temperature;

    BME688::BME688() {}

    void BME688::Config(uint8_t pinSDA, uint8_t pinSCL, uint8_t busNumber)
    {
        myAddress = AlternateAddress ? 0x76 : 0x77;
        myWire = I2C::I2C_Wire(pinSDA, pinSCL, busNumber);
        myWire.Start();
    }
    void BME688::Measure()
    {
    }

    // Temperature Data
    float BME688::TemperatureFloat_C()
    {
        return temperature;
    }
    float TemperatureFloat_K()
    {
        return temperature + 273.15f;
    }
    float TemperatureFloat_F()
    {
        return temperature * 1.8 + 32;
    }

    // Helpers
    void Write(uint8_t reg, uint8_t data)
    {
        myWire.Write(myAddress, reg, data);
    }
    uint8_t Read(uint8_t reg)
    {
        myWire.Read(myAddress, reg);
    }
}