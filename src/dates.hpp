/**
 * @file dates.hpp
 * @brief Date and time encoding.
 * 
 * @note Space is always allocated for February 29th. For non-leap years, this space is not used.
 */

#pragma once

#include <string>
#include <unordered_map>

enum MONTH : unsigned char
{
    ERR_MONTH = 0,
    JANUARY = 1,
    FEBRUARY,
    MARCH,
    APRIL,
    MAY,
    JUNE,
    JULY,
    AUGUST,
    SEPTEMBER,
    OCTOBER,
    NOVEMBER,
    DECEMBER
};

const std::string month_strings[] = {
    "EMO",
    "Jan",
    "Feb",
    "Mar",
    "Apr",
    "May",
    "Jun",
    "Jul",
    "Aug",
    "Sep",
    "Oct",
    "Nov",
    "Dec"
};

class Date
{
    /**
     * @var bitmask
     * @brief Bitmask for the date.
     * 
     * Bit allocation:
     * - 0 (1, 1): AD/BC (0 = BC, 1 = AD)
     * - 1-20 (20, 1048575): Year (1 - 999999)
     * - 21-24 (4, 15): Month (1 - 12)
     * - 25-29 (5, 31): Day (1 - 31)
     * - 30-31 (2, 3): Not used
     */
    uint32_t bitmask;
public:
    Date(unsigned int year = 0, MONTH month = ERR_MONTH, unsigned char day = 0, bool ad = true) : bitmask(0) {
        setYear(year);
        setMonth(month);
        setDay(day);
        bitmask |= (ad ? 1 : 0);
    }
    Date(const Date& other) : bitmask(other.getBitmask()) {}

    // Getters
    unsigned int getBitmask() const { return bitmask; }
    unsigned int getYear() const { return (bitmask >> 1) & 0xFFFFF; }
    bool getAD() const { return (bitmask & 1) == 1; }
    MONTH getMonth() const {
        switch ((bitmask >> 21) & 0xF) {
            case 1:
            return JANUARY;
            case 2:
            return FEBRUARY;
            case 3:
            return MARCH;
            case 4:
            return APRIL;
            case 5:
            return MAY;
            case 6:
            return JUNE;
            case 7:
            return JULY;
            case 8:
            return AUGUST;
            case 9:
            return SEPTEMBER;
            case 10:
            return OCTOBER;
            case 11:
            return NOVEMBER;
            case 12:
            return DECEMBER;
            default:
            return JANUARY;
        }
    }
    unsigned char getDay() const { return (bitmask >> 25) & 0x1F; }

    // Setters
    void setBitmask(unsigned int bitmask) { this->bitmask = bitmask; }
    void setYear(signed int year) { bitmask = (year < 0) ? (1 << 1) | ((-year & 0xFFFFF) << 1) : (year & 0xFFFFF) << 1; }
    void setMonth(MONTH month) { bitmask = (bitmask & ~(0xF << 21)) | (month << 21); }
    void setDay(unsigned char day) { bitmask = (bitmask & ~(0x1F << 25)) | (day << 25); }
    Date& operator=(const Date& other) {
        if (this != &other) {
            bitmask = other.getBitmask();
        }
        return *this;
    }

    // Formatting
    std::string yearRequestStartString() const {
        return std::to_string(getYear()) + ((bitmask & 1) == 0 ? "bc" : "ad");
    }
    std::string yearRequestEndString() const {
        if ((bitmask & 1) == 0) {
            return std::to_string(getYear() + 1) + "ad";
        }
        return std::to_string(getYear() - 1) + "bc";
    }
    std::string dayRequestStartString() const {
        return std::to_string(getYear()) + ((bitmask & 1) == 0 ? "bc" : "ad") + "-" + month_strings[getMonth()] + "-" + std::to_string(getDay());
    }
    std::string dayRequestEndString() const {
        std::unordered_map<MONTH, unsigned char> days_in_month = {
            {JANUARY, 31},
            {FEBRUARY, 29},
            {MARCH, 31},
            {APRIL, 30},
            {MAY, 31},
            {JUNE, 30},
            {JULY, 31},
            {AUGUST, 31},
            {SEPTEMBER, 30},
            {OCTOBER, 31},
            {NOVEMBER, 30},
            {DECEMBER, 31}
        };

        MONTH month = getMonth();
        unsigned char day = getDay();
        unsigned int year = getYear();
        if (day == days_in_month.at(month)) { // Overflow Month
            if (month == DECEMBER) { // Overflow Year
                month = JANUARY;
                day = 1;
                if ((bitmask & 1) == 0) {
                    year++;
                } else {
                    year--;
                }
            } else {
                month = static_cast<MONTH>(month + 1);
                day = 1;
            }
        } else { // Next Day
            day++;
        }
        return (std::to_string(year) + ((bitmask & 1) == 0 ? "bc" : "ad") + "-" + std::to_string(month) + "-" + std::to_string(day));
    }
};

class DateTime
{
    /**
     * @var bitmask
     * @brief Bitmask for the date and time.
     * 
     * Bit allocation:
     * - 0 (1, 1): AD/BC (0 = BC, 1 = AD)
     * - 1-20 (20, 1048575): Year (1 - 999999)
     * - 21-24 (4, 15): Month (1 - 12)
     * - 25-29 (5, 31): Day (1 - 31)
     * - 30-34 (5, 31): Hour (0 - 23)
     * - 35-40 (6, 63): Minute (0 - 59)
     * - 41-46 (6, 63): Second (0 - 59)
     * - 47-49 (3, 7): Millisecond (0 - 999)
     * - 50-63 (14, 16383): Not used
     */
    uint64_t bitmask;
public:
    DateTime(signed int year = 0, unsigned char month = 0, unsigned char day = 0, unsigned char hour = 0, unsigned char minute = 0, unsigned char second = 0, unsigned short millisecond = 0) : bitmask(((year < 0) ? (1 << 1) | ((-year & 0xFFFFF) << 1) : (year & 0xFFFFF) << 1) | (month << 21) | (day << 25) | (hour << 30) | (minute << 35) | (second << 41) | (millisecond << 47)) {}
    DateTime(const DateTime& other) : bitmask(other.getBitmask()) {}
    DateTime(std::string file_bitmask) : bitmask(0) {
        for (unsigned int i = 0; i < 8; i++) {
            bitmask |= file_bitmask[i] << (i * 8);
        }
    }

    // Getters
    unsigned long long getBitmask() const { return bitmask; }
    unsigned int getYear() const { return (bitmask >> 1) & 0xFFFFF; }
    bool getAD() const { return (bitmask & 1) == 1; }
    MONTH getMonth() const {
        switch ((bitmask >> 21) & 0xF)
        {
        case 1:
            return JANUARY;
        case 2:
            return FEBRUARY;
        case 3:
            return MARCH;
        case 4:
            return APRIL;
        case 5:
            return MAY;
        case 6:
            return JUNE;
        case 7:
            return JULY;
        case 8:
            return AUGUST;
        case 9:
            return SEPTEMBER;
        case 10:
            return OCTOBER;
        case 11:
            return NOVEMBER;
        case 12:
            return DECEMBER;
        default:
            return ERR_MONTH;
        };
    }
    unsigned char getDay() const { return (bitmask >> 25) & 0x1F; }
    unsigned char getHour() const { return (bitmask >> 30) & 0x1F; }
    unsigned char getMinute() const { return (bitmask >> 35) & 0x3F; }
    unsigned char getSecond() const { return (bitmask >> 41) & 0x3F; }
    unsigned short getMillisecond() const { return (bitmask >> 47) & 0x7FF; }

    // Setters
    void setBitmask(unsigned long long bitmask) { this->bitmask = bitmask; }
    void setYear(unsigned int year) { (year & 0xFFFFF) << 1; }
    void setAD(bool ad) { bitmask = (bitmask & ~1) | (ad ? 1 : 0); }
    void setMonth(MONTH month) { bitmask = (bitmask & ~(0xF << 21)) | (month << 21); }
    void setDay(unsigned char day) { bitmask = (bitmask & ~(0x1F << 25)) | (day << 25); }
    void setHour(unsigned char hour) { bitmask = (bitmask & ~(0x1F << 30)) | (hour << 30); }
    void setMinute(unsigned char minute) { bitmask = (bitmask & ~(0x3F << 35)) | (minute << 35); }
    void setSecond(unsigned char second) { bitmask = (bitmask & ~(0x3F << 41)) | (second << 41); }
    void setMillisecond(unsigned short millisecond) { bitmask = (bitmask & ~(0x7FF << 47)) | (millisecond << 47); }
    DateTime& operator=(const DateTime& other) {
        if (this != &other) {
            bitmask = other.getBitmask();
        }
        return *this;
    }

    // Formatting
    std::string getDateString() const {
        return std::to_string(getYear()) + (getAD() ? " bc" : " ad") + "-" + month_strings[getMonth()] + "-" + std::to_string(getDay());
    }
    std::string getTimeString() const {
        return std::to_string(getHour()) + ":" + std::to_string(getMinute()) + ":" + std::to_string(getSecond()) + "." + std::to_string(getMillisecond());
    }
};

bool isLeapYear(signed int year) {
    return (year % 4 == 0 && year % 100 != 0) || year % 400 == 0;
}


typedef unsigned short YearIndex;

YearIndex toYearIndex(MONTH month, unsigned char day)
{
    switch (month)
    {
    case JANUARY:
        return day - 1;
    case FEBRUARY:
        return 31 + day - 1;
    case MARCH:
        return 31 + 29 + day - 1;
    case APRIL:
        return 31 + 29 + 31 + day - 1;
    case MAY:
        return 31 + 29 + 31 + 30 + day - 1;
    case JUNE:
        return 31 + 29 + 31 + 30 + 31 + day - 1;
    case JULY:
        return 31 + 29 + 31 + 30 + 31 + 30 + day - 1;
    case AUGUST:
        return 31 + 29 + 31 + 30 + 31 + 30 + 31 + day - 1;
    case SEPTEMBER:
        return 31 + 29 + 31 + 30 + 31 + 30 + 31 + 31 + day - 1;
    case OCTOBER:
        return 31 + 29 + 31 + 30 + 31 + 30 + 31 + 31 + 30 + day - 1;
    case NOVEMBER:
        return 31 + 29 + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31 + day - 1;
    case DECEMBER:
        return 31 + 29 + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31 + 30 + day - 1;
    default:
        return 0;
    };
}

struct MonthDay
{
    MONTH month;
    unsigned char day;
};

MonthDay fromYearIndex(YearIndex index)
{
    if (index < 31) { return {JANUARY, static_cast<unsigned char>(index + 1)}; }
    if (index < 31 + 29) { return {FEBRUARY, static_cast<unsigned char>(index - 31 + 1)}; }
    if (index < 31 + 29 + 31) { return {MARCH, static_cast<unsigned char>(index - 31 - 29 + 1)}; }
    if (index < 31 + 29 + 31 + 30) { return {APRIL, static_cast<unsigned char>(index - 31 - 29 - 31 + 1)}; }
    if (index < 31 + 29 + 31 + 30 + 31) { return {MAY, static_cast<unsigned char>(index - 31 - 29 - 31 - 30 + 1)}; }
    if (index < 31 + 29 + 31 + 30 + 31 + 30) { return {JUNE, static_cast<unsigned char>(index - 31 - 29 - 31 - 30 - 31 + 1)}; }
    if (index < 31 + 29 + 31 + 30 + 31 + 30 + 31) { return {JULY, static_cast<unsigned char>(index - 31 - 29 - 31 - 30 - 31 - 30 + 1)}; }
    if (index < 31 + 29 + 31 + 30 + 31 + 30 + 31 + 31) { return {AUGUST, static_cast<unsigned char>(index - 31 - 29 - 31 - 30 - 31 - 30 - 31 + 1)}; }
    if (index < 31 + 29 + 31 + 30 + 31 + 30 + 31 + 31 + 30) { return {SEPTEMBER, static_cast<unsigned char>(index - 31 - 29 - 31 - 30 - 31 - 30 - 31 - 31 + 1)}; }
    if (index < 31 + 29 + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31) { return {OCTOBER, static_cast<unsigned char>(index - 31 - 29 - 31 - 30 - 31 - 30 - 31 - 31 - 30 + 1)}; }
    if (index < 31 + 29 + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31 + 30) { return {NOVEMBER, static_cast<unsigned char>(index - 31 - 29 - 31 - 30 - 31 - 30 - 31 - 31 - 30 - 31 + 1)}; }
    if (index < 31 + 29 + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31 + 30 + 31) { return {DECEMBER, static_cast<unsigned char>(index - 31 - 29 - 31 - 30 - 31 - 30 - 31 - 31 - 30 - 31 - 30 + 1)}; }
    return {ERR_MONTH, 0};
}

template<typename T>
class Year
{
    T day_array[366];
public:
    Year() {};

    T getDay(YearIndex index) const {
        return day_array[index];
    }

    void setDay(YearIndex index, T& value) {
        day_array[index] = value;
    }

    T& operator[](YearIndex index) {
        return day_array[index];
    }
};