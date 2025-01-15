


/*

structs:
    - format_block
    - format

    - block
    - text
    - tagval
    - list
    - nested: -> linked_list of blocks

    - pbuff

    - begins_end

    ---
    build_format


*/

#include <string>
#include <vector>
#include <map>
#include <iostream>

using namespace std;

/*
    fmt block
    -name
    -type
*/

/*
     val: word|number|version|ip --simple blocks sequence
     val: tagval:  -- defined block
*/

enum RULE_TYPE {
    GO,
    
    BLOCK,
    LIST,

    WORD,
    NUMBER,
    //VERSION,
    //IP
    PRM_BEGINS,
    PRM_ENDS,
    PRM_DELIM,

    TAGVAL, // useword
    TAGVAL_TAG, // block | value
    TAGVAL_VAL, // block | value

    // literals...
    LITR,
    LITR_WORD,
    LITR_NUM,
};

map<RULE_TYPE, const char*> _typenames {
    {GO, "go"},
    {BLOCK, "block"},
    {LIST, "list"},

    {WORD, "word"},
    {NUMBER, "number"},

    {LITR, "literal"},
    {LITR_WORD, "litr_word"},
    {LITR_NUM, "litr_num"},
};

// block -> block -> block -> *list

/*
    base block (str) for all values
*/


/*
    // 'good : job'
    block: word | tagval | int (build_list)

    tagval:
        tag: block(->list/block/word) | literal()
        val

    word: block(ST) | literal(ST)
    literal: word(VAL) | word_like (VAL)
*/

// =======  =======
//    VALUES

struct format_base_value {
    inline virtual RULE_TYPE get_type() = 0;
    inline  const char* get_name() {_typenames[this->get_type()];};
};

struct value_tagval : public format_base_value {
    inline RULE_TYPE format_base_value::get_type() {
        return TAGVAL;
    };
};

struct value_litr_word : public format_base_value {
    inline RULE_TYPE format_base_value::get_type() {
        return LITR_WORD;
    };
};

struct value_litr_num : public format_base_value {
    inline RULE_TYPE format_base_value::get_type() {
        return LITR_NUM;
    };
};


// =======  =======
//    FORMATS

struct format_base_block {
    RULE_TYPE type;
    format_base_block* next;
    format_base_value* value;
};

// has_value
// can_build

// is_block
// is_litr
// next_lex()

/*
    block: 
        tagval:
            tag: word
            val: number
*/

class lexer {
    private:
    int _cursor;
    string& _src;
    size_t _sz;
    public:
        const short npos = -1;

        lexer(string& sr) : _src(sr), _sz(_src.size()), _cursor(0) {};

        inline int can_read() {return this->_cursor - _sz; };
        inline int can_read(int count) { return (this->_cursor+count) < _sz ;} 

        int cursor_get() {return _cursor;};
        void cursor_move(int delta) {this->_cursor += delta;};
        void cursor_set(int pos) {this->_cursor = pos;}; 

        void skip(const char* s) {
            int ofs = _src.find_first_not_of(s, _cursor);
            if (ofs != string::npos) {
                cursor_move(ofs-_cursor);
            }
        }

        bool next_symbol(char& s) {
            if (can_read()) {
                s = _src[this->_cursor++];
                return true;
            }else {
                return false;
            };
        };

        short next_word(string& out) {
            
            short sz = 0;
            skip(" ");
            int cur = this->_cursor;
            while(cur < _sz) {
                if (_src[cur] >= 'a' && _src[cur] <= 'z') {
                    sz++;
                    cur++;
                }
                else break;
            }

            if (sz == 0) {
                return lexer::npos;
            }

            out = _src.substr(this->_cursor, sz);
            cursor_move(sz);
            return sz;
        }

        short next_id(string& out) {
            short sz = 0;
            skip(" ");
            int cur = this->_cursor;
            printf("_curs=%i\n", cur);
            while(cur < _sz) {
                if ((_src[cur] >= 'a' && _src[cur] <= 'z') ||
                    (_src[cur] >= '0' && _src[cur] <= '9') ||
                    _src[cur] == '_') {
                    sz++;
                    cur ++;
                }
                else break;
            }

            if (sz == 0) {
                return lexer::npos;
            }

            
            out = _src.substr(this->_cursor, sz);
            cursor_move(sz);
            return sz;
        }

        short next_float(string& out) {
            // 123.123
            // 123.3
            // 123
            short sz = 0;
            skip(" ");
            int cur = this->_cursor;
            int dotp = 0;
            //printf("_curs=%i\n", cur);
            while(cur < _sz) {
                //printf("_cursor=%c\n", _src[cur]);
                if ((_src[cur] >= '0' && _src[cur] <= '9')) {
                    sz++;
                    cur ++;
                }
                else if (_src[cur] == '.') {
                    if (sz == 0) {
                        return lexer::npos;
                    }
                    else {
                        if (dotp != 0) {
                            break;
                        }
                        else 
                        {
                            dotp = cur;
                            sz++;
                            cur++;
                        }
                    }
                }
                else break;
            }

            if (sz == 0) {
                return lexer::npos;
            }

            out = _src.substr(this->_cursor, sz);
            cursor_move(sz);
            return sz;
        };

        short next_int(string& out) {
            short sz = 0;
            skip(" ");
            int cur = this->_cursor;
            int dotp = 0;
            //printf("_curs=%i\n", cur);
            while(cur < _sz) {
                //printf("_cursor=%c\n", _src[cur]);
                if ((_src[cur] >= '0' && _src[cur] <= '9')) {
                    sz++;
                    cur ++;
                }
                else {
                    break;
                }
            }

            if (sz == 0) {
                return lexer::npos;
            }

            out = _src.substr(this->_cursor, sz);
            cursor_move(sz);
            return sz;
        }

        // next_like(begins, ends)
        // next_ends
        // next_between(s, e) curs move to e

  
        void back_symbol() {
            this->cursor_move(-1);
        };

        void get_info(ostream& os) {
            if (this->_cursor > 2) {
                string around = this->_src.substr(this->_cursor-3, 3);
                os << "lexer: cs=" << this->_cursor << " '" << around.c_str() << "|'\n";
            }
            else os << "lexer: cs=" << this->_cursor << "..|...\n";
        }
        
};

class format_analyzer {
    private:
    int cursor;

    // build_tag
    // build_list
    // build_block
    // ...

    public:
    format_base_block* build(string& fmt_src) {
        
    };

};



int main( ){
    string sr = " apple vibe: .123.42.";
    lexer lx(sr);
    
    string c;
    lx.next_word(c);
    printf("%s\n", c.c_str());

    lx.get_info(cout);

    lx.next_id(c);
    printf("%s\n", c.c_str());

    lx.cursor_move(1);
    //printf("%s\n", c.c_str());

    lx.next_float(c);
    printf("%s\n", c.c_str());

    lx.get_info(cout);

    lx.cursor_move(1);

    lx.next_int(c);
    printf("%s\n", c.c_str());
}



/*

// format_base_block

// ...
// format_

struct format_rule {
    RULE_TYPE type;
    string* value;
};

struct format_block {
    // list of rules
    vector<format_rule> rules;
};

struct format_user_rule {
    format_rule rule;
    string name;
};

// nested: all: tagval -> {{apple: good, time: sec}, {foo: bar}}
// nested:  

// ---------- USER VALUES
*/