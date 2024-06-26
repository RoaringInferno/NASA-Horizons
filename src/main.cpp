/**
 * Argument Format:
 * mm:dd:yyyyyy<a/b>
 * 
 * Options:
 * > -o, --object <name>: Adds a celestial object to the query
 */

#include "input_parsing.hpp"
#include "equatorial-horizontal.hpp"
#include "pull_request.hpp"
#include "cache/cache.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <thread>

// Display a message to the console with the specified character width
void displayPrint(std::string text, unsigned long width)
{
    unsigned long text_length = text.length();
    unsigned long padding = (width - text_length) / 2;
    for (unsigned long i = 0; i < padding; ++i)
    {
        std::cout << " ";
    }
    std::cout << text;
    for (unsigned long i = 0; i < padding; ++i)
    {
        std::cout << " ";
    }
}

int main(int argc, char** argv)
{
    CLIArgs options(argc, argv);

    const unsigned long COLUMN_WIDTH = 29;

    unsigned long object_count = options.args.object.size();
    unsigned long date_count = options.args.date.size();

    Query queries[date_count][object_count];
    std::thread** threads = new std::thread*[date_count];
    CacheMeta cache_meta;
    for (unsigned long j = 0; j < date_count; ++j)
    {
        threads[j] = new std::thread[object_count];
        for (unsigned long i = 0; i < object_count; ++i)
        {
            queries[j][i] = {options.args.object[i], options.args.date[j]};
            if (options.args.detailed) threads[j][i] = std::thread(pullDay, &queries[j][i], std::ref(cache_meta));
            else threads[j][i] = std::thread(pullYear, &queries[j][i], std::ref(cache_meta));
        }
    }
    for (unsigned long i = 0; i < object_count; ++i)
    {
        for (unsigned long j = 0; j < date_count; ++j)
        {
            threads[j][i].join();
        }
    }

    for (unsigned long j = 0; j < date_count; ++j)
    {
        delete[] threads[j];
    }
    for (unsigned long j = 0; j < date_count; ++j)
    {
        std::cout << "Date: " << options.args.date[j].dayRequestStartString() << std::endl << "           ";
        for (unsigned long i = 0; i < object_count; ++i)
        {
            std::cout << " ";
            displayPrint(options.args.object[i], COLUMN_WIDTH);
            std::cout << " |";
        }
    }
}