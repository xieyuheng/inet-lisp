#include "index.h"

void x_print(worker_t *worker) {
    value_t x = stack_pop(worker->value_stack);
    value_print(x, stdout);
    stack_push(worker->value_stack, x);
}

void x_newline(worker_t *worker) {
    (void) worker;
    printf("\n");
}

void x_println(worker_t *worker) {
    x_print(worker);
    x_newline(worker);
}
