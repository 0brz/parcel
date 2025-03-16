cmake_minimum_required(VERSION 3.2)

project(bound_parsing)

include_directories(include)

enable_testing()
add_subdirectory(googletest)

add_executable(core
    single_values.cpp)

add_executable(client
    client.cpp)

target_link_libraries(core 
    parcellib)

target_link_libraries(client 
    parcellib)


target_link_libraries(core gtest_main gmock_main)