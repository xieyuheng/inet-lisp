#include "index.h"

void
define(mod_t *self, const char *name, value_t value) {
    assert(hash_set(self->value_hash, string_copy(name), value));
}

void
define_rule(mod_t *self, const char *name, rule_t *rule) {
    value_t value = mod_find(self, name);
    node_ctor_t *node_ctor = as_node_ctor(value);
    array_push(node_ctor->rule_array, rule);
}

void
define_rule_star(worker_t *worker, list_t *node_pattern_list, list_t *exp_list) {
    assert(list_length(node_pattern_list) == 2);
    node_pattern_t *left_node_pattern = list_first(node_pattern_list);
    node_pattern_t *right_node_pattern = list_next(node_pattern_list);

    list_t *local_name_list = list_new();

    for (size_t i = 0; i < left_node_pattern->ctor->arity; i++) {
        port_info_t *port_info = left_node_pattern->port_infos[i];
        if (!port_info->is_principal) {
            list_push(local_name_list, port_info->name);
        }
    }

    for (size_t i = 0; i < right_node_pattern->ctor->arity; i++) {
        port_info_t *port_info = right_node_pattern->port_infos[i];
        if (!port_info->is_principal) {
            list_push(local_name_list, port_info->name);
        }
    }

    size_t arity = list_length(local_name_list);
    function_t *function = function_new(arity);

    compile_set_variable_list(worker, function, local_name_list);
    compile_exp_list(worker, function, exp_list);

    rule_t *rule = rule_new(left_node_pattern->ctor, right_node_pattern->ctor, function);
    define_rule(worker->mod, left_node_pattern->ctor->name, rule);
    define_rule(worker->mod, right_node_pattern->ctor->name, rule);
    return;
}

void
define_node(worker_t *worker, const char *name, list_t *port_name_list) {
    size_t arity = list_length(port_name_list);
    node_ctor_t *node_ctor = node_ctor_new(name, arity);

    size_t index = 0;
    char *port_name = list_first(port_name_list);
    while (port_name) {
        node_ctor->port_infos[index] =
            port_info_from_name(string_copy(port_name));
        port_name = list_next(port_name_list);
        index++;
    }

    define(worker->mod, name, node_ctor);
    return;
}

void
define_primitive_fn(mod_t *mod, const char *name, size_t input_arity, size_t output_arity, primitive_fn_t *primitive_fn) {
    define(mod, name, primitive_from_fn(name, input_arity, output_arity, primitive_fn));
}

void
define_primitive_fn_0(mod_t *mod, const char *name, primitive_fn_0_t *primitive_fn_0) {
    define(mod, name, primitive_from_fn_0(name, primitive_fn_0));
}

void
define_primitive_fn_1(mod_t *mod, const char *name, primitive_fn_1_t *primitive_fn_1) {
    define(mod, name, primitive_from_fn_1(name, primitive_fn_1));
}

void
define_primitive_fn_2(mod_t *mod, const char *name, primitive_fn_2_t *primitive_fn_2) {
    define(mod, name, primitive_from_fn_2(name, primitive_fn_2));
}

void
define_primitive_fn_3(mod_t *mod, const char *name, primitive_fn_3_t *primitive_fn_3) {
    define(mod, name, primitive_from_fn_3(name, primitive_fn_3));
}

void
define_primitive_fn_4(mod_t *mod, const char *name, primitive_fn_4_t *primitive_fn_4)  {
    define(mod, name, primitive_from_fn_4(name, primitive_fn_4));
}

void
define_primitive_node(mod_t *mod, const char *name, const char *port_names[]) {
    value_t value = mod_find(mod, name);
    if (!is_primitive(value)) {
        who_printf("expect value of name: %s to be a primitive, instead of: \n", name);
        value_print(value, stdout);
        exit(1);
    }

    primitive_t *primitive = as_primitive(value);
    size_t arity = primitive->input_arity + primitive->output_arity;
    node_ctor_t *node_ctor = node_ctor_new(name, arity);
    for (size_t i = 0; i < arity; i++) {
        assert(port_names[i]);
        node_ctor->port_infos[i] =
            port_info_from_name(string_copy(port_names[i]));
    }

    primitive->node_ctor = node_ctor;
    node_ctor->primitive = primitive;
}

void
define_primitive_fn_node(mod_t *mod, const char *name, size_t input_arity, size_t output_arity, primitive_fn_t *primitive_fn, const char *port_names[]) {
    define_primitive_fn(mod, name, input_arity, output_arity, primitive_fn);
    define_primitive_node(mod, name, port_names);
}

void
define_primitive_fn_node_0(mod_t *mod, const char *name, primitive_fn_0_t *primitive_fn_0, const char *port_names[]) {
    define_primitive_fn_0(mod, name, primitive_fn_0);
    define_primitive_node(mod, name, port_names);
}

void
define_primitive_fn_node_1(mod_t *mod, const char *name, primitive_fn_1_t *primitive_fn_1, const char *port_names[]) {
    define_primitive_fn_1(mod, name, primitive_fn_1);
    define_primitive_node(mod, name, port_names);
}

void
define_primitive_fn_node_2(mod_t *mod, const char *name, primitive_fn_2_t *primitive_fn_2, const char *port_names[]) {
    define_primitive_fn_2(mod, name, primitive_fn_2);
    define_primitive_node(mod, name, port_names);
}

void
define_primitive_fn_node_3(mod_t *mod, const char *name, primitive_fn_3_t *primitive_fn_3, const char *port_names[]) {
    define_primitive_fn_3(mod, name, primitive_fn_3);
    define_primitive_node(mod, name, port_names);
}

void
define_primitive_fn_node_4(mod_t *mod, const char *name, primitive_fn_4_t *primitive_fn_4, const char *port_names[]) {
    define_primitive_fn_4(mod, name, primitive_fn_4);
    define_primitive_node(mod, name, port_names);
}
