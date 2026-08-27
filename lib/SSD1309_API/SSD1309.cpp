#include <SSD1309.hpp>

namespace SSD1309
{
    U8G2_SSD1309_128X64_NONAME2_F_SW_I2C display(
        U8G2_R0,
        6,              // SCL
        7,              // SDA
        U8X8_PIN_NONE   // RESET
    );

    void Begin()
    {
        Serial.println("Starting U8g2...");

        display.setI2CAddress(0x3C * 2);

        display.begin();

        Serial.println("U8g2 started!");
    }
    
}