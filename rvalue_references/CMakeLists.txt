cmake_minimum_required(VERSION 2.8)

project(rvalue_references)
add_definitions(-std=c++11)

add_executable(template_deduction template_deduction.cpp)
