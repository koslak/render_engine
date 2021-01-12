#include "camera.h"

#include <QDebug>

namespace DFL {

Camera::Camera()
{
    const auto aspect_ratio = 16.0 / 9.0;
    auto viewport_height = 2.0;
    auto viewport_width = aspect_ratio * viewport_height;
    auto focal_length = 1.0;

    origin = DFL::Point3d<double>{ 0, 0, 0 };
    horizontal = DFL::Vector3d<double>{ viewport_width, 0, 0 };
    vertical = DFL::Vector3d<double>(0, viewport_height, 0);
    lower_left_corner = origin - horizontal/2 - vertical/2 - DFL::Vector3d<double>(0, 0, focal_length);
}

Ray Camera::get_ray(double u, double v) const noexcept
{
//    DFL::Ray ray(origin, lower_left_corner + u * horizontal + v * vertical - origin);
//    qDebug() << "ray: o: [" << ray.origin.x << ", " << ray.origin.y << ", " << ray.origin.z << "] direction: [" << ray.direction.x << ", " << ray.direction.y << ", " << ray.direction.z << "]";
    return DFL::Ray(origin, lower_left_corner + u * horizontal + v * vertical - origin);
}

bool hit_sphere(const DFL::Point3d<double>& center, double radius, const Ray& r) noexcept
{
    DFL::Vector3d<double> oc = r.origin - center;
    auto a = dot(r.direction, r.direction);
    auto b = 2.0 * dot(oc, r.direction);
    auto c = dot(oc, oc) - radius * radius;
    auto discriminant = b * b - 4 * a * c;

    return (discriminant > 0);
}

// Function that returns the color of the background (a simple gradient).
// This function linearly blends white and blue depending on the height of
// the "y" coordinate after scaling the ray direction to unit length.
Color Camera::ray_color(const Ray& ray) noexcept
{
    if(hit_sphere(DFL::Point3d<double>(0.0, 0.0, -1.0), 0.3, ray))
    {
        Color sphere_color{1.0, 0.0, 0.0};
        sphere_color *= 255.999;

        return sphere_color;
    }

    Vector3d<double> unit_direction = normalize(ray.direction);
    auto t = 0.5 * (unit_direction.y + 1.0);

    Color color{ lerp(t, Color(1.0, 1.0, 1.0), Color(0.5, 0.7, 1.0)) };
    color *= 255.999;

    return color;
}

} // namespace DFL