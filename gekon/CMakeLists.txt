cmake_minimum_required (VERSION 2.8)

project (gekon)

set(CMAKE_CXX_COMPILER "g++")
set(CMAKE_CXX_FLAGS "-Wall -pedantic -std=c++14 -g -DNUM_THREADS=8")

find_package(OpenCV REQUIRED)

set(HEADER_FILES
        gk_types.h
        gk_utils.h
        gk_functions.h
        gk_operators.h
		gnuplot-iostream.h
		gk_Worker.h
       )

set(SOURCE_FILES
        gk_functions.cpp
		gk_utils.cpp
		gk_operators.cpp
		gk_Worker.cpp
       )


add_executable(playground playground.cpp ${SOURCE_FILES} ${HEADER_FILES})
add_executable(gekon_run main.cpp ${SOURCE_FILES} ${HEADER_FILES})

#target_link_libraries(playground ${OpenCV_LIBS} "-lstdc++fs")
#target_link_libraries(gekon_run ${OpenCV_LIBS} "-lstdc++fs")
target_link_libraries(playground ${OpenCV_LIBS} "-lboost_system -lboost_filesystem -lboost_iostreams")
target_link_libraries(gekon_run ${OpenCV_LIBS} "-lboost_system -lboost_filesystem -lboost_iostreams")

add_library(gekon STATIC ${SOURCE_FILES} ${HEADER_FILES})
