#include <map>
#include <string>
#include <vector>
#include <queue>
#include <memory>
#include <algorithm>
#include <ranges>

using namespace std;

bool act_alpha(char a);
bool act_word(string& lex);
bool act_num(string& lex);
bool act_id(string& lex);
bool act_escape_char(const char a);