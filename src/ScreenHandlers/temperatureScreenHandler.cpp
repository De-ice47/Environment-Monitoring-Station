#include <ScreenHandlers/temperatureScreenHandler.hpp>

double currentTemp;
double highTemp;
double lowTemp = 500;
double deltaTemp;

double lastReadTemp = 0;
RollingBuffer<double, 1200> deltaTempStack;

void FeedTemperature(double temperatureValue)
{
    currentTemp = temperatureValue;
    lowTemp = min(temperatureValue, lowTemp);
    highTemp = max(temperatureValue, highTemp);
    if (lastReadTemp == 0)
        deltaTempStack.Push(0);
    else
        deltaTempStack.Push(temperatureValue - lastReadTemp);
    lastReadTemp = temperatureValue;
    double sumDeltas = 0;
    for (int i = 0; i < 1200; i++)
    {
        sumDeltas += deltaTempStack[i];
    }
    deltaTemp = sumDeltas;
}

void DrawTemperatureScreen(U8G2_SSD1309_128X64_NONAME2_F_SW_I2C *display)
{
    int unitMode = 2;
    double convertedTemp = currentTemp;
    double convertedLowTemp = lowTemp;
    double convertedHighTemp = highTemp;
    double convertedDeltaTemp = deltaTemp;

    SSD1309::display.drawBitmap(0, 0, 2, 16, Icon_Temperature);
    SSD1309::display.setFont(u8g2_font_6x10_tf);
    SSD1309::display.drawStr(16, 10, "Temperature");
    SSD1309::display.setFont(u8g2_font_t0_30b_tf);
    std::string sCurrentTemp = "";
    std::string sLowTemp = "";
    std::string sHiTemp = "";
    std::string sDeltaTemp = "";
    switch (unitMode)
    {
    case 0:
        sCurrentTemp = CVRT::double_to_string(convertedTemp, 1) + "C";
        break;
    case 1:
        convertedTemp = CVRT::C_to_F(currentTemp);
        convertedLowTemp = CVRT::C_to_F(lowTemp);
        convertedHighTemp = CVRT::C_to_F(highTemp);
        sCurrentTemp = CVRT::double_to_string(convertedTemp, 1) + "F";
        break;
    case 2:
        convertedTemp = CVRT::C_to_K(currentTemp);
        convertedLowTemp = CVRT::C_to_K(lowTemp);
        convertedHighTemp = CVRT::C_to_K(highTemp);
        sCurrentTemp = CVRT::double_to_string(convertedTemp, 1) + "K";
    default:
        break;
    }
    sLowTemp = CVRT::double_to_string(convertedLowTemp, 1);
    sHiTemp = CVRT::double_to_string(convertedHighTemp, 1);
    sDeltaTemp = CVRT::double_to_string(convertedDeltaTemp, 1);

    SSD1309::display.drawStr(0, 35, sCurrentTemp.c_str());
    SSD1309::display.setFont(u8g2_font_6x10_tf);
    SSD1309::display.drawStr(0, 46, (sLowTemp + "/" + sHiTemp + "  " + sDeltaTemp + "/hr").c_str());
    SSD1309::display.drawBox(0, 60, 128, 1);
}