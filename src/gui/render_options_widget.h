#ifndef RENDER_OPTIONS_WIDGET_H
#define RENDER_OPTIONS_WIDGET_H

#include <QWidget>

class QVBoxLayout;
class QLabel;
class QLineEdit;
class QProgressBar;
class QPushButton;
class QGroupBox;

class Render_options_widget : public QWidget
{
    Q_OBJECT
public:
    explicit Render_options_widget(QWidget *parent = nullptr);

signals:

private:
    QProgressBar *progress_bar;
    QPushButton *render_button;
    QGroupBox *camera_group_box;
    QLabel *look_from_label;
    QPushButton *look_from_coordinates_reset_button;
    QLabel *look_from_x_label;
    QLabel *look_from_y_label;
    QLabel *look_from_z_label;
    QLineEdit *look_from_x_line_edit;
    QLineEdit *look_from_y_line_edit;
    QLineEdit *look_from_z_line_edit;

    QVBoxLayout *create_camera_options_gui_elements() noexcept;


};

#endif // RENDER_OPTIONS_WIDGET_H
