add_library(utils STATIC src/utils.cpp)

target_link_libraries(utils PUBLIC compiler_flags)

target_include_directories(utils PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
