#include "index.h"

static stmt_t *
parse_define_node(sexp_t *sexp) {
    list_t *sexp_list = sexp_sexp_list(sexp);
    (void) list_first(sexp_list);
    sexp_t *name_sexp = list_next(sexp_list);
    char *name = string_copy(sexp_string(name_sexp));
    list_t *port_name_list = string_make_list();
    sexp_t *port_name_sexp = list_next(sexp_list);
    while (port_name_sexp) {
        list_push(port_name_list, string_copy(sexp_string(port_name_sexp)));
        port_name_sexp = list_next(sexp_list);
    }

    return stmt_define_node(name, port_name_list);
}

static stmt_t *
parse_define_value(sexp_t *sexp) {
    list_t *sexp_list = sexp_sexp_list(sexp);
    (void) list_first(sexp_list);

    sexp_t *name_sexp = list_next(sexp_list);
    char *name = string_copy(sexp_string(name_sexp));

    sexp_t *exp_sexp = list_next(sexp_list);
    assert(list_next(sexp_list) == NULL);
    exp_t *exp = parse_exp(exp_sexp);

    return stmt_define(name, exp);
}

static stmt_t *
parse_define_function(sexp_t *sexp) {
    list_t *sexp_list = sexp_sexp_list(sexp);
    (void) list_first(sexp_list);

    list_t *name_sexp_list = sexp_sexp_list(list_next(sexp_list));
    sexp_t *first_name_sexp = list_first(name_sexp_list);
    char *name = string_copy(sexp_string(first_name_sexp));

    list_t *arg_name_list = string_make_list();
    sexp_t *name_sexp = list_next(name_sexp_list);
    while (name_sexp) {
        list_push(arg_name_list, string_copy(sexp_string(name_sexp)));
        name_sexp = list_next(name_sexp_list);
    }

    list_t *exp_list = exp_make_list();
    sexp_t *exp_sexp = list_next(sexp_list);
    while (exp_sexp) {
        list_push(exp_list, parse_exp(exp_sexp));
        exp_sexp = list_next(sexp_list);
    }

    return stmt_define_function(name, arg_name_list, exp_list);
}

static stmt_t *
parse_define(sexp_t *sexp) {
    list_t *sexp_list = sexp_sexp_list(sexp);
    sexp_t *second_sexp = list_get(sexp_list, 1);
    if (is_atom_sexp(second_sexp))
        return parse_define_value(sexp);
    else
        return parse_define_function(sexp);
}

static stmt_t *
parse_define_rule(sexp_t *sexp) {
    list_t *sexp_list = sexp_sexp_list(sexp);
    (void) list_first(sexp_list);

    exp_t *pattern_exp = parse_exp(list_next(sexp_list));

    list_t *exp_list = exp_make_list();
    sexp_t *exp_sexp = list_next(sexp_list);
    while (exp_sexp) {
        list_push(exp_list, parse_exp(exp_sexp));
        exp_sexp = list_next(sexp_list);
    }

    return stmt_define_rule(pattern_exp, exp_list);
}

static stmt_t *
parse_define_rule_star(sexp_t *sexp) {
    list_t *sexp_list = sexp_sexp_list(sexp);
    (void) list_first(sexp_list);

    list_t *pattern_exp_list = exp_make_list();
    list_t *pattern_exp_sexp_list = sexp_sexp_list(list_next(sexp_list));
    sexp_t *pattern_exp_sexp = list_first(pattern_exp_sexp_list);
    while (pattern_exp_sexp) {
        list_push(pattern_exp_list, parse_exp(pattern_exp_sexp));
        pattern_exp_sexp = list_next(pattern_exp_sexp_list);
    }

    list_t *exp_list = exp_make_list();
    sexp_t *exp_sexp = list_next(sexp_list);
    while (exp_sexp) {
        list_push(exp_list, parse_exp(exp_sexp));
        exp_sexp = list_next(sexp_list);
    }

    return stmt_define_rule_star(pattern_exp_list, exp_list);
}

static stmt_t *
parse_run_exp(sexp_t *sexp) {
    return stmt_run_exp(parse_exp(sexp));
}

static stmt_t *
parse_import(sexp_t *sexp) {
    list_t *sexp_list = sexp_sexp_list(sexp);
    (void) list_first(sexp_list);
    list_t *name_list = string_make_list();
    sexp_t *name_sexp = list_next(sexp_list);
    path_t *path = NULL;
    while (name_sexp) {
        if (list_cursor_is_end(sexp_list))
            path = make_path(sexp_token(name_sexp)->string);
        else
            list_push(name_list, string_copy(sexp_string(name_sexp)));

        name_sexp = list_next(sexp_list);
    }


    return stmt_import(name_list, path);
}

stmt_t *
parse_stmt(sexp_t *sexp) {
    if (sexp_starts_with(sexp, "define-node"))
        return parse_define_node(sexp);
    if (sexp_starts_with(sexp, "define-rule"))
        return parse_define_rule(sexp);
    if (sexp_starts_with(sexp, "define-rule*"))
        return parse_define_rule_star(sexp);
    else if (sexp_starts_with(sexp, "define"))
        return parse_define(sexp);
    else if (sexp_starts_with(sexp, "import"))
        return parse_import(sexp);
    else
        return parse_run_exp(sexp);
}

list_t *
parse_stmt_list(list_t *sexp_list) {
    list_t *stmt_list = make_list_with((destroy_fn_t *) stmt_destroy);
    sexp_t *sexp = list_first(sexp_list);
    while (sexp) {
        list_push(stmt_list, parse_stmt(sexp));
        sexp = list_next(sexp_list);
    }

    return stmt_list;
}
