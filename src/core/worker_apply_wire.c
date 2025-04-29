#include "index.h"

#define MAX_APPLY_ARITY 4

static primitive_fn_t x_apply1;
static primitive_fn_t x_apply2;
static primitive_fn_t x_apply3;
static primitive_fn_t x_apply4;

static primitive_t *apply_primitives[MAX_APPLY_ARITY];
static bool is_initialized = false;

static void
initialize_apply_primitives(void) {
    if (is_initialized) return;

    apply_primitives[0] = primitive_from_fn("apply1", 2, 1, x_apply1);
    primitive_set_node_ctor(apply_primitives[0], (const char*[]) { "target!", "arg1", "result" });

    apply_primitives[1] = primitive_from_fn("apply2", 3, 1, x_apply2);
    primitive_set_node_ctor(apply_primitives[1], (const char*[]) { "target!", "arg1", "arg2", "result" });

    apply_primitives[2] = primitive_from_fn("apply3", 4, 1, x_apply3);
    primitive_set_node_ctor(apply_primitives[2], (const char*[]) { "target!", "arg1", "arg2", "arg3", "result" });

    apply_primitives[3] = primitive_from_fn("apply4", 5, 1, x_apply4);
    primitive_set_node_ctor(apply_primitives[3], (const char*[]) { "target!", "arg1", "arg2", "arg3", "arg4", "result" });

    is_initialized = true;
}

void
worker_apply_wire(worker_t *worker, wire_t *wire, size_t arity) {
    value_t defuzed = defuze(wire);
    if (!is_wire(defuzed)) {
        worker_apply(worker, defuzed, arity);
        return;
    }

    if (!(0 < arity && arity <= 4)) {
        who_printf("arity should be from 1 to 4, arity: %lu\n", arity);
        exit(1);
    }

    initialize_apply_primitives();
    primitive_t *primitive_apply = apply_primitives[arity - 1];
    stack_tuck_n(worker->value_stack, wire, arity);
    worker_apply(worker, primitive_apply, arity + 1);
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
