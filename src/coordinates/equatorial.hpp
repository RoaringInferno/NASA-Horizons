/**
 * @file equatorial.hpp
 * @brief Equatorial coordinate system.
 */

#pragma once

#include "degrees.hpp"

typedef VerticalDegreeCoordinate Declination;
typedef CircularDegreeCoordinate RightAscension;

class EquatorialCoordinate {
    Declination declination;
    RightAscension right_ascension;
public:
    EquatorialCoordinate() {}
    EquatorialCoordinate(const Declination& declination, const RightAscension& right_ascension) : declination(declination), right_ascension(right_ascension) {}
    EquatorialCoordinate(const EquatorialCoordinate& other) : declination(other.getDeclination()), right_ascension(other.getRightAscension()) {}

    // Getters
    Declination getDeclination() const { return declination; }
    RightAscension getRightAscension() const { return right_ascension; }

    // Setters
    void setDeclination(const Declination& declination) { this->declination = declination; }
    void setRightAscension(const RightAscension& right_ascension) { this->right_ascension = right_ascension; }
    EquatorialCoordinate& operator=(const EquatorialCoordinate& other) {
        if (this != &other) {
            declination = other.getDeclination();
            right_ascension = other.getRightAscension();
        }
        return *this;
    }
};