#include "tools/tokenizator.h"


using namespace parcel::tools;


void Tokenizator::set_source(string& s) {
    _lex.set_buff(s);
    _lex.cursor_set(0);
};

bool _next_symbol_whitespace(Lexer& lx) {
    char cur_char;
    if (lx.next_symbol(cur_char)) {
        return cur_char == ' ';
    }

    return false;
}

bool Tokenizator::next_token(string& in) {
    if (!_lex.can_read())
    {
        return false;
    }
    
    size_t old = _lex.cursor_get();
    short token_size = 0;
    if ((token_size = _lex.next_word(in)) != _lex.npos) {
        //_lex.get_info(cout);
        if (_next_symbol_whitespace(_lex)) {
            return true;
        }
        else {
            _lex.cursor_set(old);
            //_lex.get_info(cout);
        }
    }

    if (_options.count(option::FlaotAsTwoInts)) {
        if ((token_size = _lex.next_int(in)) != _lex.npos) {
            
            _lex.cursor_move(token_size);
            if (!_next_symbol_whitespace(_lex)) {
                return true;
            }
            else {
                _lex.cursor_set(old);
            }
        }
    }
    else {
        if((token_size = _lex.next_float(in)) != _lex.npos) {
            if (_next_symbol_whitespace(_lex)) {
                return true;
            }
            else {
                _lex.cursor_set(old);
                //_lex.get_info(cout);
            }
        }
    }

    if ((token_size = _lex.next_id(in)) != _lex.npos) {
        if (_next_symbol_whitespace(_lex) || !_lex.can_read()) {
            return true;
        }
        else {
            _lex.cursor_set(old);
        }
    }

    // next_until
    if ((token_size = _lex.next_until(" \t\r\n", in)) != _lex.npos) {
        if (token_size > 0)
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