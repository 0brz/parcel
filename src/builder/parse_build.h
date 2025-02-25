#ifndef _PARSE_BUILD_H_
#define _PARSE_BUILD_H_

#include "../lang/lang.h"
#include "../lexems/lexem.h"
#include "../tools/lexer/lex.h"
#include "../tools/offset_table.h"
#include "../types/types.h"
#include "../expr/expr.h"
#include "../funcs/fn.h"
#include "lex_build.h"
#include "../parse/parser.h"
// #include "lex_build.h"

using namespace std;
using namespace parcel::type;
using namespace parcel::lexems;
using namespace parcel::lang;
using namespace parcel::expr;
using namespace parcel::funcs;
using namespace parcel::parser;
using namespace parcel::build;

namespace parcel
{
    namespace build
    {
        class instr
        {
            // for operate with data.
            std::vector<token_hook *> hooks;

            // prog entrypoints
            std::vector<element *> roots{};

        public:
            void add_hook(token_hook *hook);
            void delete_hook(token_hook *hook);
            void add_entry(element *root);

            instr(std::vector<element *> &roots) : roots(roots) {};
            instr(element *root)
            {
                roots.push_back(root);
            };

            void propagate(string &lex)
            {
                for (element *e : roots)
                {
                    if (e->type == lex_type::HOOK_DEF)
                    {
                        e->act(lex, NULL);
                    }
                }
            };
        };

        // s
        instr *build_instr(offset_table<link_lex *> *lex);
    }
}

#endif
