#ifndef _EXPR_TREE_
#define _EXPR_TREE_

#include <iostream>
#include <string>
#include "../functions/funcs.h"

using namespace std;


template<typename FnPrototype>
struct fn_btree {
    bool and_or0;
    fn_btree* left;
    fn_btree* right;
    FnPrototype* value;

    fn_btree(bool and_or0, fn_btree* left, fn_btree* right) : and_or0(and_or0), left(left), right(right), value(NULL) {};
    fn_btree(FnPrototype* single_fn): value(single_fn),  left(NULL), right(NULL), and_or0(0) {}; 
};

using fn_btree_refs = fn_btree<value_fn_ref>;
using fn_btree_calls = fn_btree<fn_proto>;

struct value_fn_expr_refs : public graph_value {
    inline RULE_TYPE graph_value::get_type() {
        return RULE_TYPE::FN_REF_EXPR;
    };

    fn_btree_refs* fn_tree;
};


#endif