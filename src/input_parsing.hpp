#pragma once

#include "dates.hpp"
#include "parsing.hpp"
#include "coordinates/surface.hpp"
#include "debugging.hpp"

#include <string>
#include <iostream>
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
        DEBUG_PRINT("\tParsing Date: " << date.readAll());
        MONTH month = ERR_MONTH;
        unsigned char day = 0;
        bool ad = true;


        // Parse Month
        DEBUG_PRINT("\t\tParsing Month");
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
        const std::string months[] = {
            "ERR",
            "January",
            "February",
            "March",
            "April",
            "May",
            "June",
            "July",
            "August",
            "September",
            "October",
            "November",
            "December"
        };
        DEBUG_PRINT("\t\tMonth: " << months[month]);
        date.advance(); // Colon between month and day

        // Parse Day
        DEBUG_PRINT("\t\tParsing Day");
        date.advance(); // First digit of day field
        switch (date.peek())
        {
            case '0':
                date.advance(); // Second digit of day field
                switch(date.peek())
                {
                    case '1':
                        day = 1;
                        break;
                    case '2':
                        day = 2;
                        break;
                    case '3':
                        day = 3;
                        break;
                    case '4':
                        day = 4;
                        break;
                    case '5':
                        day = 5;
                        break;
                    case '6':
                        day = 6;
                        break;
                    case '7':
                        day = 7;
                        break;
                    case '8':
                        day = 8;
                        break;
                    case '9':
                        day = 9;
                        break;
                };
                break;
            case '1':
                date.advance(); // Second digit of day field
                switch (date.peek())
                {
                    case '0':
                        day = 10;
                        break;
                    case '1':
                        day = 11;
                        break;
                    case '2':
                        day = 12;
                        break;
                    case '3':
                        day = 13;
                        break;
                    case '4':
                        day = 14;
                        break;
                    case '5':
                        day = 15;
                        break;
                    case '6':
                        day = 16;
                        break;
                    case '7':
                        day = 17;
                        break;
                    case '8':
                        day = 18;
                        break;
                    case '9':
                        day = 19;
                        break;
                };
                break;
            case '2':
                date.advance(); // Second digit of day field
                switch (date.peek())
                {
                    case '0':
                        day = 20;
                        break;
                    case '1':
                        day = 21;
                        break;
                    case '2':
                        day = 22;
                        break;
                    case '3':
                        day = 23;
                        break;
                    case '4':
                        day = 24;
                        break;
                    case '5':
                        day = 25;
                        break;
                    case '6':
                        day = 26;
                        break;
                    case '7':
                        day = 27;
                        break;
                    case '8':
                        day = 28;
                        break;
                    case '9':
                        day = 29;
                        break;
                };
                break;
            case '3':
                date.advance(); // Second digit of day field
                switch(date.peek())
                {
                    case '0':
                        day = 30;
                        break;
                    case '1':
                        day = 31;
                        break;
                };
                break;
            default:
                //day = 0; // Day field was empty
                break;
        };
        DEBUG_PRINT("\t\tDay: " << std::to_string(day));
        date.advance(); // Colon between day and year

        // Parse Year
        DEBUG_PRINT("\t\tParsing Year");
        date.advance(); // First digit of year field
        unsigned int year = stoi(date.read(YEAR_LENGTH));
        date.advance(YEAR_LENGTH); // AD/BC

        // Parse AD/BC
        if (date.peek() == 'b')
        {
            ad = false;
        }
        DEBUG_PRINT("\t\tYear: " << year << (ad ? " AD" : " BC"));

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
        std::vector<std::string> object;
        std::vector<Date> date;
        Latitude latitude{0};
        bool has_latitude{false};
        bool verbose{false};
        bool detailed{false};
        bool eclipse{false};
    } args;

    CLIArgs(int argc, char** argv)
    {
        DEBUG_PRINT("Parsing CLI Arguments");
        DEBUG_PRINT("Arg Count: " << argc);

        if (argc == 1) { return; }
        for (unsigned long argi = 1; argi < argc; ++argi)
        {
            DEBUG_PRINT("Parsing Arg #" << argi << ": " << argv[argi]);
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
                        DEBUG_PRINT("Pushing --object");
                        this->args.object.push_back(argv[++argi]);
                    }
                    else if (arg == "latitude")
                    {
                        DEBUG_PRINT("Pushing --latitude");
                        this->args.has_latitude = true;
                        this->args.latitude = parseLatitude(std::string(argv[++argi]));
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
                            DEBUG_PRINT("Pushing -o");
                            this->args.object.push_back(argv[++argi]);
                        }
                        else if (arg == 'l')
                        {
                            DEBUG_PRINT("Pushing -l");
                            this->args.has_latitude = true;
                            this->args.latitude = parseLatitude(std::string(argv[++argi]));
                        }
                    }
                }
            }
            else
            {
                DEBUG_PRINT("Pushing Date");
                // Add Arg
                this->args.date.push_back(parseDate(dry_arg));
            }
        }
    }
};