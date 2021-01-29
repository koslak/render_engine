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

bool Sphere::hit(const DFL::Ray& r, double t_min, double t_max, Hit_record& hit_record) const
{
    return false;
}


#endif // SPHERE_H
