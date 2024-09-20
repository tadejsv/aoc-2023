add_executable(Day6Tests tests.cpp)

target_link_libraries(Day6Tests PRIVATE Day6Lib)

target_link_libraries(Day6Tests PRIVATE compiler_flags)

target_link_libraries(Day6Tests PRIVATE Catch2::Catch2WithMain)

catch_discover_tests(Day6Tests)
