#ifndef _PARSETREE_BUILD_H_
#define _PARSETREE_BUILD_H_

#include "../lang/lang.h"
#include "../lexems/lexem.h"
#include "../tools/lexer/lex.h"
#include "../tools/offset_table.h"
#include "../types/types.h"
#include "../expr/expr.h"
#include "../funcs/fn.h"
#include "lextree_build.h"
#include "../parse/elements.h"
// #include "lex_build.h"
#include <vector>

using namespace std;
using namespace parcel::type;
using namespace parcel::lexems;
using namespace parcel::lang;
using namespace parcel::expr;
using namespace parcel::funcs;
using namespace parcel::parser;
using namespace parcel::build;

// using ps_elem = parcel::parser::ParseElement;

namespace parcel
{
    namespace build
    {
        struct ParseTree
        {
            std::map<string, token_hook *> hooks;
            std::vector<prog_go *> roots;
            ParseTree() {};
        };

        ParseTree *build_parsetree(LexTree *lextree, shared_ptr<ParseCursor> &cursor);

    }
}

#endif