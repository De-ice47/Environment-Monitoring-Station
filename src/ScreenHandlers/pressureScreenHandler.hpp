#pragma once
#include <Source.hpp>

void FeedPressure(double temperatureValue);
void DrawPressureScreen(U8G2_SSD1309_128X64_NONAME2_F_SW_I2C *display);
void ChangePressureUnit();
std::string GetFormattedPressure();