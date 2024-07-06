#include "equatorial-coordinates.hpp"

EquatorialCoordinates::EquatorialCoordinates() :
    RA_hour(0),
    RA_minute(0),
    RA_second(0),
    DEC_sign(false),
    DEC_degree(0),
    DEC_minute(0),
    DEC_second(0)
{
}

EquatorialCoordinates::EquatorialCoordinates(const EquatorialCoordinates &other) :
    RA_hour(other.getRA_hour()),
    RA_minute(other.getRA_minute()),
    RA_second(other.getRA_second()),
    DEC_sign(other.getDEC_sign()),
    DEC_degree(other.getDEC_degree()),
    DEC_minute(other.getDEC_minute()),
    DEC_second(other.getDEC_second())
{
}

EquatorialCoordinates::EquatorialCoordinates(uint8_t RA_hour, uint8_t RA_minute, uint8_t RA_second, bool DEC_sign, uint8_t DEC_degree, uint8_t DEC_minute, uint8_t DEC_second) :
    RA_hour(RA_hour),
    RA_minute(RA_minute),
    RA_second(RA_second),
    DEC_sign(DEC_sign),
    DEC_degree(DEC_degree),
    DEC_minute(DEC_minute),
    DEC_second(DEC_second)
{
}

void EquatorialCoordinates::setRA_hour(uint8_t hour)
{
    this->RA_hour = hour;
}

void EquatorialCoordinates::setRA_minute(uint8_t minute)
{
    this->RA_minute = minute;
}

void EquatorialCoordinates::setRA_second(uint8_t second)
{
    this->RA_second = second;
}

void EquatorialCoordinates::setDEC_sign(bool sign)
{
    this->DEC_sign = sign;
}

void EquatorialCoordinates::setDEC_degree(uint8_t degree)
{
    this->DEC_degree = degree;
}

void EquatorialCoordinates::setDEC_minute(uint8_t minute)
{
    this->DEC_minute = minute;
}

void EquatorialCoordinates::setDEC_second(uint8_t second)
{
    this->DEC_second = second;
}

void EquatorialCoordinates::setRA(uint8_t hour, uint8_t minute, uint8_t second)
{
    setRA_hour(hour);
    setRA_minute(minute);
    setRA_second(second);
}

void EquatorialCoordinates::setDEC(bool sign, uint8_t degree, uint8_t minute, uint8_t second)
{
    setDEC_sign(sign);
    setDEC_degree(degree);
    setDEC_minute(minute);
    setDEC_second(second);
}

uint32_t EquatorialCoordinates::get_RA_bitmask() const
{
    return (this->RA_hour << 25) | (this->RA_minute << 19) | (this->RA_second << 10);
}

uint8_t EquatorialCoordinates::getRA_hour() const
{
    return this->RA_hour;
}

uint8_t EquatorialCoordinates::getRA_minute() const
{
    return this->RA_minute;
}

uint8_t EquatorialCoordinates::getRA_second() const
{
    return this->RA_second;
}

uint32_t EquatorialCoordinates::get_DEC_bitmask() const
{
    return (this->DEC_sign << 28) | (this->DEC_degree << 19) | (this->DEC_minute << 13) | (this->DEC_second << 7);
}

bool EquatorialCoordinates::getDEC_sign() const
{
    return this->DEC_sign;
}

uint8_t EquatorialCoordinates::getDEC_degree() const
{
    return this->DEC_degree;
}

uint8_t EquatorialCoordinates::getDEC_minute() const
{
    return this->DEC_minute;
}

uint8_t EquatorialCoordinates::getDEC_second() const
{
    return this->DEC_second;
}
