cmake_minimum_required(VERSION 3.0.0)
project(graph VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(third-party)

add_executable(bigint main.cpp)
target_link_libraries(bigint spdlog)

