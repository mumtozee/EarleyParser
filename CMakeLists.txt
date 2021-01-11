cmake_minimum_required(VERSION 3.17)
project(earley_parser)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)
add_subdirectory(lib)
add_executable(main main.cpp)
target_link_libraries(main main_lib)
enable_testing()