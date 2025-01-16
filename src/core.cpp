


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
#include <cctype>

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
        const char* symbols_no_id = " @#$%^&^*()";

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

        //inline bool is_alpha(char t) {return t >= 'a' && t <= 'b'};

        // "word: begins: apple ends: s delims: @#$%^^&^&**()"
        short next_like(string& out, const char* begins, const char* ends, const char* trash_delims) {
            // or begns or write
            if (begins == NULL &&
                ends == NULL) {
                    return lexer::npos;
                } 

            // trash symbols logic
            if (trash_delims == NULL) {
                trash_delims = lexer::symbols_no_id;
            }

            skip(" ");
            if (begins != NULL && ends != NULL) {
                // begins + ends rule
                size_t _beg = _src.find_first_of(begins, _cursor);
                size_t _end = _src.find_first_of(ends, _cursor);
                if (_beg != string::npos &&
                    _end != string::npos) {
                       
                        // check no trash between
                        auto sz(_end-_beg + strlen(ends));
                        string btw = _src.substr(_beg, sz);
                        //printf("___BTW='%s'\n", btw.c_str());
                        if (btw.find_first_of(trash_delims, _beg) == string::npos) {
                            // ok
                            out = _src.substr(_beg, sz);
                            cursor_move(sz);
                            return sz;
                        }
                }
            } 
            else if (begins != NULL) {
                // |app|__&^_le
                // get until trash symbols finded
                size_t _beg = _src.find(begins, _cursor);
                size_t _end = _src.find_first_of(trash_delims, _beg);
                 
                if (_end == string::npos) {
                    _end = this->_sz; //
                }
                out = _src.substr(_beg, _end-_beg);
                cursor_move(_end-_beg);
            }
            else if (ends != NULL) {
                // |app__&^|_le|
                // if no trash between -> app__&^_le 
                size_t _beg = _src.find(ends, _cursor);

                auto sz(_beg-_cursor+strlen(ends));
                string btw = _src.substr(_cursor, sz);
                size_t _end = btw.find_first_of(trash_delims);

                if (_end == string::npos) {
                    out = btw;
                    cursor_move(sz);
                }
            }
        }

        short next_between(string& out, const char* begins, const char* ends) {
            // no trash symbols there
            // returs first begins and last ends
            // ret: 'for |<str> deep 1 <str> deep 2 </str> </str>| example data'

            if (begins == NULL &&
                ends == NULL) {
                    return lexer::npos;
            }

            auto _s = lexer::first_begins(_src, _cursor, begins);
            if (_s == -1) {return lexer::npos;}
            auto _e = lexer::last_ends(_src, _cursor, ends);
            printf("__E=%i\n", _e);
            if (_e== -1) {return lexer::npos;}

            auto sz(_e-_s+strlen(ends));
            out = _src.substr(_s, sz);
            cursor_move(sz);
            return sz;
        }

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
        
        static size_t last_ends(string& src, size_t cursor, const char* ends) {
            
            if (ends == nullptr) return -1;
            
            int sz = src.size();
            int t = sz;
            int sz_ends = strlen(ends);
            int pos_ends = sz_ends-1;

            while(t >= cursor) {
                
                if (pos_ends < 0) {
                    return t+1;
                }

                if (src[t] == ends[pos_ends]) {
                    t--;
                    pos_ends--;
                }
                else {
                    t--;
                    pos_ends = sz_ends-1;
                }
            }

            return -1;
        }

        static size_t first_begins(string& src, size_t cursor, const char* begins) {
            if (begins == nullptr) return -1;

            int sz = src.size();
            int t = cursor;
            int sz_ends = strlen(begins);
            int pos_ends = 0;

            while(t < sz) {
                
                if (pos_ends == sz_ends) {
                    return t-sz_ends;
                }

                if (src[t] == begins[pos_ends]) {
                    t++;
                    pos_ends++;
                }
                else {
                    t++;
                    pos_ends =0;
                }
            }

            return -1;
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
    string sr = " app__&^_le vibe _le: .123.42.";
    lexer lx(sr);
    
    string c;

    int t = lx.next_between(c, "app", "_le");
    printf("_t=%s tt=%i\n", c.c_str(), t);
    lx.get_info(cout);
    
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