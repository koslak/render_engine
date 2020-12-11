cmake_minimum_required(VERSION 3.5)

set(PROJECT_CODE_NAME "DFL")

project(${PROJECT_CODE_NAME} VERSION 0.0.1 LANGUAGES CXX)

######################################################################################
# Git submodules validation
######################################################################################
if(NOT IS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/src/ext/google/googletest")
  message(FATAL_ERROR "The Google Test submodule directory is missing! "
    "You probably did not clone the project with --recursive. It is possible to recover "
    "by running \"git submodule update --init --recursive\"")
endif()

if(NOT IS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/src/ext/google/benchmark")
  message(FATAL_ERROR "The Google Benchmark submodule directory is missing! "
    "You probably did not clone the project with --recursive. It is possible to recover "
    "by running \"git submodule update --init --recursive\"")
endif()

######################################################################################
# Compiler specific configurations
######################################################################################
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

######################################################################################
# Finding External Packages
######################################################################################
find_package(QT NAMES Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)

######################################################################################
# Definition of the source/headers files
######################################################################################
set(CORE_SOURCE
    src/main/main.cpp
    src/gui/main_window.cpp
  )

set(CORE_HEADERS
    src/gui/main_window.h
  )

set(ALL_SRC_CODE ${CORE_SOURCE} ${CORE_HEADERS})

include_directories(src)

######################################################################################
# Source code static library
######################################################################################
add_library(${PROJECT_CODE_NAME}_library STATIC ${ALL_SRC_CODE})
target_include_directories(${PROJECT_CODE_NAME}_library PUBLIC ${Qt5Widgets_INCLUDE_DIRS})

if(WIN32)
  # Avoid a name clash when building on Visual Studio
  set_target_properties (${PROJECT_CODE_NAME}_library PROPERTIES OUTPUT_NAME lib${PROJECT_CODE_NAME} )
endif()

######################################################################################
# Adding Executable
######################################################################################
add_executable(${PROJECT_CODE_NAME}_executable src/main/main.cpp)

# You must build your code with position independent code (i.e., use the fPIC flag for the library)
# if Qt was built with -reduce-relocations.
target_compile_options(${PROJECT_CODE_NAME}_library PRIVATE -fPIC)

target_link_libraries(${PROJECT_CODE_NAME}_executable            # target
                      PUBLIC ${PROJECT_CODE_NAME}_library        # item: library
                      PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)    # item: library
