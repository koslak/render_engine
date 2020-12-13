#include "main_window.h"

#include "render_widget.h"

#include <QVBoxLayout>
#include <QPushButton>
#include <QDebug>

#include <memory>

Main_window::Main_window(QWidget *parent) : QMainWindow(parent), render_widget{new Render_widget(this)}, render_button{new QPushButton("Render Button")}
{
    QWidget *central_widget = new QWidget{this};

    QVBoxLayout *vertical_layout = new QVBoxLayout;
    vertical_layout->addWidget(render_widget);
    vertical_layout->addWidget(render_button);

    central_widget->setLayout(vertical_layout);

    this->setCentralWidget(central_widget);

    QObject::connect(render_button, &QPushButton::clicked, this, &Main_window::render_button_clicked);
}

void Main_window::render_button_clicked()
{
    render_widget->refresh();
}
