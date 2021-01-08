#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>

class QPushButton;
class QProgressBar;
class Render_widget;

class Main_window : public QMainWindow
{
    Q_OBJECT

public:
    Main_window(QWidget *parent = nullptr);

public slots:
    void render_button_clicked();
    void update_progress_bar(int progress);

private:
    QProgressBar *progress_bar;
    QPushButton *render_button;

    Render_widget *render_widget;
};

#endif // MAIN_WINDOW_H
