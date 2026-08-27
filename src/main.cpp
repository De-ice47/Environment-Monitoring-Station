#include <Arduino.h>
#include <BME688.hpp>
#include <stdlib.h>
#include <SSD1309.hpp>

BME688::Sensor mySensor;

float updateInterval_sensor = 1000;
float updateInterval_display = 1000;
float timerDisplay = 0;
float timerSensor = 0;

void setup()
{
  delay(1000);
  Serial.begin(9600);
  mySensor = BME688::Sensor();
  mySensor.Begin(5, 4, 2);

  SSD1309::Begin();
}

void loop()
{
  if (millis() - timerSensor > updateInterval_sensor)
  {
    timerSensor = millis();
    mySensor.Measure();
    delay(1);
    float temperature = mySensor.Temperature_F();
    float pressure = mySensor.Pressure_kPa();
    float humidity = mySensor.Humidity();
    float gas = mySensor.Gas();
    Serial.println("Current Temperature: " + String(temperature, 1) +
                   "F, Pressure: " + String(pressure) +
                   " kPa, Humidity: " + String(humidity, 0) +
                   "%, Gas Resist: " + String(gas));
  }
  if (millis() - timerDisplay > updateInterval_display)
  {
    timerDisplay = millis();

    // Draw all of the shit here
    SSD1309::display.clearBuffer();

    SSD1309::display.setFont(u8g2_font_ncenB08_tr);
    SSD1309::display.drawStr(0, 20, "Hello World!");

    SSD1309::display.sendBuffer();
  }
}
