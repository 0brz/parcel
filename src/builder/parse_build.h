#ifndef _PARSE_BUILD_H_
#define _PARSE_BUILD_H_

#include "../lang/lang.h"
#include "../lexems/lexem.h"
#include "../tools/lexer/lex.h"
#include "../tools/offset_table.h"
#include "../types/types.h"
#include "../expr/expr.h"
#include "../funcs/fn.h"
#include "lex_build.h"
#include "../parse/parser.h"
// #include "lex_build.h"
#include <vector>

using namespace std;
using namespace parcel::type;
using namespace parcel::lexems;
using namespace parcel::lang;
using namespace parcel::expr;
using namespace parcel::funcs;
using namespace parcel::parser;
using namespace parcel::build;

using ps_elem = parcel::parser::element;

namespace parcel
{
    namespace build
    {
        class instr
        {
            // for operate with data.
            std::vector<token_hook *> hooks;

            // prog entrypoints
            std::vector<prog_go *> entries{};

        public:
            void add_entry(prog_go *go)
            {
                entries.push_back(go);
            };
            // void add_hook(token_hook *hook);
            // void delete_hook(token_hook *hook);
            // void add_entry(element *root);

            void propagate(string &lex)
            {
                for (prog_go *e : entries)
                {
                    e->act(lex, NULL, NULL);
                }
            };
        };

        ps_elem *new_elem(lex_type type)
        {
            ps_elem *e = new ps_elem(type, NULL);
            return e;
        };

        // s

        bool is_prog_entrypoing(lex_type type)
        {
            return type == lex_type::GO;
        }

        // if list
        // list = new list(val=deep_build(next))

        bool check_lex_size(link_lex *lex, int need)
        {
            return lex->entries.size() >= need;
        };

        ps_elem *deep_build(link_lex *cur_lex, std::vector<ps_elem *> &builded)
        {
            if (cur_lex == NULL)
                return NULL;

            auto ttype = cur_lex->val->type;

            if (ttype == BL_LIST)
            {
                if (!check_lex_size(cur_lex, 1))
                {
                    printf("deep_build: <list> check_lex_size\n");
                    return NULL;
                }

                auto pick_next = cur_lex->entries.at(0);
                ps_elem *build_next = deep_build(pick_next, builded);
                if (build_next == NULL)
                {
                    printf("deep_build: <list> build_next returned 'null'\n");
                    return NULL;
                }

                builded.push_back(build_next);
                ps_elem *list = new ps_elem(lex_type::BL_LIST, new parser::list(build_next));
                builded.push_back(list);

                printf("deep_build: [ok] <list>\n");
                return list;
            }
            else if (ttype == BL_WORD)
            {
                ps_elem *word = new ps_elem(lex_type::BL_WORD, new parser::word());
                builded.push_back(word);
                printf("deep_build: [ok] <word>\n");
                return word;
            }
        }

        instr *
        build_instr(offset_table<link_lex> *lex)
        {
            printf("build::build_instr\n");
            instr *ins = new instr();

            std::vector<pair<int, std::vector<link_lex>>>
                levels = lex->as_list();
            if (levels.size() == 0 ||
                levels.at(0).second.size() == 0)
                return NULL;

            link_lex entry = levels.at(0).second.at(0);

            queue<link_lex> q;
            q.push(entry);

            std::vector<ps_elem *> builded;
            ps_elem *current = NULL;

            while (!q.empty())
            {
                auto cur = q.front();
                q.pop();
                auto ttype = cur->val->type;

                // 0.entrypoint
                if (is_prog_entrypoing(ttype))
                {
                    // build go
                    link_lex next;
                    if (cur.entries.size() == 0)
                    {
                        printf("build::instr: [ERR] <go> have no entries.\n");
                        // clear builded.
                        return NULL;
                    }

                    next = cur->entries.at(0);
                    ps_elem *build_next = deep_build(next, builded);

                    prog_go *g = new prog_go(build_next);
                    printf("build::instr: [OK] instr.add_entry(go)\n");
                    ins->add_entry(g);
                }
            }

            return ins;
        }
    }
}

#endif
