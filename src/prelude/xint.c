#include "index.h"

void
xint_dup(worker_t *worker) {
    value_t target = stack_top(worker->value_stack);
    assert(is_xint(target));
    stack_push(worker->value_stack, target);
}
