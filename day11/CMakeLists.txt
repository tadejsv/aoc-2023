# add_executable(Day11 main.cpp)
add_executable(Day11 main2.cpp)

target_link_libraries(Day11 PRIVATE compiler_flags utils)

configure_file(test_input.txt ${CMAKE_CURRENT_BINARY_DIR}/test_input.txt COPYONLY)
configure_file(input.txt ${CMAKE_CURRENT_BINARY_DIR}/input.txt COPYONLY)
