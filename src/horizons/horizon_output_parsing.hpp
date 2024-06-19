#pragma once

#include "../dates.hpp"
#include "../coordinates/equatorial.hpp"

#include <string>
#include <fstream>
#include <vector>
#include <unordered_map>

class HorizonEphemerisDataLine
{
    /**
     * Date string in the format "YYYY-MON-DD HH:MM"
     */
    DateTime parseHorizonDateTime(const std::string& date_string)
    {
        /**
         * Jan
         * Feb
         * Mar
         * Apr
         * May
         * Jun
         * Jul
         * Aug
         * Sep
         * Oct
         * Nov
         * Dec
         */
        const auto month_hash = [](const std::string& month) -> MONTH {
            switch ((month[0] - 'A') * 4 + (month[1] - 'a')) {
                case 29: return JANUARY; 
                case 37: return FEBRUARY;
                case 45: return MARCH; 
                case 15: return APRIL; 
                case 12: return MAY; 
                case 43: return JUNE; 
                case 44: return JULY; 
                case 48: return AUGUST; 
                case 30: return SEPTEMBER; 
                case 24: return OCTOBER; 
                case 49: return NOVEMBER; 
                case 33: return DECEMBER; 
                default: return ERR_MONTH;
            }
        };

        DateTime date;

        date.setYear(std::stoi(date_string.substr(0, 4)));
        date.setDay(std::stoi(date_string.substr(8, 2)));
        date.setHour(std::stoi(date_string.substr(11, 2)));
        date.setMinute(std::stoi(date_string.substr(14, 2)));
        date.setMonth(month_hash(date_string.substr(5, 3)));

        return date;
        
    }

    /**
     * RA in the format HH MM SS.ff
     */
    RightAscension parseHorizonRA(const std::string& ra_string)
    {
        RightAscension ra;

        ra.setDegrees(std::stoi(ra_string.substr(0, 2)) * 15);
        ra.setMinutes(std::stoi(ra_string.substr(3, 2)));
        ra.setSeconds(std::stod(ra_string.substr(6, 5)));
        ra.setMilliseconds(std::stoi(ra_string.substr(9, 2)) * 10);

        return ra;
    }

    /**
     * DEC in the format sDD MM SS.ff
     */
    Declination parseHorizonDEC(const std::string& dec_string)
    {
        Declination dec;

        dec.setDegrees(std::stoi(dec_string.substr(1, 2)) * ((dec_string[0] == '-') ? -1 : 1));
        dec.setMinutes(std::stoi(dec_string.substr(4, 2)));
        dec.setSeconds(std::stod(dec_string.substr(7, 5)));
        dec.setMilliseconds(std::stoi(dec_string.substr(10, 2)) * 10);

        return dec;
    }

    DateTime date;
    EquatorialCoordinate equatorial;
public:
    HorizonEphemerisDataLine() : date(DateTime()), equatorial(EquatorialCoordinate()) {}
    HorizonEphemerisDataLine(std::string& line) :
        date(parseHorizonDateTime(line.substr(1, 17))), 
        equatorial(
            parseHorizonDEC(line.substr(35, 11)),
            parseHorizonRA(line.substr(23, 11))
        )
    {}
    HorizonEphemerisDataLine(const HorizonEphemerisDataLine& other) : date(other.getDate()), equatorial(other.getEquatorial()) {}

    DateTime getDate() const { return date; }
    EquatorialCoordinate getEquatorial() const { return equatorial; }

    HorizonEphemerisDataLine& operator=(const HorizonEphemerisDataLine& other) {
        date = other.getDate();
        equatorial = other.getEquatorial();
        return *this;
    }
};



class ParsedHorizonGeneratedEphemeris
{

    HorizonEphemerisDataLine* ephemeris_data;

    unsigned long size;
public:
    ParsedHorizonGeneratedEphemeris(HorizonGeneratedEphemeris& generated_ephemeris) :
        size(generated_ephemeris.getRawEphemerisData().size()),
        ephemeris_data(new HorizonEphemerisDataLine[size])
    {
        for (unsigned int i = 0; i < size; i++)
        {
            ephemeris_data[i] = HorizonEphemerisDataLine(generated_ephemeris.getRawEphemerisData()[i]);
        }
    }

    HorizonEphemerisDataLine* getEphemerisData() const { return ephemeris_data; }

    unsigned long getSize() const { return size; }
};

class HorizonGeneratedEphemeris
{
    std::vector<std::string> raw_ephemeris_data;
public:
    HorizonGeneratedEphemeris(HorizonOutputFile& output_file) : raw_ephemeris_data(std::vector<std::string>(366))
    {
        std::string line;
        // Open output file
        std::ifstream file(output_file.getFilePath());
        if (!file.is_open())
        {
            throw std::runtime_error("Failed to open output file");
        }
        // Find start of ephemeris
        while (std::getline(file, line))
        {
            if (line == "$$SOE")
            {
                break;
            }
        }
        // Traverse
        while (std::getline(file, line))
        {
            if (line == "$$EOE")
            {
                break;
            }
            // Store in raw_ephemeris_data
            raw_ephemeris_data.push_back(line);
        }
    }

    std::vector<std::string> getRawEphemerisData() const { return raw_ephemeris_data; }
};

class HorizonOutputFile
{
    const std::string file_path;
public:
    HorizonOutputFile(const std::string file_path) : file_path(file_path) {}

    std::string getFilePath() const { return file_path; }
};