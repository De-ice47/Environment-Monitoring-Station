#include <ScreenHandlers/GasScreenHandler.hpp>

double currentGas;
double highGas;
double lowGas = 100;
double deltaGas;
 
double lastReadGas = 0;
RollingBuffer<double, 1200> deltaGasStack;

void FeedGas(double GasValue)
{
    currentGas = GasValue;
    lowGas = min(GasValue, lowGas);
    highGas = max(GasValue, highGas);
    if (lastReadGas == 0)
        deltaGasStack.Push(0);
    else
        deltaGasStack.Push(GasValue - lastReadGas);
    lastReadGas = GasValue;
    double sumDeltas = 0;
    for (int i = 0; i < 1200; i++)
    {
        sumDeltas += deltaGasStack[i];
    }
    deltaGas = sumDeltas;
}

void DrawGasScreen(U8G2_SSD1309_128X64_NONAME2_F_SW_I2C *display)
{
    int unitMode = 2;
    double convertedGas = currentGas;
    double convertedLowGas = lowGas;
    double convertedHighGas = highGas;
    double convertedDeltaGas = deltaGas;

    SSD1309::display.drawBitmap(0, 0, 2, 16, Icon_Gas);
    SSD1309::display.setFont(u8g2_font_6x10_tf);
    SSD1309::display.drawStr(16, 10, "AQI");
    SSD1309::display.setFont(u8g2_font_t0_30b_tf);
    std::string sCurrentGas = "";
    std::string sLowGas = "";
    std::string sHiGas = "";
    std::string sDeltaGas = "";

    sCurrentGas = CVRT::double_to_string(convertedGas, 0);
    
    sLowGas = CVRT::double_to_string(convertedLowGas, 0);
    sHiGas = CVRT::double_to_string(convertedHighGas, 0);
    sDeltaGas = CVRT::double_to_string(convertedDeltaGas, 0);

    SSD1309::display.drawStr(0, 35, sCurrentGas.c_str());
    SSD1309::display.setFont(u8g2_font_6x10_tf);
    SSD1309::display.drawStr(0, 46, (sLowGas + "/" + sHiGas + "  " + sDeltaGas + "/hr").c_str());
    SSD1309::display.drawBox(0, 60, 128, 1);
}