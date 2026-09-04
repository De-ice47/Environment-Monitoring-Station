#include <ScreenHandlers/allDataScreenHandler.hpp>
#include <Func_Display.hpp>
#include <SystemArt.h>

void DrawAllDataScreen()
{
    SSD1309::display.setFont(u8g2_font_7x14_tf);
    SSD1309::display.drawStr(8, 10, "Environment Data");

    SSD1309::display.drawBitmap(0,16,2,16,Icon_Temperature);
    String temperatureText = String("Temp: ") + GetFormattedTemperature().c_str();
    SSD1309::display.drawStr(17, 29, temperatureText.c_str());

    SSD1309::display.drawBitmap(0,32,2,16,Icon_Pressure);
    String pressureText = String("Pres: ") + GetFormattedPressure().c_str();
    SSD1309::display.drawStr(17, 45, pressureText.c_str());

    SSD1309::display.drawBitmap(0,48,2,16,Icon_Humidity);
    String humidityText = String("Hum: ") + GetFormattedHumidity().c_str();
    SSD1309::display.drawStr(17, 61, humidityText.c_str());
}