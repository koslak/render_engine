#ifndef HITTABLE_H
#define HITTABLE_H

#include "core/geometry.h"

class Material;

struct Hit_record
{
    DFL::Point3d<double> point;
    DFL::Vector3d<double> normal;
    std::shared_ptr<Material> material_ptr;
    double t;
    bool front_face;

    inline void set_face_normal(const DFL::Ray &ray, const DFL::Vector3d<double> outward_normal)
    {
        front_face = dot(ray.direction, outward_normal) < 0.0;
        normal = front_face ? outward_normal : -outward_normal;
    }
};

class Hittable
{
public:
    virtual bool hit(const DFL::Ray &r, double t_min, double t_max, Hit_record &hit_record) const = 0;
    virtual ~Hittable() = default;
};

#endif // HITTABLE_H
