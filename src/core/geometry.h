#ifndef GEOMETRY_H
#define GEOMETRY_H

// To avoid circular dependencies, we include the last header that has all the
// different geometry classes. Clients should only include the "geometry.h" header
// in their code.

#include "core/ray.h"

namespace DFL {

using Color = Point3d<double>;

// Function that returns the color of the background (a simple gradient).
// this function linearly blends white and blue depending on the height of
// the "y" coordinate after scaling the ray direction to unit length.
Color ray_color(const Ray& ray)
{
    Vector3d<double> unit_direction = normalize(ray.direction);
    auto t = 0.5 * (unit_direction.y + 1.0);

    return lerp(t, Color(1.0, 1.0, 1.0), Color(0.5, 0.7, 1.0));
}

}

#endif // GEOMETRY_H
