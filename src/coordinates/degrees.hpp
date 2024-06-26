/**
 * @file degrees.hpp
 * @brief Degrees-based coordinate systems.
 */

#pragma once
#include <math.h>

class CircularDegreeCoordinate
{
    /**
     * @var bitmask
     * @brief Bitmask for the circular degree coordinate.
     * 
     * Bit allocation:
     * - 0-8 (9, 511) : Degrees(0-360)
     * - 7-14 (6, 63) : Minutes(0-59)
     * - 15-20 (6, 63): Seconds(0-59)
     * - 21-30 (10, 1023): Milliseconds(0-999)
     * - 31 (1, 1): Not used
     */
    unsigned int bitmask;
public:
    CircularDegreeCoordinate() : bitmask(0) {}
    CircularDegreeCoordinate(long double radians) {
        long double degrees = radians * 180.0 / M_PI;
        unsigned short intDegrees = (unsigned short)degrees;
        unsigned char minutes = (unsigned char)((degrees - intDegrees) * 60.0);
        unsigned char seconds = (unsigned char)(((degrees - intDegrees) * 60.0 - minutes) * 60.0);
        unsigned short milliseconds = (unsigned short)((((degrees - intDegrees) * 60.0 - minutes) * 60.0 - seconds) * 1000.0);
        setCoordinate(intDegrees, minutes, seconds, milliseconds);
    };
    CircularDegreeCoordinate(unsigned short degrees, unsigned char minutes, unsigned char seconds, unsigned short milliseconds) {
        setCoordinate(degrees, minutes, seconds, milliseconds);
    }
    CircularDegreeCoordinate(const CircularDegreeCoordinate& other) : bitmask(other.getBitmask()) {}

    // Getters
    unsigned int getBitmask() const { return bitmask; }
    unsigned short getDegrees() const { return bitmask & 0x1FF; }
    unsigned char getMinutes() const { return (bitmask >> 9) & 0x3F; }
    unsigned char getSeconds() const { return (bitmask >> 15) & 0x3F; }
    unsigned short getMilliseconds() const { return (bitmask >> 21) & 0x3FF; }
    long double asDecimal() const { return getDegrees() + getMinutes() / 60.0 + getSeconds() / 3600.0 + getMilliseconds() / 3600000.0; }
    long double asRadians() const { return asDecimal() * M_PI / 180.0; }

    // Setters
    void setBitmask(unsigned int bitmask) { this->bitmask = bitmask; }
    void setCoordinate(unsigned short degrees, unsigned char minutes, unsigned char seconds, unsigned short milliseconds) {
        bitmask = (degrees & 0x1FF) | ((minutes & 0x3F) << 9) | ((seconds & 0x3F) << 15) | ((milliseconds & 0x3FF) << 21);
    }
    void setDegrees(unsigned short degrees) { bitmask = (bitmask & 0xFFFFFE00) | (degrees & 0x1FF); }
    void setMinutes(unsigned char minutes) { bitmask = (bitmask & 0xFFFF01FF) | ((minutes & 0x3F) << 9); }
    void setSeconds(unsigned char seconds) { bitmask = (bitmask & 0xFFFC1FFF) | ((seconds & 0x3F) << 15); }
    void setMilliseconds(unsigned short milliseconds) { bitmask = (bitmask & 0xFFE003FF) | ((milliseconds & 0x3FF) << 21); }
    CircularDegreeCoordinate& operator=(const CircularDegreeCoordinate& other) {
        if (this != &other) {
            bitmask = other.getBitmask();
        }
        return *this;
    }
};

class VerticalDegreeCoordinate
{
    /**
     * @var bitmask
     * @brief Bitmask for the vertical degree coordinate.
     * 
     * Bit allocation:
     * - 0 (1, 1): Sign(0: +, 1: -)
     * - 1-7 (7, 127) : Degrees(0-90)
     * - 8-13 (6, 63) : Minutes(0-59)
     * - 14-19 (6, 63): Seconds(0-59)
     * - 20-29 (10, 1023): Milliseconds(0-999)
     * - 30-31 (2, 3): Not used
     */
    unsigned int bitmask;
public:
    VerticalDegreeCoordinate() : bitmask(0) {}
    VerticalDegreeCoordinate(long double radians) {
        bool sign = radians < 0;
        long double degrees = fabs(radians) * 180.0 / M_PI;
        unsigned char intDegrees = (unsigned char)degrees;
        unsigned char minutes = (unsigned char)((degrees - intDegrees) * 60.0);
        unsigned char seconds = (unsigned char)(((degrees - intDegrees) * 60.0 - minutes) * 60.0);
        unsigned short milliseconds = (unsigned short)((((degrees - intDegrees) * 60.0 - minutes) * 60.0 - seconds) * 1000.0);
        setCoordinate(sign, intDegrees, minutes, seconds, milliseconds);
    }
    VerticalDegreeCoordinate(bool sign, unsigned char degrees, unsigned char minutes, unsigned char seconds, unsigned short milliseconds) {
        setCoordinate(sign, degrees, minutes, seconds, milliseconds);
    }
    VerticalDegreeCoordinate(const VerticalDegreeCoordinate& other) : bitmask(other.getBitmask()) {}

    // Getters
    unsigned int getBitmask() const { return bitmask; }
    bool getSign() const { return bitmask & 0x1; }
    unsigned char getDegrees() const { return (bitmask >> 1) & 0x7F; }
    unsigned char getMinutes() const { return (bitmask >> 8) & 0x3F; }
    unsigned char getSeconds() const { return (bitmask >> 14) & 0x3F; }
    unsigned short getMilliseconds() const { return (bitmask >> 20) & 0x3FF; }
    long double asDecimal() const { return (getSign() ? -1 : 1) * (getDegrees() + getMinutes() / 60.0 + getSeconds() / 3600.0 + getMilliseconds() / 3600000.0); }
    long double asRadians() const { return asDecimal() * M_PI / 180.0; }

    // Setters
    void setBitmask(unsigned int bitmask) { this->bitmask = bitmask; }
    void setCoordinate(bool sign, unsigned char degrees, unsigned char minutes, unsigned char seconds, unsigned short milliseconds) {
        bitmask = (sign ? 0x1 : 0x0) | ((degrees & 0x7F) << 1) | ((minutes & 0x3F) << 8) | ((seconds & 0x3F) << 14) | ((milliseconds & 0x3FF) << 20);
    }
    void setSign(bool sign) { bitmask = (bitmask & 0xFFFFFFFE) | (sign ? 0x1 : 0x0); }
    void setDegrees(unsigned char degrees) { bitmask = (bitmask & 0xFFFFFF80) | ((degrees & 0x7F) << 1); }
    void setMinutes(unsigned char minutes) { bitmask = (bitmask & 0xFFFFC07F) | ((minutes & 0x3F) << 8); }
    void setSeconds(unsigned char seconds) { bitmask = (bitmask & 0xFFFC3FFF) | ((seconds & 0x3F) << 14); }
    void setMilliseconds(unsigned short milliseconds) { bitmask = (bitmask & 0xFF003FFF) | ((milliseconds & 0x3FF) << 20); }
    VerticalDegreeCoordinate& operator=(const VerticalDegreeCoordinate& other) {
        if (this != &other) {
            bitmask = other.getBitmask();
        }
        return *this;
    }
};

class DegreeCoordinatePair
{
    VerticalDegreeCoordinate vertical;
    CircularDegreeCoordinate circular;
public:
    DegreeCoordinatePair() {}
    DegreeCoordinatePair(const VerticalDegreeCoordinate& vertical, const CircularDegreeCoordinate& circular) : vertical(vertical), circular(circular) {}
    DegreeCoordinatePair(const DegreeCoordinatePair& other) : vertical(other.getVertical()), circular(other.getCircular()) {}

    // Getters
    VerticalDegreeCoordinate getVertical() const { return vertical; }
    CircularDegreeCoordinate getCircular() const { return circular; }

    // Setters
    void setVertical(const VerticalDegreeCoordinate& vertical) { this->vertical = vertical; }
    void setCircular(const CircularDegreeCoordinate& circular) { this->circular = circular; }
    DegreeCoordinatePair& operator=(const DegreeCoordinatePair& other) {
        if (this != &other) {
            vertical = other.getVertical();
            circular = other.getCircular();
        }
        return *this;
    }
};