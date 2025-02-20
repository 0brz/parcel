#include <iostream>

#define PARCEL_DEV
#include <../include/parcel.h>
#include <../include/tools.h>
#include <../src/builder/builder.h>
#include <../src/tools/lexer/lex.h>

int main()
{
    printf("Client code...\n");
    string sr = lexer::read_source_file("C:/git.local/parsing/bound_parsing/tests/lang.yml");
    offset_table<lex *> *gt = parcel::builder::build_lex_table(sr);
    if (gt != NULL)
    {
        delete gt;
    }
}