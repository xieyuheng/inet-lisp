#include "index.h"

inline static task_t *
rule_match(const rule_t *rule, principal_wire_t *left, principal_wire_t *right) {
    if ((rule->left_node_ctor == left->node->ctor) &&
        (rule->right_node_ctor == right->node->ctor)) {
        return task_new_active_pair(left, right, rule);
    } else if ((rule->left_node_ctor == right->node->ctor) &&
               (rule->right_node_ctor == left->node->ctor)) {
        return task_new_active_pair(right, left, rule);
    } else {
        return NULL;
    }
}

inline static task_t *
connect_active_pair(principal_wire_t *left, principal_wire_t *right) {
    left->oppsite = right;
    right->oppsite = left;

    for (size_t i = 0; i < array_length(left->node->ctor->rule_array); i++) {
        rule_t *rule = array_get(left->node->ctor->rule_array, i);
        task_t* task = rule_match(rule, left, right);
        if (task) {
            return task;
        }
    }

    for (size_t i = 0; i < array_length(right->node->ctor->rule_array); i++) {
        rule_t *rule = array_get(right->node->ctor->rule_array, i);
        task_t* task = rule_match(rule, left, right);
        if (task) {
            return task;
        }
    }

    return NULL;
}

inline task_t *
fuze(wire_t *wire, value_t value) {
    // expected `wire->atomic_fuzed_value` to be NULL
    value_t expected_fuzed_value = NULL;

    if (atomic_compare_exchange_strong_explicit(
            &wire->atomic_fuzed_value,
            &expected_fuzed_value,
            value,
            memory_order_release,
            memory_order_acquire))
    {
        return NULL;
    }

    else {
        // The `expected_fuzed_value` now stores the value
        // that was in `wire->atomic_fuzed_value`.
        wire_destroy(&wire);
        return connect(expected_fuzed_value, value);
    }
}

inline static task_t *
collect_primitive_arg(principal_wire_t *principal_wire, value_t arg) {
    node_t *node = principal_wire->node;
    assert(node_is_primitive(principal_wire->node));
    primitive_t *primitive = node->ctor->primitive;
    size_t index = principal_wire->index;
    principal_wire_destroy(&principal_wire);

    node_set_value(node, index, arg);
    size_t arg_count = node_primitive_arg_count_fetch_add1(node) + 1;
    if (arg_count == primitive->input_arity) {
        return task_new_primitve(node);
    } else {
        return NULL;
    }
}

task_t *
connect(value_t left, value_t right) {
    if (is_wire(left)) {
        return fuze(as_wire(left), right);
    } else if (is_wire(right)) {
        return fuze(as_wire(right), left);
    } else if (is_principal_wire(left) && is_principal_wire(right)) {
        return connect_active_pair(as_principal_wire(left), as_principal_wire(right));
    } else if (is_principal_wire(left)) {
        return collect_primitive_arg(as_principal_wire(left), right);
    } else if (is_principal_wire(right)) {
        return collect_primitive_arg(as_principal_wire(right), left);
    } else {
        file_lock(stdout);
        who_printf("can not connect non-wire value with non-wire value!\n");
        who_printf("left: "); value_print(left, stdout); printf("\n");
        who_printf("right: "); value_print(right, stdout); printf("\n");
        file_unlock(stdout);
        assert(false);
    }
}
