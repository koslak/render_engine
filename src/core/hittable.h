#ifndef HITTABLE_H
#define HITTABLE_H

#include "core/geometry.h"

class Ray;

struct Hit_record
{
    DFL::Point3d<double> point;
    DFL::Vector3d<double> normal;
    double t;
};

class Hittable
{
public:
    virtual bool hit(const DFL::Ray& r, double t_min, double t_max, Hit_record& hit_record) const = 0;
};

#endif // HITTABLE_H
