


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
#include <fstream>

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
        const char* symbols_no_id = " @#%^&*(){}[]?><";

        lexer(string& sr) : _src(sr), _sz(_src.size()), _cursor(0) {};

        inline int can_read() {return this->_cursor - _sz; };
        inline int can_read(int count) { return (this->_cursor+count) < _sz ;} 

        int cursor_get() {return _cursor;};
        void cursor_move(int delta) {this->_cursor += delta;};
        void cursor_set(int pos) {this->_cursor = pos;}; 

        size_t skip(const char* s) {
            int ofs = _src.find_first_not_of(s, _cursor);
            size_t old_c = _cursor;
            if (ofs != string::npos) {
                cursor_move(ofs-_cursor);
            }

            return ofs-old_c;
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
            //printf("_curs=%i\n", cur);
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

        short next_like(string& out, const char* begins, const char* ends, const char* trash_delims, bool offset=false) {
            // or begns or write
            if (begins == NULL &&
                ends == NULL) {
                    return lexer::npos;
                } 

            // trash symbols logic
            if (trash_delims == NULL) {
                trash_delims = lexer::symbols_no_id;
            }

            if (!can_read()) {
                return -1;
            }

            skip(" ");
            if (begins != NULL && ends != NULL) {
                // begins + ends rule
                size_t _beg = _src.find(begins, _cursor);
                short _ofs = (offset == true ? 1 : 0); 
                size_t _end = _src.find(ends, _cursor + _ofs);
                if (_beg != string::npos &&
                    _end != string::npos) {
                       
                        // check no trash between
                        auto sz(_end-_beg + strlen(ends));
                        string btw = _src.substr(_beg, sz);
                        //printf("___BTW='%s' sz=%zi END=%zi\n", btw.c_str(), sz, _end);
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
                
                if (_beg == string::npos) {
                    return lexer::npos;
                }

                if (_end == string::npos) {
                    _end = this->_sz; //
                    printf("____(-1) beg=%zi ends=%zi\n", _beg, _end);
                }
                else {
                     printf("____(+) beg=%zi ends=%zi\n", _beg, _end);
                }

                //auto sz(_beg-_cursor+strlen(ends));
                
                out = _src.substr(_beg, _end-_beg);
                //cursor_move(_end-_beg);
                cursor_set(_end);
            }
            else if (ends != NULL) {
                // |app__&^|_le|
                // if no trash between -> app__&^_le 
                size_t _beg_ends = _src.find(ends, _cursor);
                auto sz(_beg_ends-_cursor+strlen(ends));
                string btw = _src.substr(_cursor, sz);

                size_t _trash_btwn = btw.find_first_of(trash_delims);
                if (_trash_btwn != string::npos) {
                    return lexer::npos;
                }

                //printf("__curs=%i _ends=%i\n", _cursor, _beg_ends);
                out = btw;
                cursor_set(_cursor+sz);

                return sz;
            }
        }

        short next_like_rounded(string& out, const char* begins, const char* ends, const char* trash_delims) {
            return next_like(out, begins, ends, trash_delims, true);
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

        void go_back(int delta) {
            this->cursor_move(delta);
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
    size_t _cursor;
    string& _src;


    // build_tag
    // build_list
    // build_block
    // ...

    /*
        get tags and try to build instructions (blocks)
        sequence blocks are defined 
    */

    public:
    //format_analyzer(,)



    // check_block() -- we need begin, end positions of buff
    // build_block() -- 

    // check_tagval
    // check_

    /*
    format_base_block* build(string& fmt_src, ) {
        
    };

    format_base_block* build_tagval(string& fmt_src, size_t cursor) {

    }
    */

};

// parser


void get_literals() {
    //string litrs = "tagval: tag: \"apple\" val: \"good\" volume: 123.12478"; // 
    string litrs = "tagval: tag: \"apple\" val: \"good\" volume: 123.12478"; // 
    lexer lx(litrs);
    string cur;

    // we should parse tags and get next format 'word in brackets | float'

    while(lx.can_read()) {
        char delim;
        // find tag -> tag=word + :
        // lx.get_info(cout);
        short word_len = 0;
        if ((word_len = lx.next_id(cur)) != lx.npos &&
            lx.next_symbol(delim) != lx.npos) { // or use next_like with ':'
            if (delim == ':') {
                // this is a tag.
                // 
                cur += delim;
                printf("[tag] %s\n", cur.c_str());
            }
            else {
                lx.cursor_move(-1);
                lx.cursor_move(-word_len);
                //lx.
            }
        }
        
        // this is not a our rule.
        // maybe this is a literal
            
        // word litr
        if (lx.next_like_rounded(cur, "\"",  "\"", "") != lx.npos) {
            printf("[litr.word] '%s'\n", cur.c_str());
        }
        // float litr
        else if (lx.next_float(cur) != lx.npos) {
            printf("[litr.float] '%s'\n", cur.c_str());
        }
        else  lx.cursor_move(1);
    }
}

// get_words
// get_literals
// get_


/*

    # check/build (block)
    tagval:
    1) tagval
    2) tag,val

    str = pick()
    -> map.find(st)r

*/


// ------------------------------------------

#include <optional>

struct lex_graph_path {
    string name;
    vector<lex_graph_path> paths; 
};

struct lex_graph {
    vector<lex_graph_path> entries;
};

class graph_floor {
    map<int, vector<lex_graph_path>> _entries;
    int _last_level;
    public:

        graph_floor(): _last_level(0) {};

        bool find(int offset, string& entry_name, lex_graph_path& out) {
            auto fn = _entries.find(offset);
            if (fn != end(_entries)) {

                for (const auto &en: (*fn).second) {
                    if (en.name == entry_name) {
                        out = en;
                        return true;
                    }
                }
            }
            
            return false;
        };

        bool prev(lex_graph_path& out) {
            auto fn = _entries.find(_last_level);
            if (fn != end(_entries)) {
                auto vec = (*fn).second;
                if (vec.size() > 0) {
                    out = vec.back();
                    return true;
                }
                
                return false;
            }
            
            return false;
        }

        bool parent(int offset, lex_graph_path& out) {
            auto fn = _entries.find(offset);
            if (fn != end(_entries)) {
                auto vec = (*fn).second;
                if (vec.size() > 0) {
                    out = vec.back();
                    return true;
                }
                
                return false;
            }
            
            return false;
        }

        void add(lex_graph_path& p, int offset) {
            auto fn = _entries.find(offset);
            if (fn != end(_entries)) {
                (*fn).second.push_back(p);
            }
            else {
                vector<lex_graph_path> v;
                v.push_back(p);
                _entries.insert({offset, v});
            }

            _last_level= offset;
        };

        void dump() {
            printf("[graph_floor]\n");
            for(auto &e : _entries) {
                for(auto&p : e.second) {
                    printf("[graph_floor].[%i] %s\n", e.first, p.name.c_str());
                }
            }
        };

        ~graph_floor() {
            _entries.clear();
        };
};

lex_graph* build_graph(string& src) {
    lex_graph* g = new lex_graph();
    graph_floor floor;
    lexer lx(src);

    string cur;
    size_t line_offset = 0;

    while(lx.can_read()) {
        if (lx.next_id(cur) != lx.npos) {

            char delim ;
            lx.next_symbol(delim);

            // find_
            /*
            lex_graph_path par;
            if (floor.parent(line_offset-2, par)) {
                //printf("(parent) %s\n", par.name.c_str());
            }
            else {
                par.name = "_";
            }
            */

            if (delim == ':') {
                // next_entry (relate previos path)
                // add path to parent path

                //printf("[tag] (%s) %s\n", par.name.c_str(), cur.c_str());

                lex_graph_path pt;
                pt.name = string(cur.c_str());
                floor.add(pt, line_offset);

                // par.add()
                lex_graph_path _par;
                if (floor.parent(line_offset-2, _par)) {
                    printf("[g] %s . %s\n", _par.name.c_str(), cur.c_str());
                    _par.paths.push_back(pt);
                }
                else {
                    printf("[ERR] _par not found\n");
                }
            }
            else if (delim == '|') {
                
                lex_graph_path pt;
                pt.name = string(cur.c_str());
                floor.add(pt, line_offset);

                // prev().add()
                lex_graph_path _tag;
                if (floor.prev(_tag)) {
                    printf("[g] %s . %s\n", _tag.name.c_str(), cur.c_str());
                    _tag.paths.push_back(pt);
                }
                else {
                    printf("[ERR] _tag not found\n");
                }
            }
            else {

                lx.go_back(-1);
                //printf("[val] (%s) %s\n", par.name.c_str(), cur.c_str());
                
                // next tag started.
                // add to paths in parent
                // prev().add()
                lex_graph_path pt;
                pt.name = string(cur.c_str());
                
                lex_graph_path _tag;
                if (floor.prev(_tag)) {
                    printf("[g] %s . %s\n", _tag.name.c_str(), cur.c_str());
                    _tag.paths.push_back(pt);
                }
                else {
                    printf("[ERR] _tag not found\n");
                }
            }
        }
        else {
            char delim;
            lx.next_symbol(delim);
            if (delim == '\n') {
                // this is signal to next line
                // we can left padding size of next level
                line_offset = lx.skip(" \t");
                
            }
        }
    }

    floor.dump();

    return g;
}  

#include <sstream>

string file_read_all(const char* path) {
    ifstream fs(path);
    if (fs.is_open()) {
        ostringstream ss;
        ss << fs.rdbuf();
        return ss.str();
    }
    else {
        cout << "file bad open\n";
    }
}

int main( ){

    string sr = file_read_all("C:/git.local/parsing/bound_parsing/src/fmt.yml");
    //printf("s=%s\n", sr.c_str());
    build_graph(sr);


    string s1 = "literal: lit_word|lit_number";
    string s2 = "tagval: tag: literal val: lit_word|lit_number";
    

    //get_literals();
    
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