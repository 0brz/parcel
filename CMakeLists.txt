cmake_minimum_required(VERSION 3.2)

project(bound_parsing)

add_executable(tt
    src/tests/client.cpp
    src/tools/lex.h
    src/types/types.cpp
    src/types/types.h
)