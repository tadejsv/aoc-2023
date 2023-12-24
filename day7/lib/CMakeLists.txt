add_library(Day7Lib STATIC day7.cpp)

target_link_libraries(Day7Lib PRIVATE compiler_flags)

target_include_directories(Day7Lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
