#include "scene.h"

#include "core/geometry.h"
#include "core/sphere.h"
#include "core/material.h"
#include "core/hittable_list.h"

namespace DFL {

std::unique_ptr<Hittable> Scene::create_world(Scene::Type scene_type) noexcept
{
    std::unique_ptr<Hittable> world{ nullptr };

    switch(scene_type)
    {
        case Scene::Type::Basic:
            world = create_basic_world();
        break;

        case Scene::Type::Advanced:
            world = create_advanced_world();
        break;

        case Scene::Type::Complex:
            world = create_complex_world();
        break;
    }

    return world;
}

std::unique_ptr<Hittable> Scene::create_basic_world() noexcept
{
    Hittable_list *world = new Hittable_list;

    auto material_ground = std::make_shared<Lambertian>(Color(0.7, 0.7, 0.7));
    auto material_center = std::make_shared<Lambertian>(Color(0.1, 0.2, 0.5));
    auto material_malva  = std::make_shared<Lambertian>(Color(0.2, 0.0, 0.5));
    auto material_dark   = std::make_shared<Lambertian>(Color(0.3, 0.0, 0.0));
    world->add(make_shared<Sphere>(Point( 0.0, -100.5, -1.0), 100.0, material_ground));
    world->add(make_shared<Sphere>(Point( 0.0,    0.0, -1.0),   0.5, material_center));
    world->add(make_shared<Sphere>(Point( 0.0,    0.2,  0.2),   0.2, material_malva));
    world->add(make_shared<Sphere>(Point( 1.3,   -0.2, -0.4),   0.2, material_dark));

    return std::unique_ptr<Hittable>(world);
}

std::unique_ptr<Hittable> Scene::create_advanced_world() noexcept
{
    Hittable_list *world = new Hittable_list;

    auto material_ground = std::make_shared<Lambertian>(Color(0.7, 0.7, 0.7));
    auto material_cristal = std::make_shared<Dielectric>(1.5);
    auto material_metal   = std::make_shared<Metal>(Color(0.7, 0.6, 0.5), 0.0);
    auto material_copper  = std::make_shared<Metal>(Color(0.8, 0.6, 0.2), 0.3);
    auto material_difuse  = std::make_shared<Lambertian>(Color(0.1, 0.2, 0.5));

    world->add(std::make_shared<Sphere>(Point( 0.0, -100.5, -1.0), 100.0, material_ground));
    world->add(std::make_shared<Sphere>(Point( 0.3,    0.0, -0.5),  0.45, material_cristal));
    world->add(std::make_shared<Sphere>(Point(-1.0,    0.2, -1.1),   0.7, material_metal));
    world->add(std::make_shared<Sphere>(Point( 1.2,    0.0, -1.6),   0.5, material_copper));
    world->add(std::make_shared<Sphere>(Point(-0.5,   -0.3, -0.5),   0.2, material_difuse));

    return std::unique_ptr<Hittable>(world);
}

std::unique_ptr<Hittable> Scene::create_complex_world() noexcept
{
    Hittable_list *world = new Hittable_list;

    auto ground_material = std::make_shared<Lambertian>(Color(0.5, 0.5, 0.5));
    world->add(std::make_shared<Sphere>(DFL::Point(0, -1000, 0), 1000, ground_material));

    for (int a = -11; a < 11; a++)
    {
        for (int b = -11; b < 11; b++)
        {
            auto choose_mat = DFL::random_double();
            Point center{ a + 0.9 * DFL::random_double(), 0.2, b + 0.9 * DFL::random_double() };

            if ((center - Point(4, 0.2, 0)).length() > 0.9)
            {
                std::shared_ptr<Material> sphere_material;

                if(choose_mat < 0.8)
                {
                    // diffuse
                    Color albedo = Color::random() * Color::random();
                    sphere_material = std::make_shared<Lambertian>(albedo);
                    world->add(std::make_shared<Sphere>(center, 0.2, sphere_material));
                } else if (choose_mat < 0.95) {
                    // metal
                    Color albedo = Color::random(0.5, 1);
                    auto fuzz = DFL::random_double(0, 0.5);
                    sphere_material = std::make_shared<Metal>(albedo, fuzz);
                    world->add(std::make_shared<Sphere>(center, 0.2, sphere_material));
                } else {
                    // glass
                    sphere_material = std::make_shared<Dielectric>(1.5);
                    world->add(std::make_shared<Sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }

    auto material1 = std::make_shared<Dielectric>(1.5);
    world->add(std::make_shared<Sphere>(Point(0, 1, 0), 1.0, material1));

    auto material2 = std::make_shared<Lambertian>(Color(0.4, 0.2, 0.1));
    world->add(std::make_shared<Sphere>(Point(-4, 1, 0), 1.0, material2));

    auto material3 = std::make_shared<Metal>(Color(0.7, 0.6, 0.5), 0.0);
    world->add(std::make_shared<Sphere>(Point(4, 1, 0), 1.0, material3));

    return std::unique_ptr<Hittable>(world);
}

} // namespace DFL
