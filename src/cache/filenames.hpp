#pragma once

#include <string>

const std::string CACHE_DIR = "cache/";

struct YearFileID
{
    std::string object;
    std::string year;
};

class YearSaveFilePath
{
    const YearFileID& id;

    const std::string YEAR_FILE_EXT = ".horizonyear";
public:
    YearSaveFilePath(const YearFileID& id) : id(id) {};

    std::string get() const {
        return CACHE_DIR + id.object + "-" + id.year + YEAR_FILE_EXT;
    };
};

struct DayFileID
{
    std::string object;
    std::string date;
};

class DaySaveFilePath
{
    const DayFileID& id;

    const std::string DAY_FILE_EXT = ".horizonday";
public:
    DaySaveFilePath(const DayFileID& id) : id(id) {};

    std::string get() const {
        return CACHE_DIR + id.object + "-" + id.date + DAY_FILE_EXT;
    };
};