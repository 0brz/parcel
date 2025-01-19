#ifndef _BUILDER_
#define _BUILDER_

#include "parcel.h"
#include "graph_table.h"

using namespace std;
using namespace lex;

namespace build {
    bool is_function(lexer& lx);
    bool is_hook(lexer& lx);
    bool is_literal(lexer& lx);

    graph_block* build_function(lexer& lx);
    graph_block* build_hook(lexer& lx);
    graph_table<graph_block*> *build_lex_graph(string &src);

};

#endif