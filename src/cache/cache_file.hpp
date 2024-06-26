#pragma once

#include "../horizons/horizon_output_parsing.hpp"
#include "filenames.hpp"

#include <string>
#include <iostream>

/**
 * Write data as a csv file with the .horizonday extension
 * 
 * DateTime, RA, DEC
 */
void writeCacheFile(ParsedHorizonGeneratedEphemeris& pephem, const std::string file_path)
{
    // Open file
    std::ofstream file(file_path);
    if (!file.is_open()) {
        // Handle error if file cannot be opened
        std::cerr << "Failed to open file: " << file_path << std::endl;
        return;
    }
    // Write header
    file << "DateTime, RA, DEC" << std::endl;
    // Write data
    // For each data line
    for (unsigned int i = 0; i < pephem.getSize(); i++)
    {
        HorizonEphemerisDataLine& data = pephem.getEphemerisData()[i];
        // Write data line
        file << data.getDate().getBitmask() << ','
             << data.getEquatorial().getRightAscension().getBitmask() << ','
             << data.getEquatorial().getDeclination().getBitmask() << std::endl;
    }
}