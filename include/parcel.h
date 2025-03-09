
#ifndef _PARCEL_H_
#define _PARCEL_H_

#include <vector>
#include <memory>
#include "tools/tokenizator.h"
#include "tokens.h"

namespace parcel {
    namespace parser {
        struct ParseCursor;
    };

    namespace build {
        struct Instr;
    };

    class Programm {
        private:
            shared_ptr<parser::ParseCursor> _cursor;
            tools::Tokenizator _tokenizator;
            parcel::build::Instr* _instr;
            string _empty;

        public:
            Programm();
            bool build(const char* langSource);
            void run(const char* parseSource);
            std::vector<tokens::token*> get_hooks();
            tokens::token* find_hook(const char* name);

            bool is_builded();
            void clean();
    };
}

#endif