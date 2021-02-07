#ifndef RENDER_WIDGET_H
#define RENDER_WIDGET_H

#include <QWidget>

#include "cameras/camera.h"
#include "core/geometry.h"

class Render_thread;

class Render_widget : public QWidget
{
    Q_OBJECT
public:
    explicit Render_widget(QWidget *parent = nullptr);
    void start_render_image() noexcept;

protected:
    virtual void paintEvent(QPaintEvent *event) override;
    virtual void resizeEvent(QResizeEvent *event) override;

signals:
    void update_gui(int progress);
    void render_image_finished();

private slots:
    void update_image(const QImage &image, int progress);
    void finished_rendering_image();

private:
    std::unique_ptr<Render_thread> render_thread;

    QPixmap pixmap;
    uint32_t image_width{ 1 };
    uint32_t image_height{ 1 };
};

#endif // RENDER_WIDGET_H
