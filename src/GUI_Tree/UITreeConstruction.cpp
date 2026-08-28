#include <GUI_Tree/UITreeConstruction.hpp>

// Layer 1
NGUI::UINode DEBUG;
// Layer 2
NGUI::UINode AllData;
NGUI::UINode Temperature;
NGUI::UINode Pressure;
NGUI::UINode Humidity;
NGUI::UINode AQI;
// Layer 3
NGUI::UINode Settings;

void ConstructTree(){
    DEBUG = NGUI::UINode("DEBUG");

    AllData = NGUI::UINode("AllData");
    Temperature = NGUI::UINode("Temperature");
    Pressure = NGUI::UINode("Pressure");
    Humidity = NGUI::UINode("Humidity");
    AQI = NGUI::UINode("AQI");
    
    Settings = NGUI::UINode("Settings");

    // Linking
    NGUI::RootNode.AddChild(&DEBUG);

    DEBUG.AddChild(&AllData);
    DEBUG.AddChild(&Temperature);
    DEBUG.AddChild(&Pressure);
    DEBUG.AddChild(&Humidity);
    DEBUG.AddChild(&AQI);
}