#ifndef _FUNCS_
#define _FUNCS_

enum fn_type {
    gt,
    egt,
    less,
    eless,
};

struct fn_value_basic {};

struct fn_gt : public fn_value_basic {
    float val;
    fn_gt(float val): val(val){};
};

struct fn_less : public fn_value_basic {
    float val;
    fn_less(float val): val(val){};
};

struct fn_t {
    fn_type type;
    fn_value_basic* val;
    fn_t(fn_value_basic* val, fn_type type) : val(val), type(type){};
};

struct fn_list {
    fn_t fn;
    fn_list* next;

    fn_list(fn_value_basic* val, fn_type type) : fn(val, type), next(nullptr){};
    fn_list(fn_t fn, fn_list* next): fn(fn), next(next) {};
};



// ---------------------
//      NUMBERS

#endif
