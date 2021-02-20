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
    src/gui/render_options_widget.cpp
    src/core/vector3d.cpp
    src/core/point3d.cpp
    src/core/render_thread.cpp
    src/cameras/camera.cpp
    src/core/api.cpp
    src/core/scene.cpp
  )

set(CORE_HEADERS
    src/gui/main_window.h
    src/gui/render_widget.h
    src/gui/render_options_widget.h
    src/core/vector3d.h
    src/core/point3d.h
    src/core/dfl.h
    src/core/geometry.h
    src/core/ray.h
    src/core/render_thread.h
    src/core/hittable.h
    src/core/hittable_list.h
    src/core/sphere.h
    src/cameras/camera.h
    src/core/material.h
    src/core/api.h
    src/core/scene.h
  )

set(ALL_SRC_CODE ${CORE_SOURCE} ${CORE_HEADERS})

file(GLOB TEST_SOURCE
    src/tests/test.cpp
    src/tests/performance_test.cpp
    src/tests/main.cpp
    src/tests/vector3d_tests.cpp
  )

include_directories(src)

qt5_add_resources(ALL_SRC_CODE resource_file.qrc)

if(UNIX AND NOT APPLE)
    set(LINUX TRUE)
endif()

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
                      PRIVATE Qt${QT_VERSION_MAJOR}::Widgets     # item: library
                      PUBLIC ${Boost_STACKTRACE_BASIC_LIBRARY}
                      PUBLIC ${Boost_STACKTRACE_BACKTRACE_LIBRARY}
                      PUBLIC ${Boost_STACKTRACE_ADDR2LINE_LIBRARY}
                      PUBLIC ${Boost_STACKTRACE_NOOP_LIBRARY}
                      PUBLIC pthread dl)

######################################################################################
# Boost Library
######################################################################################
find_package(Boost 1.75.0)

if(${Boost_FOUND})
    message(STATUS "---------------------")
    message(STATUS "Boost library found")

    if(LINUX)
        #set(BOOST_ROOT "/usr/include/boost")
        set(BOOST_ROOT ${Boost_INCLUDE_DIRS})
        include_directories(include)
    endif()

    if(APPLE)
        set(BOOST_ROOT ${Boost_INCLUDE_DIRS})
        include_directories(${Boost_INCLUDE_DIRS})

        #flag required to use Boost Stacktrace in Apple's platform.
        add_compile_definitions("_GNU_SOURCE")
    endif()

    message(STATUS "Setting BOOST_ROOT to: " ${BOOST_ROOT})
    message(STATUS "Setting Boost_INCLUDE_DIRS to: " ${Boost_INCLUDE_DIRS})
    message(STATUS "---------------------")
elseif()
    message(FATAL_ERROR "The Boost library has not being found.")

endif()

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
target_link_libraries(${PROJECT_CODE_NAME}_tests
                      ${PROJECT_CODE_NAME}_library
                      gtest benchmark::benchmark
                      ${Boost_STACKTRACE_BASIC_LIBRARY}
                      ${Boost_STACKTRACE_BACKTRACE_LIBRARY}
                      ${Boost_STACKTRACE_ADDR2LINE_LIBRARY}
                      ${Boost_STACKTRACE_NOOP_LIBRARY}
                      pthread dl)

######################################################################################
# Adding CMAKE Test
######################################################################################
add_test(test_name ${PROJECT_CODE_NAME}_tests)
