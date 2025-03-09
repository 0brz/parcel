#ifndef _PARCELAPI_H_
#define _PARCELAPI_H_

#include <vector>
#include "parse/token.h"
#include "parse/elements.h"
#include "input/tokenizator.h"
#include "input/tokenizator.h"
#include "builder/lextree_build.h"
#include "builder/parseinstr_build.h"

namespace parcel {
    class Programm {
        private:
            shared_ptr<parser::ParseCursor> _cursor;
            input::Tokenizator _tokenizator;
            parcel::build::Instr* _instr;
            string _empty;

        public:
            Programm();
            bool build(const char* langSource);
            void run(const char* parseSource);
            std::vector<token*> get_hooks();
            tokens::token* find_hook(const char* name);

            bool is_builded();
            void clean();
    };
}


#endif