cmake_minimum_required (VERSION 2.8)
project (gekon)

set(CMAKE_CXX_COMPILER "g++")
set(CMAKE_CXX_FLAGS "-Wall -pedantic -std=c++14")

find_package(OpenCV REQUIRED)

add_executable(gekon main.cpp gk_utils.h gk_functions.h gk_operators.h gk_functions.cpp)

target_link_libraries( gekon ${OpenCV_LIBS} )
