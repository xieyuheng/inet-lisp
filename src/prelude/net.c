#include "index.h"

void
x_connect(worker_t *worker) {
    value_t second = stack_pop(worker->value_stack);
    value_t first = stack_pop(worker->value_stack);

    assert(second);
    assert(first);

    worker_connect(worker, first, second);
}

void
x_link(worker_t *worker) {
    wire_t *wire = wire_new();

    stack_push(worker->value_stack, wire);
    stack_push(worker->value_stack, wire);
}
