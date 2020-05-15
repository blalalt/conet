project(conet)
cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 14)

add_subdirectory(./log)
add_subdirectory(./net)

add_subdirectory(./test)