#include <Arduino.h>
#include <BME688.hpp>
#include <stdlib.h>

BME688::Sensor mySensor;

void setup() {
  delay(1000);
  Serial.begin(9600);
  mySensor = BME688::Sensor();
  mySensor.Begin(5,4,2);
}

void loop() {
  delay(2500);
  mySensor.Measure();
  delay(10);
  float temperature = mySensor.TemperatureFloat_F();
  Serial.println("Current Temperature: " + String(temperature) + "F");
}