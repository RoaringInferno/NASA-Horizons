#include "input_parsing.hpp"

void parse_arg(unsigned int& i, char** argv, Args& args)
{
    std::string arg = argv[i];
    if (arg == "-d" || arg == "--date")
    {
        args.dates.push_back(Date(argv[++i]));
        return;
    }
    args.objects.push_back(arg);
}

void parse_args(int argc, char *argv[], Args &args)
{
    for (unsigned int i = 1; i < argc; i++)
    {
        parse_arg(i, argv, args);
    }
}