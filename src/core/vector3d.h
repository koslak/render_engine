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
        assertm(f != 0, "The parameter f is zero. Division by zero is not allowed");
        double inv = static_cast<double>(1) / f;

        return Vector3d<T>{x * inv, y * inv, z * inv};
    }

    template <typename U>
    Vector3d<T> &operator/=(U f) noexcept
    {
        assertm(f != 0, "The parameter f is zero. Division by zero is not allowed");
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

template <typename T, typename U>
inline Vector3d<T> operator*(U f, const Vector3d<T> &v)
{
    return v * f;
}

template <typename T>
inline std::ostream &operator<<(std::ostream &os, const Vector3d<T> &v)
{
    os << "[ " << v.x << ", " << v.y << ", " << v.z << " ]";

    return os;
}

template <typename T>
inline T dot(const Vector3d<T> &v1, const Vector3d<T> &v2)
{
    assert(!v1.HasNaNs() && !v2.HasNaNs());

    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

template <typename T>
inline T absolute_dot(const Vector3d<T> &v1, const Vector3d<T> &v2)
{
    assert(!v1.HasNaNs() && !v2.HasNaNs());

    return std::abs(Dot(v1, v2));
}

template <typename T>
inline Vector3d<T> cross(const Vector3d<T> &v1, const Vector3d<T> &v2)
{
    assert(!v1.HasNaNs() && !v2.HasNaNs());
    double v1x = v1.x, v1y = v1.y, v1z = v1.z;
    double v2x = v2.x, v2y = v2.y, v2z = v2.z;

    return Vector3d<T>((v1y * v2z) - (v1z * v2y), (v1z * v2x) - (v1x * v2z), (v1x * v2y) - (v1y * v2x));
}

template <typename T>
inline Vector3d<T> normalize(const Vector3d<T> &v)
{
    return v / v.Length();
}

} // namespace DFL

#endif // VECTOR3D_H