#include <Conversions.hpp>

namespace CVRT
{
    // Formatting
    std::string double_to_string(double value, int decimal_places)
    {
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(decimal_places) << value;
        return oss.str();
    }
    // Temperatures
    double C_to_F(double value)
    {
        return value * 1.8 + 32;
    }
    double C_to_K(double value)
    {
        return value + 273.15;
    }
    // Pressures
    double kPa_to_atm(double value){
        return value * 0.009869;
    }
    double kPa_to_bar(double value){
        return value * 0.01;
    }
    double kPa_to_mmHg(double value){
        return value * 7.500638;
    }
    double kPa_to_inHg(double value){
        return value * 0.296134;
    }
    double kPa_to_psi(double value){
        return value * 0.145038;
    }
}
