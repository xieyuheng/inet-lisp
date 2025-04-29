#include "index.h"

value_t x_eq(value_t x, value_t y) {
    return xbool(x == y);
}

void x_apply1(worker_t *worker) {
    value_t arg1 = stack_pop(worker->value_stack);
    value_t target = stack_pop(worker->value_stack);
    stack_push(worker->value_stack, arg1);
    worker_apply(worker, target, 1);
}

void x_apply2(worker_t *worker) {
    value_t arg2 = stack_pop(worker->value_stack);
    value_t arg1 = stack_pop(worker->value_stack);
    value_t target = stack_pop(worker->value_stack);
    stack_push(worker->value_stack, arg1);
    stack_push(worker->value_stack, arg2);
    worker_apply(worker, target, 2);
}

void x_apply3(worker_t *worker) {
    value_t arg3 = stack_pop(worker->value_stack);
    value_t arg2 = stack_pop(worker->value_stack);
    value_t arg1 = stack_pop(worker->value_stack);
    value_t target = stack_pop(worker->value_stack);
    stack_push(worker->value_stack, arg1);
    stack_push(worker->value_stack, arg2);
    stack_push(worker->value_stack, arg3);
    worker_apply(worker, target, 3);
}

void x_apply4(worker_t *worker) {
    value_t arg4 = stack_pop(worker->value_stack);
    value_t arg3 = stack_pop(worker->value_stack);
    value_t arg2 = stack_pop(worker->value_stack);
    value_t arg1 = stack_pop(worker->value_stack);
    value_t target = stack_pop(worker->value_stack);
    stack_push(worker->value_stack, arg1);
    stack_push(worker->value_stack, arg2);
    stack_push(worker->value_stack, arg3);
    stack_push(worker->value_stack, arg4);
    worker_apply(worker, target, 4);
}
