cmake_minimum_required(VERSION 3.2)

project(bound_parsing)

include_directories(include)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fpermissive")

add_library(parcellib SHARED 
    #tests/cl2.cpp
    #src/tools/lex.h

    
    src/types/types.cpp
    src/types/types.h
    src/lang/lang.cpp
    src/lang/lang.h
    src/lexems/lexem.h
    src/expr/expr.cpp
    src/expr/expr.h
    src/funcs/fn.h
    
    src/tools/offset_table.h
    src/tools/lexer/lex.h
    src/tools/btree.h
    src/tools/tree.h
    src/tools/log.h
    src/tools/logger.h
    src/builder/lextree_build.cpp
    src/builder/lextree_build.h
    src/builder/parseinstr_build.cpp
    src/builder/parseinstr_build.h

    src/tools/ring.h
    src/parse/elements.cpp
    src/parse/elements.h
    src/parse/token.cpp
    src/parse/token.h

    src/tools/dump.h
    src/input/tokenizator.cpp
    src/input/tokenizator.h

    src/parcel.cpp
    src/parcelapi.cpp
)



add_executable(tt
    tests/cl2.cpp
)

target_link_libraries(tt 
    parcellib)
