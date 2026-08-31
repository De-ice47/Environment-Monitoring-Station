#include <Source.hpp>
#include <ScreenHandlers.hpp>

BME688::Sensor mySensor;

float updateInterval_sensor = 1000;
float updateInterval_display = 200;
float timerDisplay = 0;
float timerSensor = 0;

float temperature = 0;
float pressure = 0;
float humidity = 0;
float gas = 0;

void SerialOutput(const char *message);

void Clear();
void Draw();
void DrawLogs();
U8G2_SSD1309_128X64_NONAME2_F_SW_I2C *display = &SSD1309::display;

void setup()
{
  //
  // 1. Serial
  //
  delay(1000);
  Serial.begin(9600);
  LL::OutputCallback(SerialOutput);
  LL::Println("[ROOT] Starting Serial Output");
  Clear();
  DrawLogs();
  Draw();
  //
  // 2. Display
  //
  delay(100);
  SSD1309::Begin();
  LL::Println("[SSD1309] Display Active");
  Clear();
  DrawLogs();
  Draw();
  //
  // 3. Sensor
  //
  delay(100);
  mySensor = BME688::Sensor();
  mySensor.Begin(5, 4, 2);
  LL::Println("[BME688] Sensor OK");
  Clear();
  DrawLogs();
  Draw();
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
  Clear();
  DrawLogs();
  Draw();
  //
  //
  //
  delay(250);
  LL::Println("[ROOT] Startup Complete!");
  Clear();
  DrawLogs();
  Draw();
  delay(1500);
}

void loop()
{
  //
  // Read the sensor
  //

  if (millis() - timerSensor > updateInterval_sensor)
  {
    timerSensor = millis();
    mySensor.Measure();
    delay(1);
    temperature = mySensor.Temperature_C();
    pressure = mySensor.Pressure_kPa();
    humidity = mySensor.Humidity();
    gas = mySensor.Gas();
  }
  //
  // Display
  //
  Serial.println("here");
  if (millis() - timerDisplay > updateInterval_display)
  {
    timerDisplay = millis();

    // Draw all of the shit here
    Clear();
    FeedTemperature(temperature);
    DrawTemperatureScreen(display);
    Draw();
  }
}

void SerialOutput(const char *message)
{
  Serial.println(message);
}
inline void Clear()
{
  SSD1309::display.clearBuffer();
}
inline void Draw()
{
  SSD1309::display.sendBuffer();
}
void DrawLogs()
{
  int logsToShow = min((uint8_t)9, LL::GetLineCount());
  int startIndex = LL::GetLineCount() - logsToShow;
  display->setFont(u8g2_font_tiny5_t_all);
  for (uint8_t i = startIndex, yPos = 0; i < logsToShow; i++, yPos++)
  {
    display->drawStr(2, 6 + yPos * 6, LL::GetLine(i));
  }
}