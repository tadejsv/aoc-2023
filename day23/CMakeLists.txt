# add_executable(Day23 main.cpp)
add_executable(Day23 main2.cpp)

target_link_libraries(Day23 PRIVATE Eigen3::Eigen compiler_flags utils)

configure_file(test_input.txt ${CMAKE_CURRENT_BINARY_DIR}/test_input.txt COPYONLY)
configure_file(input.txt ${CMAKE_CURRENT_BINARY_DIR}/input.txt COPYONLY)
