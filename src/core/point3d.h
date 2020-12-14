#ifndef Point3dD_H
#define Point3dD_H

#include "core/dfl.h"

#include "core/vector3d.h"

namespace DFL {

template <typename T>
class Point3d
{

public:
    Point3d() { x = y = z = 0; }
    Point3d(T x, T y, T z) : x(x), y(y), z(z) { assert(!HasNaNs()); }

    template <typename U>
    explicit Point3d(const Point3d<U> &p) : x((T)p.x), y((T)p.y), z((T)p.z)
    {
        assert(!HasNaNs());
    }

    template <typename U>
    explicit operator Vector3d<U>() const
    {
        return Vector3d<U>(x, y, z);
    }

    Point3d<T> operator+(const Vector3d<T> &v) const
    {
        assert(!v.HasNaNs());

        return Point3d<T>(x + v.x, y + v.y, z + v.z);
    }

    Point3d<T> &operator+=(const Vector3d<T> &v)
    {
        assert(!v.HasNaNs());

        x += v.x;
        y += v.y;
        z += v.z;

        return *this;
    }

    Vector3d<T> operator-(const Point3d<T> &p) const
    {
        assert(!p.HasNaNs());

        return Vector3d<T>(x - p.x, y - p.y, z - p.z);
    }

    Point3d<T> operator-(const Vector3d<T> &v) const
    {
        assert(!v.HasNaNs());

        return Point3d<T>(x - v.x, y - v.y, z - v.z);
    }

    Point3d<T> &operator-=(const Vector3d<T> &v)
    {
        assert(!v.HasNaNs());

        x -= v.x;
        y -= v.y;
        z -= v.z;

        return *this;
    }

    Point3d<T> &operator+=(const Point3d<T> &p)
    {
        assert(!p.HasNaNs());

        x += p.x;
        y += p.y;
        z += p.z;

        return *this;
    }

    Point3d<T> operator+(const Point3d<T> &p) const
    {
        assert(!p.HasNaNs());

        return Point3d<T>(x + p.x, y + p.y, z + p.z);
    }

    template <typename U>
    Point3d<T> operator*(U f) const
    {
        return Point3d<T>(f * x, f * y, f * z);
    }

    template <typename U>
    Point3d<T> &operator*=(U f)
    {
        x *= f;
        y *= f;
        z *= f;

        return *this;
    }

    template <typename U>
    Point3d<T> operator/(U f) const
    {
        assertm(f != 0, "The parameter f is zero. Division by zero is not allowed");

        double inv = static_cast<double>(1) / f;

        return Point3d<T>(inv * x, inv * y, inv * z);
    }

    template <typename U>
    Point3d<T> &operator/=(U f)
    {
        assertm(f != 0, "The parameter f is zero. Division by zero is not allowed");

        double inv = static_cast<double>(1) / f;

        x *= inv;
        y *= inv;
        z *= inv;

        return *this;
    }

    T operator[](int i) const
    {
        assert(i >= 0 && i <= 2);

        if (i == 0) return x;
        if (i == 1) return y;

        return z;
    }

    T &operator[](int i)
    {
        assert(i >= 0 && i <= 2);

        if (i == 0) return x;
        if (i == 1) return y;

        return z;
    }

    bool operator==(const Point3d<T> &p) const
    {
        return x == p.x && y == p.y && z == p.z;
    }

    bool operator!=(const Point3d<T> &p) const
    {
        return x != p.x || y != p.y || z != p.z;
    }

    bool HasNaNs() const { return isNaN(x) || isNaN(y) || isNaN(z); }

    Point3d<T> operator-() const { return Point3d<T>(-x, -y, -z); }

    // Point3d Public Data
    T x, y, z;
};

template <typename T>
inline std::ostream &operator<<(std::ostream &os, const Point3d<T> &v)
{
    os << "[ " << v.x << ", " << v.y << ", " << v.z << " ]";

    return os;
}

template <typename T>
inline double distance(const Point3d<T> &p1, const Point3d<T> &p2)
{
    return (p1 - p2).length();
}

template <typename T>
inline double distance_squared(const Point3d<T> &p1, const Point3d<T> &p2)
{
    return (p1 - p2).length_squared();
}

template <typename T, typename U>
inline Point3d<T> operator*(U f, const Point3d<T> &p)
{
    assert(!p.HasNaNs());

    return p * f;
}

} // namespace DFL

#endif // Point3dD_H
