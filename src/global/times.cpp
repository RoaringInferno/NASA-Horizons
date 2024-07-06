#include "times.hpp"

Time::Time() :
    bitmask(0)
{
}

Time::Time(const uint8_t hour, const uint8_t minute) :
    bitmask((hour << 6) | minute)
{
}

Time::Time(const Time &time) :
    bitmask(time.get_bitmask())
{
}

Time::Time(const std::string &time) :
    bitmask(0)
{
    this->set_hour(std::stoi(time.substr(0, 2)));
    this->set_minute(std::stoi(time.substr(3, 2)));
}

uint16_t Time::get_bitmask() const
{
    return this->bitmask;
}

uint8_t Time::get_hour() const
{
    return this->bitmask >> 6 & 0x1F;
}

uint8_t Time::get_minute() const
{
    return this->bitmask & 0x3F;
}

void Time::set_hour(const uint8_t hour)
{
    this->bitmask = (hour << 6) | this->get_minute();
}

void Time::set_minute(const uint8_t minute)
{
    this->bitmask = (this->get_hour() << 6) | minute;
}

void Time::increment_hour()
{
    uint8_t hour = this->get_hour();
    if (hour == 23)
    {
        this->set_hour(0);
    }
    else
    {
        this->set_hour(hour + 1);
    }
}

void Time::increment_minute()
{
    uint8_t minute = this->get_minute();
    if (minute == 59)
    {
        this->set_minute(0);
        this->increment_hour();
    }
    else
    {
        this->set_minute(minute + 1);
    }
}

std::string Time::to_string() const
{
    return std::to_string(this->get_hour()) + ":" + std::to_string(this->get_minute());
}
