#include <parcel.h>
#include <tools/lexer.h>
#include <dump.h>
#include <algorithm>
#include <ranges>

using namespace std;

namespace ex {
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
            return (act_numeric(v) == true) || (act_alpha(v) == true) || (act_id_symbol(v) == true) == true;
        });
    };
    
};



int main(int argc, char **argv) {
        
    parcel::Programm pg;

    string src =  parcel::tools::Lexer::read_source_file("/home/gcreep/github.local/parcel_dev/parcel/tests/cl.yml");
    string src_input =  parcel::tools::Lexer::read_source_file("/home/gcreep/github.local/parcel_dev/parcel/tests/cl.txt");

    if (pg.build(src.c_str(), [](bool ok, string msg) {
        if (!ok) {
            printf("Error when building: %s\n", msg.c_str()); 
        }})) {
    }
    else {
        cout << "Not builded!\n";
    }

    // TESTING
    if (pg.is_builded()) {
        printf("run prog.\n");
        pg.run(src_input.c_str());
    }

    // parcel::tools::dump
    vector<parcel::tokens::token*> s = pg.get_hooks();
    for (auto & v: s) {
        parcel::dump::hook(v);
        // convert hook
    }
}