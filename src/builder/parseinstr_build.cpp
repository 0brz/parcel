#include "parseinstr_build.h"
#include <stack>

using namespace parcel::build;
using namespace parcel::parser;

struct BuildTable
{
    std::vector<ParseElement *> all_builds;
    std::map<string, token_hook *> hooks;
    std::vector<prog_go *> entries;
    shared_ptr<ParseCursor> cursor;
    BuildTable(shared_ptr<ParseCursor> &cursor) : cursor(cursor), entries{}, hooks{}, all_builds{} {};
};

ParseElement *deep_build(LinkedLex *current, BuildTable &table);

typedef ParseElement *(*BuildElementImpl)(LinkedLex *, BuildTable &table);

namespace utils
{
    bool check_lex_size(LinkedLex *lex, int need)
    {
        return lex->entries.size() >= need;
    };
}

namespace collections
{
    ParseElement *build_list(LinkedLex *current, BuildTable &table)
    {
        if (!utils::check_lex_size(current, 1))
        {
            printf("deep_build: [ERR] <list> check_lex_size\n");
            return NULL;
        }

        auto pick_next = current->entries.at(0);
        ParseElement *build_next = deep_build(pick_next, table);
        if (build_next == NULL)
        {
            printf("deep_build: [ERR] <list> build_next returned 'null'\n");
            return NULL;
        }

        // table.all_builds.push_back(build_next);
        ParseElement *list = new ParseElement(lex_type::BL_LIST, new parcel::parser::list(build_next));
        table.all_builds.push_back(list);

        printf("deep_build: [ok] <list>\n");
        return list;
    };

    ParseElement *build_set(LinkedLex *current, BuildTable &table)
    {
        if (!utils::check_lex_size(current, 1))
        {
            printf("deep_build: [ERR] <set> check_lex_size\n");
            return NULL;
        }

        // set has many childs.
        std::vector<ParseElement *> set_values;
        for (LinkedLex *l : current->entries)
        {
            ParseElement *child_build = deep_build(l, table);
            if (child_build == NULL)
            {
                printf("deep_build: [ERR] <set> Error with build child element. lex=%s\n", l->val->name());
                return NULL;
            }

            set_values.push_back(child_build);
        }

        ParseElement *set = new ParseElement(lex_type::BL_SET, new parcel::parser::set(set_values));
        table.all_builds.push_back(set);

        printf("deep_build: [ok] <set(%i)>\n", set_values.size());
        return set;
    };
};

namespace sequences
{
    ParseElement *build_vec(LinkedLex *current, BuildTable &table)
    {
        if (!utils::check_lex_size(current, 1))
        {
            printf("deep_build: [ERR] <vec> check_lex_size\n");
            return NULL;
        }

        // BUILD CHILDS.
        // vec/seq has many childs.
        std::vector<ParseElement *> vec_values;
        for (LinkedLex *l : current->entries)
        {
            ParseElement *child_build = deep_build(l, table);
            if (child_build == NULL)
            {
                printf("deep_build: [ERR] <vec> Error with build child element. lex=%s\n", l->val->name());
                return NULL;
            }

            vec_values.push_back(child_build);
        }

        ParseElement *el = new ParseElement(lex_type::BL_VEC, new parcel::parser::vector(vec_values, table.cursor));

        table.all_builds.push_back(el);
        printf("deep_build: [ok] <vec(%i)>\n", vec_values.size());

        return el;
    };

    ParseElement *build_seq(LinkedLex *current, BuildTable &table)
    {
        if (!utils::check_lex_size(current, 1))
        {
            printf("deep_build: [ERR] <seq> check_lex_size\n");
            return NULL;
        }

        // BUILD CHILDS.
        // vec/seq has many childs.
        std::vector<ParseElement *> seq_values;
        for (LinkedLex *l : current->entries)
        {
            ParseElement *child_build = deep_build(l, table);
            if (child_build == NULL)
            {
                printf("deep_build: [ERR] <seq> Error with build child element. lex=%s\n", l->val->name());
                return NULL;
            }

            seq_values.push_back(child_build);
        }

        ParseElement *el = new ParseElement(lex_type::BL_SEQ, new parcel::parser::seq(seq_values, table.cursor));

        table.all_builds.push_back(el);
        printf("deep_build: [ok] <seq(%i)>\n", seq_values.size());

        return el;
    };
};

namespace entries
{
    token_hook *build_hook(LinkedLex *current, BuildTable &table)
    {
        LinkedLex *pick_child = current->entries.at(0);
        ParseElement *build_child = deep_build(pick_child, table);

        hook_def *def = static_cast<hook_def *>(current->val->value);
        if (def != NULL)
        {
            token_hook *tok_hook = new token_hook(def->name, build_child);

            return tok_hook;
        }
    };

    prog_go *build_go(LinkedLex *current, BuildTable &table)
    {
        if (!utils::check_lex_size(current, 1))
        {
            // log err
            return NULL;
        }

        LinkedLex *child_entry = current->entries.at(0);
        ParseElement *build_child = deep_build(child_entry, table);
        prog_go *go = new prog_go(build_child);

        return go;
    };
};

namespace refs
{
    ParseElement *build_hook_ref(LinkedLex *current, BuildTable &table)
    {
        hook_ref *v = static_cast<hook_ref *>(current->val->value);
        if (v != NULL)
        {
            // use hook with linking.
            auto find_hook = table.hooks.find(v->name);
            if (find_hook != end(table.hooks))
            {
                token_hook *hk = (*find_hook).second;
                ParseElement *he = new ParseElement(lex_type::HOOK_REF, hk);
                return he;
            }
        }

        return NULL;
    };
};

namespace basetypes
{
    ParseElement *build_word(LinkedLex *current, BuildTable &table)
    {
        return new ParseElement(BL_WORD, new parcel::parser::word());
    };

    ParseElement *build_id(LinkedLex *current, BuildTable &table)
    {
        return new ParseElement(BL_ID, new parcel::parser::id());
    };

    ParseElement *build_number(LinkedLex *current, BuildTable &table)
    {
        return new ParseElement(BL_NUMBER, new parcel::parser::num());
    };

    ParseElement *build_char(LinkedLex *current, BuildTable &table)
    {
        return new ParseElement(BL_CHAR, new parcel::parser::base_char());
    };
};

namespace literal
{
    ParseElement *build_char(LinkedLex *current, BuildTable &table)
    {
        value_litr_char *v = static_cast<value_litr_char *>(current->val->value);
        if (v == NULL)
        {
            printf("deep_build: [ERR] <litr.char> cant cast LEX-Value\n");
            return NULL;
        }

        ParseElement *el = new ParseElement(lex_type::LITR_CHAR, new parcel::parser::literal_char(v->value));
        table.all_builds.push_back(el);

        printf("deep_build: [ok] <litr.char)>\n");
        return el;
    };

    ParseElement *build_int(LinkedLex *current, BuildTable &table)
    {
        value_litr_int *v = static_cast<value_litr_int *>(current->val->value);
        if (v == NULL)
        {
            printf("deep_build: [ERR] <litr.int> cant cast LEX-Value\n");
            return NULL;
        }

        string sv = std::to_string(v->value);
        ParseElement *el = new ParseElement(lex_type::LITR_INT, new parcel::parser::literal_int(sv.c_str()));
        table.all_builds.push_back(el);

        printf("deep_build: [ok] <litr.int)>\n");
        return el;
    };

    ParseElement *build_float(LinkedLex *current, BuildTable &table)
    {
        value_litr_float *v = static_cast<value_litr_float *>(current->val->value);
        if (v == NULL)
        {
            printf("deep_build: [ERR] <litr.float> cant cast LEX-Value\n");
            return NULL;
        }

        ParseElement *el = new ParseElement(lex_type::LITR_FLOAT, new parcel::parser::literal_float(v->value));
        table.all_builds.push_back(el);

        printf("deep_build: [ok] <litr.float)>\n");
        return el;
    };

    ParseElement *build_string(LinkedLex *current, BuildTable &table)
    {
        value_litr_string *v = static_cast<value_litr_string *>(current->val->value);
        if (v == NULL)
        {
            printf("deep_build: [ERR] <litr.string> cant cast LEX-Value\n");
            return NULL;
        }

        ParseElement *el = new ParseElement(lex_type::LITR_STR, new parcel::parser::literal_string(v->value));
        table.all_builds.push_back(el);

        printf("deep_build: [ok] <litr.string)>\n");
        return el;
    };
};

map<lex_type, BuildElementImpl> build_impls{
    // BASE
    {BL_LIST, &(collections::build_list)},
    {BL_SET, &(collections::build_set)},
    {BL_VEC, &(sequences::build_vec)},
    {BL_SEQ, &(sequences::build_seq)},
    // REFS
    {HOOK_REF, &(refs::build_hook_ref)},
    // BT
    {BL_WORD, &(basetypes::build_word)},
    {BL_CHAR, &(basetypes::build_char)},
    {BL_NUMBER, &(basetypes::build_number)},
    {BL_ID, &(basetypes::build_id)},
    // LITS
    {LITR_CHAR, &(literal::build_char)},
    {LITR_FLOAT, &(literal::build_float)},
    {LITR_INT, &(literal::build_int)},
    {LITR_STR, &(literal::build_string)},
};

BuildElementImpl _find_build_impl(lex_type type)
{
    auto fn = build_impls.find(type);
    if (fn != end(build_impls))
        return (*fn).second;
    else
        return NULL;
};

ParseElement *deep_build(LinkedLex *current, BuildTable &table)
{
    if (current == NULL)
        return NULL;

    auto ttype = current->val->type;
    auto impl = _find_build_impl(ttype);
    if (impl == NULL)
    {
        // log
        return NULL;
    }

    ParseElement *el = impl(current, table);

    delete current->val;

    return el;
};

Instr *parcel::build::build_parseinstr(LexTree *lextree)
{
    BuildTable bt(make_shared<ParseCursor>(0, 5));

    Instr *tree = new Instr();
    std::vector<ParseElement *> all_builds;

    for (const auto &lex : lextree->roots)
    {
        if (!parcel::lang::is_instr_entrypoint(lex->val->type))
        {
            // log err.
            printf("UNRECOGNIZED ENTRYPOINT\n");
            return NULL;
        }

        if (lex->val->type == GO)
        {
            prog_go *g = entries::build_go(lex, bt);
            bt.entries.push_back(g);
            delete lex->val;
        }
        else if (lex->val->type == HOOK_DEF)
        {
            token_hook *hook = entries::build_hook(lex, bt);
            if (hook != NULL)
                bt.hooks[hook->name] = hook;

            delete lex->val;
        }
        else
        {
            printf("instr_builder: [ERR] unrecognized prog entry lex.\n");
        }
    }

    // copy from BT to tree.
    for (auto &e : bt.entries)
    {
        tree->roots.push_back(e);
    }

    for (auto &e : bt.hooks)
    {
        tree->hooks.insert(e);
    }

    return tree;
};