cmake_minimum_required(VERSION 3.2)

project(bound_parsing)

include_directories(include)

add_executable(client client.cpp)

target_link_libraries(client 
    parcellib)

enable_testing()
add_subdirectory(googletest)

add_subdirectory(basetypes)

add_subdirectory(literals)

add_subdirectory(collections)