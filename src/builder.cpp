#include <string>
#include "parcel.h"
#include "builder.h"


using namespace std;
using namespace lex;


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
        lx.cursor_move(cur.size());
        return true;
    } 
    else if (lx.next_like_rounded(cur, "'", "'", "") != lx.npos) { // 'g'
        lx.cursor_move(-cur.size());
        if (cur.size() == 3) {
            return true;
        }
    }

    return false;
};

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

    graph_block *b = new graph_block();

    b->type = RULE_TYPE::FUNCTION;
    //b->value = b;
    printf("[build] fn: %s(%s)\n", _name.c_str(), _args.c_str());

    return b;
};

graph_block* build_hook(lexer& lx) {
    // get name
    // get args
    //lx.get_info(cout);
    string _name;
    if (lx.next_id(_name) == lx.npos) {
        printf("[ERR] build.hook: name\n");
        //lx.get_info(cout);
        return NULL;
    }

    graph_block *b = new graph_block();

    b->type = RULE_TYPE::DATA_HOOK;
    //b->value = b;
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
    else {
        //lx.get_info(cout);
        //printf("_pkd=%c\n", single_prefix);
        lx.cursor_move(1);
    }

    /*
        is_define
            -is_func()
            -is_hook
        
        is_literal
            -float()
            -int()
        
        is_tag
            -id()
    */

   /*
    if (lx.next_symbol(single_prefix)) {
      if (single_prefix == LANG_PREFIX) {
        if (build::is_function(lx)) {
          auto fn = build::build_function(lx);
          printf("[func]\n");
        } else if (build::is_hook(lx)) {

          printf("[hook]\n");
        }
      }

      lx.go_back(1);
      // other stuff.
      if (build::is_literal(lx)) {
        printf("[litr]\n");
      } else if (lx.next_id(cur) != lx.npos) {
        printf("[tag]\n");
      }

        //printf("__char=%c\n", single_prefix);
    }
  }
  */
  }

  return gt;
};


