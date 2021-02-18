#include "render_widget.h"

#include <QDebug>
#include <QImage>
#include <QPainter>
#include <QPaintEvent>
#include <QPixmap>
#include <QPoint>

#include <memory>

#include "core/api.h"
#include "core/dfl.h"
#include "core/geometry.h"
#include "core/render_thread.h"
#include "core/scene.h"
#include "cameras/camera.h"

using namespace DFL;

Render_widget::~Render_widget() = default;

Render_widget::Render_widget(QWidget *parent) : QWidget(parent)
{
    scene = render_options->make_scene();
    camera = render_options->make_camera();

    render_thread = std::make_unique<Render_thread>();
    render_options = std::make_unique<DFL::Render_options>();

    connect(render_thread.get(), &Render_thread::rendered_image_progress, this, &Render_widget::update_image);
    connect(render_thread.get(), &Render_thread::finished_rendering_image, this, &Render_widget::finished_rendering_image);
}

void Render_widget::paintEvent(QPaintEvent */*event*/)
{
    QPainter painter(this);
    painter.fillRect(0, 0, width(), height(), QColor(Qt::darkGray));

    if(pixmap.isNull())
    {
        painter.setPen(Qt::white);
        painter.drawText(rect(), Qt::AlignCenter, tr("Rendering image, please wait..."));
    }

    QRectF exposed = painter.transform().inverted().mapRect(rect()).adjusted(-1, -1, 1, 1);
    painter.drawPixmap(exposed, pixmap, exposed);
}

void Render_widget::resizeEvent(QResizeEvent *event)
{
    this->image_width = event->size().width();
    this->image_height = event->size().height();

    QImage resized_image{ QImage(QSize(image_width, image_height), QImage::Format_ARGB32) };
    pixmap = QPixmap::fromImage(resized_image);
}

void Render_widget::mouseMoveEvent(QMouseEvent *mouse_event)
{
//    pan_X += (mouse_event->pos().x() - last_mouse_position.x()); ///(zoom_delta * 100);
//    pan_Y += (mouse_event->pos().y() - last_mouse_position.y()); ///(zoom_delta * 100);
    last_mouse_position = mouse_event->pos();

}

void Render_widget::mousePressEvent(QMouseEvent *eventPress)
{
    initial_mouse_position = eventPress->pos();
    qDebug() << "Initial mouse position" << initial_mouse_position;
}

void Render_widget::mouseReleaseEvent(QMouseEvent *eventPress)
{
    last_mouse_position = eventPress->pos();
    qDebug() << "Last mouse position" << last_mouse_position;
    pan_X = (last_mouse_position.x() - initial_mouse_position.x()); ///(zoom_delta * 100);
    pan_Y = (last_mouse_position.y() - initial_mouse_position.y()); ///(zoom_delta * 100);

    /*
    double alpha{ std::atan(pan_X/(2 * 40.0)) * 180/M_PI };
    qDebug() << "pan_X" << pan_X;
    qDebug() << "pan_Y" << pan_Y;
    qDebug() << "alpha" << alpha;
    qDebug() << "PI" << M_PI;
    */
    emit start_render_by_mouse_action();

}

void Render_widget::wheelEvent(QWheelEvent *wheelEvent)
{
    QPoint angle_delta{ wheelEvent->angleDelta() };

    if (angle_delta.y() < 0)
    {
        zoom_out();
    } else{
        zoom_in();
    }

    emit start_render_by_mouse_action();
}

void Render_widget::zoom_in() noexcept
{
//    zoom_delta = DFL::clamp(zoom_delta * 1.25, 0.05, 20.0);
    zoom_delta += 0.5;
}

void Render_widget::zoom_out() noexcept
{
    zoom_delta -= 0.5;
    zoom_delta = DFL::clamp(zoom_delta, 0.0, 20.0);
}

void Render_widget::update_image(const QImage &image, int progress) noexcept
{
    pixmap = QPixmap::fromImage(image);

    emit update_gui(progress);

    update();
}

void Render_widget::start_render_image() noexcept
{
    render_thread->render(image_width, image_height, scene.get(), camera.get(), pan_X, pan_Y, zoom_delta);
}

void Render_widget::finished_rendering_image() noexcept
{
    emit render_image_finished();
}
