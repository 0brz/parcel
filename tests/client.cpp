#include <iostream>

#define PARCEL_DEV
#include <../include/parcel.h>
#include <../include/tools.h>
#include <../src/builder/lex_build.h>
#include <../src/tools/lexer/lex.h>
#include <../src/tools/dump.h>

int main()
{
    printf("Client code...\n");
    string sr = lexer::read_source_file("C:/git.local/parsing/bound_parsing/tests/lang.yml");
    offset_table<parcel::build::link_lex> *gt = parcel::build::build_lex_table(sr);
    if (gt != NULL)
    {
        dump_lex(*gt);

        delete gt;
    }
}