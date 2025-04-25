#include "index.h"

static void
print_connected(worker_t *worker, value_t value, file_t *file) {
    node_t *node = NULL;

    if (is_wire(value)) {
        wire_t *wire = as_wire(value);
        node = worker_lookup_node_by_wire(worker, wire);
    }

    if (is_principal_wire(value)) {
        principal_wire_t *principal_wire = as_principal_wire(value);
        node = principal_wire->node;
    }

    if (!node) {
        value_print(value, file);
        fprintf(file, "\n");
        return;
    }

    hash_t *node_neighborhood_hash = build_node_neighborhood_hash(worker->node_allocator);
    fprintf(file, "<net>\n");
    fprintf(file, ":root "); node_print(node, file); fprintf(file, "\n");

    array_t *node_array = connected_node_array(node, node_neighborhood_hash);
    for (size_t i = 0; i < array_length(node_array); i++) {
        node_t *node = array_get(node_array, i);
        node_neighborhood_t *node_neighborhood = hash_get(node_neighborhood_hash, node);
        assert(node_neighborhood);
        node_neighborhood_print(node_neighborhood, file);
    }

    array_destroy(&node_array);
    hash_destroy(&node_neighborhood_hash);

    fprintf(file, "</net>\n");
    fprintf(stdout, "\n");

}

static void
print_top_until(worker_t *worker, size_t base_length, file_t *file) {
    if (DEBUG_NODE_ALLOCATOR_DISABLED) {
        who_printf("can not print when compiled with DEBUG_NODE_ALLOCATOR_DISABLED");
        exit(1);
    }

    size_t length = stack_length(worker->value_stack);
    if (length <= base_length) return;
    for (size_t count = 0; count < length - base_length; count++) {
        size_t index = length - base_length - 1 - count;
        value_t value = stack_pick(worker->value_stack, index);
        print_connected(worker, value, file);
    }
}

static void
build_net(worker_t *worker, exp_t *exp) {
    size_t arity = 0;
    function_t *function = function_new(arity);
    compile_exp(worker, function, exp);

    size_t base_length = stack_length(worker->return_stack);
    frame_t *frame = frame_new(function);
    stack_push(worker->return_stack, frame);
    worker_run_until(worker, base_length);

    function_destroy(&function);
}

void
run_exp(worker_t *worker, exp_t *exp) {
    size_t base_length = stack_length(worker->value_stack);

    build_net(worker, exp);

    if (!no_run_top_level_exp) {
        worker_work(worker);
    }

    if (print_top_level_exp_flag) {
        print_top_until(worker, base_length, stdout);
    }
}
