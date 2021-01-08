#include "render_widget.h"

#include <QDebug>
#include <QImage>
#include <QPainter>
#include <QPaintEvent>
#include <QPixmap>

#include <memory>

#include "core/geometry.h"

Render_widget::Render_widget(QWidget *parent) : QWidget(parent), image{std::make_unique<QImage>(QSize(1, 1), QImage::Format_ARGB32)}
{
    connect(&render_thread, &Render_thread::rendered_image, this, &Render_widget::update_image);
}

void Render_widget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
//    painter.fillRect(0, 0, width(), height(), QColor(Qt::darkGray));

    if (!image)
    {
        return;
    }

    QRect exposedRect = painter.matrix().inverted().mapRect(event->rect()).adjusted(-1, -1, 1, 1);
    painter.drawImage(exposedRect, *image, exposedRect);
}

void Render_widget::resizeEvent(QResizeEvent *event)
{
    this->image_width = event->size().width();
    this->image_height = event->size().height();

    image.reset(new QImage(QSize(image_width, image_height), QImage::Format_ARGB32));
}

void Render_widget::update_image(const QImage &image, int progress)
{

}

void Render_widget::start_render_image() noexcept
{
    render_thread.render(image_width, image_height);
}
