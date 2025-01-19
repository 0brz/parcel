#include <string>
#include "parcel.h"
#include "builder.h"


using namespace std;
using namespace lex;


// -------------------------
graph_block* create_function(string& name, string& args) {
    graph_block* b = new graph_block();
    value_function* v = new value_function(name, args);
    b->type = RULE_TYPE::FUNCTION;
    b->value = v;
    return b;
};

graph_block* create_hook(string& name) {
    graph_block* b = new graph_block();
    value_datahook* v = new value_datahook(name);
    b->type = RULE_TYPE::DATA_HOOK;
    b->value = v;
    return b;
};

template<typename ValueType, RULE_TYPE RuleType>
graph_block* create_literal(ValueType value) {
    graph_block* b = new graph_block();
    value_literal<ValueType, RuleType>* v = new value_literal<ValueType, RuleType>(value);
    b->type = RuleType;
    b->value = v;
    return b;
};

graph_block* create_block(RULE_TYPE type, graph_value* val) {
    graph_block* bl = new graph_block();
    bl->type = type;
    bl->value = val;
    return bl;
}

// -----------------------

bool build::is_function(lexer& lx) {
    //printf("______________\n");
    //lx.get_info(cout);
    size_t move_back_size = 0;
            string id;
            size_t ofs = 0;
            if ((ofs = lx.next_id(id)) != lx.npos) {
                //printf("___is_fun: id=%s\n", id.c_str());
                move_back_size+=ofs;
                //lx.get_info(cout);

                if (lx.check_sequence("()", ":")) {
                    //printf("___is_fun: sec=\n");
                    lx.cursor_move(-move_back_size);

                    //lx.get_info(cout);
                    //printf("______________\n");
                    return true;
                }
            }

    lx.cursor_move(-move_back_size);
    return false;
} ;

bool build::is_hook(lexer& lx) {
    size_t move_back_size = 0;
    char single_pref = '_';
            string id;
            if (lx.next_id(id) != lx.npos) {
                move_back_size+=id.size();
                if (lx.next_symbol(single_pref)) {
                    move_back_size++;
                    if (single_pref == LANG_TAG_PREFIX) {
                        lx.cursor_move(-move_back_size);
                        return true;
                    }
                }
            }

    lx.cursor_move(-move_back_size);
    return false;
} ;

bool is_hook_ref(lexer& lx) {
    size_t move_back_size = 0;
    char single_pref = '_';
            string id;
            if (lx.next_id(id) != lx.npos) {
                move_back_size+=id.size();
                lx.cursor_move(-move_back_size);
                return true;
            }

    lx.cursor_move(-move_back_size);
    return false;
} ;

bool build::is_literal(lexer& lx) {
    // 123.123.123 (ip...)
    // 123.123 (float)
    // 123 (int)
    // "word" -> litr_word
    // 'a' -> litr_symbol
    // "a" -> litr_word

    //printf("______________\n");
    //lx.get_info(cout);

    char single_pref = '_';
    string cur;
    if (lx.next_float(cur) != lx.npos) { // 123.123
        lx.cursor_move(-cur.size());
        return true;
    }
    else if (lx.next_int(cur) != lx.npos) { // 123
        lx.cursor_move(-cur.size());
        return true;
    }
    else if (lx.next_like_rounded(cur, "\"", "\"", "") != lx.npos) { // "app"
        lx.cursor_move(-cur.size());
        return true;
    } 
    else if (lx.next_like_rounded(cur, "'", "'", "") != lx.npos) { // 'g'
        lx.cursor_move(-cur.size());
        if (cur.size() == 3) {
            return true;
        }
    }

    //lx.get_info(cout);
    //printf("______________\n");
    return false;
};

 // ----------------------

graph_block* build::build_function(lexer& lx) {
    // get prefix
    // get name
    // get args
    lx.get_info(cout);
    string _name;
    if (lx.next_id(_name) == lx.npos) {
        printf("[ERR] build.function: name\n");
        lx.get_info(cout);
        return NULL;
    }

    string _args;
    if (lx.next_like_rounded(_args, "(", ")", lx.symbols_no_id_nospace) ==
        lx.npos) {
        printf("[ERR] build.function: args\n");
        lx.get_info(cout);
        return NULL;
    }

    graph_block* bl = create_function(_name, _args);
    printf("[build] fn: %s(%s)\n", _name.c_str(), _args.c_str());
    return bl;
};

graph_block* build::build_hook(lexer& lx, bool is_ref) {
    // get name
    // get args
    //lx.get_info(cout);
    string _name;
    if (lx.next_id(_name) == lx.npos) {
        printf("[ERR] build.hook: name\n");
        //lx.get_info(cout);
        return NULL;
    }

    graph_block *b = create_hook(_name);
    if (is_ref) {
        b->type == RULE_TYPE::DATA_HOOK_REF;
    }
    return b;
}

bool step_char_if_eq(lexer& lx, char check_symbol) {
    lx.skip(" ");
    if (lx.can_read()) {
        char t;
        lx.next_symbol(t);
        if (t == check_symbol) {
            return true;
        }
        else {
            lx.cursor_move(-1);
        }
    }

    return false;
};


graph_table<graph_block*> *build::build_lex_graph(string &src) {
    graph_table<graph_block*>* gt = new graph_table<graph_block*>();

  lexer lx(src);
  string cur;
  size_t line_offset = 0;
  char single_prefix = '_';

  while (lx.can_read()) {
    
    if (step_char_if_eq(lx, LANG_PREFIX)) {
        //printf("[build].is_def\n");
        if (build::is_function(lx)) {
            graph_block* _bl = build::build_function(lx);
            gt->add(_bl, line_offset);
            printf("~%zi [gt].func\n", line_offset);
        }
        else if (build::is_hook(lx)) {
            auto _bl = build_hook(lx, false);
            gt->add(_bl, line_offset);
            printf("~%zi [gt].hook\n", line_offset);
            //lx.get_info(cout);
        }
        else if ( is_hook_ref(lx)) {
            auto _bl = build_hook(lx, true);
            gt->add(_bl, line_offset);
            printf("~%zi [gt].hook(ref)\n", line_offset);
        }
    }

    if (is_literal(lx)) {

        graph_block* _last;
        string _last_name;
        if (gt->last(_last)) _last_name = lex::nameof(_last->type);
        else _last_name = "<undf>";

        //printf("[build] is_lit\n");
        if (lx.next_float(cur) != lx.npos) {
            printf("~%zi [build] lit.float=%s\n", line_offset, cur.c_str() );
            int _val = stof(cur.c_str());
            auto _bl = create_literal<float, RULE_TYPE::LITR_FLOAT>(_val);
        }
        else if (lx.next_int(cur) != lx.npos) {
            printf("~%zi [build] lit.int=%s\n", line_offset, cur.c_str());
            int _val = stoi(cur.c_str());
            auto _bl = create_literal<float, RULE_TYPE::LITR_INT>(_val);
        }
        else if (lx.next_like_rounded(cur, "\"", "\"", "") != lx.npos) {
            string _val = string(cur.c_str());
            auto _bl = create_literal<string, RULE_TYPE::LITR_STRING>(_val);
            printf("~%zi [gt] [lit.str] %s.%s\n", line_offset, _last_name.c_str(), cur.c_str());

        }
        else if (lx.next_like_rounded(cur, "'", "'", "") != lx.npos) {
            printf("~%zi [build] lit.char=%s\n", line_offset, cur.c_str());
            // create_char
            char _val = cur[1]; // we can take, because size of char view=3, like 'a', 'b'
            auto _bl = create_literal<char, RULE_TYPE::LITR_STRING>(_val);
        }
    }

    //lx.get_info(cout);
    if (lx.next_word(cur) != lx.npos) {
        //printf("___word\n");
        //lx.get_info(cout);
        if (step_char_if_eq(lx, LANG_TAG_PREFIX)) {

            RULE_TYPE _type = lex::typeof(cur);
            if (_type == RULE_TYPE::_TYPE_ERROR) {
                // error
                printf("~%zi [ERR] build: (err type) tag='%s'\n", line_offset, cur.c_str());
            }

            if (_type == RULE_TYPE::GO) {
                graph_block* _bl = create_block(RULE_TYPE::GO, NULL);
                gt->add(_bl, line_offset);
                printf("~%zi [gt] [tag].go\n", line_offset);
            }
            else {
                // add to parent
                graph_block* _last;
                string _last_name;
                if (gt->last(_last)) _last_name = lex::nameof(_last->type);
                else _last_name = "<undf>";

                graph_block* _bl = create_block(_type, NULL);
                gt->add(_bl, line_offset);
                printf("~%zi [gt] %s.%s\n", line_offset, _last_name.c_str(), cur.c_str());
            }

            continue;
        }
        else {

        }
    }
    

    if (step_char_if_eq(lx, '\n')) {
        line_offset = lx.skip(" \t");
    }
    else  {
        lx.cursor_move(1);
    }
  }

  return gt;
};


