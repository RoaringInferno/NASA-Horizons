#pragma once

#include <cmath> // M_PI
#include "equatorial.hpp"
#include "horizontal.hpp"
#include "surface.hpp"

HorizontalCoordinate equatorialToHorizontal(EquatorialCoordinate& equatorial, Latitude latitude)
{
    return HorizontalCoordinate(
        Altitude(
            asin(sin(latitude.asRadians())*sin(equatorial.getDeclination().asRadians()) + cos(latitude.asRadians())*cos(equatorial.getDeclination().asRadians())*cos(equatorial.getRightAscension().asRadians()))
        ),
        Azimuth(
            -1*atan2(sin(equatorial.getRightAscension().asRadians()), cos(equatorial.getRightAscension().asRadians())*sin(equatorial.getDeclination().asRadians()) - tan(latitude.asRadians())*cos(equatorial.getDeclination().asRadians())) + M_PI
        )
    );
}