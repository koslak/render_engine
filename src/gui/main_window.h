#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>

class Render_widget;
class QPushButton;
class QProgressBar;

class Main_window : public QMainWindow
{
    Q_OBJECT

public:
    Main_window(QWidget *parent = nullptr);

public slots:
    void render_button_clicked();
    void update_render_progress(const int &step);

private:
    Render_widget *render_widget;
    QProgressBar *progress_bar;
    QPushButton *render_button;
};

#endif // MAIN_WINDOW_H
