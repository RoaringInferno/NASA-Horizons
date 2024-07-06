#pragma once

#include <cstdint>

class EquatorialCoordinates
{
    uint8_t RA_hour, RA_minute, RA_second, DEC_degree, DEC_minute, DEC_second;
    bool DEC_sign;
public:
    EquatorialCoordinates();
    EquatorialCoordinates(const EquatorialCoordinates &other);
    EquatorialCoordinates(uint8_t RA_hour, uint8_t RA_minute, uint8_t RA_second, bool DEC_sign, uint8_t DEC_degree, uint8_t DEC_minute, uint8_t DEC_second);

    void setRA_hour(uint8_t hour);
    void setRA_minute(uint8_t minute);
    void setRA_second(uint8_t second);

    void setDEC_sign(bool sign);
    void setDEC_degree(uint8_t degree);
    void setDEC_minute(uint8_t minute);
    void setDEC_second(uint8_t second);

    void setRA(uint8_t hour, uint8_t minute, uint8_t second);
    void setDEC(bool sign, uint8_t degree, uint8_t minute, uint8_t second);

    uint32_t get_RA_bitmask() const;
    uint8_t getRA_hour() const;
    uint8_t getRA_minute() const;
    uint8_t getRA_second() const;

    uint32_t get_DEC_bitmask() const;
    bool getDEC_sign() const;
    uint8_t getDEC_degree() const;
    uint8_t getDEC_minute() const;
    uint8_t getDEC_second() const;
};