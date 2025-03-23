#include <parcel.h>
#include <tools/lexer.h>
#include <dump.h>

#include "../googletest/googlemock/include/gmock/gmock.h"
#include "../googletest/googletest/include/gtest/gtest.h"

struct CollectionProg : public testing::Test {
    parcel::Programm pg;
    void SetUp()
    {
        string lang =  parcel::tools::Lexer::read_source_file("/home/gcreep/github.local/parcel_dev/parcel/tests/collections/collections.yml");
        string source =  parcel::tools::Lexer::read_source_file("/home/gcreep/github.local/parcel_dev/parcel/tests/collections/collections.txt");    
        
        pg.build(lang.c_str());
        pg.run(source.c_str());

    };

    void TearDown() {
        //pg.clean();
    };
};

TEST_F(CollectionProg, fn_with_params) {
    auto hook = pg.find_hook("fn_with_params");
    ASSERT_NE(hook, NULL);

    if (hook != NULL) {
        string vals = parcel::values_to_string(hook, " ");
        string exp("fn add_one ( usize )");
        ASSERT_STREQ(vals.c_str(), exp.c_str());
    }
};

TEST_F(CollectionProg, fn_no_params) {
    auto hook = pg.find_hook("fn_no_params");
    ASSERT_NE(hook, NULL);

    if (hook != NULL) {
        string vals = parcel::values_to_string(hook, " ");
        string exp("fn example ( )");
        ASSERT_STREQ(vals.c_str(), exp.c_str());
    }
};

int main(int argc, char **argv) {

    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}