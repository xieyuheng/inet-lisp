#pragma once

typedef enum {
    EXP_VAR,
    EXP_AP,

    EXP_ASSIGN,
    // (assign <name> ... <exp>)
    // (= <name> ... <exp>)

    EXP_INT,
    EXP_FLOAT,
} exp_kind_t;

struct exp_t {
    exp_kind_t kind;
    union {
        struct { char *name; } var;
        struct { exp_t *target; list_t *arg_list; } ap;
        struct { list_t *name_list; exp_t *exp; } assign;
        struct { int64_t target; } i;
        struct { double target; } f;
    };
};

exp_t *exp_var(char *name);
exp_t *exp_ap(exp_t *target, list_t *arg_list);
exp_t *exp_assign(list_t *name_list, exp_t *exp);
exp_t *exp_int(int64_t target);
exp_t *exp_float(double target);

list_t *exp_make_list(void);

void exp_destroy(exp_t **self_pointer);

exp_t *exp_copy(const exp_t *self);
list_t *exp_list_copy(list_t *exp_list);

void name_list_print(list_t *name_list, file_t *file);
void exp_list_print(list_t *exp_list, file_t *file);
void exp_list_print_as_tail(list_t *exp_list, file_t *file);
void exp_list_print_as_list(list_t *exp_list, file_t *file);
void exp_print(const exp_t *self, file_t *file);
