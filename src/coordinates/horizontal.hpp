/**
 * @file horizontal.hpp
 * @brief Horizontal coordinate system.
 */

#pragma once

#include "degrees.hpp"

typedef VerticalDegreeCoordinate Altitude;
typedef CircularDegreeCoordinate Azimuth;

class HorizontalCoordinate
{
    Altitude altitude;
    Azimuth azimuth;
public:
    HorizontalCoordinate() {}
    HorizontalCoordinate(const Altitude& altitude, const Azimuth& azimuth) : altitude(altitude), azimuth(azimuth) {}
    HorizontalCoordinate(const HorizontalCoordinate& other) : altitude(other.getAltitude()), azimuth(other.getAzimuth()) {}

    // Getters
    Altitude getAltitude() const { return altitude; }
    Azimuth getAzimuth() const { return azimuth; }

    // Setters
    void setAltitude(const Altitude& altitude) { this->altitude = altitude; }
    void setAzimuth(const Azimuth& azimuth) { this->azimuth = azimuth; }
    HorizontalCoordinate& operator=(const HorizontalCoordinate& other) {
        if (this != &other) {
            altitude = other.getAltitude();
            azimuth = other.getAzimuth();
        }
        return *this;
    }
};