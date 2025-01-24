#ifndef _EXPR_TREE_
#define _EXPR_TREE_

#include "../functions/funcs.h"

struct bit_expr_fn {
    bool and1_or0;
    fn_t left;
    fn_t right;

    bit_expr_fn(bool and_or, const fn_t& left, const fn_t& right) : and1_or0(and_or), left(left), right(right){};
};


#endif