#pragma once

#include <string>

class Time
{
    /**
     * @var bitmask
     * @brief Bitmask for the time.
     * 
     * Bit allocation (16 total):
     * - 5 bits for the hour (24)
     * - 6 bits for the minute (60)
     * - 5 unused bits
     */
    uint16_t bitmask;
public:
    Time();
    Time(const uint8_t hour, const uint8_t minute);
    Time(const Time& time);
    // HH:MM
    Time(const std::string& time);

    uint16_t get_bitmask() const;
    uint8_t get_hour() const;
    uint8_t get_minute() const;

    void set_hour(const uint8_t hour);
    void set_minute(const uint8_t minute);

    void increment_hour();
    void increment_minute();

    std::string to_string() const;
};