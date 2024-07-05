#pragma once


#include <string>

enum MONTH : uint8_t
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

inline std::string month_to_string(const MONTH month);

inline MONTH int_to_month(const uint8_t month);

class Date
{
    /**
     * @var bitmask
     * @brief Bitmask for the date.
     * 
     * Bit allocation (24 total):
     * - 11 bits for the year (2000)
     * - 4 bits for the month (12)
     * - 5 bits for the day (31)
     */
    uint32_t bitmask;
public:
    Date();
    Date(const uint16_t year, const MONTH month, const uint8_t day);
    Date(const Date& date);
    // MM:DD:YYYY
    Date(const std::string& date);

    uint32_t get_bitmask() const;
    uint16_t get_year() const;
    MONTH get_month() const;
    uint8_t get_day() const;

    void set_year(const uint16_t year);
    void set_month(const MONTH month);
    void set_day(const uint8_t day);

    void increment_day();

    std::string to_string() const;
    std::string to_string_url_encoded() const;
};