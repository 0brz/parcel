#ifndef _EXPR_TREE_
#define _EXPR_TREE_

#include <iostream>
#include <string>
#include "../functions/funcs.h"

using namespace std;

struct bit_fn_expr {
    bool and1_or0;
    fn_t left;
    fn_t right;

    //bit_fn_expr(bool and_or, const fn_t& left, const fn_t& right) : and1_or0(and_or), left(left), right(right){};
};

struct bit_fn_expr_list {
    bit_fn_expr expr;
    bit_fn_expr_list* next;
};


#endif