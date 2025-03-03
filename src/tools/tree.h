#ifndef _TOOLS_TREE_H_
#define _TOOLS_TREE_H_

#include <vector>

template <typename TypeOf>
struct TreeValue
{
    TypeOf val;
    std::vector<TreeValue *> entries; // maybe fix to small_vec
    TreeValue(TypeOf &v) : val(v), entries{} {};
};

#endif