#ifndef DFL_H
#define DFL_H

#include <cmath>
#include <limits>
#include <memory>
#include <cassert>
#include <iostream>
#include <random>

// Use (void) to silent unused warnings.
#define assertm(exp, msg) assert(((void)msg, exp))

namespace DFL {

constexpr const double Infinity{ std::numeric_limits<double>::infinity() };
constexpr const double PI{ 3.1415926535897932385 };

inline double degrees_to_radians(double degrees)
{
    return degrees * PI / 180.0;
}

inline double random_double()
{
    static std::uniform_real_distribution<double> distribution(0.0, 1.0);
    static std::mt19937 generator;

    return distribution(generator);
}

inline double random_double(double min, double max)
{
    // returns a random real in [min, max)
    return min + (max - min) * random_double();
}

inline double clamp(double x, double min, double max)
{
    if(x < min) return min;
    if(x > max) return max;

    return x;
}

}  // namespace DFL

#endif // DFL_H
