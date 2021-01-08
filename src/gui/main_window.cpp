#include "main_window.h"

#include "render_widget.h"

#include <QProgressBar>
#include <QPushButton>
#include <QVBoxLayout>

Main_window::Main_window(QWidget *parent) : QMainWindow(parent), progress_bar{new QProgressBar(this)},
                                                                 render_button{new QPushButton("Render Button")},
                                                                 render_widget{new Render_widget(this)}
{
    progress_bar->setMaximum(99);

    QWidget *central_widget = new QWidget{this};

    QVBoxLayout *vertical_layout = new QVBoxLayout;
    vertical_layout->addWidget(render_widget);
    vertical_layout->addWidget(render_button);
    vertical_layout->addWidget(progress_bar);

    QObject::connect(render_button, &QPushButton::clicked, this, &Main_window::render_button_clicked);

    central_widget->setLayout(vertical_layout);
    this->setCentralWidget(central_widget);
}

void Main_window::render_button_clicked()
{
    render_widget->start_render_image();
}

