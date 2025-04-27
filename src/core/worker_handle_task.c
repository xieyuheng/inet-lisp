#include "index.h"

static void
worker_handle_active_pair_task(worker_t *worker, task_t *task) {
    worker_disconnect_node(worker, task->active_pair.left->node);
    worker_disconnect_node(worker, task->active_pair.right->node);

    size_t base_length = stack_length(worker->return_stack);
    frame_t *frame = frame_new(task->active_pair.rule->function);
    stack_push(worker->return_stack, frame);
    worker_run_until(worker, base_length);
}

static void
worker_handle_primitive_task(worker_t *worker, task_t *task) {
    node_t *node = task->primitive.node;
    assert(node_is_primitive(node));
    primitive_t *primitive = node->ctor->primitive;

    for (size_t count = 0; count < primitive->input_arity; count++) {
        size_t index = count;
        value_t value = node_get_value(node, index);
        assert(is_non_wire(value));
        stack_push(worker->value_stack, value);
    }

    worker_apply_primitive(worker, primitive);

    size_t arity = primitive->input_arity + primitive->output_arity;
    for (size_t count = 0; count < primitive->output_arity; count++) {
        size_t index = arity - 1 - count;
        wire_t *wire = as_wire(node_get_value(node, index));
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
        worker_handle_active_pair_task(worker, task);
        break;
    }

    case TASK_PRIMITIVE: {
        worker_handle_primitive_task(worker, task);
        break;
    }
    }

#if DEBUG_TASK_LOCK
    mutex_unlock(task->mutex);
#endif

    task_destroy(&task);
}
