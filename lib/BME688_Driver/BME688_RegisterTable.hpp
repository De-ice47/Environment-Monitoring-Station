#include "BME688_Source.h"

namespace BME688
{
    namespace RT
    {
        // Identification
        constexpr uint8_t VARIANT_ID = 0xf0;
        constexpr uint8_t CHIP_ID = 0xd0;

        // General Control
        constexpr uint8_t RESET = 0xe0;
        constexpr uint8_t STATUS = 0x73;

        // Measurement Configuration
        constexpr uint8_t CONFIG = 0x75;

        constexpr uint8_t CTRL_MEAS = 0x74;
        constexpr uint8_t CTRL_HUM = 0x72;

        constexpr uint8_t CTRL_GAS_1 = 0x71;
        constexpr uint8_t CTRL_GAS_2 = 0x70;

        constexpr uint8_t GAS_WAIT_SHARED = 0x6e;
        constexpr uint8_t GAS_WAIT_0 = 0x64;
        constexpr uint8_t RES_HEAT_0 = 0x5a;
        constexpr uint8_t IDAC_HEAT_0 = 0x50; 

        // Output values
        // Parallel Measure 2
        constexpr uint8_t GAS_RANGE_RESIST_LSB_2 = 0x4f;
        constexpr uint8_t GAS_RESIST_MSB_2 = 0x4e;

        constexpr uint8_t HUM_LSB_2 = 0x48;
        constexpr uint8_t HUM_MSB_2 = 0x47;

        constexpr uint8_t TEMP_XLSB_2 = 0x46;
        constexpr uint8_t TEMP_LSB_2 = 0x45;
        constexpr uint8_t TEMP_HSB_2 = 0x44;

        constexpr uint8_t PRESS_XLSB_2 = 0x43;
        constexpr uint8_t PRESS_LSB_2 = 0x42;
        constexpr uint8_t PRESS_MSB_2 = 0x41;

        constexpr uint8_t SUB_MEAS_INDEX_2 = 0x40;
        constexpr uint8_t MEAS_STATUS_2 = 0x3f;
        // Parallel Measure 1
        constexpr uint8_t GAS_RANGE_RESIST_LSB_1 = 0x3e;
        constexpr uint8_t GAS_RESIST_MSB_1 = 0x3d;

        constexpr uint8_t HUM_LSB_1 = 0x37;
        constexpr uint8_t HUM_MSB_1 = 0x36;

        constexpr uint8_t TEMP_XLSB_1 = 0x35;
        constexpr uint8_t TEMP_LSB_1 = 0x34;
        constexpr uint8_t TEMP_MSB_1 = 0x33;

        constexpr uint8_t PRESS_XLSB_1 = 0x32;
        constexpr uint8_t PRESS_LSB_1 = 0x31;
        constexpr uint8_t PRESS_MSB_1 = 0x30;
        
        constexpr uint8_t SUB_MEAS_INDEX_1 = 0x2f;
        constexpr uint8_t MEAS_STATUS_1 = 0x2e;
        // Parallel Measure 0
        constexpr uint8_t GAS_RANGE_RESIST_LSB_0 = 0x2d;
        constexpr uint8_t GAS_RESIST_MSB_0 = 0x2c;

        constexpr uint8_t HUM_LSB_0 = 0x26;
        constexpr uint8_t HUM_MSB_0 = 0x25;

        constexpr uint8_t TEMP_XLSB_0 = 0x24;
        constexpr uint8_t TEMP_LSB_0 = 0x23;
        constexpr uint8_t TEMP_MSB_0 = 0x22;

        constexpr uint8_t PRESS_XLSB_0 = 0x21;
        constexpr uint8_t PRESS_LSB_0 = 0x20;
        constexpr uint8_t PRESS_MSB_0 = 0x1f;
        
        constexpr uint8_t SUB_MEAS_INDEX_0 = 0x1e;
        constexpr uint8_t MEAS_STATUS_0 = 0x1d;
        // Temperature Computation Values
        constexpr uint8_t PAR_T1_LSB = 0xe9;
        constexpr uint8_t PAR_T1_MSB = 0xea;
        constexpr uint8_t PAR_T2_LSB = 0x8a;
        constexpr uint8_t PAR_T2_MSB = 0x8b;
        constexpr uint8_t PAR_T3 = 0x8c;
        // Pressure Computation Values
        constexpr uint8_t PAR_P1_LSB = 0x8e;
        constexpr uint8_t PAR_P1_MSB = 0x8f;
        constexpr uint8_t PAR_P2_LSB = 0x90;
        constexpr uint8_t PAR_P2_MSB = 0x91;
        constexpr uint8_t PAR_P3 = 0x92;
        constexpr uint8_t PAR_P4_LSB = 0x94;
        constexpr uint8_t PAR_P4_MSB = 0x95;
        constexpr uint8_t PAR_P5_LSB = 0x96;
        constexpr uint8_t PAR_P5_MSB = 0x97;
        constexpr uint8_t PAR_P6 = 0x99;
        constexpr uint8_t PAR_P7 = 0x98;
        constexpr uint8_t PAR_P8_LSB = 0x9c;
        constexpr uint8_t PAR_P8_MSB = 0x9d;
        constexpr uint8_t PAR_P9_LSB = 0x9e;
        constexpr uint8_t PAR_P9_MSB = 0x9f;
        constexpr uint8_t PAR_P10 = 0xA0;
        // Humidity Computation Values
        constexpr uint8_t PAR_H1_LSB = 0xe2; // bits 3 to 0
        constexpr uint8_t PAR_H1_MSB = 0xe3; // bits 7 to 4
        constexpr uint8_t PAR_H2_LSB = 0xe2;
        constexpr uint8_t PAR_H2_MSB = 0xe1;
        constexpr uint8_t PAR_H3 = 0xe4;
        constexpr uint8_t PAR_H4 = 0xe5;
        constexpr uint8_t PAR_H5 = 0xe6;
        constexpr uint8_t PAR_H6 = 0xe7;
        constexpr uint8_t PAR_H7 = 0xe8;
        // Gas Computation Values
        constexpr uint8_t PAR_G1 =0xed;
        constexpr uint8_t PAR_G2_LSB = 0xeb;
        constexpr uint8_t PAR_G2_MSB = 0xec;
        constexpr uint8_t PAR_G3 = 0xee;
        constexpr uint8_t RES_HEAT_RANGE = 0x02; // bits 5 and 4
        constexpr uint8_t RES_HEAT_VAL = 0x00;
    }
}