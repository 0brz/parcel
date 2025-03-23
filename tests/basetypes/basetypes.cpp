#include <parcel.h>
#include <tools/lexer.h>
#include <dump.h>

#include "../googletest/googlemock/include/gmock/gmock.h"
#include "../googletest/googletest/include/gtest/gtest.h"

struct SingleValue_set : public testing::Test {
    parcel::Programm pg;
    void SetUp()
    {
        string lang =  parcel::tools::Lexer::read_source_file("/home/gcreep/github.local/parcel_dev/parcel/tests/basetypes/basetypes.yml");
        string source =  parcel::tools::Lexer::read_source_file("/home/gcreep/github.local/parcel_dev/parcel/tests/basetypes/basetypes.txt");    
        
        pg.build(lang.c_str());
        pg.run(source.c_str());

    };

    void TearDown() {
        //pg.clean();
    };
};

TEST_F(SingleValue_set, id_hook) {
    auto hook = pg.find_hook("id_hook");
    ASSERT_NE(hook, NULL);

    if (hook != NULL) {
        string vals = parcel::values_to_string(hook, " ");
        string exp("id1 _id3 _guid-11-22-33-44_ apple Island PROTOTYPE CocaCola A a b B");
        ASSERT_STREQ(vals.c_str(), exp.c_str());
    }
};

TEST_F(SingleValue_set, word_is_ok) {
    auto hook = pg.find_hook("word_hook");
    ASSERT_NE(hook, NULL);

    if (hook != NULL) {
        string vals = parcel::values_to_string(hook, " ");
        string exp("apple Island PROTOTYPE CocaCola A a b B");
        ASSERT_STREQ(vals.c_str(), exp.c_str());
    }
};

TEST_F(SingleValue_set,   num_is_ok) {
    auto hook = pg.find_hook("num_hook");
    ASSERT_NE(hook, NULL);

    if (hook != NULL) {
        string vals = parcel::values_to_string(hook, " ");
        string exp("1 22 333");
        ASSERT_STREQ(vals.c_str(), exp.c_str());
    }
};

TEST_F(SingleValue_set,   char_is_ok) {
    auto hook = pg.find_hook("char_hook");
    ASSERT_NE(hook, NULL);

    if (hook != NULL) {
        string vals = parcel::values_to_string(hook, " ");
        string exp("1 A a b B");
        ASSERT_STREQ(vals.c_str(), exp.c_str());
    }
};

int main(int argc, char **argv) {

    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}