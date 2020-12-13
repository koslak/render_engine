#ifndef VECTOR3D_H
#define VECTOR3D_H

#include "core/dfl.h"

namespace DFL {

template<typename T>
inline bool isNaN(const T x)
{
    return std::isnan(x);
}

template <>
inline bool isNaN(const int x)
{
    return false;
}

template <typename T>
class Vector3d
{

public:
    Vector3d() { x = y = z = 0; }
    Vector3d(T x, T y, T z) : x(x), y(y), z(z) { /*DCHECK(!HasNaNs());*/ }

    bool HasNaNs() const noexcept
    {
        return isNaN(x) || isNaN(y) || isNaN(z);
    }

    Vector3d<T> operator +(const Vector3d<T> &v) const noexcept
    {
        return Vector3d<T>{ x + v.x, y + v.y, z + v.z };
    }

    Vector3d<T> &operator +=(const Vector3d<T> &v) noexcept
    {
        x += v.x;
        y += v.y;
        z += v.z;

        return *this;
    }

    Vector3d<T> operator -(const Vector3d<T> &v) const noexcept
    {
        return Vector3d<T>{ x - v.x, y - v.y, z - v.z };
    }

    Vector3d<T> &operator -=(const Vector3d<T> &v) noexcept
    {
        x -= v.x;
        y -= v.y;
        z -= v.z;

        return *this;
    }

    bool operator ==(const Vector3d<T> &v) const noexcept
    {
        return (x == v.x && y == v.y && z == v.z);
    }

    bool operator !=(const Vector3d<T> &v) const noexcept
    {
        return (x != v.x || y != v.y || z != v.z);
    }

    template <typename U>
    Vector3d<T> operator *(U s) const noexcept
    {
        return Vector3d<T>{ s * x, s * y, s * z };
    }

    template <typename U>
    Vector3d<T> &operator *=(U s) noexcept
    {
        x *= s;
        y *= s;
        z *= s;

        return *this;
    }

    template <typename U>
    Vector3d<T> operator /(U f) const noexcept
    {
        CHECK_NE(f, 0);
        double inv = static_cast<double>(1) / f;

        return Vector3d<T>{x * inv, y * inv, z * inv};
    }

    template <typename U>
    Vector3d<T> &operator/=(U f) noexcept
    {
        CHECK_NE(f, 0);
        double inv = static_cast<double>(1) / f;

        x *= inv;
        y *= inv;
        z *= inv;

        return *this;
    }

    Vector3d<T> operator -() const noexcept
    {
        return Vector3d<T>{ -x, -y, -z };
    }

    double length_squared() const noexcept
    {
        return (x * x + y * y + z * z);
    }

    double length() const noexcept
    {
        return std::sqrt(length_squared());
    }

    // Public data
    T x, y, z;
};

} // namespace DFL

#endif // VECTOR3D_H
