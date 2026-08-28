#include "BME688.hpp"
#include "BME688_RegisterTable.hpp"
namespace BME688
{
    I2C::I2C_Wire myWire;
    uint8_t myAddress;
    bool AlternateAddress = false;
    // Data Values
    double temperature;
    double pressure;
    double humidity;
    double gasResistence;

    uint16_t gasADC;

    void Sensor::Begin(uint8_t pinSDA, uint8_t pinSCL, uint8_t busNumber)
    {
        LL::Println("Initializing BME688 Sensor...");
        myAddress = AlternateAddress ? 0x76 : 0x77;
        myWire = I2C::I2C_Wire(pinSDA, pinSCL, busNumber);
        myWire.Start();
        ConfigureGas(300, 100);
        LL::Println("BME688 Initialized.");
    }
    void Sensor::Reset()
    {
        Write(RT::RESET, 0xb6);
        myWire.Stop();
        myWire.Start();
    }

    void Sensor::Measure()
    {
        LL::Println("BME688 Measurement Started");
        // -------------------------------------------------
        // 1. Configure gas heater
        // -------------------------------------------------

        uint8_t heater_res = CalculateHeaterResistance(300);
        uint8_t heater_dur = CalculateHeaterDuration(100);

        Write(RT::RES_HEAT_0, heater_res);
        Write(RT::GAS_WAIT_0, heater_dur);

        // CTRL_GAS_0
        // No heater disable / profile changes needed for profile 0.
        uint8_t ctrl_gas_0 = 0x20;

        // CTRL_GAS_1
        // bit 5 = run_gas_h
        // bits 3:0 = heater profile (0)
        uint8_t ctrl_gas_1 = 0x20;

        Write(RT::CTRL_GAS_0, ctrl_gas_0);
        Write(RT::CTRL_GAS_1, ctrl_gas_1);

        // -------------------------------------------------
        // 2. Configure humidity
        // -------------------------------------------------

        uint8_t ctrl_hum = 0;

        // os_hum = 5 -> 16x oversampling
        ctrl_hum |= (5 << 0);

        Write(RT::CTRL_HUM, ctrl_hum);

        // -------------------------------------------------
        // 3. Configure temperature + pressure
        // -------------------------------------------------

        uint8_t ctrl_meas = 0;

        // os_temp = 2 -> 2x oversampling
        ctrl_meas |= (2 << 5);

        // os_pres = 1 -> 1x oversampling
        ctrl_meas |= (1 << 2);

        // mode = 01 -> forced mode
        ctrl_meas |= 0x01;

        // -------------------------------------------------
        // 5. Start measurement LAST
        // -------------------------------------------------

        Write(RT::CTRL_MEAS, ctrl_meas);

        // -------------------------------------------------
        // 6. Wait for measurement to complete
        // -------------------------------------------------

        delay(300);

        // -------------------------------------------------
        // 7. Read status
        // -------------------------------------------------

        uint8_t status = Read(RT::MEAS_STATUS_0);
        uint8_t variant = Read(RT::VARIANT_ID);

        LL::Println("BME688 Measurement Complete");
    }

    // Fetching (public calls)
    // Temperature Data
    double Sensor::Temperature_C()
    {
        ComputeTemperature();
        return temperature;
    }
    double Sensor::Temperature_K()
    {
        return Temperature_C() + 273.15f;
    }
    double Sensor::Temperature_F()
    {
        return Temperature_C() * 1.8 + 32;
    }
    // Pressure Data
    double Sensor::Pressure_Pa()
    {
        ComputePressure();
        return pressure;
    }
    double Sensor::Pressure_kPa()
    {
        return Pressure_Pa() * .001f;
    }
    double Sensor::Pressure_bar()
    {
        return Pressure_Pa() * .00001f;
    }
    double Sensor::Pressure_mmhg()
    {
        return Pressure_Pa() * 0.007500638f;
    }
    double Sensor::Pressure_inhg()
    {
        return Pressure_Pa() * 0.00029529983f;
    }
    // Humidity
    double Sensor::Humidity()
    {
        ComputeHumidity();
        return humidity;
    }
    // Gas
    double Sensor::Gas()
    {
        ComputeGas();
        return gasResistence;
    }
    uint16_t Sensor::GasADC(){
        return gasADC;
    }
    // Computation

    // Computation
    double t_fine;
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
        double var1 = (((double)temp_adc / 16384.0) - ((double)par_t1 / 1024.0)) * (double)par_t2;
        double var2 = ((((double)temp_adc / 131072.0) - ((double)par_t1 / 8192.0)) *
                       (((double)temp_adc / 131072.0) - ((double)par_t1 / 8192.0))) *
                      ((double)par_t3 * 16.0);
        t_fine = var1 + var2;
        temperature = t_fine / 5120.0;
    }
    void Sensor::ComputePressure()
    {
        // Pulling Data from Registers
        uint32_t press_adc =
            ((uint32_t)Read(RT::PRESS_MSB_0) << 12) |
            ((uint32_t)Read(RT::PRESS_LSB_0) << 4) |
            ((uint32_t)Read(RT::PRESS_XLSB_0) >> 4);
        uint16_t par_p1 = ((uint16_t)Read(RT::PAR_P1_MSB) << 8) | Read(RT::PAR_P1_LSB);
        int16_t par_p2 = (int16_t)((uint16_t)Read(RT::PAR_P2_MSB) << 8) | Read(RT::PAR_P2_LSB);
        int8_t par_p3 = Read(RT::PAR_P3);
        int16_t par_p4 = (int16_t)((uint16_t)Read(RT::PAR_P4_MSB) << 8) | Read(RT::PAR_P4_LSB);
        int16_t par_p5 = (int16_t)((uint16_t)Read(RT::PAR_P5_MSB) << 8) | Read(RT::PAR_P5_LSB);
        int8_t par_p6 = Read(RT::PAR_P6);
        int8_t par_p7 = Read(RT::PAR_P7);
        int16_t par_p8 = (int16_t)((uint16_t)Read(RT::PAR_P8_MSB) << 8) | Read(RT::PAR_P8_LSB);
        int16_t par_p9 = (int16_t)((uint16_t)Read(RT::PAR_P9_MSB) << 8) | Read(RT::PAR_P9_LSB);
        uint8_t par_p10 = Read(RT::PAR_P10);
        // Equations
        double var1 = ((double)t_fine / 2.0) - 64000.0;
        double var2 = var1 * var1 * ((double)par_p6 / 131072.0);
        var2 = var2 + (var1 * (double)par_p5 * 2.0);
        var2 = (var2 / 4.0) + ((double)par_p4 * 65536.0);
        var1 = ((((double)par_p3 * var1 * var1) / 16384.0) +
                ((double)par_p2 * var1)) /
               524288.0;
        var1 = (1.0 + (var1 / 32768.0)) * (double)par_p1;
        double press_comp = 1048576.0 - (double)press_adc;
        press_comp = ((press_comp - (var2 / 4096.0)) * 6250.0) / var1;
        var1 = ((double)par_p9 * press_comp * press_comp) / 2147483648.0;
        var2 = press_comp * ((double)par_p8 / 32768.0);
        double var3 = (press_comp / 256.0) * (press_comp / 256.0) *
                      (press_comp / 256.0) * (par_p10 / 131072.0);
        press_comp = press_comp + (var1 + var2 + var3 +
                                   ((double)par_p7 * 128.0)) /
                                      16.0;
        pressure = press_comp;
    }
    void Sensor::ComputeHumidity()
    {
        ComputeTemperature();
        // Pulling Data from Registers
        uint16_t hum_adc = ((uint16_t)Read(RT::HUM_MSB_0) << 8) | Read(RT::HUM_LSB_0);
        uint8_t e1 = Read(RT::PAR_H2_MSB);
        uint8_t e2 = Read(RT::PAR_H2_LSB);
        uint8_t e3 = Read(RT::PAR_H1_MSB);
        uint16_t par_h1 =
            ((uint16_t)e3 << 4) |
            (e2 & 0x0F);

        uint16_t par_h2 =
            ((uint16_t)e1 << 4) |
            (e2 >> 4);

        int8_t par_h3 = Read(RT::PAR_H3);
        int8_t par_h4 = Read(RT::PAR_H4);
        int8_t par_h5 = Read(RT::PAR_H5);
        uint8_t par_h6 = Read(RT::PAR_H6);
        int8_t par_h7 = Read(RT::PAR_H7);
        // Equations
        double var1 = hum_adc - (((double)par_h1 * 16.0) + (((double)par_h3 / 2.0) * temperature));
        double var2 = var1 * (((double)par_h2 / 262144.0) * (1.0 + (((double)par_h4 / 16384.0) * temperature) + (((double)par_h5 / 1048576.0) * temperature * temperature)));
        double var3 = (double)par_h6 / 16384.0;
        double var4 = (double)par_h7 / 2097152.0;
        humidity = var2 + ((var3 + (var4 * temperature)) * var2 * var2);
    }
    void Sensor::ComputeGas()
    {
        uint8_t gas_msb = Read(RT::GAS_RESIST_MSB_0);
        uint8_t gas_lsb = Read(RT::GAS_RANGE_RESIST_LSB_0);
        uint16_t gas_adc = ((uint16_t)Read(RT::GAS_RESIST_MSB_0) << 2) |
                           ((Read(RT::GAS_RANGE_RESIST_LSB_0) >> 6) & 0x03);
        uint8_t gas_range = Read(RT::GAS_RANGE_RESIST_LSB_0) & 0x0F;
        // Equations
        uint32_t var1 = UINT32_C(262144) >> gas_range;
        int32_t var2 = (int32_t)gas_adc - INT32_C(512);
        var2 *= INT32_C(3);
        var2 = INT32_C(4096) + var2;
        gasResistence = 1000000.0f * (float)var1 / (float)var2;
        gasADC = gas_adc;
    }

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
    uint16_t target_temp;
    void Sensor::ConfigureGas(uint16_t targetTemp, uint16_t durationMs)
    {
        // Configure heater temperature and duration here.
        //
        // These values will eventually be calculated using
        // the BME688 heater calibration coefficients.

        uint8_t heater_resistance = CalculateHeaterResistance(targetTemp);
        uint8_t heater_duration = CalculateHeaterDuration(durationMs);
        target_temp = target_temp;

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
        Write(RT::CTRL_GAS_0, 0x00);
    }
    uint8_t Sensor::CalculateHeaterResistance(uint16_t targetTemp)
    {
        int16_t amb_temp = temperature;
        int8_t par_g1 =
            (int8_t)Read(RT::PAR_G1);

        int16_t par_g2 =
            (int16_t)(((uint16_t)Read(RT::PAR_G2_MSB) << 8) |
                      Read(RT::PAR_G2_LSB));

        int8_t par_g3 =
            (int8_t)Read(RT::PAR_G3);

        uint8_t res_heat_range =
            (Read(RT::RES_HEAT_RANGE) >> 4) & 0x03;

        int8_t res_heat_val =
            (int8_t)Read(RT::RES_HEAT_VAL);

        double var1 =
            ((double)par_g1 / 16.0) + 49.0;

        double var2 =
            (((double)par_g2 / 32768.0) * 0.0005) + 0.00235;

        double var3 =
            (double)par_g3 / 1024.0;

        double var4 =
            var1 * (1.0 + (var2 * (double)targetTemp));

        double var5 =
            var4 + (var3 * (double)amb_temp);

        uint8_t res_heat_x =
            (uint8_t)(3.4 *
                      ((
                           var5 *
                           (4.0 / (4.0 + (double)res_heat_range)) *
                           (1.0 / (1.0 + ((double)res_heat_val * 0.002)))) -
                       25.0));
        return res_heat_x;
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