cmake_minimum_required(VERSION 3.2)

include_directories(include)

enable_testing()

add_executable(basetypes
    basetypes.cpp)

target_link_libraries(basetypes 
    parcellib)

target_link_libraries(basetypes
    gtest_main gmock_main)