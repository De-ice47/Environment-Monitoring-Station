#include <ScreenHandlers/temperatureScreenHandler.hpp>

double currentTemperature;
double highTemperature;
double lowTemperature = 500;
double deltaTemp;

double lastRead = 0;
RollingBuffer<double,1200> deltaTemps;

void FeedTemperature(double temperatureValue)
{
    currentTemperature = temperatureValue;
    lowTemperature = min(temperatureValue, lowTemperature);
    highTemperature = max(temperatureValue, highTemperature);
    if(lastRead == 0) deltaTemps.Push(0);
    else deltaTemps.Push(temperatureValue - lastRead);
    lastRead = temperatureValue;
    double sumDeltas = 0;
    for (int i = 0; i < 1200; i++)
    {
        sumDeltas += deltaTemps[i];
    }
    deltaTemp = sumDeltas;
}

double C_to_F(double value)
{
    return value * 1.8 + 32;
}
double C_to_K(double value)
{
    return value + 273.15;
}
std::string double_to_string(double value, int decimal_places) {
    std::ostringstream oss;
    // std::fixed ensures fixed-point notation
    // std::setprecision sets the number of decimal places
    oss << std::fixed << std::setprecision(decimal_places) << value;
    return oss.str();
}

void DrawTemperatureScreen(U8G2_SSD1309_128X64_NONAME2_F_SW_I2C *display)
{

    int unitMode = 1;
    double convertedTemp = currentTemperature;
    double convertedLowTemp = lowTemperature;
    double convertedHighTemp = highTemperature;
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
        sCurrentTemp = double_to_string(convertedTemp,1) + "C";
        break;
    case 1:
        convertedTemp = C_to_F(currentTemperature);
        convertedLowTemp = C_to_F(lowTemperature);
        convertedHighTemp = C_to_F(highTemperature);
        sCurrentTemp = double_to_string(convertedTemp,1) + "F";
        break;
    case 2:
        convertedTemp = C_to_K(currentTemperature);
        convertedLowTemp = C_to_K(lowTemperature);
        convertedHighTemp = C_to_K(highTemperature);
        sCurrentTemp = double_to_string(convertedTemp,1) + "K";
    default:
        break;
    }
    sLowTemp = double_to_string(convertedLowTemp,1);
    sHiTemp = double_to_string(convertedHighTemp,1);
    sDeltaTemp = double_to_string(convertedDeltaTemp,1);
    
        Serial.print("Temp: ");
        Serial.print(convertedTemp);
        Serial.print("   ");
        Serial.print(sCurrentTemp.c_str());
        Serial.print("   ");
        Serial.println(currentTemperature);

    SSD1309::display.drawStr(0, 35, sCurrentTemp.c_str());
    SSD1309::display.setFont(u8g2_font_6x10_tf);
    SSD1309::display.drawStr(0, 46, (sLowTemp + "/" + sHiTemp + "  " + sDeltaTemp + "/hr").c_str());
    SSD1309::display.drawBox(0, 60, 128, 1);
}