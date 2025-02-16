
#include "lang/lang.h"
#include "lexems/lexem.h"
#include "tools/lexer/lex.h"
#include "tools/offset_table.h"
#include "types/types.h"

using namespace std;
using namespace parcel::type;
using namespace parcel::lexems;
using namespace parcel::lang;

namespace parcel
{
    namespace builder
    {
        lex *implace_build_tag(lexer &lx);
        lex *implace_build_hook_ref(lexer &lx);
        lex *implace_build_hook_def(lexer &lx);
        lex *implace_build_link_def(lexer &lx);
        lex *implace_build_link_def(lexer &lx);
        lex *implace_build_literal(lexer &lx);
        lex *implace_build_fn_expr(lexer &lx);
        lex *implace_build_fn_ref(lexer &lx);

        offset_table<lex *> build_lex_table();

        // build_instr_table
    }
}
