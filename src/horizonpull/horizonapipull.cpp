#include "horizonapipull.hpp"
#include "url.hpp"

#include <unordered_map>
#include <iostream>


const std::string API_URL = "https://ssd.jpl.nasa.gov/api/horizons.api";

const std::string SHELL_APOSTROPHE = "'\\''";

std::string generate_request_url(std::unordered_map<std::string, std::string> &params)
{
    std::string url = API_URL + "?";
    for (auto &param : params)
    {
        url +=  param.first + "=" + param.second + "&";
    }
    url.pop_back();
    return url;
}

const std::unordered_map<std::string, std::string> BASE_PULL_ARGS = {
    {"format", "text"},
    {"OBJ_DATA", "NO"},
    {"SUPPRESS_RANGE_RATE", "YES"},
};

void pull_from_horizon_api(const std::string &object, const Date &date, const std::string &output_file_path)
{
    std::unordered_map<std::string, std::string> params = BASE_PULL_ARGS;
    Date stop_date = date;
    stop_date.increment_day();

    params["COMMAND"] = SHELL_APOSTROPHE + object + SHELL_APOSTROPHE;
    params["START_TIME"] = SHELL_APOSTROPHE + date.to_string_url_encoded() + URL_SPACE + "00" + URL_COLON + "00" + URL_COLON + "00" + SHELL_APOSTROPHE;
    params["STOP_TIME"] = SHELL_APOSTROPHE + stop_date.to_string_url_encoded() + URL_SPACE + "00" + URL_COLON + "00" + URL_COLON + "00" + SHELL_APOSTROPHE;
    params["STEP_SIZE"] = "'1m'";

    std::string url = generate_request_url(params);

    std::string command = "curl -s -o " + output_file_path + " '" + url + "'";
    system(command.c_str());
}