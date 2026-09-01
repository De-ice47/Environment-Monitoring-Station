#include <Source.hpp>
#include <ScreenHandlers.hpp>
#include <Func_Serial.hpp>
#include <Func_Display.hpp>
#include <Func_Sensor.hpp>

// Settings
float updateInterval_sensor = 1000;
float updateInterval_display = 200;
// Timers
float timerDisplay = 0;
float timerSensor = 0;

void DrawGUI();

void setup()
{
  //
  // 1. Serial
  //
  delay(1000);
  SetupSerial();
  DrawLogs();
  //
  // 2. Display
  //
  delay(100);
  SetupDisplay();
  DrawLogs();
  //
  // 3. Sensor
  //
  delay(100);
  SetupSensor();
  DrawLogs();
  //
  // 4. NGUI UITree Construct
  //
  delay(100);
  LL::Println("[NGUI] Initializing");
  NGUI::Initialize();
  LL::Println("[NGUI] Constructing Tree");
  ConstructTree();
  String debugStructure = NGUI::DebugStructure();
  Serial.println(debugStructure);
  DrawLogs();
  //
  // 5. Encoder
  //
  delay(100);

  LL::Println("[EC11] Encoder Ready");
  DrawLogs();
  //
  delay(250);
  LL::Println("[ROOT] Startup Complete!");
  DrawLogs();
  delay(1500);
}

void loop()
{

  // Read the sensor
  if (millis() - timerSensor > updateInterval_sensor)
  {
    timerSensor = millis();
    MeasureEnvironment();
  }

  // Display
  if (millis() - timerDisplay > updateInterval_display)
  {
    timerDisplay = millis();
    DrawGUI();
  }
}

void DrawGUI(){
  Clear();
    int dataPage = 0;
    switch (dataPage)
    {
    case 0:
      FeedTemperature(temperature);
      DrawTemperatureScreen(display);
      break;
    case 1:
      FeedPressure(pressure);
      DrawPressureScreen(display);
      break;

    default:
      break;
    }
    Draw();
}