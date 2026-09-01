#include <ScreenHandlers/pressureScreenHandler.hpp>

double currentPres;
double highPres;
double lowPres = 500;
double deltaPres;

double lastReadPres = 0;
RollingBuffer<double, 1200> deltaPresStack;

void FeedPressure(double pressureValue)
{
    currentPres = pressureValue;
    lowPres = min(pressureValue, lowPres);
    highPres = max(pressureValue, highPres);
    if (lastReadPres == 0)
        deltaPresStack.Push(0);
    else
        deltaPresStack.Push(pressureValue - lastReadPres);
    lastReadPres = pressureValue;
    double sumDeltas = 0;
    for (int i = 0; i < 1200; i++)
    {
        sumDeltas += deltaPresStack[i];
    }
    deltaPres = sumDeltas;
}

double C_to_F(double value)
{
    return value * 1.8 + 32;
}
double C_to_K(double value)
{
    return value + 273.15;
}
void DrawPressureScreen(U8G2_SSD1309_128X64_NONAME2_F_SW_I2C *display)
{

    int unitMode = 1;
    double convertedPres = currentPres;
    double convertedLowPres = lowPres;
    double convertedHighPres = highPres;
    double convertedDeltaPres = deltaPres;

    SSD1309::display.drawBitmap(0, 0, 2, 16, Icon_Pressure); // Change this icon to the pressure one
    SSD1309::display.setFont(u8g2_font_6x10_tf);
    SSD1309::display.drawStr(16, 10, "Pressure");
    SSD1309::display.setFont(u8g2_font_t0_30b_tf);
    std::string sCurrentPres = "";
    std::string sLowPres = "";
    std::string sHiPres = "";
    std::string sDeltaPres = "";
    switch (unitMode)
    {
    case 0:
        sCurrentPres = CVRT::double_to_string(convertedPres, 2) + "kPa";
        break;
    case 1:
        convertedPres = CVRT::kPa_to_atm(currentPres);
        convertedLowPres = CVRT::kPa_to_atm(lowPres);
        convertedHighPres = CVRT::kPa_to_atm(highPres);
        sCurrentPres = CVRT::double_to_string(convertedPres, 2) + "atm";
        break;
    case 2:
        convertedPres = CVRT::kPa_to_bar(currentPres);
        convertedLowPres = CVRT::kPa_to_bar(lowPres);
        convertedHighPres = CVRT::kPa_to_bar(highPres);
        sCurrentPres = CVRT::double_to_string(convertedPres, 2) + "bar";
        break;
        case 3:
        convertedPres = CVRT::kPa_to_inHg(currentPres);
        convertedLowPres = CVRT::kPa_to_inHg(lowPres);
        convertedHighPres = CVRT::kPa_to_inHg(highPres);
        sCurrentPres = CVRT::double_to_string(convertedPres, 2) + "inHg";
        break;
    default:
        break;
    }
    sLowPres = CVRT::double_to_string(convertedLowPres, 1);
    sHiPres = CVRT::double_to_string(convertedHighPres, 1);
    sDeltaPres = CVRT::double_to_string(convertedDeltaPres, 1);

    SSD1309::display.drawStr(0, 35, sCurrentPres.c_str());
    SSD1309::display.setFont(u8g2_font_6x10_tf);
    SSD1309::display.drawStr(0, 46, (sLowPres + "/" + sHiPres + "  " + sDeltaPres + "/hr").c_str());
    SSD1309::display.drawBox(0, 60, 128, 1);
}