#include "I2C.h"
namespace BME688
{
    class Sensor
    {
    public:
        // Config
        void Begin(uint8_t pinSDA, uint8_t pinSCL, uint8_t busNumber);
        void Reset();
        void ConfigureGas(uint16_t targetTemp, uint16_t durationMs);
        // Measureing
        void Measure();
        // Fetching
        double Temperature_K();
        double Temperature_F();
        double Temperature_C();

        double Pressure_Pa();
        double Pressure_kPa();
        double Pressure_bar();
        double Pressure_mmhg();
        double Pressure_inhg();

        double Humidity();

        double Gas();

    private:
        I2C::I2C_Wire myWire;
        uint8_t myAddress;
        bool usingAlternateAddress = false;
        // Computation
        void ComputeTemperature();
        void ComputePressure();
        void ComputeHumidity();
        void ComputeGas();
        // Gas Helpers
        uint8_t CalculateHeaterResistance(uint16_t targetTemp);
        uint8_t CalculateHeaterDuration(uint16_t durationMs);

        // Helpers
        void Write(uint8_t reg, uint8_t data);
        uint8_t Read(uint8_t reg);
    };
}