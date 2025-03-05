#include "lang.h"

bool parcel::lang::is_basic_tag(string &str)
{
    lex_type tp = type::get_type(str);
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
};

bool parcel::lang::is_basetype(lex_type type)
{
    switch (type)
    {
    case BL_WORD:
    case BL_NUMBER:
    case BL_CHAR:
    case BL_ID:
        return true;

    default:
        return false;
    }
};

bool parcel::lang::is_instr_entrypoint(lex_type type)
{
    switch (type)
    {
    case GO:
    case LINK_DEF:
    case HOOK_DEF:
        return true;

    default:
        return false;
    }
};

bool parcel::lang::is_basetype(string &str)
{
    lex_type tp = type::get_type(str);
    return is_basetype(tp);
};

bool parcel::lang::is_collection(lex_type type)
{
    switch (type)
    {
    case type::lex_type::BL_SET:
    case type::lex_type::BL_LIST:
        return true;

    default:
        return false;
    }
};
