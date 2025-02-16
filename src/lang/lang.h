#ifndef _LANG_
#define _LANG_

#include "types/types.h"

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