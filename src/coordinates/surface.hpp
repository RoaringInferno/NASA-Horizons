/**
 * @file surface.hpp
 * @brief Surface coordinate system.
 */

#pragma once

#include "degrees.hpp"

typedef VerticalDegreeCoordinate Latitude;
typedef CircularDegreeCoordinate Longitude;

class SurfaceCoordinate
{
    Latitude latitude;
    Longitude longitude;
public:
    SurfaceCoordinate() {}
    SurfaceCoordinate(const Latitude& latitude, const Longitude& longitude) : latitude(latitude), longitude(longitude) {}
    SurfaceCoordinate(const SurfaceCoordinate& other) : latitude(other.getLatitude()), longitude(other.getLongitude()) {}

    // Getters
    Latitude getLatitude() const { return latitude; }
    Longitude getLongitude() const { return longitude; }

    // Setters
    void setLatitude(const Latitude& vertical) { this->latitude = latitude; }
    void setLongitude(const Longitude& longitude) { this->longitude = longitude; }
    SurfaceCoordinate& operator=(const SurfaceCoordinate& other) {
        if (this != &other) {
            latitude = other.getLatitude();
            longitude = other.getLongitude();
        }
        return *this;
    }
};