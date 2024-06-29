#pragma once

#include "../horizons/horizon_output_parsing.hpp"
#include "filenames.hpp"
#include "../debugging.hpp"

#include <string>
#include <iostream>

/**
 * Write data as a file with the .horizonday extension
 * 
 * DateTime, RA, DEC
 */
void writeCacheFile(ParsedHorizonGeneratedEphemeris& pephem, const std::string file_path)
{
    DEBUG_PRINT("\t\tWriting Cache File: " << file_path);
    // Open file
    std::ofstream file(file_path);
    if (!file.is_open()) {
        // Handle error if file cannot be opened
        std::cerr << "Failed to open file: " << file_path << std::endl;
        return;
    }
    // Write data
    // For each data line
    for (unsigned int i = 0; i < pephem.getSize(); i++)
    {
        HorizonEphemerisDataLine& data = pephem.getEphemerisData()[i];
        // Write data line
        unsigned long long date_bitmask =  data.getDate().getBitmask();
        unsigned int ra_bitmask = data.getEquatorial().getRightAscension().getBitmask();
        unsigned int dec_bitmask = data.getEquatorial().getDeclination().getBitmask();
        file << (char)(date_bitmask & 0xFF) << (char)((date_bitmask >> 8) & 0xFF) << (char)((date_bitmask >> 16) & 0xFF) << (char)((date_bitmask >> 24) & 0xFF) << (char)((date_bitmask >> 32) & 0xFF) << (char)((date_bitmask >> 40) & 0xFF) << (char)((date_bitmask >> 48) & 0xFF) << (char)((date_bitmask >> 56) & 0xFF) << (char)(ra_bitmask & 0xFF) << (char)((ra_bitmask >> 8) & 0xFF) << (char)((ra_bitmask >> 16) & 0xFF) << (char)((ra_bitmask >> 24) & 0xFF) << (char)(dec_bitmask & 0xFF) << (char)((dec_bitmask >> 8) & 0xFF) << (char)((dec_bitmask >> 16) & 0xFF) << (char)((dec_bitmask >> 24) & 0xFF) << std::endl;
    }
}

class CacheFileLine
{
    // DateTime, RA, DEC
    const DateTime date;
    const RightAscension ra;
    const Declination dec;
public:
    CacheFileLine(std::string& line) :
        date(DateTime(line.substr(0, 8))),
        ra(RightAscension(line.substr(8, 4))),
        dec(Declination(line.substr(16, 4)))
    {
        DEBUG_PRINT("\nParsed Line: " << line << " as " << date.getBitmask() << "|" << ra.getBitmask() << "-" << dec.getBitmask());
        DEBUG_PRINT("\tas " << std::to_string(date.getHour()) << ":" << std::to_string(date.getMinute()) << ":" << " | " << std::to_string(ra.getDegrees()) << "d" << std::to_string(ra.getMinutes()) << "m" << std::to_string(ra.getSeconds()) << "s" << std::to_string(ra.getMilliseconds()) << "ms " << " " << std::to_string(dec.getDegrees()) << "d" << std::to_string(dec.getMinutes()) << "m" << std::to_string(dec.getSeconds()) << "s" << std::to_string(dec.getMilliseconds()) << "ms");
        DEBUG_PRINT("\n");
    };

    // Getters
    DateTime getDate() const { return date; }
    RightAscension getRA() const { return ra; }
    Declination getDEC() const { return dec; }
};