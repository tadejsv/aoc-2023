add_executable(Day7Tests tests.cpp)

target_link_libraries(Day7Tests PRIVATE Day7Lib)

target_link_libraries(Day7Tests PRIVATE compiler_flags)

target_link_libraries(Day7Tests PRIVATE Catch2::Catch2WithMain)

catch_discover_tests(Day7Tests)
