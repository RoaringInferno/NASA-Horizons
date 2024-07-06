#pragma once

#include "dates.hpp"

#include <string>
#include <vector>

struct Args
{
    std::vector<std::string> objects;
    std::vector<Date> dates;
};

void parse_arg(unsigned int& i, char** argv, Args& args);

void parse_args(int argc, char* argv[], Args& args);