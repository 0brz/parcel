#ifndef _LEX_BUILD_H_
#define _LEX_BUILD_H_

#include "../lang/lang.h"
#include "../lexems/lexem.h"
#include "tools/lexer.h"
#include "../tools/offset_table.h"
#include "../types/types.h"
#include "../expr/expr.h"
#include "../funcs/fn.h"
#include "../tools/tree.h"
#include <queue>
#include "../tools/log.h"

using namespace std;
using namespace parcel::type;
using namespace parcel::lexems;
using namespace parcel::lang;
using namespace parcel::expr;
using namespace parcel::funcs;
using namespace parcel::tools;

namespace parcel
{
    namespace build
    {
        using LinkedLex = TreeValue<lex *>;

        struct LexTree
        {
            std::vector<LinkedLex *> roots{};
            LexTree(std::vector<LinkedLex *> &v) : roots(v) {};
            ~LexTree() {

                //parcel::tools::Log.Warning("~LexTree");

                queue<LinkedLex*> q;
                for (auto & v : roots) {
                    q.push(v);
                }

                while(!q.empty()) {
                    LinkedLex* cur = q.front();
                    q.pop();

                    for(auto & v : cur->entries) {
                        q.push(v);
                    }

                    delete cur->val;
                    delete cur;
                }

                //parcel::tools::Log.Warning("~LexTree end.");
            }
        };

        lex *inplace_build_tag(tools::Lexer &lx);
        lex *inplace_build_basetype(tools::Lexer &lx);
        lex *inplace_build_hook_ref(tools::Lexer &lx);
        lex *inplace_build_hook_def(tools::Lexer &lx);
        lex *inplace_build_link_def(tools::Lexer &lx);
        // lex *inplace_build_link_ref(lexer &lx);
        lex *inplace_build_literal(tools::Lexer &lx);
        lex *inplace_build_fn_expr(tools::Lexer &lx);
        lex *inplace_build_fn_ref(tools::Lexer &lx);

        LexTree *build_lextree(string &src);
    }
}

#endif