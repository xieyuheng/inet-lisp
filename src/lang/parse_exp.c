#include "index.h"

static exp_t *
parse_var(sexp_t *sexp) {
    return exp_var(string_copy(sexp_string(sexp)));
}

static exp_t *
parse_int(sexp_t *sexp) {
    return exp_int(string_parse_xint(sexp_string(sexp)));
}

static exp_t *
parse_float(sexp_t *sexp) {
    return exp_float(string_parse_double(sexp_string(sexp)));
}

static exp_t *
parse_ap(sexp_t *sexp) {
    list_t *sexp_list = sexp_sexp_list(sexp);
    assert(!list_is_empty(sexp_list));
    exp_t *target = parse_exp(list_first(sexp_list));
    list_t *arg_list = exp_make_list();
    sexp_t *arg_sexp = list_next(sexp_list);
    while (arg_sexp) {
        list_push(arg_list, parse_exp(arg_sexp));
        arg_sexp = list_next(sexp_list);
    }

    return exp_ap(target, arg_list);
}

static exp_t *
parse_assign(sexp_t *sexp) {
    list_t *sexp_list = sexp_sexp_list(sexp);
    (void) list_first(sexp_list);
    list_t *name_list = string_make_list();
    sexp_t *name_sexp = list_next(sexp_list);
    exp_t *exp = NULL;
    while (name_sexp) {
        if (list_cursor_is_end(sexp_list))
            exp = parse_exp(name_sexp);
        else
            list_push(name_list, string_copy(sexp_string(name_sexp)));

        name_sexp = list_next(sexp_list);
    }

    return exp_assign(name_list, exp);
}

exp_t *
parse_exp(sexp_t *sexp) {
    if (sexp_starts_with(sexp, "=") || sexp_starts_with(sexp, "assign")) {
        return parse_assign(sexp);
    } else if (is_atom_sexp(sexp)) {
        const token_t *token = sexp_token(sexp);
        if (token->kind == INT_TOKEN)
            return parse_int(sexp);
        else if (token->kind == FLOAT_TOKEN)
            return parse_float(sexp);
        else
            return parse_var(sexp);
    } else if (!list_is_empty(sexp_sexp_list(sexp))) {
        return parse_ap(sexp);
    } else {
        assert(false && "[parse_exp] can not handle empty list sexp");
    }
}
