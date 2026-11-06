cmake_minimum_required(VERSION 3.21)

project(MojaPierwszaAplikacja LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

qt_standard_project_setup()

qt_add_executable(MojaPierwszaAplikacja
    main.cpp
    mainwindow.hpp
    mainwindow.cpp
)

target_link_libraries(MojaPierwszaAplikacja PRIVATE Qt6::Widgets)