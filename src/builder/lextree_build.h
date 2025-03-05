#ifndef _LEX_BUILD_H_
#define _LEX_BUILD_H_

#include "../lang/lang.h"
#include "../lexems/lexem.h"
#include "../tools/lexer/lex.h"
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

                parcel::tools::Log.Warning("~LexTree");

                queue<LinkedLex*> q;
                for (auto & v : roots) {
                    q.push(v);
                }

                while(!q.empty()) {
                    LinkedLex* cur = q.front();
                    q.pop();

                    printf("LEX=%s childs=%i\n", cur->val->name(), cur->entries.size());

                    for(auto & v : cur->entries) {
                        q.push(v);
                    }

                    delete cur->val;
                    delete cur;
                }

                parcel::tools::Log.Warning("~LexTree end.");
            }
        };

        lex *inplace_build_tag(lexer &lx);
        lex *inplace_build_basetype(lexer &lx);
        lex *inplace_build_hook_ref(lexer &lx);
        lex *inplace_build_hook_def(lexer &lx);
        lex *inplace_build_link_def(lexer &lx);
        // lex *inplace_build_link_ref(lexer &lx);
        lex *inplace_build_literal(lexer &lx);
        lex *inplace_build_fn_expr(lexer &lx);
        lex *inplace_build_fn_ref(lexer &lx);

        LexTree *build_lextree(string &src);
    }
}

#endif