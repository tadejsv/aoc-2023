# add_executable(Day7 main.cpp)
add_executable(Day7 main2.cpp)

add_subdirectory(lib)
add_subdirectory(tests)

target_link_libraries(Day7 PRIVATE compiler_flags utils)
target_link_libraries(Day7 PRIVATE Day7Lib)

configure_file(test_input.txt ${CMAKE_CURRENT_BINARY_DIR}/test_input.txt COPYONLY)
configure_file(input.txt ${CMAKE_CURRENT_BINARY_DIR}/input.txt COPYONLY)
