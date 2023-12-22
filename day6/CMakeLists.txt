# add_executable(Day6 main.cpp)
add_executable(Day6 main2.cpp)

target_link_libraries(Day6 PUBLIC compiler_flags)

target_link_libraries(Day6 PUBLIC utils)

configure_file(test_input.txt ${CMAKE_CURRENT_BINARY_DIR}/test_input.txt COPYONLY)
configure_file(input.txt ${CMAKE_CURRENT_BINARY_DIR}/input.txt COPYONLY)
