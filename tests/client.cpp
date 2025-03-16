#include <parcel.h>
#include <tools/lexer.h>
#include <dump.h>

int main(int argc, char **argv) {
        
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