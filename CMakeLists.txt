cmake_minimum_required(VERSION 3.5)

project(render_engine LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)

include_directories(src)

add_executable(render_engine
    src/main/main.cpp
    src/gui/main_window.cpp
    src/gui/main_window.h
)

target_link_libraries(render_engine PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
