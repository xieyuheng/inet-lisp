#pragma once

typedef enum {
    STMT_DEFINE,
    STMT_DEFINE_FUNCTION,
    STMT_DEFINE_NODE,
    STMT_DEFINE_RULE,
    STMT_DEFINE_RULE_STAR,
    STMT_RUN_EXP,
    STMT_IMPORT,
} stmt_kind_t;

struct stmt_t {
    stmt_kind_t kind;
    union {
        struct { char *name; exp_t *exp; } define;
        struct { char *name; list_t *arg_name_list; list_t *exp_list; } define_function;
        struct { char *name; list_t *port_name_list; } define_node;
        struct { exp_t *pattern_exp; list_t *exp_list; } define_rule;
        struct { list_t *pattern_exp_list; list_t *exp_list; } define_rule_star;
        struct { exp_t *exp; } run_exp;
        struct { list_t *name_list; path_t *path; } import;
    };
};

stmt_t *stmt_define(char *name, exp_t *exp);
stmt_t *stmt_define_function(char *name, list_t *arg_name_list, list_t *exp_list);
stmt_t *stmt_define_node(char *name, list_t *port_name_list);
stmt_t *stmt_define_rule(exp_t *pattern_exp, list_t *exp_list);
stmt_t *stmt_define_rule_star(list_t *pattern_exp_list, list_t *exp_list);
stmt_t *stmt_run_exp(exp_t *exp);
stmt_t *stmt_import(list_t *name_list, path_t *path);

void stmt_destroy(stmt_t **self_pointer);

void stmt_print(const stmt_t *self, file_t *file);
