#include "horizons.hpp"

std::string generate_horizonpull_output_filename(const std::string &object, const Date &date)
{
    return object + "_" + date.to_string() + ".horizonday";
}

std::string generate_horizonread_output_filename(const std::string &object, const Date &date)
{
    return generate_horizonpull_output_filename(object, date) + ".csv";
}
