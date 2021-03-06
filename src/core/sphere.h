#ifndef SPHERE_H
#define SPHERE_H

#include "core/hittable.h"

using namespace DFL;

class Sphere : public Hittable
{
public:
    Sphere() = default;
    Sphere(Point center, double radius, std::shared_ptr<Material> material) : center(center), radius(radius), material_ptr(material){}

    virtual bool hit(const Ray& r, double t_min, double t_max, Hit_record& hit_record) const override;

private:
    Point center;
    double radius;
    std::shared_ptr<Material> material_ptr;
};

bool Sphere::hit(const Ray& ray, double t_min, double t_max, Hit_record& hit_record) const
{
    Vector oc{ ray.origin - center };
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
    Vector outward_vector{ (hit_record.point - center) / radius };
    hit_record.set_face_normal(ray, outward_vector);
    hit_record.material_ptr = material_ptr;

    return true;
}


#endif // SPHERE_H
