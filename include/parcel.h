
#ifndef _PARCEL_H_
#define _PARCEL_H_

#include <vector>
#include <memory>
#include "tools/tokenizator.h"
#include "tokens.h"
#include "callbacks.h"

namespace parcel {

    struct Instr;

    //string to_string(tokens::token* head);
    string values_to_string(tokens::token* head, const char* delimiter);

    class Programm {
        private:
            tools::Tokenizator _tokenizator;
            Instr* _instr;
            string _empty;

        public:
            Programm();
            bool build(const char* langSource);
            bool build(const char* langSource, const build_callback cb);
            void run(const char* parseSource);
            std::vector<tokens::token*> get_hooks();
            tokens::token* find_hook(const char* name);

            bool is_builded();
            void clean();
    };
}

#endif