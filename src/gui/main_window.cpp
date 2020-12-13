#include "main_window.h"

#include "render_widget.h"

#include <QVBoxLayout>
#include <QProgressBar>
#include <QPushButton>
#include <QDebug>

#include <memory>

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
}

void Main_window::render_button_clicked()
{
    render_widget->refresh();
}

void Main_window::update_render_progress(const int &step)
{
    progress_bar->setValue(step);

}
