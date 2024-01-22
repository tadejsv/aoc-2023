# add_executable(Day10 main.cpp)
add_executable(Day10 main2.cpp)

target_link_libraries(Day10 PRIVATE compiler_flags utils)

target_link_libraries (Day10 PUBLIC Eigen3::Eigen)

configure_file(test_input.txt ${CMAKE_CURRENT_BINARY_DIR}/test_input.txt COPYONLY)
configure_file(input.txt ${CMAKE_CURRENT_BINARY_DIR}/input.txt COPYONLY)
