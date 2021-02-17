#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QElapsedTimer>

class QLabel;
class QPushButton;
class QProgressBar;
class Render_widget;
class QTimer;

class Status_bar_widget : public QWidget
{
    Q_OBJECT
public:
    Status_bar_widget(QWidget *parent = nullptr);
    constexpr void set_status_bar_text(const QString &text) noexcept;

public slots:
    void update_status_bar_timer(const QString &hour, const QString &minutes, const QString &seconds, const QString &milliseconds);

private:
    QLabel *status_bar_label;
    QLabel *hours_label;
    QLabel *minutes_label;
    QLabel *seconds_label;
    QLabel *milliseconds_label;
};

class Main_window : public QMainWindow
{
    Q_OBJECT

public:
    Main_window(QWidget *parent = nullptr);

public slots:
    void render_button_clicked();
    void update_gui(int progress);
    void timer_update();
    void render_image_finished();

private:
    QString format_time(qint64 elapsed_time_in_ms) const noexcept;

    QProgressBar *progress_bar;
    QPushButton *render_button;
    QLabel *status_bar_label;
    Status_bar_widget *status_bar_widget;
    QLabel *milliseconds_label;
    std::unique_ptr<QElapsedTimer> elapsed_timer;
    QTimer *timer;

    Render_widget *render_widget;
};

#endif // MAIN_WINDOW_H
