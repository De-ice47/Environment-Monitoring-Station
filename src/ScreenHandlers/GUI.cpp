#include <GUI.hpp>

// Navigation
void SwitchPushed()
{
    if (NGUI::TreeLevel == 2)
        NGUI::Fall(0);
    if (NGUI::TreeLevel == 3)
        NGUI::Rise();
}
void RotaryC()
{
    NGUI::Step(1);
}
void RotaryCC()
{
    NGUI::Step(-1);
}
// GUI Drawing
void DrawGUI()
{
    Clear();
    //Serial.println("Tree Level: " + String(NGUI::TreeLevel) + " Node: " + String(NGUI::CurrentNode->name));
    switch (NGUI::TreeLevel)
    {
    case 0:
        NGUI::Fall(0);
        break;
    case 1:
        break;
    case 2:
        switch (NGUI::CurrentNode->parent->IndexOf(NGUI::CurrentNode))
        {
        case 0:
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
        default:
            break;
        }
    case 3:
        // Settings
        break;
    default:
        break;
    }

    Draw();
}