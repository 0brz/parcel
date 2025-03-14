#include <parcel.h>
#include <tools/lexer.h>
#include <dump.h>

#include "googletest/googlemock/include/gmock/gmock.h"
#include "googletest/googletest/include/gtest/gtest.h"

struct ParcelTest : public testing::Test {
    parcel::Programm pg;
    void SetUp()
    {
        string src =  parcel::tools::Lexer::read_source_file("/home/gcreep/github.local/parcel_dev/parcel/tests/lang.yml");
        string src_input =  parcel::tools::Lexer::read_source_file("/home/gcreep/github.local/parcel_dev/parcel/tests/source.txt");    
        pg.build(src.c_str());
        pg.run(src_input.c_str());

    };

    void TearDown() {
        //pg.clean();
    };
};

TEST_F(ParcelTest, parcel_build_ok) {
    ASSERT_TRUE(pg.is_builded() == true);
}


int main(int argc, char **argv) {

    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();


        
    parcel::Programm pg;

    string src =  parcel::tools::Lexer::read_source_file("/home/gcreep/github.local/parcel_dev/parcel/tests/lang.yml");
    string src_input =  parcel::tools::Lexer::read_source_file("/home/gcreep/github.local/parcel_dev/parcel/tests/source.txt");

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