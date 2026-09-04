#include <Source.hpp>
#include <Func_Serial.hpp>
#include <Func_Display.hpp>
#include <Func_Sensor.hpp>
#include <Func_EC11.hpp>
#include <SpeedPin.h>
#include <GUI.hpp>

// Settings
float updateInterval_sensor = 150;
float updateInterval_display = 100;
// Timers
float timerDisplay = 0;
float timerSensor = 0;

void setup()
{
  //
  // 1. Serial
  //
  SetupSerial();
  DrawLogs();
  //
  // 2. Display
  //
  SetupDisplay();
  DrawLogs();
  //
  // 3. Sensor
  //
  SetupSensor();
  DrawLogs();
  //
  // 4. NGUI UITree Construct
  //
  LL::Println("[NGUI] Initializing");
  NGUI::Initialize();
  LL::Println("[NGUI] Constructing Tree");
  ConstructTree();
  String debugStructure = NGUI::DebugStructure();
  Serial.println(debugStructure);
  NGUI::Fall(0);
  DrawLogs();
  //
  // 5. Encoder
  //
  SetupEC11();
  Set_CC_Rotation_Callback(RotaryCC);
  Set_C_Rotation_Callback(RotaryC);
  Set_Key_Callback(SwitchPushed);
  DrawLogs();
  //
  LL::Println("[ROOT] Startup Complete!");
  DrawLogs();
  delay(1500);
  NGUI::Fall(0);
}

void loop()
{
  LoopEC11();
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