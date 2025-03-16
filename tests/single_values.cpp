#include <parcel.h>
#include <tools/lexer.h>
#include <dump.h>

#include "googletest/googlemock/include/gmock/gmock.h"
#include "googletest/googletest/include/gtest/gtest.h"

struct SingleValue_set : public testing::Test {
    parcel::Programm pg;
    void SetUp()
    {
        string lang =  parcel::tools::Lexer::read_source_file("/home/gcreep/github.local/parcel_dev/parcel/tests/single_values.yml");
        string source =  parcel::tools::Lexer::read_source_file("/home/gcreep/github.local/parcel_dev/parcel/tests/single_values.txt");    
        
        pg.build(lang.c_str());
        pg.run(source.c_str());

    };

    void TearDown() {
        //pg.clean();
    };
};

TEST_F(SingleValue_set, build_ok) {
    ASSERT_TRUE(pg.is_builded() == true);
};

TEST_F(SingleValue_set, word_is_ok) {
    auto hook = pg.find_hook("word_hook");
    ASSERT_NE(hook, NULL);

    if (hook != NULL) {
        string vals = parcel::values_to_string(hook, " ");
        string exp("good Q A B");
        ASSERT_STREQ(vals.c_str(), exp.c_str());
    }
};

TEST_F(SingleValue_set,   num_is_ok) {
    auto hook = pg.find_hook("num_hook");
    ASSERT_NE(hook, NULL);

    if (hook != NULL) {
        string vals = parcel::values_to_string(hook, " ");
        string exp("250");
        ASSERT_STREQ(vals.c_str(), exp.c_str());
    }
};

TEST_F(SingleValue_set,   char_is_ok) {
    auto hook = pg.find_hook("char_hook");
    ASSERT_NE(hook, NULL);

    if (hook != NULL) {
        string vals = parcel::values_to_string(hook, " ");
        string exp("Q A B");
        ASSERT_STREQ(vals.c_str(), exp.c_str());
    }
};


int main(int argc, char **argv) {

    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}