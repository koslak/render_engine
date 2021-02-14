#ifndef RENDER_THREAD_H
#define RENDER_THREAD_H

#include <QMutex>
#include <QThread>
#include <QWaitCondition>
#include <QColor>
#include <QElapsedTimer>

#include "core/geometry.h"

class QImage;
class Hittable;
class Hittable_list;

namespace DFL
{
    class Scene;
    class Camera;
}

class Render_thread : public QThread
{
    Q_OBJECT
public:
    Render_thread(QObject *parent = nullptr);
    ~Render_thread();

    void render(uint32_t image_width, uint32_t image_height, DFL::Scene *scene_var, DFL::Camera *camera_var, double zoom_delta);

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

    DFL::Scene *scene;
    DFL::Camera *camera;

    void set_scene(double zoom_delta) noexcept;
    DFL::Color ray_color(const DFL::Ray &ray, Hittable *world, int depth) noexcept;
    QRgb gamma_correction(const DFL::Color pixel_color, int samples_per_pixel) const noexcept;

    int samples_per_pixel;
    int max_depth;
    std::unique_ptr<Hittable> world;
};

#endif // RENDER_THREAD_H
