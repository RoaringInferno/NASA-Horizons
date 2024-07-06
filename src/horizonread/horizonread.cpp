#include "horizonread.hpp"

#include <fstream>
#include <vector>

#include <iostream>
#include <cassert>

const std::string HorizonEphemeris::CSV_HEADER = "Date,Time,RA Hour,RA Minute,RA Second,DEC Sign,DEC Degree,DEC Minute,DEC Second";

HorizonEphemerisLine::HorizonEphemerisLine() :
    date(),
    time(),
    coordinates()
{
}

HorizonEphemerisLine::HorizonEphemerisLine(const HorizonEphemerisLine &other) :
    date(other.getDate()),
    time(other.getTime()),
    coordinates(other.getCoordinates())
{
}

HorizonEphemerisLine::HorizonEphemerisLine(const std::string &line) :
    date(),
    time(line.substr(13, 5)),
coordinates()
{
    this->date.set_year(std::stoi(line.substr(1, 4)));
    std::string month = line.substr(6, 3);
    MONTH month_enum = ERR_MONTH;
    if (month == "Jan") month_enum = JANUARY;
    else if (month == "Feb") month_enum = FEBRUARY;
    else if (month == "Mar") month_enum = MARCH;
    else if (month == "Apr") month_enum = APRIL;
    else if (month == "May") month_enum = MAY;
    else if (month == "Jun") month_enum = JUNE;
    else if (month == "Jul") month_enum = JULY;
    else if (month == "Aug") month_enum = AUGUST;
    else if (month == "Sep") month_enum = SEPTEMBER;
    else if (month == "Oct") month_enum = OCTOBER;
    else if (month == "Nov") month_enum = NOVEMBER;
    else if (month == "Dec") month_enum = DECEMBER;
    this->date.set_month(month_enum);
    this->date.set_day(std::stoi(line.substr(10, 2)));

    this->coordinates.setRA(
        stoi(line.substr(48, 2)),
        stoi(line.substr(51, 2)),
        stof(line.substr(54, 2))
    );
    this->coordinates.setDEC(
        (line[60] == '+'),
        stoi(line.substr(61, 2)),
        stoi(line.substr(64, 2)),
        stof(line.substr(67, 2))
    );
    assert((line[60] == '+') == this->coordinates.getDEC_sign());
}

std::string HorizonEphemerisLine::writeCSV() const
{
    return
        this->date.to_string() + ","
        + this->time.to_string() + ","

        + std::to_string(this->coordinates.getRA_hour()) + ","
        + std::to_string(this->coordinates.getRA_minute()) + ","
        + std::to_string(this->coordinates.getRA_second()) + ","

        + (this->coordinates.getDEC_sign() ? "+" : "-") + ","
        + std::to_string(this->coordinates.getDEC_degree()) + ","
        + std::to_string(this->coordinates.getDEC_minute()) + ","
        + std::to_string(this->coordinates.getDEC_second()) + ","
    ;
}

EquatorialCoordinates HorizonEphemerisLine::getCoordinates() const
{
    return this->coordinates;
}

Time HorizonEphemerisLine::getTime() const
{
    return this->time;
}

Date HorizonEphemerisLine::getDate() const
{
    return this->date;
}

HorizonEphemeris::HorizonEphemeris(const std::string &file_path) :
    lines(nullptr),
    line_count(0)
{
    // Open file
    std::ifstream file(file_path);
    if (!file.is_open())
    {
        throw std::runtime_error("Could not open file: " + file_path);
    }

    // Jump to the first line of data
    // Begins after "$$SOE" line
    std::string line;
    while (line != "$$SOE")
    {
        std::getline(file, line);
        if (file.eof())
        {
            return;
        }
    }

    // Read the ephemeris into a vector
    std::vector<std::string> raw_lines;
    while (true)
    {
        std::getline(file, line);
        if (line == "$$EOE")
        {
            break;
        }
        raw_lines.push_back(line);
    }

    // Allocate memory for the ephemeris
    this->line_count = raw_lines.size();
    this->lines = new HorizonEphemerisLine[this->line_count];

    // Parse the ephemeris
    for (size_t i = 0; i < this->line_count; i++)
    {
        this->lines[i] = HorizonEphemerisLine(raw_lines[i]);
    }
}

void HorizonEphemeris::writeCSV(const std::string &file_path) const
{
    if (this->lines == nullptr)
    {
        system(("echo \"" + this->CSV_HEADER + "\" >> " + file_path).c_str());
        return;
    }

    // Create the CSV string
    std::string csv = this->CSV_HEADER + "\n";

    for (size_t i = 0; i < this->line_count; i++)
    {
        csv += this->lines[i].writeCSV() + "\n";
    }

    // Write the CSV string to a file
    std::string command = "echo \"" + csv + "\" >> " + file_path;
    system(command.c_str());
}