#include <string>
#include "parcel.h"
#include "builder.h"


using namespace std;
using namespace lex;

bool build::is_function(lexer& lx) {
    size_t move_back_size = 0;
            string id;
            if (lx.next_id(id) != lx.npos) {
                move_back_size+=id.size();
                if (lx.check_sequence("()", ":\n")) {
                    lx.go_back(move_back_size);
                    return true;
                }
            }

    lx.go_back(move_back_size);
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
                        lx.go_back(move_back_size);
                        return true;
                    }
                }
            }

    lx.go_back(move_back_size);
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
        lx.go_back(cur.size());
        return true;
    }
    else if (lx.next_int(cur) != lx.npos) { // 123
        lx.go_back(cur.size());
        return true;
    }
    else if (lx.next_like_rounded(cur, "\"", "\"", "") != lx.npos) { // "app"
        lx.go_back(cur.size());
        return true;
    } 
    else if (lx.next_like_rounded(cur, "'", "'", "") != lx.npos) { // 'g'
        lx.go_back(cur.size());
        if (cur.size() == 3) {
            return true;
        }
    }

    return false;
};

graph_block* _build_function(lexer& lx) {
    // get prefix
    // get name
    // get args
    char _pref = '_';
    lx.next_symbol(_pref);
    if (_pref != LANG_PREFIX) {
        return NULL;
    }

    string _name;
    if (lx.next_id(_name) == lx.npos) {
        return NULL;
    }

    string _args;
    if (lx.next_like_rounded(_args, "(", ")", lx.symbols_no_id_nospace) ==
        lx.npos) {
        return NULL;
    }

    graph_block *b = new graph_block();

    b->type = RULE_TYPE::FUNCTION;
    //b->value = b;
    return b;
};

graph_table<graph_block> *build_lex_graph(string &src) {
  graph_table<graph_block> *gt = new graph_table<graph_block>();

  lexer lx(src);
  string cur;
  size_t line_offset = 0;
  char single_prefix = '_';

  while (lx.can_read()) {

    if (lx.next_symbol(single_prefix)) {
      if (single_prefix == LANG_PREFIX) {
        if (build::is_function(lx)) {
          // build_func
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
    }

    /*
        id:
            is_tag
            is_literal
            is_function

        is_hook()
        is_func()
        is_tag()
        is_literal()

    */
  }

  return gt;
};


