cmake_minimum_required(VERSION 3.28)
project(lab2)

set(CMAKE_CXX_STANDARD 20)
add_subdirectory(tree)
include_directories(tree)
add_subdirectory(DFA)
include_directories(DFA)
add_subdirectory(regex)
include_directories(regex)
add_executable(lab2 main.cpp)
target_link_libraries(lab2 ST DFA reg)

