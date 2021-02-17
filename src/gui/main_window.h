#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>
#include <QElapsedTimer>

class QLabel;
class QPushButton;
class QProgressBar;
class Render_widget;
class QTimer;

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
    QLabel *milliseconds_label;
    std::unique_ptr<QElapsedTimer> elapsed_timer;
    QTimer *timer;

    Render_widget *render_widget;
};

#endif // MAIN_WINDOW_H
