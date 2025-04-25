#include "index.h"

static void
worker_handle_normal_task(worker_t *worker, task_t *task) {
    worker_disconnect_node(worker, task->left->node);
    worker_disconnect_node(worker, task->right->node);

    size_t base_length = stack_length(worker->return_stack);
    frame_t *frame = frame_new(task->rule->function);
    task_destroy(&task);
    stack_push(worker->return_stack, frame);
    worker_run_until(worker, base_length);
}

static void
worker_handle_primitive_task(worker_t *worker, task_t *task) {
    (void) worker;
    (void) task;
}

void
worker_handle_task(worker_t *worker, task_t *task) {
#if DEBUG_TASK_LOCK
    mutex_t *mutex = task->mutex;
    while (!mutex_try_lock(mutex)) {
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

    if (task_is_primitive(task)) {
        worker_handle_primitive_task(worker, task);
    } else {
        worker_handle_normal_task(worker, task);
    }

#if DEBUG_TASK_LOCK
    mutex_unlock(mutex);
    mutex_destroy(&mutex);
#endif
}
