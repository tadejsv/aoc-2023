cmake_minimum_required(VERSION 3.27)

project(AOC2023
    VERSION 1.0
    LANGUAGES CXX
)

add_library(compiler_flags INTERFACE)
target_compile_features(compiler_flags INTERFACE cxx_std_23)

add_subdirectory(utils)

# Don't want the past to hold me back :)
# add_subdirectory(day1)
# add_subdirectory(day2)
add_subdirectory(day3)
add_subdirectory(day4)
add_subdirectory(day5)