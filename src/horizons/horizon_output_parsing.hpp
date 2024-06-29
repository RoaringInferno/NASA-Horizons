#pragma once

#include "../dates.hpp"
#include "../coordinates/equatorial.hpp"
#include "../debugging.hpp"

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
        //DEBUG_PRINT("\t\tParsing Date: " << date_string << ".")
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

        date.setAD(!(date_string[0] == 'b'));
        date.setYear(std::stoi(date_string.substr(1, 4)));
        date.setMonth(month_hash(date_string.substr(6, 3)));
        date.setDay(std::stoi(date_string.substr(9, 2)));
        date.setHour(std::stoi(date_string.substr(13, 2)));
        date.setMinute(std::stoi(date_string.substr(16, 2)));

        return date;
        
    }

    /**
     * RA in the format" HH MM SS.ff"
     */
    RightAscension parseHorizonRA(const std::string& ra_string)
    {
        //DEBUG_PRINT("\t\tParsing RA: " << ra_string << ".")
        RightAscension ra;

        ra.setDegrees(std::stoi(ra_string.substr(0, 2)) * 15);
        ra.setMinutes(std::stoi(ra_string.substr(3, 2)));
        ra.setSeconds(std::stod(ra_string.substr(6, 2)));
        ra.setMilliseconds(0);
        //ra.setMilliseconds(std::stoi(ra_string.substr(9, 2)) * 10);

        return ra;
    }

    /**
     * DEC in the format "sDD MM SS.ff"
     */
    Declination parseHorizonDEC(const std::string& dec_string)
    {
        //DEBUG_PRINT("\t\tParsing DEC: " << dec_string << ".")
        Declination dec;

        dec.setDegrees(std::stoi(dec_string.substr(1, 2)) * ((dec_string[0] == '-') ? -1 : 1));
        dec.setMinutes(std::stoi(dec_string.substr(4, 2)));
        dec.setSeconds(std::stod(dec_string.substr(7, 2)));
        dec.setMilliseconds(0);
        //dec.setMilliseconds(std::stoi(dec_string.substr(10, 1)) * 10);

        return dec;
    }

    DateTime date;
    EquatorialCoordinate equatorial;
public:
    HorizonEphemerisDataLine() : date(DateTime()), equatorial(EquatorialCoordinate()) {}
    HorizonEphemerisDataLine(std::string& line) :
        date(parseHorizonDateTime(line.substr(0, 18))), 
        equatorial(
            parseHorizonDEC(line.substr(60, 11)),
            parseHorizonRA(line.substr(48, 11))
        )
    {
        //DEBUG_PRINT("Parsed Line: " << line << " as " << date.getBitmask() << "|" << equatorial.getRightAscension().getBitmask() << "-" << equatorial.getDeclination().getBitmask());
    }
    HorizonEphemerisDataLine(const HorizonEphemerisDataLine& other) : date(other.getDate()), equatorial(other.getEquatorial()) {}

    DateTime getDate() const { return date; }
    EquatorialCoordinate getEquatorial() const { return equatorial; }

    HorizonEphemerisDataLine& operator=(const HorizonEphemerisDataLine& other) {
        date = other.getDate();
        equatorial = other.getEquatorial();
        return *this;
    }
};

class HorizonOutputFile
{
    const std::string file_path;
public:
    HorizonOutputFile(const std::string file_path) : file_path(file_path) {}

    std::string getFilePath() const { return file_path; }
};

class HorizonGeneratedEphemeris
{
    std::vector<std::string> raw_ephemeris_data;
public:
    HorizonGeneratedEphemeris(HorizonOutputFile& output_file) :
        raw_ephemeris_data(std::vector<std::string>())
    {
        DEBUG_PRINT("Reading Ephemeris Data from " << output_file.getFilePath());

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
            //DEBUG_PRINT("Stored Line: " << line);
        }
    }

    std::vector<std::string> getRawEphemerisData() const { return raw_ephemeris_data; }

    std::string& operator[](unsigned long index) { return raw_ephemeris_data[index]; }
};

class ParsedHorizonGeneratedEphemeris
{

    HorizonEphemerisDataLine* ephemeris_data;

    unsigned long size;
public:
    ParsedHorizonGeneratedEphemeris(HorizonGeneratedEphemeris& generated_ephemeris) :
        size(generated_ephemeris.getRawEphemerisData().size()),
        ephemeris_data(new HorizonEphemerisDataLine[generated_ephemeris.getRawEphemerisData().size()])
    {
        DEBUG_PRINT("Size: " << size);
        for (unsigned int i = 0; i < size; i++)
        {
            //DEBUG_PRINT("Parsing Line " << i+1 << " of " << size);
            //DEBUG_PRINT("Line: " << generated_ephemeris[i] << ".");
            ephemeris_data[i] = HorizonEphemerisDataLine(generated_ephemeris[i]);
        }
    }

    HorizonEphemerisDataLine* getEphemerisData() const { return ephemeris_data; }

    unsigned long getSize() const { return size; }
};