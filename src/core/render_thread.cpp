#include "render_thread.h"

#include "core/geometry.h"
#include "cameras/camera.h"
#include "core/hittable.h"
#include "core/hittable_list.h"
#include "core/sphere.h"
#include "core/material.h"
#include "core/Tonemap.h"

#include <QImage>
#include <QFile>
#include <QTextStream>

Render_thread::Render_thread(QObject *parent) : QThread(parent), look_from{ DFL::Point3d<double>{ 3.0, 2.0, 8.0 } },
                                                                 look_at{ DFL::Point3d<double>{ 0.0, 0.0, 0.0 } },
                                                                 vup{ DFL::Vector3d<double>{ 0.0, 1.0, 0.0 } }
{
    world = std::make_unique<Hittable_list>( generate_random_scene() );

    aspect_ratio = 16.0 / 9.0;
    samples_per_pixel = 50;
    max_depth = 50;

    distance_to_focus = 10.0;
    aperture = 0.1;
    vertical_field_of_view = 20.0;

    QFile file("result_image.ppm");
    file.open(QFile::WriteOnly|QFile::Truncate);
    file.close();
}

Render_thread::~Render_thread()
{
    mutex.lock();
        // We set abort to true to tell run() to stop running as soon as possible.
        is_abort = true;

        // We call QWaitCondition::wakeOne() to wake up the thread if it's sleeping.
        // Note: In the run() method, the thread is put to sleep when it has nothing to do.
        wait_condition.wakeOne();
    mutex.unlock();

    // At the end of the destructor, we call QThread::wait() to wait until run()
    // has exited before the base class destructor is invoked.

    // QThread::wait(): Blocks the thread until the thread associated with this
    //                  QThread object has finished execution (i.e. when it returns from run()).
    wait();
}

void Render_thread::render(uint32_t image_width, uint32_t image_height)
{
    QMutexLocker mutex_locker(&mutex);

    this->image_width = image_width;
    this->image_height = image_height;

    if(!isRunning())
    {
        start(LowPriority);
    }else{
        is_restart = true;
        wait_condition.wakeOne();
    }
}

QRgb Render_thread::tonemap(const DFL::Color &c) const
{
    float _pow2Exposure{ 1.0f };
    DFL::Vector3d<double> cc{ c * _pow2Exposure * 255.0f };

    double x = DFL::clamp(cc.x, 0.0, 255.0);
    double y = DFL::clamp(cc.y, 0.0, 255.0);
    double z = DFL::clamp(cc.z, 0.0, 255.0);

//    DFL::Vector3d<double> pixel(DFL::clamp(cc, DFL::Vector3d<double>(0.0f), DFL::Vector3d<double>(255.0f)));
    DFL::Vector3d<double> pixel(x, y, z);

//    DFL::Vector3d<double> result{ Tonemap::tonemap(Tonemap::Reinhard, DFL::max(pixel, DFL::Vector3d<double>(0.0))) };
    DFL::Vector3d<double> result{ Tonemap::tonemap(Tonemap::LinearOnly, pixel) };

    return qRgb(result.x, result.y, result.z);
}

Hittable_list Render_thread::generate_random_scene()
{
    Hittable_list world;

    auto ground_material = std::make_shared<Lambertian>(DFL::Color(0.5, 0.5, 0.5));
    world.add(std::make_shared<Sphere>(DFL::Point3d<double>(0,-1000,0), 1000, ground_material));

    for (int a = -11; a < 11; a++)
    {
        for (int b = -11; b < 11; b++)
        {
            auto choose_mat = DFL::random_double();
            DFL::Point3d<double> center{ a + 0.9 * DFL::random_double(), 0.2, b + 0.9 * DFL::random_double() };

            if ((center - DFL::Point3d<double>(4, 0.2, 0)).length() > 0.9)
            {
                std::shared_ptr<Material> sphere_material;

                if(choose_mat < 0.8)
                {
                    // diffuse
                    DFL::Color albedo = DFL::Color::random() * DFL::Color::random();
                    sphere_material = std::make_shared<Lambertian>(albedo);
                    world.add(std::make_shared<Sphere>(center, 0.2, sphere_material));
                } else if (choose_mat < 0.95) {
                    // metal
                    DFL::Color albedo = DFL::Color::random(0.5, 1);
                    auto fuzz = DFL::random_double(0, 0.5);
                    sphere_material = std::make_shared<Metal>(albedo, fuzz);
                    world.add(std::make_shared<Sphere>(center, 0.2, sphere_material));
                } else {
                    // glass
                    sphere_material = std::make_shared<Dielectric>(1.5);
                    world.add(std::make_shared<Sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }

    auto material1 = std::make_shared<Dielectric>(1.5);
    world.add(std::make_shared<Sphere>(DFL::Point3d<double>(0, 1, 0), 1.0, material1));

    auto material2 = std::make_shared<Lambertian>(DFL::Color(0.4, 0.2, 0.1));
    world.add(std::make_shared<Sphere>(DFL::Point3d<double>(-4, 1, 0), 1.0, material2));

    auto material3 = std::make_shared<Metal>(DFL::Color(0.7, 0.6, 0.5), 0.0);
    world.add(std::make_shared<Sphere>(DFL::Point3d<double>(4, 1, 0), 1.0, material3));

    return world;
}

DFL::Color Render_thread::ray_color(const DFL::Ray &ray, Hittable *world, int depth)
{
    Hit_record hit_record;

    // If we've exceeded the ray bounce limit, no more light is gathered.
    if(depth <= 0)
    {
        return DFL::Color{0.0, 0.0, 0.0};
    }

    if(world->hit(ray, 0.001, DFL::Infinity, hit_record))
    {
        DFL::Ray scattered_ray;
        DFL::Color attenuation_color;

        if(hit_record.material_ptr->scatter(ray, hit_record, attenuation_color, scattered_ray))
        {
            return (attenuation_color * ray_color(scattered_ray, world, depth - 1));
        }

        return DFL::Color{0.0, 0.0, 0.0};
    }

    DFL::Vector3d<double> unit_direction{ normalize(ray.direction) };
    auto t = 0.5 * (unit_direction.y + 1.0);
    DFL::Color color{ (1.0 - t) * DFL::Color(1.0, 1.0, 1.0) + t * DFL::Color(0.5, 0.7, 1.0) };

    return color;
}

// The function body is an infinite loop which starts by storing the rendering parameters
// in local variables. As usual, we protect accesses to the member variables using the
// class's mutex. Storing the member variables in local variables allows us to minimize
// the amout of code that needs to be protected by a mutex. This ensures that the main thread
// will never have to block for too long when it needs to access Render_thread's member
// variables (e.g., in render()).
void Render_thread::run()
{
    forever
    {
        mutex.lock();
            const int image_width{ static_cast<int>(this->image_width) };
            const int image_height{ static_cast<int>(this->image_height) };
            uint32_t idx{ 0 };
            const int total_pixels_image{ image_width * image_height };
            DFL::Color color{ 0.0, 0.0, 0.0 };
        mutex.unlock();

        // Create image
        QImage image(QSize(image_width, image_height), QImage::Format_ARGB32);
        QRgb *pixels = reinterpret_cast<QRgb *>(image.bits());

        auto material_ground = std::make_shared<Lambertian>(DFL::Color(0.8, 0.8, 0.0));
        auto material_center = std::make_shared<Lambertian>(DFL::Color(0.1, 0.2, 0.5));
        auto material_left   = std::make_shared<Dielectric>(1.5);
        auto material_right  = std::make_shared<Metal>(DFL::Color(0.8, 0.6, 0.2), 1.0);

        world.reset();
        world = std::make_unique<Hittable_list>();
        world->add(make_shared<Sphere>(DFL::Point3d<double>( 0.0, -100.5, -1.0), 100.0, material_ground));
        world->add(make_shared<Sphere>(DFL::Point3d<double>( 0.0,    0.0, -1.0),   0.5, material_center));
        world->add(make_shared<Sphere>(DFL::Point3d<double>(-1.0,    0.0, -1.0),   0.5, material_left));
        world->add(make_shared<Sphere>(DFL::Point3d<double>(-1.0,    0.0, -1.0),  -0.4, material_left));
        world->add(make_shared<Sphere>(DFL::Point3d<double>( 1.0,    0.0, -1.0),   0.5, material_right));

        samples_per_pixel = 10;
        max_depth = 50;

        DFL::Camera camera{ look_from, look_at, vup, vertical_field_of_view, aspect_ratio, aperture, distance_to_focus };

        QFile *file = new QFile("result_image.ppm");
        if (!file->open(QIODevice::WriteOnly | QIODevice::Append))
        {
            return;
        }

        for(int j = image_height - 1; j >= 0; --j)
        {
            if(is_restart)
            {
                break;
            }

            if(is_abort)
            {
                return;
            }

            for(int i = 0; i < image_width; ++i, ++idx)
            {
                DFL::Color pixel_color{0.0, 0.0, 0.0};

                for(int samples = 0; samples < samples_per_pixel; ++samples)
                {
                    auto u = (i + DFL::random_double()) / (image_width - 1);
                    auto v = (j + DFL::random_double()) / (image_height - 1);

                    DFL::Ray ray{ camera.get_ray(u, v) };
                    pixel_color += ray_color(ray, world.get(), max_depth);
                }

                auto r{ pixel_color.x };
                auto g{ pixel_color.y };
                auto b{ pixel_color.z };

                // Replace NaN components with zero. See explanation in Ray Tracing: The Rest of Your Life.
                if (r != r) r = 0.0;
                if (g != g) g = 0.0;
                if (b != b) b = 0.0;

                // Divide the color by the number of samples and gamma-correct for gamma = 2.0.
                auto scale{ 1.0 / samples_per_pixel };
                /*
                r = std::sqrt(scale * r);
                g = std::sqrt(scale * g);
                b = std::sqrt(scale * b);
                */
                const double gamma{ 2.2 };
                r = std::pow((scale * r), 1.0 / gamma);
                g = std::pow((scale * g), 1.0 / gamma);
                b = std::pow((scale * b), 1.0 / gamma);

                DFL::Color result_color;

                result_color.x = static_cast<int>(256 * DFL::clamp(r, 0.0, 0.999));
                result_color.y = static_cast<int>(256 * DFL::clamp(g, 0.0, 0.999));
                result_color.z = static_cast<int>(256 * DFL::clamp(b, 0.0, 0.999));

                pixels[ idx ] = qRgb(result_color.x, result_color.y, result_color.z);

//                pixels[ idx ] = tonemap(pixel_color);

//                DFL::Vector3d<double> tonemap_vector = Tonemap::tonemap(Tonemap::GammaOnly, DFL::max(pixel_color, DFL::Vector3d<double>(0.0f)));
//                pixels[ idx ] = tonemap(pixel_color);
                write_color(file, result_color, samples_per_pixel);
            }

            if(!is_restart)
            {
                auto progress{ (total_pixels_image != 0) ? (idx * 100 / total_pixels_image) : 1 };
                emit rendered_image(image, progress);
            }
        }

        emit finished_rendering_image();

        mutex.lock();
            if (!is_restart)
            {
                // The thread is put to sleep when there is nothing to do.
                wait_condition.wait(&mutex);
            }

            is_restart = false;
        mutex.unlock();
    }
}

void Render_thread::write_color(QIODevice *file, DFL::Color pixel_color, int samples_per_pixel)
{
    QTextStream out(file);
    static int first_time{ true };

    if(first_time == true)
    {
        out << "P3\n" << image_width << ' ' << image_height << "\n255\n";
        first_time = false;
    }

    /*
    auto r = pixel_color.x;
    auto g = pixel_color.y;
    auto b = pixel_color.z;

    // Replace NaN components with zero. See explanation in Ray Tracing: The Rest of Your Life.
    if (r != r) r = 0.0;
    if (g != g) g = 0.0;
    if (b != b) b = 0.0;

    // Divide the color by the number of samples and gamma-correct for gamma=2.0.
    auto scale = 1.0 / samples_per_pixel;
    r = sqrt(scale * r);
    g = sqrt(scale * g);
    b = sqrt(scale * b);

    // Write the translated [0,255] value of each color component.
    out << static_cast<int>(256 * DFL::clamp(r, 0.0, 0.999)) << ' '
        << static_cast<int>(256 * DFL::clamp(g, 0.0, 0.999)) << ' '
        << static_cast<int>(256 * DFL::clamp(b, 0.0, 0.999)) << '\n';
    */
    out << static_cast<int>(pixel_color.x) << ' '
        << static_cast<int>(pixel_color.y) << ' '
        << static_cast<int>(pixel_color.z) << '\n';

}
























