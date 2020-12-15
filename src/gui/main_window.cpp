#include "main_window.h"

#include "render_widget.h"

#include <QVBoxLayout>
#include <QProgressBar>
#include <QPushButton>
#include <QDebug>

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

    DFL::Color a;
    camera = std::make_unique<DFL::Camera>();
}

void Main_window::render_button_clicked()
{

    const auto aspect_ratio = 16.0 / 9.0;
    const int image_width = render_widget->image_width();
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    std::vector<DFL::Color> image_pixels;

    for (int j = image_height - 1; j >= 0; --j)
    {
        for (int i = 0; i < image_width; ++i)
        {
            auto u = double(i) / (image_width - 1);
            auto v = double(j) / (image_height - 1);

            DFL::Ray ray{ camera->get_ray(u, v) };
            DFL::Color pixel_color = camera->ray_color(ray);

            image_pixels.emplace_back(pixel_color);
        }
    }

    render_widget->refresh(image_pixels);
}

void Main_window::update_render_progress(const int &step)
{
    progress_bar->setValue(step);

}
