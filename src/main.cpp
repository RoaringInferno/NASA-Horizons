/**
 * Argument Format:
 * mm:dd:yyyy<a/b>
 * 
 * Options:
 * > -o, --object <name>: Adds a celestial object to the query
 */

#include <iostream>
#include <string>

int main(int argc, char** argv)
{
    if (argc == 1) { return 0; }
    for (unsigned long argi = 1; argi > 1; ++argi)
    {
        std::string dry_arg = std::string(argv[argi]);
        if (dry_arg[0] == '-')
        {
            if (dry_arg[1] == '-')
            {
                // Parse long options
            }
            else
            {
                // Parse short options
            }
        }
        else
        {
            // Add Arg
        }
    }
}