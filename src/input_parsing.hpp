#pragma once

#include "dates.hpp"
#include "parsing.hpp"
#include "coordinates/surface.hpp"

#include <string>
#include <vector>

class CLIArgs
{
    const unsigned char YEAR_LENGTH = 4;

    /**
     * mm:dd:yyyy<a/b>
     * 
     * Sets the value to 0 if the field is empty.
     * Year field is required.
     */
    Date parseDate(ReadHead date)
    {
        MONTH month = ERR_MONTH;
        unsigned char day = 0;
        bool ad = true;


        // Parse Month
        switch (date.peek()) // First digit of month field
        {
            case '0':
                date.advance(); // Second digit of month field
                switch (date.peek())
                {
                    case '1':
                        month = JANUARY;
                        break;
                    case '2':
                        month = FEBRUARY;
                        break;
                    case '3':
                        month = MARCH;
                        break;
                    case '4':
                        month = APRIL;
                        break;
                    case '5':
                        month = MAY;
                        break;
                    case '6':
                        month = JUNE;
                        break;
                    case '7':
                        month = JULY;
                        break;
                    case '8':
                        month = AUGUST;
                        break;
                    case '9':
                        month = SEPTEMBER;
                        break;
                    default:
                        //month = ERR_MONTH; // Month field contained only a '0'
                        break;
                };
                break;
            case '1':
                date.advance(); // Second digit of month field
                switch (date.peek())
                {
                    case '0':
                        month = OCTOBER;
                        break;
                    case '1':
                        month = NOVEMBER;
                        break;
                    case '2':
                        month = DECEMBER;
                        break;
                    default:
                        //month = ERR_MONTH; // Month field contained only a '1'
                        break;
                };
                break;
            default:
                //month = ERR_MONTH; // Month field was empty
                break;
        };
        date.advance(); // Colon between month and day

        // Parse Day
        date.advance(); // First digit of day field
        switch (date.peek())
        {
            case '0':
                date.advance(); // Second digit of day field
                break;
            case '1':
                date.advance(); // Second digit of day field
                break;
            default:
                //day = 0; // Day field was empty
                break;
        };
        date.advance(); // Colon between day and year

        // Parse Year
        date.advance(); // First digit of year field
        unsigned int year = stoi(date.read(YEAR_LENGTH));
        date.advance(YEAR_LENGTH); // AD/BC

        // Parse AD/BC
        if (date.peek() == 'b')
        {
            ad = false;
        }

        return Date(year, month, day, ad);
    }

    /**
     * <N/S>xx.xxxxx
     */
    Latitude parseLatitude(ReadHead latitude)
    {
        char sign = latitude.peek(); // Read the first character (N/S)
        latitude.advance(); // Skip the first character
        long double degrees = stold(latitude.readAll()); // Read the degrees as a long double

        // Apply the sign based on the first character
        if (sign == 'S')
        {
            degrees = -degrees;
        }

        return Latitude(degrees);
    }
public:
    struct {
        std::vector<std::string> object{};
        std::vector<Date> date{};
        Latitude latitude{0};
        bool has_latitude{false};
        bool verbose{false};
        bool detailed{false};
        bool eclipse{false};
    } args;

    CLIArgs(int argc, char** argv)
    {
        if (argc == 1) { return; }
        for (unsigned long argi = 1; argi > 1; ++argi)
        {
            std::string dry_arg = std::string(argv[argi]);
            if (dry_arg[0] == '-')
            {
                if (dry_arg[1] == '-')
                {
                    // Parse long options
                    std::string arg = dry_arg.substr(2);
                    if (arg == "verbose") { this->args.verbose = true; }
                    else if (arg == "detailed") { this->args.detailed = true; }
                    else if (arg == "eclipse") { this->args.eclipse = true; }
                    else if (arg == "object")
                    {
                        if (argi + 1 < argc)
                        {
                            this->args.object.push_back(argv[argi + 1]);
                            ++argi;
                        }
                        else
                        {
                            std::cerr << "Error: Expected argument after --object" << std::endl;
                            return;
                        }
                    }
                    else if (arg == "latitude")
                    {
                        if (argi + 1 < argc)
                        {
                            this->args.has_latitude = true;
                            this->args.latitude = parseLatitude(std::string(argv[argi + 1]));
                            ++argi;
                        }
                        else
                        {
                            std::cerr << "Error: Expected argument after --longitude" << std::endl;
                            return;
                        }
                    }
                }
                else
                {
                    // Parse short options
                    std::string args = dry_arg.substr(1);
                    for (char arg : args)
                    {
                        if (arg == 'v') { this->args.verbose = true; }
                        else if (arg == 'd') { this->args.detailed = true; }
                        else if (arg == 'o')
                        {
                            if (argi + 1 < argc)
                            {
                                this->args.object.push_back(argv[argi + 1]);
                                ++argi;
                            }
                            else
                            {
                                std::cerr << "Error: Expected argument after -o" << std::endl;
                                return;
                            }
                        }
                        else if (arg == 'l')
                        {
                            if (argi + 1 < argc)
                            {
                                this->args.has_latitude = true;
                                this->args.latitude = parseLatitude(std::string(argv[argi + 1]));
                                ++argi;
                            }
                            else
                            {
                                std::cerr << "Error: Expected argument after -l" << std::endl;
                                return;
                            }
                        }
                    }
                }
            }
            else
            {
                // Add Arg
                this->args.date.push_back(parseDate(dry_arg));
            }
        }
    }
};