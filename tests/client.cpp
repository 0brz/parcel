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
        
    string t("\n");
    cout << ex::act_alpha(t[0]) << "\n";
    cout << ex::act_id_symbol(t[0]) << "\n";
    cout << ex::act_numeric(t[0]) << "\n";
    cout << ex::act_id(t) << "\n";


    return 5;
    parcel::Programm pg;

    string src =  parcel::tools::Lexer::read_source_file("/home/gcreep/github.local/parcel_dev/parcel/tests/single_values.yml");
    string src_input =  parcel::tools::Lexer::read_source_file("/home/gcreep/github.local/parcel_dev/parcel/tests/single_values.txt");

    if (pg.build(src.c_str())) {
        cout << "Builded!\n";
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
    }
}