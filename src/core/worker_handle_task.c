#include "index.h"

static void
handle_by_primitive(worker_t *worker, task_t *task) {
    node_t *node = task->primitive_node;
    primitive_t *primitive = as_primitive(node->ctor->primitive);
    assert(primitive);

    // return input port values to the stack
    for (size_t i = 0; i < primitive->input_arity; i++) {
        value_t value = node_get_value(node, i);
        if (is_wire(value)) {
            wire_t *wire = as_wire(value);
            assert(!is_wire(wire->opposite));
            stack_push(worker->value_stack, wire->opposite);
            wire_free_from_node(wire);
            wire_destroy(&wire);
        } else {
            stack_push(worker->value_stack, value);
        }
    }

    worker_apply(worker, primitive, primitive->input_arity);

    // connect output port wires to values in the stack
    for (size_t i = 0; i < primitive->output_arity; i++) {
        size_t arity = primitive->input_arity + primitive->output_arity;
        size_t c = arity - 1 - i;
        value_t value = node_get_value(node, c);
        wire_t *wire = as_wire(value);
        value_t top_value = stack_pop(worker->value_stack);
        worker_connect(worker, wire, top_value);
    }

    worker_recycle_node(worker, task->primitive_node);
    task_destroy(&task);
}

static void
return_local_values(worker_t *worker, net_matcher_t *net_matcher) {
    array_t *local_name_array =
        net_pattern_local_name_array(net_matcher->net_pattern);

    for (size_t i = 0; i < array_length(local_name_array); i++) {
        char *name = array_get(local_name_array, i);
        value_t value = hash_get(net_matcher->value_hash, name);
        assert(value);

        if (is_wire(value)) {
            wire_t *wire = as_wire(value);
            wire_free_from_node(wire);
        }

        stack_push(worker->value_stack, value);
    }
}

static void
delete_matched_nodes(worker_t *worker, net_matcher_t *net_matcher) {
    size_t length = net_pattern_length(net_matcher->net_pattern);
    for (size_t i = 0; i < length; i++) {
        node_t *matched_node = net_matcher->matched_nodes[i];
        assert(matched_node);
        for (size_t k = 0; k < matched_node->ctor->arity; k++) {
            if (!is_wire(node_get_value(matched_node, k)))
                continue;

            value_t value = node_get_value(matched_node, k);
            if (is_wire(value)) {
                wire_t *wire = as_wire(value);
                if (wire_is_principal(wire))
                    wire_destroy(&wire);
            }
        }

        worker_recycle_node(worker, matched_node);
    }
}

static void
handle_by_rule(worker_t *worker, task_t *task) {
    return_local_values(worker, task->net_matcher);
    delete_matched_nodes(worker, task->net_matcher);

    size_t base_length = stack_length(worker->return_stack);
    frame_t *frame = frame_new(task->rule->function);

    task_destroy(&task);

    stack_push(worker->return_stack, frame);
    worker_run_until(worker, base_length);
}

void
worker_handle_task(worker_t *worker, task_t *task) {
    if (task_is_primitive(task)) {
        handle_by_primitive(worker, task);
    } else {
        handle_by_rule(worker, task);
    }
}
