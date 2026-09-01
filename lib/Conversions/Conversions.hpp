#pragma once
#include <string.h>
#include <iostream>
#include <sstream>
#include <iomanip>

namespace CVRT
{
    std::string double_to_string(double value, int decimal_places);
    // Temperature
    double C_to_F(double value);
    double C_to_K(double value);
    // Pressure
    double kPa_to_atm(double value);
    double kPa_to_bar(double value);
    double kPa_to_mmHg(double value);
    double kPa_to_inHg(double value);
}
