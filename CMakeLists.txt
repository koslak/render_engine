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
    src/gui/render_widget.cpp
    src/core/vector3d.cpp
    src/core/point3d.cpp
    src/core/render_thread.cpp
    src/core/hittable.cpp
    src/cameras/camera.cpp
  )

set(CORE_HEADERS
    src/gui/main_window.h
    src/gui/render_widget.h
    src/core/vector3d.h
    src/core/point3d.h
    src/core/dfl.h
    src/core/geometry.h
    src/core/ray.h
    src/core/render_thread.h
    src/core/hittable.h
    src/core/sphere.h
    src/cameras/camera.h
  )

set(ALL_SRC_CODE ${CORE_SOURCE} ${CORE_HEADERS})

file(GLOB TEST_SOURCE
    src/tests/test.cpp
    src/tests/performance_test.cpp
    src/tests/main.cpp
    src/tests/vector3d_tests.cpp
  )

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
# if Qt was built with -reduce-relocations
target_compile_options(${PROJECT_CODE_NAME}_library PRIVATE -fPIC)

target_link_libraries(${PROJECT_CODE_NAME}_executable            # target
                      PUBLIC ${PROJECT_CODE_NAME}_library        # item: library
                      PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)    # item: library


######################################################################################
# Boost Library
######################################################################################
set(BOOST_ROOT "/usr/include/boost")
include_directories(include)

######################################################################################
# Google Test Framework
######################################################################################
# Prevent GoogleTest from overriding our compiler/linker options when building with Visual Studio
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

add_subdirectory(src/ext/google/googletest)
include_directories(src/ext/google/googletest/googletest/src ${CMAKE_SOURCE_DIR}/src/ext/google/googletest/googletest/src)

######################################################################################
# Google Benchmark Framework
######################################################################################
set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "Disable benchmark testing" FORCE)
set(BENCHMARK_ENABLE_EXCEPTIONS OFF CACHE BOOL "Disable benchmark exceptions" FORCE)
set(BENCHMARK_ENABLE_INSTALL OFF CACHE BOOL "Don't install benchmark" FORCE)
set(BENCHMARK_DOWNLOAD_DEPENDENCIES OFF CACHE BOOL "Don't download dependencies" FORCE)
set(BENCHMARK_ENABLE_GTEST_TESTS OFF CACHE BOOL "Disable Google Test in benchmark" FORCE)

add_subdirectory(src/ext/google/benchmark)
include_directories(src/ext/google/benchmark/src ${CMAKE_SOURCE_DIR}/src/ext/google/benchmark)

add_executable(${PROJECT_CODE_NAME}_tests ${TEST_SOURCE})
target_link_libraries(${PROJECT_CODE_NAME}_tests ${PROJECT_CODE_NAME}_library gtest benchmark::benchmark)

######################################################################################
# Adding CMAKE Test
######################################################################################
add_test(test_name ${PROJECT_CODE_NAME}_tests)
