cmake_minimum_required(VERSION 3.2)

include_directories(include)

enable_testing()

add_executable(collections
collections.cpp)

target_link_libraries(collections 
    parcellib)

target_link_libraries(collections
    gtest_main gmock_main)