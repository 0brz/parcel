#include "tools/tokenizator.h"


using namespace parcel::tools;


void Tokenizator::set_source(string& s) {
    _lex.set_buff(s);
    _lex.cursor_set(0);
};

bool Tokenizator::next_token(string& in) {
    if (!_lex.can_read())
    {
        return false;
    }
    
    short token_size = 0;
    if ((token_size = _lex.next_word(in)) != _lex.npos) {
        return true;
    }

    if (_options.count(option::FlaotAsTwoInts)) {
        if ((token_size = _lex.next_int(in)) != _lex.npos) {
            return true;
        }
    }
    else {
        if((token_size = _lex.next_float(in)) != _lex.npos) {
            return true;
        }
    }

    if((token_size = _lex.next_id(in)) != _lex.npos) {
        return true;
    }
    
    char cur_char;
    if (_lex.next_symbol(cur_char)) {
        in = cur_char;
        return true;
    }

    _lex.cursor_move(1);

    // word
    // int
    // float
    // id
    // char
}