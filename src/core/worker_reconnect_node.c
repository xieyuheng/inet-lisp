#include "index.h"

static void
reconnect_input(node_t *node, size_t index, value_t value) {
    node_set_value(node, index, value);
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
        reconnect_input(node, index, value);
    }

    size_t output_arity = node->ctor->arity - arity;
    for (size_t count = 0; count < output_arity; count++) {
        size_t index = arity + count;
        value_t value = reconnect_output(node, index);
        stack_push(worker->value_stack, value);
    }

    maybe_return_task_by_node_and_neighbor(worker, node);
}
