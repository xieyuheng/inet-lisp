#include "index.h"

static void
compile_set_variable(worker_t *worker, function_t *function, const char *name) {
    (void) worker;

    size_t index = hash_length(function->local_index_hash);
    if (hash_has(function->local_index_hash, name)) {
        size_t old_index = (size_t) hash_get(function->local_index_hash, name);
        function_add_opcode(function, opcode_set_variable(old_index));
    } else {
        assert(hash_set(function->local_index_hash, string_copy(name), (void *) index));
        function_add_opcode(function, opcode_set_variable(index));
    }
}

void
compile_set_variable_list(worker_t *worker, function_t *function, list_t *name_list) {
    (void) worker;

    char *name = list_last(name_list);
    while (name) {
        compile_set_variable(worker, function, name);
        name = list_prev(name_list);
    }
}

void
compile_exp_list(worker_t *worker, function_t *function, list_t *exp_list) {
    exp_t *exp = list_first(exp_list);
    while (exp) {
        compile_exp(worker, function, exp);
        exp = list_next(exp_list);
    }
}

static bool
maybe_compile_get_variable(function_t *function, const char *name) {
    if (!hash_has(function->local_index_hash, name)) return false;

    size_t index = (size_t) hash_get(function->local_index_hash, name);
    function_add_opcode(function, opcode_get_variable(index));
    return true;
}

static void
compile_literal(worker_t *worker, function_t *function, const char *name) {
    value_t value = mod_find(worker->mod, name);
    if (value == NULL) {
        who_printf("undefined name: %s\n", name);
        who_printf("function:\n");
        function_print(function, stdout);
        exit(1);
    }

    function_add_opcode(function, opcode_literal(value));
}

void
compile_exp(worker_t *worker, function_t *function, exp_t *exp) {
    switch (exp->kind) {
    case EXP_VAR: {
        if (maybe_compile_get_variable(function, exp->var.name))
            return;

        compile_literal(worker, function, exp->var.name);
        return;
    }

    case EXP_AP: {
        compile_exp_list(worker, function, exp->ap.arg_list);
        compile_exp(worker, function, exp->ap.target);
        size_t arity = list_length(exp->ap.arg_list);
        function_add_opcode(function, opcode_apply(arity));
        return;
    }

    case EXP_ASSIGN: {
        compile_exp(worker, function, exp->assign.exp);
        compile_set_variable_list(worker, function, exp->assign.name_list);
        return;
    }

    case EXP_INT: {
        function_add_opcode(function, opcode_literal(xint(exp->i.target)));
        return;
    }

    case EXP_FLOAT: {
        function_add_opcode(function, opcode_literal(xfloat(exp->f.target)));
        return;
    }
    }
}
