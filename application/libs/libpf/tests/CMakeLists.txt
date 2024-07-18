cmake_minimum_required(VERSION 3.5)

project(ADC_trans_tests VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(GTest REQUIRED)

set(TEST_SOURCES
    test_main.cpp
)

add_executable(ADC_trans_tests ${TEST_SOURCES})
add_subdirectory(../lib build)
include_directories(../lib/src)


target_link_libraries(ADC_trans_tests GTest::GTest GTest::Main ADC_trans_lib)