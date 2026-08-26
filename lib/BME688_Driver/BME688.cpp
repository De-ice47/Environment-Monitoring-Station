#include "BME688.hpp"
#include "BME688_RegisterTable.hpp"
namespace BME688
{
    I2C::I2C_Wire myWire;
    uint8_t myAddress;
    bool AlternateAddress = false;
    // Data Values
    float temperatureF;
    int temperature;
    float pressureF;
    int pressure;
    float humidityF;
    int humidity;
    float gasF;
    int gas;

    void Sensor::Begin(uint8_t pinSDA, uint8_t pinSCL, uint8_t busNumber)
    {
        myAddress = AlternateAddress ? 0x76 : 0x77;
        myWire = I2C::I2C_Wire(pinSDA, pinSCL, busNumber);
        myWire.Start();
        ConfigureGas(300, 100);
    }
    void Sensor::Reset()
    {
        Write(RT::RESET, 0xb6);
        myWire.Stop();
        myWire.Start();
    }

    void Sensor::Measure()
    {
        uint8_t ctrl_hum = 0;

        ctrl_hum |= (5 << 0);

        Write(RT::CTRL_HUM, ctrl_hum);

        uint8_t ctrl_meas = 0;

        ctrl_meas |= (2 << 5); // temperature ×2
        ctrl_meas |= (1 << 2); // pressure ×1
        ctrl_meas |= 0x01;     // forced mode

        Write(RT::CTRL_MEAS, ctrl_meas);

        while (Read(RT::MEAS_STATUS_0) & (1 << 5))
        {
            delayMicroseconds(100);
        }
    }

    // Fetching (public calls)
    // Temperature Data
    float Sensor::TemperatureFloat_C()
    {
        return temperatureF;
    }
    float Sensor::TemperatureFloat_K()
    {
        return temperatureF + 273.15f;
    }
    float Sensor::TemperatureFloat_F()
    {
        return temperatureF * 1.8 + 32;
    }

    // Computation

    // Computation
    void Sensor::ComputeTemperature()
    {
        // Pulling Data from Registers
        uint32_t temp_adc =
            ((uint32_t)Read(RT::TEMP_MSB_0) << 12) |
            (Read(RT::TEMP_LSB_0) << 4) |
            (Read(RT::TEMP_XLSB_0) >> 4);
        uint16_t par_t1 = ((uint16_t)Read(RT::PAR_T1_MSB) << 8) | Read(RT::PAR_T1_LSB);
        int16_t par_t2 = ((int16_t)(uint16_t)Read(RT::PAR_T2_MSB) << 8) | Read(RT::PAR_T2_LSB);
        int8_t par_t3 = (int8_t)Read(RT::PAR_T3);
        // Equations
        int32_t var1 = ((int32_t)temp_adc >> 3) - ((int32_t)par_t1 << 1);
        int32_t var2 = (var1 * (int32_t)par_t2) >> 11;
        int32_t var3 = ((((var1 >> 1) * (var1 >> 1)) >> 12) * ((int32_t)par_t3 << 4)) >> 14;
        int32_t t_fine = var2 + var3;
        temperature = ((t_fine * 5) + 128) >> 8;
    }
    void Sensor::ComputePressure() {}
    void Sensor::ComputeHumidity() {}
    void Sensor::ComputeGas() {}
    void Sensor::ComputeTemperatureF()
    {
        // Pulling Data from Registers
        uint32_t temp_adc =
            ((uint32_t)Read(RT::TEMP_MSB_0) << 12) |
            (Read(RT::TEMP_LSB_0) << 4) |
            (Read(RT::TEMP_XLSB_0) >> 4);
        uint16_t par_t1 = ((uint16_t)Read(RT::PAR_T1_MSB) << 8) | Read(RT::PAR_T1_LSB);
        int16_t par_t2 = ((int16_t)(uint16_t)Read(RT::PAR_T2_MSB) << 8) | Read(RT::PAR_T2_LSB);
        int8_t par_t3 = (int8_t)Read(RT::PAR_T3);
        // Equations
        int32_t var1 = (((double)temp_adc / 16384.0) - ((double)par_t1 / 1024.0)) * (double)par_t2;
        int32_t var2 = ((((double)temp_adc / 131072.0) - ((double)par_t1 / 8192.0)) *
                (((double)temp_adc / 131072.0) - ((double)par_t1 / 8192.0))) *
               ((double)par_t3 * 16.0);
        int32_t t_fine = var1 + var2;
        temperatureF = t_fine / 5120.0;
    }
    void Sensor::ComputePressureF() {}
    void Sensor::ComputeHumidityF() {}
    void Sensor::ComputeGasF() {}

    // Helpers
    void Sensor::Write(uint8_t reg, uint8_t data)
    {
        myWire.Write(myAddress, reg, data);
    }
    uint8_t Sensor::Read(uint8_t reg)
    {
        return myWire.Read(myAddress, reg);
    }
    // Gas Helpers
    void Sensor::ConfigureGas(uint16_t targetTemp, uint16_t durationMs)
    {
        // Configure heater temperature and duration here.
        //
        // These values will eventually be calculated using
        // the BME688 heater calibration coefficients.

        uint8_t heater_resistance = CalculateHeaterResistance(targetTemp);
        uint8_t heater_duration = CalculateHeaterDuration(durationMs);

        Write(RT::RES_HEAT_0, heater_resistance);
        Write(RT::GAS_WAIT_0, heater_duration);

        // Enable gas measurement
        //
        // Bit 4 = run_gas
        // Bits 3:0 = heater profile
        uint8_t ctrl_gas_1 = 0;

        ctrl_gas_1 |= (1 << 4); // run_gas
        ctrl_gas_1 |= 0;        // heater profile 0

        Write(RT::CTRL_GAS_1, ctrl_gas_1);
    }
    uint8_t Sensor::CalculateHeaterResistance(uint16_t targetTemp)
    {
        int32_t var1;
        int32_t var2;
        int32_t var3;
        int32_t var4;
        int32_t var5;
        int32_t heatr_res_x100;

        int8_t par_g1 = Read(RT::PAR_G1);
        int16_t par_g2 = (Read(RT::PAR_G2_MSB) << 8) | Read(RT::PAR_G2_LSB);
        int8_t par_g3 = Read(RT::PAR_G3);
        int8_t res_heat_range = Read(RT::RES_HEAT_RANGE);
        int8_t res_heat_val = Read(RT::RES_HEAT_VAL);

        var1 = ((int32_t)par_g1 * 5) + 275;
        var2 = ((int32_t)par_g2 * 100);

        var3 = ((int32_t)par_g3 * 100);
        var4 = ((int32_t)res_heat_range + 4) * 100;

        var5 = ((int32_t)res_heat_val * 34);

        heatr_res_x100 =
            (((((int32_t)targetTemp * 100) - var1) * var2) / 1000) + var3 + var4 + var5;

        heatr_res_x100 /= 100;

        if (heatr_res_x100 < 0)
            heatr_res_x100 = 0;

        if (heatr_res_x100 > 255)
            heatr_res_x100 = 255;

        return (uint8_t)heatr_res_x100;
    }
    uint8_t Sensor::CalculateHeaterDuration(uint16_t durationMs)
    {
        uint8_t factor = 0;
        uint8_t duration = durationMs;

        while (duration > 63 && factor < 3)
        {
            duration /= 4;
            factor++;
        }

        return (factor << 6) | duration;
    }
}