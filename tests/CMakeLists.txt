include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

set(CMAKE_CXX_FLAGS "-std=c++14")
find_package(OpenCV REQUIRED)
find_library(gekon_lib REQUIRED)

add_executable(runGekonTests
        first_test.cpp)

target_link_libraries(runGekonTests gtest gtest_main gekon_lib ${OpenCV_LIBS})

add_subdirectory(lib)
