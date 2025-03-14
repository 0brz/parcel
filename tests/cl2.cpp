#include "../include/parcel.h"
#include "../include/tools/lexer.h"
#include "../include/dump.h"

int main( ) {

    parcel::Programm pg;

    string src =  parcel::tools::Lexer::read_source_file("/home/gcreep/github.local/parcel_dev/parcel/tests/lang.yml");
    string src_input =  parcel::tools::Lexer::read_source_file("/home/gcreep/github.local/parcel_dev/parcel/tests/source.txt");
    parcel::tools::Lexer lx(src);

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
    for (parcel::tokens::token* h : s) {
        parcel::dump::hook(h);
    }
    
}