#include "../include/parcel.h"
#include "../include/tools/lexer.h"

int main( ) {
    parcel::Programm pg;
    string src =  parcel::tools::Lexer::read_source_file("/home/gcreep/github.local/parcel_dev/parcel/tests/lang.yml");
    parcel::tools::Lexer lx(src);
    
    if (pg.build(src.c_str())) {
        cout << "Builded!\n";
    }
    else {
        cout << "Not builded!\n";
    }
}