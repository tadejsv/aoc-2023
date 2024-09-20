# add_executable(Day6 main.cpp)
add_executable(Day6 main2.cpp)

add_subdirectory(lib)
add_subdirectory(tests)

target_link_libraries(Day6 PRIVATE compiler_flags utils)
target_link_libraries(Day6 PRIVATE Day6Lib)

configure_file(test_input.txt ${CMAKE_CURRENT_BINARY_DIR}/test_input.txt COPYONLY)
configure_file(input.txt ${CMAKE_CURRENT_BINARY_DIR}/input.txt COPYONLY)
