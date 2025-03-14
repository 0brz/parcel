#ifndef _DUMP_
#define _DUMP_

#include "../lexems/lexem.h"
#include "../builder/lextree_build.h"
#include "../parse/elements.h"
#include "dump.h"
#include <tokens.h>
#include "offset_table.h"
#include <iomanip>
#include <memory>
#include <queue>

using namespace parcel::lexems;
using namespace parcel::build;


void parcel::dump::hook(parcel::tokens::token *h) {
    queue<pair<token *, short>> q;
    q.push({h, 0});

    printf("Dump hook '%s':\n", nameof(h->type));

    while (!q.empty())
    {
        auto cur = q.front();

        if (cur.first->val != NULL)
        {
            cout << setw(cur.second) << "[" << parcel::tokens::nameof(cur.first->type) << "]" << " v='" << cur.first->val->str() << "'\n";
        }
        else
        {
            printf("(%s) Not Builded!\n",  parcel::tokens::nameof(cur.first->type));
            // break;
        }

        if (cur.first->type == parcel::tokens::list)
        {
            val_list *ls = static_cast<val_list *>(cur.first->val);
            if (ls != NULL)
            {
                printf("dump.list: count=%i\n", ls->v.size());
                // cout << setw(cur.second) << "[List] (";
                for (token *n : ls->v)
                {
                    // cout <<tk_nameof(cur.first->type) << ",";
                    q.push({n, cur.second + 3});
                }
            }
        }
        else if (cur.first->type == parcel::tokens::set)
        {
            val_set *ls = static_cast<val_set *>(cur.first->val);
            if (ls != NULL)
            {
                // cout << setw(cur.second) << "[Set] (";
                for (token *n : ls->v)
                {
                    // cout <<tk_nameof(cur.first->type) << ",";
                    q.push({n, cur.second + 3});
                }
                // cout << ")\n";
            }
        }
        else if (cur.first->type == parcel::tokens::vec)
        {
            val_vector *ls = static_cast<val_vector *>(cur.first->val);
            if (ls != NULL)
            {
                // cout << setw(cur.second) << "[Vec] (";
                for (token *n : ls->v)
                {
                    // cout <<tk_nameof(cur.first->type) << ",";
                    q.push({n, cur.second + 3});
                }
                // cout << ")\n";
            }
        }
        else if (cur.first->type == parcel::tokens::seq)
        {
            val_seq *ls = static_cast<val_seq *>(cur.first->val);
            if (ls != NULL)
            {
                // cout << setw(cur.second) << "[Vec] (";
                for (token *n : ls->v)
                {
                    // cout <<tk_nameof(cur.first->type) << ",";
                    q.push({n, cur.second + 3});
                }
                // cout << ")\n";
            }
        }

        q.pop();
    }
}

namespace tools
{
    namespace dump2
    {
        /*
        void lextable(offset_table<LinkedLex *> *table)
        {
            std::vector<pair<int, std::vector<LinkedLex *>>> v = table->as_list();
            printf("Dump lex table. (count=%i)\n", v.size());

            for (const pair<int, std::vector<LinkedLex *>> &e : v)
            {
                for (const LinkedLex *t : e.second)
                {
                    cout << e.first << ":" << setw(e.first) << "[" << t->val->name() << "]\n";
                }
            }
        };
        */

    }
}

#endif