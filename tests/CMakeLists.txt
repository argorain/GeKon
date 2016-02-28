project(gekon_tests)

add_definitions(-std=c++14)
add_subdirectory(gtest/googletest)

include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

find_package(OpenCV REQUIRED)

add_executable(runGekonTests
        first_test.cpp)

target_link_libraries(runGekonTests gtest gtest_main gekon ${OpenCV_LIBS})

