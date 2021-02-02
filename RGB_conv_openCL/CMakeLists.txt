cmake_minimum_required(VERSION 3.10)
project(OpenCL_Tutorial VERSION 1.0)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_COMPILER "/usr/bin/g++")

add_definitions(-DHW=${HW})

add_executable(Rgb_convertor ./sources/main.cpp)
include_directories(./includes)
target_link_libraries(Rgb_convertor OpenCL)


