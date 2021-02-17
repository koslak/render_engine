#ifndef RENDER_WIDGET_H
#define RENDER_WIDGET_H

#include <QWidget>

class Render_thread;

namespace DFL {
    class Render_options;
    class Scene;
    class Camera;
}

class Render_widget : public QWidget
{
    Q_OBJECT
public:
    ~Render_widget();

    explicit Render_widget(QWidget *parent = nullptr);
    void start_render_image() noexcept;

protected:
    virtual void paintEvent(QPaintEvent *event) override;
    virtual void resizeEvent(QResizeEvent *event) override;
    virtual void mouseMoveEvent(QMouseEvent *mouse_event) override;
    virtual void mousePressEvent(QMouseEvent *eventPress) override;
    virtual void mouseReleaseEvent(QMouseEvent *eventPress) override;
    virtual void wheelEvent(QWheelEvent *wheelEvent) override;

signals:
    void update_gui(int progress);
    void render_image_finished();
    void start_render_by_mouse_action();

private slots:
    void update_image(const QImage &image, int progress) noexcept;
    void finished_rendering_image() noexcept;
    void zoom_in() noexcept;
    void zoom_out() noexcept;

private:
    std::unique_ptr<Render_thread> render_thread;
    std::unique_ptr<DFL::Render_options> render_options;
    std::unique_ptr<DFL::Scene> scene;
    std::unique_ptr<DFL::Camera> camera;

    QPixmap pixmap;
    uint32_t image_width{ 1 };
    uint32_t image_height{ 1 };

    double zoom_delta{ 0.0 };
    QPoint initial_mouse_position;
    QPoint last_mouse_position;
    double pan_X, pan_Y;
};

#endif // RENDER_WIDGET_H
