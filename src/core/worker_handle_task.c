#include "index.h"

void
worker_handle_task(worker_t *worker, task_t *task) {
    worker_disconnect_node(worker, task->left->node);
    worker_disconnect_node(worker, task->right->node);

    size_t base_length = stack_length(worker->return_stack);
    frame_t *frame = frame_new(task->rule->function);
    task_destroy(&task);
    stack_push(worker->return_stack, frame);
    worker_run_until(worker, base_length);
}
