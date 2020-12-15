#ifndef CAMERA_H
#define CAMERA_H

#include "core/geometry.h"

namespace DFL {

class Camera
{
public:
    Camera();

    Ray get_ray(double u, double v) const noexcept;
    Color ray_color(const Ray& ray) noexcept;

private:
    Point3d<double> origin;
    Point3d<double> lower_left_corner;
    Vector3d<double> horizontal;
    Vector3d<double> vertical;
};

} // namespace DFL

#endif // CAMERA_H
