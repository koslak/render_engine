#include "main_window.h"

#include "render_widget.h"

#include <QVBoxLayout>
#include <QProgressBar>
#include <QPushButton>
#include <QDebug>
#include <QImage>

#include <memory>

#include "core/geometry.h"

Main_window::Main_window(QWidget *parent) : QMainWindow(parent), render_widget{new Render_widget(this)},
                                                                 progress_bar{new QProgressBar(this)},
                                                                 render_button{new QPushButton("Render Button")}
{
    progress_bar->setMaximum(99);

    QWidget *central_widget = new QWidget{this};

    QVBoxLayout *vertical_layout = new QVBoxLayout;
    vertical_layout->addWidget(render_widget);
    vertical_layout->addWidget(render_button);
    vertical_layout->addWidget(progress_bar);

    central_widget->setLayout(vertical_layout);
    this->setCentralWidget(central_widget);

    QObject::connect(render_button, &QPushButton::clicked, this, &Main_window::render_button_clicked);
    QObject::connect(render_widget, &Render_widget::renderer_progress, this, &Main_window::update_render_progress);
    QObject::connect(this, &Main_window::update_progress_bar, this, &Main_window::update_render_progress);

    DFL::Color a;
    camera = std::make_unique<DFL::Camera>();
}

void Main_window::render_button_clicked()
{
    const auto aspect_ratio = 16.0 / 9.0;
    const int image_width = render_widget->image_width();
    const int image_height = render_widget->image_height(); // static_cast<int>(image_width / aspect_ratio);
    std::vector<DFL::Color> image_pixels;

    QRgb *pixels = reinterpret_cast<QRgb *>(render_widget->get_image()->bits());
    uint32_t idx{ 0 };
    const uint32_t total_pixels_image( image_width * image_height );

    for (int j = image_height - 1; j >= 0; --j)
    {
        for (int i = 0; i < image_width; ++i, ++idx)
        {
            auto u = double(i) / (image_width - 1);
            auto v = double(j) / (image_height - 1);

            DFL::Ray ray{ camera->get_ray(u, v) };
            DFL::Color pixel_color = camera->ray_color(ray);

//            image_pixels.emplace_back(pixel_color);
            pixels[ idx ] = qRgb(pixel_color.x, pixel_color.y, pixel_color.z);
            emit update_progress_bar( idx * 100 / total_pixels_image );
        }
    }

    render_widget->update();
//    qDebug() << "Image size A: " << render_widget->image_width() << render_widget->image_height();
//    qDebug() << "Image size B: " << image_width << image_height;

//    render_widget->refresh(image_pixels);
}

void Main_window::update_render_progress(const int &step)
{
    progress_bar->setValue(step);

}
