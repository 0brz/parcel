#include "activates.h"

bool act_escape_char(const char a) {
    switch (a) {
        case '\\':
        case '\"':
        case '\'':
        case '\?':
        case '\a':
        case '\b':
        case '\f':
        case '\n':
        case '\r':
        case '\t':
        case '\v':
        case '\0':
            return true;
        default:
            return false;
    }

    return 0;
}

bool act_alpha(const char a) {
    return (a >= 'a' && a <= 'z') ||
            (a >= 'A' && a <= 'Z');
};

bool act_numeric(const char v) {
    return '0' <= v && v <= '9';
};


bool act_id_symbol(const char v) {
    switch (v)
    {
    case '_':
    case '-':
        return true;

    default:
        return false;
    }
};

bool act_word(string& lex) {
    return std::all_of(begin(lex), end(lex), [](const char v) {
        return act_alpha(v);
    });
};

bool act_num(string& lex) {
    return std::all_of(begin(lex), end(lex), [](const char v) {
        return act_numeric(v);
    });
};

bool act_id(string& lex) {
    if (lex.size() > 0 && act_numeric(lex.at(0))) {
        return false;
    }

    return std::all_of(begin(lex), end(lex), [](const char v) {
        return act_numeric(v) || act_alpha(v) || act_id_symbol(v);
    });
};
