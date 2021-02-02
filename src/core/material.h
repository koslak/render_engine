#ifndef MATERIAL_H
#define MATERIAL_H


#include "core/geometry.h"

struct Hit_record;

class Material
{
public:
    virtual bool scatter(const DFL::Ray &ray_in, const Hit_record &hit_record, DFL::Color &attenuation_color, DFL::Ray &scattered_ray) const = 0;
};

class Lambertian : public Material
{
public:
    Lambertian(const DFL::Color &albedo_color) : albedo_color(albedo_color) {}

    virtual bool scatter(const DFL::Ray &, const Hit_record &hit_record, DFL::Color &attenuation_color, DFL::Ray &scattered_ray) const override
    {
        DFL::Vector3d<double> scatter_direction{ hit_record.normal + DFL::random_unit_vector<double>() };

        // Catch degenerate scatter direction
        if( scatter_direction.near_zero() )
        {
            scatter_direction = hit_record.normal;
        }

        scattered_ray = DFL::Ray(hit_record.point, scatter_direction);
        attenuation_color = albedo_color;

        return true;
    }

    DFL::Color albedo_color;
};

class Metal : public Material
{
public:
    Metal(const DFL::Color &albedo_color) : albedo_color(albedo_color) {}

    virtual bool scatter(const DFL::Ray &ray_in, const Hit_record &hit_record, DFL::Color &attenuation_color, DFL::Ray &scattered_ray) const override
    {
        DFL::Vector3d<double> reflected{ DFL::reflect(DFL::normalize(ray_in.direction), hit_record.normal) };
        scattered_ray = DFL::Ray(hit_record.point, reflected);
        attenuation_color = albedo_color;

        return (DFL::dot(scattered_ray.direction, hit_record.normal) > 0);
    }

    DFL::Color albedo_color;
};
#endif // MATERIAL_H
