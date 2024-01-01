# add_executable(Day8 main.cpp)
add_executable(Day8 main2.cpp)

target_link_libraries(Day8 PRIVATE compiler_flags utils)

configure_file(test_input1.txt ${CMAKE_CURRENT_BINARY_DIR}/test_input1.txt COPYONLY)
configure_file(test_input2.txt ${CMAKE_CURRENT_BINARY_DIR}/test_input2.txt COPYONLY)
configure_file(input.txt ${CMAKE_CURRENT_BINARY_DIR}/input.txt COPYONLY)
