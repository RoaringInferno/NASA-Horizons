#pragma once

#include "dates.hpp"

#include <string>

const std::string HORIZONPULL_OUTPUT_DIR = "horizonpulls/";

std::string generate_horizonpull_output_filename(const std::string &object, const Date &date);

const std::string HORIZONREAD_OUTPUT_DIR = "horizonreads/";

std::string generate_horizonread_output_filename(const std::string &object, const Date &date);