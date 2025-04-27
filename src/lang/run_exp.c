#include "index.h"

static void
print_connected(worker_t *worker, value_t value, file_t *file) {
    value = walk(value);

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

    const char *prefix = " ";
    hash_t *node_neighborhood_hash = build_node_neighborhood_hash(worker->node_allocator);
    fprintf(file, "(net\n");

    fprintf(file, "%s", prefix);
    fprintf(file, ":root ");
    node_print(node, file);
    fprintf(file, "\n");

    array_t *node_array = connected_node_array(node, node_neighborhood_hash);
    for (size_t i = 0; i < array_length(node_array); i++) {
        node_t *node = array_get(node_array, i);
        node_neighborhood_t *node_neighborhood = hash_get(node_neighborhood_hash, node);
        assert(node_neighborhood);
        node_neighborhood_print(node_neighborhood, file, prefix);

        if (i == array_length(node_array) - 1) {
            fprintf(file, ")");
            fprintf(stdout, "\n");
        } else {
            fprintf(stdout, "\n");
        }
    }

    array_destroy(&node_array);
    hash_destroy(&node_neighborhood_hash);
}

static void
print_top(worker_t *worker, size_t value_count, file_t *file) {
    if (DEBUG_NODE_ALLOCATOR_DISABLED) {
        who_printf("can not print when compiled with DEBUG_NODE_ALLOCATOR_DISABLED");
        exit(1);
    }

    for (size_t count = 0; count < value_count; count++) {
        size_t index = value_count - 1 - count;
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

size_t
run_exp(worker_t *worker, exp_t *exp) {
    size_t base_value_count = stack_length(worker->value_stack);

    build_net(worker, exp);

    if (!no_run_top_level_exp) {
        worker_work(worker);
    }

    assert(stack_length(worker->value_stack) >= base_value_count);
    size_t value_count = stack_length(worker->value_stack) - base_value_count;
    return value_count;
}

size_t
run_exp_and_print(worker_t *worker, exp_t *exp) {
    size_t value_count = run_exp(worker, exp);

    if (print_top_level_exp_flag) {
        print_top(worker, value_count, stdout);
    }

    return value_count;
}
