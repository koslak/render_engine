#include "render_thread.h"

#include "core/geometry.h"
#include "cameras/camera.h"
#include "core/hittable.h"
#include "core/scene.h"
#include "core/material.h"

#include <QImage>

using namespace DFL;

Render_thread::Render_thread(QObject *parent) : QThread(parent)
{
    samples_per_pixel = 50;
    max_depth = 50;
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

void Render_thread::render(uint32_t image_width, uint32_t image_height, Scene *scene_var, Camera *camera_var)
{
    QMutexLocker mutex_locker(&mutex);

    scene = scene_var;
    camera = camera_var;

    this->image_width = image_width;
    this->image_height = image_height;

    set_scene();

    if(!isRunning())
    {
        start(LowPriority);
    }else{
        is_restart = true;
        wait_condition.wakeOne();
    }
}

Color Render_thread::ray_color(const Ray &ray, Hittable *world, int depth) noexcept
{
    Hit_record hit_record;

    // If we've exceeded the ray bounce limit, no more light is gathered.
    if(depth <= 0)
    {
        return Color{0.0, 0.0, 0.0};
    }

    if(world->hit(ray, 0.001, DFL::Infinity, hit_record))
    {
        Ray scattered_ray;
        Color attenuation_color;

        if(hit_record.material_ptr->scatter(ray, hit_record, attenuation_color, scattered_ray))
        {
            return (attenuation_color * ray_color(scattered_ray, world, depth - 1));
        }

        return Color{0.0, 0.0, 0.0};
    }

    Vector unit_direction{ normalize(ray.direction) };
    auto t = 0.5 * (unit_direction.y + 1.0);
    Color color{ (1.0 - t) * Color(1.0, 1.0, 1.0) + t * Color(0.5, 0.7, 1.0) };

    return color;
}

QRgb Render_thread::gamma_correction(const Color pixel_color, int samples_per_pixel) const noexcept
{
    auto r{ pixel_color.x };
    auto g{ pixel_color.y };
    auto b{ pixel_color.z };

    // Replace NaN components with zero. See explanation in Ray Tracing: The Rest of Your Life.
    if (r != r) r = 0.0;
    if (g != g) g = 0.0;
    if (b != b) b = 0.0;

    // Divide the color by the number of samples and gamma-correct for gamma = 2.0.
    auto scale{ 1.0 / samples_per_pixel };

    r = std::sqrt(scale * r);
    g = std::sqrt(scale * g);
    b = std::sqrt(scale * b);

    return qRgb(static_cast<int>(256 * DFL::clamp(r, 0.0, 0.999)),
                static_cast<int>(256 * DFL::clamp(g, 0.0, 0.999)),
                static_cast<int>(256 * DFL::clamp(b, 0.0, 0.999)));
}

void Render_thread::set_scene() noexcept
{
    world = scene->create_world(Scene::Type::Advanced);

    Point look_from{ 3.0, 3.0, 2.0 };
    Point look_at{ 0.0, 0.0, -1.0 };
    double focus_distance = 80;

    camera->set_camera_direction(look_from, look_at, focus_distance);

    samples_per_pixel = 1;
    max_depth = 5;
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
            Color color{ 0.0, 0.0, 0.0 };
        mutex.unlock();

        // Create image
        QImage image(QSize(image_width, image_height), QImage::Format_ARGB32);
        QRgb *pixels = reinterpret_cast<QRgb *>(image.bits());

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
                Color pixel_color{0.0, 0.0, 0.0};

                for(int samples = 0; samples < samples_per_pixel; ++samples)
                {
                    auto u = (i + DFL::random_double()) / (image_width - 1);
                    auto v = (j + DFL::random_double()) / (image_height - 1);

                    Ray ray{ camera->get_ray(u, v) };
                    pixel_color += ray_color(ray, world.get(), max_depth);
                }

                pixels[ idx ] = gamma_correction(pixel_color, samples_per_pixel);
            }

            if(!is_restart)
            {
                auto progress{ (total_pixels_image != 0) ? (idx * 100 / total_pixels_image) : 1 };
                emit rendered_image_progress(image, progress);
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




