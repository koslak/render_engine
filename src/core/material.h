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
    Metal(const DFL::Color &albedo_color, double fuzzy) : albedo_color(albedo_color), fuzz(fuzzy < 1 ? fuzzy : 1) {}

    virtual bool scatter(const DFL::Ray &ray_in, const Hit_record &hit_record, DFL::Color &attenuation_color, DFL::Ray &scattered_ray) const override
    {
        DFL::Vector3d<double> reflected{ DFL::reflect(DFL::normalize(ray_in.direction), hit_record.normal) };
        scattered_ray = DFL::Ray(hit_record.point, reflected + fuzz * DFL::random_in_unit_sphere<double>());
        attenuation_color = albedo_color;

        return (DFL::dot(scattered_ray.direction, hit_record.normal) > 0);
    }

    DFL::Color albedo_color;
    double fuzz;
};

class Dielectric : public Material
{
public:
    Dielectric(double index_of_refraction) : index_of_refraction{index_of_refraction} {}

    virtual bool scatter(const DFL::Ray &ray_in, const Hit_record &hit_record, DFL::Color &attenuation_color, DFL::Ray &scattered_ray) const override
    {
        attenuation_color = DFL::Color{1.0, 1.0, 1.0};
        double refraction_ratio{ hit_record.front_face ? (1.0 / index_of_refraction) : index_of_refraction };

        DFL::Vector3d<double> unit_direction{ DFL::normalize(ray_in.direction) };
        DFL::Vector3d<double> refracted{ DFL::refract(unit_direction, hit_record.normal, refraction_ratio) };

        scattered_ray = DFL::Ray{ hit_record.point, refracted };

        return true;
    }

    double index_of_refraction;
};



#endif // MATERIAL_H
