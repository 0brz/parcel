#include "../include/parcel.h"
#include "../include/tools.h"

int main( ) {
    parcel::Programm pg;
    string src =  parcel::tools::lexer::read_source_file("/home/gcreep/github.local/parcel_dev/parcel/tests/lang.yml");
    parcel::tools::lexer lx(src);
    
    if (pg.build(src.c_str())) {
        cout << "Builded!\n";
    }
    else {
        cout << "Not builded!\n";
    }
    

}