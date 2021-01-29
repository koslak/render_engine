#ifndef SPHERE_H
#define SPHERE_H

#include "core/hittable.h"

class Sphere : public Hittable
{
public:
    Sphere() = default;
    Sphere(DFL::Point3d<double> center, double radius) : center(center), radius(radius) {}

    virtual bool hit(const DFL::Ray& r, double t_min, double t_max, Hit_record& hit_record) const override;

private:
    DFL::Point3d<double> center;
    double radius;
};

bool Sphere::hit(const DFL::Ray& ray, double t_min, double t_max, Hit_record& hit_record) const
{
    DFL::Vector3d<double> oc{ ray.origin - center };
    auto a = ray.direction.length_squared();
    auto half_b = dot(oc, ray.direction);
    auto c = oc.length_squared() - radius * radius;
    auto discriminant = half_b * half_b - a * c;

    if(discriminant < 0)
    {
        return false;
    }

    auto sqrtd{ std::sqrt(discriminant) };

    // Find the nearest root that lies in the acceptable range.
    auto root = (-half_b - sqrtd) / a;
    if (root < t_min || t_max < root)
    {
        root = (-half_b + sqrtd) / a;

        if (root < t_min || t_max < root)
        {
            return false;
        }
    }

    hit_record.t = root;
    hit_record.point = ray(hit_record.t);
    hit_record.normal = (hit_record.point - center) / radius;

    return true;
}


#endif // SPHERE_H
