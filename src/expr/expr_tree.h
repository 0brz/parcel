#ifndef _EXPR_TREE_
#define _EXPR_TREE_

#include <iostream>
#include <string>
#include "../functions/funcs.h"

using namespace std;

struct fn_expr_tree : public graph_value {
    inline RULE_TYPE graph_value::get_type() {
        return RULE_TYPE::FN_EXPR;
    };

    fn_bit_tree* fn_tree;
};

struct fn_bit_tree {
    bool and_or0;
    fn_bit_tree* left;
    fn_bit_tree* right;
    fn_proto* value;

    fn_bit_tree(bool and_or0, fn_bit_tree* left, fn_bit_tree* right) : and_or0(and_or0), left(left), right(right), value(NULL) {};
    fn_bit_tree(fn_proto* single_fn): value(single_fn),  left(NULL), right(NULL), and_or0(0) {}; 
};

#endif