#include "index.h"

static task_t *
rule_match(const rule_t *rule, wire_t *left, wire_t *right) {
    assert(rule->left_node_ctor);
    assert(rule->right_node_ctor);

    if ((rule->left_node_ctor == left->node->ctor) &&
        (rule->right_node_ctor == right->node->ctor)) {
        return task_new(left, right, rule);
    } else if ((rule->left_node_ctor == right->node->ctor) &&
               (rule->right_node_ctor == left->node->ctor)) {
        return task_new(right, left, rule);
    } else {
        return NULL;
    }
}

static task_t *
connect_active_pair(wire_t *left, wire_t *right) {
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

void
worker_maybe_add_active_wire(worker_t *worker, value_t value) {
    if (is_wire(value)) {
        wire_t *left = as_wire(value);
        if (!wire_is_principal(left)) return;

        value_t right_value = left->opposite;
        if (!right_value) return;
        if (is_wire(right_value)) {
            wire_t *right = as_wire(right_value);
            if (!wire_is_principal(right)) return;

            task_t *task = connect_active_pair(left, right);
            if (task) {
                list_push(worker->task_list, task);
            }
        }
    }
}

static void
reconnect_input(worker_t *worker, node_t *node, size_t index, value_t value) {
    node_set_value(node, index, value);
    worker_maybe_add_active_wire(worker, value);
}

static value_t
reconnect_output(node_t *node, size_t index) {
    wire_t *node_wire = wire_new();
    wire_t *free_wire = wire_new();

    node_set_value(node, index, node_wire);

    node_wire->opposite = free_wire;
    free_wire->opposite = node_wire;

    node_wire->node = node;
    node_wire->index = index;

    return free_wire;
}

void
worker_reconnect_node(worker_t *worker, node_t *node, size_t arity) {
    for (size_t count = 0; count < arity; count++) {
        size_t index = arity - 1 - count;
        value_t value = stack_pop(worker->value_stack);
        reconnect_input(worker, node, index, value);
    }

    size_t output_arity = node->ctor->arity - arity;
    for (size_t count = 0; count < output_arity; count++) {
        size_t index = arity + count;
        value_t value = reconnect_output(node, index);
        stack_push(worker->value_stack, value);
    }
}
