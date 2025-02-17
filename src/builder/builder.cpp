#include "builder.h"
#include <stack>

using namespace parcel::lang;
using namespace parcel::type;
using namespace parcel::lexems;
using namespace parcel::expr;

namespace parcel::builder
{
    // fix
    lex *_new_lex(lex_type type, lvalue *val)
    {
        lex *bl = new lex(type, val);
        return bl;
    };

    lex *_new_hook_def(string &name)
    {
        hook_def *v = new hook_def(name);
        lex *b = new lex(lex_type::HOOK_DEF, v);
        return b;
    }

    bool _is_literal(lexer &lx)
    {
        // 123.123.123 (ip...)
        // 123.123 (float)
        // 123 (int)
        // "word" -> litr_word
        // 'a' -> litr_symbol
        // "a" -> litr_word

        size_t old = lx.cursor_get();

        char single_pref = '_';
        string cur;
        if (lx.next_float(cur) != lx.npos)
        { // 123.123
            // lx.cursor_move(-cur.size());
            lx.cursor_set(old);
            return true;
        }
        else if (lx.next_int(cur) != lx.npos)
        { // 123
            // lx.cursor_move(-cur.size());
            lx.cursor_set(old);
            return true;
        }
        else if (lx.next_like_rounded(cur, "\"", "\"", "") != lx.npos)
        { // "app"
            // lx.cursor_move(-cur.size());
            lx.cursor_set(old);
            return true;
        }
        else if (lx.next_like_rounded(cur, "'", "'", "") != lx.npos)
        { // 'g'
            // lx.cursor_move(-cur.size());
            if (cur.size() == 3)
            {
                // lx.cursor_move(-cur.size());
                lx.cursor_set(old);
                return true;
            }
        }

        lx.cursor_set(old);

        return false;
    };

    bool _is_tspace(char t)
    {
        string trash = "\r\t \n";
        return trash.find(t) != string::npos;
    };

    string _clear_expr_string(string &src)
    {
        stringstream ss;
        for (char t : src)
        {
            if (_is_tspace(t) || t == '\n')
                continue;
            else
                ss << t;
        }

        return ss.str();
    };

    bool _check_s(string &s, const char *contains)
    {
        if (s.find_first_of(contains) != string::npos)
        {
            return true;
        }

        return false;
    };

    /*
    ============================
    */

    lex *inplace_build_tag(lexer &lx)
    {
        string tagname;
        auto c = lx.cursor_get();

        if (lx.next_word(tagname) != lx.npos)
        {
            lx.skip(" \t");
            if (lx.at(lx.cursor_get()) == LANG_TAG_PREFIX)
            {
                lx.cursor_move(1);

                lex_type type = typeof(tagname);
                lex *_bl = _new_lex(type, NULL); // tagword doesnt have a value
                return _bl;
            }
        }

        lx.cursor_set(c);
        return NULL;
    };

    lex *inplace_build_hook_def(lexer &lx)
    {
        auto old = lx.cursor_get();
        if (lx.at(old) == LANG_PREFIX)
        {
            lx.cursor_move(1);
            string v;
            if (lx.next_id(v) != lx.npos)
            {
                lx.skip(" \r\t");
                if (lx.at(lx.cursor_get()) == LANG_TAG_PREFIX)
                {
                    lex *bl = _new_hook_def(v);
                    return bl;
                }
            }
        }

        lx.cursor_set(old);
        return NULL;
    };

    lex *inplace_build_link_def(lexer &lx)
    {
        auto old = lx.cursor_get();
        if (lx.at(old) == LANG_LINK_PREFIX)
        {
            lx.cursor_move(1);
            string v;
            if (lx.next_id(v) != lx.npos)
            {
                lx.skip(" \r\t");
                if (lx.at(lx.cursor_get()) == LANG_TAG_PREFIX)
                {
                    lex *bl = _new_lex(lex_type::LINK_DEF, new link_def(v));
                    return bl;
                }
            }
        }

        lx.cursor_set(old);
        return NULL;
    };

    lex *inplace_build_literal(lexer &lx)
    {
        if (!_is_literal(lx))
            return NULL;

        size_t old = lx.cursor_get();

        string cur;
        if (lx.next_float(cur) != lx.npos)
        {
            float _val = stof(cur.c_str());
            auto _bl = _new_lex(lex_type::LITR_FLOAT, new value_litr_float(_val));
            return _bl;
            //_link_last_block(gt, _bl);
            // printf("~%zi [gt(link.last)] (float) %s -> %s\n", line_offset, _last_name.c_str(), cur.c_str());
        }
        else if (lx.next_int(cur) != lx.npos)
        {
            int _val = stoi(cur.c_str());
            auto _bl = _new_lex(lex_type::LITR_INT, new value_litr_int(_val));
            return _bl;
            //_link_last_block(gt, _bl);
            // printf("~%zi [gt(link.last)] (int) %s -> %s\n", line_offset, _last_name.c_str(), cur.c_str());
        }
        else if (lx.next_like_rounded(cur, "\"", "\"", "") != lx.npos)
        {
            string _val = string(cur.c_str());
            auto _bl = _new_lex(lex_type::LITR_STR, new value_litr_string(_val));
            return _bl;
            //_link_last_block(gt, _bl);
            // printf("~%zi [gt(link.last)] (str) %s -> %s\n", line_offset, _last_name.c_str(), cur.c_str());
        }
        else if (lx.next_like_rounded(cur, "'", "'", "") != lx.npos)
        {
            char _val = cur[1]; // we can take, because size of char view=3, like 'a', 'b'
            auto _bl = _new_lex(lex_type::LITR_CHAR, new value_litr_char(_val));
            return _bl;
            //_link_last_block(gt, _bl);
            // printf("~%zi [gt(link.last)] (char) %s -> %s\n", line_offset, _last_name.c_str(), cur.c_str());
        }
        else
        {
            lx.cursor_set(old);
            return NULL;
        }
    };

    lex *inplace_build_fn_expr(lexer &lx)
    {
        string expr_s;
        auto old = lx.cursor_get();

        lx.next_until("\n\r", expr_s);
        lx.cursor_set(old);
        auto next_cursor = old += expr_s.size();

        if (_check_s(expr_s, LEX_SYMBOLS_NO_EXPR))
            return NULL;

        expr_s.insert(0, "(");
        expr_s.append(")");

        string normalized_expr_buff = _clear_expr_string(expr_s);
        lexer lx2(normalized_expr_buff);

        stack<string> expr_postfix;
        if (!expr::to_postfix(lx2, expr_postfix))
        {
            return NULL;
        }

        // DEBUG_MSG("[try_build_fn_expr] deep_expr_postfix ok");

        if (expr_postfix.size() <= 1)
        {
            lx.cursor_set(old);
            return NULL;
        }

        fn_btree_refs *tr = new fn_btree_refs();
        if (!try_build_fn_tree(expr_postfix, tr))
        {
            DEBUG_MSG("[try_build_fn_expr] Cant build fn expr tree");
            return NULL;
        }

        lx.cursor_set(next_cursor);

        value_fn_expr_refs *expr_val = new value_fn_expr_refs(tr);

        return expr_val;
    }

    // fn_ref

}