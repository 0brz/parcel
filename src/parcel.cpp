#include "parcel.h"

using namespace parcel;
using namespace parcel::tokens;
using namespace parcel::tools;

#include "builder/lextree_build.h"
#include "builder/parseinstr_build.h"
using namespace parcel::build;

/*
            Programm(string& sourceLang);
            Programm()
            bool is_builded();
            void run();
            void clean();
            std::vector<tokens::token*> get_hooks();
            tokens::token* find_hook(const char* name);
*/

void _ltrim(string& str) {
    if (str.find_first_of(' ') != string::npos) {
        str.erase(0, str.find_first_not_of(' '));
    }
};

void _rtrim(string& str) {
    if (str.find_first_of(' ') != string::npos)  {
        str.erase(str.find_last_not_of(' ')+1, str.find_first_of(' '));
    }
    
};

string parcel::values_to_string(tokens::token* head, const char* delimiter) {
    
    stringstream ss;

    queue<token *> q;
    q.push(head);

    while (!q.empty())
    {
        auto cur = q.front();

        if (cur->type == parcel::tokens::list)
        {
            val_list *ls = static_cast<val_list *>(cur->val);
            if (ls != NULL)
                for (token *n : ls->v)
                    q.push(n);
        }
        else if (cur->type == parcel::tokens::set)
        {
            val_set *ls = static_cast<val_set *>(cur->val);
            if (ls != NULL)
                for (token *n : ls->v)
                    q.push(n);
        }
        else if (cur->type == parcel::tokens::vec)
        {
            val_vector *ls = static_cast<val_vector *>(cur->val);
            if (ls != NULL)
                for (token *n : ls->v)
                    q.push(n);
        }
        else if (cur->type == parcel::tokens::seq)
        {
            val_seq *ls = static_cast<val_seq *>(cur->val);
            if (ls != NULL)
                for (token *n : ls->v)
                    q.push(n);
        }
        else {
            ss << cur->val->to_string();
        }

        ss << delimiter;

        q.pop();
    }

    auto out_str = ss.str();

    _ltrim(out_str);
    _rtrim(out_str);

    return out_str;
}

Programm::Programm() : 
    _empty(""),
    _tokenizator(_empty),
    _instr(NULL) {};

bool parcel::Programm::build(const char* sourceLang, const build_callback cb) {
    string sourceHolder(sourceLang);
    
    // get errors.
    LexTree *gt = parcel::build::build_lextree(sourceHolder, cb);
    if (gt == NULL) {
        // error log
        return false;
    }
    
    _instr = parcel::build::build_parseinstr(gt, cb);
    delete gt;

    if (_instr == NULL) {
        // error log
        return false;
    }
    
    

    return true;
}

bool parcel::Programm::build(const char* sourceLang) {
    return build(sourceLang, NULL);
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
    (*this->_instr->cursor).size = cp.size();

    // propagate.
    string lx;
    while(_tokenizator.next_token(lx)) {
        //printf("prop='%s'\n", lx.c_str());
        _instr->propagate(lx);
    };
};

std::vector<token*> Programm::get_hooks() {
    std::vector<token*>  v;
    if (_instr != NULL) {
        for (auto & s : _instr->hooks) {
            v.push_back(s.second->tk);
        }
    }

    return v;
};

tokens::token* Programm::find_hook(const char* name) {
    string nm(name);
    if (_instr != NULL) {
        auto fn = _instr->hooks.find(nm);
        if (fn == end(_instr->hooks)) {
            return NULL;
        }

        return (*fn).second->tk;
    }
    else return NULL;
};


void Programm::clean() {
    // clear instructions.
    if (this->_instr != NULL) {
        delete this->_instr;
    }
};


