# add_executable(Day3 main.cpp)
add_executable(Day3 main2.cpp)

target_link_libraries(Day3 PUBLIC compiler_flags)

target_link_libraries(Day3 PUBLIC utils)

target_link_libraries (Day3 PUBLIC Eigen3::Eigen)

configure_file(test_input.txt ${CMAKE_CURRENT_BINARY_DIR}/test_input.txt COPYONLY)
configure_file(input.txt ${CMAKE_CURRENT_BINARY_DIR}/input.txt COPYONLY)
