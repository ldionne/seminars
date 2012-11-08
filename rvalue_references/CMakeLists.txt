cmake_minimum_required(VERSION 2.8)

project(rvalue_references)
add_definitions(-std=c++11 -Wno-unused-variable
                -Wno-unused-value -Wno-vexing-parse)

add_executable(template_deduction template_deduction.cpp)
add_executable(value_categories value_categories.cpp)
