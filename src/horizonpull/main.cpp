#include "../global/dates.hpp"
#include "../global/input_parsing.hpp"
#include "horizonapipull.hpp"

#include <thread>
#include <string>
#include <vector>


const std::string OUTPUT_DIR = "output/";

std::string generate_output_path(const std::string &object, const Date &date)
{
    return OUTPUT_DIR + object + "_" + date.to_string() + ".horizonday";
}

int main(int argc, char* argv[])
{
    Args args;
    parse_args(argc, argv, args);

    std::thread** threads = new std::thread*[args.objects.size()];
    for (size_t i = 0; i < args.objects.size(); i++)
    {
        threads[i] = new std::thread[args.dates.size()];
        for (size_t j = 0; j < args.dates.size(); j++)
        {
            std::string output_path = generate_output_path(args.objects[i], args.dates[j]);
            threads[i][j] = std::thread(pull_from_horizon_api, args.objects[i], args.dates[j], output_path);
        }
    }

    for (size_t i = 0; i < args.objects.size(); i++)
    {
        for (size_t j = 0; j < args.dates.size(); j++)
        {
            threads[i][j].join();
        }
        delete[] threads[i];
    }
    delete[] threads;

    return 0;
}