#include "input_parsing.hpp"

void parse_args(int argc, char *argv[], Args &args)
{
    int i = 1;

    auto next_arg = [&]() -> std::string {
        return std::string(argv[++i]);
    };

    for (; i < argc; i++)
    {
        std::string arg = argv[i];
        if (arg == "-d" || arg == "--date")
        {
            std::string date_str = next_arg();
            args.dates.push_back(Date(date_str));
        }
        else
        {
            args.objects.push_back(arg);
        }
    }
}