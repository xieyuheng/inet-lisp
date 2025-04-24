#include "index.h"

void
x_connect(worker_t *worker) {
    value_t second = stack_pop(worker->value_stack);
    value_t first = stack_pop(worker->value_stack);

    assert(second);
    assert(first);

    task_t *task = connect(first, second);
    if (task) {
        worker_add_task(worker, task);
    }
}

void
x_link(worker_t *worker) {
    wire_t *wire = wire_new();

    stack_push(worker->value_stack, wire);
    stack_push(worker->value_stack, wire);
}

void
x_run(worker_t *worker) {
    worker_work(worker);
}

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

    hash_t *node_adjacency_hash = build_node_adjacency_hash(worker->node_allocator);
    node_print_connected(node, node_adjacency_hash, stdout);
    hash_destroy(&node_adjacency_hash);
    fprintf(stdout, "\n");
}

static void
print_top_connected(worker_t *worker) {
    value_t value = stack_top(worker->value_stack);
    if (!value) {
        who_printf("expect top value\n");
        return;
    }

    print_connected(worker, value);
}

void
x_inspect_run(worker_t *worker) {
#if DEBUG_NODE_ALLOCATOR_DISABLED
    (void) print_top_connected;
    worker_work(worker);
#else
    print_top_connected(worker);
    worker_work(worker);
    print_top_connected(worker);
#endif
}
