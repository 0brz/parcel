#ifndef _LANG_
#define _LANG_

#include "types/types.h"

#define LANG_EXPR_BR_OPEN '('
#define LANG_EXPR_BR_CLOSE ')'
#define LANG_LOGIC_AND '&'
#define LANG_LOGIC_OR '|'

#define LEX_SYMBOLS_NO_EXPR ":@!*^%$<>{}:\"'~;"
#define LEX_SYMBOLS_SPACES " \r\t\n"

using namespace parcel::type;

/*
    lang based logic.
*/

namespace parcel::lang
{
    bool basic_tag(string &str)
    {
        lex_type tp = typeof(str);
        switch (tp)
        {
        case BL_NUMBER:
        case BL_WORD:
        case BL_TAGVAL:
        case BL_TAGVAL_TAG:
        case BL_TAGVAL_VAL:
        case GO:
            return true;

        default:
            return false;
        }
    }
}

#endif