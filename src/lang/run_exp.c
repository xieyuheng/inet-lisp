#include "index.h"

static void
print_connected(worker_t *worker, value_t value) {
    node_t *node = NULL;

    if (is_wire(value)) {
        wire_t *wire = as_wire(value);
        node = worker_lookup_node_by_wire(worker, wire);
    }

    if (is_principal_wire(value)) {
        principal_wire_t *principal_wire = as_principal_wire(value);
        node = principal_wire->node;
    }

    assert(node);

    hash_t *node_neighborhood_hash = build_node_neighborhood_hash(worker->node_allocator);
    node_print_connected(node, node_neighborhood_hash, stdout);
    hash_destroy(&node_neighborhood_hash);
    fprintf(stdout, "\n");
}

static void
print_top_connected(worker_t *worker) {
    if (DEBUG_NODE_ALLOCATOR_DISABLED) {
        who_printf("can not print when compiled with DEBUG_NODE_ALLOCATOR_DISABLED");
        exit(1);
    }

    value_t value = stack_top(worker->value_stack);
    if (!value) {
        who_printf("expect top value\n");
        return;
    }

    print_connected(worker, value);
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
    build_net(worker, exp);

    if (!no_run_top_level_exp) {
        worker_work(worker);
    }

    if (print_top_level_exp_flag) {
        print_top_connected(worker);
    }
}
