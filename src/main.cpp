/**
 * Argument Format:
 * mm:dd:yyyyyy<a/b>
 * 
 * Options:
 * > -o, --object <name>: Adds a celestial object to the query
 */

#include "input_parsing.hpp"
#include "equatorial-horizontal.hpp"

#include <iostream>
#include <string>
#include <vector>

struct CLIArgs
{
    std::vector<std::string> objects; // -o, --object
    bool verbose = false; // -v, --verbose
    bool detailed = false; // -d, --detailed
    std::string latitude; // -l, --latitude
    bool has_latitude = false;
    std::vector<std::string> dates; // Args
};

int main(int argc, char** argv)
{
    CLIArgs options;

    if (argc == 1) { return 0; }
    for (unsigned long argi = 1; argi > 1; ++argi)
    {
        std::string dry_arg = std::string(argv[argi]);
        if (dry_arg[0] == '-')
        {
            if (dry_arg[1] == '-')
            {
                // Parse long options
                std::string arg = dry_arg.substr(2);
                if (arg == "verbose") { options.verbose = true; }
                else if (arg == "detailed") { options.detailed = true; }
                else if (arg == "object")
                {
                    if (argi + 1 < argc)
                    {
                        options.objects.push_back(argv[argi + 1]);
                        ++argi;
                    }
                    else
                    {
                        std::cerr << "Error: Expected argument after --object" << std::endl;
                        return 1;
                    }
                }
                else if (arg == "latitude")
                {
                    if (argi + 1 < argc)
                    {
                        options.has_latitude = true;
                        options.latitude = std::string(argv[argi + 1]);
                        ++argi;
                    }
                    else
                    {
                        std::cerr << "Error: Expected argument after --longitude" << std::endl;
                        return 1;
                    }
                }
            }
            else
            {
                // Parse short options
                std::string args = dry_arg.substr(1);
                for (char arg : args)
                {
                    if (arg == 'v') { options.verbose = true; }
                    else if (arg == 'd') { options.detailed = true; }
                    else if (arg == 'o')
                    {
                        if (argi + 1 < argc)
                        {
                            options.objects.push_back(argv[argi + 1]);
                            ++argi;
                        }
                        else
                        {
                            std::cerr << "Error: Expected argument after -o" << std::endl;
                            return 1;
                        }
                    }
                    else if (arg == 'l')
                    {
                        if (argi + 1 < argc)
                        {
                            options.has_latitude = true;
                            options.latitude = std::string(argv[argi + 1]);
                            ++argi;
                        }
                        else
                        {
                            std::cerr << "Error: Expected argument after -l" << std::endl;
                            return 1;
                        }
                    }
                }
            }
        }
        else
        {
            // Add Arg
            options.dates.push_back(dry_arg);
        }
    }

    unsigned long object_count = options.objects.size();
    unsigned long date_count = options.dates.size();

    // Allocate
    EquatorialCoordinate** coordinates = new EquatorialCoordinate*[date_count];
    for (unsigned long i = 0; i < date_count; ++i)
    {
        coordinates[i] = new EquatorialCoordinate[object_count];
    }



    // Deallocate coordinates
    for (unsigned long i = 0; i < date_count; ++i)
    {
        delete[] coordinates[i];
    }
    delete[] coordinates;
}