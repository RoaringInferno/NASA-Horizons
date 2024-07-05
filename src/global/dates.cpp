#include "dates.hpp"
#include "url.hpp"

const std::string MONTH_STRINGS[] = {
    "ERR_MONTH",
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

inline std::string month_to_string(const MONTH month)
{
    return MONTH_STRINGS[month];
}

inline MONTH int_to_month(const uint8_t month)
{
    return static_cast<MONTH>(month);
}

Date::Date() : bitmask(0)
{
}

Date::Date(const uint16_t year, const MONTH month, const uint8_t day) :
    bitmask((year << 18) | (static_cast<uint32_t>(month) << 14) | (day << 9))
{
}

Date::Date(const Date &date) : bitmask(date.get_bitmask())
{
}

Date::Date(const std::string &date) : bitmask(0)
{
    this->set_month(int_to_month(std::stoi(date.substr(0, 2))));
    this->set_day(std::stoi(date.substr(3, 2)));
    this->set_year(std::stoi(date.substr(6, 4)));
}

uint32_t Date::get_bitmask() const
{
    return this->bitmask;
}

uint16_t Date::get_year() const
{
    return this->bitmask >> 18;
}

MONTH Date::get_month() const
{
    return int_to_month((this->bitmask >> 14) & 0x0F);
}

uint8_t Date::get_day() const
{
    return (this->bitmask >> 9) & 0x1F;
}

void Date::set_year(const uint16_t year)
{
    this->bitmask = (this->bitmask & 0x0003FFFF) | (year << 18);
}

void Date::set_month(const MONTH month)
{
    this->bitmask = (this->bitmask & 0xFFFC3FFF) | (static_cast<uint32_t>(month) << 14);

}

void Date::set_day(const uint8_t day)
{
    this->bitmask = (this->bitmask & 0xFFFFFE1F) | (day << 9);
}

void Date::increment_day()
{
    uint8_t day = this->get_day();
    if (day == 31)
    {
        this->set_day(1);
        if (this->get_month() == DECEMBER)
        {
            this->set_month(JANUARY);
            this->set_year(this->get_year() + 1);
            return;
        }
        this->set_month(static_cast<MONTH>(this->get_month() + 1));
        return;
    }
    this->set_day(day + 1);
}

std::string Date::to_string() const
{
    return std::to_string(this->get_year()) + ":" + month_to_string(this->get_month()) + ":" + std::to_string(this->get_day());
}

std::string Date::to_string_url_encoded() const
{
    return std::to_string(this->get_year()) + URL_COLON + month_to_string(this->get_month()) + URL_COLON + std::to_string(this->get_day());
}
