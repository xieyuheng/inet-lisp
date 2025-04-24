#include "index.h"

static node_pattern_t *
build_node_pattern(worker_t *worker, exp_t *pattern_exp) {
    assert(pattern_exp->kind == EXP_AP);
    exp_t *target = pattern_exp->ap.target;
    list_t *arg_list = pattern_exp->ap.arg_list;

    assert(target->kind == EXP_VAR);
    value_t value = mod_find(worker->mod, target->var.name);
    if (!value) {
        who_printf("undefined node: %s\n", target->var.name);
        exit(1);
    }

    node_ctor_t *node_ctor = as_node_ctor(value);
    node_pattern_t *node_pattern = node_pattern_new(node_ctor);

    exp_t *arg_exp = list_first(arg_list);
    size_t index = 0;
    while (arg_exp) {
        assert(arg_exp->kind == EXP_VAR);
        char *name = string_copy(arg_exp->var.name);
        port_info_t *port_info = port_info_from_name(name);
        node_pattern_set_port_info(node_pattern, index, port_info);
        arg_exp = list_next(arg_list);
        index++;
    }

    return node_pattern;
}

static list_t *
build_node_pattern_list(worker_t *worker, list_t *pattern_exp_list) {
    list_t *node_pattern_list =
        list_new_with((destroy_fn_t *) node_pattern_destroy);
    exp_t *pattern_exp = list_first(pattern_exp_list);
    while (pattern_exp) {
        list_push(node_pattern_list, build_node_pattern(worker, pattern_exp));
        pattern_exp = list_next(pattern_exp_list);
    }

    return node_pattern_list;
}

static void
translate_pattern_sub_tree(
    worker_t *worker,
    exp_t *pattern_exp,
    const char *last_arg_name,
    list_t *pattern_exp_list
);

static list_t *
translate_pattern_arg_list(
    worker_t *worker,
    list_t *arg_list,
    list_t *pattern_exp_list
) {
    list_t *new_arg_list = exp_list_new();
    exp_t *arg_exp = list_first(arg_list);
    while (arg_exp) {
        if (arg_exp->kind == EXP_VAR) {
            list_push(new_arg_list, exp_copy(arg_exp));
        } else {
            char *name = string_copy("!");
            translate_pattern_sub_tree(worker, arg_exp, name, pattern_exp_list);
            list_push(new_arg_list, exp_var(name));
        }

        arg_exp = list_next(arg_list);
    }

    return new_arg_list;
}

static void
translate_pattern_sub_tree(
    worker_t *worker,
    exp_t *pattern_exp,
    const char *last_arg_name,
    list_t *pattern_exp_list
) {
    assert(pattern_exp->kind == EXP_AP);
    exp_t *target = exp_copy(pattern_exp->ap.target);
    list_t *arg_list = translate_pattern_arg_list(worker, pattern_exp->ap.arg_list, pattern_exp_list);
    list_push(arg_list, exp_var(string_copy(last_arg_name)));
    list_unshift(pattern_exp_list, exp_ap(target, arg_list));
}

static list_t *
translate_pattern_tree(worker_t *worker, exp_t *pattern_exp) {
    assert(pattern_exp->kind == EXP_AP);
    exp_t *target = exp_copy(pattern_exp->ap.target);
    list_t *pattern_exp_list = exp_list_new();
    list_t *arg_list = translate_pattern_arg_list(worker, pattern_exp->ap.arg_list, pattern_exp_list);
    list_unshift(pattern_exp_list, exp_ap(target, arg_list));
    return pattern_exp_list;
}

void
execute(worker_t *worker, stmt_t *stmt) {
    switch (stmt->kind) {
    case STMT_DEFINE: {
        run_exp(worker, stmt->define.exp);
        value_t value = stack_pop(worker->value_stack);
        define(worker->mod, stmt->define.name, value);
        return;
    }

    case STMT_DEFINE_FUNCTION: {
        size_t arity = list_length(stmt->define_function.arg_name_list);
        function_t *function = function_new(arity);
        function->name = string_copy(stmt->define_function.name);
        compile_set_variable_list(worker, function, stmt->define_function.arg_name_list);
        compile_exp_list(worker, function, stmt->define_function.exp_list);
        define(worker->mod, stmt->define_function.name, function);
        return;
    }

    case STMT_DEFINE_NODE: {
        define_node(worker, stmt->define_node.name, stmt->define_node.port_name_list);
        return;
    }

    case STMT_DEFINE_RULE: {
        list_t *pattern_exp_list = translate_pattern_tree(worker, stmt->define_rule.pattern_exp);
        define_rule_star(
            worker,
            build_node_pattern_list(worker, pattern_exp_list),
            stmt->define_rule.exp_list);
        list_destroy(&pattern_exp_list);
        return;
    }

    case STMT_DEFINE_RULE_STAR: {
        list_t *pattern_exp_list = stmt->define_rule_star.pattern_exp_list;
        define_rule_star(
            worker,
            build_node_pattern_list(worker, pattern_exp_list),
            stmt->define_rule_star.exp_list);
        return;
    }

    case STMT_RUN_EXP: {
        run_exp(worker, stmt->run_exp.exp);
        return;
    }

    case STMT_IMPORT: {
        path_t *path = path_copy(worker->mod->path);
        path_join(path, "..");
        path_join(path, path_string(stmt->import.path));
        mod_t *imported_mod = load_mod(path);
        char *name = list_first(stmt->import.name_list);
        while (name) {
            value_t value = mod_find(imported_mod, name);
            if (value == NULL) {
                who_printf("can not import unknown name: %s", name);
                stmt_print(stmt, stdout);
                exit(1);
            }

            define(worker->mod, name, value);
            name = list_next(stmt->import.name_list);
        }

        return;
    }
    }
}
