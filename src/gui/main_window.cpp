#include "main_window.h"

#include "render_widget.h"

#include <QDebug>
#include <QLabel>
#include <QProgressBar>
#include <QPushButton>
#include <QStatusBar>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTimer>

Main_window::Main_window(QWidget *parent) : QMainWindow(parent), progress_bar{new QProgressBar(this)},
                                                                 render_button{new QPushButton("Start Rendering")},
                                                                 render_widget{new Render_widget(this)}
{
    elapsed_timer = std::make_unique<QElapsedTimer>();
    timer = new QTimer{this};

    progress_bar->setMaximum(99);

    QWidget *central_widget = new QWidget{this};
    status_bar_label = new QLabel{this};
    status_bar_label->setMinimumWidth(250);
    status_bar_label->setMaximumWidth(250);
    milliseconds_label = new QLabel{this};

    QVBoxLayout *vertical_layout = new QVBoxLayout;
    vertical_layout->addWidget(render_widget);
    vertical_layout->addWidget(render_button);
    vertical_layout->addWidget(progress_bar);

    statusBar()->addPermanentWidget(status_bar_label);

    QObject::connect(render_widget, &Render_widget::update_gui, this, &Main_window::update_gui);
    QObject::connect(render_widget, &Render_widget::render_image_finished, this, &Main_window::render_image_finished);
    QObject::connect(render_widget, &Render_widget::start_render_by_mouse_action, this, &Main_window::render_button_clicked);
    QObject::connect(render_button, &QPushButton::clicked, this, &Main_window::render_button_clicked);
    QObject::connect(timer, &QTimer::timeout, this, &Main_window::timer_update);

    central_widget->setLayout(vertical_layout);
    this->setCentralWidget(central_widget);
}

void Main_window::render_button_clicked()
{
    elapsed_timer->restart();
    timer->start(1);
    render_widget->start_render_image();
}

QString Main_window::format_time(qint64 elapsed_time_in_ms) const noexcept
{
    QString formatted_time;

    int hours = elapsed_time_in_ms / (1000 * 60 * 60);
    int minutes = (elapsed_time_in_ms - (hours * 1000 * 60 * 60)) / (1000 * 60);
    int seconds = (elapsed_time_in_ms - (minutes * 1000 * 60) - (hours * 1000 * 60 * 60)) / 1000;
    int milliseconds = elapsed_time_in_ms - (seconds * 1000) - (minutes * 1000 * 60) - (hours * 1000 * 60 * 60);

    formatted_time.append(QString("%1").arg(hours, 2, 10, QLatin1Char('0')) + ":" +
                         QString( "%1" ).arg(minutes, 2, 10, QLatin1Char('0')) + ":" +
                         QString( "%1" ).arg(seconds, 2, 10, QLatin1Char('0')) + ":" +
                         QString( "%1" ).arg(milliseconds, 3, 10, QLatin1Char('0')));

    return formatted_time;
}

void Main_window::timer_update()
{
    status_bar_label->setText("Rendering time (h:m:s:ss): " + format_time(elapsed_timer->elapsed()));
}

void Main_window::update_gui(int progress)
{
    progress_bar->setValue(progress);
}

void Main_window::render_image_finished()
{
    timer->stop();
}
