#include "index.h"

value_t x_eq(value_t x, value_t y) {
    return xbool(x == y);
}

void x_apply1(worker_t *worker) {
    file_lock(stdout);
    who_printf("here\n");
    file_unlock(stdout);

    value_t arg = stack_pop(worker->value_stack);
    value_t target = stack_pop(worker->value_stack);
    stack_push(worker->value_stack, arg);
    worker_apply(worker, target, 1);
}
