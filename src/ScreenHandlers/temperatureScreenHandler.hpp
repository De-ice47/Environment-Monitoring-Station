#pragma once
#include <Source.hpp>

void FeedTemperature(double temperatureValue);
void DrawTemperatureScreen(U8G2_SSD1309_128X64_NONAME2_F_SW_I2C *display);
void ChangeTemperatureUnit();
std::string GetFormattedTemperature();