cmake_minimum_required(VERSION 3.28)

project(AOC2023
    VERSION 1.0
    LANGUAGES CXX
)

add_library(compiler_flags INTERFACE)
target_compile_features(compiler_flags INTERFACE cxx_std_23)

# 3rd party dependencies
find_package(Eigen3 3.4.90 REQUIRED NO_MODULE)
find_package(Catch2 3.5.3 REQUIRED NO_MODULE)

include(CTest)
include(Catch)

add_subdirectory(utils)

add_subdirectory(day1)
add_subdirectory(day2)
add_subdirectory(day3)
add_subdirectory(day4)
add_subdirectory(day5)
add_subdirectory(day6)
add_subdirectory(day7)
add_subdirectory(day8)
add_subdirectory(day9)
add_subdirectory(day10)
add_subdirectory(day11)
add_subdirectory(day12)
add_subdirectory(day13)
add_subdirectory(day14)