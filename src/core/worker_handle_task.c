#include "index.h"

static void
worker_handle_task_active_pair(
    worker_t *worker,
    principal_wire_t *left,
    principal_wire_t *right,
    const rule_t *rule
) {
    worker_disconnect_node(worker, left->node);
    worker_disconnect_node(worker, right->node);

    size_t return_stack_base = stack_length(worker->return_stack);
    frame_t *frame = frame_new(rule->function);
    stack_push(worker->return_stack, frame);
    worker_run_until(worker, return_stack_base);
}

static void
worker_handle_task_primitive(worker_t *worker, node_t *node) {
    assert(node_is_primitive(node));
    primitive_t *primitive = node->ctor->primitive;

    for (size_t count = 0; count < primitive->input_arity; count++) {
        size_t index = count;
        value_t value = node_get_value(node, index);
        stack_push(worker->value_stack, value);
    }

    worker_apply_primitive(worker, primitive);

    size_t arity = primitive->input_arity + primitive->output_arity;
    for (size_t count = 0; count < primitive->output_arity; count++) {
        size_t index = arity - 1 - count;
        value_t value = node_get_value(node, index);
        // output position of a primitive node ctor must be wire.
        wire_t *wire = as_wire(value);
        value_t result = stack_pop(worker->value_stack);
        fuze(wire, result);
    }

    worker_recycle_node(worker, node);
}

void
worker_handle_task(worker_t *worker, task_t *task) {
#if DEBUG_TASK_LOCK
    while (!mutex_try_lock(task->mutex)) {
        file_lock(stdout);
        who_printf("lock contention! ");
        printf("worker #%ld, ", worker->index);
        printf("task: "); task_print(task, stdout);
        printf("\n");
        file_unlock(stdout);
    }
#endif

#if DEBUG_TASK_LOG
    file_lock(stdout);
    who_printf("worker #%ld, ", worker->index);
    printf("task: "); task_print(task, stdout);
    printf("\n");
    file_unlock(stdout);
#endif

    switch (task->kind) {
    case TASK_ACTIVE_PAIR: {
        worker_handle_task_active_pair(
            worker,
            task->active_pair.left,
            task->active_pair.right,
            task->active_pair.rule);
        break;
    }

    case TASK_PRIMITIVE: {
        worker_handle_task_primitive(
            worker,
            task->primitive.node);
        break;
    }
    }

#if DEBUG_TASK_LOCK
    mutex_unlock(task->mutex);
#endif

    task_destroy(&task);
}
