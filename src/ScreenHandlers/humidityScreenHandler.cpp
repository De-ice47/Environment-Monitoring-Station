#include <ScreenHandlers/humidityScreenHandler.hpp>

double currentHum;
double highHum;
double lowHum = 100;
double deltaHum;
 
double lastReadHum = 0;
RollingBuffer<double, 1200> deltaHumStack;

void FeedHumidity(double humidityValue)
{
    currentHum = humidityValue;
    lowHum = min(humidityValue, lowHum);
    highHum = max(humidityValue, highHum);
    if (lastReadHum == 0)
        deltaHumStack.Push(0);
    else
        deltaHumStack.Push(humidityValue - lastReadHum);
    lastReadHum = humidityValue;
    double sumDeltas = 0;
    for (int i = 0; i < 1200; i++)
    {
        sumDeltas += deltaHumStack[i];
    }
    deltaHum = sumDeltas;
}

void DrawHumidityScreen(U8G2_SSD1309_128X64_NONAME2_F_SW_I2C *display)
{
    double convertedHum = currentHum;
    double convertedLowHum = lowHum;
    double convertedHighHum = highHum;
    double convertedDeltaHum = deltaHum;

    SSD1309::display.drawBitmap(0, 0, 2, 16, Icon_Humidity);
    SSD1309::display.setFont(u8g2_font_6x10_tf);
    SSD1309::display.drawStr(16, 10, "Humidity");
    SSD1309::display.setFont(u8g2_font_t0_30b_tf);
    std::string sCurrentHum = "";
    std::string sLowHum = "";
    std::string sHiHum = "";
    std::string sDeltaHum = "";

    sCurrentHum = CVRT::double_to_string(convertedHum, 1) + "%";
    
    sLowHum = CVRT::double_to_string(convertedLowHum, 1);
    sHiHum = CVRT::double_to_string(convertedHighHum, 1);
    sDeltaHum = CVRT::double_to_string(convertedDeltaHum, 1);

    SSD1309::display.drawStr(0, 35, sCurrentHum.c_str());
    SSD1309::display.setFont(u8g2_font_6x10_tf);
    SSD1309::display.drawStr(0, 46, (sLowHum + "/" + sHiHum + "  " + sDeltaHum + "/hr").c_str());
    SSD1309::display.drawBox(0, 60, 128, 1);
}
std::string GetFormattedHumidity(){
    double convertedHum = currentHum;
    std::string sCurrentHum = CVRT::double_to_string(convertedHum, 1) + "%";
    return sCurrentHum;
}