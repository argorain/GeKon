cmake_minimum_required (VERSION 2.8)
project (gekon)

find_package(OpenCV REQUIRED)

add_executable(gekon main.cpp)

target_link_libraries( gekon ${OpenCV_LIBS} )
