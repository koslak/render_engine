#ifndef RAY_H
#define RAY_H

#include "core/dfl.h"

#include "core/point3d.h"

namespace DFL {

class Ray
{
public:
    Ray() = default;
    Ray(const Point3d<double> &origin, const Vector3d<double> &direction) : origin(origin), direction(direction) {}

    Point3d<double> operator()(double t) const { return origin + direction * t; }

    friend std::ostream &operator<<(std::ostream &os, const Ray &ray)
    {
        os << "[origin=" << ray.origin << ", direction=" << ray.direction << "]";

        return os;
    }

    Point3d<double> origin;
    Vector3d<double> direction;
};

} // namespace DFL

#endif // RAY_H
