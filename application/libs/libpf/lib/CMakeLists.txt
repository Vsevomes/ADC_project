cmake_minimum_required(VERSION 3.5)

project(ADC_trans_lib VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

set(LIB_SOURCES
        src/function.cpp
        src/function.h
        src/protocol.cpp
        src/protocol.h
)

add_library(ADC_trans_lib STATIC ${LIB_SOURCES})

target_link_libraries(ADC_trans_lib PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
