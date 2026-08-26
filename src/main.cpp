#include <Arduino.h>
#include <BME688.hpp>
#include <stdlib.h>

BME688 mySensor;

void setup() {
  delay(1000);
  Serial.begin(9600);
  mySensor = BME688();
  mySensor.Config(5,4,2);
}

void loop() {
  delay(2500);
  mySensor.Measure();
  delay(50);
  double temperature = mySensor.TemperatureFloat_C();
  Serial.println("Current Temperature: " + String(temperature));
}