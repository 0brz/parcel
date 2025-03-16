cmake_minimum_required(VERSION 3.2)

include_directories(include)

enable_testing()

add_executable(literals
literals.cpp)

target_link_libraries(literals 
    parcellib)

target_link_libraries(literals
    gtest_main gmock_main)