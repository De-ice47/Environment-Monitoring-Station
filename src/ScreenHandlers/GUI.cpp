#include <GUI.hpp>

int8_t dataScreen = 0;
// Navigation
void SwitchPushed()
{
    if (NGUI::TreeLevel == 2)
    {
        switch (dataScreen)
        {
        case 0:
            break;
        case 1:
            ChangeTemperatureUnit();
            break;
        case 2:
            ChangePressureUnit();
            break;
        case 3:
            break;
        case 4:
            break;
        default:
            break;
        }
    }
}
void RotaryC()
{
    dataScreen++;
    if(dataScreen >= 5) dataScreen = 4;
}
void RotaryCC()
{
    dataScreen--;
    if(dataScreen <= 0) dataScreen = 0;
}
// GUI Drawing
void DrawGUI()
{
    Clear();
    switch (NGUI::TreeLevel)
    {
    case 0:
        NGUI::Fall(0);
        break;
    case 1:
        break;
    case 2:
        switch (dataScreen)
        {
        case 0:
            FeedTemperature(temperature);
            FeedPressure(pressure);
            FeedHumidity(humidity);
            DrawAllDataScreen();
            break;
        case 1:
            FeedTemperature(temperature);
            DrawTemperatureScreen(display);
            break;
        case 2:
            FeedPressure(pressure);
            DrawPressureScreen(display);
            break;
        case 3:
            FeedHumidity(humidity);
            DrawHumidityScreen(display);
            break;
        case 4:
            FeedGas(gas);
            DrawGasScreen(display);
            break;
        }
    default:
        break;
    }

    Draw();
}