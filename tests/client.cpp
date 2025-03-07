#include <iostream>

#define PARCEL_DEV
#include <../include/parcel.h>
#include <../include/tools.h>
#include <../src/builder/lextree_build.h>
#include <../src/tools/lexer/lex.h>
#include <../src/builder/parseinstr_build.h>
#include <../src/tools/dump.h>
#include <../src/tools/log.h>

#include <../src/input/tokenizator.h>

int main()
{

    printf("Client code...\n");

        /*
    string sr = lexer::read_source_file("/home/gcreep/github.local/parcel_dev/parcel/tests/source.txt");
    parcel::input::Tokenizator tz(sr);
    
    string cur_token;
    while(tz.next_token(cur_token)) {
        printf("cur_token=%s\n", cur_token.c_str());
    }
        */

    printf("end\n");

    //std::cout << "\033[1;31mbold red text\033[0m\n" << endl;

       
    string sr = lexer::read_source_file("/home/gcreep/github.local/parcel_dev/parcel/tests/lang.yml");
    LexTree *gt = parcel::build::build_lextree(sr);
    shared_ptr<parcel::parser::ParseCursor> cursor = make_shared<parcel::parser::ParseCursor>(0, 6);
    Instr* ins = parcel::build::build_parseinstr(gt);
    
    delete gt;
    delete ins;
        
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


        ///////////////////
        void propagate(string &lex)
            {
                for (prog_go *e : reg_entries)
                {
                    e->act(lex, NULL, NULL);
                }
            };

            void move_cursor(size_t to)
            {
                (*cursor).pos += to;
            };

            token_hook *find_hook(string name)
            {
                auto f = map_hooks.find(name);
                if (f != end(map_hooks))
                {
                    return (*f).second;
                }
                else
                    return NULL;
            };
    }
        */
}