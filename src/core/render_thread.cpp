#include "render_thread.h"

#include "core/geometry.h"
#include "cameras/camera.h"
#include "core/hittable.h"
#include "core/hittable_list.h"
#include "core/sphere.h"

#include <QImage>

Render_thread::Render_thread(QObject *parent) : QThread(parent)
{

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

DFL::Color Render_thread::ray_color(const DFL::Ray &ray, const Hittable &world, int depth)
{
    Hit_record hit_record;

    // If we've exceeded the ray bounce limit, no more light is gathered.
    if(depth <= 0)
    {
        return DFL::Color{0.0, 0.0, 0.0};
    }

    if(world.hit(ray, 0.001, DFL::Infinity, hit_record))
    {
        DFL::Point3d<double> target{ hit_record.point + hit_record.normal + DFL::random_in_unit_sphere<double>() };
//        DFL::Vector3d<double> temp_vector{ hit_record.normal + DFL::Vector3d<double>(1.0, 1.0, 1.0) };
//        DFL::Color color { 0.5 * temp_vector.x, 0.5 * temp_vector.y, 0.5 * temp_vector.z };
        DFL::Color color { 0.5 * ray_color(DFL::Ray(hit_record.point, target - hit_record.point), world, depth - 1) };

        return color;
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
        DFL::Camera camera;
        const int samples_per_pixel{ 100 };
        const int max_depth{ 50 };
        QRgb *pixels = reinterpret_cast<QRgb *>(image.bits());

        Hittable_list world;
        world.add(std::make_shared<Sphere>(DFL::Point3d<double>(0.0, 0.0, -1.0), 0.5));
        world.add(std::make_shared<Sphere>(DFL::Point3d<double>(0.0, -100.5, -1.0), 100.0));

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
                    pixel_color += ray_color(ray, world, max_depth);
                }

                auto r{ pixel_color.x };
                auto g{ pixel_color.y };
                auto b{ pixel_color.z };

                // Divide the color by the number of samples and gamma-correct for gamma = 2.0.
                auto scale{ 1.0 / samples_per_pixel };
                r *= std::sqrt(scale * r);
                g *= std::sqrt(scale * g);
                b *= std::sqrt(scale * b);

                pixels[ idx ] = qRgb(255.999 * DFL::clamp(r, 0.0, 0.999),
                                     255.999 * DFL::clamp(g, 0.0, 0.999),
                                     255.999 * DFL::clamp(b, 0.0, 0.999));
            }

            if(!is_restart)
            {
                auto progress{ (total_pixels_image != 0) ? (idx * 100 / total_pixels_image) : 1 };
                emit rendered_image(image, progress);
            }
        }

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
