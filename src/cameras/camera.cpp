#include "camera.h"

#include <QDebug>

namespace DFL {

Camera::Camera(Point look_from, Point look_at,
               Vector view_up_vector, double vertical_field_of_view,
               double aspect_ratio, double aperture, double focus_distance)
{
    auto theta{ DFL::degrees_to_radians(vertical_field_of_view) };
    auto h{ theta / 2 };
    auto viewport_height = 2.0 * h;
    auto viewport_width = aspect_ratio * viewport_height;

    w = DFL::normalize(look_from - look_at);
    u = DFL::normalize(DFL::cross(view_up_vector, w));
    v = DFL::cross(w, u);

    origin = look_from;
    horizontal = focus_distance * viewport_width * u;
    vertical = focus_distance * viewport_height * v;
    lower_left_corner = origin - horizontal/2 - vertical/2 - focus_distance * w;

    lens_radius = aperture / 2;
}

Ray Camera::get_ray(double u, double v) const noexcept
{
    Vector rd{ lens_radius * DFL::random_in_unit_disk<double>() };
    Vector offset{ u * rd.x + v * rd.y };

    return Ray(origin + offset, lower_left_corner + u * horizontal + v * vertical - origin - offset);
}

double hit_sphere(const Point& center, double radius, const Ray& r) noexcept
{
    Vector oc = r.origin - center;
    auto a = r.direction.length_squared();
    auto half_b = dot(oc, r.direction);
    auto c = oc.length_squared() - radius * radius;
    auto discriminant = half_b * half_b - a * c;

    if(discriminant < 0)
    {
        return -1.0;
    }else{
        return (-half_b - std::sqrt(discriminant)) / a;
    }
}

// Function that returns the color of the background (a simple gradient).
// This function linearly blends white and blue depending on the height of
// the "y" coordinate after scaling the ray direction to unit length.
Color Camera::ray_color(const Ray& ray) noexcept
{
    double sphere_radius{0.5};
    auto t = hit_sphere(Point(0.0, 0.0, -1.0), sphere_radius, ray);
    if(t > 0.0)
    {
        Point N = normalize( ray(t) - Vector(0.0, 0.0, -1.0) );
        Color sphere_color{N.x + 1, N.y + 1, N.z + 1};
        sphere_color *= 0.5;
        sphere_color *= 255.999;

        return sphere_color;
    }

    Vector3d<double> unit_direction = normalize(ray.direction);
    t = 0.5 * (unit_direction.y + 1.0);

    Color color{ lerp(t, Color(1.0, 1.0, 1.0), Color(0.5, 0.7, 1.0)) };
    color *= 255.999;

    return color;
}

} // namespace DFL
