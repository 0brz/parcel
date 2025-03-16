#include <parcel.h>
#include <tools/lexer.h>
#include <dump.h>

#include "../googletest/googlemock/include/gmock/gmock.h"
#include "../googletest/googletest/include/gtest/gtest.h"

struct LiteralProg : public testing::Test {
    parcel::Programm pg;
    void SetUp()
    {
        string lang =  parcel::tools::Lexer::read_source_file("/home/gcreep/github.local/parcel_dev/parcel/tests/literals/literals.yml");
        string source =  parcel::tools::Lexer::read_source_file("/home/gcreep/github.local/parcel_dev/parcel/tests/literals/literals.txt");    
        
        pg.build(lang.c_str());
        pg.run(source.c_str());

    };

    void TearDown() {
        //pg.clean();
    };
};

TEST_F(LiteralProg, lit_num) {
    auto hook = pg.find_hook("lit_num");
    ASSERT_NE(hook, NULL);

    if (hook != NULL) {
        string vals = parcel::values_to_string(hook, " ");
        string exp("abc-123_@(*)2-- okay _some_id_ v ok250 d 0.0.0.1");
        ASSERT_STREQ(vals.c_str(), exp.c_str());
    }
};

int main(int argc, char **argv) {

    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}