#include "parsetree_build.h"
#include <stack>

using namespace parcel::build;
using namespace parcel::parser;

// PRIVATE

namespace utils
{
    bool check_lex_size(LinkedLex *lex, int need)
    {
        return lex->entries.size() >= need;
    };
}
parcel::parser::ParseElement *deep_build(LinkedLex *current, std::vector<parcel::parser::ParseElement *> &builded);

// deep_build

parcel::parser::ParseElement *create_basetype(LinkedLex *current)
{
    parcel::parser::ParseElement *cur = NULL;
    auto ttype = current->val->type;
    switch (ttype)
    {
    case BL_WORD:
        cur = new parcel::parser::ParseElement(ttype, new parcel::parser::word());
        break;

    case BL_ID:
        cur = new parcel::parser::ParseElement(ttype, new parcel::parser::id());
        break;

    case BL_NUMBER:
        cur = new parcel::parser::ParseElement(ttype, new parcel::parser::num());
        break;

    case BL_CHAR:
        cur = new parcel::parser::ParseElement(ttype, new parcel::parser::base_char());
        break;

    default:
        return NULL;
    }

    return cur;
}

namespace collections
{
    parcel::parser::ParseElement *build_list(LinkedLex *current, std::vector<parcel::parser::ParseElement *> &builded)
    {
        if (!utils::check_lex_size(current, 1))
        {
            printf("deep_build: [ERR] <list> check_lex_size\n");
            return NULL;
        }

        auto pick_next = current->entries.at(0);
        parcel::parser::ParseElement *build_next = deep_build(pick_next, builded);
        if (build_next == NULL)
        {
            printf("deep_build: [ERR] <list> build_next returned 'null'\n");
            return NULL;
        }

        builded.push_back(build_next);
        parcel::parser::ParseElement *list = new parcel::parser::ParseElement(lex_type::BL_LIST, new parcel::parser::list(build_next));
        builded.push_back(list);

        printf("deep_build: [ok] <list>\n");
        return list;
    };

}

parcel::parser::ParseElement *deep_build(LinkedLex *current, std::vector<parcel::parser::ParseElement *> &builded)
{
    if (current == NULL)
        return NULL;

    auto ttype = current->val->type;
    switch (ttype)
    {
    case BL_LIST:
        return collections::build_list(current, builded);

    default:
        return NULL;
    }
};

token_hook *build_hook(LinkedLex *current, std::vector<parcel::parser::ParseElement *> &builded)
{
    printf("build_hook\n");
    LinkedLex *pick_child = current->entries.at(0);
    parcel::parser::ParseElement *build_child = deep_build(pick_child, builded);

    hook_def *def = static_cast<hook_def *>(current->val->value);
    if (def != NULL)
    {
        token_hook *tok_hook = new token_hook(def->name, build_child);

        printf("build_hook [ok]\n");

        return tok_hook;
    }
};

prog_go *build_go(LinkedLex *current, std::vector<parcel::parser::ParseElement *> &builded)
{
    printf("build_go\n");

    if (!utils::check_lex_size(current, 1))
    {
        // log err
        return NULL;
    }

    LinkedLex *child_entry = current->entries.at(0);
    parcel::parser::ParseElement *build_child = deep_build(child_entry, builded);
    prog_go *go = new prog_go(build_child);

    printf("build_go [ok]\n");

    return go;
};

// IMPL
ParseTree *parcel::build::build_parsetree(LexTree *lextree)
{
    ParseTree *tree = new ParseTree();
    std::vector<parcel::parser::ParseElement *> all_builds;

    printf("build_parsetree; roots=%i\n", lextree->roots.size());

    for (const auto &lex : lextree->roots)
    {

        if (lex == NULL || lex->val == NULL)
        {
            printf("lex is NULL\n");
            return NULL;
        }

        if (!parcel::lang::is_instr_entrypoint(lex->val->type))
        {
            // log err.
            printf("UNRECOGNIZED ENTRYPOINT\n");
            return NULL;
        }

        if (lex->val->type == GO)
        {
            printf("BUILD GO \n");
            // prog_go *g = build_go(lex, all_builds);
            // tree->roots.push_back(g);
        }
        else if (lex->val->type == HOOK_DEF)
        {
            printf("BUILD HOOK\n");
            // token_hook *hook = build_hook(lex, all_builds);
            // if (hook != NULL)
            // tree->hooks[hook->name] = hook;
        }
        else
        {
            printf("instr_builder: [ERR] unrecognized prog entry lex.\n");
        }
    }

    printf("build_parsetree [ok]\n");

    return tree;
};