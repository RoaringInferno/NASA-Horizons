#include "input_parsing.hpp"
#include "horizonread.hpp"
#include "horizons.hpp"

#include <filesystem>

void ensure_output_dir()
{
    std::string command = "mkdir -p " + HORIZONREAD_OUTPUT_DIR;
    system(command.c_str());
}

void read(Args& args)
{
    for (const std::string &object : args.objects)
    {
        for (const Date &date : args.dates)
        {

            std::string input_path = HORIZONPULL_OUTPUT_DIR + generate_horizonpull_output_filename(object, date);
            std::string output_path = HORIZONREAD_OUTPUT_DIR + generate_horizonread_output_filename(object, date);

            if (std::filesystem::exists(output_path)) continue;

            HorizonEphemeris ephemeris(input_path);
            ephemeris.writeCSV(output_path);
        }
    }
}

int main(int argc, char *argv[])
{
    ensure_output_dir();

    Args args;
    parse_args(argc, argv, args);

    read(args);
    return 0;
}