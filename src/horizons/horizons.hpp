#pragma once

#include "../dates.hpp"
#include "../coordinates/surface.hpp"

#include <string>
#include <unordered_map>

class HorizonsAPICallParameters
{
    // Parameters
    std::unordered_map<const std::string, std::string> parameters = {
        // Common
        {"format", "text"},
        {"OBJ_DATA", "NO"},

        // Ephemeris-specific
        {"SITE_COORD", "0,0,0"},
        {"SUPPRESS_RANGE_RATE", "YES"},
    };
public:
    HorizonsAPICallParameters() {}
    HorizonsAPICallParameters(const HorizonsAPICallParameters& other) : parameters(other.parameters) {}

    void setParameter(const std::string& key, const std::string& value) { parameters[key] = value; }

    std::unordered_map<const std::string, std::string> getParameters() const { return parameters; }
};

class HorizonsPull
{
    HorizonsAPICallParameters api_call;

    const std::string API_URL = "https://ssd.jpl.nasa.gov/api/horizons.api";
public:
    HorizonsPull(const std::string object, const std::string start_time, const std::string stop_time, const std::string step_size = "1d") : api_call() {
        api_call.setParameter("COMMAND", "'" + object + "'");
        api_call.setParameter("START_TIME", start_time);
        api_call.setParameter("STOP_TIME", stop_time);
        api_call.setParameter("STEP_SIZE", step_size);
    }

    std::string generateRequestURL() const {
        std::string url = API_URL + "?";
        for (auto& [key, value] : api_call.getParameters()) {
            url += key + "=" + value + "&";
        }
        return url;
    }

    void makeRequest(std::string output_file) const {
        system(("curl " + generateRequestURL() + " -s >> " + output_file).c_str());
    }
};