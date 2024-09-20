# add_executable(Day4 main.cpp)
add_executable(Day4 main2.cpp)

target_link_libraries(Day4 PUBLIC compiler_flags)

target_link_libraries(Day4 PUBLIC utils)

configure_file(test_input.txt ${CMAKE_CURRENT_BINARY_DIR}/test_input.txt COPYONLY)
configure_file(input.txt ${CMAKE_CURRENT_BINARY_DIR}/input.txt COPYONLY)
