/**
 * @file dates.hpp
 * @brief Date and time encoding.
 * 
 * @note Space is always allocated for February 29th. For non-leap years, this space is not used.
 */

#pragma once

enum MONTH : unsigned char
{
    JANUARY,
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
    unsigned int bitmask;
public:
    Date(unsigned int bitmask = 0) : bitmask(bitmask) {}
    Date(const Date& other) : bitmask(other.getBitmask()) {}
    Date(signed int year, unsigned char month, unsigned char day) : bitmask((year < 0) ? (1 << 1) | ((-year & 0xFFFFF) << 1) : (year & 0xFFFFF) << 1) {
        bitmask |= (month << 21);
        bitmask |= (day << 25);
    }

    // Getters
    unsigned int getBitmask() const { return bitmask; }
    signed int getYear() const { return ((bitmask & 1) == 0) ? -(bitmask >> 1) & 0xFFFFF : (bitmask >> 1) & 0xFFFFF; }
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
    void setMonth(unsigned char month) { bitmask = (bitmask & ~(0xF << 21)) | (month << 21); }
    void setDay(unsigned char day) { bitmask = (bitmask & ~(0x1F << 25)) | (day << 25); }
    Date& operator=(const Date& other) {
        if (this != &other) {
            bitmask = other.getBitmask();
        }
        return *this;
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

template<typename T>
class Year
{
    T[366] day_array;
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