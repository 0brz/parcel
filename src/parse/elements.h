#ifndef _PARSE_PARSER_H_
#define _PARSE_PARSER_H_

#include <map>
#include <string>
#include <vector>
#include "tokens.h"
#include <queue>
#include "../types/types.h"
#include "../lang/lang.h"
#include "../tools/ring.h"
#include "../tools/log.h"
#include <memory>

using namespace std;
using namespace parcel::tokens;
using namespace parcel::type;
using namespace parcel::lang;

namespace parcel
{
    namespace parser
    {
        class ParseCursor
        {
        public:
            size_t pos;
            size_t size;
            ParseCursor(size_t pos, size_t sz) : pos(pos), size(sz) {};
            bool at_end() { return pos == size - 1; };
        };

        enum act_result
        {
            MOD,
            ACT,
            FAIL,
            ERR_TOKEN_TYPE,
        };

        struct ParseValue
        {
            // fn info
            virtual act_result act(string &lex, token *par, token *t2 = NULL) = 0;
            virtual void reset() = 0;
            virtual ~ParseValue() {};
        };

        struct ParseElement
        {
            lex_type type;
            ParseValue *val;

            ParseElement(lex_type type, ParseValue *v) : type(type), val(v) {};
            void reset() {};

            act_result act(string &lex, token *par, token *t2 = NULL)
            {
                if (val != NULL)
                {
                    return val->act(lex, par, t2);
                }

                return FAIL;
            };

            ~ParseElement() {
                parcel::tools::Log.Link("~ParseElement");
                delete val;
            }
        };

        // ------------- TOOLS
        void list_add(token *list, token *tk);

        void set_add(token *list, token *tk);

        token *create_tk_for(ParseElement *el);

        // ------------- UTILS.
        void deep_clear_tokens(parcel::tokens::token *root);

        void deep_reset_elems(ParseElement *root);

        bool collection_empty(token *tk);

        bool is_id_char(char t);

        // ------------- PARSING.BASETYPES

        struct word : ParseValue
        {
            void reset() {};
            act_result act(string &lex, token *par, token *t2 = NULL)
            {
                // printf("pr_word\n");
                if (!isalpha(lex[0]))
                    return FAIL;

                par->type = tokens::type::word;
                par->val = new val_word(lex);
                return ACT;
            }
        };

        struct num : ParseValue
        {
            void reset() {};
            act_result act(string &lex, token *par, token *t2 = NULL)
            {
                // printf("pr_num\n");
                if (lex[0] >= '0' && lex[0] <= '9')
                {
                    par->type = tokens::type::number;
                    par->val = new val_num(lex);
                    return ACT;
                }

                // int v = stoi(lex);
                return FAIL;
            }
        };

        struct base_char : ParseValue
        {
            void reset() {};
            act_result act(string &lex, token *par, token *t2 = NULL)
            {
                // printf("pr_num\n");
                if (lex.size() == 1)
                {
                    par->type = tokens::type::CHAR;
                    par->val = new val_char(lex.at(0));
                    return ACT;
                }

                // int v = stoi(lex);
                return FAIL;
            }
        };

        struct id : ParseValue
        {
            void reset() {};
            act_result act(string &lex, token *par, token *t2 = NULL)
            {
                // printf("pr_num\n");
                if (lex.size() > 1)
                {
                    bool success = true;
                    for (int i = 0; i < lex.size() - 1; i++)
                    {
                        if (is_id_char(lex[i]) && is_id_char(lex[i + 1]))
                        {
                        }
                        else
                        {
                            success = false;
                            break;
                        };
                    }

                    if (success)
                    {
                        par->type = tokens::type::ID;
                        par->val = new val_id(lex);
                        return ACT;
                    }
                }

                // int v = stoi(lex);
                return FAIL;
            }
        };

        // ------------- TOOLS

        // ------------- PARSING.LITERALS

        struct literal_string : ParseValue
        {
            string val;
            int _cursor = 0;
            size_t _val_size;
            string _trash_skip;

            void reset() {};

            bool _check(string& lex) {
                return lex == val;
            }

            act_result act(string &lex, token *par, token *t2 = NULL)
            {
                //printf("STRING COMP=%s %s\n", lex.c_str(), val.c_str());
                

                if (_check(lex))
                {
                    _cursor = 0;

                    if (par != NULL)
                    {
                        par->type = tokens::type::literal_string;
                        par->val = new val_string(lex);
                        return ACT;
                    }

                    return ACT;
                }
                else
                    return FAIL;
            }

            literal_string(const char *t) : val(t) {};
            literal_string(string &t) : val(t), _trash_skip(LEX_SYMBOLS_SPACES), _val_size(t.size()) {};
        };

        struct literal_float : ParseValue
        {
            float val;

            void reset() {};

            act_result act(string &lex, token *par, token *t2 = NULL)
            {
                try
                {
                    float fv = stof(lex);
                    if (!lex.empty() && val == fv)
                    {
                        if (par != NULL)
                        {
                            float v = stof(lex);
                            par->type = tokens::type::literal_float;
                            par->val = new val_float(v);
                            return ACT;
                        }

                        return ACT;
                    }
                }
                catch (invalid_argument ex)
                {
                    return FAIL;
                }

                return FAIL;
            }

            literal_float(float &v) : val(v) {};
        };

        struct literal_int : ParseValue
        {
            string val;

            void reset() {};

            act_result act(string &lex, token *par, token *t2 = NULL)
            {
                // compare like two strings
                if (!lex.empty() && lex == val)
                {
                    if (par != NULL)
                    {
                        int v = stoi(lex);
                        par->type = tokens::type::literal_int;
                        par->val = new val_int(v);
                        return ACT;
                    }

                    return ACT;
                }
                else
                    return FAIL;
            }

            literal_int(const char *t) : val(t) {};
        };

        struct literal_char : ParseValue
        {
            char val;

            void reset() {};

            act_result act(string &lex, token *par, token *t2 = NULL)
            {
                if (lex.size() == 1 && lex.at(0) == val)
                {
                    if (par != NULL)
                    {
                        par->type = tokens::type::literal_char;
                        par->val = new val_char(val);
                        return ACT;
                    }

                    return ACT;
                }
                else
                    return FAIL;
            }

            literal_char(char t) : val(t) {};
        };

        // ------------- PARSING.COLLECTIONS

        struct list : ParseValue
        {
            ParseElement *el;
            token *el_t;  // storing the deep stucture
            token *el_t2; // t2 param

            void reset() {
                // el->reset();
            };

            list(ParseElement *basetype)
            {
                el = basetype;
                el_t = create_tk_for(el);
                el_t2 = create_tk_for(el);
            }

            ~list() {
                parcel::tools::Log.Link("~token::list");
                delete el;
                delete el_t;
                delete el_t2;
            }

            act_result act(string &lex, token *par, token *t2 = NULL)
            { // par=list

                auto res = el->act(lex, el_t, el_t2);

                if (res == ACT)
                {

                    if (el_t2->val != NULL)
                    {
                        //printf("__[list]: ACT (r2) s=%s\n", lex.c_str());
                        list_add(par, el_t2);
                        el_t2 = create_tk_for(el);
                        // fix: recreate toke
                    }
                    else
                    {
                        printf("__[list]: ACT s=%s\n", lex.c_str());
                    }

                    if (par->val != NULL)
                    {
                        // printf("list_add_1\n");
                        list_add(par, el_t);
                        // printf("list_add\n");
                        el_t = create_tk_for(el);
                    }
                }
                else if (res == MOD)
                {
                    //printf("__[list]: mod s=%s\n", lex.c_str());
                    // keep building...
                }
                else if (res == FAIL)
                {
                    //printf("__[list]: fail s=%s\n", lex.c_str());
                    // clear token?
                }

                return res;
            };
        };

        struct set : ParseValue
        {
            // for each pair el-token
            std::vector<ParseElement *> els_row;
            std::vector<token *> tk_row; // storing the deep stucture

            void reset() {};

            set(std::vector<ParseElement *> &els)
            {
                els_row = els;
                for (ParseElement *e : els_row)
                {
                    tk_row.push_back(create_tk_for(e));
                }
            }

            act_result act(string &lex, token *par, token *t2 = NULL)
            { // par=set
                // printf("pr_list\n");

                bool is_act = false;
                bool is_mod = false;

                for (int i = 0; i < els_row.size() && i < tk_row.size(); i++)
                {
                    auto res = els_row[i]->act(lex, tk_row[i]);

                    if (res == ACT)
                    {
                        set_add(par, tk_row[i]);
                        // recreate old token store
                        tk_row[i] = create_tk_for(els_row[i]);
                        is_act = true;
                    }
                    else if (res == MOD)
                    {
                        // keep building...
                        is_mod = true;
                    }
                    else if (res == FAIL)
                    {
                        // clear token?
                    }
                }

                if (is_act)
                    return ACT;
                if (is_mod)
                    return MOD;
            };
        };

        struct vector : ParseValue
        {
            ring<pair<ParseElement *, token *> *> el_ring;
            shared_ptr<ParseCursor> lex_cursor;

            void reset()
            {
                // reset deep refs
                el_ring.to_begin();

                auto row = el_ring.as_row();
                for (auto &e : row)
                {
                    e->first->reset();
                    // deep_reset_elems(e->first);
                    deep_clear_tokens(e->second);
                }
            };

            void build_out(token *par)
            {
                std::vector<token *> builded_row;
                auto elems = el_ring.as_row();
                for (auto &e : elems)
                {
                    builded_row.push_back(e->second);
                    e->second = create_tk_for(e->first);
                }

                val_vector *vec = new val_vector(builded_row);
                par->type = tokens::vec;
                par->val = vec;
            }

            vector(std::vector<ParseElement *> &v, shared_ptr<ParseCursor> cursor)
            {
                lex_cursor = cursor;

                for (ParseElement *e : v)
                {
                    pair<ParseElement *, token *> *pr = new pair<ParseElement *, token *>{e, create_tk_for(e)};
                    el_ring.add_end(pr);
                }

                el_ring.to_begin();
                printf("vec.ring.builded=%i\n", el_ring.len());
            };

            bool is_ring_ending()
            {
                if (el_ring.pos() == el_ring.len() - 1)
                {
                    return true;
                }
                else
                    return false;
            };

            act_result act(string &lex, token *par, token *t2 = NULL)
            { // vec

                pair<ParseElement *, token *> *cur_pair = el_ring.get_current();
                act_result res = cur_pair->first->act(lex, cur_pair->second);

                printf("-------------[vec].act() s=%s ring=%s\n", lex.c_str(), nameof(cur_pair->first->type));

                bool prev_ending = is_ring_ending();

                if (res == ACT)
                {
                    if (is_ring_ending())
                    { // finish build vec.
                        bool is_coll = parcel::lang::is_collection(cur_pair->first->type);
                        // printf("[vec].act() END.ACT s=%s is_coll=%i\n", lex.c_str(), is_coll);

                        //
                        if (is_ring_ending() && is_collection(cur_pair->first->type) && (lex_cursor.get()->at_end()))
                        {
                            printf("[vec].act() (ACT) -> ACT  list,ring_end,curs_end s='%s' ring=%i\n", lex.c_str(), el_ring.pos());
                            // ring end
                            // curs end
                            // last coll
                            build_out(par);
                            el_ring.to_begin();

                            return ACT;
                        }

                        // end.list (cont rule)
                        if (is_collection(cur_pair->first->type))
                        {
                            printf("[vec].act() (ACT) -> MOD  list,ring_end s='%s' ring=%i\n", lex.c_str(), el_ring.pos());
                            return MOD;
                        }
                        // end.t
                        else
                        {
                            printf("[vec].act() (ACT) -> ACT  T,ring_end s='%s' ring=%i\n", lex.c_str(), el_ring.pos());

                            build_out(par);
                            el_ring.to_begin();

                            return ACT;
                        }
                    }
                    else
                    {
                        // printf("[vec].act() MID.ACT s=%s\n", lex.c_str());

                        // mid.list
                        if (is_collection(cur_pair->first->type))
                        {
                            printf("[vec].act() (ACT) -> MOD mid.list s='%s' ring=%i\n", lex.c_str(), el_ring.pos());
                            return MOD;
                        }
                        // mid.t
                        else
                        {
                            printf("[vec].act() (ACT) -> MOD  mid.T s='%s' ring=%i\n", lex.c_str(), el_ring.pos());
                            el_ring.to_next();
                            return MOD;
                        }
                    }
                }
                else if (res == MOD)
                {
                    // end.mod
                    // mid.mod
                    // curs.end
                    // printf("LAST_ENDING curs=%i\n", lex_cursor.get()->pos);
                    if (is_ring_ending() && is_collection(cur_pair->first->type) && (lex_cursor.get()->at_end()))
                    {

                        printf("[vec].act() (MOD) -> ACT curs_end s='%s' ring=%i\n", lex.c_str(), el_ring.pos());
                        // ring end
                        // curs end
                        // last coll
                        build_out(par);
                        el_ring.to_begin();

                        return ACT;
                    }

                    printf("[vec].act() (MOD) -> MOD s='%s' ring=%i\n", lex.c_str(), el_ring.pos());
                    return MOD;
                }
                else if (res == FAIL)
                {

                    // list take next rule.
                    if (is_collection(cur_pair->first->type) &&
                        !collection_empty(cur_pair->second))
                    {
                        // try pick next.
                        // end/mid/beg
                        el_ring.to_next();
                        auto next_pair = el_ring.get_current();
                        // use temp token to not change current state.
                        // FIX: we need deep CLEARED head fr this vector
                        token *temp_token = create_tk_for(next_pair->first);
                        act_result next_res = next_pair->first->act(lex, temp_token);

                        // ring_end, curs_end, next_res, next_type
                        /*
                            coll, t, coll
                            coll, t
                        */
                        if (next_res == ACT)
                        {
                            // n.act.end
                            // n.end, t
                            // n.end, list (curs_end)
                            if (is_ring_ending() && is_collection(next_pair->first->type))
                            {
                                if (lex_cursor.get()->at_end())
                                {
                                    printf("+++++++++ s=%s [FAIL, ACT, coll, ring-end, curs-end] -> ACT %s\n", lex.c_str(), nameof(cur_pair->first->type));
                                    next_pair->second = temp_token;
                                    build_out(par);
                                    el_ring.to_begin();
                                    return ACT;
                                }
                                else
                                {
                                    printf("+++++++++ s=%s [FAIL, ACT, coll, ring-end] -> MOD %s\n", lex.c_str(), nameof(cur_pair->first->type));
                                    next_pair->second = temp_token;
                                    // its not curs_end, keep building last list.
                                    return MOD;
                                }
                            }
                            else if (is_ring_ending() && !is_collection(next_pair->first->type))
                            {
                                // on last elem. T, Build
                                printf("+++++++++ s=%s [FAIL, ACT, !coll, ring-end] -> ACT %s\n", lex.c_str(), nameof(cur_pair->first->type));
                                next_pair->second = temp_token;
                                build_out(par);
                                el_ring.to_begin();
                                return ACT;
                            }
                            else if (prev_ending)
                            {
                                printf("+++++++++ s=%s [FAIL, ACT, coll, prev-end] -> ACT %s\n", lex.c_str(), nameof(cur_pair->first->type));
                                build_out(par);
                                next_pair->second = temp_token;
                                if (!is_collection(next_pair->first->type))
                                    el_ring.to_next();

                                return ACT;
                            }
                            else
                            {
                                // activate first elem, last list
                                printf("+++++++++ s=%s [FAIL, ACT, any] -> MOD %s\n", lex.c_str(), nameof(cur_pair->first->type));
                                next_pair->second = temp_token;
                                // step on next in the mid
                                return MOD;
                            }
                        }
                        else if (next_res == MOD)
                        {
                            // begin build first|next block
                            // go to first from last list.
                            // TEMMP ONLY THERE!
                            if (prev_ending)
                            {
                                build_out(par);
                                next_pair->second = temp_token;
                                return ACT;
                            }
                            else
                            {
                                next_pair->second = temp_token;
                                // curs_end,...
                                return MOD;
                            }
                        }
                        else if (next_res == FAIL)
                        {
                            this->reset();
                            return FAIL;
                        }
                    }

                    printf("[vec].act() (FAIL) -> FAIL <reset> s='%s' ring=%i\n", lex.c_str(), el_ring.pos());
                    this->reset();
                    // other elem
                    return FAIL;
                }

                printf("[vec].act() (UNDEF CASE) -> FAIL  s='%s' ring=%i\n", lex.c_str(), el_ring.pos());
                return FAIL;
            };
        };

        struct seq : ParseValue
        {
            ring<pair<ParseElement *, token *> *> el_ring;
            shared_ptr<ParseCursor> lex_cursor;

            void reset()
            {
                // reset deep refs
                el_ring.to_begin();

                auto row = el_ring.as_row();
                for (auto &e : row)
                {
                    e->first->reset();
                    // deep_reset_elems(e->first);
                    deep_clear_tokens(e->second);
                }
            };

            void build_out(token *par)
            {
                std::vector<token *> builded_row;
                auto elems = el_ring.as_row();
                for (auto &e : elems)
                {
                    builded_row.push_back(e->second);
                    e->second = create_tk_for(e->first);
                }

                val_seq *vec = new val_seq(builded_row);
                par->type = tokens::seq;
                par->val = vec;
            }

            seq(std::vector<ParseElement *> &v, shared_ptr<ParseCursor> cursor)
            {
                lex_cursor = cursor;

                for (ParseElement *e : v)
                {
                    pair<ParseElement *, token *> *pr = new pair<ParseElement *, token *>{e, create_tk_for(e)};
                    el_ring.add_end(pr);
                }

                el_ring.to_begin();
                //printf("vec.ring.builded=%i\n", el_ring.len());
            };

            bool is_ring_ending()
            {
                if (el_ring.pos() == el_ring.len() - 1)
                {
                    return true;
                }
                else
                    return false;
            }

            ~seq() {
                auto ls = el_ring.as_row();
                for (auto & v : ls) {
                    delete v->first;
                    delete v->second;
                }
            }

            act_result act(string &lex, token *par, token *t2 = NULL)
            { // vec

                pair<ParseElement *, token *> *cur_pair = el_ring.get_current();
                act_result res = cur_pair->first->act(lex, cur_pair->second);

                //printf("-------------[seq].act() s=%s ring=%s\n", lex.c_str(), nameof(cur_pair->first->type));

                bool prev_ending = is_ring_ending();

                if (res == ACT)
                {
                    if (is_ring_ending())
                    { // finish build vec.

                        if (is_ring_ending() && is_collection(cur_pair->first->type) && (lex_cursor.get()->at_end()))
                        {
                            //printf("[seq].act() (ACT) -> ACT  list,ring_end,curs_end s='%s' ring=%i\n", lex.c_str(), el_ring.pos());
                            // ring end
                            // curs end
                            // last coll
                            build_out(par);
                            el_ring.to_begin();

                            return ACT;
                        }

                        // end.list (cont rule)
                        if (is_collection(cur_pair->first->type))
                        {
                            //printf("[seq].act() (ACT) -> MOD  list,ring_end s='%s' ring=%i\n", lex.c_str(), el_ring.pos());
                            return MOD;
                        }
                        // end.t
                        else
                        {
                            //printf("[seq].act() (ACT) -> ACT  T,ring_end s='%s' ring=%i\n", lex.c_str(), el_ring.pos());

                            build_out(par);
                            el_ring.to_begin();

                            return ACT;
                        }
                    }
                    else
                    {
                        // printf("[vec].act() MID.ACT s=%s\n", lex.c_str());

                        // mid.list
                        if (is_collection(cur_pair->first->type))
                        {
                            //printf("[seq].act() (ACT) -> MOD mid.list s='%s' ring=%i\n", lex.c_str(), el_ring.pos());
                            return MOD;
                        }
                        // mid.t
                        else
                        {
                            //printf("[seq].act() (ACT) -> MOD  mid.T s='%s' ring=%i\n", lex.c_str(), el_ring.pos());
                            el_ring.to_next();
                            return MOD;
                        }
                    }
                }
                else if (res == MOD)
                {
                    // end.mod
                    // mid.mod
                    // curs.end
                    if (is_ring_ending() && is_collection(cur_pair->first->type) && (lex_cursor.get()->at_end()))
                    {

                        //printf("[seq].act() (MOD) -> ACT curs_end s='%s' ring=%i\n", lex.c_str(), el_ring.pos());
                        // ring end
                        // curs end
                        // last coll
                        build_out(par);
                        el_ring.to_begin();

                        return ACT;
                    }

                    //printf("[seq].act() (MOD) -> MOD s='%s' ring=%i\n", lex.c_str(), el_ring.pos());
                    return MOD;
                }
                else if (res == FAIL)
                {

                    // list take next rule.
                    if (is_collection(cur_pair->first->type) &&
                        !collection_empty(cur_pair->second))
                    {
                        // try pick next.
                        // end/mid/beg
                        el_ring.to_next();
                        auto next_pair = el_ring.get_current();
                        // use temp token to not change current state.
                        // FIX: we need deep CLEARED head fr this vector
                        token *temp_token = create_tk_for(next_pair->first);
                        act_result next_res = next_pair->first->act(lex, temp_token);

                        // ring_end, curs_end, next_res, next_type
                        /*
                            coll, t, coll
                            coll, t
                        */
                        if (next_res == ACT)
                        {
                            // n.act.end
                            // n.end, t
                            // n.end, list (curs_end)
                            if (is_ring_ending() && is_collection(next_pair->first->type))
                            {
                                if (lex_cursor.get()->at_end())
                                {
                                    //printf("+++++++++ s=%s [FAIL, ACT, coll, ring-end, curs-end] -> ACT %s\n", lex.c_str(), nameof(cur_pair->first->type));
                                    next_pair->second = temp_token;
                                    build_out(par);
                                    el_ring.to_begin();
                                    return ACT;
                                }
                                else
                                {
                                    //printf("+++++++++ s=%s [FAIL, ACT, coll, ring-end] -> MOD %s\n", lex.c_str(), nameof(cur_pair->first->type));
                                    next_pair->second = temp_token;
                                    // its not curs_end, keep building last list.
                                    return MOD;
                                }
                            }
                            else if (is_ring_ending() && !is_collection(next_pair->first->type))
                            {
                                // on last elem. T, Build
                                //printf("+++++++++ s=%s [FAIL, ACT, !coll, ring-end] -> ACT %s\n", lex.c_str(), nameof(cur_pair->first->type));
                                next_pair->second = temp_token;
                                build_out(par);
                                el_ring.to_begin();
                                return ACT;
                            }
                            else if (prev_ending)
                            {
                                //printf("+++++++++ s=%s [FAIL, ACT, coll, prev-end] -> ACT %s\n", lex.c_str(), nameof(cur_pair->first->type));
                                build_out(par);
                                next_pair->second = temp_token;
                                if (!is_collection(next_pair->first->type))
                                    el_ring.to_next();

                                return ACT;
                            }
                            else
                            {
                                // activate first elem, last list
                                //printf("+++++++++ s=%s [FAIL, ACT, any] -> MOD %s\n", lex.c_str(), nameof(cur_pair->first->type));
                                next_pair->second = temp_token;
                                // step on next in the mid
                                return MOD;
                            }
                        }
                        else if (next_res == MOD)
                        {
                            // begin build first|next block
                            // go to first from last list.
                            // TEMMP ONLY THERE!
                            if (prev_ending)
                            {
                                build_out(par);
                                next_pair->second = temp_token;
                                return ACT;
                            }
                            else
                            {
                                next_pair->second = temp_token;
                                // curs_end,...
                                return MOD;
                            }
                        }
                        else if (next_res == FAIL)
                        {
                            // this->reset();
                            return FAIL;
                        }
                    }

                    //printf("[seq].act() (FAIL) -> FAIL <reset> s='%s' ring=%i\n", lex.c_str(), el_ring.pos());
                    // this->reset();
                    //  other elem
                    return FAIL;
                }

                //printf("[seq].act() (UNDEF CASE) -> FAIL  s='%s' ring=%i\n", lex.c_str(), el_ring.pos());
                return FAIL;
            };
        };

        struct prog_go : ParseValue
        {
            ParseElement *base; // by value

            void reset() {};

            prog_go(ParseElement *bs)
            {
                base = bs;
            }

            act_result act(string &lex, token *par, token *r2)
            {
                return base->act(lex, par, r2);
            }

            ~prog_go() {
                delete base;
            }
        };

        struct token_hook : ParseValue
        {
            string name;
            ParseElement *base; // by value
            token *tk;          // storing deep structure

            void reset() {};

            token_hook(string nam, ParseElement *bs)
            {
                name = nam;
                base = bs;
                tk = create_tk_for(base);
            }

            act_result act(string &lex, token *par, token *r2)
            {
                act_result res = base->act(lex, tk);
                if (res == act_result::FAIL)
                {
                    // deep clear ?
                    // feature: hook strict mode. clear after fail
                }

                return res;
            };

            ~token_hook() {
                //parcel::tools::Log.Link("~token_hook");
                delete base;
                delete tk;
            }
        };

        // ------------- ANALYZER.

    }
}

#endif