#ifndef _PARSETREE_BUILD_H_
#define _PARSETREE_BUILD_H_

#include "../lang/lang.h"
#include "../lexems/lexem.h"
#include "tools/lexer.h"
#include "../tools/offset_table.h"
#include "../types/types.h"
#include "../expr/expr.h"
#include "../funcs/fn.h"
#include "lextree_build.h"
#include "../parse/elements.h"
// #include "lex_build.h"
#include <vector>
#include <memory>

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
        struct Instr
        {
            std::map<string, token_hook *> hooks;
            std::vector<prog_go *> roots;
            shared_ptr<ParseCursor> cursor;
            Instr() {};
            ~Instr() {
                // clear hooks
                for (auto & v : hooks) {
                    delete v.second;
                }

                // clear go
                for (auto & v : roots) {
                    delete v;
                }
            };

            void propagate(string& lex);
        };

        Instr *build_parseinstr(LexTree *lextree);
        
    }
}

#endif