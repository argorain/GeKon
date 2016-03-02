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
		gk_utils.cpp
       )


add_executable(playground playground.cpp ${SOURCE_FILES} ${HEADER_FILES})
add_executable(gekon_run main.cpp ${SOURCE_FILES} ${HEADER_FILES})

target_link_libraries(playground ${OpenCV_LIBS} "-lstdc++fs")
target_link_libraries(gekon_run ${OpenCV_LIBS} "-lstdc++fs")

add_library(gekon STATIC ${SOURCE_FILES} ${HEADER_FILES})
