cmake_minimum_required(VERSION 2.8)

project(opencode9)
add_definitions(-Wall -Wextra -pedantic -std=c++11)
if (${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang")
    add_definitions(-stdlib=libc++)
endif()

add_executable(find_disjoint use_cases/find_disjoint.cpp)
add_executable(distance use_cases/distance.cpp)
