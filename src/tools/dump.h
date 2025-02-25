#ifndef _DUMP_
#define _DUMP_

#include "../lexems/lexem.h"
#include "../builder/lex_build.h"
#include "offset_table.h"
#include <iomanip>
#include <memory>
#include <queue>

using namespace parcel::lexems;
using namespace parcel::builder;

void dump_lex(offset_table<link_lex> &table)
{
    vector<pair<int, vector<link_lex>>> v = table.as_list();
    printf("Dump lex table. (count=%i)\n", v.size());

    for (const pair<int, vector<link_lex>> &e : v)
    {
        for (const link_lex &t : e.second)
        {
            cout << e.first << ":" << setw(e.first) << "[" << t.val->name() << "]\n";
        }
    }
};

#endif