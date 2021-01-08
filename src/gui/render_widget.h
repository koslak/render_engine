#ifndef RENDER_WIDGET_H
#define RENDER_WIDGET_H

#include <QWidget>

#include "cameras/camera.h"
#include "core/geometry.h"
#include "core/render_thread.h"

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
    void update_progress_bar(int progress);

private slots:
    void update_image(const QImage &image, int progress);

private:
    Render_thread render_thread;

    QPixmap pixmap;
    uint32_t image_width{ 1 };
    uint32_t image_height{ 1 };
};

#endif // RENDER_WIDGET_H
