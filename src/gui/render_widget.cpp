#include "render_widget.h"

#include <QDebug>
#include <QImage>
#include <QPainter>
#include <QPaintEvent>
#include <QPixmap>

#include <memory>

#include "core/geometry.h"
#include "core/render_thread.h"

Render_widget::Render_widget(QWidget *parent) : QWidget(parent)
{
    render_thread = std::make_unique<Render_thread>();

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

void Render_widget::update_image(const QImage &image, int progress)
{
    pixmap = QPixmap::fromImage(image);

    emit update_gui(progress);

    update();
}

void Render_widget::start_render_image() noexcept
{
    render_thread->render(image_width, image_height);
}

void Render_widget::finished_rendering_image()
{
    emit render_image_finished();
}
