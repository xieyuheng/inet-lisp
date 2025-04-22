#include "index.h"

static bool
is_directly_appliable(worker_t *worker, primitive_t *primitive, size_t arity) {
    if (primitive->input_arity != arity) return false;

    for (size_t i = 0; i < primitive->input_arity; i++) {
        if (primitive->node_ctor == NULL ||
            primitive->node_ctor->port_infos[i]->is_principal)
        {
            value_t value = stack_pick(worker->value_stack, i);
            if (is_wire(value)) return false;
        }
    }

    return true;
}

static void
apply_primitive_directly(worker_t *worker, primitive_t *primitive) {
    switch (primitive->fn_kind) {
    case PRIMITIVE_FN: {
        primitive->primitive_fn(worker);
        return;
    }

    case PRIMITIVE_FN_0: {
        value_t result = primitive->primitive_fn_0();
        stack_push(worker->value_stack, result);
        return;
    }

    case PRIMITIVE_FN_1: {
        value_t x = stack_pop(worker->value_stack);
        value_t result = primitive->primitive_fn_1(x);
        stack_push(worker->value_stack, result);
        return;
    }

    case PRIMITIVE_FN_2: {
        value_t y = stack_pop(worker->value_stack);
        value_t x = stack_pop(worker->value_stack);
        value_t result = primitive->primitive_fn_2(x, y);
        stack_push(worker->value_stack, result);
        return;
    }

    case PRIMITIVE_FN_3: {
        value_t z = stack_pop(worker->value_stack);
        value_t y = stack_pop(worker->value_stack);
        value_t x = stack_pop(worker->value_stack);
        value_t result = primitive->primitive_fn_3(x, y, z);
        stack_push(worker->value_stack, result);
        return;
    }

    case PRIMITIVE_FN_4: {
        value_t w = stack_pop(worker->value_stack);
        value_t z = stack_pop(worker->value_stack);
        value_t y = stack_pop(worker->value_stack);
        value_t x = stack_pop(worker->value_stack);
        value_t result = primitive->primitive_fn_4(x, y, z, w);
        stack_push(worker->value_stack, result);
        return;
    }
    }
}

void
worker_apply(worker_t *worker, value_t target, size_t arity) {
    if (is_node_ctor(target)) {
        node_ctor_t *node_ctor = as_node_ctor(target);
        node_t *node = worker_new_node(worker, node_ctor);
        worker_reconnect_node(worker, node, arity);
    } else if (is_function(target)) {
        function_t *function = as_function(target);
        // TODO supplement wire if arity is short
        assert(function->arity == arity);
        frame_t *frame = frame_new(function);
        stack_push(worker->return_stack, frame);
    } else if (is_primitive(target)) {
        primitive_t *primitive = as_primitive(target);
        // TODO supplement wire if arity is short
        assert(primitive->input_arity == arity);
        if (primitive->node_ctor == NULL ||
            is_directly_appliable(worker, primitive, arity)) {
            apply_primitive_directly(worker, primitive);
        } else {
            node_t *node = worker_new_node(worker, primitive->node_ctor);
            worker_reconnect_node(worker, node, arity);
        }
    } else {
        fprintf(stderr, "[apply] unknown target: ");
        value_print(target, stderr);
        exit(1);
    }
}
