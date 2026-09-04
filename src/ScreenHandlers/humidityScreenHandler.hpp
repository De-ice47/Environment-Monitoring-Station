#pragma once
#include <Source.hpp>

void FeedHumidity(double humidityValue);
void DrawHumidityScreen(U8G2_SSD1309_128X64_NONAME2_F_SW_I2C *display);
std::string GetFormattedHumidity();