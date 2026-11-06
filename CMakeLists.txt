cmake_minimum_required(VERSION 3.16)

project(LFM LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

qt_add_executable(LFM
    main.cpp
    mainwindow.h
    mainwindow.cpp
)

target_link_libraries(LFM PRIVATE Qt6::Widgets)