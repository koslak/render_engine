#ifndef RENDER_THREAD_H
#define RENDER_THREAD_H

#include <QMutex>
#include <QThread>
#include <QWaitCondition>
#include <QColor>
#include <QElapsedTimer>

#include "core/geometry.h"

class QImage;
class Ray;
class Hittable;
class Hittable_list;

class Render_thread : public QThread
{
    Q_OBJECT
public:
    Render_thread(QObject *parent = nullptr);
    ~Render_thread();

    void render(uint32_t image_width, uint32_t image_height);

signals:
    void rendered_image_progress(const QImage &image, int progress);
    void finished_rendering_image();

protected:
    void run() override;

private:
    QMutex mutex;
    QWaitCondition wait_condition;

    bool is_restart{ false };
    bool is_abort{ false };

    uint32_t image_width{ 1 };
    uint32_t image_height{ 1 };

    Hittable_list generate_random_scene();
    DFL::Color ray_color(const DFL::Ray &ray, Hittable *world, int depth) noexcept;
    QRgb gamma_correction(const DFL::Color pixel_color, int samples_per_pixel) const noexcept;

    double aspect_ratio;
    int samples_per_pixel;
    int max_depth;
    std::unique_ptr<Hittable_list> world;

    DFL::Point3d<double> look_from;
    DFL::Point3d<double> look_at;
    DFL::Vector3d<double> vup;
    double distance_to_focus;
    double aperture;
    double vertical_field_of_view;
};

#endif // RENDER_THREAD_H
