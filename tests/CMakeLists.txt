include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

add_executable(runGekonTests
        first_test.cpp)

target_link_libraries(runGekonTests gtest gtest_main)

add_subdirectory(lib)
