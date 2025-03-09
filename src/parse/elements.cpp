#include "elements.h"
#include "tokens.h"

using namespace parcel::tokens;
using namespace parcel::type;
using namespace parcel::parser;

void parcel::parser::deep_clear_tokens(parcel::tokens::token *root)
{
    queue<token *> q;
    q.push(root);

    printf("____[deep_clear_lists]\n");

    while (!q.empty())
    {
        token *t = q.front();
        q.pop();

        if (t->type == parcel::tokens::list)
        {
            val_list *ls = static_cast<val_list *>(t->val);
            printf("_____LS=%i\n", ls->v.size());
            for (auto tk : ls->v)
            {
                q.push(tk);
            }
            ls->v.clear();
            printf("_____LS2=%i\n", ls->v.size());
        }
        else if (t->type == parcel::tokens::set)
        {
            val_set *ls = static_cast<val_set *>(t->val);
            for (auto tk : ls->v)
            {
                q.push(tk);
            }
            ls->v.clear();
        }
        else if (t->type == parcel::tokens::vec)
        {
            val_vector *vec = static_cast<val_vector *>(t->val);
            for (auto tk : vec->v)
            {
                q.push(tk);
            }
            // vec->v.clear();
        }
    }

    if (root->type == parcel::tokens::list)
    {
    }
};

void parcel::parser::deep_reset_elems(ParseElement *root)
{
    queue<ParseElement *> q;
    q.push(root);

    while (!q.empty())
    {
        ParseElement *t = q.front();
        q.pop();

        // pay attention to collecitons.
        // they need be clear all represented values
        // tokens: deep_clear lists

        if (t->type == parcel::type::BL_SET)
        {
            parcel::parser::set *set = static_cast<parcel::parser::set *>(t->val);
            if (set != NULL)
            {
                for (auto de : set->els_row)
                {
                    q.push(de);
                }

                for (parcel::tokens::token *tk : set->tk_row)
                {
                    deep_clear_tokens(tk);
                }
            }
        }
        else if (t->type == parcel::type::BL_LIST)
        {
            parcel::parser::list *ls = static_cast<parcel::parser::list *>(t->val);
            if (ls != NULL)
            {
                q.push(ls->el);
                deep_clear_tokens(ls->el_t);
            }
        }
        /*
        else if (t->type == cond_type::type_ps_vec) {
            pr_vector* vec = static_cast<pr_vector*>(t->val);
            vec->reset();
        }
            */

        t->val->reset();
    }
};

bool parcel::parser::collection_empty(token *tk)
{
    if (tk->type == tokens::list)
    {
        val_list *ls = static_cast<val_list *>(tk->val);
        return ls->v.empty();
    }
    else if (tk->type == tokens::set)
    {
        val_set *ls = static_cast<val_set *>(tk->val);
        return ls->v.empty();
    }
    return true;
};

void parcel::parser::list_add(token *list, token *tk)
{
    val_list *v = static_cast<val_list *>(list->val);
    if (v != NULL)
    {
        v->v.push_back(tk);
    }
};

void parcel::parser::set_add(token *list, token *tk)
{
    val_set *v = static_cast<val_set *>(list->val);
    if (v != NULL)
    {
        v->v.push_back(tk);
    }
};

token *parcel::parser::create_tk_for(ParseElement *el)
{
    auto tk = new token();
    if (el->type == parcel::type::BL_LIST)
    {
        tk->type = tokens::list;
        tk->val = new val_list();
    }
    else if (el->type == parcel::type::BL_SET)
    {
        tk->type = tokens::set;
        tk->val = new val_set();
    }
    else if (el->type == parcel::type::BL_VEC)
    {
        tk->type = tokens::vec;
        // tk->val = new val_vector();
    }
    else if (el->type == parcel::type::BL_SEQ)
    {
        tk->type = tokens::seq;
        // tk->val = new val_vector();
    }

    return tk;
};

bool parcel::parser::is_id_char(char t)
{
    if (t >= '0' && t <= '9')
    {
        return true;
    }
    else if (isalpha(t))
        return true;
    else if (t == '_')
        return true;

    return false;
};