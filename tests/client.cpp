#include <iostream>

#define PARCEL_DEV
#include <../include/parcel.h>
#include <../include/tools.h>
#include <../src/builder/lextree_build.h>
#include <../src/tools/lexer/lex.h>
#include <../src/builder/parsetree_build.h>
#include <../src/tools/dump.h>

int main()
{
    printf("Client code...\n");
    string sr = lexer::read_source_file("C:/git.local/parsing/bound_parsing/tests/lang.yml");
    LexTree *gt = parcel::build::build_lextree(sr);
    shared_ptr<parcel::parser::ParseCursor> cursor = make_shared<parcel::parser::ParseCursor>(0, 6);
    auto t2 = parcel::build::build_parsetree(gt, cursor);

    /*
    if (gt != NULL)
    {
        tools::dump::lextable(gt);

        instr ins(0, 5);
        ins.build(gt);

        // working with tokenizer.
        std::vector<string> toks{
            "13", "w1", "w2", "a", "!", "_", "12.55", "13.1234", "1235"};
        for (string &s : toks)
        {
            ins.propagate(s);
            ins.move_cursor(1);
        }

        printf("propagate=ok\n");
        parcel::parser::token_hook *k1 = ins.find_hook("ex1");
        if (k1 != NULL)
        {
            tools::dump::hook(k1);
        }

        delete gt;
    }
        */
}