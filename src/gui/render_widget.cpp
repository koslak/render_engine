#include "render_widget.h"

#include <QDebug>
#include <QImage>
#include <QPainter>
#include <QPaintEvent>

Render_widget::Render_widget(QWidget *parent) : QWidget(parent), image{std::make_unique<QImage>(QSize(1, 1), QImage::Format_ARGB32)}, width_image{1}, height_image{1}
{
}

void Render_widget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.fillRect(0, 0, width(), height(), QColor(Qt::darkGray));

    if (!image)
    {
        return;
    }

    QRect exposedRect = painter.matrix().inverted().mapRect(event->rect()).adjusted(-1, -1, 1, 1);
    painter.drawImage(exposedRect, *image, exposedRect);
}

void Render_widget::resizeEvent(QResizeEvent *event)
{
    width_image = event->size().width();
    height_image = event->size().height();

    image.reset(new QImage(QSize(width_image, height_image), QImage::Format_ARGB32));
}

void Render_widget::refresh() noexcept
{
    if (!image)
    {
        return;
    }

    QRgb *pixels = reinterpret_cast<QRgb *>(image->bits());

    uint32_t idx{ 0 };
    const uint32_t total_pixels_image{ width_image * height_image };

    for (uint32_t y = 0; y < height_image; ++y)
    {
        for (uint32_t x = 0; x < width_image; ++x, ++idx)
        {
            pixels[ idx ] = qRgb(80, 0, 0);

            emit renderer_progress( idx * 100 / total_pixels_image );
        }
    }

    update();
}