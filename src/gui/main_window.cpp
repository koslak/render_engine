#include "main_window.h"

#include "render_widget.h"

#include <QLabel>
#include <QProgressBar>
#include <QPushButton>
#include <QStatusBar>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTimer>

Status_bar_widget::Status_bar_widget(QWidget *parent) : QWidget(parent)
{
    status_bar_label = new QLabel{ this };
    hours_label = new QLabel{ this };
    minutes_label = new QLabel{ this };
    seconds_label = new QLabel{ this };
    milliseconds_label = new QLabel{ this };
    milliseconds_label->setMinimumWidth(50);
    milliseconds_label->setMaximumWidth(50);

    QSpacerItem *spacer_item{ new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Expanding) };

    QHBoxLayout *horizontal_layout = new QHBoxLayout;
    horizontal_layout->addItem(spacer_item);
    horizontal_layout->addWidget(status_bar_label);
    horizontal_layout->addWidget(hours_label);
    horizontal_layout->addWidget(minutes_label);
    horizontal_layout->addWidget(seconds_label);
    horizontal_layout->addWidget(milliseconds_label);

    this->setLayout(horizontal_layout);
}

constexpr void Status_bar_widget::set_status_bar_text(const QString &text) noexcept
{
    status_bar_label->setText(text);
    milliseconds_label->setText("");
}

void Status_bar_widget::update_status_bar_timer(const QString &hour, const QString &minutes, const QString &seconds, const QString &milliseconds)
{
    status_bar_label->setText("Elapsed Time: ");
    hours_label->setText(hour);
    minutes_label->setText(minutes);
    seconds_label->setText(seconds);
    milliseconds_label->setText(milliseconds);
}

Main_window::Main_window(QWidget *parent) : QMainWindow(parent), progress_bar{new QProgressBar(this)},
                                                                 render_button{new QPushButton("Start Rendering")},
                                                                 render_widget{new Render_widget(this)}
{
    elapsed_timer = std::make_unique<QElapsedTimer>();
    timer = new QTimer{this};

    progress_bar->setMaximum(99);

    QWidget *central_widget = new QWidget{this};
    status_bar_label = new QLabel{this};
    status_bar_label->setMinimumWidth(170);
    status_bar_label->setMaximumWidth(170);
    milliseconds_label = new QLabel{this};

    QVBoxLayout *vertical_layout = new QVBoxLayout;
    vertical_layout->addWidget(render_widget);
    vertical_layout->addWidget(render_button);
    vertical_layout->addWidget(progress_bar);

    status_bar_widget = new Status_bar_widget{ this };
    statusBar()->addPermanentWidget(status_bar_label);

    QObject::connect(render_widget, &Render_widget::update_gui, this, &Main_window::update_gui);
    QObject::connect(render_widget, &Render_widget::render_image_finished, this, &Main_window::render_image_finished);
    QObject::connect(render_button, &QPushButton::clicked, this, &Main_window::render_button_clicked);
    QObject::connect(timer, &QTimer::timeout, this, &Main_window::timer_update);

    central_widget->setLayout(vertical_layout);
    this->setCentralWidget(central_widget);
}

void Main_window::render_button_clicked()
{
    elapsed_timer->start();
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

    /*
    status_bar_widget->update_status_bar_timer(QString("%1").arg(hours, 2, 10, QLatin1Char('0')),
                                               QString( "%1" ).arg(minutes, 2, 10, QLatin1Char('0')),
                                               QString( "%1" ).arg(seconds, 2, 10, QLatin1Char('0')),
                                               QString( "%1" ).arg(milliseconds, 3, 10, QLatin1Char('0')));
    */

    return formatted_time;
}

void Main_window::timer_update()
{
//    format_time(elapsed_timer->elapsed());
//    status_bar_widget->set_status_bar_text("Elapsed time: " + format_time(elapsed_timer->elapsed()));
    status_bar_label->setText("Elapsed time: " + format_time(elapsed_timer->elapsed()));
}

void Main_window::update_gui(int progress)
{
    progress_bar->setValue(progress);
}

void Main_window::render_image_finished()
{
    timer->stop();
}













