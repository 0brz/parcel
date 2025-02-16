cmake_minimum_required(VERSION 3.2)

project(bound_parsing)

include_directories(include)

add_executable(tt
    tests/client.cpp
    #src/tools/lex.h

    src/types/types.cpp
    src/types/types.h
    src/lang/lang.h
    src/lexems/lexem.h
)