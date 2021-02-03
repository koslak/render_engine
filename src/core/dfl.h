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
    return rand() / (RAND_MAX + 1.0);
}

inline double random_double(double min, double max)
{
    // returns a random real in [min, max)
    return min + (max - min) * random_double();
}

template<typename T>
T min(const T &a, const T &b)
{
    return a < b ? a : b;
}

template<typename T, typename... Ts>
T min(const T &a, const T &b, const Ts &... ts)
{
    return min(min(a, b), ts...);
}

template<typename T>
T max(const T &a, const T &b)
{
    return a > b ? a : b;
}

template<typename T, typename... Ts>
T max(const T &a, const T &b, const Ts &... ts)
{
    return max(max(a, b), ts...);
}

template<typename T>
T clamp(T val, T min_val, T max_val)
{
    /*
    if(val < min_val) return min_val;
    if(val > max_val) return max_val;

    return val;
    */

    return DFL::min(DFL::max(val, min_val), max_val);
}

}  // namespace DFL

#endif // DFL_H
