#include "input_parsing.hpp"

void parse_args(int argc, char *argv[], Args &args)
{
    for (unsigned int i = 1; i < argc; i++)
    {
        parse_arg(i, argv, args);
    }
}