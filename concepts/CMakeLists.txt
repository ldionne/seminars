cmake_minimum_required(VERSION 2.8)

project(concepts)

add_definitions(-Wall -Wextra -pedantic)

add_executable(usage_example usage_example.cpp)
add_executable(creation_example creation_example.cpp)
add_executable(archetype_example archetype_example.cpp)
