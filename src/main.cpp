/**
 * Argument Format:
 * mm:dd:yyyyyy<a/b>
 * 
 * Options:
 * > -o, --object <name>: Adds a celestial object to the query
 */

#include "input_parsing.hpp"
#include "coordinates/equatorial.hpp"
#include "pull_request.hpp"
#include "cache/cache.hpp"
#include "debugging.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <thread>

// Display a message to the console with the specified character width
void displayPrint(std::string text, unsigned long width)
{
    unsigned long text_length = text.length();
    unsigned long padding = width - text_length;
    std::cout << text;
    for (unsigned long i = 0; i < padding; ++i)
    {
        std::cout << " ";
    }
}

int main(int argc, char** argv)
{
    CLIArgs options(argc, argv);

    const unsigned long COLUMN_WIDTH = 31;

    unsigned long object_count = options.args.object.size();
    DEBUG_PRINT("Object Count: " << object_count);
    unsigned long date_count = options.args.date.size();
    DEBUG_PRINT("Date Count: " << date_count);

    Query queries[date_count][object_count];
    std::thread** threads = new std::thread*[date_count];
    CacheMeta cache_meta;
    for (unsigned long j = 0; j < date_count; ++j)
    {
        threads[j] = new std::thread[object_count];
        for (unsigned long i = 0; i < object_count; ++i)
        {
            // Pull all data
            DEBUG_PRINT("Pulling Data for " << options.args.object[i] << " on " << options.args.date[j].dayRequestStartString());
            DEBUG_PRINT("Date: " << std::to_string(options.args.date[j].getMonth()) << "/" << std::to_string(options.args.date[j].getDay()) << "/" << std::to_string(options.args.date[j].getYear()));
            queries[j][i] = {options.args.object[i], options.args.date[j]};
            //if (options.args.detailed) threads[j][i] = std::thread(pullDay, &queries[j][i], std::ref(cache_meta));
            //else threads[j][i] = std::thread(pullYear, &queries[j][i], std::ref(cache_meta));
            //std::thread(pullDay, &queries[j][i], std::ref(cache_meta));
            pullDay(&queries[j][i], std::ref(cache_meta));
        }
    }
    for (unsigned long i = 0; i < object_count; ++i)
    {
        for (unsigned long j = 0; j < date_count; ++j)
        {
            //threads[j][i].join();
        }
    }

    for (unsigned long j = 0; j < date_count; ++j)
    {
        delete[] threads[j];
    }
    for (unsigned long j = 0; j < date_count; ++j)
    {
        // Get data file paths
        DaySaveFilePath file_paths[object_count];
        for (unsigned long i = 0; i < object_count; ++i)
        {
            file_paths[i] = DaySaveFilePath(DayFileID{options.args.object[i], options.args.date[j]});
        }

        // Open files
        std::ifstream files[object_count];
        for (unsigned long i = 0; i < object_count; ++i)
        {
            files[i] = std::ifstream(file_paths[i].getPath());
        }

        // Display Dates
        std::cout << "Date: " << options.args.date[j].dayRequestStartString() << std::endl << "     ";
        for (unsigned long i = 0; i < object_count; ++i) // Print headers
        {
            std::cout << " ";
            displayPrint(options.args.object[i], COLUMN_WIDTH);
            std::cout << " |";
        }
        std::cout << std::endl;
        
        unsigned char hour = 0;
        unsigned char minute = 0;

        // Lambdas
        auto display_number_fixed_width = [](unsigned long num, unsigned long width) {
            std::string num_str = std::to_string(num);
            unsigned long padding = width - num_str.length();
            for (unsigned long i = 0; i < padding; ++i)
            {
                std::cout << "0";
            }
            std::cout << num_str;
        };
        auto display_time_section = [&display_number_fixed_width](unsigned char num) {
            display_number_fixed_width(num, 2);
        };

        auto display_time = [&hour, &minute, &display_time_section]() {
            display_time_section(hour);
            std::cout << ":";
            display_time_section(minute);
        };

        auto increment_minute = [&hour, &minute]() {
            if (minute == 59)
            {
                minute = 0;
                if (hour == 23)
                {
                    hour = 0;
                }
                else
                {
                    ++hour;
                }
            }
            else
            {
                ++minute;
            }
        };

        auto displayRA = [&display_number_fixed_width](RightAscension ra) {
            display_number_fixed_width(ra.getDegrees(), 3);
            std::cout << "d";
            display_number_fixed_width(ra.getMinutes(), 2);
            std::cout << "m";
            display_number_fixed_width(ra.getSeconds(), 2);
            std::cout << "s";
            //display_number_fixed_width(ra.getMilliseconds(), 3);
            //std::cout << "ms";
        };

        auto displayDEC = [&display_number_fixed_width](Declination dec) {
            display_number_fixed_width(dec.getDegrees(), 3);
            std::cout << "d";
            display_number_fixed_width(dec.getMinutes(), 2);
            std::cout << "m";
            display_number_fixed_width(dec.getSeconds(), 2);
            std::cout << "s";
            //display_number_fixed_width(dec.getMilliseconds(), 3);
            //std::cout << "ms";
        };

        // Display Data
        for (unsigned long i = 0; i < (60 * 24); ++i) // For each data line (minute in a day)
        {
            display_time(); // Timestamp
            for (unsigned long k = 0; k < object_count; ++k) // For each object
            {
                std::cout << " ";

                std::string line;
                std::getline(files[k], line);
                CacheFileLine cache_line(line); // Load data line

                RightAscension ra = cache_line.getRA();
                Declination dec = cache_line.getDEC();

                displayRA(ra); // Display RA
                std::cout << " ";
                displayDEC(dec); // Display DEC
                std::cout << " |";
            }
            increment_minute();
            std::cout << std::endl;
        }
    }
}