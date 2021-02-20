#include "gui/main_window.h"

#include <QApplication>
#include <QDebug>
#include <QScreen>

#include <boost/type_index.hpp>
#include <boost/config.hpp>

#include <iostream>

template<typename T>
void print_type_name(const T &parameter)
{
    qDebug() << "T         = " << QString::fromUtf8(boost::typeindex::type_id_with_cvr<T>().pretty_name().c_str());
    qDebug() << "parameter = " << QString::fromUtf8(boost::typeindex::type_id_with_cvr<decltype(parameter)>().pretty_name().c_str());

    std::cout << "T         = " << boost::typeindex::type_id_with_cvr<T>().pretty_name() << "\n";
    std::cout << "parameter = " << boost::typeindex::type_id_with_cvr<decltype(parameter)>().pretty_name() << "\n";
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Q_INIT_RESOURCE(resource_file);

    QRect window_size{ 0, 0, 0, 0 };

    const QList<QScreen *> screen_list{QGuiApplication::screens()};
    for(QScreen *screen : screen_list)
    {
        if(screen)
        {
            int strings_comparison_result{ QString::compare(screen->name(), QGuiApplication::primaryScreen()->name(), Qt::CaseInsensitive) };
            bool is_screen_name_strings_equal{ true ? strings_comparison_result == 0 : false };

            if(is_screen_name_strings_equal)
            {
                window_size.setSize(screen->geometry().size());
                window_size.adjust(300, 300, -300, -300);

                constexpr const auto aspect_ratio = 16.0 / 9.0;
                window_size.setHeight(static_cast<int>(window_size.width() / aspect_ratio));

                constexpr const int adjust_parameter_for_window_width{ 120 };
                window_size.setWidth( window_size.width() - adjust_parameter_for_window_width);
            }
        }
    }

    Main_window main_window;
    main_window.setWindowTitle("Render Engine Scene Editor");
    main_window.setGeometry(window_size);
    main_window.show();

    return a.exec();
}
