cmake_minimum_required(VERSION 3.27)

set(CMAKE_CXX_CLANG_TIDY clang-tidy)
set(CMAKE_CXX_INCLUDE_WHAT_YOU_USE include-what-you-use)
set(LINK_WHAT_YOU_USE ON)
set(CMAKE_CXX_EXTENSIONS OFF)

project(AOC2023
    VERSION 1.0
    LANGUAGES CXX
)

add_library(compiler_flags INTERFACE)
target_compile_features(compiler_flags INTERFACE cxx_std_20)
target_compile_options(compiler_flags INTERFACE -Wall -Wextra -Wpedantic -Wconversion -Wsign-conversion -stdlib=libc++)
target_link_options(compiler_flags INTERFACE -stdlib=libc++)

add_subdirectory(utils)

add_subdirectory(day1)
add_subdirectory(day2)
add_subdirectory(day3)