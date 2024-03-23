# add_executable(Day12 main.cpp)
add_executable(Day12 main2.cpp)

target_link_libraries(Day12 PRIVATE compiler_flags utils)

configure_file(test_input.txt ${CMAKE_CURRENT_BINARY_DIR}/test_input.txt COPYONLY)
configure_file(input.txt ${CMAKE_CURRENT_BINARY_DIR}/input.txt COPYONLY)
