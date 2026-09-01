#include <Func_Sensor.hpp>

BME688::Sensor mySensor;
// Data Values
float temperature = 0;
float pressure = 0;
float humidity = 0;
float gas = 0;

void SetupSensor()
{
    mySensor = BME688::Sensor();
    mySensor.Begin(5, 4, 2);
    LL::Println("[BME688] Sensor OK");
}

void MeasureEnvironment(){
    mySensor.Measure();
    delay(1);
    temperature = mySensor.Temperature_C();
    pressure = mySensor.Pressure_kPa();
    humidity = mySensor.Humidity();
    gas = mySensor.Gas();
}