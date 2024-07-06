#pragma once

#include "times.hpp"
#include "dates.hpp"
#include "equatorial-coordinates.hpp"

#include <string>

class HorizonEphemerisLine
{
private:
    EquatorialCoordinates coordinates;
    Time time;
    Date date;
public:
    HorizonEphemerisLine();
    HorizonEphemerisLine(const HorizonEphemerisLine &other);
    HorizonEphemerisLine(const std::string &line);

    std::string writeCSV() const;

    EquatorialCoordinates getCoordinates() const;
    Time getTime() const;
    Date getDate() const;
};

class HorizonEphemeris
{
private:
    static const std::string CSV_HEADER;

    HorizonEphemerisLine *lines;
    size_t line_count;
public:
    HorizonEphemeris(const std::string &file_path);

    void writeCSV(const std::string &file_path) const;
};