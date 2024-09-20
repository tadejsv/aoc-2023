# add_executable(Day21 main.cpp)
add_executable(Day21 main2.cpp)
# add_executable(Day21 good.cpp)

target_link_libraries(Day21 PRIVATE Eigen3::Eigen compiler_flags utils)

configure_file(test_input.txt ${CMAKE_CURRENT_BINARY_DIR}/test_input.txt COPYONLY)
configure_file(input.txt ${CMAKE_CURRENT_BINARY_DIR}/input.txt COPYONLY)
configure_file(ti2.txt ${CMAKE_CURRENT_BINARY_DIR}/ti2.txt COPYONLY)
