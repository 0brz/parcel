#include "tools/tokenizator.h"


using namespace parcel::tools;


void Tokenizator::set_source(string& s) {
    _lex.set_buff(s);
    _lex.cursor_set(0);
};

bool _next_symbol_whitespace(Lexer& lx) {
    char cur_char;
    if (lx.next_symbol(cur_char)) {
        lx.cursor_move(-1);
        return cur_char == ' ';
    }

    return false;
}

bool _next_symbol_word_delimiter(Lexer& lx) {
    char cur_char;
    string sw(lx.symbols_no_id_nospace);
    if (lx.next_symbol(cur_char)) {
        //printf("_next_symbol_word_delimiter=%c\n", cur_char);
        if (sw.find_first_of(cur_char) != string::npos) {
            lx.cursor_move(-1);
            return true;
        }
        else {
            lx.cursor_move(-1);
            return false;
        }
    }

    return false;
}

bool _next_single_token(Lexer& lx, char& in) {
    string sw(lx.symbols_no_id_nospace);
    if (lx.next_symbol(in)) {
        //printf("CUR_CHAR=%c\n", in);
        if (sw.find_first_of(in) != string::npos) {
            return true;
        }
        else {
            lx.cursor_move(-1);
            return false;
        }
    }

    return false;
};

bool Tokenizator::next_token(string& in) {
    if (!_lex.can_read())
    {
        return false;
    }
    
    //_lex.get_info(cout);

    size_t old = _lex.cursor_get();
    short token_size = 0;
    if ((token_size = _lex.next_word(in)) != _lex.npos) {
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
        //_lex.get_info(cout);
        if (_next_symbol_whitespace(_lex) || _next_symbol_word_delimiter(_lex) || !_lex.can_read()) {
            return true;
        }
        else {
            _lex.cursor_set(old);
        }
    }

    // if single_token
    char cur_char;
    if (_next_single_token(_lex, cur_char)) {
        in = cur_char;
        return true;
    }

    // next_until
    if ((token_size = _lex.next_until(" \t\r\n", in)) != _lex.npos) {
        if (token_size > 0)
            return true;
    }

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