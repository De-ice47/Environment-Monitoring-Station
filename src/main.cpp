#include <Source.hpp>

BME688::Sensor mySensor;

float updateInterval_sensor = 1000;
float updateInterval_display = 200;
float timerDisplay = 0;
float timerSensor = 0;
 
void DrawTemperatureScreen();

void setup()
{
  //
  // 1. Serial
  //
  delay(1000);
  Serial.begin(9600);
  LL::Println("[ROOT] Beginning Serial Output @ 9600 Baud");
  // 
  // 2. Sensor
  // 
  delay(100);
  mySensor = BME688::Sensor();
  mySensor.Begin(5, 4, 2);
  //
  // 3. Display
  //
    delay(100);
  SSD1309::Begin();
  //
  // 4. NGUI UITree Construct
  //
    delay(100);
  Serial.println("Initializing NGUI");
  NGUI::Initialize();
  Serial.println("Constructing Tree");
  ConstructTree();
  Serial.println("printing Tree");
  String debugStructure = NGUI::DebugStructure();
  Serial.println(debugStructure);
  //
  //
  //

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
    float temperature = mySensor.Temperature_F();
    float pressure = mySensor.Pressure_kPa();
    float humidity = mySensor.Humidity();
    float gas = mySensor.Gas();
  }
  //
  // Display 
  //
  if (millis() - timerDisplay > updateInterval_display)
  {
    timerDisplay = millis();

    // Draw all of the shit here
    SSD1309::display.clearBuffer();

    DrawTemperatureScreen();

    SSD1309::display.sendBuffer();
  }
}

void DrawTemperatureScreen(){
  // Demo Temperature Window
    SSD1309::display.drawBitmap(0,0,2,16, Icon_Temperature);
    SSD1309::display.setFont(u8g2_font_6x10_tf);
    SSD1309::display.drawStr(16, 10, "Temperature");
    SSD1309::display.setFont(u8g2_font_t0_30b_tf);
    SSD1309::display.drawStr(0,35,"95.8F");
    SSD1309::display.setFont(u8g2_font_6x10_tf);
    SSD1309::display.drawStr(0, 46, "178.4/298.1  -1.2/hr");
    SSD1309::display.drawBox(0,60,128,1);
}
