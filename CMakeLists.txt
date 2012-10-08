cmake_minimum_required(VERSION 2.6)

project(seminars)

# boost package
find_package(Boost 1.51.0 REQUIRED)
include_directories(${Boost_INCLUDE_DIR})
link_directories(${Boost_LIBRARY_DIRS})
