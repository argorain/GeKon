project(gekon_tests)

add_definitions(-std=c++14)
add_subdirectory(gtest/googletest)

include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

find_package(OpenCV REQUIRED)

set(TEST_FILES
        first_test.cpp
        gk_utils_test.cpp)

add_executable(runGekonTests ${TEST_FILES})

target_link_libraries(runGekonTests gtest gtest_main gekon ${OpenCV_LIBS} "-lboost_system -lboost_filesystem -lboost_iostreams")

