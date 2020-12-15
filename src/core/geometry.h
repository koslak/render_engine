#ifndef GEOMETRY_H
#define GEOMETRY_H

// To avoid circular dependencies, we include the last header that has all the
// different geometry classes. Clients should only include the "geometry.h" header
// in their code.

#include "core/ray.h"

namespace DFL {

using Color = Point3d<double>;

}

#endif // GEOMETRY_H
