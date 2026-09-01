#include <BME688.hpp>

extern BME688::Sensor mySensor;
extern float temperature;
extern float pressure;
extern float humidity;
extern float gas;

void SetupSensor();

void MeasureEnvironment();