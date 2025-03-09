#ifndef _EXPR_
#define _EXPR_

#include "tools/lexer.h"
#include "../lang/lang.h"
#include <stack>

namespace parcel
{
    namespace expr
    {
        size_t get_logic_entry(tools::Lexer &lx);
        bool to_postfix(tools::Lexer &lx, stack<string> &call_stack);
    }
}

#endif