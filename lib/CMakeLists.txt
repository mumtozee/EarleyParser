project(Google_tests)

add_subdirectory(googletest)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

add_library(main_lib STATIC ../src/Solution.cpp)
add_executable(main_test ../test/main_test.cpp)
target_link_libraries(main_test gtest gtest_main main_lib)