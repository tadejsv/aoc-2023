add_library(Day6Lib STATIC day6.cpp)

target_link_libraries(Day6Lib PUBLIC compiler_flags)

target_include_directories(Day6Lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
