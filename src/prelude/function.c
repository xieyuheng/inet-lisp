#include "index.h"

void
x_fn_dup(worker_t *worker) {
    value_t target = stack_top(worker->value_stack);
    assert(is_function(target));
    stack_push(worker->value_stack, target);
}
