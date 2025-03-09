#include "parcelapi.h"




using namespace parcel;
using namespace parcel::input;

/*
            Programm(string& sourceLang);
            Programm()
            bool is_builded();
            void run();
            void clean();
            std::vector<tokens::token*> get_hooks();
            tokens::token* find_hook(const char* name);
*/

Programm::Programm() : 
    _cursor(make_shared<parser::ParseCursor>(0, 0)),
    _empty(""),
    _tokenizator(_empty),
    _instr(NULL) {};

bool parcel::Programm::build(const char* sourceLang) {
    string sourceHolder(sourceLang);
    
    // get errors.
    LexTree *gt = parcel::build::build_lextree(sourceHolder);
    if (gt == NULL) {
        // error log
        return false;
    }
    
    _instr = parcel::build::build_parseinstr(gt);
    delete gt;

    if (_instr == NULL) {
        // error log
        return false;
    }

    return true;
};

bool parcel::Programm::is_builded() {
    return _instr != NULL;
};

void parcel::Programm::run(const char* parseSource) {
    string cp(parseSource);
    if (_instr == NULL) {
        // log err
        return;
    }

    // fix
    cp.append(" ");

    _tokenizator.set_source(cp);
    (*_cursor).size = cp.size();

    // propagate.
    string lx;
    while(_tokenizator.next_token(lx)) {
        _instr->propagate(lx);
    };
};

std::vector<token*> Programm::get_hooks() {
    std::vector<token*>  v;
    for (auto & s : _instr->hooks) {
        v.push_back(s.second->tk);
    }

    return v;
};

tokens::token* Programm::find_hook(const char* name) {
    string nm(name);
    auto fn = _instr->hooks.find(nm);
    if (fn == end(_instr->hooks)) {
        return NULL;
    }

    return (*fn).second->tk;
};





