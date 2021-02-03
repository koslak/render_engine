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
        double cos_theta{ std::fmin(DFL::dot(-unit_direction, hit_record.normal), 1.0) };
        double sin_theta{ std::sqrt(1.0 - cos_theta * cos_theta) };

        bool cannot_refract{ refraction_ratio * sin_theta > 1.0 };
        DFL::Vector3d<double> direction_vector;

        if(cannot_refract || reflectance(cos_theta, refraction_ratio) > DFL::random_double())
        {
            direction_vector = DFL::reflect(unit_direction, hit_record.normal);
        }else{
            direction_vector = DFL::refract(unit_direction, hit_record.normal, refraction_ratio);
        }

        scattered_ray = DFL::Ray{ hit_record.point, direction_vector };

        return true;
    }

    double index_of_refraction;

private:
    static double reflectance(double cosine, double ref_idx)
    {
        // Use Schlick's approximation for reflectance.
        auto r0 = (1 - ref_idx) / (1 + ref_idx);
        r0 = r0 * r0;

        return r0 + (1 - r0) * std::pow((1 - cosine), 5);
    }
};



#endif // MATERIAL_H
