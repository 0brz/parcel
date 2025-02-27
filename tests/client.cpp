#include <iostream>

#define PARCEL_DEV
#include <../include/parcel.h>
#include <../include/tools.h>
#include <../src/builder/lex_build.h>
#include <../src/tools/lexer/lex.h>
#include <../src/tools/dump.h>
#include <../src/builder/parse_build.h>

int main()
{
    printf("Client code...\n");
    string sr = lexer::read_source_file("C:/git.local/parsing/bound_parsing/tests/lang.yml");
    offset_table<parcel::build::link_lex *> *gt = parcel::build::build_lex_table(sr);
    if (gt != NULL)
    {
        dump_lex(gt);

        instr ins;
        ins.build(gt);
        token_hook *k1 = ins.find_hook("ex1");
        if (k1 != NULL)
        {
            // dump
        }

        delete gt;
    }
}