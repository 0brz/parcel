#ifndef _LEX_
#define _LEX_
#include <iostream>
#include <string>
#include <map>
#include <cctype>
#include <fstream>
#include <iostream>
#include <optional>
#include <string>
#include <vector>
#include <sstream>
#include <cstring>

using namespace std;

namespace parcel {
    namespace tools {
        class Lexer
        {
        private:
            int _cursor;
            string &_src;
            size_t _sz;

        public:
            const short npos = -1;
            const char *symbols_no_id = " @#%^&*(){}[]?><,.";
            const char *symbols_no_id_nospace = "@#%^&*(){}[]?><,.";

            Lexer(string &sr) : _src(sr), _sz(_src.size()), _cursor(0) {};

            inline bool can_read() { return this->_cursor < _sz - 1; };
            inline int can_readn() { return _sz - 1 - this->_cursor; };
            inline int can_readn(int count) { return _sz - 1 - (this->_cursor + count); }

            static string read_source_file(const char *fpath)
            {
                ifstream fs(fpath);
                if (fs.is_open())
                {
                    ostringstream ss;
                    ss << fs.rdbuf();
                    return ss.str();
                }
                else
                {
                    cout << "file bad open\n";
                }
            }

            int cursor_get() { return _cursor; };
            void cursor_move(int delta) { this->_cursor += delta; };
            void cursor_set(int pos) { this->_cursor = pos; };

            char at(size_t offset)
            {
                return _src[offset];
            };

            void set_buff(string &src)
            {
                _src = src;
                _sz = src.size();
            }

            void str(string &out)
            {
                out = string(_src);
            }

            void str_left(size_t offset, size_t left_pad, string &out)
            {
                out = _src.substr(left_pad, offset - 1);
            };

            void str_right(size_t offset, size_t right_pad, string &out)
            {
                out = _src.substr(offset + 1, _sz - offset - 1 - right_pad);
            };

            bool has_next_not_of(size_t ofs, const char *of)
            {
                return _src.find_first_not_of(of, ofs) != string::npos;
            }

            size_t skip(const char *s)
            {
                int ofs = _src.find_first_not_of(s, _cursor);
                size_t old_c = _cursor;
                if (ofs != string::npos)
                {
                    cursor_move(ofs - _cursor);
                }

                return ofs - old_c;
            }

            bool next_symbol(char &s)
            {
                if (can_readn())
                {
                    s = _src[this->_cursor++];
                    return true;
                }
                else
                {
                    return false;
                };
            };

            short next_word(string &out)
            {
                short sz = 0;
                skip(" ");
                int cur = this->_cursor;
                while (cur < _sz)
                {
                    if (_src[cur] >= 'a' && _src[cur] <= 'z')
                    {
                        sz++;
                        cur++;
                    }
                    else
                        break;
                }

                if (sz == 0)
                {
                    return Lexer::npos;
                }

                out = _src.substr(this->_cursor, sz);
                cursor_move(sz);

                return sz;
            }

            short next_id(string &out)
            {
                short sz = 0;
                skip(" ");
                int cur = this->_cursor;
                // printf("_curs=%i\n", cur);
                while (cur < _sz)
                {
                    if ((_src[cur] >= 'a' && _src[cur] <= 'z') ||
                        (_src[cur] >= '0' && _src[cur] <= '9') ||
                        _src[cur] == '_')
                    {
                        sz++;
                        cur++;
                    }
                    else
                        break;
                }

                if (sz == 0)
                {
                    return Lexer::npos;
                }

                out = _src.substr(this->_cursor, sz);
                cursor_move(sz);
                return sz;
            }

            short next_until(const char *untils, string &out)
            {
                size_t _end = _src.find_first_of(untils, _cursor);
                if (_end == string::npos)
                {
                    _end = _sz;
                }

                auto sz(_end - _cursor);
                out = _src.substr(_cursor, sz);
                cursor_move(sz);
                return sz;
            }

            short next_float(string &out)
            {
                // 123.123
                // 123.3
                // 123

                short sz = 0;
                skip(" ");
                int cur = this->_cursor;
                int dotp = 0;
                // printf("_curs=%i\n", cur);
                while (cur < _sz)
                {
                    // printf("_cursor=%c\n", _src[cur]);
                    if ((_src[cur] >= '0' && _src[cur] <= '9'))
                    {
                        sz++;
                        cur++;
                    }
                    else if (_src[cur] == '.')
                    {
                        if (sz == 0)
                        {
                            return Lexer::npos;
                        }
                        else
                        {
                            if (dotp != 0)
                            {
                                break;
                            }
                            else
                            {
                                dotp = cur;
                                sz++;
                                cur++;
                            }
                        }
                    }
                    else
                        break;
                }

                if (dotp == 0)
                {
                    return Lexer::npos;
                }

                if (cur == dotp+1) {
                    return Lexer::npos;
                }

                if (sz == 0)
                {
                    return Lexer::npos;
                }

                out = _src.substr(this->_cursor, sz);
                cursor_move(sz);
                return sz;
            };

            short next_int(string &out)
            {
                short sz = 0;
                skip(" ");
                int cur = this->_cursor;
                int dotp = 0;
                // printf("_curs=%i\n", cur);
                while (cur < _sz)
                {
                    // printf("_cursor=%c\n", _src[cur]);
                    if ((_src[cur] >= '0' && _src[cur] <= '9'))
                    {
                        sz++;
                        cur++;
                    }
                    else
                    {
                        break;
                    }
                }

                if (sz == 0)
                {
                    return Lexer::npos;
                }

                out = _src.substr(this->_cursor, sz);
                cursor_move(sz);
                return sz;
            }

            short next_like(string &out, const char *begins, const char *ends, const char *trash_delims, bool offset = false)
            {
                // or begns or write
                if (begins == NULL &&
                    ends == NULL)
                {
                    return Lexer::npos;
                }

                // trash symbols logic
                if (trash_delims == NULL)
                {
                    trash_delims = Lexer::symbols_no_id;
                }

                if (!can_readn())
                {
                    return -1;
                }

                skip(" ");
                if (begins != NULL && ends != NULL)
                {
                    // begins + ends rule
                    size_t _beg = _src.find(begins, _cursor);
                    short _ofs = (offset == true ? 1 : 0);
                    size_t _end = _src.find(ends, _cursor + _ofs);
                    if (_beg != string::npos &&
                        _end != string::npos)
                    {

                        // check no trash between
                        auto sz(_end - _beg + strlen(ends));
                        string btw = _src.substr(_beg, sz);
                        // printf("___BTW='%s' sz=%zi END=%zi\n", btw.c_str(), sz, _end);
                        if (btw.find_first_of(trash_delims, _beg) == string::npos)
                        {
                            // ok
                            out = _src.substr(_beg, sz);
                            cursor_move(sz);
                            return sz;
                        }
                    }
                }
                else if (begins != NULL)
                {
                    // |app|__&^_le
                    // get until trash symbols finded
                    size_t _beg = _src.find(begins, _cursor);
                    size_t _end = _src.find_first_of(trash_delims, _beg);

                    if (_beg == string::npos)
                    {
                        return Lexer::npos;
                    }

                    if (_end == string::npos)
                    {
                        _end = this->_sz; //
                        printf("____(-1) beg=%zi ends=%zi\n", _beg, _end);
                    }
                    else
                    {
                        printf("____(+) beg=%zi ends=%zi\n", _beg, _end);
                    }

                    // auto sz(_beg-_cursor+strlen(ends));

                    out = _src.substr(_beg, _end - _beg);
                    // cursor_move(_end-_beg);
                    cursor_set(_end);
                }
                else if (ends != NULL)
                {
                    // |app__&^|_le|
                    // if no trash between -> app__&^_le
                    size_t _beg_ends = _src.find(ends, _cursor);
                    auto sz(_beg_ends - _cursor + strlen(ends));
                    string btw = _src.substr(_cursor, sz);

                    size_t _trash_btwn = btw.find_first_of(trash_delims);
                    if (_trash_btwn != string::npos)
                    {
                        return Lexer::npos;
                    }

                    // printf("__curs=%i _ends=%i\n", _cursor, _beg_ends);
                    out = btw;
                    cursor_set(_cursor + sz);

                    return sz;
                }
            }

            short next_like_rounded(string &out, const char *begins, const char *ends, const char *trash_delims)
            {
                short sz = next_like(out, begins, ends, trash_delims, true);
                if (sz == 1)
                {
                    cursor_move(-1);
                    return -1;
                }
            }

            short next_between(string &out, const char *begins, const char *ends)
            {
                // no trash symbols there
                // returs first begins and last ends
                // ret: 'for |<str> deep 1 <str> deep 2 </str> </str>| example data'

                if (begins == NULL &&
                    ends == NULL)
                {
                    return Lexer::npos;
                }

                auto _s = Lexer::first_begins(_src, _cursor, begins);
                if (_s == -1)
                {
                    return Lexer::npos;
                }
                auto _e = Lexer::last_ends(_src, _cursor, ends);
                // printf("__E=%i\n", _e);
                if (_e == -1)
                {
                    return Lexer::npos;
                }

                auto sz(_e - _s + strlen(ends));
                out = _src.substr(_s, sz);
                cursor_move(sz);
                return sz;
            }

            bool check_sequence(const char *char_sequence, const char *delimiters)
            {
                // func(arg...):
                // check( '()', delims=':') -> true
                // check( ')(', delims=':') -> false

                size_t cur = _cursor;
                size_t check_end = _src.find_first_of(delimiters, _cursor);
                if (check_end == string::npos)
                {
                    check_end = _sz - 1;
                };

                string check_str = string(char_sequence);
                size_t offset = 0;
                while (cur <= check_end)
                {

                    if (offset == check_str.size())
                        return true;

                    if (check_str[offset] == _src[cur])
                    {
                        offset++;
                    }

                    cur++;
                };

                return false;
            };

            bool step_next(const char *char_sequence)
            {
                string s(char_sequence);
                int ofs = 0;

                while (can_read())
                {

                    // printf("__STEP_NEXT: cur=%c ex='%s'\n", _src[_cursor], char_sequence);
                    if (ofs == s.size())
                    {
                        // printf("__STEP_NEXT (OUT): cur=%c ex='%s'\n", _src[_cursor], char_sequence);
                        return true;
                    }

                    if (_src[_cursor] == s[ofs])
                    {
                        ofs++;
                        _cursor++;
                    }
                    else
                    {
                        break;
                    }
                }

                cursor_move(-ofs);

                return false;
            }

            // bool check_closed(const char* beg, const char* end, const char* delimiters);

            void get_info(ostream &os)
            {
                if (this->_cursor > 2)
                {
                    string around = this->_src.substr(this->_cursor - 3, 3);
                    os << "Lexer: cs=" << this->_cursor << " '" << around.c_str() << "|'\n";
                }
                else
                    os << "Lexer: cs=" << this->_cursor << "..|...\n";
            }

            void get_cursor_dest(ostream &os)
            {
                if (this->_cursor > 2)
                {
                    string around = this->_src.substr(this->_cursor - 3, 3);
                    os << around.c_str() << "|";
                }
                else
                    os << this->_cursor << "..|...";
            }

            static size_t last_ends(string &src, size_t cursor, const char *ends)
            {

                if (ends == nullptr)
                    return -1;

                int sz = src.size();
                int t = sz;
                int sz_ends = strlen(ends);
                int pos_ends = sz_ends - 1;

                while (t >= cursor)
                {

                    if (pos_ends < 0)
                    {
                        return t + 1;
                    }

                    if (src[t] == ends[pos_ends])
                    {
                        t--;
                        pos_ends--;
                    }
                    else
                    {
                        t--;
                        pos_ends = sz_ends - 1;
                    }
                }

                return -1;
            }

            static size_t first_begins(string &src, size_t cursor, const char *begins)
            {
                if (begins == nullptr)
                    return -1;

                int sz = src.size();
                int t = cursor;
                int sz_ends = strlen(begins);
                int pos_ends = 0;

                while (t < sz)
                {

                    if (pos_ends == sz_ends)
                    {
                        return t - sz_ends;
                    }

                    if (src[t] == begins[pos_ends])
                    {
                        t++;
                        pos_ends++;
                    }
                    else
                    {
                        t++;
                        pos_ends = 0;
                    }
                }

                return -1;
            }
        };
    }
}

#endif