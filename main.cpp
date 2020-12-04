#include "main_window.h"

#include <QApplication>
#include <QDebug>
#include <QScreen>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QRect window_size{ 0, 0, 0, 0 };

    for(auto screen : QGuiApplication::screens())
    {
        bool is_screen_name_strings_equal = QString::compare(screen->name(), QGuiApplication::primaryScreen()->name(), Qt::CaseInsensitive);
        if(is_screen_name_strings_equal)
        {
            window_size.setSize(screen->geometry().size());
            window_size.adjust(200, 200, -200, -200);
        }
    }

    Main_window main_window;
    main_window.setWindowTitle("Render Engine Scene Editor");
    main_window.setGeometry(window_size);
    main_window.show();
    return a.exec();
}
