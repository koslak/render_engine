#ifndef CAMERA_H
#define CAMERA_H

#include "core/geometry.h"

namespace DFL {

class Camera
{
public:
    Camera(DFL::Point3d<double> look_from, DFL::Point3d<double> look_at,
           DFL::Vector3d<double> view_up_vector, double vertical_field_of_view,
           double aspect_ratio, double aperture, double focus_distance);

    Ray get_ray(double u, double v) const noexcept;
    Color ray_color(const Ray& ray) noexcept;

private:
    Point3d<double> origin;
    Point3d<double> lower_left_corner;
    Vector3d<double> horizontal;
    Vector3d<double> vertical;
    Vector3d<double> u, v, w;
    double lens_radius;
};

} // namespace DFL

#endif // CAMERA_H
