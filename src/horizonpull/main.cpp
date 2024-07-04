#include "../global/dates.hpp"
#include "../global/input_parsing.hpp"
#include "horizonapipull.hpp"

#include <thread>
#include <string>
#include <vector>
#include <iostream>
#include <chrono>


const std::string OUTPUT_DIR = "horizonpulls/";

std::string generate_output_path(const std::string &object, const Date &date)
{
    return OUTPUT_DIR + object + "_" + date.to_string() + ".horizonday";
}

void pull_in_sequence(const Args& args)
{
    for (const std::string &object : args.objects)
    {
        for (const Date &date : args.dates)
        {
            std::string output_path = generate_output_path(object, date);
            pull_from_horizon_api(object, date, output_path);
        }
    }
}

void parallel_pull(const std::string& object, const std::vector<Date>& dates) {
    for (const Date& date : dates) {
        std::string output_path = generate_output_path(object, date);
        pull_from_horizon_api(object, date, output_path);
    }
}

void pull_in_parallel(const Args& args)
{
    std::thread* threads = new std::thread[args.objects.size()];

    for (size_t i = 0; i < args.objects.size(); i++) {
        threads[i] = std::thread(parallel_pull, args.objects[i], args.dates);
    }

    for (size_t i = 0; i < args.objects.size(); i++) {
        threads[i].join();
    }
}

void ensure_output_dir()
{
    std::string command = "mkdir -p " + OUTPUT_DIR;
    system(command.c_str());
}

int main(int argc, char* argv[])
{
    ensure_output_dir();
    Args args;
    parse_args(argc, argv, args);

    auto start = std::chrono::high_resolution_clock::now();
    pull_in_sequence(args);
    //pull_in_parallel(args);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;


    std::cout << "Operation completed in " << elapsed.count() << " seconds\n";
    return 0;
}