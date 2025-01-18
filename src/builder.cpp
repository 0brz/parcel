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

// float, int, str, char


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

graph_block* build::build_hook(lexer& lx) {
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
    printf("[build] hook: %s\n", _name.c_str());
    return b;
}

bool check_define_char(lexer& lx) {
    lx.skip(" ");
    if (lx.can_read()) {
        char t;
        lx.next_symbol(t);
        if (t == LANG_PREFIX) {
            return true;
        }
        else {
            lx.cursor_move(-1);
        }
    }

    return false;
};



graph_table<graph_block> *build::build_lex_graph(string &src) {
  graph_table<graph_block> *gt = new graph_table<graph_block>();

  lexer lx(src);
  string cur;
  size_t line_offset = 0;
  char single_prefix = '_';

  while (lx.can_read()) {
    
    if (check_define_char(lx)) {
        printf("[build].is_def\n");
        if (build::is_function(lx)) {
            printf("[build].is_fun\n");
            auto _bl = build::build_function(lx);
        }
        else if (build::is_hook(lx)) {
            printf("[build].is_hook\n");
            auto _bl = build_hook(lx);
        }
    }

    if (is_literal(lx)) {
        //printf("[build] is_lit\n");
        if (lx.next_float(cur) != lx.npos) {
            printf("[build] lit.float=%s\n", cur.c_str());
            int _val = stof(cur.c_str());
            auto _bl = create_literal<float, RULE_TYPE::LITR_FLOAT>(_val);
        }
        else if (lx.next_int(cur) != lx.npos) {
            printf("[build] lit.int=%s\n", cur.c_str());
            int _val = stoi(cur.c_str());
            auto _bl = create_literal<float, RULE_TYPE::LITR_INT>(_val);
        }
        else if (lx.next_like_rounded(cur, "\"", "\"", "") != lx.npos) {
            printf("[build] lit.str=%s\n", cur.c_str());
            // create_str
            string _val = string(cur.c_str());
            auto _bl = create_literal<string, RULE_TYPE::LITR_STRING>(_val);
        }
        else if (lx.next_like_rounded(cur, "'", "'", "") != lx.npos) {
            printf("[build] lit.char=%s\n", cur.c_str());
            // create_char
            char _val = cur[1]; // we can take, because size of char view=3, like 'a', 'b'
            auto _bl = create_literal<char, RULE_TYPE::LITR_STRING>(_val);
        }
    }

    lx.cursor_move(1);
  }

  return gt;
};


