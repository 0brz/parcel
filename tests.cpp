/*
    
    void get_def_hooks(string& sr) {
        lexer lx(sr);

        string cur;
        while(lx.next_like(cur, nullptr,"$", ": ") != lx.npos) {
            //printf("[def_instr] %s\n", cur.c_str());
            lx.get_info(cout);
            lx.cursor_move(1);
        } 
    }

    get_hooks_defs_at_begin
    " $go: $block_   :   $instr124"

    get_hooks_defs_at_end
    "go$: block_$ :  instr124:$ "


    -----------
    "go$: 123*25 block_$ :  instr124:$ "; // 
    void get_ids(string& sr) {
        lexer lx(sr);

        string cur;
        while(lx.can_read()) {
            if (lx.next_id(cur) != lx.npos) {
                printf("[id] %s\n", cur.c_str());
            }
            else {
                //lx.cursor_move(1);
                // or
                char t;
                lx.next_symbol(t);
                //lx.go_back(1);
            }
        }
    }
    -----------
    btwn tags =     lx.next_like(cur, "<",  ">", "");

    -----------
    void get_literals() {
        //string litrs = "tagval: tag: \"apple\" val: \"good\" volume: 123.12478"; // 
        string litrs = "tagval: tag: \"apple\" val: \"good\" volume: 123.12478"; // 
        lexer lx(litrs);
        string cur;

        // we should parse tags and get next format 'word in brackets | float'

        while(lx.can_read()) {
            char delim;
            // find tag -> tag=word + :
            // lx.get_info(cout);
            short word_len = 0;
            if ((word_len = lx.next_id(cur)) != lx.npos &&
                lx.next_symbol(delim) != lx.npos) { // or use next_like with ':'
                if (delim == ':') {
                    // this is a tag.
                    // 
                    cur += delim;
                    printf("[tag] %s\n", cur.c_str());
                }
                else {
                    lx.cursor_move(-1);
                    lx.cursor_move(-word_len);
                    //lx.
                }
            }
            
            // this is not a our rule.
            // maybe this is a literal
                
            // word litr
            if (lx.next_like_rounded(cur, "\"",  "\"", "") != lx.npos) {
                printf("[litr.word] '%s'\n", cur.c_str());
            }
            // float litr
            else if (lx.next_float(cur) != lx.npos) {
                printf("[litr.float] '%s'\n", cur.c_str());
            }
            else  lx.cursor_move(1);
        }
    }

    ------------------------------------
    void get_literals() {
        //string litrs = "tagval: tag: \"apple\" val: \"good\" volume: 123.12478"; // 
        string litrs = "tagval: tag: \"apple\" val: \"good\" volume: 123.12478"; // 
        lexer lx(litrs);
        string cur;

        // we should parse tags and get next format 'word in brackets | float'

        while(lx.can_read()) {
            char delim;
            // find tag -> tag=word + :
            // lx.get_info(cout);
            short word_len = 0;
            if ((word_len = lx.next_id(cur)) != lx.npos &&
                lx.next_symbol(delim) != lx.npos) { // or use next_like with ':'
                if (delim == ':') {
                    // this is a tag.
                    // 
                    cur += delim;
                    printf("[tag] %s\n", cur.c_str());
                }
                else {
                    lx.cursor_move(-1);
                    lx.cursor_move(-word_len);
                    //lx.
                }
            }
            
            // this is not a our rule.
            // maybe this is a literal
                
            // word litr
            if (lx.next_like_rounded(cur, "\"",  "\"", "") != lx.npos) {
                printf("[litr.word] '%s'\n", cur.c_str());
            }
            // float litr
            else if (lx.next_float(cur) != lx.npos) {
                printf("[litr.float] '%s'\n", cur.c_str());
            }
            else  lx.cursor_move(1);
        }
    }
*/