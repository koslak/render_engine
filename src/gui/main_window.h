#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>

class Render_widget;
class QPushButton;

class Main_window : public QMainWindow
{
    Q_OBJECT

public:
    Main_window(QWidget *parent = nullptr);

public slots:
    void render_button_clicked();

private:
    Render_widget *render_widget;
    QPushButton *render_button;
};

#endif // MAIN_WINDOW_H
