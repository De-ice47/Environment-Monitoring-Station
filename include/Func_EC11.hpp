#pragma once
#include <LoggerLite.hpp>
#include <ESP32Encoder.h>
#include <Arduino.h>
#include <SpeedPin.h>

extern ESP32Encoder encoder;

typedef void (*Callback)();

void Set_CC_Rotation_Callback(Callback cb);
void Set_C_Rotation_Callback(Callback cb);
void Set_Key_Callback(Callback cb);

void SetupEC11();
void LoopEC11();