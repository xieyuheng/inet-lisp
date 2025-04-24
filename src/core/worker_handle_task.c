#include "index.h"

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

void
worker_handle_task(worker_t *worker, task_t *task) {
    return_local_values(worker, task->net_matcher);
    delete_matched_nodes(worker, task->net_matcher);

    size_t base_length = stack_length(worker->return_stack);
    frame_t *frame = frame_new(task->rule->function);

    task_destroy(&task);

    stack_push(worker->return_stack, frame);
    worker_run_until(worker, base_length);
}
