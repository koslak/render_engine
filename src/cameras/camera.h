#ifndef CAMERA_H
#define CAMERA_H

#include "core/geometry.h"

namespace DFL {

class Camera
{
public:
    Camera(Point look_from, Point look_at,
           Vector view_up_vector, double vertical_field_of_view,
           double aspect_ratio, double aperture, double focus_distance);

    Ray get_ray(double u, double v) const noexcept;
    Color ray_color(const Ray& ray) noexcept;

private:
    Point origin;
    Point lower_left_corner;
    Vector horizontal;
    Vector vertical;
    Vector u, v, w;
    double lens_radius;
};

Camera *create_camera(/*const ParamSet &params*/) noexcept;

} // namespace DFL

#endif // CAMERA_H
