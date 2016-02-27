cmake_minimum_required (VERSION 2.8)
project (gekon)

set(CMAKE_CXX_COMPILER "g++")
set(CMAKE_CXX_FLAGS "-Wall -pedantic -std=c++14")

find_package(OpenCV REQUIRED)

set(HEADER_FILES
        gk_types.h
        gk_utils.h
        gk_functions.h
        gk_operators.h
       )

set(SOURCE_FILES
        gk_functions.cpp
       )

add_executable(gekon main.cpp ${SOURCE_FILES} ${HEADER_FILES})

target_link_libraries(gekon ${OpenCV_LIBS})

add_library(gekon_lib STATIC ${SOURCE_FILES} ${HEADER_FILES})