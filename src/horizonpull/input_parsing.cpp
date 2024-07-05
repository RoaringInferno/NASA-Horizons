#include "../global/input_parsing.hpp"

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