#include "token.h"
using namespace parcel::tokens;

map<type, const char *> token_names{
    {word, "word"},
    {number, "number"},
    {CHAR, "char"},
    {ID, "id"},

    {list, "list"},
    {vec, "vec"},
    {seq, "seq"},
    {set, "set"},

    {literal_string, "litr.string"},
    {literal_char, "litr.char"},
    {literal_float, "litr.float"},
    {literal_int, "litr.int"}};

const char *parcel::tokens::nameof(type type)
{
    auto fnd = token_names.find(type);
    if (fnd != end(token_names))
    {
        return (*fnd).second;
    }
    else
        return "<null>";
};