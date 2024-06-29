#pragma once

#include "../dates.hpp"

#include <string>

const std::string CACHE_PATH = "cache/";

class FilePath
{
public:
    virtual std::string getFileName() const = 0;
    virtual std::string getPath() const = 0;
};

struct YearFileID
{
    std::string object;
    Date date;
};

class YearSaveFilePath : public FilePath
{
    const YearFileID& id;

    const std::string YEAR_FILE_EXT = ".horizonyear";
public:
    YearSaveFilePath(const YearFileID& id) : id(id) {};

    std::string getFileName() const override {
        if (id.date.getYear() < 0) {
            return id.object + "-" + std::to_string(-id.date.getYear()) + "bc" + YEAR_FILE_EXT;
        }
        return id.object + "-" + std::to_string(id.date.getYear()) + "ad" + YEAR_FILE_EXT;
    };
    std::string getPath() const override {
        return CACHE_PATH + getFileName();
    };

    YearFileID getID() const {
        return id;
    };
};

struct DayFileID
{
    std::string object;
    Date date; // YYYY<ad/bc>-Mon-DD
};

class DaySaveFilePath : public FilePath
{
    DayFileID id;

    const std::string DAY_FILE_EXT = ".horizonday";
public:
    DaySaveFilePath() : id(*(new DayFileID())) {};
    DaySaveFilePath(DayFileID id) : id(id) {};

    std::string getFileName() const override {
        return id.object + "-" + id.date.dayRequestStartString() + DAY_FILE_EXT;
    };
    std::string getPath() const override {
        return CACHE_PATH + getFileName();
    };

    DayFileID getID() const {
        return id;
    };

    DaySaveFilePath& operator=(const DaySaveFilePath& other) {
        if (this == &other) {
            return *this;
        }
        id = other.id;
        return *this;
    }
};