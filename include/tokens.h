#ifndef _PARSE_TOKEN_H_
#define _PARSE_TOKEN_H_

#include <map>
#include <string>
#include <vector>

using namespace std;

namespace parcel
{
    namespace tokens
    {
        enum type
        {
            word,
            number,
            CHAR,
            ID,

            list,
            vec,
            set,
            seq,

            literal_string,
            literal_char,
            literal_float,
            literal_int
        };

        const char *nameof(type type);

        // struct token;

        struct tvalue
        {
            virtual const char *str() = 0;
        };

        class token
        {
        public:
            type type;
            tvalue *val;
            string *dbg_val;
            // str?
            ~token() {
                delete val;
            }
        };

        struct val_word : tvalue
        {
            string v;
            val_word(string &v) : v(v) {};
            const char *str()
            {
                return v.c_str();
            }
        };

        struct val_string : tvalue
        {
            string v;
            val_string(string &v) : v(v) {};
            const char *str()
            {
                return v.c_str();
            }
        };

        struct val_float : tvalue
        {
            float v;
            val_float(float &v) : v(v) {};
            const char *str()
            {
                return "<float>";
            }
        };

        struct val_int : tvalue
        {
            int v;
            val_int(int &v) : v(v) {};
            const char *str()
            {
                return "<int>";
            }
        };

        struct val_char : tvalue
        {
            string v;
            val_char(char v) : v(1, v) {};
            const char *str()
            {
                return v.c_str();
            }
        };

        struct val_id : tvalue
        {
            string v;
            val_id(string &v) : v(v) {};
            const char *str()
            {
                return v.c_str();
            }
        };

        struct val_num : tvalue
        {
            string v;
            val_num(string &v) : v(v) {};
            const char *str()
            {
                // string s = std::to_string(v);
                return v.c_str();
            }
        };

        struct val_list : tvalue
        {
            std::vector<token *> v;
            val_list(std::vector<token *> &v) : v(v) {};
            val_list() {};
            const char *str()
            {
                return "<list>";
            }

            ~val_list() {
                
            }
        };

        struct val_vector : tvalue
        {
            std::vector<token *> v;
            val_vector(std::vector<token *> &v) : v(v) {};
            const char *str()
            {
                return "<vec>";
            }
        };

        struct val_set : tvalue
        {
            std::vector<token *> v;
            val_set(std::vector<token *> &v) : v(v) {};
            val_set() {};
            const char *str()
            {
                return "<set>";
            }
        };

        struct val_seq : tvalue
        {
            std::vector<token *> v;
            val_seq(std::vector<token *> &v) : v(v) {};
            val_seq() {};
            const char *str()
            {
                return "<seq>";
            }
        };
    }
}

#endif