#ifndef CAMERA_H
#define CAMERA_H

#include "core/geometry.h"

namespace DFL {

class Camera
{
public:
    Camera(Point look_from, Point look_at, double vertical_field_of_view,
           Vector view_up_vector, double aspect_ratio, double aperture, double focus_distance);

    void initialize_camera(Point look_from, Point look_at,
                           double vertical_field_of_view = 20.0,
                           Vector view_up_vector = Vector{ 0.0, 1.0, 0.0 },
                           double aspect_ratio = (16.0 / 9.0),
                           double aperture = 0.1,
                           double focus_distance = 10.0) noexcept;

    Ray get_ray(double u, double v) const noexcept;
    Color ray_color(const Ray& ray) noexcept;
    void set_camera_direction(const Point &look_from, const Point &look_at, double vertical_field_of_view) noexcept;

private:
    Point origin;
    Point lower_left_corner;
    Vector horizontal;
    Vector vertical;
    Vector u, v, w;
    double lens_radius;
};

Camera *create_camera() noexcept;

} // namespace DFL

#endif // CAMERA_H
