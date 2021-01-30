#ifndef DFL_H
#define DFL_H

#include <cmath>
#include <limits>
#include <memory>
#include <cassert>
#include <iostream>

// Use (void) to silent unused warnings.
#define assertm(exp, msg) assert(((void)msg, exp))

constexpr const double infinity{ std::numeric_limits<double>::infinity() };
constexpr const double PI{ 3.1415926535897932385 };

inline double degrees_to_radians(double degrees)
{
    return degrees * PI / 180.0;
}

#endif // DFL_H
