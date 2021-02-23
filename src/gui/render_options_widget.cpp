#include "render_options_widget.h"

#include <QDebug>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QProgressBar>
#include <QPushButton>
#include <QVBoxLayout>

Render_options_widget::Render_options_widget(QWidget *parent) : QWidget(parent)
{
    render_button = new QPushButton("Start Rendering");
    progress_bar = new QProgressBar(this);

    camera_group_box = new QGroupBox{ "Camera Options" };
    camera_group_box->setLayout(create_camera_options_gui_elements());

    QVBoxLayout *vertical_layout = new QVBoxLayout;
    vertical_layout->addWidget(camera_group_box);
    vertical_layout->addWidget(progress_bar);

    QSpacerItem *vertical_spacer = new QSpacerItem(0, 1000, QSizePolicy::Expanding, QSizePolicy::Expanding);
    vertical_layout->addItem(vertical_spacer);
    vertical_layout->addWidget(render_button);

    this->setLayout(vertical_layout);
}

QVBoxLayout *Render_options_widget::create_camera_options_gui_elements() noexcept
{
    look_from_label = new QLabel{ "Look From Coordinates"};
    look_from_coordinates_reset_button = new QPushButton{};
    look_at_label = new QLabel{ "Look At Coordinates"};
    look_at_coordinates_reset_button = new QPushButton{};
    QPixmap pixmap(":/img/arrow_reset_button_icon.png");
    QIcon iconBack(pixmap);
    look_from_coordinates_reset_button->setIcon(iconBack);
    look_from_coordinates_reset_button->setFlat(true);
    look_from_coordinates_reset_button->setMinimumWidth(35);
    look_from_coordinates_reset_button->setMaximumWidth(35);
    look_at_coordinates_reset_button->setIcon(iconBack);
    look_at_coordinates_reset_button->setFlat(true);
    look_at_coordinates_reset_button->setMinimumWidth(35);
    look_at_coordinates_reset_button->setMaximumWidth(35);

    look_from_x_label = new QLabel{ "X" };
    look_from_y_label = new QLabel{ "Y" };
    look_from_z_label = new QLabel{ "Z" };
    look_from_x_line_edit = new QLineEdit{};
    look_from_x_line_edit->setMinimumWidth(50);
    look_from_x_line_edit->setMaximumWidth(50);
    look_from_y_line_edit = new QLineEdit{};
    look_from_y_line_edit->setMinimumWidth(50);
    look_from_y_line_edit->setMaximumWidth(50);
    look_from_z_line_edit = new QLineEdit{};
    look_from_z_line_edit->setMinimumWidth(50);
    look_from_z_line_edit->setMaximumWidth(50);

    look_at_x_label = new QLabel{ "X" };
    look_at_y_label = new QLabel{ "Y" };
    look_at_z_label = new QLabel{ "Z" };
    look_at_x_line_edit = new QLineEdit{};
    look_at_x_line_edit->setMinimumWidth(50);
    look_at_x_line_edit->setMaximumWidth(50);
    look_at_y_line_edit = new QLineEdit{};
    look_at_y_line_edit->setMinimumWidth(50);
    look_at_y_line_edit->setMaximumWidth(50);
    look_at_z_line_edit = new QLineEdit{};
    look_at_z_line_edit->setMinimumWidth(50);
    look_at_z_line_edit->setMaximumWidth(50);

    QHBoxLayout *look_from_hbox_coordinates = new QHBoxLayout;
    look_from_hbox_coordinates->addWidget(look_from_x_label);
    look_from_hbox_coordinates->addWidget(look_from_x_line_edit);
    look_from_hbox_coordinates->addWidget(look_from_y_label);
    look_from_hbox_coordinates->addWidget(look_from_y_line_edit);
    look_from_hbox_coordinates->addWidget(look_from_z_label);
    look_from_hbox_coordinates->addWidget(look_from_z_line_edit);
    look_from_hbox_coordinates->addWidget(look_from_coordinates_reset_button);

    QHBoxLayout *look_at_hbox_coordinates = new QHBoxLayout;
    look_at_hbox_coordinates->addWidget(look_at_x_label);
    look_at_hbox_coordinates->addWidget(look_at_x_line_edit);
    look_at_hbox_coordinates->addWidget(look_at_y_label);
    look_at_hbox_coordinates->addWidget(look_at_y_line_edit);
    look_at_hbox_coordinates->addWidget(look_at_z_label);
    look_at_hbox_coordinates->addWidget(look_at_z_line_edit);
    look_at_hbox_coordinates->addWidget(look_at_coordinates_reset_button);

    QVBoxLayout *look_from_vbox_coordinates = new QVBoxLayout;
    look_from_vbox_coordinates->addWidget(look_from_label);
    look_from_vbox_coordinates->addLayout(look_from_hbox_coordinates);
    look_from_vbox_coordinates->addWidget(look_at_label);
    look_from_vbox_coordinates->addLayout(look_at_hbox_coordinates);

    return look_from_vbox_coordinates;
}
