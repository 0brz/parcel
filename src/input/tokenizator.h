#ifndef _TOKENIZATION_TOKENIZATOR_H_
#define _TOKENIZATION_TOKENIZATOR_H_

#include <string>
#include <map>
#include <set>
#include "../tools/lexer/lex.h"

using namespace std;


namespace parcel {
    namespace input {
        enum option {
            FlaotAsTwoInts,
        };

        class Tokenizator {
            private:
            std::set<input::option> _options;
            lexer _lex;

            public:
                Tokenizator(string& source) : _lex(source), _options{} {};
                bool next_token(string& in);
                void clean_options();
                void add_option(input::option opt);
        };
    }
}

#endif