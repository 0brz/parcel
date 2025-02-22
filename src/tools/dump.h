#ifndef _DUMP_
#define _DUMP_

#include "../lexems/lexem.h"
#include "offset_table.h"
#include <iomanip>
#include <memory>
#include <queue>

using namespace parcel::lexems;

void dump_lex(offset_table<lex *> &table)
{
    vector<pair<int, vector<lex *>>> v = table.as_list();
    printf("Dump lex table. (count=%i)\n", v.size());

    for (const pair<int, vector<lex *>> &e : v)
    {
        for (lex *t : e.second)
        {
            cout << setw(e.first) << "[" << t->name() << "]\n";
        }
    }
};

#endif