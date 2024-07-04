#pragma once

#include "../global/dates.hpp"

#include <string>

void pull_from_horizon_api(const std::string &object, const Date &date, const std::string &output_file_path);